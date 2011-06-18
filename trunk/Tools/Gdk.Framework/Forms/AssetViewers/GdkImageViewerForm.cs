using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using Gdk;

namespace Gdk.Forms
{
    public partial class GdkImageViewerForm : Form
    {
        // Properties
        string imageFilePath;

        /// <summary>
        /// Constructor
        /// </summary>
        /// <param name="imageFilePath"></param>
        public GdkImageViewerForm(string imageFilePath)
        {
            InitializeComponent();

            // Setup the background settings
            cmbBackground.Items.Add("Purple");
            cmbBackground.Items.Add("White");
            cmbBackground.Items.Add("Black");
            cmbBackground.Items.Add("Checkered");
            //cmbBackground.Items.Add("Image");

            butBGColor1.BackColor = System.Drawing.Color.White;
            butBGColor2.BackColor = System.Drawing.Color.LightGray;

            cmbBackground.SelectedIndex = 3; // Checkered
            
            // Setup the "Scale Mode" combo box
            cmbScaleMode.Items.Add("None");
            cmbScaleMode.Items.Add("Linear");
            cmbScaleMode.Items.Add("Cubic");
            cmbScaleMode.SelectedIndex = 0;

            // Load the image
            LoadImage(imageFilePath);
        }


        /// <summary>
        /// Loads the given image into the picturebox
        /// </summary>
        public void LoadImage(string imageFilePath)
        {
            this.imageFilePath = imageFilePath;
            this.Text = imageFilePath;

            // Load the image into a surface
            PixelFormats pixelFormat;
            Surface surface = Surface.FromGdkImageFile(imageFilePath, out pixelFormat);
            
            // Load the image into the window's picture box
            pictureBox1.Image = surface.GetGDIBitmap();

            // Get the window extents relative to the picture box
            Size windowExtents = this.Size - pictureBox1.Size;
            
            // Resize the window to fit the full image
            this.Size = new Size(windowExtents.Width + surface.Width, windowExtents.Height + surface.Height);

            // Setup the labels
            statusLabel1.Text = "Size: " + surface.Width + " x " + surface.Height;
            statusLabel2.Text = "PixelFormat: " + pixelFormat.ToString();
            //statusLabel3.Text = "Scale: " + Scale + ";
            pictureBox1.SizeMode = PictureBoxSizeMode.Zoom;
        }

        /// <summary>
        ///  Called when the user clicks BG Color Button 1
        /// </summary>
        private void butBGColor1_Click(object sender, EventArgs e)
        {
            ColorDialog dlg = new ColorDialog();
            dlg.Color = butBGColor1.BackColor;
            if (dlg.ShowDialog() == System.Windows.Forms.DialogResult.OK)
            {
                butBGColor1.BackColor = dlg.Color;
                UpdateBackground();
            }
        }

        /// <summary>
        ///  Called when the user clicks BG Color Button 1
        /// </summary>
        private void butBGColor2_Click(object sender, EventArgs e)
        {
            ColorDialog dlg = new ColorDialog();
            dlg.Color = butBGColor2.BackColor;
            if (dlg.ShowDialog() == System.Windows.Forms.DialogResult.OK)
            {
                butBGColor2.BackColor = dlg.Color;
                UpdateBackground();
            }
        }

        /// <summary>
        ///  Called when the user changes the background combo box
        /// </summary>
        private void cmbBackground_SelectedIndexChanged(object sender, EventArgs e)
        {
            UpdateBackground();
        }

        /// <summary>
        /// Sets the background image to a tiled image of the given colors
        /// </summary>
        private void ApplyCheckerBackground()
        {
            // Show the bgcolor buttons
            butBGColor1.Visible = true;
            butBGColor2.Visible = true;

            // Create the 2 brushes
            Brush brushA = new SolidBrush(butBGColor1.BackColor);
            Brush brushB = new SolidBrush(butBGColor2.BackColor);

            // Create a checkerboard image
            int checkerSize = 8;
            Image bgImage = new Bitmap(checkerSize * 2, checkerSize * 2);
            Graphics g = Graphics.FromImage(bgImage);

            // Draw the checkerboard
            g.FillRectangle(brushA, 0, 0, checkerSize, checkerSize);
            g.FillRectangle(brushB, 0, checkerSize, checkerSize, checkerSize);
            g.FillRectangle(brushB, checkerSize, 0, checkerSize, checkerSize);
            g.FillRectangle(brushA, checkerSize, checkerSize, checkerSize, checkerSize);

            pictureBox1.BackgroundImage = bgImage;
        }

        /// <summary>
        /// Sets the background to a solid color
        /// </summary>
        private void ApplySolidBackground()
        {
            // Show just 1 color button
            butBGColor1.Visible = true;
            butBGColor2.Visible = false;

            // Set a solid color background
            pictureBox1.BackColor = butBGColor1.BackColor;
            pictureBox1.BackgroundImage = null;
        }

        /// <summary>
        /// Updates the background image / color based on the background settings
        /// </summary>
        private void UpdateBackground()
        {
            string bgType = cmbBackground.SelectedItem as string;
            switch (bgType)
            {
                case "White":
                    butBGColor1.BackColor = System.Drawing.Color.White;
                    ApplySolidBackground();
                    break;

                case "Black":
                    butBGColor1.BackColor = System.Drawing.Color.Black;
                    ApplySolidBackground();
                    break;

                case "Purple":
                    butBGColor1.BackColor = System.Drawing.Color.Purple;
                    ApplySolidBackground();
                    break;

                case "Checkered":
                    ApplyCheckerBackground();
                    break;

                case "Image":
                    ApplyCheckerBackground();
                    break;
            }

        }

        /// <summary>
        /// Called when the "Shift Colors" button is hit
        /// </summary>
        private void butShiftColors_Click(object sender, EventArgs e)
        {
            // Get the bitmap of the image
            Bitmap bitmap = (Bitmap)pictureBox1.Image;

            // Loop through the pixels of the image
            for (int y = 0; y < bitmap.Height; y++)
            {
                for (int x = 0; x < bitmap.Width; x++)
                {
                    // Get this pixel
                    System.Drawing.Color color = bitmap.GetPixel(x, y);
                    
                    // Shift the color channels
                    System.Drawing.Color newColor = System.Drawing.Color.FromArgb(color.R, color.G, color.B, color.A);

                    // Save the pixel back
                    bitmap.SetPixel(x, y, newColor);
                }
            }

            // Redraw
            pictureBox1.Invalidate();
        }

        /// <summary>
        /// Called when the user changes the Scale Mode combo box selection
        /// </summary>
        private void cmbScaleMode_SelectedIndexChanged(object sender, EventArgs e)
        {
            switch (cmbScaleMode.SelectedIndex)
            {
                case 0: pictureBox1.ImageDrawMode = System.Drawing.Drawing2D.InterpolationMode.NearestNeighbor; break;
                case 1: pictureBox1.ImageDrawMode = System.Drawing.Drawing2D.InterpolationMode.Bilinear; break;
                case 2: pictureBox1.ImageDrawMode = System.Drawing.Drawing2D.InterpolationMode.HighQualityBicubic; break;
            }
        }

    }
}
