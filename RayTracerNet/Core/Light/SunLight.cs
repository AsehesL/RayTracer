using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;

namespace RayTracerNet
{
    public class SunLight : RayTracerObject
    {
        [DllImport("RayTracerLib.dll")]
        private extern static int CreateSunLight();

        [DllImport("RayTracerLib.dll", CallingConvention = CallingConvention.Cdecl)]
        private extern static void RemoveSunLight(int sunLightID);

        [DllImport("RayTracerLib.dll", CallingConvention = CallingConvention.Cdecl)]
        private extern static void SetSunLightEuler(int sunlightID, double x, double y, double z);

        [DllImport("RayTracerLib.dll", CallingConvention = CallingConvention.Cdecl)]
        private extern static void GetSunLightEuler(int sunlightID, ref double x, ref double y, ref double z);

        [DllImport("RayTracerLib.dll", CallingConvention = CallingConvention.Cdecl)]
        private extern static void SetSunLightIntensity(int sunlightID, float intensity);

        [DllImport("RayTracerLib.dll", CallingConvention = CallingConvention.Cdecl)]
        private extern static float GetSunLightIntensity(int sunlightID);

        [DllImport("RayTracerLib.dll", CallingConvention = CallingConvention.Cdecl)]
        private extern static void SetSunLightColor(int sunlightID, float r, float g, float b);

        [DllImport("RayTracerLib.dll", CallingConvention = CallingConvention.Cdecl)]
        private extern static void GetSunLightColor(int sunlightID, ref float r, ref float g, ref float b);

        [DllImport("RayTracerLib.dll", CallingConvention = CallingConvention.Cdecl)]
        private extern static void SetSunLightShadowBias(int sunlightID, float shadowBias);

        [DllImport("RayTracerLib.dll", CallingConvention = CallingConvention.Cdecl)]
        private extern static float GetSunLightShadowBias(int sunlightID);

        [VectorPropertyDisplay(VectorPropertyType.Vector3, "Rotation")]
        public Vector3 euler
        {
            get
            {
                if (objectID < 0)
                    return Vector3.zero;
                double x = 0, y = 0, z = 0;
                GetSunLightEuler(objectID, ref x, ref y, ref z);
                return new Vector3(x, y, z);

            }
            set
            {
                if (objectID < 0)
                    return;
                SetSunLightEuler(objectID, value.x, value.y, value.z);
            }
        }

        [NumberPropertyDisplay("Intensity", min = 0.0)]
        public float intensity
        {
            get
            {
                if (objectID < 0)
                    return 0;
                return GetSunLightIntensity(objectID);
            }
            set
            {
                if (objectID < 0)
                    return;
                SetSunLightIntensity(objectID, value);
            }
        }

        [ColorPropertyDisplay("Color")]
        public Color color
        {
            get
            {
                if (objectID < 0)
                    return default(Color);
                float r = 0, g = 0, b = 0;
                GetSunLightColor(objectID, ref r, ref g, ref b);
                return new Color(r, g, b);
            }
            set
            {
                if (objectID < 0)
                    return;
                SetSunLightColor(objectID, value.r, value.g, value.b);
            }
        }

        [NumberPropertyDisplay("ShadowBias", min = 0.0)]
        public float shadowBias
        {
            get
            {
                if (objectID < 0)
                    return 0;
                return GetSunLightShadowBias(objectID);
            }
            set
            {
                if (objectID < 0)
                    return;
                SetSunLightShadowBias(objectID, value);
            }
        }

        public override string GetObjectName()
        {
            return "SunLight_" + objectID;
        }

        public override string GetTypeName()
        {
            return "SunLight";
        }

        private SunLight(int sunLightID) : base(sunLightID)
        {
        }

        internal override void Destroy()
        {
            if (objectID < 0)
                return;
            MessageHandler.Broadcast<RayTracerObject>(MessageName.RemoveSceneObject, this);
            RemoveSunLight(objectID);
            objectID = -1;
        }

        internal static SunLight Create()
        {
            RayTracer instance = RayTracer.GetInstance();
            if (!instance.IsInitialized())
                return null;
            int sunLightID = CreateSunLight();
            if (sunLightID < 0)
                return null;

            SunLight sunLight = new SunLight(sunLightID);
            sunLight.euler = new Vector3(50, -30, 0);

            MessageHandler.Broadcast<RayTracerObject>(MessageName.AddNewSceneObject, sunLight);

            return sunLight;
        }
    }
}
