using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;
using System.Xml.Serialization;
using System.Reflection;

namespace RayTracerNet.Serialization
{

    //[System.AttributeUsage(AttributeTargets.Property, AllowMultiple = false, Inherited = false)]
    //public class PropertySerializationAttribute : System.Attribute
    //{
    //    public string propertyName;
    //}

    [XmlRoot("SceneRoot")]
    public class SceneDataSerialization
    {
        [XmlArray("Textures")]
        [XmlArrayItem("Texture")]
        public List<TextureDataSerialization> textures;

        [XmlArray("Meshes")]
        [XmlArrayItem("Meshes")]
        public List<MeshDataSerialization> meshes;

        [XmlArray("Shaders")]
        [XmlArrayItem("Shader")]
        public List<ShaderDataSerialization> shaders;

        [XmlElement("Sky")] public SkyData sky;

        [XmlElement("Camera")] public CameraDataSerialization camera;

        [XmlArray("Geometries")]
        [XmlArrayItem("Geometry")]
        public List<PrimitiveDataSerialization> geometries;
    }

    public class CameraDataSerialization
    {
        [XmlAttribute("Position")] public string position;

        [XmlAttribute("Euler")] public string euler;

        [XmlAttribute("Near")] public double near;

        [XmlAttribute("FieldOfView")] public double fieldOfView;

        [XmlAttribute("Radius")] public float radius;

        [XmlAttribute("Focal")] public float focal;

        [XmlAttribute("UseThinLens")] public bool useThinLens;

        [XmlAttribute("Width")] public int width;

        [XmlAttribute("Height")] public int height;

        public Camera CreateCamera(Camera defaultCamera, ref int defaultWidth, ref int defaultHeight)
        {
            if (width > 0)
                defaultWidth = width;
            if (height > 0)
                defaultHeight = height;

            Vector3 pos = Vector3.Parse(position);
            Vector3 rot = Vector3.Parse(euler);

            Camera cam = defaultCamera == null ? Camera.Create() : defaultCamera;
            cam.position = pos;
            cam.euler = rot;
            cam.fieldOfView = fieldOfView;
            cam.useThinLens = useThinLens;
            cam.focal = focal;
            cam.radius = radius;

            if (useThinLens)
            {
                Log.Info($"相机创建成功:Position:{pos},Rotation:{rot},焦距:{focal},光圈半径:{radius}");
            }
            else
            {
                Log.Info($"相机创建成功:Position:{pos},Rotation:{rot}");
            }
            return cam;
        }
    }

    public struct ShaderNameSerialization
    {
        [XmlAttribute("Name")]
        public string shaderName;
    }

    public class PrimitiveDataSerialization
    {
        [XmlAttribute("Type")] public string type;

        [XmlArray("Params")]
        [XmlArrayItem("Param")]
        public List<PrimitiveParamDataSerialization> geoParams;

        [XmlArray("Shaders")]
        [XmlArrayItem("Shader")]
        public List<ShaderNameSerialization> shaders;

        public void CreatePrimitive(string scenePath, Dictionary<string, MaterialShader> shadersDic, Dictionary<string, Mesh[]> meshDic, List<PrimitiveBase> output)
        {
            //Dictionary<string, PrimitiveParamDataSerialization> geoParamDic = new Dictionary<string, PrimitiveParamDataSerialization>();
            //for (int i = 0; i < geoParams.Count; i++)
            //{
            //    geoParamDic[geoParams[i].paramName] = geoParams[i];
            //}

            List<MaterialShader> s = new List<MaterialShader>();
            for (int i = 0; i < shaders.Count; i++)
            {
                if (shadersDic.ContainsKey(shaders[i].shaderName))
                    s.Add(shadersDic[shaders[i].shaderName]);
            }

            System.Type[] types = typeof(PrimitiveBase).Assembly.GetTypes();
            System.Type geoSerializationType = typeof(PrimitiveSerialization);
            PrimitiveSerialization geoSerialization = null;
            for (int i = 0; i < types.Length; i++)
            {
                if (types[i].IsAbstract)
                    continue;
                if (types[i].IsSubclassOf(geoSerializationType))
                {
                    var attributes = types[i].GetCustomAttributes(typeof(PrimitiveAnalyseAttribute), false);
                    if (attributes == null || attributes.Length == 0)
                        continue;
                    var geoattribute = attributes[0] as PrimitiveAnalyseAttribute;
                    if (geoattribute == null)
                        continue;
                    if (geoattribute.type == type)
                    {
                        geoSerialization = System.Activator.CreateInstance(types[i]) as PrimitiveSerialization;
                        break;
                    }
                }
            }

            if (geoSerialization == null)
            {
                Log.Warn($"几何体创建失败！不确定的几何体类型：{type}");
                return;
            }

            for (int i = 0; i < geoParams.Count; i++)
            {
                SceneSerialization.SetParameterToObject(geoSerialization, geoParams[i].paramName, geoParams[i].paramValue, null);
            }

            geoSerialization.meshes = meshDic;
            geoSerialization.shaders = s;
            geoSerialization.GenerateGeometry(scenePath, output);
        }
    }

