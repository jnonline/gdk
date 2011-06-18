using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace Gdk.Forms
{
    // ====================================================

    public partial class GdkPropertyGrid : UserControl
    {
        // =============================================
        // Properties

        public int RowHeight = 18;
        public System.Drawing.Color HeaderBackColor = System.Drawing.Color.FromArgb(244, 247, 252);
        public System.Drawing.Color HighlightBackColor = System.Drawing.SystemColors.Highlight;
        public System.Drawing.Color GridColor = System.Drawing.Color.FromArgb(238, 245, 252);
        public string DefaultValueColumnName = "Default";

        private TableLayoutPanel tablePanel = null;
        private Font normalFont = System.Windows.Forms.Control.DefaultFont;
        private Font boldFont = new Font(System.Windows.Forms.Control.DefaultFont, FontStyle.Bold);
        private Font underlineFont = new Font(System.Windows.Forms.Control.DefaultFont, FontStyle.Underline);
        private Font boldUnderlineFont = new Font(System.Windows.Forms.Control.DefaultFont, FontStyle.Bold | FontStyle.Underline);


        // =============================================
        // Delegates & Events

        public delegate void GridPropertyEventHandler(GridProperty property);

        public event GridPropertyEventHandler PropertyValueChanged;
        public event GridPropertyEventHandler PropertyCheckChanged;

        // =============================================
        // Methods

        /// <summary>
        /// Constructor
        /// </summary>
        public GdkPropertyGrid()
        {
            InitializeComponent();
        }

        /// <summary>
        /// Called when a cell of the table is repainted
        /// </summary>
        void tablePanel_CellPaint(object sender, TableLayoutCellPaintEventArgs e)
        {
            Graphics g = e.Graphics;
            Rectangle r = e.CellBounds;

            using (Pen pen = new Pen(GridColor))
            {
                pen.Alignment = System.Drawing.Drawing2D.PenAlignment.Center;
                pen.DashStyle = System.Drawing.Drawing2D.DashStyle.Solid;

                // decrease border rectangle height/width by pen's width for last row/column cell
                if (e.Row == (tablePanel.RowCount - 1))
                {
                    r.Height -= 1;
                }

                if (e.Column == (tablePanel.ColumnCount - 1))
                {
                    r.Width -= 1;
                }

                // use graphics mehtods to draw cell's border
                e.Graphics.DrawRectangle(pen, r);
            }
        }

        /// <summary>
        /// Re-binds the given list of properties
        /// </summary>
        /// <param name="properties"></param>
        public void BindProperties(List<GridProperty> properties)
        {
            // Detach any old table panel
            if (this.tablePanel != null)
                this.splitContainer1.Panel1.Controls.Remove(this.tablePanel);

            // Create the table panel
            this.tablePanel = new System.Windows.Forms.TableLayoutPanel();           
            this.tablePanel.SuspendLayout();

            this.tablePanel.BackColor = System.Drawing.SystemColors.Window;
            this.tablePanel.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tablePanel.Location = new System.Drawing.Point(0, 0);
            this.tablePanel.Margin = new System.Windows.Forms.Padding(0);
            this.tablePanel.Name = "tablePanel";
            this.tablePanel.TabIndex = 0;

            // Bind to the CellPaint method of the table layout panel
            this.tablePanel.CellPaint += new TableLayoutCellPaintEventHandler(tablePanel_CellPaint);

            this.tablePanel.VerticalScroll.Enabled = true;
            this.tablePanel.VerticalScroll.Visible = true;
            this.tablePanel.AutoScroll = true;

            // Sort the properties by category, then name
            properties.Sort(CompareProperties);

            // Split up the properties by category
            // ------------------------------------------------

            // Create the map of properties by category
            Dictionary<string, List<GridProperty>> propertiesByCategory = new Dictionary<string, List<GridProperty>>();
            
            // Loop through the properties
            foreach (GridProperty property in properties)
            {
                List<GridProperty> propertyList;

                // Do we have this category already?
                if(propertiesByCategory.ContainsKey(property.Category) == true)
                {
                    // Get the list
                    propertyList = propertiesByCategory[property.Category];
                }
                else
                {
                    // Create the list & put it in the map
                    propertyList = new List<GridProperty>();
                    propertiesByCategory.Add(property.Category, propertyList);
                }

                // Add this property to the list
                propertyList.Add(property);
            }

            // Configure the table
            // --------------------------------------------

            // Setup the columns

            // Name / Default / Override / Value
            this.tablePanel.ColumnCount = 4;
            this.tablePanel.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 20F));
            this.tablePanel.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 32F));
            this.tablePanel.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 15F));
            this.tablePanel.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 33F));

            // Setup the rows   1 header, N per category, N per property, 1 ending row
            int rowCount = 1 + propertiesByCategory.Count + properties.Count + 1;
            this.tablePanel.RowCount = rowCount;
            for (int i = 0; i < rowCount; i++ )
            {
                this.tablePanel.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, this.RowHeight));
            }

            // Add the header row
            // --------------------------------------------

            // Name
            Label headerNameLabel = CreateLabel("Property", HeaderBackColor, true, true);
            this.tablePanel.Controls.Add(headerNameLabel, 0, 0);

            // Default
            Label headerDefaultLabel = CreateLabel(DefaultValueColumnName, HeaderBackColor, true, true);
            this.tablePanel.Controls.Add(headerDefaultLabel, 1, 0);

            // Override
            Label headerOverrideLabel = CreateLabel("Override", HeaderBackColor, true, true);
            this.tablePanel.Controls.Add(headerOverrideLabel, 2, 0);

            // Override
            Label headerValueLabel = CreateLabel("Value", HeaderBackColor, true, true);
            this.tablePanel.Controls.Add(headerValueLabel, 3, 0);

            // Add the property rows
            // --------------------------------------------

            Font categoryFont = new Font(System.Windows.Forms.Control.DefaultFont, FontStyle.Bold);

            // Loop through the categories
            int rowIndex = 1;
            foreach(KeyValuePair<string, List<GridProperty> > category in propertiesByCategory)
            {
                // Create a label for this category
                Label categoryLabel = CreateLabel(category.Key, HeaderBackColor, true, false);
                this.tablePanel.Controls.Add(categoryLabel, 0, rowIndex);
                this.tablePanel.SetColumnSpan(categoryLabel, this.tablePanel.ColumnCount);

                rowIndex++;

                // Loop through the properties
                foreach(GridProperty property in category.Value)
                {
                    AddPropertyRow(property, rowIndex);
                    rowIndex++;
                }
            }

            // Now let the layout resume
            this.splitContainer1.Panel1.Controls.Add(this.tablePanel);
            tablePanel.ResumeLayout(true);
        }

        /// <summary>
        /// Utility method to create a label & stick it into the given column/row
        /// </summary>
        private Label CreateLabel(string text, System.Drawing.Color backColor, bool bold, bool underline)
        {
            // Create a label for this category
            Label label = new Label();
            
            label.Text = text;
            label.Font = normalFont;
            if (bold)
            {
                if (underline)
                    label.Font = boldUnderlineFont;
                else
                    label.Font = boldFont;
            }
            else if(underline)
                label.Font = underlineFont;

            label.BorderStyle = BorderStyle.None;
            label.BackColor = backColor;
            label.Dock = DockStyle.Fill;
            label.Padding = new Padding(0, 2, 0, 0);
            label.Margin = new Padding(0, 0, 0, 0);

            return label;
        }

        /// <summary>
        /// Creates a single row in the table panel for the given property
        /// </summary>
        private void AddPropertyRow(GridProperty property, int rowIndex)
        {
            // Create a tag for the row
            GridPropertyRowTag tag = new GridPropertyRowTag();
            tag.Property = property;

            // Add the label (and checkbox)
            // --------------------------------------------

            // Create a label for the Name column
            Label nameLabel = CreateLabel(property.Name, System.Drawing.SystemColors.Window, false, false);
            tag.NameLabel = nameLabel;
            nameLabel.Tag = tag;
            nameLabel.Click += new EventHandler(nameLabel_Click);
            this.tablePanel.Controls.Add(nameLabel, 0, rowIndex);

            // Create a label for the Default column
            Label defaultValueLabel = CreateLabel(property.DefaultValue, System.Drawing.SystemColors.Window, false, false);
            tag.DefaultValueLabel = defaultValueLabel;
            defaultValueLabel.Tag = tag;
            defaultValueLabel.Click += new EventHandler(defaultValueLabel_Click);
            this.tablePanel.Controls.Add(defaultValueLabel, 1, rowIndex);

            // Create a checkbox for the Override column
            CheckBox overrideCheckBox = new CheckBox();
            tag.OverrideCheckBox = overrideCheckBox;
            overrideCheckBox.Tag = tag;
            overrideCheckBox.FlatStyle = FlatStyle.Popup;
            overrideCheckBox.Checked = property.Checked;
            overrideCheckBox.Padding = new Padding(1, 1, 0, 0);
            overrideCheckBox.Margin = new Padding(0, 0, 0, 0);
            overrideCheckBox.CheckedChanged += new EventHandler(overrideCheckBox_CheckedChanged);
            this.tablePanel.Controls.Add(overrideCheckBox, 2, rowIndex); 
            
            // Add the Value Type-specific control
            // ------------------------------------------

            if (property.Type.IsEnum == true)
            {
                // enum - ComboBox
                // -----------------

                ComboBox comboBox = new ComboBox();
                tag.ValueControl = comboBox;
                comboBox.Tag = tag;
                comboBox.FlatStyle = FlatStyle.Flat;
                comboBox.DropDownStyle = ComboBoxStyle.DropDownList;
                comboBox.Margin = new Padding(0, 0, 0, 0);
                comboBox.Dock = DockStyle.Fill;
                comboBox.ItemHeight = 12;

                // Populate the combo box with all valid enum options
                int index = 0, selectIndex = 0;
                string[] enumNames = Enum.GetNames(property.Type);
                foreach (string name in enumNames)
                {
                    comboBox.Items.Add(name);
                    if (string.Compare(name, property.Value) == 0)
                        selectIndex = index;
                    index++;
                }

                comboBox.SelectedIndexChanged += new EventHandler(comboBox_SelectedIndexChanged);
                comboBox.SelectedIndex = selectIndex;
            }
            else if (property.Type == typeof(bool))
            {
                // bool - CheckBox
                // ----------------------------------

                // Create a checkbox
                CheckBox checkBox = new CheckBox();
                tag.ValueControl = checkBox;
                checkBox.Tag = tag;
                checkBox.TextAlign = ContentAlignment.MiddleLeft;
                checkBox.FlatStyle = FlatStyle.Popup;
                checkBox.Checked = bool.Parse(property.Value);
                checkBox.Text = checkBox.Checked.ToString();
                checkBox.Padding = new Padding(1, 1, 0, 0);
                checkBox.Margin = new Padding(0, 0, 0, 0);
                checkBox.CheckedChanged += new EventHandler(checkBox_CheckedChanged);
            }
            else if (property.Type == typeof(Color))
            {
                // Color - ColorSelector
                // ----------------------------------

                // Create a color selector
                ColorSelector colorSelector = new ColorSelector();
                tag.ValueControl = colorSelector;
                colorSelector.Tag = tag;
                colorSelector.Color = Color.Parse(property.Value);
                colorSelector.Padding = new Padding(1, 1, 0, 0);
                colorSelector.Margin = new Padding(0, 0, 0, 0);
                colorSelector.ColorChanged += new ColorSelector.ColorEventHandler(colorSelector_ColorChanged);
            }
            else
            {
                // other - TextBox
                // -----------------

                TextBox textBox = new TextBox();
                tag.ValueControl = textBox;
                textBox.Tag = tag;
                textBox.Padding = new Padding(0, 0, 0, 0);
                textBox.Margin = new Padding(1, 2, 0, 0);
                textBox.BorderStyle = BorderStyle.None;
                textBox.Dock = DockStyle.Fill;
                textBox.TextChanged += new EventHandler(textBox_TextChanged);
                textBox.Text = property.Value;
            }

            // Bind to the GotFocus event of the value control
            tag.ValueControl.GotFocus += new EventHandler(valueControl_GotFocus);
            tag.ValueControl.LostFocus += new EventHandler(valueControl_LostFocus);
            tag.ValueControl.Enabled = property.Checked;

            // Add the value control to the table
            this.tablePanel.Controls.Add(tag.ValueControl, 3, rowIndex);
        }

        /// <summary>
        /// Called when the color of the ColorSelector changes
        /// </summary>
        void colorSelector_ColorChanged(object sender, Color color)
        {
            // Get the color selector box
            ColorSelector colorSelector = sender as ColorSelector;
            if (colorSelector == null) return;
            GridPropertyRowTag tag = colorSelector.Tag as GridPropertyRowTag;
            if (tag == null) return;

            // Update the value
            tag.Property.Value = colorSelector.Color.ToString();

            // Fire the PropertyValueChanged event
            if (PropertyValueChanged != null)
                PropertyValueChanged(tag.Property);
        }

        /// <summary>
        /// Called when the user changes the a checkbox value
        /// </summary>
        void checkBox_CheckedChanged(object sender, EventArgs e)
        {
            // Get the check box
            CheckBox checkBox = sender as CheckBox;
            if (checkBox == null) return;
            GridPropertyRowTag tag = checkBox.Tag as GridPropertyRowTag;
            if (tag == null) return;

            // Update the value
            tag.Property.Value = checkBox.Checked.ToString();
            checkBox.Text = tag.Property.Value;

            // Fire the PropertyValueChanged event
            if (PropertyValueChanged != null)
                PropertyValueChanged(tag.Property);

        }

        /// <summary>
        /// Called when the user changes the a combo box drop down value
        /// </summary>
        void comboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            // Get the combo box
            ComboBox comboBox = sender as ComboBox;
            if (comboBox == null) return;
            GridPropertyRowTag tag = comboBox.Tag as GridPropertyRowTag;
            if (tag == null) return;

            // Update the value
            tag.Property.Value = comboBox.SelectedItem as string;

            // Fire the PropertyValueChanged event
            if (PropertyValueChanged != null)
                PropertyValueChanged(tag.Property);
        }

        /// <summary>
        /// Called when the user changes the text in the TextBox value control
        /// </summary>
        void textBox_TextChanged(object sender, EventArgs e)
        {
            // Get the text box
            TextBox textBox = sender as TextBox;
            if(textBox == null) return;
            GridPropertyRowTag tag = textBox.Tag as GridPropertyRowTag;
            if(tag == null) return;

            bool valueChanged = false;

            // Handle type-specific value handling
            if (tag.Property.Type == typeof(float))
            {
                // Make sure the value can be converted to a float
                float floatValue;
                if (float.TryParse(textBox.Text, out floatValue) == false)
                {
                    // This isnt a valid float value... Change the text to red,
                    textBox.ForeColor = System.Drawing.Color.Red;
                }
                else
                {
                    // Just use the string value directly
                    valueChanged = true;
                    tag.Property.Value = textBox.Text;
                    textBox.ForeColor = System.Drawing.Color.Black;
                }
            }
            else if (tag.Property.Type == typeof(int) || tag.Property.Type == typeof(short))
            {
                // Make sure the value can be converted to a int
                int intValue;
                if (int.TryParse(textBox.Text, out intValue) == false)
                {
                    // This isnt a valid int value... Change the text to red,
                    textBox.ForeColor = System.Drawing.Color.Red;
                }
                else
                {
                    // Just use the string value directly
                    valueChanged = true;
                    tag.Property.Value = textBox.Text;
                    textBox.ForeColor = System.Drawing.Color.Black;
                }
            }
            else
            {
                // Just use the string value directly
                valueChanged = true;
                tag.Property.Value = textBox.Text;
            }

            // Fire the PropertyValueChanged event
            if (valueChanged && PropertyValueChanged != null)
                PropertyValueChanged(tag.Property);
        }

        /// <summary>
        /// Called when a value control gets focus
        /// </summary>
        void valueControl_GotFocus(object sender, EventArgs e)
        {
            // Get the control & the tag
            Control control = sender as Control;
            if (control == null) 
                return;
            GridPropertyRowTag tag = control.Tag as GridPropertyRowTag;
            if (tag == null)
                return;

            // Update the description block to reflect the property
            SetDescription(tag.Property);

            // Highlight the property label
            tag.NameLabel.BackColor = HighlightBackColor;
        }

        /// <summary>
        /// Called when a value control loses focus
        /// </summary>
        void valueControl_LostFocus(object sender, EventArgs e)
        {
            // Get the control & the tag
            Control control = sender as Control;
            if (control == null)
                return;
            GridPropertyRowTag tag = control.Tag as GridPropertyRowTag;
            if (tag == null)
                return;

            // Highlight the property label
            tag.NameLabel.BackColor = System.Drawing.Color.Transparent;
        }

        /// <summary>
        /// Called when the user clicks a property name label
        /// </summary>
        void nameLabel_Click(object sender, EventArgs e)
        {
            // Get the control & the tag
            Control control = sender as Control;
            if (control == null)
                return;
            GridPropertyRowTag tag = control.Tag as GridPropertyRowTag;
            if (tag == null)
                return;

            // Update the description block to reflect the property
            SetDescription(tag.Property);

            // Set focus on the value control
            tag.ValueControl.Focus();
        }

        /// <summary>
        /// Called when the user clicks a property default label
        /// </summary>
        void defaultValueLabel_Click(object sender, EventArgs e)
        {
            // Get the control & the tag
            Control control = sender as Control;
            if (control == null)
                return;
            GridPropertyRowTag tag = control.Tag as GridPropertyRowTag;
            if (tag == null)
                return;

            // Update the description block to reflect the property
            SetDescription(tag.Property);

            // Set focus on the value control
            tag.ValueControl.Focus();
        }

        /// <summary>
        /// Called when the mouse enters the name label
        /// </summary>
        void nameLabel_MouseEnter(object sender, EventArgs e)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Called whenever the override checkbox for a row is changed
        /// </summary>
        void overrideCheckBox_CheckedChanged(object sender, EventArgs e)
        {
            // Get the check box
            CheckBox overrideCheckBox = sender as CheckBox;
            if (overrideCheckBox == null) return;
            GridPropertyRowTag tag = overrideCheckBox.Tag as GridPropertyRowTag;
            if (tag == null) return;

            // Update the check state
            tag.Property.Checked = overrideCheckBox.Checked;

            // Enable/Disable the value control
            tag.ValueControl.Enabled = overrideCheckBox.Checked;

            // Fire the PropertyCheckChanged event
            if (PropertyCheckChanged != null)
                PropertyCheckChanged(tag.Property);
        }

        /// <summary>
        /// Updates the description block at the bottom of the grid, with the description of the given property
        /// </summary>
        void SetDescription(GridProperty property)
        {
            lblDescriptionHeader.Text = property.Name;
            lblDescription.Text = property.Description;
        }

        /// <summary>
        /// Clears all the current controls on the table panel
        /// </summary>
        private void ClearTablePanel()
        {
            this.tablePanel.Controls.Clear();
            this.tablePanel.RowStyles.Clear();
            this.tablePanel.ColumnStyles.Clear();
            this.tablePanel.RowCount = 0;
            this.tablePanel.ColumnCount = 0;
        }

        /// <summary>
        /// Compares 2 GridProperty items for sorting purposes
        /// </summary>
        private static int CompareProperties(GridProperty a, GridProperty b)
        {
            // Compare the category
            int catCompare = string.Compare(a.Category, b.Category, true);
            if (catCompare != 0)
                return catCompare;

            // Compare the name
            int nameCompare = string.Compare(a.Name, b.Name, true);
            return nameCompare;
        }
    }

    // ====================================================

    public class GridProperty
    {
        public string Name;
        public string Category;
        public string Description;
        public string Value;
        public string DefaultValue;
        public Type Type;
        public bool Checked;

        /// <summary>
        /// CTor
        /// </summary>
        public GridProperty()
        {
        }

        /// <summary>
        /// CTor
        /// </summary>
        public GridProperty(string name, string category, string description, bool isChecked, Type type, string value, string defaultValue)
        {
            this.Name = name;
            this.Category = category;
            this.Value = value;
            this.Checked = isChecked;
            this.Type = type;
            this.Description = description;
            this.DefaultValue = defaultValue;
        }
    }

    // ====================================================

    public class GridPropertyRowTag
    {
        public GridProperty Property;
        public Label NameLabel;
        public Label DefaultValueLabel;
        public CheckBox OverrideCheckBox;
        public Control ValueControl;
    }
 
}
