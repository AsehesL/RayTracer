#include "PhotonMapper.h"
#include "../../Scene/PhotonMapperScene.h"
#include "../../../Camera/Camera.h"
#include "../../Primitive/RayTracerPrimitive.h"
#include "../../Shader/RayTracerMaterialShader.h"
#include "../../Render/RayTracerRenderTarget.h"
#include "../../../RealtimeRender/RayTracingPreviewRenderer.h"
#include "../../../Scene/Scene.h"
#include <random>
#include <functional>

namespace Random
{
	thread_local std::mt19937_64 photonMapRandomDevice(std::random_device{}());
}

struct EmissionWork
{
public:
	EmissionWork(RayTracer::PrimitiveBase* light, int numEmissions, const Color& photonFlux)
	{
		this->light = light;
		this->numEmissions = numEmissions;
		this->photonFlux = photonFlux;
	}

	EmissionWork() :EmissionWork(nullptr, 0, Color()) {}

	RayTracer::PrimitiveBase* light;
	int numEmissions;
	Color photonFlux;
};

RayTracer::PhotonMapper::PhotonMapper(RayTracingPreviewRenderer* previewRenderer) : RayTracer::IntegratorBase(previewRenderer)
{
	minBounce = 3;
	maxBounce = 90;
	causticFactor = 2.0f;
	photonEmissions = 2e6;
	maxRadius = 10.f;
	nearestPhotons = 50;
	maxCausticRadius = 5.f;

	directVisualization = false;
	useShadowPhotons = false;

	m_nonCausticReject = 1.0f/causticFactor;
	m_minBounceDistance = 5.0f * maxRadius;

	m_causticPhotonMap = nullptr;
}

RayTracer::PhotonMapper::~PhotonMapper()
{
	if (m_causticPhotonMap)
		delete m_causticPhotonMap;
}

