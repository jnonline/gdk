/*
  Copyright (c) 2011, Raincity Games LLC
  Licensed under the MIT license: http://www.opensource.org/licenses/mit_license.php
*/

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml;
using Gdk.Framework;

namespace Gdk.Framework.Content
{
    public class ParameterSet : Dictionary<string, string>
    {
        // Make clone of this parameters object
        public ParameterSet Clone()
        {
            ParameterSet newParams = new ParameterSet();
            foreach (KeyValuePair<string, string> pair in this)
                newParams.Add(pair.Key, pair.Value);
            return newParams;
        }

        /// <summary>
        /// Merges a set of Parameters into this parameter set.  If the parameter already exists, it's value is overridden
        /// </summary>
        public void Merge(ParameterSet parameters)
        {
            foreach (KeyValuePair<string, string> pair in parameters)
                this[pair.Key] = pair.Value;
        }

        /// <summary>
        /// Generates a hash of the parameter strings
        /// </summary>
        public override int GetHashCode()
        {
            string stringBlob = string.Empty;
            foreach (KeyValuePair<string, string> pair in this)
                stringBlob += pair.Key + "=" + pair.Value + "|";
            return stringBlob.GetHashCode();
        }

        /// <summary>
        /// Gets a value of a named parameter, or returns the default value if the named parameter doesn't exist
        /// </summary>
        public string GetValue(string name, string defaultValue)
        {
            string value;
            if(this.TryGetValue(name, out value) == false)
                return defaultValue;
            return value;
        }

        /// <summary>
        /// Gets a value of a named parameter, or returns the default value if the named parameter doesnt exist
        /// </summary>
        public int GetValue(string name, int defaultValue)
        {
            string value;
            if (this.TryGetValue(name, out value) == false)
                return defaultValue;
            return int.Parse(value);
        }

        /// <summary>
        /// Gets a value of a named parameter, or returns the default value if the named parameter doesnt exist
        /// </summary>
        public float GetValue(string name, float defaultValue)
        {
            string value;
            if (this.TryGetValue(name, out value) == false)
                return defaultValue;
            return float.Parse(value);
        }

        /// <summary>
        /// Gets a value of a named parameter, or returns the default value if the named parameter doesnt exist
        /// </summary>
        public bool GetValue(string name, bool defaultValue)
        {
            string value;
            if (this.TryGetValue(name, out value) == false)
                return defaultValue;
            return bool.Parse(value);
        }

        /// <summary>
        /// Gets a value of a named parameter, or returns the default value if the named parameter doesnt exist
        /// </summary>
        public Color GetValue(string name, Color defaultValue)
        {
            string value;
            if (this.TryGetValue(name, out value) == false)
                return defaultValue;
            return Color.Parse(value);
        }

        /// <summary>
        /// Gets a value of a named parameter, or returns the default value if the named parameter doesnt exist
        /// </summary>
        public Vector2 GetValue(string name, Vector2 defaultValue)
        {
            string value;
            if (this.TryGetValue(name, out value) == false)
                return defaultValue;
            return Vector2.Parse(value, true);
        }

        /// <summary>
        /// Gets a value of a named parameter, or returns the default value if the named parameter doesnt exist
        /// </summary>
        public Vector3 GetValue(string name, Vector3 defaultValue)
        {
            string value;
            if (this.TryGetValue(name, out value) == false)
                return defaultValue;
            return Vector3.Parse(value, true);
        }

        /// <summary>
        /// Gets a value of a named parameter, or returns the default value if the named parameter doesnt exist
        /// </summary>
        public T GetEnumValue<T>(string name, T defaultValue)
        {
            string value;
            if (this.TryGetValue(name, out value) == false)
                return defaultValue;
            return (T)Enum.Parse(typeof(T), value, true);
        }

        /// <summary>
        /// Writes this parameter set to the given xml writer
        /// </summary>
        public void WriteXml(XmlWriter writer, string elementName)
        {
            // Star the <Parameters> element
            writer.WriteStartElement(elementName);

            // Loop through the parameters
            foreach(KeyValuePair<string,string> pair in this)
            {
                // Write the <Parameter Name="name">Value</Parameter>  blob
                writer.WriteStartElement("Parameter");
                writer.WriteAttributeString("Name", pair.Key);
                writer.WriteValue(pair.Value);
                writer.WriteEndElement();
            }

            // End the </Parameters> element
            writer.WriteEndElement();
        }

        public void WriteXml(XmlWriter writer)
        {
            WriteXml(writer, "Parameters");
        }

        /// <summary>
        /// Loads a parameters list from an xml <Parameters> element
        /// </summary>
        public static ParameterSet FromXml(XmlNode xmlParameters)
        {
            // Create the parameters
            ParameterSet parameters = new ParameterSet();

            // Loop through all the child <Parameter> nodes
            foreach (XmlNode xmlChild in xmlParameters.ChildNodes)
            {
                // Is this a <Parameter> node?
                if (xmlChild.Name == "Parameter")
                {
                    // Get the parameter name & value
                    string name = xmlChild.Attributes["Name"].Value;
                    string value = xmlChild.InnerXml;
                    parameters.Add(name, value);
                }
            }

            return parameters;
        }
    }
}
