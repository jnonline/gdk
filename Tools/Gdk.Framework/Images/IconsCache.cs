/*
  Copyright (c) 2011, Raincity Games LLC
  Licensed under the MIT license: http://www.opensource.org/licenses/mit_license.php
*/

using System;
using System.Collections.Generic;
using System.Reflection;

namespace Gdk.Framework
{
	/// <summary>
	/// This enumeration defines all the icons that are available via the Gdk.Framework.IconCache class
	/// </summary>
	public enum Icons
	{
		Atlas,
		Audio,
		File,
		Folder,
		Font,
		Model,
		Package,
		Shader,
		Texture,
		Video
	}
	
	/// <summary>
	/// This class contains a global cache of the icons used throughout the Gdk .NET framework 
	/// </summary>
	static public class IconsCache
	{
		static public Gdk.Pixbuf GetIconPixbuf(Icons icon)
		{
			// Make sure the cache is initialized
			EnsurePixbufCacheInitalized();
			
			// Return the pixbuf for this icon
			return iconPixbufs[icon];
		}
		
		/// <summary>
		/// Makes sure the Pixbuf icon cache is initialized 
		/// </summary>
		static private void EnsurePixbufCacheInitalized()
		{
			// Bail early if we're already initialized
			if(pixbufCacheInitialized == true)
				return;
			
			// Get a threadsafe lock
			lock(initLock)
			{
				// Make sure we didnt get initialized while waiting for the lock
				if(pixbufCacheInitialized == true)
					return;
			
				// Create the pixbuf dictionary
				iconPixbufs = new Dictionary<Icons, Gdk.Pixbuf>();
				
				// Get this assembly
				Assembly asm = Assembly.GetExecutingAssembly();

				// Add the icons to the pix buf
				iconPixbufs.Add(Icons.Atlas, new Gdk.Pixbuf(asm, "Gdk.Framework.Images.16x16.Atlas.png"));
				iconPixbufs.Add(Icons.Audio, new Gdk.Pixbuf(asm, "Gdk.Framework.Images.16x16.Audio.png"));
				iconPixbufs.Add(Icons.File, new Gdk.Pixbuf(asm, "Gdk.Framework.Images.16x16.File.png"));
				iconPixbufs.Add(Icons.Folder, new Gdk.Pixbuf(asm, "Gdk.Framework.Images.16x16.Folder.png"));
				iconPixbufs.Add(Icons.Font, new Gdk.Pixbuf(asm, "Gdk.Framework.Images.16x16.Font.png"));
				iconPixbufs.Add(Icons.Model, new Gdk.Pixbuf(asm, "Gdk.Framework.Images.16x16.Model.png"));
				iconPixbufs.Add(Icons.Package, new Gdk.Pixbuf(asm, "Gdk.Framework.Images.16x16.Package.png"));
				iconPixbufs.Add(Icons.Shader, new Gdk.Pixbuf(asm, "Gdk.Framework.Images.16x16.Shader.png"));
				iconPixbufs.Add(Icons.Texture, new Gdk.Pixbuf(asm, "Gdk.Framework.Images.16x16.Texture.png"));
				iconPixbufs.Add(Icons.Video, new Gdk.Pixbuf(asm, "Gdk.Framework.Images.16x16.Video.png"));
				
				// We are initialized :)
				pixbufCacheInitialized = true;
			}
		}
		
		static private Dictionary<Icons, Gdk.Pixbuf> iconPixbufs;
		static private bool pixbufCacheInitialized;
		
		static private object initLock = new object();
	}
}