    public class PrimitiveParamDataSerialization
    {

        [XmlAttribute("Key")] public string paramName;

        [XmlAttribute("Value")] public string paramValue;

    }

    public class TextureDataSerialization
    {
        [XmlAttribute("Name")]
        public string name;

        [XmlAttribute("Path")]
        public string path;

        [XmlAttribute("WrapMode")]
        public string wrapMode;

        [XmlAttribute("FilterMode")]
        public string filterMode;

        [XmlAttribute("Linear")]
        public bool linear;

        public Texture CreateTexture(string scenePath)
        {
            var fileInfo = new System.IO.FileInfo(scenePath);
            string p = System.IO.Path.Combine(fileInfo.DirectoryName, path);
            Texture tex = Texture.CreateFromFile(p, linear);
            if (tex == null)
                return null;
            FilterMode filterMode = FilterMode.Point;
            WrapMode wrapMode = WrapMode.Clamp;
            if (tex != null)
            {
                if (this.wrapMode == "Clamp")
                    wrapMode = WrapMode.Clamp;
                else if (this.wrapMode == "Repeat")
                    wrapMode = WrapMode.Repeat;
                if (this.filterMode == "Point")
                    filterMode = FilterMode.Point;
                else if (this.filterMode == "Bilinear")
                    filterMode = FilterMode.Bilinear;
                tex.filterMode = filterMode;
                tex.wrapMode = wrapMode;
            }
            return tex;
        }
    }

    public class MeshDataSerialization
    {
        [XmlAttribute("Name")]
        public string name;

        [XmlAttribute("Path")]
        public string path;

        public Mesh[] CreateMesh(string scenePath)
        {
            var fileInfo = new System.IO.FileInfo(scenePath);
            string p = System.IO.Path.Combine(fileInfo.DirectoryName, path);
            var meshes = Mesh.CreateFromFile(p);
            return meshes;
        }
    }

    public class ShaderDataSerialization
    {
        [XmlAttribute("Name")]
        public string name;

        [XmlAttribute("ShaderType")]
        public string shaderType;

        [XmlArray("Params")]
        [XmlArrayItem("Param")]
        public List<ShaderParamDataSerialization> shaderParams;

        public MaterialShader CreateShader(Dictionary<string, Texture> textures)
        {
            var assembly = typeof(MaterialShader).Assembly;
            var types = assembly.GetTypes();
            MethodInfo createMethod = null;
            for (int i = 0; i < types.Length; i++)
            {
                if (types[i].IsAbstract || !types[i].IsSubclassOf(typeof(MaterialShader)))
                    continue;
                var attributes = types[i].GetCustomAttributes(typeof(ShaderTypeAttribute), true);
                if (attributes != null && attributes.Length > 0)
                {
                    var shaderAttribute = attributes[0] as ShaderTypeAttribute;
                    if (shaderAttribute != null && shaderAttribute.shaderType == shaderType)
                    {
                        var method = types[i].GetMethod("Create", BindingFlags.Public | BindingFlags.NonPublic | BindingFlags.Static);
                        if (method != null)
                        {
                            createMethod = method;
                            break;
                        }
                    }
                }
            }
            if (createMethod == null)
            {
                Log.Warn($"Shader初时化失败！未找到该类型的Shader:{shaderType}");
                return null;
            }
            MaterialShader shader = createMethod.Invoke(null, new object[] { }) as MaterialShader;

            if (shader == null)
                return null;
            if (shaderParams != null)
            {
                for (int i = 0; i < shaderParams.Count; i++)
                {
                    SceneSerialization.SetParameterToObject(shader, shaderParams[i].paramName, shaderParams[i].paramValue, textures);
                }
            }

            return shader;
        }
    }

    public class ShaderParamDataSerialization
    {

        [XmlAttribute("Key")]
        public string paramName;

        [XmlAttribute("Value")]
        public string paramValue;
    }

    public class SkyData
    {

        [XmlAttribute("ShaderType")]
        public string shaderType;

        [XmlElement("SunLight")]
        public SunLightData sun;

        [XmlArray("Params")]
        [XmlArrayItem("Param")]
        public List<ShaderParamDataSerialization> shaderParams;

        public SunLight CreateSunLight()
        {
            return sun != null ? sun.CreateSunLight() : null;
        }

