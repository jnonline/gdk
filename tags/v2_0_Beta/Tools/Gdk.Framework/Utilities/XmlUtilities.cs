/*
  Copyright (c) 2011, Raincity Games LLC
  Licensed under the MIT license: http://www.opensource.org/licenses/mit_license.php
*/

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml;

namespace Gdk
{
    public class XmlUtilities
    {
        /// <summary>
        /// Determines if the given attribute exists
        /// </summary>
        static public bool AttributeExists(XmlNode xmlNode, string attributeName)
        {
            XmlAttribute attribute = xmlNode.Attributes[attributeName];
            if (attribute == null)
                return false;
            return true;
        }

        /// <summary>
        /// Verify attributes exist in the given xml node
        /// </summary>
        /// <param name="xmlNode">Xml node that contains the attributes</param>
        /// <param name="attributes">List of attributes to look for in the xml node</param>
        static public bool VerifyAttributesExist(XmlNode xmlNode, bool throwOnFailure, params string[] attributes)
        {
            // Loop through and verify each attribute exists
            foreach (string attributeName in attributes)
            {
                if (xmlNode.Attributes[attributeName] == null)
                {
                    // Throw an exception on failure?
                    if (throwOnFailure == true)
                        throw new XmlException("The xml node " + xmlNode.Name + " is missing the attribute " + attributeName);

                    return false;
                }
            }

            return true;
        }

        /// <summary>
        /// Retrieves the value of the given attribute or the default value if specified
        /// </summary>
        static public string GetAttribute(XmlNode xmlNode, string attributeName, string defaultValue)
        {
            if (xmlNode == null)
                return defaultValue;
            XmlAttribute attribute = xmlNode.Attributes[attributeName];
            if (attribute == null)
                return defaultValue;
            return attribute.Value;
        }

        /// <summary>
        /// Retrieves the value of the given attribute or the default value if specified
        /// </summary>
        static public bool GetAttribute(XmlNode xmlNode, string attributeName, bool defaultValue)
        {
            if (xmlNode == null)
                return defaultValue;
            XmlAttribute attribute = xmlNode.Attributes[attributeName];
            if (attribute == null)
                return defaultValue;
            return Boolean.Parse(attribute.Value);
        }

        /// <summary>
        /// Retrieves the value of the given attribute or the default value if specified
        /// </summary>
        static public int GetAttribute(XmlNode xmlNode, string attributeName, int defaultValue)
        {
            if (xmlNode == null)
                return defaultValue;
            XmlAttribute attribute = xmlNode.Attributes[attributeName];
            if (attribute == null)
                return defaultValue;
            return Int32.Parse(attribute.Value);
        }

        /// <summary>
        /// Retrieves the value of the given attribute or the default value if specified
        /// </summary>
        static public int[] GetAttribute(XmlNode xmlNode, string attributeName, int[] defaultValues)
        {
            if (xmlNode == null)
                return defaultValues;
            XmlAttribute attribute = xmlNode.Attributes[attributeName];
            if (attribute == null)
                return defaultValues;

            // Break the comma-delimited list into its parts
            string[] parts = attribute.Value.Split(',');
            int numParts = parts.GetUpperBound(0);

            // Copy the parts to the array
            for (int index = 0; index <= numParts; index++)
                defaultValues[index] = int.Parse(parts[index]);
            return defaultValues;
        }

        /// <summary>
        /// Retrieves the value of the given attribute or the default value if specified
        /// </summary>
        static public byte[] GetAttribute(XmlNode xmlNode, string attributeName, byte[] defaultValues)
        {
            if (xmlNode == null)
                return defaultValues;
            XmlAttribute attribute = xmlNode.Attributes[attributeName];
            if (attribute == null)
                return defaultValues;

            // Break the comma-delimited list into its parts
            string[] parts = attribute.Value.Split(',');
            int numParts = parts.GetUpperBound(0);
           
            // Copy the parts to the array
            for (int index = 0; index <= numParts; index++)
                defaultValues[index] = byte.Parse(parts[index]);
            return defaultValues;
        }

        /// <summary>
        /// Retrieves the value of the given attribute or the default value if specified
        /// </summary>
        static public float GetAttribute(XmlNode xmlNode, string attributeName, float defaultValue)
        {
            if (xmlNode == null)
                return defaultValue;
            XmlAttribute attribute = xmlNode.Attributes[attributeName];
            if (attribute == null)
                return defaultValue;
            return float.Parse(attribute.Value);
        }

