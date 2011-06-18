using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Xml;

using Gdk;

namespace Gdk.Content
{
    public class PoTAtlasProcessor : Processor
    {
        // Internal properties
        private float masterImageScale = 1.0f;
        private bool force32Bit = false;

        private const int MAX_TEXTURE_SIZE = 1024;

        /// <summary>
        /// Gets the descriptive details of this processor
        /// </summary>
        /// <returns></returns>
        public override ProcessorDetails GetDetails()
        {
            ProcessorDetails details = new ProcessorDetails(
                "Processes a .Atlas file into a .GdkAtlas and multiple sheets as .GdkImage files.  The generated atlas sheets are Guaranteed to be Power-of-Two textures",
                Icons.AtlasIcon
                );

            // File extensions
            details.FileExtensions.Add("atlas");

            // Parameters
            details.AddParameter("ImageScale", "Resizes the images prior to the atlasing process by this scalar value.  This scalar is multiplied by the scalar within the atlas file.  The images will not be scaled larger than the source images.", "Atlas", typeof(float), (float)1.0);
            details.AddParameter("Force32Bit", "Forces the output atlases to use 32-bit pixel formats", "Atlas", typeof(bool), false);
            
            return details;
        }

        /// <summary>
        /// Builds an atlas file
        /// </summary>
        /// <param name="atlasFile"></param>
        public override void Process()
        {
            // Build the full path to the atlas file
            string atlasFilePath = Path.Combine(Context.ContentFolder, Asset.Path);

            // Verify the file exists
            if (File.Exists(atlasFilePath) == false)
                throw new FileNotFoundException("Atlas file not found", atlasFilePath);

            // Get the processor parameters
            this.masterImageScale = Context.Parameters.GetValue("ImageScale", 1.0f);
            this.force32Bit = Context.Parameters.GetValue("Force32Bit", false);

            // Open the atlas file
            XmlDocument atlasXmlDoc = new XmlDocument();
            atlasXmlDoc.Load(atlasFilePath);

            // Verify the root element is an <Atlas> element
            if (atlasXmlDoc.DocumentElement.Name != "Atlas")
                throw new BuildException("The input atlas xml does not have a root Atlas element");

            Context.LogInfo("Processing Atlas: " + atlasFilePath);

            // Create an atlas for this file
            Atlas atlas = new Atlas();
            atlas.ContentFolder = Path.GetDirectoryName(atlasFilePath);
            atlas.Name = Path.GetFileNameWithoutExtension(atlasFilePath);

            // Get the root element of the xml
            XmlElement atlasElement = atlasXmlDoc.DocumentElement;

            // Process all the <Sheet> nodes in the atlas
            int sheetId = 1;
            foreach (XmlNode sheetNode in atlasElement.SelectNodes("Sheet"))
            {
                // Is this an Sheet node?
                ProcessSheet(atlas, sheetNode, sheetId);
                sheetId++;
            }

            // Process all the <Animation> nodes in the atlas
            foreach (XmlNode animationNode in atlasElement.SelectNodes("Animation"))
            {
                // Process the animation
                ProcessAnimation(atlas, animationNode);
            }

            // Write out the atlas data
            Write(atlas);
        }

