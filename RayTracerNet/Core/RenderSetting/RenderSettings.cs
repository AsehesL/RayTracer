using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace RayTracerNet
{
    public enum IntegratorType
    {
        PathTracer,
        PhotonMapper,
        Debug,
    }

    public enum SamplerType
    {
        Random,
        Jittered,
        Hammersley,
        Regular,
    }

    public class RenderSettings
    {
        [EnumPropertyDisplay("渲染器")]
        public IntegratorType integratorType
        {
            get
            {
                return m_IntegratorType;
            }
            set
            {
                if (m_IntegratorType != value)
                {
                    SetIntegrateType((int)value);
                    MessageHandler.Broadcast<IntegratorType>(MessageName.ChangeIntegratorType, value);
                }
            }
        }

        [NumberPropertyDisplay("渲染宽度", min = 1)]
        public int renderWidth
        {
            get
            {
                return (int)GetRenderWidth();
            }
            set
            {
                SetRenderWidth(value);
            }
        }

        [NumberPropertyDisplay("渲染高度", min = 1)]
        public int renderHeight
        {
            get
            {
                return (int)GetRenderHeight();
            }
            set
            {
                SetRenderHeight(value);
            }
        }

        [DllImport("RayTracerLib.dll", CallingConvention = CallingConvention.Cdecl)]
        private extern static void SetRenderWidth(int width);

        [DllImport("RayTracerLib.dll", CallingConvention = CallingConvention.Cdecl)]
        private extern static void SetRenderHeight(int height);

        [DllImport("RayTracerLib.dll")]
        private extern static uint GetRenderWidth();

        [DllImport("RayTracerLib.dll")]
        private extern static uint GetRenderHeight();

        [DllImport("RayTracerLib.dll", CallingConvention = CallingConvention.Cdecl)]
        private extern static void SetIntegrateType(int type);

        protected IntegratorType m_IntegratorType;

    }

    public class PathTracerRenderSettings : RenderSettings
    {
        public PathTracerRenderSettings()
        {
            m_IntegratorType = IntegratorType.PathTracer;
        }

        [NumberPropertyDisplay("反弹次数", min = 0)]
        public int bounce
        {
            get
            {
                return GetPathTracerBounce();
            }
            set
            {
                SetPathTracerBounce(value);
            }
        }

        [NumberPropertyDisplay("采样次数", min = 0)]
        public int sampleNums
        {
            get
            {
                return GetPathTracerSampleNums();
            }
            set
            {
                SetPathTracerSampleNums(value);
            }
        }

        [EnumPropertyDisplay("采样器")]
        public SamplerType sampler
        {
            get
            {
                return (SamplerType)GetPathTracerSampler();
            }
            set
            {
                SetPathTracerSampler((int)value);
            }
        }

        [DllImport("RayTracerLib.dll", CallingConvention = CallingConvention.Cdecl)]
        private extern static void SetPathTracerBounce(int bounce);

        [DllImport("RayTracerLib.dll")]
        private extern static int GetPathTracerBounce();

        [DllImport("RayTracerLib.dll", CallingConvention = CallingConvention.Cdecl)]
        private extern static void SetPathTracerSampleNums(int sampleNums);

        [DllImport("RayTracerLib.dll")]
        private extern static int GetPathTracerSampleNums();

        [DllImport("RayTracerLib.dll", CallingConvention = CallingConvention.Cdecl)]
        private extern static void SetPathTracerSampler(int sampler);

        [DllImport("RayTracerLib.dll")]
        private extern static int GetPathTracerSampler();
    }

    public class PhotonMapperRenderSettings : RenderSettings
    {
        public PhotonMapperRenderSettings()
        {
            m_IntegratorType = IntegratorType.PhotonMapper;
        }
    }

    public class DebugRendererRenderSettings : RenderSettings
    {
        public DebugRendererRenderSettings()
        {
            m_IntegratorType = IntegratorType.Debug;
        }
    }
}
