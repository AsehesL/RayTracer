using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Reflection;
using RayTracerNet;

namespace RayTracerForm
{
    public partial class PropertyPanel : Panel
    {
        private class PropertyPool<T> where T : PropertyControl
        {
            private Stack<T> m_PropertyPool = new Stack<T>();
            private Stack<T> m_usingProperties = new Stack<T>();

            private PropertyPanel m_panel;

            public PropertyPool(PropertyPanel panel)
            {
                m_panel = panel;
            }

            public int AddProperty(System.Object propertyTarget, System.Attribute attribute, System.Reflection.PropertyInfo propertyInfo, int uiPosition)
            {
                T propertyControl = null;
                if (m_PropertyPool.Count > 0)
                {
                    propertyControl = m_PropertyPool.Pop();
                }
                else
                    propertyControl = System.Activator.CreateInstance(typeof(T), new object[] { m_panel }) as T;
                uiPosition = propertyControl.SetProperty(propertyTarget, propertyInfo, attribute, uiPosition);

                m_usingProperties.Push(propertyControl);

                return uiPosition;
            }

            public void DisableAllControls()
            {
                while (m_usingProperties.Count > 0)
                {
                    var property = m_usingProperties.Pop();
                    m_PropertyPool.Push(property);
                }
            }
        }

        private abstract class PropertyControl
        {
            public PropertyControl(PropertyPanel rootPanel)
            {
            }

            public abstract int SetProperty(System.Object propertyTarget, System.Reflection.PropertyInfo property, System.Attribute attribute, int height);
        }

        private class SwitchPropertyControl : PropertyControl
        {
            public Label propertyNameLabel;
            public CheckBox propertyCheckBox;
            public System.Reflection.PropertyInfo property;
            public System.Object propertyTarget;

            public SwitchPropertyControl(PropertyPanel rootPanel) : base(rootPanel)
            {
                propertyNameLabel = new Label();
                propertyCheckBox = new CheckBox();

                rootPanel.Controls.Add(propertyNameLabel);
                rootPanel.Controls.Add(propertyCheckBox);

                propertyNameLabel.Visible = false;
                propertyNameLabel.AutoSize = true;

                propertyCheckBox.Visible = false;
                propertyCheckBox.AutoSize = true;
                propertyCheckBox.CheckedChanged += new System.EventHandler(this.OnCheckedChanged);
            }

            public override int SetProperty(object propertyTarget, PropertyInfo property, Attribute attribute, int height)
            {
                this.property = property;
                this.propertyTarget = propertyTarget;

                bool currentValue = (bool)property.GetValue(propertyTarget);

                this.propertyNameLabel.Visible = true;
                this.propertyNameLabel.Location = new System.Drawing.Point(7, height + 6);
                this.propertyNameLabel.Name = "propertyLabel_" + propertyTarget.GetHashCode();
                this.propertyNameLabel.Size = new System.Drawing.Size(53, 12);
                this.propertyNameLabel.Text = ((RayTracerNet.SwitchPropertyDisplayAttribute)attribute).propertyName + ":";

                this.propertyCheckBox.Visible = true;
                this.propertyCheckBox.Location = new System.Drawing.Point(35, height + 24);
                this.propertyCheckBox.Name = "checkBox_" + propertyTarget.GetHashCode();
                this.propertyCheckBox.Size = new System.Drawing.Size(192, 20);
                this.propertyCheckBox.Text = ((RayTracerNet.SwitchPropertyDisplayAttribute)attribute).propertyName;
                this.propertyCheckBox.Checked = currentValue;

                return height + 50;
            }

            private void OnCheckedChanged(object sender, EventArgs e)
            {
                CheckBox checkBox = sender as CheckBox;

                property.SetValue(propertyTarget, checkBox.Checked);
            }
        }

        private class TextPropertyControl : PropertyControl
        {
            public Label propertyNameLabel;
            public System.Reflection.PropertyInfo property;
            public System.Object propertyTarget;

            public TextPropertyControl(PropertyPanel rootPanel) : base(rootPanel)
            {
                propertyNameLabel = new Label();

                rootPanel.Controls.Add(propertyNameLabel);

                propertyNameLabel.Visible = false;
                propertyNameLabel.AutoSize = true;
            }

