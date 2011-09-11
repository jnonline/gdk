
using System;
using System.Collections.Generic;
using Gtk;

using Gdk.Framework;

namespace Gdk.Framework.UI.GtkSharp
{
	public partial class GdkImageViewerWindow : Gtk.Window
	{
		// Properties
		private Surface sourceImage;
		
		private Gdk.Color backColor1;
		private Gdk.Color backColor2;
		private Gdk.GC backgroundGC1;
		private Gdk.GC backgroundGC2;
		private Gdk.Pixmap backgroundPixmap;
		
		// ******************************************************************
		public GdkImageViewerWindow (string imagePath) : 
				base(Gtk.WindowType.Toplevel)
		{
			this.Build ();
			
			// Set the window icon
			this.Icon = Gdk.Framework.IconsCache.GetIconPixbuf(Icons.Texture);
			
			// Setup the backgrounds
			// -----------------------------
			
			// Assign the imagedisplay to have it's own style
			imageDisplay.Parent.Style = imageDisplay.Parent.Style.Copy();
			
			// Create the pixmap used for backgrounds
			backgroundPixmap = new Gdk.Pixmap(this.imageDisplay.GdkWindow, 64, 64, -1);
			
			// Create GCs for background drawing
			backgroundGC1 = new GC(backgroundPixmap);
			backgroundGC2 = new GC(backgroundPixmap);
			backgroundGC1.Copy(this.Style.WhiteGC);
			backgroundGC2.Copy(this.Style.WhiteGC);
			
			// Set the default BG colors
			this.backColor1 = new Gdk.Color(255, 255, 255);
			this.backColor2 = new Gdk.Color(196, 196, 196);
			
			UpdateDisplayBackground();
			
			// Load the image
			// -----------------------------
			
			LoadImage(imagePath);
		}
		
		// ******************************************************************
		public void LoadImage(string imagePath)
		{
			// Update the window title
			this.Title = imagePath;
			
			// Load the image into a Surface
			PixelFormats pixelFormat;
			sourceImage = Surface.FromGdkImageFile(imagePath, out pixelFormat);
			
			// Update the display image
			UpdateDisplayImage();
			
			// Resize the window to fit the image
			int windowWidth, windowHeight;
			this.GetSize(out windowWidth, out windowHeight);
			int widgetWidth, widgetHeight;
			imageDisplay.GdkWindow.GetSize(out widgetWidth, out widgetHeight);
			int newWidth = windowWidth - widgetWidth + sourceImage.Width;
			int newHeight = windowHeight - widgetHeight + sourceImage.Height;
			newWidth = Math.Max(256, newWidth);
			newHeight = Math.Max(256, newHeight);
			this.Resize(newWidth, newHeight);
			
			// Update the status labels
			labelBottom1.Markup = 
				"<b>Size:</b> " + sourceImage.Width + " x " + sourceImage.Height + "   " +
				"<b>PixelFormat:</b> " + pixelFormat.ToString()
				;
		}
		
		// ******************************************************************
		private void UpdateDisplayImage()
		{
			// Get the scale factor from the zoom combo
			string zoomLevel = comboZoom.ActiveText.Trim(' ', '\r', '\n', '\r', '%');
			int zoom;
			if(int.TryParse(zoomLevel, out zoom) == false)
				zoom = 100;
			double scaleFactor = zoom / 100.0f;
			
			// Calculate the scaled size of the image
			int newWidth = Math.Max(1, (int)(sourceImage.Width * scaleFactor));
			int newHeight = Math.Max(1, (int)(sourceImage.Height * scaleFactor));
			
			// Get the scaling mode from the combo
			Gdk.InterpType interpolationType = InterpType.Bilinear;
			switch(comboScaling.ActiveText.ToLower())
			{
				case "nearest":	interpolationType = InterpType.Nearest; break;
				case "tiles":	interpolationType = InterpType.Tiles; break;
				case "hyper":	interpolationType = InterpType.Hyper; break;
			}
			
			// Get a Pixbuf from the surface
			Pixbuf originalPixbuf = sourceImage.GetGTKPixbuf();
			
			// Create a new pixbuf by scaling the image
			Pixbuf newPixbuf = originalPixbuf.ScaleSimple(newWidth, newHeight, interpolationType);
			
			// Put the new pixbuf into the image
			imageDisplay.Pixbuf = newPixbuf;
		}
		
