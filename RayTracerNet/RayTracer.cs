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

        public List<ResourceObject> resourceList;

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

        public bool InitRayTracer(IntPtr hwnd, int width, int height)
        {
            if (m_isInitialized)
                return m_isInitialized;
            m_isInitialized = Init(hwnd, width, height);
            if (m_isInitialized)
            {
                m_scene = new Scene();
            }
            MessageHandler.AddListener<ResourceObject>(MessageName.AddNewResource, this.OnAddNewResource);
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
            return StartRayTracing();
        }

        public Scene GetScene()
        {
            return m_scene;
        }

        private void OnAddNewResource(ResourceObject resource)
        {
            if (resourceList == null)
                resourceList = new List<ResourceObject>();
            resourceList.Add(resource);
        }
    }
}
