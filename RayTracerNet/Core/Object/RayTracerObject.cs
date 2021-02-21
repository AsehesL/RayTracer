using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace RayTracerNet
{
    public abstract class RayTracerObject
    {
        public abstract string GetTypeName();

        public abstract string GetObjectName();

        public abstract void Destroy();

        public int objectID
        {
            get;
            protected set;
        }

        protected RayTracerObject(int objectID)
        {
            this.objectID = objectID;
        }
    }
}
