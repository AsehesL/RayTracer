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
        private Dictionary<string, Texture> m_textures = null;

        public override string GetFullPathName()
        {
            return null;
        }

        protected MaterialShader(int shaderID) : base(shaderID)
        {
            m_textures = new Dictionary<string, Texture>();
        }

        public void SetColor(string key, Color color)
        {
            SetShaderColor(objectID, key, color.r, color.g, color.b, color.a);
        }

        public Color GetColor(string key)
        {
            float r = 0, g = 0, b = 0, a = 0;
            GetShaderColor(objectID, key, ref r, ref g, ref b, ref a);
            return new Color(r, g, b, a);
        }

        public void SetFloat(string key, float number)
        {
            SetShaderFloat(objectID, key, number);
        }

        public float GetFloat(string key)
        {
            return GetShaderFloat(objectID, key);
        }

        public void SetVector2(string key, Vector2 vector)
        {
            SetShaderVector2(objectID, key, vector.x, vector.y);
        }

        public Vector2 GetVector2(string key)
        {
            double x = 0, y = 0;
            GetShaderVector2(objectID, key, ref x, ref y);
            return new Vector2(x, y);
        }

        public void SetVector3(string key, Vector3 vector)
        {
            SetShaderVector3(objectID, key, vector.x, vector.y, vector.z);
        }

        public Vector3 GetVector3(string key)
        {
            double x = 0, y = 0, z = 0;
            GetShaderVector3(objectID, key, ref x, ref y, ref z);
            return new Vector3(x, y, z);
        }

        public void SetVector4(string key, Vector4 vector)
        {
            SetShaderVector4(objectID, key, vector.x, vector.y, vector.z, vector.w);
        }

        public Vector4 GetVector4(string key)
        {
            double x = 0, y = 0, z = 0, w = 0;
            GetShaderVector4(objectID, key, ref x, ref y, ref z, ref w);
            return new Vector4(x, y, z, w);
        }

        public void SetTexture(string key, Texture texture)
        {
            m_textures[key] = texture;
            if (texture == null)
                SetShaderTexture(objectID, key, -1);
            else
                SetShaderTexture(objectID, key, texture.objectID);
        }

        public Texture GetTexture(string key)
        {
            return m_textures.ContainsKey(key) ? m_textures[key] : null;
        }


        [DllImport("RayTracerLib.dll", CallingConvention = CallingConvention.Cdecl)]
        private extern static void GetShaderColor(int shaderID, string key, ref float r, ref float g, ref float b, ref float a);

        [DllImport("RayTracerLib.dll", CallingConvention = CallingConvention.Cdecl)]
        private extern static void SetShaderColor(int shaderID, string key, float r, float g, float b, float a);

        [DllImport("RayTracerLib.dll", CallingConvention = CallingConvention.Cdecl)]
        private extern static float GetShaderFloat(int shaderID, string key);

        [DllImport("RayTracerLib.dll", CallingConvention = CallingConvention.Cdecl)]
        private extern static void SetShaderFloat(int shaderID, string key, float v);

        [DllImport("RayTracerLib.dll", CallingConvention = CallingConvention.Cdecl)]
        private extern static void SetShaderTexture(int shaderID, string key, int textureID);

        [DllImport("RayTracerLib.dll", CallingConvention = CallingConvention.Cdecl)]
        private extern static void GetShaderVector2(int shaderID, string key, ref double x, ref double y);

        [DllImport("RayTracerLib.dll", CallingConvention = CallingConvention.Cdecl)]
        private extern static void SetShaderVector2(int shaderID, string key, double x, double y);

        [DllImport("RayTracerLib.dll", CallingConvention = CallingConvention.Cdecl)]
        private extern static void GetShaderVector3(int shaderID, string key, ref double x, ref double y, ref double z);

        [DllImport("RayTracerLib.dll", CallingConvention = CallingConvention.Cdecl)]
        private extern static void SetShaderVector3(int shaderID, string key, double x, double y, double z);

        [DllImport("RayTracerLib.dll", CallingConvention = CallingConvention.Cdecl)]
        private extern static void GetShaderVector4(int shaderID, string key, ref double x, ref double y, ref double z, ref double w);

        [DllImport("RayTracerLib.dll", CallingConvention = CallingConvention.Cdecl)]
        private extern static void SetShaderVector4(int shaderID, string key, double x, double y, double z, double w);
    }

    [ShaderType("Emissive")]
    public class EmissiveShader : MaterialShader
    {
        [DllImport("RayTracerLib.dll")]
        private extern static int CreateEmissiveShader();

        protected EmissiveShader(int shaderID) : base(shaderID)
        { }

        [ColorPropertyDisplay("Color")]
        public Color color
        {
            get
            {
                if (objectID < 0)
                    return default(Color);
                return GetColor("color");
            }
            set
            {
                if (objectID < 0)
                    return;
                SetColor("color", value);
            }
        }

        [NumberPropertyDisplay("Intensity", min = 0.0)]
        public float intensity
        {
            get
            {
                if (objectID < 0)
                    return 0;
                return GetFloat("intensity");
            }
            set
            {
                if (objectID < 0)
                    return;
                SetFloat("intensity", value);
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

        protected TransparencyShader(int shaderID) : base(shaderID)
        { }

        [ColorPropertyDisplay("Color")]
        public Color color
        {
            get
            {
                if (objectID < 0)
                    return default(Color);
                return GetColor("color");
            }
            set
            {
                if (objectID < 0)
                    return;
                SetColor("color", value);
            }
        }

        [NumberPropertyDisplay("Refractive")]
        public float refractive
        {
            get
            {
                if (objectID < 0)
                    return 0;
                return GetFloat("refractive");
            }
            set
            {
                if (objectID < 0)
                    return;
                SetFloat("refractive", value);
            }
        }

        [NumberPropertyDisplay("Roughness")]
        public float roughness
        {
            get
            {
                if (objectID < 0)
                    return 0;
                return GetFloat("roughness");
            }
            set
            {
                if (objectID < 0)
                    return;
                SetFloat("roughness", value);
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
        public Color color
        {
            get
            {
                return GetColor("albedoColor");
            }
            set
            {
                SetColor("albedoColor", value);
            }
        }

        [ResourcePropertyDisplay("Albedo")]
        public Texture albedo
        {
            get
            {
                return GetTexture("albedoTexture");
            }
            set
            {
                SetTexture("albedoTexture", value);
            }
        }

        [NumberPropertyDisplay("Roughness", min = 0.01, max = 1.0)]
        public float roughness
        {
            get
            {
                return GetFloat("roughness");
            }
            set
            {
                SetFloat("roughness", value);
            }
        }

        [NumberPropertyDisplay("Metallic", min = 0.0, max = 1.0)]
        public float metallic
        {
            get
            {
                return GetFloat("metallic");
            }
            set
            {
                SetFloat("metallic", value);
            }
        }


        [ResourcePropertyDisplay("Normal")]
        public Texture bump
        {
            get
            {
                return GetTexture("bumpTex");
            }
            set
            {
                SetTexture("bumpTex", value);
            }
        }

        [ResourcePropertyDisplay("MRO")]
        public Texture mroTex
        {
            get
            {
                return GetTexture("mroTex");
            }
            set
            {
                SetTexture("mroTex", value);
            }
        }

        [ColorPropertyDisplay("EmissiveColor")]
        public Color emissive
        {
            get
            {
                return GetColor("emissive");
            }
            set
            {
                SetColor("emissive", value);
            }
        }

        [ResourcePropertyDisplay("Emissive")]
        public Texture emissiveTex
        {
            get
            {
                return GetTexture("emissiveTex");
            }
            set
            {
                SetTexture("emissiveTex", value);
            }
        }

        [ResourcePropertyDisplay("UVTile")]
        public Vector2 tile
        {
            get
            {
                return GetVector2("uvTile");
            }
            set
            {
                SetVector2("uvTile", value);
            }
        }
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