            public override int SetProperty(object propertyTarget, PropertyInfo property, Attribute attribute, int height)
            {
                this.property = property;
                this.propertyTarget = propertyTarget;

                object currentValue = property.GetValue(propertyTarget);

                this.propertyNameLabel.Visible = true;
                this.propertyNameLabel.Location = new System.Drawing.Point(7, height + 6);
                this.propertyNameLabel.Name = "propertyLabel_" + propertyTarget.GetHashCode();
                this.propertyNameLabel.Size = new System.Drawing.Size(53, 12);
                this.propertyNameLabel.Text = ((RayTracerNet.TextPropertyDisplayAttribute)attribute).propertyName + ":" + currentValue.ToString();

                return height + 30;
            }
        }

        private class EnumPropertyControl : PropertyControl
        {
            public Label propertyNameLabel;
            public ComboBox comboBox;
            public System.Reflection.PropertyInfo property;
            public System.Object propertyTarget;

            public EnumPropertyControl(PropertyPanel rootPanel) : base(rootPanel)
            {
                propertyNameLabel = new Label();
                comboBox = new ComboBox();

                rootPanel.Controls.Add(propertyNameLabel);
                rootPanel.Controls.Add(comboBox);

                propertyNameLabel.Visible = false;
                propertyNameLabel.AutoSize = true;

                comboBox.Visible = false;
                comboBox.SelectedIndexChanged += new System.EventHandler(this.OnSelectedIndexChanged);
            }

            public override int SetProperty(object propertyTarget, PropertyInfo property, Attribute attribute, int height)
            {
                this.property = property;
                this.propertyTarget = propertyTarget;

                object currentEnum = property.GetValue(propertyTarget);

                this.propertyNameLabel.Visible = true;
                this.propertyNameLabel.Location = new System.Drawing.Point(7, height + 6);
                this.propertyNameLabel.Name = "propertyLabel_" + propertyTarget.GetHashCode();
                this.propertyNameLabel.Size = new System.Drawing.Size(53, 12);
                this.propertyNameLabel.Text = ((RayTracerNet.EnumPropertyDisplayAttribute)attribute).propertyName + ":";

                this.comboBox.Visible = true;
                this.comboBox.Location = new System.Drawing.Point(35, height + 24);
                this.comboBox.Name = "comboBox_" + propertyTarget.GetHashCode();
                this.comboBox.Size = new System.Drawing.Size(192, 20);
                this.comboBox.SelectedItem = null;
                this.comboBox.Items.Clear();

                Array enumList = System.Enum.GetValues(property.PropertyType);

                foreach (var item in enumList)
                {
                    this.comboBox.Items.Add(item);
                }
                this.comboBox.SelectedItem = currentEnum;

                return height + 50;
            }

            private void OnSelectedIndexChanged(object sender, EventArgs e)
            {
                ComboBox comboBox = sender as ComboBox;
                if (comboBox != null)
                {
                    var item = comboBox.SelectedItem;
                    if (item != null)
                    {
                        this.property.SetValue(propertyTarget, item);
                    }
                }
            }
        }

        private class ColorPropertyControl : PropertyControl
        {
            public Label propertyNameLabel;
            public Button propertyButton;
            public ColorDialog colorDialog;
            public System.Reflection.PropertyInfo property;
            public System.Object propertyTarget;

            public ColorPropertyControl(PropertyPanel rootPanel) : base(rootPanel)
            {
                propertyNameLabel = new Label();
                propertyButton = new Button();
                colorDialog = new ColorDialog();

                rootPanel.Controls.Add(propertyNameLabel);
                rootPanel.Controls.Add(propertyButton);

                propertyNameLabel.Visible = false;
                propertyNameLabel.AutoSize = true;
                propertyButton.Visible = false;
                propertyButton.AutoSize = true;
                propertyButton.Click += new System.EventHandler(this.OnButtonClick);
            }

            public override int SetProperty(object propertyTarget, PropertyInfo property, Attribute attribute, int height)
            {
                this.property = property;
                this.propertyTarget = propertyTarget;

                RayTracerNet.Color color = (RayTracerNet.Color)property.GetValue(propertyTarget);

                System.Drawing.Color syscolor = color.ToSystemColor();

                this.propertyNameLabel.Visible = true;
                this.propertyNameLabel.Location = new System.Drawing.Point(7, height + 6);
                this.propertyNameLabel.Name = "propertyLabel_" + propertyTarget.GetHashCode();
                this.propertyNameLabel.Size = new System.Drawing.Size(53, 12);
                this.propertyNameLabel.Text = ((RayTracerNet.ColorPropertyDisplayAttribute)attribute).propertyName + ":";

                this.propertyButton.Visible = true;
                this.propertyButton.Location = new System.Drawing.Point(35, height + 24);
                this.propertyButton.Name = "propertyButton_" + propertyTarget.GetHashCode();
                this.propertyButton.Size = new System.Drawing.Size(192, 20);
                this.propertyButton.Text = syscolor.Name;
                this.propertyButton.ForeColor = syscolor;

                return height + 50;
            }

