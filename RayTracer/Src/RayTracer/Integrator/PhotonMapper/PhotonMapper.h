#pragma once
#include "../IntegratorBase.h"
#include "../../../Core/ConcurrentQueue.hpp"
#include "../../Render/RayTracerRenderTile.h"
#include "../../RayTracingResult.h"
#include "../../../Core/Color.h"
#include "../../../Core/Ray.h"
#include "../../Shader/RayTracerMaterialShader.h"
#include "Photon.h"
#include "OcTree.hpp"

class RayTracingPreviewRenderer;
class Camera;

namespace RayTracer
{
	class SamplerBase;
	class PhotonMapperScene;

	class PhotonMapper : public IntegratorBase
	{
	public:
		PhotonMapper(RayTracingPreviewRenderer* previewRenderer);
		virtual ~PhotonMapper() override;

		virtual void Render(class RayTracerScene* scene, RenderTarget* renderTarget) override;
		virtual RayTracerScene* BuildScene(class Scene* scene) override;

	private:
		void ProcessPhotonMapping(ConcurrentQueue<RenderTile>& tiles, RenderTarget* renderTarget, Camera* camera, SamplerBase* sampler, PhotonMapperScene* scene);

		void UpdateRenderTarget(ConcurrentQueue<RenderTile>& tiles, RenderTarget* renderTarget);

		void EmitPhoton(const Ray& ray, const Color& flux, SamplerBase* sampler, PhotonMapperScene* scene, bool specular, int depth, int threadID);

		//void CreateShadowPhotons(const Ray& ray, PhotonMapperScene* scene, int depth, int threadID);

		void SampleRay(const Ray& ray, SamplerBase* sampler, PhotonMapperScene* scene, bool specular, int depth, Color& outColor);

		bool HasShadowPhotons();

		Color EstimateCausticRadiance(SamplerBase* sampler, RayTracingResult& hitResult);
		Color EvaluateDirect();
		Color EstimateRadiance(const std::vector<SearchResult<Photon>>& photons, SamplerBase* sampler, RayTracingResult& hitResult);
		Color EvaluateDiffuse(const Color& emittance, const Color& caustics, SamplerBase* sampler, PhotonMapperScene* scene, bool specular, int depth, const PhotonMappingResult& photonMappingResult);

		OcTree<Photon>* CreatePhotonMap(std::vector<std::vector<Photon>>& photons);
	public:
		float photonEmissions;
		float causticFactor;
		int minBounce;
		int maxBounce;
		float maxRadius;
		bool useShadowPhotons;
		bool directVisualization;
		int nearestPhotons;
		float maxCausticRadius;

	private:
		float m_nonCausticReject;
		float m_minBounceDistance;
		std::vector<class SamplerBase*> m_samplers;

		std::vector<std::vector<Photon>> m_causticPhotons;
		std::vector<std::vector<Photon>> m_directPhotons;
		std::vector<std::vector<Photon>> m_indirectPhotons;
		//std::vector<std::vector<ShadowPhoton>> shadow_vecs;

		OcTree<Photon>* m_causticPhotonMap;
		OcTree<Photon>* m_directPhotonMap;
		OcTree<Photon>* m_indirectPhotonMap;
	};
}