using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;

namespace RayTracerNet
{
    [AttributeUsage(AttributeTargets.Class, AllowMultiple = false, Inherited = true)]
    public class ShaderTypeAttribute : System.Attribute
    {
        public string shaderType;

        public ShaderTypeAttribute(string shaderType)
        {
            this.shaderType = shaderType;
        }
    }

    public abstract class MaterialShader : ResourceObject
    {
        public override string GetFullPathName()
        {
            return null;
        }

        protected MaterialShader(int shaderID) : base(shaderID)
        {
        }
    }

    [ShaderType("Emissive")]
    public class EmissiveShader : MaterialShader
    {
        [DllImport("RayTracerLib.dll")]
        private extern static int CreateEmissiveShader();

        [DllImport("RayTracerLib.dll", CallingConvention = CallingConvention.Cdecl)]
        private extern static void GetEmissiveShaderColor(int shaderID, ref float r, ref float g, ref float b, ref float a);

        [DllImport("RayTracerLib.dll", CallingConvention = CallingConvention.Cdecl)]
        private extern static void SetEmissiveShaderColor(int shaderID, float r, float g, float b, float a);

        [DllImport("RayTracerLib.dll", CallingConvention = CallingConvention.Cdecl)]
        private extern static float GetEmissiveShaderIntensity(int shaderID);

        [DllImport("RayTracerLib.dll", CallingConvention = CallingConvention.Cdecl)]
        private extern static void SetEmissiveShaderIntensity(int shaderID, float intensity);

        protected EmissiveShader(int shaderID) : base(shaderID)
        { }

        [ColorPropertyDisplay("Color")]
        public Color color
        {
            get
            {
                if (objectID < 0)
                    return default(Color);
                float r = 0, g = 0, b = 0, a = 0;
                GetEmissiveShaderColor(objectID, ref r, ref g, ref b, ref a);
                return new Color(r, g, b, a);
            }
            set
            {
                if (objectID < 0)
                    return;
                SetEmissiveShaderColor(objectID, value.r, value.g, value.b, value.a);
            }
        }

        [NumberPropertyDisplay("Intensity", min = 0.0)]
        public float intensity
        {
            get
            {
                if (objectID < 0)
                    return 0;
                return GetEmissiveShaderIntensity(objectID);
            }
            set
            {
                if (objectID < 0)
                    return;
                SetEmissiveShaderIntensity(objectID, value);
            }
        }

        public static EmissiveShader Create()
        {
            RayTracer instance = RayTracer.GetInstance();
            if (!instance.IsInitialized())
                return null;
            int shaderID = CreateEmissiveShader();
            if (shaderID >= 0)
            {
                var shader = new EmissiveShader(shaderID);
                MessageHandler.Broadcast<ResourceObject>(MessageName.AddNewResource, shader);
                return shader;
            }
            return null;
        }

        public override string GetObjectName()
        {
            return "自发光材质_" + objectID;
        }

        public override string GetTypeName()
        {
            return "自发光材质";
        }

        public override string ToString()
        {
            return "自发光材质_" + objectID;
        }
    }

    [ShaderType("Transparency")]
    public class TransparencyShader : MaterialShader
    {
        [DllImport("RayTracerLib.dll")]
        private extern static int CreateTransparencyShader();

        [DllImport("RayTracerLib.dll", CallingConvention = CallingConvention.Cdecl)]
        private extern static void GetTransparencyShaderColor(int shaderID, ref float r, ref float g, ref float b, ref float a);

        [DllImport("RayTracerLib.dll", CallingConvention = CallingConvention.Cdecl)]
        private extern static void SetTransparencyShaderColor(int shaderID, float r, float g, float b, float a);

        protected TransparencyShader(int shaderID) : base(shaderID)
        { }

        [ColorPropertyDisplay("Color")]
        public Color color
        {
            get
            {
                if (objectID < 0)
                    return default(Color);
                float r = 0, g = 0, b = 0, a = 0;
                GetTransparencyShaderColor(objectID, ref r, ref g, ref b, ref a);
                return new Color(r, g, b, a);
            }
            set
            {
                if (objectID < 0)
                    return;
                SetTransparencyShaderColor(objectID, value.r, value.g, value.b, value.a);
            }
        }

