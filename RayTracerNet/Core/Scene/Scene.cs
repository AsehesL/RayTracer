using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace RayTracerNet
{
    public class Scene
    {
        private Camera m_activeCamera;

        private List<RayTracerObject> m_sceneObjects;
        private List<RayTracerObject> m_resObjects;


        [DllImport("RayTracerLib.dll", CallingConvention = CallingConvention.Cdecl)]
        private extern static int GetPrimitiveByScreenPos(int screenPosX, int screenPosY);

        public Scene()
        {
            m_activeCamera = Camera.Create();
            m_activeCamera.position = new Vector3(0, 1, -10);
            m_activeCamera.euler = new Vector3(0, 0, 0);

            m_sceneObjects = new List<RayTracerObject>();
            m_resObjects = new List<RayTracerObject>();
        }

        public PrimitiveBase GetPrimitiveRaycasted(int screenPosX, int screenPosY)
        {
            int objId = GetPrimitiveByScreenPos(screenPosX, screenPosY);
            if (objId < 0)
                return null;
            foreach(var obj in m_sceneObjects)
            {
                if (obj is PrimitiveBase && obj.objectID == objId)
                    return obj as PrimitiveBase;
            }
            return null;
        }

        public T CreateSceneObject<T>() where T : RayTracerObject
        {
            if (typeof(T).IsSubclassOf(typeof(RayTracerObject)) && typeof(T).IsAbstract == false)
            {
                MethodInfo createMethod = typeof(T).GetMethod("Create", BindingFlags.Public | BindingFlags.NonPublic | BindingFlags.Static);
                if (createMethod != null)
                {
                    T obj = createMethod.Invoke(null, new object[] { }) as T;
                    if (obj != null)
                        m_sceneObjects.Add(obj);
                    return obj;
                }
            }
            return null;
        }

        public void DestroySceneObject(object obj)
        {
            if (obj == null)
                return;
            if (obj.GetType().IsSubclassOf(typeof(RayTracerObject)) && obj.GetType().IsAbstract == false)
            {
                MethodInfo destroyMethod = obj.GetType().GetMethod("Destroy", BindingFlags.Public | BindingFlags.NonPublic | BindingFlags.Instance);
                if (destroyMethod != null)
                {
                    destroyMethod.Invoke(obj, new object[] { });
                }
            }
        }

        public Camera GetActiveCamera()
        {
            return m_activeCamera;
        }

        public void Create()
        {
            Clear();

            if (m_activeCamera == null)
            {
                m_activeCamera = Camera.Create();
                m_activeCamera.position = new Vector3(0, 1, -10);
                m_activeCamera.euler = new Vector3(0, 0, 0);
            }
        }

        public bool CreateFromFile(string scenePath)
        {
            var sceneData = RayTracerNet.Serialization.SceneSerialization.Deserialize(scenePath);
            if (sceneData == null)
            {
                Log.Err("场景序列化失败！");
                return false;
            }
            Clear();

            RenderSettings renderSettings = RayTracer.GetInstance().renderSettings;
            int renderWidth = renderSettings.renderWidth;
            int renderHeight = renderSettings.renderHeight;

            m_activeCamera = sceneData.camera.CreateCamera(m_activeCamera, ref renderWidth, ref renderHeight);

            renderSettings.renderWidth = renderWidth;
            renderSettings.renderHeight = renderHeight;

            Dictionary<string, Texture> textures = new Dictionary<string, Texture>();
            if (sceneData.textures != null)
            {
                for (int i = 0; i < sceneData.textures.Count; i++)
                {
                    var t = sceneData.textures[i].CreateTexture(scenePath);
                    if (t != null)
                    {
                        m_resObjects.Add(t);
                        textures.Add(sceneData.textures[i].name, t);
                    }
                }
            }

            Log.Info($"纹理加载完毕：共{textures.Count}张纹理");

            Dictionary<string, Mesh[]> meshes = new Dictionary<string, Mesh[]>();
            if (sceneData.meshes != null)
            {
                for (int i = 0; i < sceneData.meshes.Count; i++)
                {
                    var m = sceneData.meshes[i].CreateMesh(scenePath);
                    if (m != null)
                    {
                        m_resObjects.AddRange(m);
                    }
                }
            }

            var sky = sceneData.sky != null ? sceneData.sky.CreateSkyLight(textures) : null;
            var sun = sceneData.sky != null ? sceneData.sky.CreateSunLight() : null;
            if (sky != null)
                m_sceneObjects.Add(sky);
            if (sun != null)
                m_sceneObjects.Add(sun);

            Dictionary<string, MaterialShader> shaders = new Dictionary<string, MaterialShader>();
            if (sceneData.shaders != null)
            {
                for (int i = 0; i < sceneData.shaders.Count; i++)
                {
                    var s = sceneData.shaders[i].CreateShader(textures);
                    if (shaders != null)
                    {
                        m_resObjects.Add(s);
                        shaders.Add(sceneData.shaders[i].name, s);
                    }
                }
            }

            Log.Info($"Shader加载完毕：共{shaders.Count}个Shader");

            List<PrimitiveBase> primivites = new List<PrimitiveBase>();

            if (sceneData.geometries != null)
            {
                for (int i = 0; i < sceneData.geometries.Count; i++)
                {
                    sceneData.geometries[i].CreatePrimitive(scenePath, shaders, meshes, primivites);
                }
            }
            for (int i = 0; i < primivites.Count; i++)
                m_sceneObjects.Add(primivites[i]);

            foreach (var mesh in meshes)
            {
                m_resObjects.AddRange(mesh.Value);
            }

            Log.Info($"几何体加载完毕：共{primivites.Count}个几何体");

            return true;
        }

        public void Clear()
        {
            for(int i=0;i<m_sceneObjects.Count;i++)
            {
                m_sceneObjects[i].Destroy();
            }
            m_sceneObjects.Clear();
            for (int i=0;i< m_resObjects.Count;i++)
            {
                m_resObjects[i].Destroy();
            }
            m_resObjects.Clear();
        }
    }
}
