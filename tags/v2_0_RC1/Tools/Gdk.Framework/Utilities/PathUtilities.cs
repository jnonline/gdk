/*
  Copyright (c) 2011, Raincity Games LLC
  Licensed under the MIT license: http://www.opensource.org/licenses/mit_license.php
*/

using System;
using System.Collections.Generic;
using System.Reflection;
using System.Text;
using System.IO;

namespace Gdk.Framework
{
	/// <summary>
	/// This utility class provides properties & methods for doing various manipulations with path strings
	/// </summary>
    static public class PathUtilities
	{	
		/// <summary>
		/// This method fixes path strings to be cross-plat friendly, by changing '\'s to '/'s
		/// </summary>
		static public string FixPathSeperators(string input)
		{
			return input.Replace('\\', '/');
		}
		
		/// <summary>
		/// This combines two path strings [like Path.Combine()], but uses the cross-plat friendly / seperator
		/// </summary>
		static public string Combine(string part1, string part2)
		{
			return FixPathSeperators(Path.Combine(part1, part2));
		}
	}
}