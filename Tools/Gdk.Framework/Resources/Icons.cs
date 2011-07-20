/*
  Copyright (c) 2011, Raincity Games LLC
  Licensed under the MIT license: http://www.opensource.org/licenses/mit_license.php
*/

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Drawing;

using Gdk;
using Gdk.Content;

namespace Gdk
{
    public enum Icons
    {
        FolderIcon,
        FileIcon,
        GhostFolderIcon,
        GhostFileIcon,
        TextureIcon,
        AtlasIcon,
        FontIcon,
        ModelIcon,
        AudioIcon
    }

    public sealed class IconManager
    {
        // Properties
        private static ImageList imageList = new ImageList();
        public static ImageList ImageList { get { return imageList; } }

        /// <summary>
        /// Static Constructor
        /// </summary>
        static IconManager()
        {
            imageList.ImageSize = new Size(20, 20);

            // Load all the images into the icon manager
            foreach (Icons icon in Enum.GetValues(typeof(Icons)))
            {
                // Load the icon image
                string imageResourceName = icon.ToString();
                object obj = Resources.ResourceManager.GetObject(imageResourceName, Resources.Culture);
                Bitmap iconBitmap = (System.Drawing.Bitmap)obj;
                imageList.Images.Add(iconBitmap);
            }
        }
        
        /// <summary>
        /// Gets the image for the associated icon
        /// </summary>
        /// <param name="icon"></param>
        /// <returns></returns>
        public static Image GetIconImage(Icons icon)
        {
            return imageList.Images[(int)icon];
        }
    }
}