        public SkyLight CreateSkyLight(Dictionary<string, Texture> textures)
        {
            var assembly = typeof(SkyLight).Assembly;
            var types = assembly.GetTypes();
            MethodInfo createMethod = null;
            for (int i = 0; i < types.Length; i++)
            {
                if (types[i].IsAbstract || !types[i].IsSubclassOf(typeof(SkyLight)))
                    continue;
                var attributes = types[i].GetCustomAttributes(typeof(SkyLightTypeAttribute), true);
                if (attributes != null && attributes.Length > 0)
                {
                    var shaderAttribute = attributes[0] as SkyLightTypeAttribute;
                    if (shaderAttribute != null && shaderAttribute.skyLightType == shaderType)
                    {
                        var method = types[i].GetMethod("Create", BindingFlags.Public | BindingFlags.NonPublic | BindingFlags.Static);
                        if (method != null)
                        {
                            createMethod = method;
                            break;
                        }
                    }
                }
            }

            if (createMethod == null)
                return null;
            SkyLight sky = createMethod.Invoke(null, new object[] { }) as SkyLight;

            if (sky == null)
                return null;

            if (shaderParams != null)
            {
                for (int i = 0; i < shaderParams.Count; i++)
                {
                    SceneSerialization.SetParameterToObject(sky, shaderParams[i].paramName, shaderParams[i].paramValue, textures);
                }
            }

            Log.Info($"天空盒创建成功:{sky.GetType()}");


            return sky;
        }
    }

    public class SunLightData
    {
        [XmlAttribute("Euler")] public string euler;

        [XmlAttribute("Color")] public string color;

        [XmlAttribute("Intensity")] public float intensity;

        [XmlAttribute("VolumetricLighting")] public bool renderParticipatingMedia;

        public SunLight CreateSunLight()
        {
            Vector3 rot = Vector3.Parse(euler);
            Color col = Color.Parse(color);

            SunLight sunLight = SunLight.Create();
            sunLight.color = col;
            sunLight.intensity = intensity;
            sunLight.euler = string.IsNullOrEmpty(euler) ? new Vector3(50, -30, 0) : rot;


            Log.Info($"创建SunLight成功:旋转:{sunLight.euler},颜色:{col},强度:{intensity},是否渲染参与介质:{renderParticipatingMedia}");
            return sunLight;
        }
    }

    public static class SceneSerialization
    {
        internal static void SetParameterToObject(object targetObject, string propertyName, string propertyValue, Dictionary<string, Texture> textures)
        {
            if (targetObject == null)
                return;
            PropertyInfo propertyInfo = targetObject.GetType().GetProperty(propertyName, BindingFlags.Public | BindingFlags.Instance);
            if (propertyInfo != null)
            {
                System.Object valueObject = CreateParamValue(propertyInfo.PropertyType, propertyValue, textures);
                propertyInfo.SetValue(targetObject, valueObject);
            }
            else
            {
                var properties = targetObject.GetType().GetProperties(BindingFlags.Public | BindingFlags.Instance);
                for (int i = 0; i < properties.Length; i++)
                {
                    //PropertySerializationAttribute attribute = properties[i].GetCustomAttribute<PropertySerializationAttribute>();
                    //if (attribute != null && attribute.propertyName == propertyName)
                    if (properties[i].Name == propertyName)
                    {
                        System.Object valueObject = CreateParamValue(properties[i].PropertyType, propertyValue, textures);
                        properties[i].SetValue(targetObject, valueObject);
                        return;
                    }
                }
            }
        }

        internal static System.Object CreateParamValue(System.Type propertyType, string propertyValue, Dictionary<string, Texture> textures)
        {
            if (propertyType == typeof(Color))
            {
                return Color.Parse(propertyValue);
            }
            else if (propertyType == typeof(double))
            {
                return double.Parse(propertyValue);
            }
            else if (propertyType == typeof(float))
            {
                return float.Parse(propertyValue);
            }
            else if (propertyType == typeof(int))
            {
                return int.Parse(propertyValue);
            }
            else if (propertyType == typeof(bool))
            {
                return bool.Parse(propertyValue);
            }
            else if (propertyType == typeof(Texture))
            {
                if (textures.ContainsKey(propertyValue))
                    return textures[propertyValue];
                return null;
            }
            else if (propertyType == typeof(Vector2))
            {
                return Vector2.Parse(propertyValue);
            }
            else if (propertyType == typeof(Vector3))
            {
                return Vector3.Parse(propertyValue);
            }
            else if (propertyType == typeof(Vector4))
            {
                return Vector4.Parse(propertyValue);
            }
            return propertyValue;
        }

        public static SceneDataSerialization Deserialize(string path)
        {
            if (System.IO.File.Exists(path) == false)
            {
                Log.Err($"无法定位该文件位置：" + path);
                return null;
            }

            FileStream stream = new FileStream(path, FileMode.Open, FileAccess.Read);

            XmlSerializer serializer = new XmlSerializer(typeof(SceneDataSerialization));
            SceneDataSerialization root = null;
            try
            {
                root = (SceneDataSerialization)serializer.Deserialize(stream);
            }
            catch (System.Exception e)
            {
                Log.Err(e.Message);
            }

            stream.Dispose();
            stream.Close();

            return root;
        }
    }
}
