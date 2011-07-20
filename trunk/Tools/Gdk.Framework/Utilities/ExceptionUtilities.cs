/*
  Copyright (c) 2011, Raincity Games LLC
  Licensed under the MIT license: http://www.opensource.org/licenses/mit_license.php
*/

using System;
using System.Collections.Generic;
using System.Reflection;
using System.Text;
using System.Xml;


namespace Gdk
{
	/// <summary>
	/// Exception Utilities methods
	/// </summary>
	public class ExceptionUtilities
	{
		static public string GetExceptionText(Exception ex)
		{
			string result = string.Empty;

			// Get the inner exception text first
			if (ex.InnerException != null)
				result += GetExceptionText(ex.InnerException);

			// [ExceptionName]: Message
			result += "[" + ex.GetType().Name + "]: " + ex.Message + System.Environment.NewLine;

			// Stacktrace
			result += ex.StackTrace + System.Environment.NewLine;

			return result;
		}
	}
}