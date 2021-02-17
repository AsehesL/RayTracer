using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace RayTracerForm
{

    class ResourceListViewItem : ListViewItem
    {
        public RayTracerNet.ResourceObject resObject;

        public ResourceListViewItem(RayTracerNet.ResourceObject resObject)
        {
            this.resObject = resObject;

            this.Text = resObject.GetTypeName();

            this.SubItems.Add(resObject.GetObjectName());
            this.SubItems.Add(resObject.GetFullPathName());
        }
    }
}
