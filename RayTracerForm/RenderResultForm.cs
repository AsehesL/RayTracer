using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace RayTracerForm
{
    public partial class RenderResultForm : Form
    {
        private Bitmap m_result;

        public RenderResultForm()
        {
            InitializeComponent();
        }

        public void UpdateResult()
        {
            uint width = GetRayTracingResultWidth();
            uint height = GetRayTracingResultHeight();
            if (width == 0 || height == 0)
                return;
            if (m_result != null && (m_result.Width != width || m_result.Height != height))
            {
                m_result.Dispose();
                m_result = null;
            }
            if (m_result == null)
                m_result = new Bitmap((int)width, (int)height);
            for (int i = 0; i < width; i++)
            {
                for (int j = 0; j < height; j++)
                {
                    float r = 0, g = 0, b = 0, a = 0;
                    GetRayTracingResultPixel(i, j, ref r, ref g, ref b, ref a);
                    r = (float)Math.Min(Math.Max(Math.Pow(r, 0.45), 0.0), 1.0);
                    g = (float)Math.Min(Math.Max(Math.Pow(g, 0.45), 0.0), 1.0);
                    b = (float)Math.Min(Math.Max(Math.Pow(b, 0.45), 0.0), 1.0);
                    a = 1.0f;
                    if (float.IsNaN(r))
                        r = 0.0f;
                    if (float.IsNaN(g))
                        g = 0.0f;
                    if (float.IsNaN(b))
                        b = 0.0f;
                    System.Drawing.Color c = System.Drawing.Color.FromArgb((int)(r * 255.0f),
                        (int)(g * 255.0f), (int)(b * 255.0f));
                    m_result.SetPixel(i, j, c);
                }
            }
            this.BackgroundImage = m_result;
            this.ClientSize = m_result.Size;
            this.MinimumSize = this.MaximumSize = this.Size;
            this.Refresh();
        }

        [DllImport("RayTracerLib.dll")]
        private extern static uint GetRayTracingResultWidth();

        [DllImport("RayTracerLib.dll")]
        private extern static uint GetRayTracingResultHeight();

        [DllImport("RayTracerLib.dll", CallingConvention = CallingConvention.Cdecl)]
        private extern static void GetRayTracingResultPixel(int x, int y, ref float r, ref float g, ref float b, ref float a);


    }
}
