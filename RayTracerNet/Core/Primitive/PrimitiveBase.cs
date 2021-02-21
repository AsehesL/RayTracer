using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;

namespace RayTracerNet
{
    public abstract class PrimitiveBase : SceneObject
    {

        private MaterialShader m_MaterialShader;

        [DllImport("RayTracerLib.dll", CallingConvention = CallingConvention.Cdecl)]
        private extern static void SetPrimitiveMaterialShader(int primitiveID, int materialShaderID);

        [DllImport("RayTracerLib.dll", CallingConvention = CallingConvention.Cdecl)]
        private extern static void SetPrimitivePosition(int primitiveID, double x, double y, double z);

        [DllImport("RayTracerLib.dll", CallingConvention = CallingConvention.Cdecl)]
        private extern static void GetPrimitivePosition(int primitiveID, ref double x, ref double y, ref double z);

        [DllImport("RayTracerLib.dll", CallingConvention = CallingConvention.Cdecl)]
        private extern static void SetPrimitiveEuler(int primitiveID, double x, double y, double z);

        [DllImport("RayTracerLib.dll", CallingConvention = CallingConvention.Cdecl)]
        private extern static void GetPrimitiveEuler(int primitiveID, ref double x, ref double y, ref double z);

        [DllImport("RayTracerLib.dll", CallingConvention = CallingConvention.Cdecl)]
        private extern static void SetPrimitiveScale(int primitiveID, double x, double y, double z);

        [DllImport("RayTracerLib.dll", CallingConvention = CallingConvention.Cdecl)]
        private extern static void GetPrimitiveScale(int primitiveID, ref double x, ref double y, ref double z);

        [DllImport("RayTracerLib.dll", CallingConvention = CallingConvention.Cdecl)]
        private extern static void SelectPrimitive(int primitiveID);

        [DllImport("RayTracerLib.dll")]
        private extern static void RemovePrimitive(int objectID);

        protected PrimitiveBase(int objectID) : base(objectID)
        {
        }

        public override void Destroy()
        {
            if (objectID < 0)
                return;
            MessageHandler.Broadcast<RayTracerObject>(MessageName.RemoveSceneObject, this);
            RemovePrimitive(objectID);
            objectID = -1;
        }

        [VectorPropertyDisplay(VectorPropertyType.Vector3, "Position")]
        public override Vector3 position
        {
            get
            {
                double x = 0, y = 0, z = 0;
                GetPrimitivePosition(objectID, ref x, ref y, ref z);
                return new Vector3(x, y, z);
            }
            set
            {
                SetPrimitivePosition(objectID, value.x, value.y, value.z);
            }
        }

        [VectorPropertyDisplay(VectorPropertyType.Vector3, "Rotation")]
        public override Vector3 euler
        {
            get
            {
                double x = 0, y = 0, z = 0;
                GetPrimitiveEuler(objectID, ref x, ref y, ref z);
                return new Vector3(x, y, z);

            }
            set
            {
                SetPrimitiveEuler(objectID, value.x, value.y, value.z);
            }
        }

        [VectorPropertyDisplay(VectorPropertyType.Vector3, "Scale")]
        public Vector3 scale
        {
            get
            {
                double x = 0, y = 0, z = 0;
                GetPrimitiveScale(objectID, ref x, ref y, ref z);
                return new Vector3(x, y, z);

            }
            set
            {
                SetPrimitiveScale(objectID, value.x, value.y, value.z);
            }
        }

        [ResourcePropertyDisplay("Material")]
        public MaterialShader material
        {
            get
            {
                return m_MaterialShader;
            }
            set
            {
                m_MaterialShader = value;
                if (m_MaterialShader != null)
                    SetPrimitiveMaterialShader(objectID, m_MaterialShader.objectID);
                else
                    SetPrimitiveMaterialShader(objectID, -1);
            }
        }

        public void Select()
        {
            SelectPrimitive(objectID);
        }
    }
}
