using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace Gdk.Forms
{
    public class AssetViewerHelper
    {
        /// <summary>
        /// This method creates & returns an asset viewer form for the given asset URL
        /// </summary>
        public static Form GetViewerFormForAssetFile(string assetFilePath)
        {
            // Get the extension of the file
            string ext = System.IO.Path.GetExtension(assetFilePath);

            // Switch on the extension
            switch (ext)
            {
                case ".gdkimage":
                    return new GdkImageViewerForm(assetFilePath);
            }

            // No viewer for the given 
            return null;
        }
    }

    public class CustomPictureBox : PictureBox
    {
        private System.Drawing.Drawing2D.InterpolationMode imageDrawMode;
        public System.Drawing.Drawing2D.InterpolationMode ImageDrawMode
        {
            get { return imageDrawMode; }
            set { imageDrawMode = value; this.Invalidate(); }
        }

        protected override void OnPaint(PaintEventArgs pe)
        {
            pe.Graphics.InterpolationMode = imageDrawMode;
            base.OnPaint(pe);
        }
    }
}
