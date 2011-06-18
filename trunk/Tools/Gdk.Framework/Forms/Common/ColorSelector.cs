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
    public partial class ColorSelector : UserControl
    {
        public ColorSelector()
        {
            InitializeComponent();

            // Set the default color
            this.Color = Color.Black;
        }

        public delegate void ColorEventHandler(object sender, Color color);
        public event ColorEventHandler ColorChanged;

        private Color color;

        /// <summary>
        /// Gets or Sets the currently selected color
        /// </summary>
        public Color Color
        {
            get
            {
                // Get
                return this.color;
            }

            set
            {
                // Set
                this.color = value;
                pictureBoxColor.BackColor = System.Drawing.Color.FromArgb(255, this.color.ToSystemColor());
                textColor.Text = this.color.ToString();

                // Fire the ColorChanged event
                if (ColorChanged != null)
                    ColorChanged(this, this.color);
            }
        }

        /// <summary>
        /// Called when the user clicks on the color picture box
        /// </summary>
        private void pictureBoxColor_Click(object sender, EventArgs e)
        {
            // Show a color selection dialog
            ColorDialog dialog = new ColorDialog();
            dialog.Color = this.color.ToSystemColor();
            dialog.AnyColor = true;
            dialog.SolidColorOnly = false;
            if (dialog.ShowDialog() == DialogResult.OK)
            {
                // Update the color
                this.Color = Color.FromSystemColor(dialog.Color);
            }
        }

        /// <summary>
        /// Called when the user types in the color text box
        /// </summary>
        private void textColor_TextChanged(object sender, EventArgs e)
        {
            // Is the text a valid color?
            Color parsedColor;
            if (Color.TryParse(textColor.Text, out parsedColor) == true)
            {
                // Set the updated color
                this.color = parsedColor;
                pictureBoxColor.BackColor = System.Drawing.Color.FromArgb(255, this.color.ToSystemColor());

                // Fire the ColorChanged event
                if (ColorChanged != null)
                    ColorChanged(this, this.color);

                textColor.ForeColor = System.Drawing.Color.Black;
            }
            else
            {
                // Invalid color text
                textColor.ForeColor = System.Drawing.Color.Red;
            }
        }
    }
}
