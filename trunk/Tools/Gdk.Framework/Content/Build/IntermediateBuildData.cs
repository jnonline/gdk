/*
  Copyright (c) 2011, Raincity Games LLC
  Licensed under the MIT license: http://www.opensource.org/licenses/mit_license.php
*/

using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Xml;
using Gdk.Framework;

/*
 * Intermediate build file format:
 * 
 *	<ContentBuild>
 *
 *		// Dependency tracker information, 1 <Asset> per asset
 * 		<TrackedAssets>
 * 			<Asset
 * 				Path="[asset rel path]"
 * 				AssetHash="[UInt32]"
 * 				>
 * 				<Input>[file rel path]</Input>
 *				<Output>[file rel path]</Output>
 *			</Asset>
 *			...
 *		</TrackedAssets>
 *
 *	</ContentBuild>
 *
 */

namespace Gdk.Framework.Content
{
    public class IntermediateBuildData
    {
		public string SourcePath;
		
		
		private DependencyTracker dependencyTracker = new DependencyTracker();
		public DependencyTracker DependencyTracker { get { return dependencyTracker; } }
		
        /// <summary>
        /// Creates a new tracker that tracks the given content
        /// </summary>
        public IntermediateBuildData(string filePath)
        {
			this.SourcePath = filePath;
			
			// Does the intermediate build data file NOT exist?
            if (File.Exists(filePath) == false)
            {
				return;
			}
			
			// Load the intermediate build file xml
			XmlDocument xmlDoc = new XmlDocument();
			try
			{
				xmlDoc.Load(filePath);
			}
			catch(Exception)
			{
				// Failed to load the intermediate file, just start out blank
				return;
			}
			
			// Is the intermediate build file older than this assembly?  
            string exeFilePath = System.Reflection.Assembly.GetEntryAssembly().Location;
            DateTime exeLastBuildTime = File.GetLastWriteTimeUtc(exeFilePath);
            DateTime trackerLastWriteTime = File.GetLastWriteTimeUtc(this.SourcePath);
            if (exeLastBuildTime > trackerLastWriteTime)
			{
				// The content is old & should be fully rebuilt
				
				// Leave the dependency tracker empty, so a full rebuild happens
			}
			else
			{
				// Load the dependency tracker data from the file xml
				XmlNode xmlTrackedAssets = xmlDoc.DocumentElement.SelectSingleNode("TrackedAssets");
				if(xmlTrackedAssets != null)
					this.dependencyTracker.LoadFromXml(xmlTrackedAssets);
            }
        }

        /// <summary>
        /// Saves the intermediate build data to the file
        /// </summary>
        public void Save()
        {
			// Create an XmlWriter for the file
			XmlWriter writer = XmlWriter.Create(this.SourcePath);

            // Write the root element
            writer.WriteStartElement("ContentBuild");
			
			// Write the dependency tracker data
			writer.WriteStartElement("TrackedAssets");
			this.dependencyTracker.SaveToXml(writer);
			writer.WriteEndElement();
			
			// Close the root element & the file
			writer.WriteEndElement();
			writer.Flush();
			writer.Close();
        }
        
    }
}
