using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;

namespace RayTracerNet
{
    public class PlanePrimitive : PrimitiveBase
    {
        [DllImport("RayTracerLib.dll")]
        private extern static int CreatePlanePrimitive();

        private PlanePrimitive(int primitiveID) : base(primitiveID)
        {
        }

        internal static PlanePrimitive Create()
        {
            RayTracer instance = RayTracer.GetInstance();
            if (!instance.IsInitialized())
                return null;
            int primitiveID = CreatePlanePrimitive();
            if (primitiveID < 0)
                return null;

            PlanePrimitive plane = new PlanePrimitive(primitiveID);

            MessageHandler.Broadcast<RayTracerObject>(MessageName.AddNewSceneObject, plane);

            return plane;
        }

        public override string GetObjectName()
        {
            return "Plane_" + objectID;
        }

        public override string GetTypeName()
        {
            return "Plane";
        }
    }
}