void RayTracer::PhotonMapper::Render(RayTracerScene* scene, RenderTarget* renderTarget)
{
	if (scene == nullptr || renderTarget == nullptr)
		return;
	Camera* camera = scene->camera;
	if (camera == nullptr)
		return;

	for (int i = 0; i < m_samplers.size(); i++)
	{
		SamplerBase* sampler = m_samplers[i];
		delete sampler;
	}
	m_samplers.clear();

	unsigned int width = camera->GetRenderWidth();
	unsigned int height = camera->GetRenderHeight();

	int photonEm = this->photonEmissions * causticFactor;
	m_nonCausticReject = 1.0f / causticFactor;

	for (int i = 0; i < m_threadCount; i++)
	{
		SamplerBase* sampler = CreateSampler(samplerType, 2000);
		m_samplers.push_back(sampler);
	}

	PhotonMapperScene* pmScene = (PhotonMapperScene*)scene;

	float totalFlux = 0.0f;
	for (int i = 0; i < pmScene->GetLightCounts(); ++i)
	{
		PrimitiveBase* light = pmScene->GetLight(i);
		if (light)
		{
			Color emissions = light->material->GetEmissions() * light->GetArea();
			totalFlux += emissions.r + emissions.g + emissions.b;
		}
	}

	std::vector< EmissionWork> emissionWorks;
	for (int i = 0; i < pmScene->GetLightCounts(); ++i)
	{
		PrimitiveBase* light = pmScene->GetLight(i);
		if (light)
		{
			Color lightFlux = light->material->GetEmissions() * light->GetArea();
			float photonEmissionsShare = (lightFlux.r + lightFlux.g + lightFlux.b) / totalFlux;
			int numLightEmissions = int(photonEm * photonEmissionsShare);
			Color photonFlux = lightFlux / float(numLightEmissions);

			int j = 0;
			while (j != numLightEmissions)
			{
				int emissions = j + 100000 > numLightEmissions ? numLightEmissions - j : 100000;
				emissionWorks.emplace_back(light, emissions, photonFlux);
				j+=emissions;
			}
		}
	}

	ConcurrentQueue<EmissionWork> emissionWorkQueue(emissionWorks);
	emissionWorks.clear();

	std::vector<std::unique_ptr<std::thread>> emitPhotonThreads(m_threadCount);

	m_directPhotons.resize(emitPhotonThreads.size());
	m_indirectPhotons.resize(emitPhotonThreads.size());
	m_causticPhotons.resize(emitPhotonThreads.size());
	//shadow_vecs.resize(emitPhotonThreads.size());

	for (int threadId = 0; threadId < emitPhotonThreads.size(); threadId++)
	{
		SamplerBase* sampler = m_samplers[threadId];
		emitPhotonThreads[threadId] = std::make_unique<std::thread>
			(
				[this, &emissionWorkQueue, sampler, pmScene, threadId]()
		{
			EmissionWork work;
			while (emissionWorkQueue.Pop(work))
			{
				for (int i = 0; i < work.numEmissions; i++)
				{
					Vector2 pos2D;
					sampler->Sample(pos2D);
					Vector3 pos = work.light->GetSurfacePosition(sampler, pos2D);
					Vector3 normal = work.light->GetSurfaceNormal(pos);
					Vector3 hemiSample;
					sampler->SampleHemiSphere(0, hemiSample);
					Vector3 tangent = abs(normal.x) > abs(normal.y) ? 
					(Vector3(-normal.z, 0, normal.x) / sqrt(normal.x* normal.x + normal.z* normal.z)) :
					Vector3(0, normal.z, -normal.y) / sqrt(normal.y* normal.y + normal.z* normal.z);
					Vector3 binormal = Vector3::Cross(normal, tangent);

					Vector3 dir = tangent * hemiSample.x + binormal * hemiSample.y + normal * hemiSample.z;

					EmitPhoton(Ray(pos, dir), work.photonFlux, sampler, pmScene, false, 0, threadId);
				}
			}
		}
		);
	}

	for (auto& thread : emitPhotonThreads)
	{
		thread->join();
	}

	if (m_directPhotonMap)
		delete m_directPhotonMap;
	if (m_causticPhotonMap)
		delete m_causticPhotonMap;
	if (m_indirectPhotonMap)
		delete m_indirectPhotonMap;

	m_directPhotonMap = CreatePhotonMap(m_directPhotons);
	m_causticPhotonMap = CreatePhotonMap(m_causticPhotons);
	m_indirectPhotonMap = CreatePhotonMap(m_indirectPhotons);

	/*Matrix4x4 worldToView = camera->GetWorldToViewMatrix();
	Matrix4x4 projection = camera->GetProjectionMatrix();
	Matrix4x4 pj = projection * worldToView;
	unsigned int renderWidth = renderTarget->GetWidth();
	unsigned int renderHeight = renderTarget->GetHeight();
	for (int i = 0; i < m_directPhotons.size(); i++)
	{
		for (int j = 0; j < m_directPhotons[i].size(); j++)
		{
			Color flux = m_directPhotons[i][j].flux;

			Vector4 pos = Vector4(m_directPhotons[i][j].position.x, m_directPhotons[i][j].position.y, m_directPhotons[i][j].position.z, 1.0);
			Vector4 pjpos = pj * pos;
			Vector3 scpos = Vector3(pjpos.x / pjpos.w, pjpos.y / pjpos.w, pjpos.z / pjpos.w);
			float screenX = scpos.x * 0.5f + 0.5f;
			float screenY = 1.0f - (scpos.y * 0.5f + 0.5f);

			int pixX = screenX * renderWidth;
			int pixY = screenY * renderHeight;

			renderTarget->SetPixel(pixX, pixY, flux * 10);
		}
	}*/

	int i = 0;
	int j = 0;
	std::vector<RenderTile> tiles;
	while (true)
	{
		if (i < width && j < height)
		{
			int tilewidth = TILE_SIZE;
			int tileheight = TILE_SIZE;
			if (i + tilewidth > width)
				tilewidth = width - i;
			if (j + tileheight > height)
				tileheight = height - j;
			RenderTile tile;
			tile.x = i;
			tile.y = j;
			tile.width = tilewidth;
			tile.height = tileheight;
			tiles.push_back(tile);
		}
		j += TILE_SIZE;
		if (j >= height)
		{
			j = 0;
			i += TILE_SIZE;
			if (i >= width)
				break;
		}
	}
	std::shuffle(tiles.begin(), tiles.end(), Random::photonMapRandomDevice);

	ConcurrentQueue<RenderTile> renderTiles(tiles);
	tiles.clear();

	std::function<void(PhotonMapper*, ConcurrentQueue<RenderTile>&, RenderTarget*, Camera*, SamplerBase*, PhotonMapperScene*)> photonMapperTask = &PhotonMapper::ProcessPhotonMapping;

	std::vector<std::unique_ptr<std::thread>> sampleRayThreads(m_threadCount);

	for (int i = 0; i < m_threadCount; i++)
	{
		sampleRayThreads[i] = std::make_unique<std::thread>(photonMapperTask, this, std::ref(renderTiles), renderTarget, camera, m_samplers[i], pmScene);
	}

	if (m_previewRenderer != nullptr)
	{
		std::function<void(PhotonMapper*, ConcurrentQueue<RenderTile>&, RenderTarget*)> renderTargetUpdateFunc = &PhotonMapper::UpdateRenderTarget;
		std::thread renderTargetUpdateThread(renderTargetUpdateFunc, this, std::ref(renderTiles), renderTarget);

		renderTargetUpdateThread.join();
	}

	for (auto& thread : sampleRayThreads)
	{
		thread->join();
	}
}