            private void OnButtonClick(object sender, EventArgs e)
            {
                DialogResult dialogResult = colorDialog.ShowDialog();
                if (dialogResult == DialogResult.OK)
                {
                    var col = colorDialog.Color;
                    float r = (float)col.R / 255.0f;
                    float g = (float)col.G / 255.0f;
                    float b = (float)col.B / 255.0f;
                    float a = (float)col.A / 255.0f;

                    RayTracerNet.Color color = new RayTracerNet.Color(r, g, b, a);
                    property.SetValue(propertyTarget, color);
                    System.Drawing.Color syscolor = color.ToSystemColor();
                    this.propertyButton.Text = syscolor.Name;
                    this.propertyButton.ForeColor = syscolor;
                }
            }
        }

        private class ResourcePropertyControl : PropertyControl
        {
            public Label propertyNameLabel;
            public ComboBox comboBox;
            public System.Reflection.PropertyInfo property;
            public System.Object propertyTarget;
            //public Type resourceType;

            public ResourcePropertyControl(PropertyPanel rootPanel) : base(rootPanel)
            {
                propertyNameLabel = new Label();
                comboBox = new ComboBox();

                rootPanel.Controls.Add(propertyNameLabel);
                rootPanel.Controls.Add(comboBox);

                propertyNameLabel.Visible = false;
                propertyNameLabel.AutoSize = true;

                comboBox.Visible = false;
                comboBox.SelectedIndexChanged += new System.EventHandler(this.OnSelectedIndexChanged);
            }

            public override int SetProperty(object propertyTarget, PropertyInfo property, Attribute attribute, int height)
            {
                this.property = property;
                this.propertyTarget = propertyTarget;
                //this.resourceType = attribute.resourceType;
                object currentRes = property.GetValue(propertyTarget);

                this.propertyNameLabel.Visible = true;
                this.propertyNameLabel.Location = new System.Drawing.Point(7, height + 6);
                this.propertyNameLabel.Name = "propertyLabel_" + propertyTarget.GetHashCode();
                this.propertyNameLabel.Size = new System.Drawing.Size(53, 12);
                this.propertyNameLabel.Text = ((RayTracerNet.ResourcePropertyDisplayAttribute)attribute).propertyName + ":";

                this.comboBox.Visible = true;
                this.comboBox.Location = new System.Drawing.Point(35, height + 24);
                this.comboBox.Name = "comboBox_" + propertyTarget.GetHashCode();
                this.comboBox.Size = new System.Drawing.Size(192, 20);
                this.comboBox.SelectedItem = null;
                this.comboBox.Items.Clear();

                List<ResourceObject> resourceObjects = RayTracerNet.RayTracer.GetInstance().resourceList;
                if (resourceObjects != null)
                {
                    foreach (var resource in resourceObjects)
                    {
                        //if (resource.GetType() == property.PropertyType)
                        if (property.PropertyType.IsInstanceOfType(resource))
                        {
                            this.comboBox.Items.Add(resource);
                        }
                    }
                }
                this.comboBox.SelectedItem = currentRes;

                return height + 50;
            }

            private void OnSelectedIndexChanged(object sender, EventArgs e)
            {
                ComboBox comboBox = sender as ComboBox;
                if (comboBox != null)
                {
                    var item = comboBox.SelectedItem as ResourceObject;
                    if (item != null)
                    {
                        this.property.SetValue(propertyTarget, item);
                    }
                }
            }
        }

        private class NumberPropertyControl : PropertyControl
        {
            public Label propertyNameLabel;
            public TextBox propertyTextBox;
            public TrackBar propertyTrackBar;
            public System.Reflection.PropertyInfo property;
            public System.Object propertyTarget;
            public double min;
            public double max;

            public NumberPropertyControl(PropertyPanel rootPanel) : base(rootPanel)
            {
                propertyNameLabel = new Label();
                propertyTextBox = new TextBox();
                propertyTrackBar = new TrackBar();

                ((System.ComponentModel.ISupportInitialize)(this.propertyTrackBar)).BeginInit();

                rootPanel.Controls.Add(propertyNameLabel);
                rootPanel.Controls.Add(propertyTextBox);
                rootPanel.Controls.Add(propertyTrackBar);

                propertyNameLabel.Visible = false;
                propertyNameLabel.AutoSize = true;

                propertyTextBox.Visible = false;
                propertyTextBox.LostFocus += new System.EventHandler(this.OnTextBoxChanged);
                propertyTextBox.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.OnEnterText);