        /// <summary>
        /// Processes a Sheet element in the atlas source xml
        /// </summary>
        private void ProcessSheet(Atlas atlas, XmlNode sheetNode, int sheetId)
        {
            // Begin processing the <Sheet> element
            Context.LogVerbose("- Processing <Sheet>");

            // Create a new sheet content holder
            AtlasSheet sheet = new AtlasSheet();

            // Add the new sheet to the Atlas
            sheet.Index = atlas.Sheets.Count;
            atlas.Sheets.Add(sheet);

            // Get the sheet PixelFormat
            sheet.PixelFormat = XmlUtilities.GetAttributeEnumValue<PixelFormats>(sheetNode, "PixelFormat", PixelFormats.RGBA_5551);

            // Are we forcing 32-bit pixel formats?
            if (this.force32Bit)
            {
                // Change any 16-bit pixel formats to 32-bit
                switch (sheet.PixelFormat)
                {
                    case PixelFormats.RGB_565:
                        sheet.PixelFormat = PixelFormats.RGB_888;
                        break;

                    case PixelFormats.RGBA_4444:
                    case PixelFormats.RGBA_5551:
                        sheet.PixelFormat = PixelFormats.RGBA_8888;
                        break;
                }
            }

            // Process any child Image or Images elements
            ImageProcessParameters defaults = new ImageProcessParameters();
            foreach (XmlNode childImagesNode in sheetNode.SelectNodes("Images"))
                ProcessImages(atlas, sheet, childImagesNode, defaults);
            foreach (XmlNode childImageNode in sheetNode.SelectNodes("Image"))
                ProcessImage(atlas, sheet, childImageNode, defaults);

            // Find the best fitting sheet size for the images
            // ----------------------------------------------------

            Context.LogVerbose("+-- Atlasing Sheet [Images: " + sheet.Images.Count + "]");
            Context.LogVerbose("  +-- Pixel Format: " + sheet.PixelFormat.ToString());

            int sheetSize = 16;
            bool fitFound = false;

            // Loop until we find a sheet that fits
            while(fitFound == false)
            {
                // Sort the images by height
                sheet.Images.Sort(delegate(AtlasImage image1, AtlasImage image2)
                {
                    return image2.Height.CompareTo(image1.Height);
                });

                // Test this sheet size
                if(DoImagesFitInSheet(sheetSize, sheet.Images) == true)
                {
                    fitFound = true;
                    break;
                }

                // Sort the images by width
                sheet.Images.Sort(delegate(AtlasImage image1, AtlasImage image2)
                {
                    return image2.Width.CompareTo(image1.Width);
                });

                // Test this sheet size
                if(DoImagesFitInSheet(sheetSize, sheet.Images) == true)
                {
                    fitFound = true;
                    break;
                }

                // No fit yet, double the sheet size
                sheetSize *= 2;

                // Is the sheet too big?
                if (sheetSize > MAX_TEXTURE_SIZE)
                {
                    throw new BuildException("Unable to fit all images onto a sheet.  Max sheet size: " + MAX_TEXTURE_SIZE);
                }
            }

            // Use this sheet size
            sheet.Width = sheetSize;
            sheet.Height = sheetSize;

            // Place all the images into the sheet
            // --------------------------------------

            // Create the texture for this sheet
            sheet.Texture = new Surface(sheet.Width, sheet.Height);

            // Loop through the images
            int currentImageId = 0;
            foreach (AtlasImage image in sheet.Images)
            {
                currentImageId++;

                // Blit the image into the sheet texture
                sheet.Texture.Blit(image.Texture, 0, 0, (int)image.SheetPosition.X, (int)image.SheetPosition.Y, image.Texture.Width, image.Texture.Height);

                // Log the atlas'd image
                Context.LogVerbose(string.Format("  +-- Image Size({0}x{1}) atlas to ({2},{3}) - [Image:{4}/{5}]", image.Texture.Width, image.Texture.Height, (int)image.SheetPosition.X, (int)image.SheetPosition.Y, currentImageId, sheet.Images.Count));
            }
        }

        /// <summary>
        /// Checks if the images in the sheet will fit a texture of the given size
        /// </summary>
        private bool DoImagesFitInSheet(int sheetSize, List<AtlasImage> images)
        {
            // Create a region for the given size sheet
            System.Drawing.Region region = new System.Drawing.Region(new System.Drawing.Rectangle(0, 0, sheetSize, sheetSize));

            // Loop through the images
            foreach (AtlasImage image in images)
            {
                // Track if we placed the image
                bool imagePlaced = false;

                // Loop through all the rectangular scans in the region
                System.Drawing.RectangleF[] regionScans = region.GetRegionScans(new System.Drawing.Drawing2D.Matrix());
                foreach (System.Drawing.RectangleF regionScan in regionScans)
                {
                    // Test the whole x strip to see if the rect fits in the region
                    bool rectFits = true;
                    for (float x = regionScan.X; x < regionScan.X + image.CroppedWidth; x += 1f)
                        if (region.IsVisible(x, regionScan.Y) == false)
                            rectFits = false;
                    if (rectFits == false)
                        continue;
                    
                    // Test the whole y strip to see if the rect fits in the region
                    rectFits = true;
                    for (float y = regionScan.Y; y < regionScan.Y + image.CroppedHeight; y += 1f)
                        if (region.IsVisible(regionScan.X, y) == false)
                            rectFits = false;
                    if (rectFits == false)
                        continue;

                    // Remove the rectangle from the region
                    region.Xor(new System.Drawing.Rectangle((int)regionScan.X, (int)regionScan.Y, image.Texture.Width, image.Texture.Height));

                    // Set the texture coords of the image
                    float texelsPerX = 1.0f / sheetSize;
                    float texelsPerY = 1.0f / sheetSize;
                    image.TopLeftTexCoord.X = (regionScan.X) * texelsPerX;
                    image.TopLeftTexCoord.Y = (regionScan.Y) * texelsPerY;
                    image.BottomRightTexCoord.X = (regionScan.X + image.Texture.Width) * texelsPerX;
                    image.BottomRightTexCoord.Y = (regionScan.Y + image.Texture.Height) * texelsPerY;

                    // Store the location where the image should be blitted into the sheet
                    image.SheetPosition.X = regionScan.X;
                    image.SheetPosition.Y = regionScan.Y;

                    // The image was placed, break out of the loop
                    imagePlaced = true;
                    break;
                }

                // Did the image get placed?
                if (imagePlaced == false)
                    return false;  // This image wont fit..
            }

            // If we got here, then all the images will fit!
            // Log the sheet size
            Context.LogVerbose("  +-- Sheet Size: " + sheetSize + "x" + sheetSize);

            // Calculate the number of pixels that were un-unsed
            int totalSheetPixels = sheetSize * sheetSize;
            int unusedPixels = 0;
            System.Drawing.RectangleF[] regionScans2 = region.GetRegionScans(new System.Drawing.Drawing2D.Matrix());
            foreach (System.Drawing.RectangleF rect in regionScans2)
            {
                unusedPixels += (int)(rect.Width * rect.Height);
            }

            // If we used < half the pixels, throw a warning
            int percentSheetUsed = 100 - (unusedPixels * 100) / totalSheetPixels;
            if (percentSheetUsed < 50)
            {
                Context.LogWarn("  +-- Warning: only " + percentSheetUsed + "% of the sheet texture is used, consider resizing the images or splitting into multiple sheets.");
            }
            else
            {
                Context.LogVerbose("  +-- Sheet usage: " + percentSheetUsed + "%");
            }
                
            // Return success
            return true;
        }

