using System;
using System.Collections.Generic;
using System.IO;

namespace Gdk
{
    /// <summary>
    /// Wrapping modes for texture coordinates outside the 0-1 range
    /// </summary>
    public enum TextureWrapMode
    {
        Wrap,
        Mirror,
        Clamp
    }

    /// <summary>
    /// Filtering modes for sub-pixel texels
    /// </summary>
    public enum TextureFilterMode
    {
        Closest,
        Bilinear
    }

    /// <summary>
    /// 90 degree angle rotations
    /// </summary>
    public enum RightAngles
    {
        Rotate90,
        Rotate180,
        Rotate270
    }

    /// <summary>
    /// Manages texture and surface information
    /// </summary>
    public class Surface
    {
        // Member Fields
        public Color[] Pixels;

        // Member Variables
        private int width;
        private int height;

        /// <summary>
        /// Gets or Sets the width of the surface image
        /// </summary>
        public int Width
        {
            get { return this.width; }
            set { this.width = value; CreateBuffer(); }
        }

        /// <summary>
        /// Gets or Sets the height of the surface image
        /// </summary>
        public int Height
        {
            get { return this.height; }
            set { this.height = value; CreateBuffer(); }
        }

        /// <summary>
        /// Constructor
        /// </summary>
        public Surface(int width, int height)
        {
            this.width = width;
            this.height = height;

            CreateBuffer();
        }

        /// <summary>
        /// Creates the internal pixel buffer
        /// </summary>
        private void CreateBuffer()
        {
            // Create the image buffer
            int numPixels = width * height;
            Pixels = new Color[numPixels];
        }

        #region Drawing Operations

        /// <summary>
        /// Clears all the pixels in the surface to the given color
        /// </summary>
        public void Clear()
        {
            this.Clear(0,0,0,0);
        }

        /// <summary>
        /// Clears all the pixels in the surface to the given color
        /// </summary>
        public void Clear(Color color)
        {
            this.Clear(color.R, color.G, color.B, color.A);
        }

        /// <summary>
        /// Clears all the pixels in the surface to the given color
        /// </summary>
        public void Clear(byte r, byte g, byte b, byte a)
        {
            for (int index = 0; index < Pixels.Length; index++)
            {
                this.Pixels[index].R = r;
                this.Pixels[index].G = g;
                this.Pixels[index].B = b;
                this.Pixels[index].A = a;
            }
        }

        /// <summary>
        /// Sets the color of the pixel at the given coordinates
        /// </summary>
        public void SetPixel(int x, int y, Color color)
        {
            this.Pixels[y * this.width + x] = color;
        }

        /// <summary>
        /// Gets the color of the pixel at the given coordinates
        /// </summary>
        public Color GetPixel(int x, int y)
        {
            return this.Pixels[y * this.width + x];
        }

        /// <summary>
        /// Blits a fixed size rectangular area from the source Surface onto this surface
        /// </summary>
        public void Blit(Surface source, int srcX, int srcY, int destX, int destY, int width, int height)
        {
            // Loop through the rectangle pixels
            for (int y = 0; y < height; y++)
            {
                for (int x = 0; x < width; x++)
                {
                    Color c = source.GetPixel(x + srcX, y + srcY);
                    this.SetPixel(x + destX, y + destY, c);
                }
            }
        }

        /// <summary>
        /// Blits a fixed size rectangular area from the source Surface onto this surface, also rotates by a right angle
        /// </summary>
        public void Blit(Surface source, int srcX, int srcY, int destX, int destY, int width, int height, RightAngles angle )
        {
            // Which type of rotation are we doing?
            switch (angle)
            {
                case RightAngles.Rotate90:
                    // Loop through the rectangle pixels
                    for (int y = 0; y < height; y++)
                    {
                        for (int x = 0; x < width; x++)
                        {
                            Color c = source.GetPixel(x + srcX, y + srcY);
                            this.SetPixel(height - 1 - y + destX, x + destY, c);
                        }
                    }
                    break;

                case RightAngles.Rotate180:
                    // Loop through the rectangle pixels
                    for (int y = 0; y < height; y++)
                    {
                        for (int x = 0; x < width; x++)
                        {
                            Color c = source.GetPixel(x + srcX, y + srcY);
                            this.SetPixel(width - x - 1 + destX, height - y - 1 + destY, c);
                        }
                    }
                    break;

                case RightAngles.Rotate270:
                    // Loop through the rectangle pixels
                    for (int y = 0; y < height; y++)
                    {
                        for (int x = 0; x < width; x++)
                        {
                            Color c = source.GetPixel(x + srcX, y + srcY);
                            this.SetPixel(y + destX, width - x - 1 + destY, c);
                        }
                    }
                    break;
            }
        }