                propertyTrackBar.Visible = false;
                propertyTrackBar.Minimum = 0;
                propertyTrackBar.Maximum = 5000;
                propertyTrackBar.LargeChange = 1;
                propertyTrackBar.AutoSize = false;
                propertyTrackBar.Size = new System.Drawing.Size(176, 20);
                propertyTrackBar.BackColor = System.Drawing.Color.White;
                propertyTrackBar.TickStyle = System.Windows.Forms.TickStyle.None;
                propertyTrackBar.ValueChanged += new System.EventHandler(this.OnValueChanged);

                ((System.ComponentModel.ISupportInitialize)(this.propertyTrackBar)).EndInit();
            }

            public override int SetProperty(object propertyTarget, PropertyInfo property, Attribute attribute, int height)
            {
                this.property = property;
                this.propertyTarget = propertyTarget;
                this.min = ((RayTracerNet.NumberPropertyDisplayAttribute)attribute).min;
                this.max = ((RayTracerNet.NumberPropertyDisplayAttribute)attribute).max;

                object value = property.GetValue(propertyTarget);

                this.propertyNameLabel.Visible = true;
                this.propertyNameLabel.Location = new System.Drawing.Point(7, height + 6);
                this.propertyNameLabel.Name = "propertyLabel_" + propertyTarget.GetHashCode();
                this.propertyNameLabel.Size = new System.Drawing.Size(53, 12);
                this.propertyNameLabel.Text = ((RayTracerNet.NumberPropertyDisplayAttribute)attribute).propertyName + ":";

                if (this.min > double.MinValue && this.max < double.MaxValue)
                {
                    this.propertyTrackBar.Visible = true;
                    this.propertyTrackBar.Location = new System.Drawing.Point(35, height + 24);
                    this.propertyTrackBar.Name = "propertyTrackBar_" + propertyTarget.GetHashCode();
                    int v = 0;
                    if (property.PropertyType == typeof(System.Double))
                    {
                        double dbvalue = (double)value;
                        v = (int)((dbvalue - this.min) / (this.max - this.min) * 5000.0);
                    }
                    else if (property.PropertyType == typeof(System.Single))
                    {
                        float flvalue = (float)value;
                        v = (int)((flvalue - this.min) / (this.max - this.min) * 5000.0);
                    }
                    else if (property.PropertyType == typeof(System.Int32))
                    {
                        int intvalue = (int)value;
                        v = (int)(((double)intvalue - this.min) / (this.max - this.min) * 5000.0);
                    }
                    this.propertyTrackBar.Value = v;
                }
                else
                {
                    this.propertyTextBox.Visible = true;
                    this.propertyTextBox.Location = new System.Drawing.Point(35, height + 24);
                    this.propertyTextBox.Name = "propertyTextBox_" + propertyTarget.GetHashCode();
                    this.propertyTextBox.Size = new System.Drawing.Size(104, 21);
                    this.propertyTextBox.Text = value.ToString();
                }

                return height + 50;
            }

            private void OnNumberChanged(object sender)
            {
                object value = property.GetValue(propertyTarget);
                TextBox textBox = sender as TextBox;
                try
                {
                    double num = double.Parse(textBox.Text);
                    if (num < min)
                        num = min;
                    if (num > max)
                        num = max;
                    if (property.PropertyType == typeof(System.Double))
                    {
                        double v = num;
                        textBox.Text = v.ToString();
                        property.SetValue(propertyTarget, v);
                    }
                    else if (property.PropertyType == typeof(System.Single))
                    {
                        float v = (float)num;
                        textBox.Text = v.ToString();
                        property.SetValue(propertyTarget, v);
                    }
                    else if (property.PropertyType == typeof(System.Int32))
                    {
                        int v = (int)num;
                        textBox.Text = v.ToString();
                        property.SetValue(propertyTarget, v);
                    }
                    return;
                }
                catch (System.Exception exp)
                {
                    textBox.Text = value.ToString();
                }
            }

            private void OnTextBoxChanged(object sender, EventArgs e)
            {
                OnNumberChanged(sender);
            }

            private void OnEnterText(object sender, KeyPressEventArgs e)
            {
                if (e.KeyChar == 0x0A || e.KeyChar == 0x0D)
                    OnNumberChanged(sender);
            }