        /// <summary>
        /// Processes an Images element in the atlas source xml
        /// </summary>
        private void ProcessImages(Atlas atlas, AtlasSheet sheet, XmlNode imagesNode, ImageProcessParameters defaultImageProcessParameters)
        {
            // Get any custom image processing parameters for this image set
            ImageProcessParameters imageProcessParameters = new ImageProcessParameters(imagesNode, defaultImageProcessParameters);

            // Process any child Image or Images elements
            foreach (XmlNode childImagesNode in imagesNode.SelectNodes("Images"))
                ProcessImages(atlas, sheet, childImagesNode, imageProcessParameters);
            foreach (XmlNode childImageNode in imagesNode.SelectNodes("Image"))
                ProcessImage(atlas, sheet, childImageNode, imageProcessParameters);
        }

        /// <summary>
        /// Processes an Image element in the atlas source xml
        /// </summary>
        private void ProcessImage(Atlas atlas, AtlasSheet sheet, XmlNode imageNode, ImageProcessParameters defaultImageProcessParameters)
        {
            // Get image processing parameters for this image
            ImageProcessParameters imageProcessParameters = new ImageProcessParameters(imageNode, defaultImageProcessParameters);

            // Get the path & name attributes for this image
            XmlUtilities.VerifyAttributesExist(imageNode, true, "Path");
            string imageName = XmlUtilities.GetAttribute(imageNode, "Name", string.Empty);
            string pathFromXml = XmlUtilities.GetAttribute(imageNode, "Path", string.Empty);

            // Try to map the path to multiple files
            string[] filePaths = Directory.GetFiles(atlas.ContentFolder, pathFromXml);
            int numFiles = filePaths.GetUpperBound(0) + 1;

            // Does the file not exist?
            if (numFiles == 0)
            {
                throw new BuildException("Image file(s) not found: " + Path.Combine(atlas.ContentFolder, pathFromXml));
            }

            // Dont allow the imageName field if we have more than 1 file
            if (numFiles > 1 && XmlUtilities.AttributeExists(imageNode, "Name") == true)
            {
                throw new BuildException("The Name attribute is NOT allowed on <Image> elements with a wildcard in the Path. [" + Path.Combine(atlas.ContentFolder, pathFromXml) + "]");
            }

            // Loop through the files
            foreach (string imageFullPath in filePaths)
            {
                string imagePath = imageFullPath.Substring(atlas.ContentFolder.Length + 1);

                // If the name is blank, use the path, minus the extension
                if (numFiles > 1 || string.IsNullOrEmpty(imageName))
                {
                    int dotIndex = imagePath.LastIndexOf('.');
                    if (dotIndex >= 0)
                        imageName = imagePath.Substring(0, dotIndex);
                    else
                        imageName = imagePath;
                }

                // Log the image processing
                Context.LogVerbose("+-- Processing Image: " + imageName);

                // Validate the image resource name is unique
                if (atlas.UniqueNames.Contains(imageName) == true)
                    throw new BuildException("Duplicate resource name detected: " + imageName);
                atlas.UniqueNames.Add(imageName);

                // Get the full path to the image file & set it as a dependency
                if (File.Exists(imageFullPath) == false)
                    throw new BuildException("Image file not found: " + imageFullPath);

                // Create a new image content
                AtlasImage image = new AtlasImage();
                image.Name = imageName;
                image.Index = atlas.Images.Count;
                image.Sheet = sheet;

                // Add the image to the master list on the Atlas object
                atlas.ImagesByName.Add(imageName, image);
                atlas.Images.Add(image);

                Context.LogVerbose("  +-- Loading PNG: " + imageFullPath);

                // Load the image
                Surface imageTexture = Surface.FromFile(imageFullPath);

                // Add the dependency to the image
                string imagePathRelativeToContent = imageFullPath.Substring(Context.ContentFolder.Length + 1);
                Context.AddInputDependency(imagePathRelativeToContent);

                // Determine the reference point
                // -----------------------------------

                if (imageProcessParameters.UseRefPointColor == false)
                {
                    // Determine the X reference point
                    if (imageProcessParameters.RefPoint[0] == ImageProcessParameters.LEFT)
                        image.ReferencePoint.X = 0.0f;
                    else if (imageProcessParameters.RefPoint[0] == ImageProcessParameters.RIGHT)
                        image.ReferencePoint.X = 1.0f;
                    else if (imageProcessParameters.RefPoint[0] == ImageProcessParameters.CENTER)
                        image.ReferencePoint.X = 0.5f;
                    else
                        image.ReferencePoint.X = imageProcessParameters.RefPoint[0] / (float)(imageTexture.Width - 1);

                    // Determine the Y reference point
                    if (imageProcessParameters.RefPoint[1] == ImageProcessParameters.TOP)
                        image.ReferencePoint.Y = 0.0f;
                    else if (imageProcessParameters.RefPoint[1] == ImageProcessParameters.BOTTOM)
                        image.ReferencePoint.Y = 1.0f;
                    else if (imageProcessParameters.RefPoint[1] == ImageProcessParameters.CENTER)
                        image.ReferencePoint.Y = 0.5f;
                    else
                        image.ReferencePoint.Y = imageProcessParameters.RefPoint[1] / (float)(imageTexture.Height - 1);

                    Context.LogVerbose("  +-- RefPoint(" + image.ReferencePoint.X + "," + image.ReferencePoint.Y + ")   [by number]");
                }
                else
                {
                    // Read the ref point out of the image
                    image.ReferencePoint = GetPointByColor(imageProcessParameters.RefPointColor, imageTexture);

                    // Remove the ref-point color
                    imageTexture.ReplaceColor(imageProcessParameters.RefPointColor, Color.TransparentBlack);

                    Context.LogVerbose("  +-- RefPoint(" + image.ReferencePoint.X + "," + image.ReferencePoint.Y + ")   [by color]");
                }

                // Set any Pixel-coord-based Child Points
                // -------------------------------------------

                foreach (int[] childPointCoords in imageProcessParameters.ChildPoints)
                {
                    // Normalize the pixel coordinates add them to the frame
                    Vector2 childPoint = new Vector2(
                        childPointCoords[0] / (float)(imageTexture.Width - 1),
                        childPointCoords[1] / (float)(imageTexture.Height - 1)
                        );
                    image.ChildPoints.Add(childPoint);

                    Context.LogVerbose("  +-- ChildPoint(" + childPoint.X + "," + childPoint.Y + ")   [by number]");
                }

                // Read out any Color-based Child Points
                // ----------------------------------------

                foreach (Color childPointColor in imageProcessParameters.ChildPointColors)
                {
                    // Read the child point out of the image
                    Vector2 childPoint = GetPointByColor(childPointColor, imageTexture);
                    if (childPoint.X >= 0)
                    {
                        image.ChildPoints.Add(childPoint);

                        // Remove the child point color
                        imageTexture.ReplaceColor(childPointColor, Color.TransparentBlack);

                        Context.LogVerbose("  +-- ChildPoint(" + childPoint.X + "," + childPoint.Y + ")   [by color]");
                    }
                    else
                    {
                        Context.LogWarn("Color (" + childPointColor.R + "," + childPointColor.G + "," + childPointColor.B + ") not found for child point in image: " + imageName);
                    }
                }

                // Do the color key
                // ------------------------------------

                if (imageProcessParameters.ColorKey != Color.TransparentBlack)
                {
                    imageTexture.ReplaceColor(imageProcessParameters.ColorKey, Color.TransparentBlack);
                    Context.LogVerbose(" +-- Replaced color key \"" + imageProcessParameters.ColorKey.R + "," + imageProcessParameters.ColorKey.G + "," + imageProcessParameters.ColorKey.B + ") with TransparentBlack");
                }

                // Find transparent edges
                // -----------------------------------

                // Check the 4 edges of the image, to see if we need to preserve translucency on them
                bool edgeTransparencyLeft = true;
                bool edgeTransparencyRight = true;
                bool edgeTransparencyTop = true;
                bool edgeTransparencyBottom = true;

                // If ANY pixel in an edge is > epsilon-alpha, then we turn off smoothing for that edge
                byte epsilonAlpha = 128;

                // Walk through the left & right sides
                for (int y = 0; y < imageTexture.Height; y++)
                {
                    // Check left 
                    Color c = imageTexture.GetPixel(0, y);
                    if (c.A > epsilonAlpha)
                        edgeTransparencyLeft = false;

                    // Check right 
                    c = imageTexture.GetPixel(imageTexture.Width - 1, y);
                    if (c.A > epsilonAlpha)
                        edgeTransparencyRight = false;
                }

                // Walk through the top & bottom sides
                for (int x = 0; x < imageTexture.Width; x++)
                {
                    // Check top 
                    Color c = imageTexture.GetPixel(x, 0);
                    if (c.A > epsilonAlpha)
                        edgeTransparencyTop = false;

                    // Check bottom 
                    c = imageTexture.GetPixel(x, imageTexture.Height - 1);
                    if (c.A > epsilonAlpha)
                        edgeTransparencyBottom = false;
                }

                // Scale the image
                // -----------------------------------

                // Multiply the image scale with the master scale
                Vector2 imageScale = imageProcessParameters.Scale * this.masterImageScale;

                // Cap the scale to a max of 1.0f
                if (imageScale.X > 1.0f)
                    imageScale.X = 1.0f;
                if (imageScale.Y > 1.0f)
                    imageScale.Y = 1.0f;

                // Resize the image (if scale is less than 1)
                if (imageScale.X < 1.0f || imageScale.Y < 1.0f)
                {
                    // Calculate the new image size
                    int newWidth = (int)(imageTexture.Width * imageScale.X);
                    int newHeight = (int)(imageTexture.Height * imageScale.Y);

                    // Create the new image
                    Surface scaledImage = new Surface(newWidth, newHeight);

                    // Scale Blit the image
                    scaledImage.Blit(imageTexture, 0, 0, imageTexture.Width, imageTexture.Height, 0, 0, newWidth, newHeight, TextureFilterMode.Bilinear);

                    // Log the scaling
                    Context.LogVerbose("  +-- Scaled Image from(" + imageTexture.Width + "," + imageTexture.Height + ") to (" + newWidth + "," + newHeight + ")");

                    // Use the new image
                    imageTexture = scaledImage;
                }
                else
                {
                    // Log the image size
                    Context.LogVerbose("  +-- Pixel Size(" + imageTexture.Width + "," + imageTexture.Height + ")");
                }

                image.Width = imageTexture.Width;
                image.Height = imageTexture.Height;


                // Crop the whitespace off the image
                // ------------------------------------------

                // Setup the row/column positions
                int xStart = 0, xEnd = imageTexture.Width - 1, yStart = 0, yEnd = imageTexture.Height - 1;
                bool foundNonTransparent = false;

                // Find the first X non-transparent pixels
                for (xStart = 0; xStart < imageTexture.Width; xStart++)
                {
                    // Loop through the column and see if there is a non-transparent pixel
                    for (int y = 0; y < imageTexture.Height; y++)
                    {
                        if (imageTexture.GetPixel(xStart, y).A > 0)
                        {
                            foundNonTransparent = true;
                            break;
                        }
                    }

                    if (foundNonTransparent)
                        break;
                }

                // Did we go through the whole image and find only transparent pixels?
                if (foundNonTransparent == false)
                {
                    // Log the transparency
                    Context.LogWarn("Image is completely transparent, skipping white space cropping: " + imageName);

                    // Use the full texture
                    image.Texture = imageTexture;
                    image.CroppedWidth = image.Width;
                    image.CroppedHeight = image.Height;
                    image.CroppedImageOffsetX = 0;
                    image.CroppedImageOffsetY = 0;
                }
                else
                {
                    // Find the last X with non-transparent pixels
                    foundNonTransparent = false;
                    for (xEnd = imageTexture.Width - 1; xEnd >= 0; xEnd--)
                    {
                        // Loop through the column and see if there is a non-transparent pixel
                        for (int y = 0; y < imageTexture.Height; y++)
                        {
                            if (imageTexture.GetPixel(xEnd, y).A > 0)
                            {
                                foundNonTransparent = true;
                                break;
                            }
                        }

                        if (foundNonTransparent)
                            break;
                    }

                    // Find the first Y non-transparent pixel
                    foundNonTransparent = false;
                    for (yStart = 0; yStart < imageTexture.Height; yStart++)
                    {
                        // Loop through the row and see if there is a non-transparent pixel
                        for (int x = 0; x < imageTexture.Width; x++)
                        {
                            if (imageTexture.GetPixel(x, yStart).A > 0)
                            {
                                foundNonTransparent = true;
                                break;
                            }
                        }

                        if (foundNonTransparent)
                            break;
                    }

                    // Find the last Y with non-transparent pixel
                    foundNonTransparent = false;
                    for (yEnd = imageTexture.Height - 1; yEnd >= 0; yEnd--)
                    {
                        // Loop through the column and see if there is a non-transparent pixel
                        for (int x = 0; x < imageTexture.Width; x++)
                        {
                            if (imageTexture.GetPixel(x, yEnd).A > 0)
                            {
                                foundNonTransparent = true;
                                break;
                            }
                        }

                        if (foundNonTransparent)
                            break;
                    }

                    // Log the cropping details
                    Context.LogVerbose("  +-- White space cropping offsets (" + xStart + ", " + yStart + ")-(" + xEnd + ", " + yEnd + ")");

                    // Store the cropped image details
                    image.CroppedImageOffsetX = xStart;
                    image.CroppedImageOffsetY = yStart;
                    image.CroppedWidth = xEnd - xStart + 1;
                    image.CroppedHeight = yEnd - yStart + 1;

                    // Create a new texture to hold the cropped data
                    Surface croppedTexture = new Surface(image.CroppedWidth, image.CroppedHeight);

                    // Blit the cropped data to the new texture
                    croppedTexture.Blit(imageTexture, image.CroppedImageOffsetX, image.CroppedImageOffsetY, 0, 0, image.CroppedWidth, image.CroppedHeight);

                    // Replace the original texture with the cropped one
                    image.Texture = croppedTexture;
                }

                // Transparent Edge Smoothing
                // ----------------------------------------

                // Do we need to pad the image for smoothing?
                if (edgeTransparencyLeft || edgeTransparencyRight || edgeTransparencyTop || edgeTransparencyBottom)
                {
                    // Adjust the cropped image size & positioning
                    if (edgeTransparencyLeft)
                    {
                        image.CroppedImageOffsetX--;
                        image.CroppedWidth++;
                    }
                    if (edgeTransparencyRight)
                    {
                        image.CroppedWidth++;
                    }
                    if (edgeTransparencyTop)
                    {
                        image.CroppedImageOffsetY--;
                        image.CroppedHeight++;
                    }
                    if (edgeTransparencyBottom)
                    {
                        image.CroppedHeight++;
                    }

                    // Create a new image with the padding space
                    Surface paddedImage = new Surface(image.CroppedWidth, image.CroppedHeight);

                    // Blit the existing image with padding
                    int xOffset = 0;
                    int yOffset = 0;
                    if (edgeTransparencyLeft)
                        xOffset = 1;
                    if (edgeTransparencyTop)
                        yOffset = 1;

                    paddedImage.Blit(image.Texture, 0, 0, xOffset, yOffset, image.Texture.Width, image.Texture.Height);

                    // Use the new padded image
                    image.Texture = paddedImage;
                }

                // Save the image into the sheet
                // ----------------------------------------

                // Add the image to the sheet
                sheet.Images.Add(image);

            } // foreach imagePath
        }

