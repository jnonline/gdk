using System;
using Gtk;

namespace Gdk.ContentStudio
{
	class MainClass
	{
		public static void Main (string[] args)
		{
			Application.Init ();
			
			//Gtk.Window win = new Gdk.Framework.UI.GtkSharp.TestWindow();
			MainWindow win = new MainWindow ();
			
			// -- This is useful for testing the asset viewer --
			//Gtk.Window win = new Gdk.Framework.UI.GtkSharp.GdkImageViewerWindow(
			//	"/Users/Justin/SVN/Gdk/trunk/Assets/Build/Base/Grass1.gdkimage");
			
			win.Show ();
			Application.Run ();
		}
	}
}
