/*
  Copyright (c) 2011, Raincity Games LLC
  Licensed under the MIT license: http://www.opensource.org/licenses/mit_license.php
*/

using System;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Xml;
using Gdk.Framework;

namespace Gdk.Framework.Content
{
    /// <summary>
    /// Represents a bundle of GDK assets
    /// </summary>
    public class Bundle
    {
        /// <summary>
        /// The name of the bundle  (Also the filename of the .bundle file)
        /// </summary>
        public string Name = string.Empty;

        /// <summary>
        /// This is the path where the .bundle file was loaded (and thusly where all the asset paths are relative to)
        /// </summary>
        public string RootPath = string.Empty;

        /// <summary>
        /// List of the assets that are part of this bundle
        /// </summary>
        public List<Asset> Assets = new List<Asset>();

        /// <summary>
        /// Create a new bundle
        /// </summary>
        public Bundle(string name, string rootPath)
        {
            this.Name = name;
            this.RootPath = rootPath;
        }

        /// <summary>
        /// Loads bundle from an xml file
        /// </summary>
        public static Bundle FromFile(string bundleFilePath)
        {
            // Load the file into an Xml Dom
            XmlDocument xmlDoc = new XmlDocument();
            xmlDoc.Load(bundleFilePath);

            // Get the root <Bundle> node
            XmlNode xmlBundle = xmlDoc.DocumentElement;

            // Set the name & root path
            string bundleName = System.IO.Path.GetFileNameWithoutExtension(bundleFilePath);
            string bundleRootPath = System.IO.Path.GetDirectoryName(bundleFilePath).Replace('\\', '/');

            // Create the bundle object
            Bundle bundle = new Bundle(bundleName, bundleRootPath);

            // Load the Assets/Asset nodes
            foreach(XmlNode xmlAssets in xmlBundle.SelectNodes("Assets"))
			{
				foreach (XmlNode xmlAsset in xmlAssets.SelectNodes("Asset"))
				{
                    bundle.Assets.Add(Asset.FromXml(xmlAsset));
				}
            }

            return bundle;
        }

        /// <summary>
        /// Saves this bundle
        /// </summary>
        public void Save()
        {
            // Build the path to the bundle file
            string bundleFilePath = GetBundleFilePath();

            // Setup the settings for the writer
            XmlWriterSettings writerSettings = new XmlWriterSettings();
            writerSettings.Indent = true;
            writerSettings.IndentChars = "\t";
            writerSettings.NewLineOnAttributes = false;
            writerSettings.OmitXmlDeclaration = true;

            // Create an XmlWriter
            XmlWriter writer = XmlWriter.Create(bundleFilePath, writerSettings);

            // Write the <Bundle> node
            writer.WriteStartElement("Bundle");

            // Write the Assets
            writer.WriteStartElement("Assets");
            foreach (Asset asset in this.Assets)
            {
                // Write the asset
                asset.WriteXml(writer);
            }
            writer.WriteEndElement();

            // Write the </Bundle> end
            writer.WriteEndElement();

            // Close the writer
            writer.Flush();
            writer.Close();
        }
		
		public string GetBundleFilePath()
		{
			return Path.Combine(this.RootPath, this.Name).Replace('\\','/') + ".bundle";
		}
    }
}

