using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace RayTracerNet
{
    public class SpherePrimitive : PrimitiveBase
    {
        [DllImport("RayTracerLib.dll")]
        private extern static int CreateSpherePrimitive();

        private SpherePrimitive(int primitiveID) : base(primitiveID)
        {
        }

        internal static SpherePrimitive Create()
        {
            RayTracer instance = RayTracer.GetInstance();
            if (!instance.IsInitialized())
                return null;
            int primitiveID = CreateSpherePrimitive();
            if (primitiveID < 0)
                return null;

            SpherePrimitive sphere = new SpherePrimitive(primitiveID);

            MessageHandler.Broadcast<RayTracerObject>(MessageName.AddNewSceneObject, sphere);

            return sphere;
        }

        public override string GetObjectName()
        {
            return "Sphere_" + objectID;
        }

        public override string GetTypeName()
        {
            return "Sphere";
        }
    }
}
