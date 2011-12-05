/*
  Copyright (c) 2011, Raincity Games LLC
  Licensed under the MIT license: http://www.opensource.org/licenses/mit_license.php
*/

using System;
using System.Collections.Generic;
using System.IO;

using ICSharpCode.SharpZipLib.Zip;
using ICSharpCode.SharpZipLib.Zip.Compression.Streams;

namespace Gdk.Framework
{
    /// <summary>
    /// Wrapping modes for texture coordinates outside the 0-1 range
    /// </summary>
    public enum TextureWrapMode
    {
        Wrap = 0,
        Mirror = 1,
        Clamp = 2
    }

    /// <summary>
    /// Filtering modes for sub-pixel texels
    /// </summary>
    public enum TextureFilterMode
    {
        Nearest = 0,
        Bilinear = 1
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
	/// Custom pixel formats   (Note, these must match the C++ GDK enum)
	/// </summary>
	public enum PixelFormats
	{
        RGB_565 = 1,
        RGB_888 = 2,
        RGBA_5551 = 3,
        RGBA_4444 = 4,
        RGBA_8888 = 5,

        LUMINANCE_ALPHA = 6,
        LUMINANCE = 7,
        ALPHA = 8,
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
		/// Draws a solid colored rect
		/// </summary>
		public void DrawRect(int x, int y, int w, int h, Color color)
		{
			for(int yLoop = y; yLoop < (y + h); yLoop++)
				for(int xLoop = x; xLoop < (x + w); xLoop++)
					SetPixel(xLoop, yLoop, color);
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
        /// Gets the color of a texel at the given coordinate. 
        /// 
        /// Note: Texel coordinates are 0.0f = top-left of image, 1.0f = bottom-right
        ///     Thus, half texel offsets are required to use the 'center' of the texel
        /// </summary>
        public Color GetTexel(Vector2 texCoord, TextureWrapMode wrapMode, TextureFilterMode filterMode)
        {
            float u = texCoord.X;
            float v = texCoord.Y;

            // Apply wrapping
            // ---------------------------

            switch (wrapMode)
            {
                case TextureWrapMode.Clamp:
                    {
                        // Do nothing, coords are clamped later
                    }
                    break;

                case TextureWrapMode.Wrap:
                    {
                        // Wrap the U coord into the 0-1 range
                        u %= 1f;
                        if (u < 0f)
                            u += 1f;

                        // Wrap the V coord into the 0-1 range
                        v %= 1f;
                        if (v < 0f)
                            v += 1f;
                    }
                    break;

                case TextureWrapMode.Mirror:
                    {
                        // Wrap the U coord into the 0-2 range
                        u %= 2f;
                        if (u < 0f)
                            u += 2f;

                        // Mirror the U coord around the 1 midpoint
                        if (u > 1f)
                            u = 2f - u;

                        // Wrap the V coord into the 0-2 range
                        v %= 2f;
                        if (v < 0f)
                            v += 2f;

                        // Mirror the V coord around the 1 midpoint
                        if (v > 1f)
                            v = 2f - v;
                    }
                    break;
            }

            // Clamp the U,V coords to (0 -> 1-Epsilon) range
            float maxTC = 1f - MathUtility.Epsilon;

            if (u < 0f)
                u = 0f;
            else if (u > maxTC)
                u = maxTC;

            if (v < 0f)
                v = 0f;
            else if (v > maxTC)
                v = maxTC;

            // Get the texel
            // --------------------------------------------

            Color texel = Color.Black;

            // Handle filtering
            switch (filterMode)
            {
                case TextureFilterMode.Nearest:
                    {
                        // Calculate the pixel of this texel
                        float px = u * width;
                        float py = v * height;

                        // Lookup the color at this exact pixel
                        texel = GetPixel((int)px, (int)py);
                    }
                    break;

                case TextureFilterMode.Bilinear:
                    {
                        // Calculate the approxiate pixel of this texel (shifting for the half-texel offset)
                        float px = (u - 0.5f / width) * width;
                        float py = (v - 0.5f / height) * height;
                        if (px < 0f)
                            px += width;
                        if (py < 0f)
                            py += height;

                        if (px >= width)
                            px = width - MathUtility.Epsilon;
                        if (py >= height)
                            py = height - MathUtility.Epsilon;

                        // Determine the 4 pixels adjacent to this texel
                        int x1 = (int)px;
                        int x2 = x1 + 1;
                        if (x2 >= width)
                            x2 = 0;
                        int y1 = (int)py;
                        int y2 = y1 + 1;
                        if (y2 >= height)
                            y2 = 0;

                        // Calculate the weights of the 4 pixels
                        float fracU = px - x1;
                        float fracV = py - y1;
                        float w1 = (1 - fracU) * (1 - fracV);
                        float w2 = fracU * (1 - fracV);
                        float w3 = (1 - fracU) * fracV;
                        float w4 = fracU * fracV;

                        // Lookup the 4 pixels adjacent to this texel
                        Color p1 = GetPixel(x1, y1);
                        Color p2 = GetPixel(x2, y1);
                        Color p3 = GetPixel(x1, y2);
                        Color p4 = GetPixel(x2, y2);

                        // Avg together all the weighted pixels
                        texel = p1 * w1 + p2 * w2 + p3 * w3 + p4 * w4;
                    }
                    break;
            }

            return texel;
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
        /// Blits a rectangular area from the source Surface onto a different rectangular area on this surface
        /// </summary>
        public void Blit(Surface source, int srcX, int srcY, int srcW, int srcH, int destX, int destY, int destW, int destH, TextureFilterMode filterMode)
        {
            // Get the source rectangle position & size in texel coordinates (with half texel offsets)
            Vector2 srcTexelPos = new Vector2((srcX + 0.5f) / source.Width, (srcY + 0.5f) / source.Height);
            Vector2 srcTexelSize = new Vector2((srcW - 1f) / source.Width, (srcH - 1f) / source.Height);

            // Loop through the destination rectangle
            for (int y = 0; y < destH; y++)
            {
                for (int x = 0; x < destW; x++)
                {
                    // Convert the destination pixel location into source image texel coordinates
                    float tx = ((float)x / (destW - 1)) * srcTexelSize.X + srcTexelPos.X;
                    float ty = ((float)y / (destH - 1)) * srcTexelSize.Y + srcTexelPos.Y;

                    // Get the texel and put it into the destination surface
                    Color c = source.GetTexel(new Vector2(tx, ty), TextureWrapMode.Wrap, filterMode);
                    this.SetPixel(x + destX, y + destY, c);
                }
            }
        }

        /// <summary>
        /// Blits a fixed size rectangular area from the source Surface onto this surface, also rotates by a right angle
        /// </summary>
        public void Blit(Surface source, int srcX, int srcY, int destX, int destY, int width, int height, RightAngles angle)
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

        #region Saving Methods
		
		/// <summary>
		/// Gets a Gtk.Pixbuf that contains the surface data
		/// </summary>
		public Gdk.Pixbuf GetGTKPixbuf()
		{
			// Get the data in RGBA format
			byte[] imageData = GetImageData(PixelFormats.RGBA_8888);
			
			// Create the Pixbuf from the image data
			return new Gdk.Pixbuf(imageData, true, 8, this.width, this.height, this.width * 4);
		}
		
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
        /// Save the image to a file using the System.Drawing APIs
        /// </summary>
        public void SaveToFile(string filePath, System.Drawing.Imaging.ImageFormat format)
        {
            // Get the surface as a GDI bitmap
            System.Drawing.Bitmap bitmap = GetGDIBitmap();

            // Save the bitmap
            bitmap.Save(filePath, format);
        }

		/// <summary>
		/// Get the image data as a byte array in the given pixel format
		/// </summary>
		public byte[] GetImageData(PixelFormats pixelFormat)
		{
			// Convert surface to pixel format
			// ------------------------------------------------

			byte[] imageData = null;
			int index = 0;

			// Do the pixel conversion depending on the pixel format
			switch (pixelFormat)
			{
                case PixelFormats.RGB_565:
                    // Create the image data buffer
                    imageData = new byte[Width * Height * 2];

                    // Loop through the pixels			- Format:  RRRRRGGG GGGBBBBB
                    for (int y = 0; y < Height; y++)
                    {
                        for (int x = 0; x < Width; x++)
                        {
                            Color pixel = GetPixel(x, y);
                            imageData[index++] = (byte)(((pixel.G & 0xFC) << 3) | (pixel.B >> 3));
                            imageData[index++] = (byte)((pixel.R & 0xF8) | (pixel.G >> 5));
                        }
                    }
                    break;

                case PixelFormats.RGB_888:
                    // Create the image data buffer
                    imageData = new byte[Width * Height * 3];

                    // Loop through the pixels
                    for (int y = 0; y < Height; y++)
                    {
                        for (int x = 0; x < Width; x++)
                        {
                            Color pixel = GetPixel(x, y);
                            imageData[index++] = pixel.R;
                            imageData[index++] = pixel.G;
                            imageData[index++] = pixel.B;
                        }
                    }
                    break;

                case PixelFormats.RGBA_5551:
                    // Create the image data buffer
                    imageData = new byte[Width * Height * 2];

                    // Loop through the pixels			- Format:  RRRRRGGG GGBBBBBA
                    for (int y = 0; y < Height; y++)
                    {
                        for (int x = 0; x < Width; x++)
                        {
                            Color pixel = GetPixel(x, y);
                            imageData[index++] = (byte)(((pixel.G & 0xF8) << 3) | ((pixel.B & 0xF8) >> 2) | (pixel.A >> 7));
                            imageData[index++] = (byte)((pixel.R & 0xF8) | (pixel.G >> 5));
                        }
                    }
                    break;

				case PixelFormats.RGBA_4444:
					// Create the image data buffer
					imageData = new byte[Width * Height * 2];

					// Loop through the pixels			- Format:  RRRRGGGG BBBBAAAA
					for (int y = 0; y < Height; y++)
					{
						for (int x = 0; x < Width; x++)
						{
							Color pixel = GetPixel(x, y);
							imageData[index++] = (byte)((pixel.B & 0xF0) | (pixel.A >> 4));
							imageData[index++] = (byte)((pixel.R & 0xF0) | (pixel.G >> 4));
						}
					}
					break;

                case PixelFormats.RGBA_8888:
                    // Create the image data buffer
                    imageData = new byte[Width * Height * 4];

                    // Loop through the pixels
                    for (int y = 0; y < Height; y++)
                    {
                        for (int x = 0; x < Width; x++)
                        {
                            Color pixel = GetPixel(x, y);
                            imageData[index++] = pixel.R;
                            imageData[index++] = pixel.G;
                            imageData[index++] = pixel.B;
                            imageData[index++] = pixel.A;
                        }
                    }
                    break;

                case PixelFormats.LUMINANCE_ALPHA:
                    // Create the image data buffer
                    imageData = new byte[Width * Height * 2];

                    // Loop through the pixels
                    for (int y = 0; y < Height; y++)
                    {
                        for (int x = 0; x < Width; x++)
                        {
                            Color pixel = GetPixel(x, y);
                            imageData[index++] = pixel.R;       // NOTE: The red channel is mapped to luminance
                            imageData[index++] = pixel.A;
                        }
                    }
                    break;

                case PixelFormats.LUMINANCE:
                    // Create the image data buffer
                    imageData = new byte[Width * Height * 1];

                    // Loop through the pixels
                    for (int y = 0; y < Height; y++)
                    {
                        for (int x = 0; x < Width; x++)
                        {
                            Color pixel = GetPixel(x, y);
                            imageData[index++] = pixel.R;       // NOTE: The red channel is mapped to luminance
                        }
                    }
                    break;

                case PixelFormats.ALPHA:
                    // Create the image data buffer
                    imageData = new byte[Width * Height * 1];

                    // Loop through the pixels
                    for (int y = 0; y < Height; y++)
                    {
                        for (int x = 0; x < Width; x++)
                        {
                            Color pixel = GetPixel(x, y);
                            imageData[index++] = pixel.A;
                        }
                    }
                    break;
			}

			return imageData;
		}
		
		/// <summary>
		/// Save the image to a compressed ZLIB file
		/// </summary>
		public void SaveToGdkImage(
			string filePath, 
			PixelFormats pixelFormat, 
			bool useMipMapping = true, 
			TextureWrapMode wrapMode = TextureWrapMode.Wrap, 
			TextureFilterMode filterMode = TextureFilterMode.Bilinear
			)
		{
			// Open the file
			System.IO.FileStream outFileStream = new System.IO.FileStream(filePath, System.IO.FileMode.Create);
			
			// Build the flags
			UInt16 flags = 0;
			
			// Bit 0 = mipmap on/off
			if(useMipMapping)
				flags |= 0x0001;
			
			// Bit 1 & 2 = wrap mode
			flags |= (UInt16)((int)wrapMode << 1);
			
			// Bit 3 & 4 = filter mode
			flags |= (UInt16)((int)filterMode << 3);

			// Write the image header
			outFileStream.Write(BitConverter.GetBytes((UInt16)Width), 0, 2);
            outFileStream.Write(BitConverter.GetBytes((UInt16)Height), 0, 2);
            outFileStream.Write(BitConverter.GetBytes((UInt16)pixelFormat), 0, 2);
            outFileStream.Write(BitConverter.GetBytes((UInt16)flags), 0, 2);
			outFileStream.Flush();

			// Open the compressed stream
			DeflaterOutputStream outZStream = new DeflaterOutputStream(outFileStream);

			// Get the image data in the given pixel format
			byte[] imageData = GetImageData(pixelFormat);
			outZStream.Write(imageData, 0, imageData.Length);

			// Flush and close the stream
			outZStream.Flush();
			outZStream.Close();
			outFileStream.Close();
		}

		#endregion

		#region Loading Methods
		
		/// <summary>
		/// Creates a new surface from the given Pixbuf 
		/// </summary>
		public static Surface FromGTKPixbuf(Pixbuf pixbuf)
		{
			// Create a new surface with the same dimensions as the pixmap
            Surface surface = new Surface(pixbuf.Width, pixbuf.Height);
			
			// Make sure the pixbuf data is understandable
			if (pixbuf.Colorspace != Colorspace.Rgb ||
			    pixbuf.NChannels < 3 || pixbuf.NChannels > 4 ||
			    pixbuf.BitsPerSample != 8)
			{
				throw new ApplicationException("Unknown pixbuf format, cannot create surface data");
			}
			
			// Loop through the pixels
            for (int y = 0; y < pixbuf.Height; y++)
            {
                for (int x = 0; x < pixbuf.Width; x++)
                {
					int pixelOffset = y * pixbuf.Rowstride + x * pixbuf.NChannels;

					// Get the data Color value for this pixel
					byte r = System.Runtime.InteropServices.Marshal.ReadByte(pixbuf.Pixels, pixelOffset);
					byte g = System.Runtime.InteropServices.Marshal.ReadByte(pixbuf.Pixels, pixelOffset + 1);
					byte b = System.Runtime.InteropServices.Marshal.ReadByte(pixbuf.Pixels, pixelOffset + 2);
					byte a = 255;
					if(pixbuf.HasAlpha && pixbuf.NChannels == 4)
						a = System.Runtime.InteropServices.Marshal.ReadByte(pixbuf.Pixels, pixelOffset + 4);
					
					// Put the pixel into the surface
					surface.SetPixel(x, y, new Color(r,g,b,a));
				}
			}
		
            return surface;
        }
		
		/// <summary>
        /// Creates a new surface from the given GDI bitmap
        /// </summary>
        public static Surface FromGDIBitmap(System.Drawing.Bitmap bitmap)
        {
            // Create a new surface with the same dimensions as the bitmap
            Surface surface = new Surface(bitmap.Width, bitmap.Height);

            // Lock the bitmap's bits.  
            System.Drawing.Rectangle rect = new System.Drawing.Rectangle(0, 0, bitmap.Width, bitmap.Height);
            System.Drawing.Imaging.BitmapData bitmapData =
                bitmap.LockBits(rect, 
                System.Drawing.Imaging.ImageLockMode.ReadOnly,
                bitmap.PixelFormat
                );

            // Declare an array to hold the bytes of the bitmap.
            int bytes = bitmapData.Stride * bitmap.Height;
            byte[] rgbaValues = new byte[bytes];

            // Copy the RGBA values into the array.
            System.Runtime.InteropServices.Marshal.Copy(bitmapData.Scan0, rgbaValues, 0, bytes);

            // Load all the pixels
            for (int y = 0; y < bitmap.Height; y++)
            {
                // Move to the next row of image data
                int byteIndex = y * bitmapData.Stride;

                for (int x = 0; x < bitmap.Width; x++)
                {
                    // Read the next 4 bytes of pixel data
                    byte b = rgbaValues[byteIndex++];
                    byte g = rgbaValues[byteIndex++];
                    byte r = rgbaValues[byteIndex++];
                    byte a = rgbaValues[byteIndex++];

                    // Convert the pixel & store it in the surface
                    surface.SetPixel(x, y, new Color(r,g,b,a));
                }
            }

            // Unlock the bits.
            bitmap.UnlockBits(bitmapData);

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

            // Load the image into a GDI bitmap
            System.Drawing.Bitmap bitmap = (System.Drawing.Bitmap)System.Drawing.Bitmap.FromFile(fullPath);

            // If the bitmap is not a 32 BPP image, convert it to one
            if (bitmap.PixelFormat != System.Drawing.Imaging.PixelFormat.Format32bppArgb)
            {
                // Create a new 32 BPP surface to hold the image
                System.Drawing.Bitmap newBitmap = new System.Drawing.Bitmap(bitmap.Width, bitmap.Height, System.Drawing.Imaging.PixelFormat.Format32bppArgb);
                
                // Copy the old image into the new one
                System.Drawing.Graphics g = System.Drawing.Graphics.FromImage(newBitmap);
                g.DrawImage(bitmap, 0, 0, bitmap.Width, bitmap.Height);

                bitmap.Dispose();
                bitmap = newBitmap;
            }

            // Convert the GDI bitmap to a Surface
			Surface surface = FromGDIBitmap(bitmap);

			// Dispose the bitmap
			bitmap.Dispose();

			return surface;
        }

		/// <summary>
		/// Loads a surface from a ZLIB file
		/// </summary>
		public static Surface FromGdkImageFile(string filePath, out PixelFormats pixelFormat)
		{
			// Open the file stream
			System.IO.FileStream inFileStream = new System.IO.FileStream(filePath, System.IO.FileMode.Open);

			// Read in the image header
			byte[] twoBytes = new byte[2];
			inFileStream.Read(twoBytes, 0, 2);
			ushort width = BitConverter.ToUInt16(twoBytes, 0);
			inFileStream.Read(twoBytes, 0, 2);
			ushort height = BitConverter.ToUInt16(twoBytes, 0);
			inFileStream.Read(twoBytes, 0, 2);
			pixelFormat = (PixelFormats)BitConverter.ToUInt16(twoBytes, 0);
            inFileStream.Read(twoBytes, 0, 2);
            ushort flags = BitConverter.ToUInt16(twoBytes, 0);

			// Open the compressed stream
			InflaterInputStream inZStream = new InflaterInputStream(inFileStream);

            // Get the number of bytes per pixel based on the pixel format
            int bytesPerPixel = 1;
            switch (pixelFormat)
            {
                case PixelFormats.ALPHA:
                case PixelFormats.LUMINANCE:
                    bytesPerPixel = 1;
                    break;
                case PixelFormats.LUMINANCE_ALPHA:
                case PixelFormats.RGB_565:
                case PixelFormats.RGBA_4444:
                case PixelFormats.RGBA_5551:
                    bytesPerPixel = 2;
                    break;
                case PixelFormats.RGB_888:
                    bytesPerPixel = 3;
                    break;
                case PixelFormats.RGBA_8888:
                    bytesPerPixel = 4;
                    break;
            }

			// Read the image data
            int numBytes = width * height * bytesPerPixel;
			byte[] imageData = new byte[numBytes];
			int bytesRead = inZStream.Read(imageData, 0, numBytes);

			// Close the stream
			inZStream.Close();
			inFileStream.Close();

			// Build a new surface with the given image data
			return FromImageData(width, height, imageData, pixelFormat);
		}

		/// <summary>
		/// Creates a new surface from a blob of image data in the given size and pixel format
		/// </summary>
		public static Surface FromImageData(int width, int height, byte[] imageData, PixelFormats pixelFormat)
		{
			// Create the surface
			Surface surface = new Surface(width, height);
			int index = 0;
	
			// Load the image data depending on the pixel format
			switch (pixelFormat)
			{
				case PixelFormats.RGB_565:
					// Loop through the pixels				Format:  RRRRRGGG GGGBBBBB
					for (int y = 0; y < height; y++)
					{
						for (int x = 0; x < width; x++)
						{
							byte byte2 = imageData[index++];
							byte byte1 = imageData[index++];

							float r = (float)((byte1 & 0xF8) >> 3) / 31.0f;
							float g = (float)(((byte1 & 0x7) << 3) | ((byte2 & 0xE0) >> 5)) / 63.0f;
							float b = (float)(byte2 & 0x1F) / 31.0f;

							Color pixel = new Color((byte)(r * 255), (byte)(g * 255), (byte)(b * 255), 255);
							surface.SetPixel(x, y, pixel);
						}
					}
					break;

                case PixelFormats.RGB_888:
                    // Loop through the pixels
                    for (int y = 0; y < height; y++)
                    {
                        for (int x = 0; x < width; x++)
                        {
                            Color pixel = new Color();
                            pixel.R = imageData[index++];
                            pixel.G = imageData[index++];
                            pixel.B = imageData[index++];
                            pixel.A = 255;
                            surface.SetPixel(x, y, pixel);
                        }
                    }
                    break;

				case PixelFormats.RGBA_5551:
					// Loop through the pixels				Format:  RRRRRGGG GGBBBBBA
					for (int y = 0; y < height; y++)
					{
						for (int x = 0; x < width; x++)
						{
							byte byte2 = imageData[index++];
							byte byte1 = imageData[index++];

							float r = (float)((byte1 & 0xF8) >> 3) / 31.0f;
							float g = (float)(((byte1 & 0x7) << 2) | ((byte2 & 0xC0) >> 6)) / 31.0f;
							float b = (float)((byte2 & 0x3E) >> 1) / 31.0f;
							byte a = (byte2 & 0x1) > 0 ? (byte)255 : (byte)0;

							Color pixel = new Color((byte)(r * 255), (byte)(g * 255), (byte)(b * 255), a);
							surface.SetPixel(x, y, pixel);
						}
					}
					break;

				case PixelFormats.RGBA_4444:
					// Loop through the pixels				Format:  RRRRGGGG BBBBAAAA
					for (int y = 0; y < height; y++)
					{
						for (int x = 0; x < width; x++)
						{
							byte byte2 = imageData[index++];
							byte byte1 = imageData[index++];

							float r = (float)((byte1 & 0xF0) >> 4) / 15.0f;
							float g = (float) (byte1 & 0x0F) / 15.0f;
							float b = (float)((byte2 & 0xF0) >> 4) / 15.0f;
							float a = (float) (byte2 & 0x0F) / 15.0f;

							Color pixel = new Color((byte)(r * 255), (byte)(g * 255), (byte)(b * 255), (byte)(a * 255));
							surface.SetPixel(x, y, pixel);
						}
					}
					break;

                case PixelFormats.RGBA_8888:
                    // Loop through the pixels
                    for (int y = 0; y < height; y++)
                    {
                        for (int x = 0; x < width; x++)
                        {
                            Color pixel = new Color();
                            pixel.R = imageData[index++];
                            pixel.G = imageData[index++];
                            pixel.B = imageData[index++];
                            pixel.A = imageData[index++];
                            surface.SetPixel(x, y, pixel);
                        }
                    }
                    break;

                case PixelFormats.LUMINANCE_ALPHA:
                    // Loop through the pixels
                    for (int y = 0; y < height; y++)
                    {
                        for (int x = 0; x < width; x++)
                        {
                            Color pixel = new Color();
                            pixel.R = pixel.G = pixel.B = imageData[index++];
                            pixel.A = imageData[index++];
                            surface.SetPixel(x, y, pixel);
                        }
                    }
                    break;

                case PixelFormats.LUMINANCE:
                    // Loop through the pixels
                    for (int y = 0; y < height; y++)
                    {
                        for (int x = 0; x < width; x++)
                        {
                            Color pixel = new Color();
                            pixel.R = pixel.G = pixel.B = imageData[index++];
                            pixel.A = 255;
                            surface.SetPixel(x, y, pixel);
                        }
                    }
                    break;

                case PixelFormats.ALPHA:
                    // Loop through the pixels
                    for (int y = 0; y < height; y++)
                    {
                        for (int x = 0; x < width; x++)
                        {
                            Color pixel = new Color();
                            pixel.R = pixel.G = pixel.B = 255;
                            pixel.A = imageData[index++];
                            surface.SetPixel(x, y, pixel);
                        }
                    }
                    break;
			}

			return surface;
		}

        #endregion
    }
}