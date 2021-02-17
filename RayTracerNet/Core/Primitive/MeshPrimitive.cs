using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace RayTracerNet
{

    public class MeshPrimitive : PrimitiveBase
    {
        [DllImport("RayTracerLib.dll")]
        private extern static int CreateMeshPrimitive();

        [DllImport("RayTracerLib.dll", CallingConvention = CallingConvention.Cdecl)]
        private extern static void SetMeshToMeshPrimitive(int objectID, int meshID);

        private Mesh m_mesh;

        private MeshPrimitive(int primitiveID) : base(primitiveID)
        {
        }

        [ResourcePropertyDisplay("Mesh")]
        public Mesh mesh
        {
            get
            {
                return m_mesh;
            }
            set
            {
                m_mesh = value;
                if (value == null)
                    SetMeshToMeshPrimitive(objectID, -1);
                else
                    SetMeshToMeshPrimitive(objectID, value.objectID);
            }
        }

        internal static MeshPrimitive Create()
        {
            RayTracer instance = RayTracer.GetInstance();
            if (!instance.IsInitialized())
                return null;
            int primitiveID = CreateMeshPrimitive();
            if (primitiveID < 0)
                return null;

            MeshPrimitive meshPrimitive = new MeshPrimitive(primitiveID);

            MessageHandler.Broadcast<RayTracerObject>(MessageName.AddNewSceneObject, meshPrimitive);

            return meshPrimitive;
        }

        public override string GetObjectName()
        {
            return "MeshPrimitive_" + objectID;
        }

        public override string GetTypeName()
        {
            return "MeshPrimitive";
        }
    }
}