        /// <summary>
        /// Processes an Animation element in the atlas source xml
        /// </summary>
        private void ProcessAnimation(Atlas atlas, XmlNode animationNode)
        {
            // Create a new animation content & load the basic settings
            AtlasAnimation animation = new AtlasAnimation();
            XmlUtilities.VerifyAttributesExist(animationNode, true, "Name");
            animation.Name = XmlUtilities.GetAttribute(animationNode, "Name", "");
            animation.FPS = XmlUtilities.GetAttribute(animationNode, "FPS", 10.0f);

            // Validate the animation name is unique
            if (atlas.UniqueNames.Contains(animation.Name) == true)
                throw new BuildException("Duplicate resource name detected: " + animation.Name);
            atlas.UniqueNames.Add(animation.Name);

            // Process all the Frame elements
            foreach (XmlNode frameNode in animationNode.SelectNodes("Frame"))
            {
                XmlUtilities.VerifyAttributesExist(frameNode, true, "Image");
                string imageName = XmlUtilities.GetAttribute(frameNode, "Image", string.Empty);

                // Does the image name have a wildcard?
                if (imageName.Contains('*'))
                {
                    // Build a list of the images that match the given name
                    List<AtlasImage> matchingImages = new List<AtlasImage>();

                    // Loop through every image
                    foreach(AtlasImage image in atlas.Images)
                    {
                        // Does this image name match?
                        if (System.Text.RegularExpressions.Regex.IsMatch(image.Name, imageName))
                        {
                            // Add the image to this animation
                            matchingImages.Add(image);
                        }
                    }
                    
                    // Sort the matching images by name
                    matchingImages.Sort(
                        delegate(AtlasImage image1, AtlasImage image2) 
                        { 
                            return image1.Name.CompareTo(image2.Name); 
                        } );

                    // Add all the matching images to the animation
                    foreach(AtlasImage image in matchingImages)
                        animation.Images.Add(image);
                }
                else
                {
                    // Validate the image name exists
                    if (atlas.ImagesByName.ContainsKey(imageName) == false)
                        throw new BuildException("Unknown image [" + imageName + "] in animation: " + animation.Name);

                    // Add the image to this animation
                    animation.Images.Add(atlas.ImagesByName[imageName]);
                }
            }

            // Empty anims are not allowed
            if (animation.Images.Count == 0)
                throw new BuildException("Animation detected with no frames: " + animation.Name);

            // Add the animation to the atlas
            atlas.Animations.Add(animation);
        }