RayTracer::RayTracerScene* RayTracer::PhotonMapper::BuildScene(Scene* scene)
{
	return scene->BuildPhotonMapperScene();
}

void RayTracer::PhotonMapper::ProcessPhotonMapping(ConcurrentQueue<RenderTile>& tiles, RenderTarget* renderTarget, Camera* camera, SamplerBase* sampler, PhotonMapperScene* scene)
{
	RenderTile tile;
	while (tiles.Pop(tile))
	{
		for (int i = 0; i < tile.width; ++i)
		{
			for (int j = 0; j < tile.height; ++j)
			{
				int pixelX = tile.x + i;
				int pixelY = tile.y + j;

				Color color = COLOR_BLACK;
				//sampler->ResetSampler();
				//while (sampler->NextSample())
				{
					/*Vector2 sample;
					sampler->Sample(sample);
					double x = pixelX + sample.x;
					double y = pixelY + sample.y;*/

					Ray ray;
					RayTracingResult result;
					result.distance = DBL_MAX;

					camera->GetRayFromPixel(pixelX, pixelY, ray);

					Color c;
					SampleRay(ray, sampler, scene, false, 0, c);
					color += c;
					//if (scene->Tracing(ray, result))
					//	color += Color(1,1,1);
					//else
					//	color += Color(1,0,0);
				}
				color.a = 1.0f;

				//color /= sampler->GetNumSamples();

				renderTarget->SetPixel(pixelX, pixelY, color);
			}
		}
		//renderTarget->ApplyData();
	}
}

void RayTracer::PhotonMapper::UpdateRenderTarget(ConcurrentQueue<RenderTile>& tiles, RenderTarget* renderTarget)
{
	if (!m_previewRenderer)
		return;

	m_previewRenderer->UpdateRenderTarget(tiles, renderTarget);
}

void RayTracer::PhotonMapper::EmitPhoton(const Ray& ray, const Color& flux, SamplerBase* sampler, PhotonMapperScene* scene, bool specular, int depth, int threadId)
{
	if (depth >= maxBounce)
	{
		return;
	}

	RayTracingResult result;
	result.distance = DBL_MAX;

	if (scene->Tracing(ray, result))
	{
		result.depth = depth;

		PhotonMappingResult pmResult;
		
		if (result.material == nullptr)
		{
			return;
		}
		else
		{
			result.material->PhotonMapperInteract(sampler, ray.direction, result, pmResult);
		}

		Color col = pmResult.flux;
		Ray newRay = pmResult.ray;

		if (pmResult.materialType == MaterialType::Diffuse)
		{
			col *= Math::PI;
			if (depth == 0 && sampler->GetRandom() < m_nonCausticReject)
			{
				m_directPhotons[threadId].push_back(Photon(flux / m_nonCausticReject, result.hit, ray.direction));
				//CreateShadowPhotons(Ray(result.hit, ray.direction), 0, threadId);
			}
			else if (specular)
			{
				m_causticPhotons[threadId].push_back(Photon(flux, result.hit, ray.direction));
			}
			else if (sampler->GetRandom() < m_nonCausticReject)
			{
				m_indirectPhotons[threadId].push_back(Photon(flux / m_nonCausticReject, result.hit, ray.direction));
			}
		}
		else if (pmResult.materialType == MaterialType::Reflect && depth == 0 && sampler->GetRandom() < m_nonCausticReject)
		{
			//CreateShadowPhotons(Ray(result.hit, ray.direction), 0, threadId);
		}

		Color newFlux = flux * col;

		double survive = Math::Min(depth > minBounce ? 0.9f : 1.0f, Math::Max(Math::Max(newFlux.r, newFlux.g), newFlux.b) / Math::Max(Math::Max(flux.r, flux.g), flux.b));

		if (sampler->GetRandom() < survive)
		{
			EmitPhoton(newRay, newFlux / survive, sampler, scene, pmResult.specular, depth + 1, threadId);
		}

	}
}