		// ******************************************************************
		private void UpdateDisplayBackground()
		{
			// Update the colors of the two color selection buttons
			backgroundColorEventBox1.ModifyBg(StateType.Normal, this.backColor1);
			backgroundColorEventBox2.ModifyBg(StateType.Normal, this.backColor2);
			
			// Set the GC colors
			backgroundGC1.RgbFgColor = backColor1;
			backgroundGC2.RgbFgColor = backColor2;
			
			// Which type of background are we displaying?
			switch(comboBackgroundMode.ActiveText.ToLower())
			{
			case "checker":
				// Draw a checkered pattern into the background pixmap
				backgroundPixmap.DrawRectangle(backgroundGC1, true,  0,  0, 32, 32);
				backgroundPixmap.DrawRectangle(backgroundGC1, true, 32, 32, 32, 32);
				backgroundPixmap.DrawRectangle(backgroundGC2, true, 32,  0, 32, 32);
				backgroundPixmap.DrawRectangle(backgroundGC2, true,  0, 32, 32, 32);
				break;
				
			case "circles":
				// Draw a checkered pattern into the background pixmap
				backgroundPixmap.DrawRectangle(backgroundGC1, true, 0,  0, 64, 64);
				
				backgroundGC2.SetLineAttributes(3, LineStyle.Solid, CapStyle.Butt, JoinStyle.Round);
				backgroundPixmap.DrawArc(backgroundGC2, false,  0,    0, 64, 64, 0, 23040);
				backgroundPixmap.DrawArc(backgroundGC2, false, -24, -24, 48, 48, 0, 23040);
				backgroundPixmap.DrawArc(backgroundGC2, false,  40, -24, 48, 48, 0, 23040);
				backgroundPixmap.DrawArc(backgroundGC2, false, -24,  40, 48, 48, 0, 23040);
				backgroundPixmap.DrawArc(backgroundGC2, false,  40,  40, 48, 48, 0, 23040);
				break;
				
			default:  // Solid background
				// Draw a solid block into the background pixmap
				backgroundPixmap.DrawRectangle(backgroundGC1, true,  0,  0, 64, 64);
				break;
				
			}
			
			// Apply the pixmap to the background
			//Gtk.Style newStyle = imageDisplay.Parent.Style.Copy();
			imageDisplay.Parent.Style.SetBgPixmap(StateType.Normal, backgroundPixmap);
			//newStyle.SetBgPixmap(StateType.Active, backgroundPixmap);
			//imageDisplay.Parent.Style = newStyle;
		}
		
		// ******************************************************************
		protected void Background_ComboBackgroundModeChanged (object sender, System.EventArgs e)
		{	
			UpdateDisplayBackground();
		}
		
		// ******************************************************************
		protected void Background_Color1ButtonPress (object o, Gtk.ButtonPressEventArgs args)
		{
			// Show a 'Choose color' dialog
			ColorSelectionDialog dlg = new ColorSelectionDialog("Choose a color");
			dlg.TransientFor = this;
			dlg.ColorSelection.PreviousColor = this.backColor1;
			dlg.ColorSelection.CurrentColor = this.backColor1;
			dlg.ColorSelection.HasPalette = true;
			
			// Run the dialog
			if(dlg.Run() == (int)ResponseType.Ok)
			{
				// Update the color boxes & background
				this.backColor1 = dlg.ColorSelection.CurrentColor;
				UpdateDisplayBackground();
			}
			
			dlg.Destroy();
		}
		
		// ******************************************************************
		protected void Background_Color2ButtonPress (object o, Gtk.ButtonPressEventArgs args)
		{
			// Show a 'Choose color' dialog
			ColorSelectionDialog dlg = new ColorSelectionDialog("Choose a color");
			dlg.TransientFor = this;
			dlg.ColorSelection.PreviousColor = this.backColor2;
			dlg.ColorSelection.CurrentColor = this.backColor2;
			dlg.ColorSelection.HasPalette = true;
			
			// Run the dialog
			if(dlg.Run() == (int)ResponseType.Ok)
			{
				// Update the color boxes & background
				this.backColor2 = dlg.ColorSelection.CurrentColor;
				UpdateDisplayBackground();
			}
			
			dlg.Destroy();
		}
		
		// ******************************************************************
		protected void Zoom_ComboChanged (object sender, System.EventArgs e)
		{
			UpdateDisplayImage();
		}
		
		// ******************************************************************
		protected void Scaling_ComboChanged (object sender, System.EventArgs e)
		{
			UpdateDisplayImage();
		}
		
		// ******************************************************************
		protected void Colors_ShiftButtonClicked (object sender, System.EventArgs e)
		{
			// Loop through the pixels in the source image
			for(int y=0; y<sourceImage.Height; y++)
			{
				for(int x=0; x<sourceImage.Width; x++)
				{
					// shift the colors in this pixel
					Color c = sourceImage.GetPixel(x,y);
					sourceImage.SetPixel(x,y, new Color(c.A, c.R, c.G, c.B)); 
				}
			}
				
			// Reupdate the image
			UpdateDisplayImage();
		}
		
		// ******************************************************************
		protected void Colors_InvertButtonClicked (object sender, System.EventArgs e)
		{
			// Loop through the pixels in the source image
			for(int y=0; y<sourceImage.Height; y++)
			{
				for(int x=0; x<sourceImage.Width; x++)
				{
					// invert the colors in this pixel
					Color c = sourceImage.GetPixel(x,y);
					sourceImage.SetPixel(x,y, new Color((byte)(255 - c.R), (byte)(255 - c.G), (byte)(255 - c.B), c.A)); 
				}
			}
				
			// Reupdate the image
			UpdateDisplayImage();
		}
	}
}