        /// <summary>
        /// Search for a point of a certain color within a Surface
        /// </summary>
        /// <param name="color"></param>
        /// <param name="imageTexture"></param>
        /// <returns></returns>
        private Vector2 GetPointByColor(Color color, Surface surface)
        {
            Vector2 point = new Vector2(-1, 0);

            // Walk down the rows
            for (int y = 0; y < surface.Height; y++)
            {
                // Walk through the columns
                for (int x = 0; x < surface.Width; x++)
                {
                    // Is this 2 matching verticle pixels?
                    if (y < surface.Height - 1 && surface.GetPixel(x, y) == color && surface.GetPixel(x, y + 1) == color)
                        point.X = x / (float)(surface.Width - 1);

                    // Is this 2 matching horizonatl pixels?
                    if (x < surface.Width - 1 && surface.GetPixel(x, y) == color && surface.GetPixel(x + 1, y) == color)
                        point.Y = y / (float)(surface.Height - 1);
                }
            }

            return point;
        }

        /// <summary>
        /// Writes out all the atlas data to the appropriate files
        /// </summary>
        /// <param name="atlas"></param>
        private void Write(Atlas atlas)
        {
            // Get the relative & absolute paths to the gdkatlas file
            string atlasRelPath = Path.ChangeExtension(Asset.Path, "gdkatlas");
            string atlasFullPath = Path.Combine(Context.OutputFolder, atlasRelPath);

            // Ensure the output directory exists
            string atlasDirectory = Path.GetDirectoryName(atlasFullPath);
            Directory.CreateDirectory(atlasDirectory);

            // Add the dependency to this output gdkatlas file
            Context.AddOutputDependency(atlasRelPath);

            // Create the gdkatlas file
            FileStream atlasFile = File.Create(atlasFullPath, 2048, FileOptions.None);
            BinaryWriter writer = new BinaryWriter(atlasFile);

            // Write the sheets
            // ----------------------

            writer.Write((short)atlas.Sheets.Count);

            // Loop through the sheets
            foreach (AtlasSheet sheet in atlas.Sheets)
            {
                // Write out the sheet information
                writer.Write((short)sheet.Width);
                writer.Write((short)sheet.Height);
                writer.Write((byte)sheet.PixelFormat);

				// Build the relative & full path to the sheet gdkimage
                string sheetRelPath = Path.ChangeExtension(atlasRelPath, null);
                sheetRelPath += "_sheet_" + sheet.Index + ".gdkimage";
                string sheetFullPath = Path.Combine(Context.OutputFolder, sheetRelPath);

                // Add the dependency to this output gdkimage file
                Context.AddOutputDependency(sheetRelPath);

				// Write out the sheet PNG
				sheet.Texture.SaveToGdkImage(sheetFullPath, sheet.PixelFormat);
				break;
            }

            // Write the images
            // ----------------------

            writer.Write((short)atlas.Images.Count);

            // Loop through the images
            foreach (AtlasImage image in atlas.Images)
            {
                writer.Write((ushort)image.Name.Length);
                writer.Write((char[])image.Name.ToCharArray());

                writer.Write((short)image.Sheet.Index);
                writer.Write((short)image.Width);
                writer.Write((short)image.Height);

                writer.Write((short)image.CroppedWidth);
                writer.Write((short)image.CroppedHeight);
                writer.Write((short)image.CroppedImageOffsetX);
                writer.Write((short)image.CroppedImageOffsetY);

                writer.Write((float)image.TopLeftTexCoord.X);
                writer.Write((float)image.TopLeftTexCoord.Y);
                writer.Write((float)image.BottomRightTexCoord.X);
                writer.Write((float)image.BottomRightTexCoord.Y);

                writer.Write((float)image.ReferencePoint.X);
                writer.Write((float)image.ReferencePoint.Y);

                writer.Write((short)image.ChildPoints.Count);
                foreach (Vector2 childPoint in image.ChildPoints)
                {
                    writer.Write((float)childPoint.X);
                    writer.Write((float)childPoint.Y);
                }
            }

            // Write the animations
            // ---------------------------

            // Write the number of animations
            writer.Write((short)atlas.Animations.Count);

            // Loop through the animations
            foreach(AtlasAnimation animation in atlas.Animations)
            {
                // Write out the animation info
                writer.Write((short)animation.Name.Length);
                writer.Write((char[])animation.Name.ToCharArray());
                writer.Write((float)animation.FPS);

                // Write the images used by this animation
                writer.Write((short)animation.Images.Count);
                foreach (AtlasImage image in animation.Images)
                    writer.Write((short)image.Index);
            }

            // Close the file
            atlasFile.Flush();
            atlasFile.Close();
        }
    }

