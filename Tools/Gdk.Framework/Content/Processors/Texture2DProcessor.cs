/*
  Copyright (c) 2011, Raincity Games LLC
  Licensed under the MIT license: http://www.opensource.org/licenses/mit_license.php
*/

using System;
using System.Collections.Generic;
using System.Linq;
using System.IO;
using System.Text;
using Gdk.Framework;

namespace Gdk.Framework.Content
{
    public class Texture2DProcessor : Processor    
    {
        /// <summary>
        /// Gets the descriptive details of this processor
        /// </summary>
        /// <returns></returns>
        public override ProcessorDetails GetDetails()
        {
            ProcessorDetails details = new ProcessorDetails(
                "Takes an input image file (PNG, JPG, BMP, etc) and generates an output GdkImage file for use as a Texture2D",
                Icons.Texture
                );
            details.FileExtensions.Add("png");
            details.FileExtensions.Add("jpg");
            details.FileExtensions.Add("jpeg");
            details.FileExtensions.Add("bmp");

            details.AddParameter("PixelFormat", "Destination pixel format for the texture", "Texture", typeof(PixelFormats), PixelFormats.RGBA_5551);
            details.AddParameter("ColorKey", "This color will be turned into TransparentBlack when processed", "Texture", typeof(Color), Color.TransparentBlack);
            details.AddParameter("ImageScale", "Resizes the image by this scalar value", "Texture", typeof(float), (float)1.0);

            return details;
        }

        /// <summary>
        /// Processes the given asset
        /// </summary>
        public override void Process()
        {
            // Load the source image
            // --------------------------

            // Get the full path to the source file
            string fullSourcePath = PathUtilities.Combine(Context.ContentFolder, Asset.Path);

            // Open the source file into a Surface 
            Surface surface = Surface.FromFile(fullSourcePath);

            // Process the image
            // -------------------------

            // Get the destination pixel format from the processor parameters
            PixelFormats pixelFormat = Context.Parameters.GetEnumValue<PixelFormats>("PixelFormat", PixelFormats.RGBA_5551);
            Context.Log("PixelFormat: " + pixelFormat.ToString());

            // Get the image scale
            float imageScale = Context.Parameters.GetValue("ImageScale", 1.0f);
            if(imageScale != 1.0f)
            {
                int newWidth = (int)Math.Round(surface.Width * imageScale);
                int newHeight = (int)Math.Round(surface.Height * imageScale);
                Context.Log("Scaling image by [" + imageScale + "].  Old Size:[" + surface.Width + "," + surface.Height + "]  New Size:[" + newWidth + "," + newHeight + "]");
                
                // Create a new surface & stretch blit to it (using bilinear filtering)
                Surface newSurface = new Surface(newWidth, newHeight);
                newSurface.Blit(surface, 0, 0, surface.Width, surface.Height, 0, 0, newWidth, newHeight, TextureFilterMode.Bilinear);
                surface = newSurface;
            }

            // Color key the image
            Color colorKey = Context.Parameters.GetValue("ColorKey", Color.TransparentBlack);
            if (colorKey != Color.TransparentBlack)
            {
                Context.Log("Color keying the image");
                surface.ReplaceColor(colorKey, Color.TransparentBlack);
            }

            // Save the output 
            // -------------------------

            // Create the relative & full path to the destination .gdkimage file
            string relativeDestPath = Path.ChangeExtension(Asset.Path, ".gdkimage");
            string fullDestPath = Path.Combine(Context.OutputFolder, relativeDestPath);

            // Make sure the destination folder exists
            Directory.CreateDirectory(Path.GetDirectoryName(fullDestPath));
           
            // Write the texture to a gdkimage file
            surface.SaveToGdkImage(fullDestPath, pixelFormat);

            // Track the destination file
            Context.AddOutputDependency(relativeDestPath);
        }
    }
}
