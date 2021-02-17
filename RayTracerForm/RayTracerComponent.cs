using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using RayTracerNet;

namespace RayTracerForm
{
    public partial class RayTracerComponent : UserControl
    {
        private class Controller
        {
            private const float kMouseMoveSpeed = 0.02f;
            private const float kMouseRotateSpeed = 0.5f;
            private const float kMouseScallingSpeed = 0.001f;

            private bool m_isMouseHandleMoving;
            private bool m_isMouseHandleRotating;

            private int m_mousePosX;
            private int m_mousePosY;

            public bool enable;

            public Controller()
            {
                m_isMouseHandleMoving = false;
                m_isMouseHandleRotating = false;
                enable = true;
            }

            public void BeginMove(MouseEventArgs e)
            {
                if (!enable)
                    return;
                if (e.Button == MouseButtons.Middle)
                {
                    m_isMouseHandleMoving = true;
                    m_mousePosX = e.X;
                    m_mousePosY = e.Y;
                }
                else if (e.Button == MouseButtons.Right)
                {
                    m_isMouseHandleRotating = true;
                    m_mousePosX = e.X;
                    m_mousePosY = e.Y;
                }
            }

            public void Moving(MouseEventArgs e)
            {
                if (!enable)
                    return;
                Camera camera = RayTracer.GetInstance().GetScene().GetActiveCamera();
                if (m_isMouseHandleMoving)
                {
                    int deltaX = e.X - m_mousePosX;
                    int deltaY = e.Y - m_mousePosY;
                    Vector3 right = camera.right;
                    Vector3 up = camera.up;
                    Vector3 pos = camera.position;
                    pos = pos + up * kMouseMoveSpeed * deltaY - right * kMouseMoveSpeed * deltaX;
                    camera.position = pos;

                    m_mousePosX = e.X;
                    m_mousePosY = e.Y;
                }
                else if (m_isMouseHandleRotating)
                {
                    int deltaX = e.X - m_mousePosX;
                    int deltaY = e.Y - m_mousePosY;

                    Vector3 euler = camera.euler;
                    euler.x -= kMouseRotateSpeed * deltaY;
                    euler.y += kMouseRotateSpeed * deltaX;
                    camera.euler = euler;

                    m_mousePosX = e.X;
                    m_mousePosY = e.Y;
                }
            }

            public void EndMove(MouseEventArgs e)
            {
                if (!enable)
                    return;
                if (e.Button == MouseButtons.Middle)
                    m_isMouseHandleMoving = false;
                if (e.Button == MouseButtons.Right)
                    m_isMouseHandleRotating = false;
            }

            public void Scalling(MouseEventArgs e)
            {
                if (!enable)
                    return;
                if (e.Button == MouseButtons.None)
                {
                    Camera camera = RayTracer.GetInstance().GetScene().GetActiveCamera();
                    Vector3 forward = camera.forward;
                    Vector3 pos = camera.position;
                    pos += forward * kMouseScallingSpeed * e.Delta;
                    camera.position = pos;
                }
            }
        }

        private Timer m_Timer = new Timer();

        private Controller m_controller;

        public RayTracerComponent()
        {
            InitializeComponent();

            m_Timer.Tick += new EventHandler(OnUpdateFrame);
            m_Timer.Interval = 17;

            m_controller = new Controller();
        }

        public void Play()
        {
            m_Timer.Start();
        }

        public void Stop()
        {
            m_Timer.Stop();
        }

        public bool controllerEnable
        {
            get
            {
                return m_controller.enable;
            }
            set
            {
                m_controller.enable = value;
            }
        }

        private void OnUpdateFrame(object sender, EventArgs e)
        {
            RayTracerNet.RayTracer instance = RayTracerNet.RayTracer.GetInstance();
            if (instance != null)
                instance.UpdateRayTracerRenderer();
        }

        private void OnMouseDown(object sender, MouseEventArgs e)
        {
            m_controller.BeginMove(e);
        }

        private void OnMouseMove(object sender, MouseEventArgs e)
        {
            m_controller.Moving(e);
        }

        private void OnMouseUp(object sender, MouseEventArgs e)
        {
            m_controller.EndMove(e);
        }

        private void OnMouseWheel(object sender, MouseEventArgs e)
        {
            m_controller.Scalling(e);
        }

        private void OnMouseClick(object sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Left)
            {
                int x = e.X;
                int y = e.Y;
                RayTracerNet.RayTracer instance = RayTracerNet.RayTracer.GetInstance();
            }
        }

        private void RayTracerComponent_Load(object sender, EventArgs e)
        {
            RayTracerNet.RayTracer instance = RayTracerNet.RayTracer.GetInstance();
            if (instance != null)
            {
                if (instance.InitRayTracer(Handle, Width, Height))
                {
                    m_Timer.Start();
                }
            }
        }
    }
}
