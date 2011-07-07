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
        /// Parse a list of ints from a string
        /// </summary>
        static public List<int> ParseInts(string input, char seperator)
        {
            List<int> results = new List<int>();

            // Split the string by the seperator
            string[] stringValues = input.Split(seperator);

            // Loop through the string values
            foreach (string stringValue in stringValues)
            {
                // Trim all white space from the value
                stringValue.Trim();
                int intValue = 0;
                if (int.TryParse(stringValue, out intValue) == true)
                    results.Add(intValue);
            }

            return results;
        }

        /// <summary>
        /// Parse a list of floats from a string
        /// </summary>
        static public List<float> ParseFloats(string input, char seperator)
        {
            List<float> results = new List<float>();

            // Split the string by the seperator
            string[] stringValues = input.Split(seperator);
                
            // Loop through the string values
            foreach (string stringValue in stringValues)
            {
                // Trim all white space from the value
                stringValue.Trim();
                float floatValue = 0.0f;
                if (float.TryParse(stringValue, out floatValue) == true)
                    results.Add(floatValue);
            }

            return results;
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