    // *****************************************************************
    internal class ImageProcessParameters
    {
        // Constants
        public const int LEFT = 100001;
        public const int RIGHT = 100002;
        public const int TOP = 100003;
        public const int BOTTOM = 100004;
        public const int CENTER = 100005;

        // Properties
        public Color ColorKey = Color.TransparentBlack;
        public int[] RefPoint = new int[2] { CENTER, BOTTOM };
        public Color RefPointColor = Color.Green;
        public bool UseRefPointColor = false;
        public List<int[]> ChildPoints = new List<int[]>();
        public List<Color> ChildPointColors = new List<Color>();
        public Vector2 Scale = Vector2.One;

        // *******************************
        public ImageProcessParameters()
        {
        }

        // *******************************
        public ImageProcessParameters(XmlNode node)
        {
            // Load the parameters from the xml
            LoadFromImageAttributes(node);
        }

        // *******************************
        public ImageProcessParameters(XmlNode node, ImageProcessParameters defaults)
        {
            // Copy parameters from the defaults
            CopyParameters(defaults);

            // Now load any new parameters from the xml
            LoadFromImageAttributes(node);
        }

        // *******************************
        private void CopyParameters(ImageProcessParameters original)
        {
            // Copy the specified parameters
            this.ColorKey = original.ColorKey;
            this.RefPoint = (int[])original.RefPoint.Clone();
            this.RefPointColor = original.RefPointColor;
            this.UseRefPointColor = original.UseRefPointColor;
            foreach (Color color in original.ChildPointColors)
                this.ChildPointColors.Add(color);
            foreach (int[] values in original.ChildPoints)
                this.ChildPoints.Add((int[])values.Clone());
            this.Scale = original.Scale;
        }

