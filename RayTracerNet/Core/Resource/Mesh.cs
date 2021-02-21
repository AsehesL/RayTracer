using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace RayTracerNet
{
    public class Mesh : ResourceObject
    {
        [DllImport("RayTracerLib.dll", CallingConvention = CallingConvention.Cdecl)]
        private extern static int CreateMesh(uint vertexCount, uint indexCount);

        [DllImport("RayTracerLib.dll", CallingConvention = CallingConvention.Cdecl)]
        private extern static void CreateMeshesFromFile(string path, ref int startMeshID, ref int endMeshID);

        [DllImport("RayTracerLib.dll", CallingConvention = CallingConvention.Cdecl)]
        private extern static uint GetMeshVertexCount(int meshID);

        [DllImport("RayTracerLib.dll", CallingConvention = CallingConvention.Cdecl)]
        private extern static uint GetMeshIndexCount(int meshID);

        [DllImport("RayTracerLib.dll", CallingConvention = CallingConvention.Cdecl)]
        private extern static void SetMeshVertex(int meshID, int i, double x, double y, double z);

        [DllImport("RayTracerLib.dll", CallingConvention = CallingConvention.Cdecl)]
        private extern static void SetMeshNormal(int meshID, int i, double x, double y, double z);

        [DllImport("RayTracerLib.dll", CallingConvention = CallingConvention.Cdecl)]
        private extern static void SetMeshUV(int meshID, int i, int channel, double x, double y);

        [DllImport("RayTracerLib.dll", CallingConvention = CallingConvention.Cdecl)]
        private extern static void SetMeshTangent(int meshID, int i, double x, double y, double z, double w);

        [DllImport("RayTracerLib.dll", CallingConvention = CallingConvention.Cdecl)]
        private extern static void SetMeshIndex(int meshID, int i, uint index);

        private string m_path = null;

        public override string GetFullPathName()
        {
            return m_path;
        }

        public override string GetObjectName()
        {
            return "Mesh_" + objectID;
        }

        public override string GetTypeName()
        {
            return "Mesh";
        }

        public override string ToString()
        {
            if (string.IsNullOrEmpty(m_path))
                return "Mesh_" + objectID;
            return "Mesh_" + objectID + ":" + m_path;
        }

        public uint vertexCount
        {
            get
            {
                return GetMeshVertexCount(objectID);
            }
        }

        public uint indexCount
        {
            get
            {
                return GetMeshIndexCount(objectID);
            }
        }

        protected Mesh(int objectID) : base(objectID)
        {

        }

        private static Vector4 CalculateTangent(Vector3 v0, Vector3 v1, Vector3 v2, Vector2 uv0, Vector2 uv1, Vector2 uv2, Vector3 normal)
        {
            double x1 = v1.x - v0.x;
            double x2 = v2.x - v0.x;
            double y1 = v1.y - v0.y;
            double y2 = v2.y - v0.y;
            double z1 = v1.z - v0.z;
            double z2 = v2.z - v0.z;
            double s1 = uv1.x - uv0.x;
            double s2 = uv2.x - uv0.x;
            double t1 = uv1.y - uv0.y;
            double t2 = uv2.y - uv0.y;
            double r = 1.0f / (s1 * t2 - s2 * t1);

            Vector3 sdir = new Vector3((t2 * x1 - t1 * x2) * r, (t2 * y1 - t1 * y2) * r, (t2 * z1 - t1 * z2) * r);
            Vector3 tdir = new Vector3((s1 * x2 - s2 * x1) * r, (s1 * y2 - s2 * y1) * r, (s1 * z2 - s2 * z1) * r);

            Vector3 tmp = (sdir - normal * Vector3.Dot(normal, sdir)).normalized;

            double sign = Vector3.Dot(Vector3.Cross(normal, sdir), tdir) > 0.0 ? -1.0 : 1.0;

            return new Vector4(tmp.x, tmp.y, tmp.z, sign);
        }

        public static Mesh Create(uint vertexCount, uint indexCount)
        {
            RayTracer instance = RayTracer.GetInstance();
            if (!instance.IsInitialized())
                return null;
            int meshID = CreateMesh(vertexCount, indexCount);
            if (meshID >= 0) 
            {
                Mesh mesh = new Mesh(meshID);
                MessageHandler.Broadcast<ResourceObject>(MessageName.AddNewResource, mesh);
                return mesh;
            };
            return null;
        }

        public static Mesh[] CreateFromFile(string path)
        {
            RayTracer instance = RayTracer.GetInstance();
            if (!instance.IsInitialized())
                return null;
            System.IO.FileInfo fileInfo = new System.IO.FileInfo(path);
            if (fileInfo.Exists == false)
                return null;
            if(fileInfo.Extension.ToLower().EndsWith(".mesh") == true)
            {
                Mesh mesh = LoadFromMesh(path);
                if (mesh != null)
                    MessageHandler.Broadcast<ResourceObject>(MessageName.AddNewResource, mesh);
                return new Mesh[] { mesh };
            }
            int startMeshID = -1;
            int endMeshID = -1;
            CreateMeshesFromFile(path, ref startMeshID, ref endMeshID);
            if (startMeshID >= 0 && endMeshID >= 0)
            {
                Mesh[] results = new Mesh[endMeshID - startMeshID + 1];
                for(int i=startMeshID;i<=endMeshID;i++)
                {
                    Mesh mesh = new Mesh(i);
                    mesh.m_path = path;
                    MessageHandler.Broadcast<ResourceObject>(MessageName.AddNewResource, mesh);
                    results[i - startMeshID] = mesh;
                }
                return results;
            }
            return null;
        }

        private static Mesh LoadFromMesh(string path)
        {
            using (FileStream fs = new FileStream(path, FileMode.Open, FileAccess.Read))
            {
                using (System.IO.BinaryReader reader = new System.IO.BinaryReader(fs))
                {
                    List<Vector3> vertexList = new List<Vector3>();
                    List<Vector3> normalList = new List<Vector3>();
                    List<Vector2> uvList = new List<Vector2>();
                    List<int> indexList = new List<int>();

                    int vertexCount = reader.ReadInt32();
                    for (int i = 0; i < vertexCount; i++)
                    {
                        float x = reader.ReadSingle();
                        float y = reader.ReadSingle();
                        float z = reader.ReadSingle();
                        vertexList.Add(new Vector3(x, y, z));
                    }
                    int normalCount = reader.ReadInt32();
                    for (int i = 0; i < normalCount; i++)
                    {
                        float x = reader.ReadSingle();
                        float y = reader.ReadSingle();
                        float z = reader.ReadSingle();
                        normalList.Add(new Vector3(x, y, z));
                    }
                    int uvCount = reader.ReadInt32();
                    for (int i = 0; i < uvCount; i++)
                    {
                        float x = reader.ReadSingle();
                        float y = reader.ReadSingle();
                        uvList.Add(new Vector2(x, y));
                    }
                    int colorCount = reader.ReadInt32();
                    for (int i = 0; i < colorCount; i++)
                    {
                        float r = reader.ReadSingle();
                        float g = reader.ReadSingle();
                        float b = reader.ReadSingle();
                        float a = reader.ReadSingle();
                    }
                    int indexCount = reader.ReadInt32();
                    for (int i = 0; i < indexCount; i++)
                    {
                        int index = reader.ReadInt32();
                        indexList.Add(index);
                    }

                    int meshID = CreateMesh((uint)vertexCount, (uint)indexCount);
                    if (meshID < 0)
                    {
                        return null;
                    };

                    Mesh mesh = new Mesh(meshID);
                    mesh.m_path = path;

                    for (int i=0;i<vertexCount;i++)
                    {
                        Vector3 normal = i < normalList.Count ? normalList[i] : Vector3.zero;
                        Vector2 uv = i < uvList.Count ? uvList[i] : Vector2.zero;
                        SetMeshVertex(mesh.objectID, i, vertexList[i].x, vertexList[i].y, vertexList[i].z);
                        SetMeshNormal(mesh.objectID, i, normal.x, normal.y, normal.z);
                        SetMeshUV(mesh.objectID, i, 0, uv.x, uv.y);
                        //SetMeshTangent(mesh.objectID, i, tangentlist[i].x, tangentlist[i].y, tangentlist[i].z, tangentlist[i].w);
                    }
                    for (int i=0;i<indexCount;i++)
                    {
                        SetMeshIndex(mesh.objectID, i, (uint)indexList[i]);
                    }
                    return mesh;
                }
            }
            return null;
        }
    }
}
