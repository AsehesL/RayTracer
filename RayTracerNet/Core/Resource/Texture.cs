using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace RayTracerNet
{
    public enum WrapMode
    {
        Repeat = 0,
        Clamp = 1,
    }

    public enum FilterMode
    {
        Point = 0,
        Bilinear = 1,
        Trilinear = 2,
    }

    public class Texture : ResourceObject
    {

        [DllImport("RayTracerLib.dll", CallingConvention = CallingConvention.Cdecl)]
        private extern static int CreateTexture(uint width, uint height);

        [DllImport("RayTracerLib.dll", CallingConvention = CallingConvention.Cdecl)]
        private extern static int CreateTextureFromFile(string path, bool isLinear);

        [DllImport("RayTracerLib.dll", CallingConvention = CallingConvention.Cdecl)]
        private extern static uint GetTextureWidth(int textureID);

        [DllImport("RayTracerLib.dll", CallingConvention = CallingConvention.Cdecl)]
        private extern static uint GetTextureHeight(int textureID);

        [DllImport("RayTracerLib.dll", CallingConvention = CallingConvention.Cdecl)]
        private extern static void FillTexture(int textureID, float r, float g, float b, float a);

        [DllImport("RayTracerLib.dll", CallingConvention = CallingConvention.Cdecl)]
        private extern static void SetTexturePixel(int textureID, int x, int y, float r, float g, float b, float a);

        [DllImport("RayTracerLib.dll", CallingConvention = CallingConvention.Cdecl)]
        private extern static void GetTexturePixel(int textureID, int x, int y, ref float r, ref float g, ref float b, ref float a);

        [DllImport("RayTracerLib.dll", CallingConvention = CallingConvention.Cdecl)]
        private extern static void ApplyTextureData(int textureID);

        [DllImport("RayTracerLib.dll", CallingConvention = CallingConvention.Cdecl)]
        private extern static void SetTextureFilterMode(int textureID, int filterMode);

        [DllImport("RayTracerLib.dll", CallingConvention = CallingConvention.Cdecl)]
        private extern static int GetTextureFilterMode(int textureID);

        [DllImport("RayTracerLib.dll", CallingConvention = CallingConvention.Cdecl)]
        private extern static void SetTextureWrapMode(int textureID, int wrapMode);

        [DllImport("RayTracerLib.dll", CallingConvention = CallingConvention.Cdecl)]
        private extern static int GetTextureWrapMode(int textureID);

        [DllImport("RayTracerLib.dll", CallingConvention = CallingConvention.Cdecl)]
        private extern static bool DestroyTexture(int textureID);

        private string m_path = null;

        public override string GetFullPathName()
        {
            return m_path;
        }

        public override string GetObjectName()
        {
            return "Texture_" + objectID;
        }

        public override string GetTypeName()
        {
            return "Texture";
        }

        public override string ToString()
        {
            if (string.IsNullOrEmpty(m_path))
                return "Texture_" + objectID;
            return "Texture_" + objectID + ":" + m_path;
        }
        
        [TextPropertyDisplay("Width")]
        public uint width
        {
            get
            {
                return GetTextureWidth(objectID);
            }
        }

        [TextPropertyDisplay("Height")]
        public uint height
        {
            get
            {
                return GetTextureHeight(objectID);
            }
        }

        [EnumPropertyDisplay("FilterMode")]
        public FilterMode filterMode
        {
            get
            {
                int v = GetTextureFilterMode(objectID);
                return (FilterMode)v;
            }
            set
            {
                int v = (int)value;
                SetTextureFilterMode(objectID, v);
            }
        }

        [EnumPropertyDisplay("WrapMode")]
        public WrapMode wrapMode
        {
            get
            {
                int v = GetTextureWrapMode(objectID);
                return (WrapMode)v;
            }
            set
            {
                int v = (int)value;
                SetTextureWrapMode(objectID, v);
            }
        }

        protected Texture(int objectID) : base(objectID)
        {

        }

        public static Texture Create(uint width, uint height)
        {
            RayTracer instance = RayTracer.GetInstance();
            if (!instance.IsInitialized())
                return null;
            int textureID = CreateTexture(width, height);
            if (textureID >= 0)
            {
                Texture texture = new Texture(textureID);
                texture.filterMode = FilterMode.Bilinear;
                MessageHandler.Broadcast<ResourceObject>(MessageName.AddNewResource, texture);
                return texture;
            };
            return null;
        }

        public static Texture CreateFromFile(string path)
        {
            RayTracer instance = RayTracer.GetInstance();
            if (!instance.IsInitialized())
                return null;
            System.IO.FileInfo fileInfo = new System.IO.FileInfo(path);
            if (fileInfo.Exists == false)
                return null;
            bool isLinear = false;
            if (fileInfo.Extension.ToLower().EndsWith(".hdr"))
                isLinear = true;
            int textureID = CreateTextureFromFile(path, isLinear);
            if (textureID >= 0)
            {
                Texture texture = new Texture(textureID);
                texture.filterMode = FilterMode.Bilinear;
                MessageHandler.Broadcast<ResourceObject>(MessageName.AddNewResource, texture);
                return texture;
            };
            return null;
        }

        public static Texture CreateFromFile(string path, bool isLinear)
        {
            RayTracer instance = RayTracer.GetInstance();
            if (!instance.IsInitialized())
                return null;
            System.IO.FileInfo fileInfo = new System.IO.FileInfo(path);
            if (fileInfo.Exists == false)
                return null;
            int textureID = CreateTextureFromFile(path, isLinear);
            if (textureID >= 0)
            {
                Texture texture = new Texture(textureID);
                texture.filterMode = FilterMode.Bilinear;
                MessageHandler.Broadcast<ResourceObject>(MessageName.AddNewResource, texture);
                return texture;
            };
            return null;
        }

        public void Fill(Color color)
        {
            FillTexture(objectID, color.r, color.g, color.b, color.a);
        }

        public void SetPixel(int x, int y, Color color)
        {
            SetTexturePixel(objectID, x, y, color.r, color.g, color.b, color.a);
        }

        public Color GetPixel(int x, int y)
        {
            float r = 0, g = 0, b = 0, a = 0;
            GetTexturePixel(objectID, x, y, ref r, ref g, ref b, ref a);
            return new Color(r, g, b, a);
        }

        public void Apply()
        {
            ApplyTextureData(objectID);
        }

        public override void Destroy()
        {
            if (objectID >= 0)
            {
                DestroyTexture(objectID);
                MessageHandler.Broadcast<ResourceObject>(MessageName.RemoveResource, this);
                objectID = -1;
            }
        }
    }
}
