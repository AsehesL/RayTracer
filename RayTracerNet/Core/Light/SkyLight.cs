using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace RayTracerNet
{
    [AttributeUsage(AttributeTargets.Class, AllowMultiple = false, Inherited = true)]
    public class SkyLightTypeAttribute : System.Attribute
    {
        public string skyLightType;

        public SkyLightTypeAttribute(string skyLightType)
        {
            this.skyLightType = skyLightType;
        }
    }

    public abstract class SkyLight : RayTracerObject
    {

        [DllImport("RayTracerLib.dll", CallingConvention = CallingConvention.Cdecl)]
        private extern static void RemoveSkyLight(int skyLightID);

        protected SkyLight(int objectID) : base(objectID)
        { }

        public override void Destroy()
        {
            if (objectID < 0)
                return;
            MessageHandler.Broadcast<RayTracerObject>(MessageName.RemoveSceneObject, this);
            RemoveSkyLight(objectID);
            objectID = -1;
        }
    }

    [SkyLightType("Environment")]
    public class EnvironmentMapSkyLight : SkyLight
    {
        [DllImport("RayTracerLib.dll")]
        private extern static int CreateEnvironmentMapSkyLight();

        [DllImport("RayTracerLib.dll", CallingConvention = CallingConvention.Cdecl)]
        private extern static void SetEnvironmentMapSkyLightTexture(int skylightID, int textureID);

        [DllImport("RayTracerLib.dll", CallingConvention = CallingConvention.Cdecl)]
        private extern static void SetEnvironmentMapSkyLightColor(int skylightID, float r, float g, float b, float a);

        [DllImport("RayTracerLib.dll", CallingConvention = CallingConvention.Cdecl)]
        private extern static void SetEnvironmentMapSkyLightIntensity(int skylightID, float intensity);

        protected EnvironmentMapSkyLight(int objectID) : base(objectID)
        { }

        private Texture m_environmentTexture;

        private Color m_environmentColor = Color.white;
        private float m_environmentIntensity = 1.0f;

        [ResourcePropertyDisplay("EnvironmentMap")]
        public Texture environment
        {
            get
            {
                return m_environmentTexture;
            }
            set 
            {
                m_environmentTexture = value;
                if (value != null)
                    SetEnvironmentMapSkyLightTexture(objectID, value.objectID);
                else
                    SetEnvironmentMapSkyLightTexture(objectID, -1);
            }
        }

        [ColorPropertyDisplay("Color")]
        public Color color
        {
            get
            {
                return m_environmentColor;
            }
            set
            {
                m_environmentColor = value;
                SetEnvironmentMapSkyLightColor(objectID, value.r, value.g, value.b, value.b);
            }
        }

        [NumberPropertyDisplay("Intensity")]
        public float intensity
        {
            get
            {
                return m_environmentIntensity;
            }
            set
            {
                m_environmentIntensity = value;
                SetEnvironmentMapSkyLightIntensity(objectID, value);
            }
        }

        public override string GetObjectName()
        {
            return "EnvironmentMapSkyLight_" + objectID;
        }

        public override string GetTypeName()
        {
            return "EnvironmentMapSkyLight";
        }

        public static EnvironmentMapSkyLight Create()
        {
            RayTracer instance = RayTracer.GetInstance();
            if (!instance.IsInitialized())
                return null;
            int skyLightID = CreateEnvironmentMapSkyLight();
            if (skyLightID < 0)
                return null;

            EnvironmentMapSkyLight skylight = new EnvironmentMapSkyLight(skyLightID);

            MessageHandler.Broadcast<RayTracerObject>(MessageName.AddNewSceneObject, skylight);

            return skylight;
        }
    }
}
