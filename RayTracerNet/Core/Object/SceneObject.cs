using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace RayTracerNet
{
    public abstract class SceneObject : RayTracerObject
    {
        public abstract Vector3 position
        {
            get;set;
        }

        public abstract Vector3 euler
        {
            get;set;
        }

        protected SceneObject(int objectID) : base(objectID)
        {

        }
    }
}
