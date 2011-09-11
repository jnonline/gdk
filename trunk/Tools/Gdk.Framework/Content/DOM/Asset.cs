/*
  Copyright (c) 2011, Raincity Games LLC
  Licensed under the MIT license: http://www.opensource.org/licenses/mit_license.php
*/

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml;
using System.IO;
using Gdk.Framework;

namespace Gdk.Framework.Content
{
    /// <summary>
    /// Information for building a single asset
    /// </summary>
    public class Asset
    {
        /// <summary>
        /// Path of the asset
        /// </summary>
        public string Path;
        public string ProcessorType;
        public ParameterSet ProcessorParameters = new ParameterSet();

        /// <summary>
        /// Custom user data
        /// </summary>
        public object Tag;
		
		/// <summary>
        /// Log storage for the build process, DO NOT USE THIS FROM A PROCESSOR
        /// </summary>
        public System.Text.StringBuilder BuildLog = new System.Text.StringBuilder();

        /// <summary>
        /// Gets the Name of the resource (Which is the file name portion of the asset file, excluding the extension)
        /// </summary>
        public string Name { get { return System.IO.Path.GetFileName(this.Path); } }

        /// <summary>
        /// Writes the details of this asset to an <Asset> node
        /// </summary>
        public void WriteXml(XmlWriter writer)
        {
            // Write the <Asset> element
            writer.WriteStartElement("Asset");

            // Attributes
            // -----------------------

            // Write the asset attributes
            writer.WriteAttributeString("Path", this.Path);
            writer.WriteAttributeString("Processor", this.ProcessorType);

            // Child Elements
            // ------------------------

            // Write the base parameters
            ProcessorParameters.WriteXml(writer, "Parameters");

            //-------------------------

            // Close the <Asset> element
            writer.WriteEndElement();
        }

        /// <summary>
        /// Loads this asset from an xml <Asset> element
        /// </summary>
        public static Asset FromXml(XmlNode xmlAsset)
        {
            // Create the asset
            Asset asset = new Asset();

            // Get the Asset attributes
            asset.Path = XmlUtilities.GetAttribute(xmlAsset, "Path", string.Empty);
            asset.ProcessorType = XmlUtilities.GetAttribute(xmlAsset, "Processor", string.Empty);

            // Load the parameters
            XmlNode xmlParameters = xmlAsset.SelectSingleNode("Parameters");
            if(xmlParameters != null) 
                asset.ProcessorParameters = ParameterSet.FromXml(xmlParameters);

            // Return the asset
            return asset;
        }

        /// <summary>
        /// Gets a hash code for this asset
        /// </summary>
        /// <returns></returns>
        public override int GetHashCode()
        {
            // Build a big string of all the data in this asset
            string tag = this.Path;
            tag += "|" + this.ProcessorType;

            // Add the processor parameters
            tag += "|" + this.ProcessorParameters.Count.ToString();
            foreach (KeyValuePair<string, string> param in this.ProcessorParameters)
            {
                tag += "|" + param.Key + "=" + param.Value;
            }

            // Get the has code of the big data string
            return tag.GetHashCode();
        }
    }
}
