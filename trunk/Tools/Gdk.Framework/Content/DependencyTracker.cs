using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Xml;
using Gdk;

namespace Gdk.Content
{
    class AssetDependencies
    {
        public string AssetPath;
        public int AssetHash;
        public List<string> InputDependencies = new List<string>();
        public List<string> OutputDependencies = new List<string>();
    }

    public class DependencyTracker
    {
        Content content;
        DateTime trackerFileTimeStamp = DateTime.MinValue;
        Dictionary<string, AssetDependencies> assetDependenciesByAsset = new Dictionary<string,AssetDependencies>();

        /// <summary>
        /// Creates a new tracker that tracks the given content
        /// </summary>
        public DependencyTracker(Content content)
        {
            this.content = content;
            string trackerFilePath = content.GetDependencyTrackerFilePath();

            // Does the tracker file exist?
            if (File.Exists(trackerFilePath) == true)
            {
                // Is the tracker file older than this executable?
                string exeFilePath = System.Reflection.Assembly.GetEntryAssembly().Location;
                DateTime exeLastBuildTime = File.GetLastWriteTimeUtc(exeFilePath);
                DateTime trackerLastWriteTime = File.GetLastWriteTimeUtc(trackerFilePath);
                if (exeLastBuildTime > trackerLastWriteTime)
                    return;

                // Load & parse the tracker file 
                XmlDocument xmlDocument;
                try
                {
                    xmlDocument = new XmlDocument();
                    xmlDocument.Load(trackerFilePath);

                    XmlNode xmlRoot = xmlDocument.DocumentElement;

                    // Load the tracked assets from the file
                    foreach (XmlNode xmlAsset in xmlRoot.ChildNodes)
                    {
                        AssetDependencies assetDependencies = new AssetDependencies();
                        assetDependencies.AssetPath = xmlAsset.Attributes["Path"].Value;
                        assetDependencies.AssetHash = int.Parse(xmlAsset.Attributes["AssetHash"].Value);

                        // Load the <Input> & <Output> nodes
                        foreach (XmlNode xmlDependency in xmlAsset.ChildNodes)
                        {
                            if (xmlDependency.Name == "Input")
                                assetDependencies.InputDependencies.Add(xmlDependency.InnerText);
                            else if (xmlDependency.Name == "Output")
                                assetDependencies.OutputDependencies.Add(xmlDependency.InnerText);
                        }

                        // Add this asset to our dictionary
                        assetDependenciesByAsset.Add(assetDependencies.AssetPath, assetDependencies);
                    }
                }
                catch (XmlException)
                {
                    // The xml in the tracker is bad, so blow away our tracker state data
                    assetDependenciesByAsset = new Dictionary<string, AssetDependencies>();
                }
            }
        }

        /// <summary>
        /// Saves this tracker info to the tracker file
        /// </summary>
        public void Save()
        {
            lock(this)
            {
                string trackerFilePath = content.GetDependencyTrackerFilePath();
                XmlWriter writer = XmlWriter.Create(trackerFilePath);

                // Write the root element
                writer.WriteStartElement("TrackedContent");

                // Loop through the assets
                foreach (KeyValuePair<string, AssetDependencies> assetDependencies in assetDependenciesByAsset)
                {
                    // Write the <Asset> element
                    writer.WriteStartElement("Asset");
                    writer.WriteAttributeString("Path", assetDependencies.Key);
                    writer.WriteAttributeString("AssetHash", assetDependencies.Value.AssetHash.ToString());

                    // Write the input dependencies
                    foreach (string inputDependency in assetDependencies.Value.InputDependencies)
                        writer.WriteElementString("Input", inputDependency);

                    // Write the output dependencies
                    foreach (string outputDependency in assetDependencies.Value.OutputDependencies)
                        writer.WriteElementString("Output", outputDependency);

                    // Close the <Asset> element
                    writer.WriteEndElement();
                }

                // Close the root element
                writer.WriteEndElement();

                writer.Flush();
                writer.Close();
            }
        }

        /// <summary>
        /// Removes all the tracker state for a dependency
        /// </summary>
        public void RemoveAsset(Asset asset)
        {
            lock (this)
            {
                if(assetDependenciesByAsset.ContainsKey(asset.Path))
                    assetDependenciesByAsset.Remove(asset.Path);
            }
        }

        /// <summary>
        /// Adds an input dependency for the given asset
        /// </summary>
        public void AddInputDependency(Asset asset, string inputFilePath)
        {
            lock (this)
            {
                // Get the dependencies for the given asset
                AssetDependencies assetDependencies = assetDependenciesByAsset[asset.Path];

                // Add the input dependency
                assetDependencies.InputDependencies.Add(inputFilePath);   
            }
        }

        /// <summary>
        /// Adds an output dependency for the given asset
        /// </summary>
        public void AddOutputDependency(Asset asset, string outputFilePath)
        {
            lock (this)
            {
                // Get the dependencies for the given asset
                AssetDependencies assetDependencies = assetDependenciesByAsset[asset.Path];

                // Add the output dependency
                assetDependencies.OutputDependencies.Add(outputFilePath);
            }
        }

        /// <summary>
        /// Adds an output dependency for the given asset
        /// </summary>
        public void AddAssetDependency(Asset asset)
        {
            lock (this)
            {
                AssetDependencies assetDependencies = new AssetDependencies();
                assetDependencies.AssetPath = asset.Path;
                assetDependencies.AssetHash = asset.GetHashCode();
                assetDependenciesByAsset.Add(asset.Path, assetDependencies);
            }
        }


        /// <summary>
        /// Checks if the given file needs 
        /// </summary>
        /// <param name="asset"></param>
        /// <returns></returns>
        public bool DoesAssetNeedRebuild(Asset asset)
        {
            lock (this)
            {
                DateTime newestInputFileTime = DateTime.MinValue;
                DateTime oldestOutputFileTime = DateTime.MaxValue;

                // Do we not even have tracker data for this file?
                if (assetDependenciesByAsset.ContainsKey(asset.Path) == false)
                    return true;

                // Get the tracker data
                AssetDependencies assetDependencies = assetDependenciesByAsset[asset.Path];

                // Does the hash not match?
                int assetHash = asset.GetHashCode();
                if (assetDependencies.AssetHash != assetHash)
                    return true;

                // Are ANY of the input files missing?
                foreach (string inputDependency in assetDependencies.InputDependencies)
                {
                    string fullPath = Path.Combine(content.RootPath, inputDependency);
                    if (File.Exists(fullPath) == false)
                        return true;

                    // Track the newest input file
                    DateTime lastWriteTime = File.GetLastWriteTime(fullPath);
                    if (lastWriteTime > newestInputFileTime)
                        newestInputFileTime = lastWriteTime;
                }

                // Are ANY of the output files missing?
                foreach (string outputDependency in assetDependencies.OutputDependencies)
                {
                    string fullPath = Path.Combine(content.GetBuildFolder(), outputDependency);
                    if (File.Exists(fullPath) == false)
                        return true;

                    // Track the oldest output file
                    DateTime lastWriteTime = File.GetLastWriteTime(fullPath);
                    if (lastWriteTime < oldestOutputFileTime)
                        oldestOutputFileTime = lastWriteTime;
                }

                // Is the most recently changed input file, newer than the oldest updated output file?
                if (newestInputFileTime > oldestOutputFileTime)
                    return true;

                // All the dependency files exist & all the input files are OLDER than all the output files
                // no need to rebuild
                return false;
            }
        }
    }
}