        /// <summary>
        /// Retrieves the value of the given attribute or the default value if specified
        /// </summary>
        static public double GetAttributeDouble(XmlNode xmlNode, string attributeName, double defaultValue)
        {
            if (xmlNode == null)
                return defaultValue;
            XmlAttribute attribute = xmlNode.Attributes[attributeName];
            if (attribute == null)
                return defaultValue;
            return double.Parse(attribute.Value);
        }

        /// <summary>
        /// Retrieves the value of the given attribute or the default value if specified
        /// </summary>
        static public Color GetAttribute(XmlNode xmlNode, string attributeName, Color defaultValue)
        {
            if (xmlNode == null)
                return defaultValue;
            XmlAttribute attribute = xmlNode.Attributes[attributeName];
            if (attribute == null)
                return defaultValue;

            // Parse the color values
            string[] parts = attribute.Value.Split(',');
            if (parts.Length == 3)
            {
                // The color entry is an rgb triplet
                return new Color(
                    byte.Parse(parts[0].Trim()),
                    byte.Parse(parts[1].Trim()),
                    byte.Parse(parts[2].Trim())
                    );
            }
            else if (parts.Length == 4)
            {
                // The color entry is an rgab quadruplet
                return new Color(
                    byte.Parse(parts[0].Trim()),
                    byte.Parse(parts[1].Trim()),
                    byte.Parse(parts[2].Trim()),
                    byte.Parse(parts[3].Trim())
                    );
            }
            else
                throw new ArgumentException("Invalid Color value in Xml: " + attribute.Value);
        }

        /// <summary>
        /// Retrieves the value of the given attribute or the default value if specified
        /// </summary>
        static public Vector3 GetAttribute(XmlNode xmlNode, string attributeName, Vector3 defaultValue)
        {
            if (xmlNode == null)
                return defaultValue;
            XmlAttribute attribute = xmlNode.Attributes[attributeName];
            if (attribute == null)
                return defaultValue;

            // Parse the vector triplet values
            string[] parts = attribute.Value.Split(',');
            if (parts.Length == 3)
            {
                // The color entry is an rgb triplet
                return new Vector3(
                    float.Parse(parts[0].Trim()),
                    float.Parse(parts[1].Trim()),
                    float.Parse(parts[2].Trim())
                    );
            }

            throw new ArgumentException("Invalid Vector3 value in Xml: " + attribute.Value);
        }

        /// <summary>
        /// Retrieves the value of the given attribute or the default value if specified
        /// </summary>
        static public Vector2 GetAttribute(XmlNode xmlNode, string attributeName, Vector2 defaultValue)
        {
            if (xmlNode == null)
                return defaultValue;
            XmlAttribute attribute = xmlNode.Attributes[attributeName];
            if (attribute == null)
                return defaultValue;

            // Parse the vector triplet values
            string[] parts = attribute.Value.Split(',');
            if (parts.Length == 2)
            {
                // The color entry is an rgb triplet
                return new Vector2(
                    float.Parse(parts[0].Trim()),
                    float.Parse(parts[1].Trim())
                    );
            }

            throw new ArgumentException("Invalid Vector2 value in Xml: " + attribute.Value);
        }

        /// <summary>
        /// Overloaded method that will return an attribute value in a specified enum type
        /// </summary>
        /// <typeparam name="T">The type of enum to return the value as</typeparam>
        /// <param name="node">XmlNode in wich the attribute may reside</param>
        /// <param name="attributeName">name of the attribute to be retrieved</param>
        /// <param name="defaultValue">the default value if the attribute is not present</param>
        /// <returns>attribute value if present, default value if not</returns>
        static public T GetAttributeEnumValue<T>(XmlNode node, string attributeName, T defaultValue)
        {
            if (node != null && node.Attributes != null)
            {
                XmlAttribute attrib = node.Attributes[attributeName];
                if (attrib != null)
                {
                    // Parse the enum value
                    return (T)Enum.Parse(typeof(T), attrib.Value, true);
                }
            }

            return defaultValue;
        }

		/// <summary>
		/// Escapes any XML characters in the string
		/// </summary>
		/// <param name="input"></param>
		/// <returns></returns>
		static public string EscapeXml(string input)
		{
			return input
				.Replace("&", "&amp;")
				.Replace("<", "&lt;")
				.Replace(">", "&gt;")
				;
		}
    }
}
