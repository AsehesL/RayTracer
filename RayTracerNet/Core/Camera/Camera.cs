using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;

namespace RayTracerNet
{
    public class Camera : SceneObject
    {

        [DllImport("RayTracerLib.dll")]
        private extern static int CreateCamera();

        [DllImport("RayTracerLib.dll", CallingConvention = CallingConvention.Cdecl)]
        private extern static void SetCameraPosition(int cameraID, double x, double y, double z);

        [DllImport("RayTracerLib.dll", CallingConvention = CallingConvention.Cdecl)]
        private extern static void GetCameraPosition(int cameraID, ref double x, ref double y, ref double z);

        [DllImport("RayTracerLib.dll", CallingConvention = CallingConvention.Cdecl)]
        private extern static void SetCameraEuler(int cameraID, double x, double y, double z);

        [DllImport("RayTracerLib.dll", CallingConvention = CallingConvention.Cdecl)]
        private extern static void GetCameraEuler(int cameraID, ref double x, ref double y, ref double z);

        [DllImport("RayTracerLib.dll", CallingConvention = CallingConvention.Cdecl)]
        private extern static void SetCameraFOV(int cameraID, double fov);

        [DllImport("RayTracerLib.dll", CallingConvention = CallingConvention.Cdecl)]
        private extern static float GetCameraFOV(int cameraID);

        [DllImport("RayTracerLib.dll")]
        private extern static void RefreshCameraTransform(int cameraID);

        [DllImport("RayTracerLib.dll", CallingConvention = CallingConvention.Cdecl)]
        private extern static void GetCameraRight(int cameraID, ref double x, ref double y, ref double z);

        [DllImport("RayTracerLib.dll", CallingConvention = CallingConvention.Cdecl)]
        private extern static void GetCameraUp(int cameraID, ref double x, ref double y, ref double z);

        [DllImport("RayTracerLib.dll", CallingConvention = CallingConvention.Cdecl)]
        private extern static void GetCameraForward(int cameraID, ref double x, ref double y, ref double z);

        [DllImport("RayTracerLib.dll", CallingConvention = CallingConvention.Cdecl)]
        private extern static int GetActiveCamera(int cameraID);

        [DllImport("RayTracerLib.dll", CallingConvention = CallingConvention.Cdecl)]
        private extern static void SetCameraUseThinLens(int cameraID, int useThinLens);

        [DllImport("RayTracerLib.dll", CallingConvention = CallingConvention.Cdecl)]
        private extern static int GetCameraUseThinLens(int cameraID);

        [DllImport("RayTracerLib.dll", CallingConvention = CallingConvention.Cdecl)]
        private extern static void SetCameraFocal(int cameraID, float focal);

        [DllImport("RayTracerLib.dll", CallingConvention = CallingConvention.Cdecl)]
        private extern static float GetCameraFocal(int cameraID);

        [DllImport("RayTracerLib.dll", CallingConvention = CallingConvention.Cdecl)]
        private extern static void SetCameraRadius(int cameraID, float radius);

        [DllImport("RayTracerLib.dll", CallingConvention = CallingConvention.Cdecl)]
        private extern static float GetCameraRadius(int cameraID);

        [DllImport("RayTracerLib.dll", CallingConvention = CallingConvention.Cdecl)]
        private extern static void RemoveCamera(int cameraID);

        private Camera(int cameraID) : base(cameraID)
        {
        }

        public override void Destroy()
        {
            if (objectID < 0)
                return;
            MessageHandler.Broadcast<RayTracerObject>(MessageName.RemoveSceneObject, this);
            RemoveCamera(objectID);
            objectID = -1;
        }

        internal static Camera Create()
        {
            RayTracer instance = RayTracer.GetInstance();
            if (!instance.IsInitialized())
                return null;
            int cameraID = CreateCamera();
            if (cameraID < 0)
                return null;

            Camera camera = new Camera(cameraID);

            MessageHandler.Broadcast<RayTracerObject>(MessageName.AddNewSceneObject, camera);

            return camera;
        }

        public void RefreshAsActive()
        {
            objectID = GetActiveCamera(objectID);
        }

        public void RefreshTransform()
        {
            if (objectID < 0)
                return;
            RefreshCameraTransform(objectID);
        }

        [VectorPropertyDisplay(VectorPropertyType.Vector3, "Position")]
        public override Vector3 position
        {
            get
            {
                if (objectID < 0)
                    return Vector3.zero;
                double x = 0, y = 0, z = 0;
                GetCameraPosition(objectID, ref x, ref y, ref z);
                return new Vector3(x, y, z);
            }
            set
            {
                if (objectID < 0)
                    return;
                SetCameraPosition(objectID, value.x, value.y, value.z);
            }
        }

        [VectorPropertyDisplay(VectorPropertyType.Vector3, "Rotation")]
        public override Vector3 euler
        {
            get
            {
                if (objectID < 0)
                    return Vector3.zero;
                double x = 0, y = 0, z = 0;
                GetCameraEuler(objectID, ref x, ref y, ref z);
                return new Vector3(x, y, z);

            }
            set
            {
                if (objectID < 0)
                    return;
                SetCameraEuler(objectID, value.x, value.y, value.z);
            }
        }

        [NumberPropertyDisplay("FOV", max = 179, min = 0.01f)]
        public double fieldOfView
        {
            get
            {
                if (objectID < 0)
                    return 0;
                return GetCameraFOV(objectID);

            }
            set
            {
                if (objectID < 0)
                    return;
                SetCameraFOV(objectID, value);
            }
        }

        [SwitchPropertyDisplay("UseThinLens")]
        public bool useThinLens
        {
            get
            {
                int v = GetCameraUseThinLens(objectID);
                return v != 0;
            }
            set
            {
                SetCameraUseThinLens(objectID, value ? 1 : 0);
            }
        }

        [NumberPropertyDisplay("Focal")]
        public float focal
        {
            get
            {
                if (objectID < 0)
                    return 0;
                return GetCameraFocal(objectID);

            }
            set
            {
                if (objectID < 0)
                    return;
                SetCameraFocal(objectID, value);
            }
        }

        [NumberPropertyDisplay("Radius")]
        public float radius
        {
            get
            {
                if (objectID < 0)
                    return 0;
                return GetCameraRadius(objectID);

            }
            set
            {
                if (objectID < 0)
                    return;
                SetCameraRadius(objectID, value);
            }
        }

        public Vector3 right
        {
            get
            {
                if (objectID < 0)
                    return Vector3.zero;
                double x = 0, y = 0, z = 0;
                GetCameraRight(objectID, ref x, ref y, ref z);
                return new Vector3(x, y, z);
            }
        }

        public Vector3 up
        {
            get
            {
                if (objectID < 0)
                    return Vector3.zero;
                double x = 0, y = 0, z = 0;
                GetCameraUp(objectID, ref x, ref y, ref z);
                return new Vector3(x, y, z);
            }
        }

        public Vector3 forward
        {
            get
            {
                if (objectID < 0)
                    return Vector3.zero;
                double x = 0, y = 0, z = 0;
                GetCameraForward(objectID, ref x, ref y, ref z);
                return new Vector3(x, y, z);
            }
        }

        public override string GetTypeName()
        {
            return "Camera";
        }

        public override string GetObjectName()
        {
            return "Camera_" + objectID;
        }
    }
}
