using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Xml;

namespace Gdk.Content
{
    class BMFontProcessor : Processor
    {
        /// <summary>
        /// Gets the descriptive details of this processor
        /// </summary>
        /// <returns></returns>
        public override ProcessorDetails GetDetails()
        {
            ProcessorDetails details = new ProcessorDetails(
                "Takes BMFont (.fnt) file and outputs the corresponding GdkFont and GdkImage files",
                Icons.TextureIcon
                );
            details.FileExtensions.Add("fnt");

            // Parameters
            details.AddParameter("PixelFormat", "Destination pixel format for the font sheets", "Font", typeof(PixelFormats), PixelFormats.RGBA_4444);
            
            return details;
        }

        /// <summary>
        /// Processes the given asset
        /// </summary>
        public override void Process()
        {
            // Get the full path to the source file
            string fontSourcePath = Path.Combine(Context.ContentFolder, Asset.Path);

            // Verify the file exists
            if (File.Exists(fontSourcePath) == false)
                throw new FileNotFoundException("Font file not found", fontSourcePath);

            // Copy the font file from the source to the destination, and rename it to .gdkfont
            // ----------------------------------------------------------

            // Create the relative & full path to the destination .gdkfont file
            string relativeDestPath = Path.ChangeExtension(Asset.Path, ".gdkfont");
            string fontDestPath = Path.Combine(Context.OutputFolder, relativeDestPath);

            // Make sure the destination folder exists
            Directory.CreateDirectory(Path.GetDirectoryName(fontDestPath));

            // Copy the BMFont to the output GdkFont
            File.Copy(fontSourcePath, fontDestPath, true);

            // Track the destination file
            Context.AddOutputDependency(relativeDestPath);

            Context.LogInfo("Converted BMFont to GdkFont");

            // Load all the sheets of the font, and turn them into .gdkimage files
            // ----------------------------------------------------------

            // Get the destination pixel format from the processor parameters
            PixelFormats pixelFormat = Context.Parameters.GetEnumValue<PixelFormats>("PixelFormat", PixelFormats.RGBA_4444);

            // Get all the png's associated with this font
            string searchPattern = Path.GetFileNameWithoutExtension(fontSourcePath) + "*.png";
            string searchFolder = Path.GetDirectoryName(fontSourcePath);
            string[] pngPaths = Directory.GetFiles(searchFolder, searchPattern);

            // Loop through the pngs
            foreach (string pngPath in pngPaths)
            {
                // Open the PNG into a surface
                Surface surface = Surface.FromFile(pngPath);

                // BMFont sticks a transparent black edge around glyphs, so lets make it transparent white.
                surface.ReplaceColor(new Color(0, 0, 0, 0), new Color(255, 255, 255, 0));
           
                // Get the destination filename & path
                string destImageFile = Path.ChangeExtension(Path.GetFileName(pngPath), ".gdkimage");
                string destImagePath = Path.Combine(Path.GetDirectoryName(fontDestPath), destImageFile);

                Context.LogInfo(" - Converting PNG font sheet to gdkimage: " + destImageFile);

                // Save the font sheet to a gdkimage
                surface.SaveToGdkImage(destImagePath, pixelFormat);

                // Track the dependency
                Context.AddOutputDependency(destImagePath);
            }
        }
    }
}