            private void OnValueChanged(object sender, EventArgs e)
            {
                TrackBar trackBar = sender as TrackBar;
                int trackBarValue = trackBar.Value;
                double num = ((double)trackBarValue) / 5000.0;
                num = this.min + (this.max - this.min) * num;

                if (property.PropertyType == typeof(System.Double))
                {
                    double v = num;
                    property.SetValue(propertyTarget, v);
                }
                else if (property.PropertyType == typeof(System.Single))
                {
                    float v = (float)num;
                    property.SetValue(propertyTarget, v);
                }
                else if (property.PropertyType == typeof(System.Int32))
                {
                    int v = (int)num;
                    property.SetValue(propertyTarget, v);
                }
            }
        }

        private class VectorPropertyControl : PropertyControl
        {
            public Label propertyNameLabel;
            public Label propertyXLabel;
            public Label propertyYLabel;
            public Label propertyZLabel;
            public TextBox propertyXTextBox;
            public TextBox propertyYTextBox;
            public TextBox propertyZTextBox;
            public System.Reflection.PropertyInfo property;
            public System.Object propertyTarget;
            public RayTracerNet.VectorPropertyType type;

            public VectorPropertyControl(PropertyPanel rootPanel) : base(rootPanel)
            {
                propertyNameLabel = new Label();
                propertyXLabel = new Label();
                propertyYLabel = new Label();
                propertyZLabel = new Label();
                propertyXTextBox = new TextBox();
                propertyYTextBox = new TextBox();
                propertyZTextBox = new TextBox();

                rootPanel.Controls.Add(propertyNameLabel);
                rootPanel.Controls.Add(propertyXLabel);
                rootPanel.Controls.Add(propertyYLabel);
                rootPanel.Controls.Add(propertyZLabel);
                rootPanel.Controls.Add(propertyXTextBox);
                rootPanel.Controls.Add(propertyYTextBox);
                rootPanel.Controls.Add(propertyZTextBox);

                propertyNameLabel.Visible = false;
                propertyNameLabel.AutoSize = true;
                propertyXLabel.Visible = false;
                propertyXLabel.AutoSize = true;
                propertyYLabel.Visible = false;
                propertyYLabel.AutoSize = true;
                propertyZLabel.Visible = false;
                propertyZLabel.AutoSize = true;

                propertyXTextBox.Visible = false;
                //propertyXTextBox.TextChanged += new System.EventHandler(this.OnXTextBoxChanged);
                propertyXTextBox.LostFocus += new System.EventHandler(this.OnXTextBoxChanged);
                //propertyXTextBox.Leave += new System.EventHandler(this.OnXTextBoxLeave);
                propertyXTextBox.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.OnXEnterText);

                propertyYTextBox.Visible = false;
                //propertyYTextBox.TextChanged += new System.EventHandler(this.OnYTextBoxChanged);
                propertyYTextBox.LostFocus += new System.EventHandler(this.OnYTextBoxChanged);
                //propertyYTextBox.Leave += new System.EventHandler(this.OnYTextBoxLeave);
                propertyYTextBox.LostFocus += new System.EventHandler(this.OnYTextBoxChanged);

                propertyZTextBox.Visible = false;
                //propertyZTextBox.TextChanged += new System.EventHandler(this.OnZTextBoxChanged);
                propertyZTextBox.LostFocus += new System.EventHandler(this.OnZTextBoxChanged);
                //propertyZTextBox.Leave += new System.EventHandler(this.OnZTextBoxLeave);
                propertyZTextBox.LostFocus += new System.EventHandler(this.OnZTextBoxChanged);
            }

