using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace RayTracerForm
{
    class WorldListViewItem : ListViewItem
    {
        public RayTracerNet.RayTracerObject rtObject;

        public WorldListViewItem(RayTracerNet.RayTracerObject rtObject)
        {
            this.rtObject = rtObject;

            this.Text = rtObject.GetTypeName();

            this.SubItems.Add(rtObject.GetObjectName());
        }
    }
}
