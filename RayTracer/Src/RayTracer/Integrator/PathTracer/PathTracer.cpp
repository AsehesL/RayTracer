#include "PathTracer.h"
#include "../../RayTracerScene.h"
#include "../../../Camera/Camera.h"
#include "../../Render/RayTracerRenderTile.h"
#include "../../Render/RayTracerRenderTarget.h"
#include "../../../Random/Random.hpp"
#include "../../../Core/ConcurrentQueue.hpp"
#include "../../Sampler/Sampler.h"
#include "../../../RealtimeRender/RayTracingPreviewRenderer.h"
#include "../../Shader/RayTracerMaterialShader.h"
#include "../../Light/RayTracerSkyLight.h"
#include <vector>
#include <thread>
#include <functional>

RayTracer::PathTracer::PathTracer(RayTracingPreviewRenderer* previewRenderer) : RayTracer::IntegratorBase(previewRenderer)
{
	bounce = 0;
	sampleNums = 0;
}

RayTracer::PathTracer::~PathTracer()
{
	for (int i = 0; i < m_samplers.size(); i++)
	{
		SamplerBase* sampler = m_samplers[i];
		delete sampler;
	}
}

void RayTracer::PathTracer::Render(RayTracerScene* scene, RayTracer::RenderTarget* renderTarget)
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
	for (int i = 0; i < m_threadCount; i++)
	{
		SamplerBase* sampler = CreateSampler(samplerType, sampleNums);
		m_samplers.push_back(sampler);
	}

	unsigned int width = camera->GetRenderWidth();
	unsigned int height = camera->GetRenderHeight();

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
		j+= TILE_SIZE;
		if (j >= height)
		{
			j = 0;
			i+= TILE_SIZE;
			if (i >= width)
				break;
		}
	}
	std::shuffle(tiles.begin(), tiles.end(), Random::randomDevice);

	ConcurrentQueue<RenderTile> renderTiles(tiles);
	tiles.clear();

	std::function<void(PathTracer*, ConcurrentQueue<RenderTile>&, RenderTarget*, Camera*, SamplerBase*, RayTracerScene*)> pathTracingTask = &PathTracer::ProcessPathTracing;

	std::vector<std::unique_ptr<std::thread>> threads(m_threadCount);
	//for (auto& thread : threads)
	for (int i=0;i<m_threadCount;i++)
	{
		threads[i] = std::make_unique<std::thread>(pathTracingTask, this, std::ref(renderTiles), renderTarget, camera, m_samplers[i], scene);
		//thread = std::make_unique<std::thread>(pathTracingTask, this, std::ref(renderTiles), nullptr, nullptr, nullptr);
	}

	if (m_previewRenderer != nullptr)
	{ 
		std::function<void(PathTracer*, ConcurrentQueue<RenderTile>&, RenderTarget*)> renderTargetUpdateFunc = &PathTracer::UpdateRenderTarget;
		std::thread renderTargetUpdateThread(renderTargetUpdateFunc, this, std::ref(renderTiles), renderTarget);

		renderTargetUpdateThread.join();
	}

	for (auto& thread : threads)
	{
		thread->join();
	}
}

void RayTracer::PathTracer::ProcessPathTracing(ConcurrentQueue<RenderTile>& tiles, RayTracer::RenderTarget* renderTarget, Camera* camera, SamplerBase* sampler, RayTracerScene* scene)
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
				sampler->ResetSampler();
				while (sampler->NextSample())
				{
					/*Vector2 sample;
					sampler->Sample(sample);
					double x = pixelX + sample.x;
					double y = pixelY + sample.y;*/

					Ray ray;
					RayTracingResult result;
					result.distance = DBL_MAX;

					camera->GetRayFromPixel(pixelX, pixelY, sampler, ray);

					Color c;
					Tracing(ray, sampler, scene, 0, c);
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

void RayTracer::PathTracer::UpdateRenderTarget(ConcurrentQueue<RenderTile>& tiles, RenderTarget* renderTarget)
{
	if (!m_previewRenderer)
		return;

	m_previewRenderer->UpdateRenderTarget(tiles, renderTarget);
}

void RayTracer::PathTracer::Tracing(const Ray& ray, SamplerBase* sampler, RayTracerScene* scene, int depth, Color& outColor)
{
	if (depth > bounce)
	{
		outColor = COLOR_BLACK;
		return;
	}

	RayTracingResult result;
	result.distance = DBL_MAX;
	Color color = Color(0, 0, 0, 1);

	bool isHit = false;
	if (scene->Tracing(ray, result))
	{
		result.depth = depth;
		//if (renderChannel == RenderChannel.Full)
		{
			if (result.material == nullptr)
				color += Color(1, 0, 1);
			else
				color += result.material->PathTracing(this, sampler, scene, ray, result);
		}

		isHit = true;
	}

	//if (!isHit || (isHit && depth == 0 && scene->GetSkyLight() != nullptr && scene->GetSkyLight()->shouldRenderParticipatingMedia))
	if (!isHit)
	{
		if (scene->GetSkyLight() != nullptr)
			color += scene->GetSkyLight()->PathTracing(this, sampler, ray);
		else
			color += COLOR_BLACK;
	}

	if (depth == 0)
		color /= sampler->GetNumSamples();

	outColor = color;
}

