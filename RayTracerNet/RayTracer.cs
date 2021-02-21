using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;

namespace RayTracerNet
{
    public class RayTracer
    {
        private static RayTracer instance;

        private bool m_isInitialized;

        private PathTracerRenderSettings m_pathTracerRenderSettings;
        private PhotonMapperRenderSettings m_photonMapperRenderSettings;
        private DebugRendererRenderSettings m_debugRendererRenderSettings;

        private Scene m_scene;

        public static RayTracer GetInstance()
        {
            if (instance == null)
                instance = new RayTracer();
            return instance;
        }

        public RenderSettings renderSettings
        {
            get
            {
                IntegratorType type = (IntegratorType)GetIntegrateType();
                if (type == IntegratorType.PathTracer)
                {
                    if (m_pathTracerRenderSettings == null)
                        m_pathTracerRenderSettings = new PathTracerRenderSettings();
                    return m_pathTracerRenderSettings;
                }
                else if (type == IntegratorType.PhotonMapper)
                {
                    if (m_photonMapperRenderSettings == null)
                        m_photonMapperRenderSettings = new PhotonMapperRenderSettings();
                    return m_photonMapperRenderSettings;
                }
                else if(type == IntegratorType.Debug)
                {
                    if (m_debugRendererRenderSettings == null)
                        m_debugRendererRenderSettings = new DebugRendererRenderSettings();
                    return m_debugRendererRenderSettings;
                }
                return null;
            }
        }

        private RayTracer()
        {
            m_isInitialized = false;
        }

        [DllImport("RayTracerLib.dll", CallingConvention = CallingConvention.Cdecl)]
        private extern static bool Init(IntPtr hwnd, int width, int height);

        [DllImport("RayTracerLib.dll")]
        private extern static void UpdateRenderer();

        [DllImport("RayTracerLib.dll")]
        private extern static void Destroy();

        [DllImport("RayTracerLib.dll")]
        private extern static int GetIntegrateType();

        [DllImport("RayTracerLib.dll")]
        private extern static bool StartRayTracing();

        [DllImport("RayTracerLib.dll")]
        private extern static bool HasResult();

        [DllImport("RayTracerLib.dll", CallingConvention = CallingConvention.Cdecl)]
        private extern static void SaveResult(string path, bool isHDR);

        public bool InitRayTracer(IntPtr hwnd, int width, int height)
        {
            if (m_isInitialized)
                return m_isInitialized;
            m_isInitialized = Init(hwnd, width, height);
            
            MessageHandler.AddListener<ResourceObject>(MessageName.AddNewResource, this.OnAddNewResource);
            MessageHandler.AddListener<RayTracerObject>(MessageName.AddNewSceneObject, this.OnAddNewSceneObject);
            MessageHandler.AddListener<RayTracerObject>(MessageName.RemoveSceneObject, this.OnRemoveNewSceneObject);
            MessageHandler.AddListener<ResourceObject>(MessageName.RemoveResource, this.OnRemoveResource);
            if (m_isInitialized)
            {
                m_scene = new Scene();
                m_scene.Create();
            }
            return m_isInitialized;
        }

        public bool IsInitialized()
        {
            return m_isInitialized;
        }

        public void UpdateRayTracerRenderer()
        {
            if (IsInitialized())
                UpdateRenderer();
        }

        public void DestroyRayTracer()
        {
            if (IsInitialized())
                Destroy();
            m_isInitialized = false;
        }

        public bool StartRayTracingRender()
        {
            if (IsInitialized())
                return StartRayTracing();
            return false;
        }

        public bool HasRayTracingResult()
        {
            if (IsInitialized())
                return HasResult();
            return false;
        }

        public void SaveRayTracingResult(string path, bool hdr)
        {
            if (IsInitialized())
            {
                SaveResult(path, hdr);
            }
        }

        public Scene GetScene()
        {
            return m_scene;
        }

        private void OnAddNewResource(ResourceObject resource)
        {
            if (m_scene != null)
                m_scene.AddResourceObject(resource);
        }

        private void OnAddNewSceneObject(RayTracerObject obj)
        {
            if (m_scene != null)
                m_scene.AddSceneObject(obj);
        }

        private void OnRemoveNewSceneObject(RayTracerObject obj)
        {
            if (m_scene != null)
                m_scene.RemoveSceneObject(obj);
        }

        private void OnRemoveResource(ResourceObject resource)
        {
            if (m_scene != null)
                m_scene.RemoveResourceObject(resource);
        }
    }
}