//void RayTracer::PhotonMapper::CreateShadowPhotons(const Ray& ray, PhotonMapperScene* scene, int depth, int threadID)
//{
//	if (!useShadowPhotons || depth > maxBounce)
//	{
//		return;
//	}
//
//	RayTracingResult result;
//	result.distance = DBL_MAX;
//	if (scene->Tracing(ray, result))
//	{
//		glm::dvec3 position = ray(intersection.t);
//		if (intersection.surface->material->can_diffusely_reflect)
//		{
//			shadow_vecs[thread].emplace_back(position);
//		}
//
//		glm::dvec3 normal = intersection.surface->normal(position);
//		if (glm::dot(normal, ray.direction) > 0.0)
//		{
//			normal = -normal;
//		}
//
//		Vector3 pos(position - normal * C::EPSILON);
//		CreateShadowPhotons(Ray(pos, pos + ray.direction), thread, depth + 1);
//	}
//}

void RayTracer::PhotonMapper::SampleRay(const Ray& ray, SamplerBase* sampler, PhotonMapperScene* scene, bool specular, int depth, Color& outColor)
{
	if (depth >= maxBounce)
		return;
	RayTracingResult hitResult;
	hitResult.distance = DBL_MAX;

	if (scene->Tracing(ray, hitResult))
	{

		hitResult.depth = depth;

		PhotonMappingResult pmResult;

		if (hitResult.material == nullptr)
		{
			outColor = COLOR_BLACK;
			return;
		}
		else
		{
			hitResult.material->PhotonMapperInteract(sampler, ray.direction, hitResult, pmResult);
		}

		Color emittance = Color(0, 0, 0, 0);

		if (pmResult.materialType != MaterialType::Diffuse)
		{
			if (depth != 0 && !specular)
			{
				outColor = emittance;
				return;
			}

			Color sampleCol;
			SampleRay(pmResult.ray, sampler, scene, pmResult.specular, depth + 1, sampleCol);
			outColor = (emittance + sampleCol * pmResult.flux);
			return;
		}
		else
		{
			Color caustics = EstimateCausticRadiance(sampler, hitResult);

			if (!directVisualization && (depth == 0 || specular || hitResult.distance >= m_minBounceDistance))
			{
				outColor = EvaluateDiffuse(emittance, caustics, sampler, scene, pmResult.specular, depth + 1, pmResult);
				return;
			}
			else
			{
				std::vector<SearchResult<Photon>> indirectPhotons;
				m_indirectPhotonMap->SearchByRadius(indirectPhotons, hitResult.hit, maxRadius, nearestPhotons);
				if (indirectPhotons.size() == nearestPhotons || directVisualization)
				{
					Color direct = Color(0, 0, 0);
					std::vector<SearchResult<Photon>> directPhotons;
					m_directPhotonMap->SearchByRadius(directPhotons, hitResult.hit, maxRadius, nearestPhotons);
					if (!directPhotons.empty())
					{
						direct = EstimateRadiance(directPhotons, sampler, hitResult);
					}
					else if (!directVisualization && useShadowPhotons && !HasShadowPhotons())
					{
						indirectPhotons.clear();
						outColor = EvaluateDiffuse(emittance, caustics, sampler, scene, pmResult.specular, depth + 1, pmResult);
						return;
					}
					Color indirect = EstimateRadiance(indirectPhotons, sampler, hitResult);
					outColor = (emittance + caustics + direct + indirect);
					return;
				}
				else
				{
					indirectPhotons.clear();
					outColor = EvaluateDiffuse(emittance, caustics, sampler, scene, pmResult.specular, depth + 1, pmResult);
					return;
				}
			}
		}
	}
}

