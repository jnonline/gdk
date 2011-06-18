using System;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Xml;
using Gdk;

namespace Gdk.Content
{
    /// <summary>
    /// Represents a set of gdk content
    /// </summary>
    public class Content
    {
        /// <summary>
        /// The name of the content  (Also the filename of the .content file)
        /// </summary>
        public string Name = string.Empty;

        /// <summary>
        /// This is the path where the .content file was loaded (and thusly where all the content paths are relative to)
        /// </summary>
        public string RootPath = string.Empty;

        /// <summary>
        /// List of the assets that are part of this content
        /// </summary>
        public List<Asset> Assets = new List<Asset>();

        /// <summary>
        /// List of additional bundles contained in this content
        /// </summary>
        public List<Bundle> Bundles = new List<Bundle>();

        /// <summary>
        /// Saves content to an xml file
        /// </summary>
        public Content(string name, string rootPath)
        {
            this.Name = name;
            this.RootPath = rootPath;
        }

        /// <summary>
        /// Loads content from an xml file
        /// </summary>
        public static Content FromFile(string contentFilePath)
        {
            // Load the file into an Xml Dom
            XmlDocument xmlDoc = new XmlDocument();
            xmlDoc.Load(contentFilePath);

            // Get the root <Content> node
            XmlNode xmlContent = xmlDoc.DocumentElement;

            // Set the name & root path
            string contentName = System.IO.Path.GetFileNameWithoutExtension(contentFilePath);
            string contentRootPath = System.IO.Path.GetDirectoryName(contentFilePath);

            // Create the content object
            Content content = new Content(contentName, contentRootPath);

            // Load the Bundles
            XmlNode xmlBundles = xmlContent.SelectSingleNode("Bundles");
            if (xmlBundles != null)
            {
                foreach (XmlNode xmlBundle in xmlBundles.SelectNodes("Bundle"))
                    content.Bundles.Add(Bundle.FromXml(xmlBundle));
            }

            // Load the Assets
            XmlNode xmlAssets = xmlContent.SelectSingleNode("Assets");
            if (xmlAssets != null)
            {
                foreach (XmlNode xmlAsset in xmlAssets.SelectNodes("Asset"))
                    content.Assets.Add(Asset.FromXml(xmlAsset));
            }

            return content;
        }

        /// <summary>
        /// Saves this content into the content file
        /// </summary>
        /// <param name="contentFilePath"></param>
        public void Save()
        {
            // Build the path to the content file
            string contentFilePath = GetContentFilePath();

            // Setup the settings for the writer
            XmlWriterSettings writerSettings = new XmlWriterSettings();
            writerSettings.Indent = true;
            writerSettings.IndentChars = "\t";
            writerSettings.NewLineOnAttributes = false;
            writerSettings.OmitXmlDeclaration = true;

            // Create an XmlWriter
            XmlWriter writer = XmlWriter.Create(contentFilePath, writerSettings);

            // Write the <Content> node
            writer.WriteStartElement("Content");

            // Write the Bundles
            writer.WriteStartElement("Bundles");
            foreach (Bundle bundle in this.Bundles)
            {
                // Write the bundle
                bundle.WriteXml(writer);
            }
            writer.WriteEndElement();

            // Write the Assets
            writer.WriteStartElement("Assets");
            foreach (Asset asset in this.Assets)
            {
                // Write the asset
                asset.WriteXml(writer);
            }
            writer.WriteEndElement();

            // Write the </Content> end
            writer.WriteEndElement();

            // Close the writer
            writer.Flush();
            writer.Close();
        }

        /// <summary>
        /// Returns true if this content has the given bundle
        /// </summary>
        public bool IsValidBundle(string bundleName)
        {
            foreach (Bundle bundle in Bundles)
                if (bundle.Name == bundleName)
                    return true;
            return false;
        }

        // Utility methods for getting paths
        // **********************************************************************************

        public const string CONTENT_EXTENSION = "content";
        public const string CONTENT_BUILD_EXTENSION = "content.build";
        public const string BUILD_FOLDER = "Build";

        /// <summary>
        /// Get the path to the content file
        /// </summary>
        /// <returns></returns>
        public string GetContentFilePath()
        {
            string fileName = this.Name + "." + CONTENT_EXTENSION;
            string filePath = Path.Combine(this.RootPath, fileName);
            return filePath;
        }

        /// <summary>
        /// Gets the path to the folder where this content will be built
        /// </summary>
        public string GetBuildFolder()
        {
            return Path.Combine(this.RootPath, BUILD_FOLDER);
        }

        /// <summary>
        /// Gets the path to the folder where a specific bundle of the content will be built
        /// </summary>
        public string GetBundleBuildFolder(string bundleName)
        {
            return Path.Combine(GetBuildFolder(), bundleName);
        }

        /// <summary>
        /// Gets the path to the dependency tracker file for this content
        /// </summary>
        /// <returns></returns>
        public string GetDependencyTrackerFilePath()
        {
            //  Output:   [BuildDestination]\..\[Content.Name].gdkcontentbuild
            string trackerFileName = this.Name + "." + CONTENT_BUILD_EXTENSION;
            string trackerFilePath = Path.Combine(this.RootPath, trackerFileName);

            return trackerFilePath;
        }
    }
}