        public static TransparencyShader Create()
        {
            RayTracer instance = RayTracer.GetInstance();
            if (!instance.IsInitialized())
                return null;
            int shaderID = CreateTransparencyShader();
            if (shaderID >= 0)
            {
                var shader = new TransparencyShader(shaderID);
                MessageHandler.Broadcast<ResourceObject>(MessageName.AddNewResource, shader);
                return shader;
            }
            return null;
        }

        public override string GetObjectName()
        {
            return "透明材质_" + objectID;
        }

        public override string GetTypeName()
        {
            return "透明材质";
        }

        public override string ToString()
        {
            return "透明材质_" + objectID;
        }
    }

    public abstract class PBRShaderBase : MaterialShader
    {
        protected PBRShaderBase(int shaderID) : base(shaderID)
        {
        }

        [ColorPropertyDisplay("AlbedoColor")]
        public Color albedoColor
        {
            get
            {
                float r = 0, g = 0, b = 0, a = 0;
                GetPBRShaderAlbedoColor(objectID, ref r, ref g, ref b, ref a);
                return new Color(r, g, b, a);
            }
            set
            {
                SetPBRShaderAlbedoColor(objectID, value.r, value.g, value.b, value.a);
            }
        }

        private Texture m_albedoTexture;

        [ResourcePropertyDisplay("Albedo")]
        public Texture albedoTexture
        {
            get
            {
                return m_albedoTexture;
            }
            set
            {
                m_albedoTexture = value;
                if (value == null)
                    SetPBRShaderAlbedoTexture(objectID, -1);
                else
                    SetPBRShaderAlbedoTexture(objectID, value.objectID);
            }
        }

        [NumberPropertyDisplay("Roughness", min = 0.01, max = 1.0)]
        public float roughness
        {
            get
            {
                return GetPBRShaderRoughness(objectID);
            }
            set
            {
                SetPBRShaderRoughness(objectID, value);
            }
        }

        [NumberPropertyDisplay("Metallic", min = 0.0, max = 1.0)]
        public float metallic
        {
            get
            {
                return GetPBRShaderMetallic(objectID);
            }
            set
            {
                SetPBRShaderMetallic(objectID, value);
            }
        }

        [DllImport("RayTracerLib.dll", CallingConvention = CallingConvention.Cdecl)]
        private extern static void GetPBRShaderAlbedoColor(int shaderID, ref float r, ref float g, ref float b, ref float a);

        [DllImport("RayTracerLib.dll", CallingConvention = CallingConvention.Cdecl)]
        private extern static void SetPBRShaderAlbedoColor(int shaderID, float r, float g, float b, float a);

        [DllImport("RayTracerLib.dll", CallingConvention = CallingConvention.Cdecl)]
        private extern static void SetPBRShaderAlbedoTexture(int shaderID, int textureID);

        [DllImport("RayTracerLib.dll", CallingConvention = CallingConvention.Cdecl)]
        private extern static float GetPBRShaderRoughness(int shaderID);

        [DllImport("RayTracerLib.dll", CallingConvention = CallingConvention.Cdecl)]
        private extern static void SetPBRShaderRoughness(int shaderID, float roughness);

        [DllImport("RayTracerLib.dll", CallingConvention = CallingConvention.Cdecl)]
        private extern static float GetPBRShaderMetallic(int shaderID);

        [DllImport("RayTracerLib.dll", CallingConvention = CallingConvention.Cdecl)]
        private extern static void SetPBRShaderMetallic(int shaderID, float metallic);
    }

    [ShaderType("PBR")]
    public class PBR_StandardShader : PBRShaderBase
    {
        protected PBR_StandardShader(int shaderID) : base(shaderID)
        {
        }

        [DllImport("RayTracerLib.dll")]
        private extern static int CreatePBRStandardShader();

        public static PBR_StandardShader Create()
        {
            RayTracer instance = RayTracer.GetInstance();
            if (!instance.IsInitialized())
                return null;
            int shaderID = CreatePBRStandardShader();
            if (shaderID >= 0) 
            {
                var shader = new PBR_StandardShader(shaderID);
                MessageHandler.Broadcast<ResourceObject>(MessageName.AddNewResource, shader);
                return shader;
            }
            return null;
        }

        public override string GetObjectName()
        {
            return "标准PBR材质_" + objectID;
        }

        public override string GetTypeName()
        {
            return "标准PBR材质";
        }

        public override string ToString()
        {
            return "标准PBR材质_" + objectID;
        }
    }
}
