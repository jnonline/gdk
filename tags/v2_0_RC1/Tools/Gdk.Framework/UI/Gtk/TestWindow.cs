using System;

namespace Gdk.Framework.UI.GtkSharp
{
	public partial class TestWindow : Gtk.Window
	{
		public TestWindow () : 
				base(Gtk.WindowType.Toplevel)
		{
			this.Build ();
			
			
		}
	}
}

