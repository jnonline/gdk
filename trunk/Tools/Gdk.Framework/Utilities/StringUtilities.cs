using System;
using System.Collections.Generic;
using System.Reflection;
using System.Text;
using System.Xml;
using System.IO;

namespace Gdk
{
	/// <summary>
	/// This utility class provides properties & methods for doing various manipulations with strings
	/// </summary>
    static public class StringUtilities
	{
        /// <summary>
        /// Parse an int array of the format "i [, j] [, k] ..."
        /// </summary>
        /// <param name="input"></param>
        /// <param name="expectedNumInts"></param>
        /// <returns></returns>
        static public int[] ParseIntArray(string input, int expectedNumInts)
        {
            // Split up the string by commas
            string[] parts = input.Split(',');
            int numValues = parts.GetUpperBound(0) + 1;
            if (numValues != expectedNumInts)
                throw new ArgumentException("Input string has an invalid number of entries: " + numValues + " (Expected: " + expectedNumInts + ")", "input");

            // Copy the values to an int[]
            int[] values = new int[numValues];
            int index = 0;
            foreach (string part in parts)
                values[index++] = Int32.Parse(part);

            return values;
        }

        /// <summary>
        /// Writes a string, in a gdk readable format
        /// </summary>
        /// <param name="writer"></param>
        /// <param name="value"></param>
        static public void WriteString(BinaryWriter writer, string value)
        {
            writer.Write((ushort)value.Length);
            writer.Write((char[])value.ToCharArray());
        }

        /// <summary>
        /// Replaces any html commands with encoded strings in the given input
        /// </summary>
        static public string EncodeHtml(string input)
        {
            string result = input
                .Replace("&", "&amp;")
                .Replace("<", "&lt;")
                .Replace(">", "&gt;")
                .Replace("\n", "<br/>")
                ;
            return result;
        }
	}
}