            public override int SetProperty(object propertyTarget, PropertyInfo property, Attribute attribute, int height)
            {
                this.property = property;
                this.propertyTarget = propertyTarget;
                this.type = ((RayTracerNet.VectorPropertyDisplayAttribute)attribute).type;

                double x = 0, y = 0, z = 0;
                if (type == VectorPropertyType.Vector2)
                {
                    RayTracerNet.Vector2 vector = (RayTracerNet.Vector2)property.GetValue(propertyTarget);
                    x = vector.x;
                    y = vector.y;
                }
                else if (type == VectorPropertyType.Vector3)
                {
                    RayTracerNet.Vector3 vector = (RayTracerNet.Vector3)property.GetValue(propertyTarget);
                    x = vector.x;
                    y = vector.y;
                    z = vector.z;
                }

                this.propertyNameLabel.Visible = true;
                this.propertyNameLabel.Location = new System.Drawing.Point(7, height + 6);
                this.propertyNameLabel.Name = "positionLabel_" + propertyTarget.GetHashCode();
                this.propertyNameLabel.Size = new System.Drawing.Size(53, 12);
                this.propertyNameLabel.Text = ((RayTracerNet.VectorPropertyDisplayAttribute)attribute).propertyName + ":";

                this.propertyXLabel.Visible = true;
                this.propertyXLabel.Location = new System.Drawing.Point(20, height + 28);
                this.propertyXLabel.Name = "positionXLabel_" + propertyTarget.GetHashCode();
                this.propertyXLabel.Size = new System.Drawing.Size(11, 12);
                this.propertyXLabel.Text = "X";

                this.propertyYLabel.Visible = true;
                this.propertyYLabel.Location = new System.Drawing.Point(90, height + 28);
                this.propertyYLabel.Name = "positionYLabel_" + propertyTarget.GetHashCode();
                this.propertyYLabel.Size = new System.Drawing.Size(11, 12);
                this.propertyYLabel.Text = "Y";

                if (((RayTracerNet.VectorPropertyDisplayAttribute)attribute).type == VectorPropertyType.Vector3)
                {
                    this.propertyZLabel.Visible = true;
                    this.propertyZLabel.Location = new System.Drawing.Point(160, height + 28);
                    this.propertyZLabel.Name = "positionZLabel_" + propertyTarget.GetHashCode();
                    this.propertyZLabel.Size = new System.Drawing.Size(11, 12);
                    this.propertyZLabel.Text = "Z";
                }
                else
                    this.propertyZLabel.Visible = false;

                this.propertyXTextBox.Visible = true;
                this.propertyXTextBox.Location = new System.Drawing.Point(35, height + 24);
                this.propertyXTextBox.Name = "positionXTextBox_" + propertyTarget.GetHashCode();
                this.propertyXTextBox.Size = new System.Drawing.Size(52, 21);
                this.propertyXTextBox.Text = x.ToString();

                this.propertyYTextBox.Visible = true;
                this.propertyYTextBox.Location = new System.Drawing.Point(105, height + 24);
                this.propertyYTextBox.Name = "positionYTextBox_" + propertyTarget.GetHashCode();
                this.propertyYTextBox.Size = new System.Drawing.Size(52, 21);
                this.propertyYTextBox.Text = y.ToString();

                if (((RayTracerNet.VectorPropertyDisplayAttribute)attribute).type == VectorPropertyType.Vector3)
                {
                    this.propertyZTextBox.Visible = true;
                    this.propertyZTextBox.Location = new System.Drawing.Point(175, height + 24);
                    this.propertyZTextBox.Name = "positionZTextBox_" + propertyTarget.GetHashCode();
                    this.propertyZTextBox.Size = new System.Drawing.Size(52, 21);
                    this.propertyZTextBox.Text = z.ToString();
                }
                else
                    this.propertyZTextBox.Visible = false;

                return height + 50;
            }

            private void OnPositionChanged(object sender, int axis)
            {
                double x = 0, y = 0, z = 0;
                if (type == VectorPropertyType.Vector2)
                {
                    RayTracerNet.Vector2 vector = (RayTracerNet.Vector2)property.GetValue(propertyTarget);
                    x = vector.x;
                    y = vector.y;
                }
                else if (type == VectorPropertyType.Vector3)
                {
                    RayTracerNet.Vector3 vector = (RayTracerNet.Vector3)property.GetValue(propertyTarget);
                    x = vector.x;
                    y = vector.y;
                    z = vector.z;
                }
                TextBox textBox = sender as TextBox;
                try
                {
                    double num = double.Parse(textBox.Text);
                    if (axis == 0)
                        x = num;
                    else if (axis == 1)
                        y = num;
                    else
                        z = num;
                    if (type == VectorPropertyType.Vector2)
                    {
                        RayTracerNet.Vector2 vector = new Vector2(x, y);
                        property.SetValue(propertyTarget, vector);
                    }
                    else if (type == VectorPropertyType.Vector3)
                    {
                        RayTracerNet.Vector3 vector = new Vector3(x, y, z);
                        property.SetValue(propertyTarget, vector);
                    }
                    return;
                }
                catch (System.Exception exp)
                {
                    if (axis == 0)
                        textBox.Text = x.ToString();
                    else if (axis == 1)
                        textBox.Text = y.ToString();
                    else
                        textBox.Text = z.ToString();
                }
            }

         
            private void OnXTextBoxChanged(object sender, EventArgs e)
            {
                OnPositionChanged(sender, 0);
            }

            private void OnYTextBoxChanged(object sender, EventArgs e)
            {
                OnPositionChanged(sender, 1);
            }

