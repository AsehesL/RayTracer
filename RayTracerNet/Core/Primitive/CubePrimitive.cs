using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;

namespace RayTracerNet
{
    public class CubePrimitive : PrimitiveBase
    {
        [DllImport("RayTracerLib.dll")]
        private extern static int CreateCubePrimitive();

        private CubePrimitive(int primitiveID) : base(primitiveID)
        {
        }

        internal static CubePrimitive Create()
        {
            RayTracer instance = RayTracer.GetInstance();
            if (!instance.IsInitialized())
                return null;
            int primitiveID = CreateCubePrimitive();
            if (primitiveID < 0)
                return null;

            CubePrimitive cube = new CubePrimitive(primitiveID);

            MessageHandler.Broadcast<RayTracerObject>(MessageName.AddNewSceneObject, cube);

            return cube;
        }

        public override string GetObjectName()
        {
            return "Cube_" + objectID;
        }

        public override string GetTypeName()
        {
            return "Cube";
        }
    }
}
