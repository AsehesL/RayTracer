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
        public List<MaterialShader> shaders;

        public Dictionary<string, Mesh[]> meshes;

        public abstract void GenerateGeometry(string scenePath, List<PrimitiveBase> output);
    }

    [PrimitiveAnalyse(type = "Cube")]
    internal class CubeSerialization : PrimitiveSerialization
    {
        public Vector3 Position
        {
            get;set;
        }

        public Vector3 Scale
        {
            get;set;
        }

        public override void GenerateGeometry(string scenePath, List<PrimitiveBase> output)
        {

            CubePrimitive cube = CubePrimitive.Create();
            cube.position = Position;
            cube.scale = Scale;
            cube.euler = Vector3.zero;
            cube.material = shaders[0];

            output.Add(cube);
        }
    }

    [PrimitiveAnalyse(type = "Plane")]
    internal class PlaneSerialization : PrimitiveSerialization
    {
        public Vector3 Position
        {
            get; set;
        }

        public Vector3 Euler
        {
            get; set;
        }

        public Vector3 Scale
        {
            get; set;
        }

        public override void GenerateGeometry(string scenePath, List<PrimitiveBase> output)
        {
            PlanePrimitive plane = PlanePrimitive.Create();
            plane.position = Position;
            plane.scale = Scale;
            plane.euler = Euler;
            plane.material = shaders[0];

            output.Add(plane);
        }
    }

    [PrimitiveAnalyse(type = "Sphere")]
    internal class SphereSerialization : PrimitiveSerialization
    {
        public Vector3 Position
        {
            get; set;
        }

        public float Radius
        {
            get; set;
        }

        public override void GenerateGeometry(string scenePath, List<PrimitiveBase> output)
        {

            SpherePrimitive sphere = SpherePrimitive.Create();
            sphere.position = Position;
            sphere.euler = Vector3.zero;
            sphere.scale = new Vector3(Radius, Radius, Radius) * 2.0f;
            sphere.material = shaders[0];

            output.Add(sphere);
        }
    }

    [PrimitiveAnalyse(type = "Mesh")]
    internal class MeshSerialization : PrimitiveSerialization
    {
        public Vector3 Position
        {
            get; set;
        }

        public Vector3 Euler
        {
            get; set;
        }

        public Vector3 Scale
        {
            get; set;
        }

        public string Path
        {
            get;set;
        }

        public override void GenerateGeometry(string scenePath, List<PrimitiveBase> output)
        {
            var fileInfo = new System.IO.FileInfo(scenePath);
            string p = System.IO.Path.Combine(fileInfo.DirectoryName, Path);

            //List<Mesh> meshes = null;
            Mesh[] meshArray = null;
            if (meshes.ContainsKey(p))
            {
                meshArray = meshes[p];
            }
            else
            {
                meshArray = Mesh.CreateFromFile(p);
                if (meshArray != null)
                    meshes[p] = meshArray;
            }

            if (meshArray != null)
            {
                for(int i=0;i<meshArray.Length;i++)
                {
                    MeshPrimitive meshprimitive = MeshPrimitive.Create();
                    meshprimitive.position = Position;
                    meshprimitive.euler = Euler;
                    meshprimitive.scale = Scale;
                    meshprimitive.mesh = meshArray[i];
                    meshprimitive.material = shaders[0];

                    output.Add(meshprimitive);
                }
            }
        }
    }
}
