using RayTracerNet;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace RayTracerForm
{
    public partial class MainForm : Form
    {
        private RayTracerComponent m_raytracerComponent;
        private RenderResultForm m_resultForm;

        public MainForm()
        {
            InitializeComponent();
        }

        private void MainForm_Load(object sender, EventArgs e)
        {
            RayTracerNet.MessageHandler.AddListener<RayTracerObject>(MessageName.AddNewSceneObject, this.OnAddNewSceneObject);
            RayTracerNet.MessageHandler.AddListener<RayTracerNet.ResourceObject>(MessageName.AddNewResource, this.OnAddNewResource);
            RayTracerNet.MessageHandler.AddListener<RayTracerObject>(MessageName.RemoveSceneObject, this.OnRemoveNewSceneObject);
            RayTracerNet.MessageHandler.AddListener<RayTracerNet.ResourceObject>(MessageName.RemoveResource, this.OnRemoveResource);
            RayTracerNet.MessageHandler.AddListener<RayTracerNet.LogItem>(MessageName.AddLog, this.OnAddNewLog);
            RayTracerNet.MessageHandler.AddListener(MessageName.ClearLogs, this.OnClearLogs);
            RayTracerNet.MessageHandler.AddListener<RayTracerNet.IntegratorType>(MessageName.ChangeIntegratorType, this.OnChangeIntegratorType);

            RayTracerNet.MessageHandler.AddListener<RayTracerObject>(MessageName.SelectSceneObject, this.OnSelectSceneObject);

            this.m_raytracerComponent = new RayTracerComponent();
            this.m_raytracerComponent.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.m_raytracerComponent.Location = new System.Drawing.Point(202, 28);
            this.m_raytracerComponent.Size = new System.Drawing.Size(584, 402);
            this.m_raytracerComponent.BorderStyle = BorderStyle.Fixed3D;
            this.Controls.Add(this.m_raytracerComponent);

            var renderSettings = RayTracerNet.RayTracer.GetInstance().renderSettings;
            this.renderSettingPanel.SetDisplayObject(renderSettings);
        }

        private void OnAddNewSceneObject(RayTracerNet.RayTracerObject newObject)
        {
            this.worldListView.Items.Add(new WorldListViewItem(newObject));
        }

        private void OnRemoveNewSceneObject(RayTracerNet.RayTracerObject rtObject)
        {
            int i = 0;
            while (i < this.worldListView.Items.Count)
            {
                WorldListViewItem item = this.worldListView.Items[i] as WorldListViewItem;
                if (item != null && item.rtObject == rtObject)
                {
                    this.worldListView.Items.RemoveAt(i);
                }
                else
                {
                    i++;
                }
            }
        }

        private void OnSelectSceneObject(RayTracerNet.RayTracerObject selectedObject)
        {
            ActivePropertyPanel(selectedObject);
        }

        private void OnAddNewResource(RayTracerNet.ResourceObject resourceObject)
        {
            this.resourceListView.Items.Add(new ResourceListViewItem(resourceObject));
        }

        private void OnRemoveResource(RayTracerNet.ResourceObject resourceObject)
        {
            int i = 0;
            while (i < this.resourceListView.Items.Count)
            {
                ResourceListViewItem item = this.resourceListView.Items[i] as ResourceListViewItem;
                if (item != null && item.resObject == resourceObject)
                {
                    this.resourceListView.Items.RemoveAt(i);
                }
                else
                {
                    i++;
                }
            }
        }


        private void OnAddNewLog(RayTracerNet.LogItem logItem)
        {
            ListViewItem item = new ListViewItem();
            item.Text = logItem.type.ToString();
            item.SubItems.Add(logItem.message);
            item.ForeColor = logItem.GetFontColor().ToSystemColor();
            item.BackColor = logItem.GetBackgroundColor().ToSystemColor();

            this.logListView.Items.Add(item);
        }

        private void OnClearLogs()
        {
            this.logListView.Items.Clear();
        }

        private void OnChangeIntegratorType(RayTracerNet.IntegratorType type)
        {
            var renderSettings = RayTracerNet.RayTracer.GetInstance().renderSettings;
            this.renderSettingPanel.SetDisplayObject(renderSettings);
        }

        private void ActivePropertyPanel(RayTracerNet.RayTracerObject rtObject)
        {
            if (rtObject != null)
            {
                this.primitivePropertyPanel.Visible = true;

                this.primitivePropertyPanel.SetDisplayObject(rtObject);
            }
            else
                this.primitivePropertyPanel.Visible = false;
        }

        private void WorldListView_SelectedIndexChanged(object sender, EventArgs e)
        {
            ListView listView = sender as ListView;
            if (listView != null && listView.SelectedItems.Count > 0)
            {
                WorldListViewItem selectItem = listView.SelectedItems[0] as WorldListViewItem;
                if (selectItem != null && selectItem.rtObject != null)
                {
                    if (selectItem.rtObject is PrimitiveBase)
                    {
                        PrimitiveBase primitive = selectItem.rtObject as PrimitiveBase;
                        if (primitive != null)
                            primitive.Select();
                    }
                    ActivePropertyPanel(selectItem.rtObject);
                }
            }
        }

        private void WorldListView_KeyDown(object sender, KeyEventArgs e)
        {
            if (Keys.Delete == e.KeyCode)
            {
                ListView listView = sender as ListView;
                if (listView.SelectedItems.Count > 0)
                {
                    WorldListViewItem item = listView.SelectedItems[0] as WorldListViewItem;
                    if (item != null && item.rtObject != null)
                    {
                        var rtObj = item.rtObject;
                        listView.Items.Remove(item);
                        rtObj.Destroy();
                    }
                }
            }
        }

        private void ResourceListView_SelectedIndexChanged(object sender, EventArgs e)
        {
            ListView listView = sender as ListView;
            if (listView != null && listView.SelectedItems.Count > 0)
            {
                ResourceListViewItem selectItem = listView.SelectedItems[0] as ResourceListViewItem;
                if (selectItem != null && selectItem.resObject != null)
                {
                    ActivePropertyPanel(selectItem.resObject);
                }
            }
        }

        private void CubePrimitiveToolStripMenuItem_Click(object sender, EventArgs e)
        {
            RayTracerNet.CubePrimitive cube = RayTracerNet.CubePrimitive.Create();
            cube.position = Vector3.zero;
            cube.euler = Vector3.zero;
            cube.scale = Vector3.one;
        }

        private void SpherePrimitiveToolStripMenuItem_Click(object sender, EventArgs e)
        {
            RayTracerNet.SpherePrimitive sphere = RayTracerNet.SpherePrimitive.Create();
            sphere.position = Vector3.zero;
            sphere.euler = Vector3.zero;
            sphere.scale = Vector3.one;
        }

        private void PlanePrimitiveToolStripMenuItem_Click(object sender, EventArgs e)
        {
            RayTracerNet.PlanePrimitive plane = RayTracerNet.PlanePrimitive.Create();
            plane.position = Vector3.zero;
            plane.euler = Vector3.zero;
            plane.scale = Vector3.one;
        }

        private void MeshPrimitiveToolStripMenuItem_Click(object sender, EventArgs e)
        {
            RayTracerNet.MeshPrimitive meshPrimitive = RayTracerNet.MeshPrimitive.Create();
            meshPrimitive.position = Vector3.zero;
            meshPrimitive.euler = Vector3.zero;
            meshPrimitive.scale = Vector3.one;
        }

        private void MeshToolStripMenuItem_Click(object sender, EventArgs e)
        {
            openFileDialog.Filter = "Obj文件|*.obj|Mesh文件|*.mesh";
            DialogResult result = openFileDialog.ShowDialog();
            if (result == DialogResult.OK)
            {
                Mesh.CreateFromFile(openFileDialog.FileName);
            }
        }

        private void TextureToolStripMenuItem_Click(object sender, EventArgs e)
        {
            openFileDialog.Filter = "所有文件|*.*";
            DialogResult result = openFileDialog.ShowDialog();
            if (result == DialogResult.OK)
            {
                Texture tex = Texture.CreateFromFile(openFileDialog.FileName);
            }
        }

        private void PBRStandardToolStripMenuItem_Click(object sender, EventArgs e)
        {
            RayTracerNet.PBR_StandardShader pbrStandardShader = RayTracerNet.PBR_StandardShader.Create();
        }

        private void EmissiveMaterialToolStripMenuItem_Click(object sender, EventArgs e)
        {
            RayTracerNet.EmissiveShader.Create();
        }

        private void TransparencyMaterialToolStripMenuItem_Click(object sender, EventArgs e)
        {
            RayTracerNet.TransparencyShader.Create();
        }

        private void EnvironmentSkyLightToolStripMenuItem_Click(object sender, EventArgs e)
        {
            RayTracerNet.EnvironmentMapSkyLight.Create();
        }

        private void SunLightToolStripMenuItem_Click(object sender, EventArgs e)
        {
            RayTracerNet.SunLight.Create();
        }

        private void NewToolStripMenuItem_Click(object sender, EventArgs e)
        {
            RayTracer.GetInstance().GetScene().Create();
        }

        private void OpenToolStripMenuItem_Click(object sender, EventArgs e)
        {
            openFileDialog.Filter = "场景文件|*.scene";
            DialogResult result = openFileDialog.ShowDialog();
            if (result == DialogResult.OK)
            {
                RayTracer.GetInstance().GetScene().CreateFromFile(openFileDialog.FileName);
            }
        }

        private void BeginRenderToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (RayTracerNet.RayTracer.GetInstance().StartRayTracingRender())
            {
                if (m_resultForm == null)
                    m_resultForm = new RenderResultForm();
                if (m_resultForm.IsDisposed)
                    m_resultForm = new RenderResultForm();
                m_resultForm.Show();
                m_resultForm.Focus();
                m_resultForm.UpdateResult();
            }
        }

        private void SaveRenderToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (RayTracerNet.RayTracer.GetInstance().HasRayTracingResult())
            {
                saveFileDialog.Filter = "JPEG文件|*.jpg|BMP文件|*.bmp|PNG文件|*.png|HDR文件|*.hdr";
                DialogResult result = saveFileDialog.ShowDialog();
                if (result == DialogResult.OK)
                {
                    System.IO.FileInfo fileInfo = new System.IO.FileInfo(saveFileDialog.FileName);
                    bool isHDR = false;
                    if (fileInfo.Extension.ToLower().Contains(".hdr"))
                    {
                        isHDR = true;
                    }
                    RayTracerNet.RayTracer.GetInstance().SaveRayTracingResult(saveFileDialog.FileName, isHDR);
                }
            }
        }

        private void PreviewToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (m_resultForm == null)
                m_resultForm = new RenderResultForm();
            if (m_resultForm.IsDisposed)
                m_resultForm = new RenderResultForm();
            m_resultForm.Show();
            m_resultForm.Focus();
            m_resultForm.UpdateResult();
        }

        private void SaveToolStripMenuItem_Click(object sender, EventArgs e)
        {
            saveFileDialog.Filter = "场景文件|*.scene";
            DialogResult result = saveFileDialog.ShowDialog();
            if (result == DialogResult.OK)
            {
                //RayTracer.GetInstance().GetScene().Save(saveFileDialog.FileName);
            }
        }
    }
}
