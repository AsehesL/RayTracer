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

        public static Mesh CreateFromFile(string path)
        {
            RayTracer instance = RayTracer.GetInstance();
            if (!instance.IsInitialized())
                return null;
            System.IO.FileInfo fileInfo = new System.IO.FileInfo(path);
            if (fileInfo.Exists == false)
                return null;
            if (fileInfo.Extension.ToLower().EndsWith(".obj") == true)
            {
                Mesh mesh = LoadFromObj(path);
                if (mesh != null)
                {
                    MessageHandler.Broadcast<ResourceObject>(MessageName.AddNewResource, mesh);
                }
                return mesh;
            }
            else if(fileInfo.Extension.ToLower().EndsWith(".mesh") == true)
            {
                Mesh mesh = LoadFromMesh(path);
                if (mesh != null)
                    MessageHandler.Broadcast<ResourceObject>(MessageName.AddNewResource, mesh);
                return mesh;
            }
            return null;
        }

        private static Mesh LoadFromObj(string path)
        {
            using (System.IO.StreamReader reader = new System.IO.StreamReader(path))
            {
                List<Vector3> vlist = new List<Vector3>();
                List<Vector3> nlist = new List<Vector3>();
                List<Vector2> ulist = new List<Vector2>();
                List<Vector4> tangentlist = new List<Vector4>();
                List<Vector3> vertexList = new List<Vector3>();
                List<Vector3> normalList = new List<Vector3>();
                List<Vector2> uv0List = new List<Vector2>();
                List<int> flist = new List<int>();

                Vector4[] tangents = null;
                int[] tangentCounts = null;

                while (reader.EndOfStream == false)
                {
                    string line = reader.ReadLine();
                    if (line.Length > 1)
                    {
                        char df0 = line[0];
                        char df1 = line[1];

                        if (df0 == 'v')
                        {
                            string[] sp = line.Substring(2).Trim().Split(' ');
                            if (df1 == 'n')
                            {
                                double nx = -double.Parse(sp[0]);
                                double ny = double.Parse(sp[1]);
                                double nz = double.Parse(sp[2]);
                                nlist.Add(new Vector3(nx, ny, nz));
                            }
                            else if (df1 == 't')
                            {
                                double ux = double.Parse(sp[0]);
                                double uy = double.Parse(sp[1]);
                                ulist.Add(new Vector2(ux, uy));
                            }
                            else
                            {
                                double px = -double.Parse(sp[0]);
                                double py = double.Parse(sp[1]);
                                double pz = double.Parse(sp[2]);
                                vlist.Add(new Vector3(px, py, pz));
                            }
                        }
                        else if (df0 == 'g')
                        {
                            //sdindex += 1;
                        }
                        else if (df0 == 'f')
                        {
                            string[] sp = line.Split(' ');
                            string[] face0 = sp[1].Split('/');
                            string[] face1 = sp[2].Split('/');
                            string[] face2 = sp[3].Split('/');

                            int vindex0 = int.Parse(face0[0]) - 1;
                            int vindex1 = int.Parse(face1[0]) - 1;
                            int vindex2 = int.Parse(face2[0]) - 1;

                            int uindex0 = int.Parse(face0[1]) - 1;
                            int uindex1 = int.Parse(face1[1]) - 1;
                            int uindex2 = int.Parse(face2[1]) - 1;

                            int nindex0 = int.Parse(face0[2]) - 1;
                            int nindex1 = int.Parse(face1[2]) - 1;
                            int nindex2 = int.Parse(face2[2]) - 1;

                            Vector3 v0 = vlist[vindex0];
                            Vector3 v1 = vlist[vindex2];
                            Vector3 v2 = vlist[vindex1];

                            Vector3 n0 = nlist[nindex0];
                            Vector3 n1 = nlist[nindex2];
                            Vector3 n2 = nlist[nindex1];

                            Vector2 uv0 = ulist[uindex0];
                            Vector2 uv1 = ulist[uindex2];
                            Vector2 uv2 = ulist[uindex1];

                            vertexList.Add(v0);
                            vertexList.Add(v1);
                            vertexList.Add(v2);

                            normalList.Add(n0);
                            normalList.Add(n1);
                            normalList.Add(n2);

                            uv0List.Add(uv0);
                            uv0List.Add(uv1);
                            uv0List.Add(uv2);

                            Vector4 tangent0 = CalculateTangent(v0, v1, v2, uv0, uv1, uv2, n0);
                            Vector4 tangent1 = CalculateTangent(v0, v1, v2, uv0, uv1, uv2, n1);
                            Vector4 tangent2 = CalculateTangent(v0, v1, v2, uv0, uv1, uv2, n2);

                            if (tangents == null)
                                tangents = new Vector4[nlist.Count];
                            if (tangentCounts == null)
                                tangentCounts = new int[nlist.Count];
                            flist.Add(nindex0);
                            flist.Add(nindex2);
                            flist.Add(nindex1);
                            tangents[nindex0] += tangent0;
                            tangents[nindex2] += tangent1;
                            tangents[nindex1] += tangent2;
                            tangentCounts[nindex0] += 1;
                            tangentCounts[nindex2] += 1;
                            tangentCounts[nindex1] += 1;
                        }
                    }
                }

                int tcount = flist.Count / 3;
                for (int i = 0; i < tcount; i++)
                {
                    int tangentIndex0 = flist[i * 3];
                    int tangentIndex1 = flist[i * 3 + 1];
                    int tangentIndex2 = flist[i * 3 + 2];
                    Vector4 tangent0 = tangents[tangentIndex0] / tangentCounts[tangentIndex0];
                    Vector4 tangent1 = tangents[tangentIndex1] / tangentCounts[tangentIndex1];
                    Vector4 tangent2 = tangents[tangentIndex2] / tangentCounts[tangentIndex2];

                    tangentlist.Add(tangent0);
                    tangentlist.Add(tangent1);
                    tangentlist.Add(tangent2);
                }

                int meshID = CreateMesh((uint)vertexList.Count, (uint)vertexList.Count);
                if (meshID < 0)
                {
                    return null;
                };

                Mesh mesh = new Mesh(meshID);
                mesh.m_path = path;

                {
                    int vcount = (int)mesh.vertexCount;
                    int icount = (int)mesh.indexCount;
                    for (int i = 0; i < vcount; i++)
                    {
                        SetMeshVertex(mesh.objectID, i, vertexList[i].x, vertexList[i].y, vertexList[i].z);
                        SetMeshNormal(mesh.objectID, i, normalList[i].x, normalList[i].y, normalList[i].z);
                        SetMeshUV(mesh.objectID, i, 0, uv0List[i].x, uv0List[i].y);
                        SetMeshTangent(mesh.objectID, i, tangentlist[i].x, tangentlist[i].y, tangentlist[i].z, tangentlist[i].w);
                        SetMeshIndex(mesh.objectID, i, (uint)i);
                    }
                }
                return mesh;
            }
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
