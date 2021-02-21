using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace RayTracerNet
{
    public abstract class ResourceObject : RayTracerObject
    {
        public abstract string GetFullPathName();

        protected ResourceObject(int objectID) : base(objectID)
        { }
    }
}