bool RayTracer::PhotonMapper::HasShadowPhotons()
{
	return false;
}

Color RayTracer::PhotonMapper::EstimateCausticRadiance(SamplerBase* sampler, RayTracingResult& hitResult)
{
	Color radiance = COLOR_BLACK;
	std::vector<SearchResult<Photon>> photons;
	m_causticPhotonMap->SearchByRadius(photons, hitResult.hit, maxCausticRadius, nearestPhotons);
	if (photons.empty()) 
		return radiance;

	double invDistance2 = 1.0 / photons.back().distance2;

	for (const auto& p : photons)
	{
		if (Vector3::Dot(p.data.direction, hitResult.normal) >= 0.0)
			continue;
		double wp = Math::Max(0.0, 1.0 - sqrt(p.distance2 * invDistance2));

		PhotonMappingResult pmResult;
		if (hitResult.material != nullptr)
			hitResult.material->PhotonMapperInteract(sampler, p.data.direction, hitResult, pmResult);

		radiance += p.data.flux * pmResult.flux * wp;
	}
	return 3.0 * radiance * invDistance2;
}

Color RayTracer::PhotonMapper::EvaluateDirect()
{
	return COLOR_BLACK;
	/*if (useShadowPhotons && HasShadowPhotons() && linear_direct_map.radiusEmpty(interaction.position, max_radius))
		return COLOR_BLACK;
	else
		return Integrator::sampleDirect(interaction);*/
}

Color RayTracer::PhotonMapper::EstimateRadiance(const std::vector<SearchResult<Photon>>& photons, SamplerBase* sampler, RayTracingResult& hitResult)
{
	Color radiance = COLOR_BLACK;
	if (photons.empty()) 
		return radiance;
	for (const auto& p : photons)
	{
		if (Vector3::Dot(p.data.direction, hitResult.normal) >= 0.0)
			continue;

		PhotonMappingResult pmResult;
		if (hitResult.material != nullptr)
			hitResult.material->PhotonMapperInteract(sampler, p.data.direction, hitResult, pmResult);

		radiance += p.data.flux * pmResult.flux;
	}
	return radiance / photons.back().distance2;
}

Color RayTracer::PhotonMapper::EvaluateDiffuse(const Color& emittance, const Color& caustics, SamplerBase* sampler, PhotonMapperScene* scene, bool specular, int depth, const PhotonMappingResult& photonMappingResult)
{
	Color col = COLOR_BLACK;
	SampleRay(photonMappingResult.ray, sampler, scene, photonMappingResult.specular, depth + 1, col);
	Color indirect = col * Math::PI;
	return (emittance + caustics + (EvaluateDirect() + indirect) * photonMappingResult.flux);
}

RayTracer::OcTree<RayTracer::Photon>* RayTracer::PhotonMapper::CreatePhotonMap(std::vector<std::vector<RayTracer::Photon>>& photons)
{
	Vector3 min = Vector3(DBL_MAX, DBL_MAX, DBL_MAX);
	Vector3 max = Vector3(DBL_MIN, DBL_MIN, DBL_MIN);
	for (int i = 0; i < photons.size(); i++)
	{
		for (int j = 0; j < photons[i].size(); j++)
		{
			Vector3 pos = photons[i][j].position;
			min = Vector3::Min(min, pos);
			max = Vector3::Max(max, pos);
		}
	}
	Vector3 size = (max - min);
	Vector3 center = (min + max) * 0.5f;
	Bounds bounds = Bounds(center, size);
	RayTracer::OcTree<RayTracer::Photon>* ocTree = new RayTracer::OcTree<RayTracer::Photon>(bounds);

	for (int i = 0; i < photons.size(); i++)
	{
		for (int j = 0; j < photons[i].size(); j++)
		{
			ocTree->Add(photons[i][j]);
		}
	}
	return ocTree;
}
