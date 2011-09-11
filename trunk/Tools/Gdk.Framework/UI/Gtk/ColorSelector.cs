/*
  Copyright (c) 2011, Raincity Games LLC
  Licensed under the MIT license: http://www.opensource.org/licenses/mit_license.php
*/

using System;
using System.Collections.Generic;
using System.Linq;

using Gtk;

namespace Gdk.Framework.UI.GtkSharp
{
	[System.ComponentModel.ToolboxItem(true)]
	public partial class ColorSelector : Gtk.Bin
	{
		// Internal Properties
		private Gdk.Framework.Color color;
		
		// Events
		public event Action<ColorSelector, Gdk.Framework.Color> ColorChanged;
		
		/// <summary>
		/// Gets/Sets the color value of this widget
		/// </summary>
		/// <value>
		/// The color.
		/// </value>
		public Gdk.Framework.Color Color
		{
			get { return color; }
			set
			{
				// Set the color value into the child widgets
                this.color = value;
				eventboxColor.ModifyBg(StateType.Normal, this.color.ToGdkColor());
				entryColor.Text = this.color.ToString();

                // Fire the ColorChanged event
                if (ColorChanged != null)
                    ColorChanged(this, this.color);
			}
		}
		
		/// <summary>
		/// Initializes a new instance of the <see cref="Gdk.Framework.UI.GtkSharp.ColorSelector"/> class.
		/// </summary>
		public ColorSelector ()
		{
			this.Build ();
			this.BorderWidth = 1;
			this.FocusChild = entryColor;
		}
		
		// **********************************************************************
		protected override void OnFocusGrabbed ()
		{
			this.entryColor.GrabFocus();
		}
		
		// **********************************************************************
		protected void EntryColor_Changed (object sender, System.EventArgs e)
		{
			// Is the text a valid color?
            Gdk.Framework.Color parsedColor;
            if (Gdk.Framework.Color.TryParse(entryColor.Text, out parsedColor) == true)
            {
                // Set the updated color
                this.color = parsedColor;
				eventboxColor.ModifyBg(StateType.Normal, this.color.ToGdkColor());

                // Fire the ColorChanged event
                if (ColorChanged != null)
                    ColorChanged(this, this.color);

                entryColor.ModifyText(StateType.Normal, new Gdk.Color(0,0,0));
            }
            else
            {
                // Invalid color text
                entryColor.ModifyText(StateType.Normal, new Gdk.Color(196,0,0));
            }

		}
		
		// **********************************************************************
		protected void EventboxColor_ButtonPressEvent (object o, Gtk.ButtonPressEventArgs args)
		{	
			// Show a color selection dialog
			ColorSelectionDialog dlg = new ColorSelectionDialog("Select a color");
			dlg.ColorSelection.CurrentColor = dlg.ColorSelection.PreviousColor = this.color.ToGdkColor();
			dlg.ColorSelection.CurrentAlpha = dlg.ColorSelection.PreviousAlpha = (ushort)(((ushort)this.color.A) << 8);
			dlg.ColorSelection.HasOpacityControl = true;
			
			// Set the parent window of the dialog to our parent
			Widget topLevel = this.Toplevel;
			if(topLevel.IsTopLevel && topLevel is Gtk.Window)
				dlg.TransientFor = topLevel as Gtk.Window;
			
			// Show the dialog
			int response = dlg.Run();
			Gdk.Color dlgColor = dlg.ColorSelection.CurrentColor;
			byte dlgAlpha = (byte)(dlg.ColorSelection.CurrentAlpha >> 8);
			dlg.Destroy();
			
			// Did the user click OK on the dialog?
			if (response == (int)ResponseType.Ok) 
			{
                // Update the color
				Gdk.Framework.Color newColor = Gdk.Framework.Color.FromGdkColor(dlgColor);
				newColor.A = dlgAlpha;
                this.Color = newColor;
            }
		}
	}
}

