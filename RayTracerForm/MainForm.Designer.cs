namespace RayTracerForm
{
    partial class MainForm
    {
        /// <summary>
        /// 必需的设计器变量。
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 清理所有正在使用的资源。
        /// </summary>
        /// <param name="disposing">如果应释放托管资源，为 true；否则为 false。</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows 窗体设计器生成的代码

        /// <summary>
        /// 设计器支持所需的方法 - 不要修改
        /// 使用代码编辑器修改此方法的内容。
        /// </summary>
        private void InitializeComponent()
        {
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.fileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.createToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.primitiveToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.cubeToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.sphereToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.planeToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.meshToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.lightToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.environmentSkyLightToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
            this.sunLightToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.ResourceToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.materialToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.pbrStandardMaterialToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.emissiveMaterialToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.transparencyMaterialToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.textureToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.meshToolStripMenuItem1 = new System.Windows.Forms.ToolStripMenuItem();
            this.openFileDialog = new System.Windows.Forms.OpenFileDialog();
            this.saveFileDialog1 = new System.Windows.Forms.SaveFileDialog();
            this.mainTabControl = new System.Windows.Forms.TabControl();
            this.tabPage1 = new System.Windows.Forms.TabPage();
            this.worldListView = new System.Windows.Forms.ListView();
            this.columnHeader1 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHeader2 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.tabPage2 = new System.Windows.Forms.TabPage();
            this.resourceListView = new System.Windows.Forms.ListView();
            this.columnHeader3 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHeader4 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHeader5 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.settingTabControl = new System.Windows.Forms.TabControl();
            this.tabPage3 = new System.Windows.Forms.TabPage();
            this.renderSettingPanel = new RayTracerForm.PropertyPanel();
            this.renderButton = new System.Windows.Forms.Button();
            this.tabPage4 = new System.Windows.Forms.TabPage();
            this.primitivePropertyPanel = new RayTracerForm.PropertyPanel();
            this.logListView = new System.Windows.Forms.ListView();
            this.columnHeader6 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHeader7 = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.newToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.openToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.renderToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.beginRenderToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.saveRenderToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.menuStrip1.SuspendLayout();
            this.mainTabControl.SuspendLayout();
            this.tabPage1.SuspendLayout();
            this.tabPage2.SuspendLayout();
            this.settingTabControl.SuspendLayout();
            this.tabPage3.SuspendLayout();
            this.tabPage4.SuspendLayout();
            this.SuspendLayout();
            // 
            // menuStrip1
            // 
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fileToolStripMenuItem,
            this.createToolStripMenuItem,
            this.ResourceToolStripMenuItem,
            this.renderToolStripMenuItem});
            this.menuStrip1.Location = new System.Drawing.Point(0, 0);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.Size = new System.Drawing.Size(1047, 25);
            this.menuStrip1.TabIndex = 0;
            this.menuStrip1.Text = "menuStrip1";
            // 
            // fileToolStripMenuItem
            // 
            this.fileToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.newToolStripMenuItem,
            this.openToolStripMenuItem});
            this.fileToolStripMenuItem.Name = "fileToolStripMenuItem";
            this.fileToolStripMenuItem.Size = new System.Drawing.Size(44, 21);
            this.fileToolStripMenuItem.Text = "文件";
            // 
            // createToolStripMenuItem
            // 
            this.createToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.primitiveToolStripMenuItem,
            this.lightToolStripMenuItem});
            this.createToolStripMenuItem.Name = "createToolStripMenuItem";
            this.createToolStripMenuItem.Size = new System.Drawing.Size(44, 21);
            this.createToolStripMenuItem.Text = "创建";
            // 
            // primitiveToolStripMenuItem
            // 
            this.primitiveToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.cubeToolStripMenuItem,
            this.sphereToolStripMenuItem,
            this.planeToolStripMenuItem,
            this.meshToolStripMenuItem});
            this.primitiveToolStripMenuItem.Name = "primitiveToolStripMenuItem";
            this.primitiveToolStripMenuItem.Size = new System.Drawing.Size(180, 22);
            this.primitiveToolStripMenuItem.Text = "几何体";
            // 
            // cubeToolStripMenuItem
            // 
            this.cubeToolStripMenuItem.Name = "cubeToolStripMenuItem";
            this.cubeToolStripMenuItem.Size = new System.Drawing.Size(112, 22);
            this.cubeToolStripMenuItem.Text = "立方体";
            this.cubeToolStripMenuItem.Click += new System.EventHandler(this.CubePrimitiveToolStripMenuItem_Click);
            // 
            // sphereToolStripMenuItem
            // 
            this.sphereToolStripMenuItem.Name = "sphereToolStripMenuItem";
            this.sphereToolStripMenuItem.Size = new System.Drawing.Size(112, 22);
            this.sphereToolStripMenuItem.Text = "球体";
            this.sphereToolStripMenuItem.Click += new System.EventHandler(this.SpherePrimitiveToolStripMenuItem_Click);
            // 
            // planeToolStripMenuItem
            // 
            this.planeToolStripMenuItem.Name = "planeToolStripMenuItem";
            this.planeToolStripMenuItem.Size = new System.Drawing.Size(112, 22);
            this.planeToolStripMenuItem.Text = "平面";
            this.planeToolStripMenuItem.Click += new System.EventHandler(this.PlanePrimitiveToolStripMenuItem_Click);
            // 
            // meshToolStripMenuItem
            // 
            this.meshToolStripMenuItem.Name = "meshToolStripMenuItem";
            this.meshToolStripMenuItem.Size = new System.Drawing.Size(112, 22);
            this.meshToolStripMenuItem.Text = "网格";
            this.meshToolStripMenuItem.Click += new System.EventHandler(this.MeshPrimitiveToolStripMenuItem_Click);
            // 
            // lightToolStripMenuItem
            // 
            this.lightToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.environmentSkyLightToolStripMenuItem,
            this.toolStripSeparator1,
            this.sunLightToolStripMenuItem});
            this.lightToolStripMenuItem.Name = "lightToolStripMenuItem";
            this.lightToolStripMenuItem.Size = new System.Drawing.Size(180, 22);
            this.lightToolStripMenuItem.Text = "光照";
            // 
            // environmentSkyLightToolStripMenuItem
            // 
            this.environmentSkyLightToolStripMenuItem.Name = "environmentSkyLightToolStripMenuItem";
            this.environmentSkyLightToolStripMenuItem.Size = new System.Drawing.Size(180, 22);
            this.environmentSkyLightToolStripMenuItem.Text = "环境反射";
            this.environmentSkyLightToolStripMenuItem.Click += new System.EventHandler(this.EnvironmentSkyLightToolStripMenuItem_Click);
            // 
            // toolStripSeparator1
            // 
            this.toolStripSeparator1.Name = "toolStripSeparator1";
            this.toolStripSeparator1.Size = new System.Drawing.Size(177, 6);
            // 
            // sunLightToolStripMenuItem
            // 
            this.sunLightToolStripMenuItem.Name = "sunLightToolStripMenuItem";
            this.sunLightToolStripMenuItem.Size = new System.Drawing.Size(180, 22);
            this.sunLightToolStripMenuItem.Text = "太阳光";
            this.sunLightToolStripMenuItem.Click += new System.EventHandler(this.SunLightToolStripMenuItem_Click);
            // 
            // ResourceToolStripMenuItem
            // 
            this.ResourceToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.materialToolStripMenuItem,
            this.textureToolStripMenuItem,
            this.meshToolStripMenuItem1});
            this.ResourceToolStripMenuItem.Name = "ResourceToolStripMenuItem";
            this.ResourceToolStripMenuItem.Size = new System.Drawing.Size(44, 21);
            this.ResourceToolStripMenuItem.Text = "资产";
            // 
            // materialToolStripMenuItem
            // 
            this.materialToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.pbrStandardMaterialToolStripMenuItem,
            this.emissiveMaterialToolStripMenuItem,
            this.transparencyMaterialToolStripMenuItem});
            this.materialToolStripMenuItem.Name = "materialToolStripMenuItem";
            this.materialToolStripMenuItem.Size = new System.Drawing.Size(180, 22);
            this.materialToolStripMenuItem.Text = "材质";
            // 
            // pbrStandardMaterialToolStripMenuItem
            // 
            this.pbrStandardMaterialToolStripMenuItem.Name = "pbrStandardMaterialToolStripMenuItem";
            this.pbrStandardMaterialToolStripMenuItem.Size = new System.Drawing.Size(147, 22);
            this.pbrStandardMaterialToolStripMenuItem.Text = "标准PBR材质";
            this.pbrStandardMaterialToolStripMenuItem.Click += new System.EventHandler(this.PBRStandardToolStripMenuItem_Click);
            // 
            // emissiveMaterialToolStripMenuItem
            // 
            this.emissiveMaterialToolStripMenuItem.Name = "emissiveMaterialToolStripMenuItem";
            this.emissiveMaterialToolStripMenuItem.Size = new System.Drawing.Size(147, 22);
            this.emissiveMaterialToolStripMenuItem.Text = "自发光材质";
            this.emissiveMaterialToolStripMenuItem.Click += new System.EventHandler(this.EmissiveMaterialToolStripMenuItem_Click);
            // 
            // transparencyMaterialToolStripMenuItem
            // 
            this.transparencyMaterialToolStripMenuItem.Name = "transparencyMaterialToolStripMenuItem";
            this.transparencyMaterialToolStripMenuItem.Size = new System.Drawing.Size(147, 22);
            this.transparencyMaterialToolStripMenuItem.Text = "透明材质";
            this.transparencyMaterialToolStripMenuItem.Click += new System.EventHandler(this.TransparencyMaterialToolStripMenuItem_Click);
            // 
            // textureToolStripMenuItem
            // 
            this.textureToolStripMenuItem.Name = "textureToolStripMenuItem";
            this.textureToolStripMenuItem.Size = new System.Drawing.Size(180, 22);
            this.textureToolStripMenuItem.Text = "纹理...";
            this.textureToolStripMenuItem.Click += new System.EventHandler(this.TextureToolStripMenuItem_Click);
            // 
            // meshToolStripMenuItem1
            // 
            this.meshToolStripMenuItem1.Name = "meshToolStripMenuItem1";
            this.meshToolStripMenuItem1.Size = new System.Drawing.Size(180, 22);
            this.meshToolStripMenuItem1.Text = "网格...";
            this.meshToolStripMenuItem1.Click += new System.EventHandler(this.MeshToolStripMenuItem_Click);
            // 
            // openFileDialog
            // 
            this.openFileDialog.FileName = "openFileDialog1";
            this.openFileDialog.Filter = "Obj文件|*.obj";
            // 
            // mainTabControl
            // 
            this.mainTabControl.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.mainTabControl.Controls.Add(this.tabPage1);
            this.mainTabControl.Controls.Add(this.tabPage2);
            this.mainTabControl.Location = new System.Drawing.Point(0, 28);
            this.mainTabControl.Name = "mainTabControl";
            this.mainTabControl.SelectedIndex = 0;
            this.mainTabControl.Size = new System.Drawing.Size(200, 565);
            this.mainTabControl.TabIndex = 1;
            // 
            // tabPage1
            // 
            this.tabPage1.Controls.Add(this.worldListView);
            this.tabPage1.Location = new System.Drawing.Point(4, 22);
            this.tabPage1.Name = "tabPage1";
            this.tabPage1.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage1.Size = new System.Drawing.Size(192, 539);
            this.tabPage1.TabIndex = 0;
            this.tabPage1.Text = "世界";
            this.tabPage1.UseVisualStyleBackColor = true;
            // 
            // worldListView
            // 
            this.worldListView.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeader1,
            this.columnHeader2});
            this.worldListView.FullRowSelect = true;
            this.worldListView.HideSelection = false;
            this.worldListView.Location = new System.Drawing.Point(3, 3);
            this.worldListView.MultiSelect = false;
            this.worldListView.Name = "worldListView";
            this.worldListView.Size = new System.Drawing.Size(186, 536);
            this.worldListView.TabIndex = 0;
            this.worldListView.UseCompatibleStateImageBehavior = false;
            this.worldListView.View = System.Windows.Forms.View.Details;
            this.worldListView.SelectedIndexChanged += new System.EventHandler(this.WorldListView_SelectedIndexChanged);
            this.worldListView.KeyDown += new System.Windows.Forms.KeyEventHandler(this.WorldListView_KeyDown);
            // 
            // columnHeader1
            // 
            this.columnHeader1.Text = "类型";
            // 
            // columnHeader2
            // 
            this.columnHeader2.Text = "名称";
            this.columnHeader2.Width = 112;
            // 
            // tabPage2
            // 
            this.tabPage2.Controls.Add(this.resourceListView);
            this.tabPage2.Location = new System.Drawing.Point(4, 22);
            this.tabPage2.Name = "tabPage2";
            this.tabPage2.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage2.Size = new System.Drawing.Size(192, 539);
            this.tabPage2.TabIndex = 1;
            this.tabPage2.Text = "资产";
            this.tabPage2.UseVisualStyleBackColor = true;
            // 
            // resourceListView
            // 
            this.resourceListView.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeader3,
            this.columnHeader4,
            this.columnHeader5});
            this.resourceListView.FullRowSelect = true;
            this.resourceListView.HideSelection = false;
            this.resourceListView.Location = new System.Drawing.Point(3, 3);
            this.resourceListView.MultiSelect = false;
            this.resourceListView.Name = "resourceListView";
            this.resourceListView.Size = new System.Drawing.Size(186, 536);
            this.resourceListView.TabIndex = 0;
            this.resourceListView.UseCompatibleStateImageBehavior = false;
            this.resourceListView.View = System.Windows.Forms.View.Details;
            this.resourceListView.SelectedIndexChanged += new System.EventHandler(this.ResourceListView_SelectedIndexChanged);
            // 
            // columnHeader3
            // 
            this.columnHeader3.Text = "类型";
            // 
            // columnHeader4
            // 
            this.columnHeader4.Text = "名称";
            // 
            // columnHeader5
            // 
            this.columnHeader5.Text = "路径";
            // 
            // settingTabControl
            // 
            this.settingTabControl.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.settingTabControl.Controls.Add(this.tabPage3);
            this.settingTabControl.Controls.Add(this.tabPage4);
            this.settingTabControl.Location = new System.Drawing.Point(788, 28);
            this.settingTabControl.Name = "settingTabControl";
            this.settingTabControl.SelectedIndex = 0;
            this.settingTabControl.Size = new System.Drawing.Size(259, 406);
            this.settingTabControl.TabIndex = 2;
            // 
            // tabPage3
            // 
            this.tabPage3.Controls.Add(this.renderSettingPanel);
            this.tabPage3.Controls.Add(this.renderButton);
            this.tabPage3.Location = new System.Drawing.Point(4, 22);
            this.tabPage3.Name = "tabPage3";
            this.tabPage3.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage3.Size = new System.Drawing.Size(251, 380);
            this.tabPage3.TabIndex = 0;
            this.tabPage3.Text = "渲染设置";
            this.tabPage3.UseVisualStyleBackColor = true;
            // 
            // renderSettingPanel
            // 
            this.renderSettingPanel.AutoScroll = true;
            this.renderSettingPanel.Location = new System.Drawing.Point(6, 6);
            this.renderSettingPanel.Name = "renderSettingPanel";
            this.renderSettingPanel.Size = new System.Drawing.Size(239, 339);
            this.renderSettingPanel.TabIndex = 0;
            // 
            // renderButton
            // 
            this.renderButton.Location = new System.Drawing.Point(83, 351);
            this.renderButton.Name = "renderButton";
            this.renderButton.Size = new System.Drawing.Size(75, 23);
            this.renderButton.TabIndex = 7;
            this.renderButton.Text = "渲染";
            this.renderButton.UseVisualStyleBackColor = true;
            this.renderButton.Click += new System.EventHandler(this.RenderButton_Click);
            // 
            // tabPage4
            // 
            this.tabPage4.Controls.Add(this.primitivePropertyPanel);
            this.tabPage4.Location = new System.Drawing.Point(4, 22);
            this.tabPage4.Name = "tabPage4";
            this.tabPage4.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage4.Size = new System.Drawing.Size(251, 380);
            this.tabPage4.TabIndex = 1;
            this.tabPage4.Text = "属性";
            this.tabPage4.UseVisualStyleBackColor = true;
            // 
            // primitivePropertyPanel
            // 
            this.primitivePropertyPanel.AutoScroll = true;
            this.primitivePropertyPanel.Location = new System.Drawing.Point(6, 6);
            this.primitivePropertyPanel.Name = "primitivePropertyPanel";
            this.primitivePropertyPanel.Size = new System.Drawing.Size(239, 339);
            this.primitivePropertyPanel.TabIndex = 0;
            this.primitivePropertyPanel.Visible = false;
            // 
            // logListView
            // 
            this.logListView.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.logListView.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeader6,
            this.columnHeader7});
            this.logListView.FullRowSelect = true;
            this.logListView.HeaderStyle = System.Windows.Forms.ColumnHeaderStyle.Nonclickable;
            this.logListView.HideSelection = false;
            this.logListView.Location = new System.Drawing.Point(202, 437);
            this.logListView.MultiSelect = false;
            this.logListView.Name = "logListView";
            this.logListView.Size = new System.Drawing.Size(840, 155);
            this.logListView.TabIndex = 5;
            this.logListView.UseCompatibleStateImageBehavior = false;
            this.logListView.View = System.Windows.Forms.View.Details;
            // 
            // columnHeader6
            // 
            this.columnHeader6.Text = "日志类型";
            this.columnHeader6.Width = 117;
            // 
            // columnHeader7
            // 
            this.columnHeader7.Text = "说明";
            this.columnHeader7.Width = 535;
            // 
            // newToolStripMenuItem
            // 
            this.newToolStripMenuItem.Name = "newToolStripMenuItem";
            this.newToolStripMenuItem.Size = new System.Drawing.Size(180, 22);
            this.newToolStripMenuItem.Text = "新建";
            this.newToolStripMenuItem.Click += new System.EventHandler(this.NewToolStripMenuItem_Click);
            // 
            // openToolStripMenuItem
            // 
            this.openToolStripMenuItem.Name = "openToolStripMenuItem";
            this.openToolStripMenuItem.Size = new System.Drawing.Size(180, 22);
            this.openToolStripMenuItem.Text = "打开...";
            this.openToolStripMenuItem.Click += new System.EventHandler(this.OpenToolStripMenuItem_Click);
            // 
            // renderToolStripMenuItem
            // 
            this.renderToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.beginRenderToolStripMenuItem,
            this.saveRenderToolStripMenuItem});
            this.renderToolStripMenuItem.Name = "renderToolStripMenuItem";
            this.renderToolStripMenuItem.Size = new System.Drawing.Size(44, 21);
            this.renderToolStripMenuItem.Text = "渲染";
            // 
            // beginRenderToolStripMenuItem
            // 
            this.beginRenderToolStripMenuItem.Name = "beginRenderToolStripMenuItem";
            this.beginRenderToolStripMenuItem.Size = new System.Drawing.Size(180, 22);
            this.beginRenderToolStripMenuItem.Text = "开始渲染";
            // 
            // saveRenderToolStripMenuItem
            // 
            this.saveRenderToolStripMenuItem.Name = "saveRenderToolStripMenuItem";
            this.saveRenderToolStripMenuItem.Size = new System.Drawing.Size(180, 22);
            this.saveRenderToolStripMenuItem.Text = "保存渲染结果...";
            // 
            // MainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1047, 595);
            this.Controls.Add(this.logListView);
            this.Controls.Add(this.settingTabControl);
            this.Controls.Add(this.mainTabControl);
            this.Controls.Add(this.menuStrip1);
            this.MainMenuStrip = this.menuStrip1;
            this.Name = "MainForm";
            this.Text = "Form1";
            this.Load += new System.EventHandler(this.MainForm_Load);
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            this.mainTabControl.ResumeLayout(false);
            this.tabPage1.ResumeLayout(false);
            this.tabPage2.ResumeLayout(false);
            this.settingTabControl.ResumeLayout(false);
            this.tabPage3.ResumeLayout(false);
            this.tabPage4.ResumeLayout(false);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        private System.Windows.Forms.MenuStrip menuStrip1;
        private System.Windows.Forms.OpenFileDialog openFileDialog;
        private System.Windows.Forms.SaveFileDialog saveFileDialog1;
        private System.Windows.Forms.ToolStripMenuItem fileToolStripMenuItem;
        private System.Windows.Forms.TabControl mainTabControl;
        private System.Windows.Forms.TabPage tabPage1;
        private System.Windows.Forms.TabPage tabPage2;
        private System.Windows.Forms.TabControl settingTabControl;
        private System.Windows.Forms.TabPage tabPage3;
        private System.Windows.Forms.TabPage tabPage4;
        private System.Windows.Forms.ListView logListView;
        private System.Windows.Forms.ListView worldListView;
        private System.Windows.Forms.ListView resourceListView;
        private System.Windows.Forms.ColumnHeader columnHeader1;
        private System.Windows.Forms.ColumnHeader columnHeader2;
        private PropertyPanel primitivePropertyPanel;
        private PropertyPanel renderSettingPanel;
        private System.Windows.Forms.ToolStripMenuItem createToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem primitiveToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem cubeToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem sphereToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem planeToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem meshToolStripMenuItem;

        #endregion

        private System.Windows.Forms.ColumnHeader columnHeader3;
        private System.Windows.Forms.ColumnHeader columnHeader4;
        private System.Windows.Forms.ColumnHeader columnHeader5;
        private System.Windows.Forms.ToolStripMenuItem ResourceToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem materialToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem textureToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem meshToolStripMenuItem1;
        private System.Windows.Forms.ToolStripMenuItem pbrStandardMaterialToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem emissiveMaterialToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem transparencyMaterialToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem lightToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem environmentSkyLightToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
        private System.Windows.Forms.ToolStripMenuItem sunLightToolStripMenuItem;
        private System.Windows.Forms.ColumnHeader columnHeader6;
        private System.Windows.Forms.ColumnHeader columnHeader7;
        private System.Windows.Forms.Button renderButton;
        private System.Windows.Forms.ToolStripMenuItem newToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem openToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem renderToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem beginRenderToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem saveRenderToolStripMenuItem;
    }
}