        // ******************************
        private void LoadFromImageAttributes(XmlNode node)
        {
            foreach (XmlAttribute attribute in node.Attributes)
            {
                // What attribute is this?
                if (attribute.Name.ToLower() == "refpointx")
                {
                    // RefPointX
                    if (attribute.Value.ToLower() == "left")
                        this.RefPoint[0] = LEFT;
                    else if (attribute.Value.ToLower() == "right")
                        this.RefPoint[0] = RIGHT;
                    else if (attribute.Value.ToLower() == "center")
                        this.RefPoint[0] = CENTER;
                    else
                        this.RefPoint[0] = Int32.Parse(attribute.Value);

                    this.UseRefPointColor = false;
                }
                else if (attribute.Name.ToLower() == "refpointy")
                {
                    // RefPointY
                    if (attribute.Value.ToLower() == "top")
                        this.RefPoint[1] = TOP;
                    else if (attribute.Value.ToLower() == "bottom")
                        this.RefPoint[1] = BOTTOM;
                    else if (attribute.Value.ToLower() == "center")
                        this.RefPoint[1] = CENTER;
                    else
                        this.RefPoint[1] = Int32.Parse(attribute.Value);

                    this.UseRefPointColor = false;
                }
                else if (attribute.Name.ToLower() == "refpointcolor")
                {
                    // RefPointColor
                    this.RefPointColor = Color.Parse(attribute.Value);
                    this.UseRefPointColor = true;
                }
                else if (attribute.Name.ToLower() == "colorkey")
                {
                    // ColorKey
                    this.ColorKey = Color.Parse(attribute.Value);
                }
                else if (attribute.Name.ToLower().StartsWith("childpointcolor") == true)
                {
                    // ChildPointColor___
                    this.ChildPointColors.Add(Color.Parse(attribute.Value));
                }
                else if (attribute.Name.ToLower().StartsWith("childpoint") == true)
                {
                    // ChildPoint___
                    this.ChildPoints.Add(StringUtilities.ParseIntArray(attribute.Value, 2));
                }
                else if (attribute.Name.ToLower().StartsWith("imagescale") == true)
                {
                    // ChildPoint___
                    this.Scale = Vector2.Parse(attribute.Value, true);
                }

            }
        }
    }
}