        /// <summary>
        /// Replaces all the pixels of color (originalColor) with the (newColor)
        /// </summary>
        public void ReplaceColor(Color originalColor, Color newColor)
        { 
            // Loop through all the pixels
            for (int index = 0; index < this.Pixels.Length; index++)
            {
                // Check the color (and replace if it matchs the original)
                if (this.Pixels[index] == originalColor)
                    this.Pixels[index] = newColor;
            }
        }

        #endregion

        #region Savings & Loading methods

        /// <summary>
        /// Returns the surface in a GDI Bitmap
        /// </summary>
        public System.Drawing.Bitmap GetGDIBitmap()
        {
            // Create the bitmap to hold the image data
            System.Drawing.Bitmap bitmap = new System.Drawing.Bitmap(
                this.Width,
                this.Height,
                System.Drawing.Imaging.PixelFormat.Format32bppArgb
                );

            // Lock the bitmap
            System.Drawing.Imaging.BitmapData bitmapData = bitmap.LockBits(
                new System.Drawing.Rectangle(0, 0, bitmap.Width, bitmap.Height),
                System.Drawing.Imaging.ImageLockMode.WriteOnly,
                System.Drawing.Imaging.PixelFormat.Format32bppArgb
                );

            // Create a buffer to hold the translated bitmap data
            int bytes = bitmapData.Stride * bitmapData.Height;
            byte[] buffer = new byte[bytes];

            // Loop through the image rows (y)
            for (int y = 0; y < this.Height; y++)
            {
                int rowOffset = bitmapData.Stride * y;

                // Loop through the x pixels
                for (int x = 0; x < this.Width; x++)
                {
                    int columnOffset = x * 4;

                    // Get this pixel as an RGBA 32-bit
                    Color pixel = Pixels[y * this.Width + x];
                 
                    // Copy the pixel to the row pixel array
                    buffer[rowOffset + columnOffset + 0] = pixel.B;
                    buffer[rowOffset + columnOffset + 1] = pixel.G;
                    buffer[rowOffset + columnOffset + 2] = pixel.R;
                    buffer[rowOffset + columnOffset + 3] = pixel.A;
                }
            }

            // Copy the buffer into the locked bitmap 
            System.Runtime.InteropServices.Marshal.Copy(
                buffer,
                0,
                bitmapData.Scan0,
                bytes
                );

            // Unlock the bitmap
            bitmap.UnlockBits(bitmapData);

            // Return the bitmap
            return bitmap;
        }

        /// <summary>
        /// Save the image to a file
        /// </summary>
        public void SaveToFile(string filePath, System.Drawing.Imaging.ImageFormat format)
        {
            // Get the surface as a GDI bitmap
            System.Drawing.Bitmap bitmap = GetGDIBitmap();

            // Save the bitmap
            bitmap.Save(filePath, format);
        }

        /// <summary>
        /// Creates a new surface from the given GDI bitmap
        /// </summary>
        public static Surface FromGDIBitmap(System.Drawing.Bitmap bitmap)
        {
            Surface surface = new Surface(bitmap.Width, bitmap.Height);

            // Load all the pixels
            for (int y = 0; y < bitmap.Height; y++)
            {
                for (int x = 0; x < bitmap.Width; x++)
                {
                    // Get this pixel
                    System.Drawing.Color bitmapColor = bitmap.GetPixel(x, y);

                    // Convert the pixel & store it in the surface
                    Color color = new Color(
                        bitmapColor.R,
                        bitmapColor.G,
                        bitmapColor.B,
                        bitmapColor.A
                        );

                    surface.SetPixel(x, y, color);
                }
            }

            return surface;
        }

        /// <summary>
        /// Creates a new surface from the given image file
        /// </summary>
        public static Surface FromFile(string filePath)
        {
            string fullPath = filePath;

            // If the path is relative, build an absolute path
            if (Path.IsPathRooted(fullPath) == false)
            {
                // Build the path to the image
                fullPath = System.IO.Path.Combine(
                    System.Environment.CurrentDirectory,
                    fullPath
                    );
            }

            // Load the image
            System.Drawing.Bitmap bitmap =
                (System.Drawing.Bitmap)System.Drawing.Bitmap.FromFile(fullPath);

            return FromGDIBitmap(bitmap);
        }

        #endregion
    }
}