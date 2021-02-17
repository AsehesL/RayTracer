using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace RayTracerNet.Serialization
{
    [System.AttributeUsage(AttributeTargets.Class, AllowMultiple = false, Inherited = false)]
    internal class PrimitiveAnalyseAttribute : System.Attribute
    {
        public string type;
    }

    internal abstract class PrimitiveSerialization
    {
        public abstract void GenerateGeometry(string scenePath, List<MaterialShader> shaders, Dictionary<string, PrimitiveParamDataSerialization> geoParams, Dictionary<string, Mesh> meshes, List<PrimitiveBase> output);
    }

    [PrimitiveAnalyse(type = "Cube")]
    internal class CubeSerialization : PrimitiveSerialization
    {
        public override void GenerateGeometry(string scenePath, List<MaterialShader> shaders, Dictionary<string, PrimitiveParamDataSerialization> geoParams, Dictionary<string, Mesh> meshes, List<PrimitiveBase> output)
        {
            string position = geoParams["Position"].paramValue;
            string scale = geoParams["Scale"].paramValue;
            Vector3 s = Vector3.Parse(scale);
            Vector3 p = Vector3.Parse(position);

            CubePrimitive cube = CubePrimitive.Create();
            cube.position = p;
            cube.scale = s;
            cube.euler = Vector3.zero;
            cube.material = shaders[0];

            output.Add(cube);
        }
    }

    [PrimitiveAnalyse(type = "Plane")]
    internal class PlaneSerialization : PrimitiveSerialization
    {
        public override void GenerateGeometry(string scenePath, List<MaterialShader> shaders, Dictionary<string, PrimitiveParamDataSerialization> geoParams, Dictionary<string, Mesh> meshes, List<PrimitiveBase> output)
        {
            string euler = geoParams["Euler"].paramValue;
            string scale = geoParams["Scale"].paramValue;
            string position = geoParams["Position"].paramValue;
            Vector3 s = Vector3.Parse(scale);
            Vector3 p = Vector3.Parse(position);
            Vector3 r = Vector3.Parse(euler);

            PlanePrimitive plane = PlanePrimitive.Create();
            plane.position = p;
            plane.scale = s;
            plane.euler = r;
            plane.material = shaders[0];

            output.Add(plane);
        }
    }

    [PrimitiveAnalyse(type = "Sphere")]
    internal class SphereSerialization : PrimitiveSerialization
    {
        public override void GenerateGeometry(string scenePath, List<MaterialShader> shaders, Dictionary<string, PrimitiveParamDataSerialization> geoParams, Dictionary<string, Mesh> meshes, List<PrimitiveBase> output)
        {
            string position = geoParams["Position"].paramValue;
            string radius = geoParams["Radius"].paramValue;

            float r = float.Parse(radius);
            Vector3 p = Vector3.Parse(position);

            SpherePrimitive sphere = SpherePrimitive.Create();
            sphere.position = p;
            sphere.euler = Vector3.zero;
            sphere.scale = new Vector3(r, r, r) * 2.0f;
            sphere.material = shaders[0];

            output.Add(sphere);
        }
    }

    [PrimitiveAnalyse(type = "Mesh")]
    internal class MeshSerialization : PrimitiveSerialization
    {
        public override void GenerateGeometry(string scenePath, List<MaterialShader> shaders, Dictionary<string, PrimitiveParamDataSerialization> geoParams, Dictionary<string, Mesh> meshes, List<PrimitiveBase> output)
        {
            string path = geoParams["Path"].paramValue;
            string euler = geoParams["Euler"].paramValue;
            string scale = geoParams["Scale"].paramValue;
            string position = geoParams["Position"].paramValue;

            var fileInfo = new System.IO.FileInfo(scenePath);
            string p = System.IO.Path.Combine(fileInfo.DirectoryName, path);

            Vector3 rot = Vector3.Parse(euler);
            Vector3 sca = Vector3.Parse(scale);
            Vector3 pos = Vector3.Parse(position);

            Mesh mesh = null;
            if (meshes.ContainsKey(p))
            {
                mesh = meshes[p];
            }
            else
            {
                mesh = Mesh.CreateFromFile(p);
                if (mesh != null)
                    meshes[p] = mesh;
            }

            MeshPrimitive meshprimitive = MeshPrimitive.Create();
            meshprimitive.position = pos;
            meshprimitive.euler = rot;
            meshprimitive.scale = sca;
            meshprimitive.mesh = mesh;
            meshprimitive.material = shaders[0];

            output.Add(meshprimitive);
        }
    }
}