            private void OnZTextBoxChanged(object sender, EventArgs e)
            {
                OnPositionChanged(sender, 2);
            }

           

            private void OnXEnterText(object sender, KeyPressEventArgs e)
            {
                if (e.KeyChar == 0x0A || e.KeyChar == 0x0D)
                    OnPositionChanged(sender, 0);
            }

            private void OnYEnterText(object sender, KeyPressEventArgs e)
            {
                if (e.KeyChar == 0x0A || e.KeyChar == 0x0D)
                    OnPositionChanged(sender, 1);
            }

            private void OnZEnterText(object sender, KeyPressEventArgs e)
            {
                if (e.KeyChar == 0x0A || e.KeyChar == 0x0D)
                    OnPositionChanged(sender, 2);
            }
        }

        private PropertyPool<VectorPropertyControl> m_vectorPropertyPool;
        private PropertyPool<NumberPropertyControl> m_numberPropertyPool;
        private PropertyPool<ResourcePropertyControl> m_resourcePropertyPool;
        private PropertyPool<ColorPropertyControl> m_colorPropertyPool;
        private PropertyPool<EnumPropertyControl> m_enumPropertyPool;
        private PropertyPool<TextPropertyControl> m_textPropertyPool;
        private PropertyPool<SwitchPropertyControl> m_switchPropertyPool;

        private int m_UIPosition = 0;

        public PropertyPanel()
        {
            InitializeComponent();

            m_vectorPropertyPool = new PropertyPool<VectorPropertyControl>(this);
            m_numberPropertyPool = new PropertyPool<NumberPropertyControl>(this);
            m_resourcePropertyPool = new PropertyPool<ResourcePropertyControl>(this);
            m_colorPropertyPool = new PropertyPool<ColorPropertyControl>(this);
            m_enumPropertyPool = new PropertyPool<EnumPropertyControl>(this);
            m_textPropertyPool = new PropertyPool<TextPropertyControl>(this);
            m_switchPropertyPool = new PropertyPool<SwitchPropertyControl>(this);
        }

        public void SetDisplayObject(System.Object obj)
        {
            DisableAllControls();
            m_UIPosition = 0;
            if (obj == null)
            {
                return;
            }
            System.Type type = obj.GetType();
            System.Reflection.PropertyInfo[] properties = type.GetProperties(System.Reflection.BindingFlags.Public | System.Reflection.BindingFlags.Instance);
            
            for (int i = 0; i < properties.Length; i++)
            {
                RayTracerNet.VectorPropertyDisplayAttribute vectorPropertyAttr = properties[i].GetCustomAttribute<RayTracerNet.VectorPropertyDisplayAttribute>();
                if (vectorPropertyAttr != null)
                {
                    m_UIPosition = m_vectorPropertyPool.AddProperty(obj, vectorPropertyAttr, properties[i], m_UIPosition);
                    continue;
                }
                RayTracerNet.NumberPropertyDisplayAttribute numberPropertyAttr = properties[i].GetCustomAttribute<RayTracerNet.NumberPropertyDisplayAttribute>();
                if (numberPropertyAttr != null)
                {
                    m_UIPosition = m_numberPropertyPool.AddProperty(obj, numberPropertyAttr, properties[i], m_UIPosition);
                    continue;
                }
                RayTracerNet.ResourcePropertyDisplayAttribute resourcePropertyAttr = properties[i].GetCustomAttribute<RayTracerNet.ResourcePropertyDisplayAttribute>();
                if (resourcePropertyAttr != null)
                {
                    m_UIPosition = m_resourcePropertyPool.AddProperty(obj, resourcePropertyAttr, properties[i], m_UIPosition);
                    continue;
                }
                RayTracerNet.ColorPropertyDisplayAttribute colorPropertyAttr = properties[i].GetCustomAttribute<RayTracerNet.ColorPropertyDisplayAttribute>();
                if (colorPropertyAttr != null)
                {
                    m_UIPosition = m_colorPropertyPool.AddProperty(obj, colorPropertyAttr, properties[i], m_UIPosition);
                    continue;
                }
                RayTracerNet.EnumPropertyDisplayAttribute enumPropertyAttr = properties[i].GetCustomAttribute<RayTracerNet.EnumPropertyDisplayAttribute>();
                if (enumPropertyAttr != null)
                {
                    m_UIPosition = m_enumPropertyPool.AddProperty(obj, enumPropertyAttr, properties[i], m_UIPosition);
                    continue;
                }
                RayTracerNet.TextPropertyDisplayAttribute textPropertyAttr = properties[i].GetCustomAttribute<RayTracerNet.TextPropertyDisplayAttribute>();
                if (textPropertyAttr != null)
                {
                    m_UIPosition = m_textPropertyPool.AddProperty(obj, textPropertyAttr, properties[i], m_UIPosition);
                    continue;
                }
                RayTracerNet.SwitchPropertyDisplayAttribute switchPropertyAttr = properties[i].GetCustomAttribute<RayTracerNet.SwitchPropertyDisplayAttribute>();
                if (switchPropertyAttr != null)
                {
                    m_UIPosition = m_switchPropertyPool.AddProperty(obj, switchPropertyAttr, properties[i], m_UIPosition);
                    continue;
                }
            }
        }

        //private void AddVectorProperty(System.Object propertyTarget, RayTracerNet.VectorPropertyDisplayAttribute attribute, System.Reflection.PropertyInfo propertyInfo)
        //{
        //    VectorPropertyControl propertyControl = null;
        //    if (m_vectorPropertyPool.Count > 0)
        //    {
        //        propertyControl = m_vectorPropertyPool.Pop();
        //    }
        //    else
        //        propertyControl = new VectorPropertyControl(this);
        //    m_UIPosition = propertyControl.SetProperty(propertyTarget, propertyInfo, attribute, m_UIPosition);

        //    m_usingVectorProperties.Push(propertyControl);
        //}

        //private void AddNumberProperty(System.Object propertyTarget, RayTracerNet.NumberPropertyDisplayAttribute attribute, System.Reflection.PropertyInfo propertyInfo)
        //{
        //    NumberPropertyControl propertyControl = null;
        //    if (m_numberPropertyPool.Count > 0)
        //    {
        //        propertyControl = m_numberPropertyPool.Pop();
        //    }
        //    else
        //        propertyControl = new NumberPropertyControl(this);
        //    m_UIPosition = propertyControl.SetProperty(propertyTarget, propertyInfo, attribute, m_UIPosition);

        //    m_usingNumberProperties.Push(propertyControl);
        //}

        //private void AddResourceProperty(System.Object propertyTarget, RayTracerNet.ResourcePropertyDisplayAttribute attribute, System.Reflection.PropertyInfo propertyInfo)
        //{
        //    ResourcePropertyControl propertyControl = null;
        //    if (m_resourcePropertyPool.Count > 0)
        //    {
        //        propertyControl = m_resourcePropertyPool.Pop();
        //    }
        //    else
        //        propertyControl = new ResourcePropertyControl(this);
        //    m_UIPosition = propertyControl.SetProperty(propertyTarget, propertyInfo, attribute, m_UIPosition);

        //    m_usingResourceProperties.Push(propertyControl);
        //}

        //private void AddColorProperty(System.Object propertyTarget, RayTracerNet.ColorPropertyDisplayAttribute attribute, System.Reflection.PropertyInfo propertyInfo)
        //{
        //    ColorPropertyControl propertyControl = null;
        //    if (m_colorPropertyPool.Count > 0)
        //    {
        //        propertyControl = m_colorPropertyPool.Pop();
        //    }
        //    else
        //        propertyControl = new ColorPropertyControl(this);
        //    m_UIPosition = propertyControl.SetProperty(propertyTarget, propertyInfo, attribute, m_UIPosition);

        //    m_usingColorProperties.Push(propertyControl);
        //}

        //private void AddEnumProperty(System.Object propertyTarget, RayTracerNet.EnumPropertyDisplayAttribute attribute, System.Reflection.PropertyInfo propertyInfo)
        //{
        //    EnumPropertyControl propertyControl = null;
        //    if (m_enumPropertyPool.Count > 0)
        //    {
        //        propertyControl = m_enumPropertyPool.Pop();
        //    }
        //    else
        //        propertyControl = new EnumPropertyControl(this);
        //    m_UIPosition = propertyControl.SetProperty(propertyTarget, propertyInfo, attribute, m_UIPosition);

        //    m_usingEnumProperties.Push(propertyControl);
        //}

        private void DisableAllControls()
        {
            foreach (var control in this.Controls)
            {
                if (control != null)
                {
                    ((Control)control).Visible = false;
                }
            }
            m_vectorPropertyPool.DisableAllControls();
            m_resourcePropertyPool.DisableAllControls();
            m_numberPropertyPool.DisableAllControls();
            m_enumPropertyPool.DisableAllControls();
            m_colorPropertyPool.DisableAllControls();
            m_textPropertyPool.DisableAllControls();
        }
    }
}
