/*
  Copyright (c) 2011, Raincity Games LLC
  Licensed under the MIT license: http://www.opensource.org/licenses/mit_license.php
*/

using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using System.Text;
using System.IO;

using Gtk;

namespace Gdk.Framework.UI.GtkSharp
{
	/// <summary>
	/// This Utility class provides easy methods to create viewers for common gdk assets
	/// </summary>
    public class AssetViewerHelper
	{
		static public Gtk.Window CreateAssetViewer(string assetPath)
		{
			// Get the extension of the file
			string ext = System.IO.Path.GetExtension(assetPath);
			
			// Process the extension
			switch(ext)
			{
				case ".gdkimage":
					return new GdkImageViewerWindow(assetPath);
			}
			
			// No known viewer for this asset type
			return null;
		}
	}
}