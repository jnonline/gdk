/*
  Copyright (c) 2011, Raincity Games LLC
  Licensed under the MIT license: http://www.opensource.org/licenses/mit_license.php
*/

using System;
using System.Collections.Generic;
using System.Linq;
using System.IO;
using System.Text;
using Gdk.Framework;

namespace Gdk.Framework.Content
{
	/// <summary>
	/// Possible status' for an asset during building
	/// </summary>
	public enum BuildStatus
	{
		Building,
		Skipped,
		Failed,
		Success,
		SuccessWithWarnings
	}
		
	/// <summary>
	/// This class builds GDK content/assets
	/// </summary>
    public class ContentBuilder
    {
		// Build events
		public event Action<bool> BuildCompleted;
		public event Action<Asset, BuildStatus> BuildStatusChanged;
		public event Action<Asset, string> ErrorGenerated;
		public event Action<Asset, string> WarningGenerated;
		public event Action<Asset, string> NewLogMessage;
		public event Action<Asset, string, string> OutputGenerated;
		

        /// <summary>
        /// Constructor
        /// </summary>
        public ContentBuilder()
        {
        }

        /// <summary>
        /// Builds the given bundle of assets
        /// </summary>
        public bool Build(Bundle bundle, string destinationPath, bool forceRebuild)
        {
			// Track the overall status of the build as successfull (until proven otherwise)
			bool buildSucceeded = true;
			
			// Build the path to the destination for the built assets
			//		[$(Destination) \ [Bundle]
			string buildOutputFolder = Path.Combine(destinationPath, bundle.Name);
			
			// Build the path to the intermediate build file
			//		[$(Destination) \ bundle.build
			string intermediateBuildFilePath = Path.Combine(destinationPath, bundle.Name + ".build");
			
            // Load/Create the intermediate build data
			IntermediateBuildData buildData = new IntermediateBuildData(intermediateBuildFilePath);
			
			// Setup the dependency tracker's input & output folders
			buildData.DependencyTracker.SourceFolder = bundle.RootPath;
			buildData.DependencyTracker.OutputFolder = buildOutputFolder;

            // Loop through the assets in the bundle
            foreach (Asset asset in bundle.Assets)
            {
				// Check & Setup dependency tracking for this asset
				// ------------------------------------------------
				
                // Does this asset need to be re-built?
                if (!forceRebuild && buildData.DependencyTracker.DoesAssetNeedRebuild(asset) == false)
                {
					LogMessage(asset, "<i>Skipping build.  Asset is already up to date</i>");
					
					// Asset is already up-to-date, skip building
					SetAssetBuildStatus(asset, BuildStatus.Skipped);
					
					// Continue to the next asset
                    continue;
                }
				
				// Update the asset BuildStatus to "Building"
				SetAssetBuildStatus(asset, BuildStatus.Building);
                
                // Remove the dependency tracking info for this asset (so it is fresh with new build data)
                buildData.DependencyTracker.RemoveAsset(asset);

                // Add dependency tracking for the asset & its source file
                buildData.DependencyTracker.AddAssetDependency(asset);
                buildData.DependencyTracker.AddInputDependency(asset, asset.Path);
				
				// Construct the processor parameters for this asset
				// -------------------------------------------------
				
				// Get the details of the processor used by this asset
				ProcessorDetails processorDetails = ProcessorManager.GetProcessorDetails(asset.ProcessorType);
                
                // Get the default parameters for the processor
                ParameterSet parameters = new ParameterSet();
                foreach (KeyValuePair<string, ProcessorParameter> processorParameter in processorDetails.Parameters)
                    parameters.Add(processorParameter.Key, processorParameter.Value.DefaultValue.ToString());

                // Apply any parameter overrides from the asset
                parameters.Merge(asset.ProcessorParameters);
				
				// Setup the build context
				// ---------------------------------------------------
				
	            // Create a context for this asset build
	            BuildContext context = new BuildContext(
				    this,
					bundle.RootPath, 
					buildOutputFolder, 
					bundle, 
					asset, 
					parameters, 
					buildData.DependencyTracker
					);
	
				// Log the processor parameters
				// ---------------------------------------------------
				
	            // Log the processor parameters
	            context.Log("<b><u>Parameters</u></b>");
	            foreach (KeyValuePair<string, string> parameter in parameters)
	            {
	                context.Log(parameter.Key + " = " + parameter.Value);
	            }
	            context.Log("");
		
				// Create & Launch the processor
				// ---------------------------------------------------
					
	            // Create an instance of the processor for this asset
	            Processor processor = ProcessorManager.GetProcessor(asset.ProcessorType);
	
	            // Setup the processor
	            processor.Asset = asset;
	            processor.Context = context;
	
	            // Log the "Start"
	            context.Log("<b><u>Starting asset build.</u></b>");
		
				// Catch build exceptions
                try
                {
		            // Process the asset
		            processor.Process();
				}
                catch (Exception ex)
                {
					// Log the exception (and generate an error for it)
                    context.LogException(ex, true);
                }
				
	            // Process the build results
				// -------------------------------------------------
				
				// Did we encounter any errors during the build?
	            if (context.NumErrors > 0)
				{
					context.Log("\n<color:#B00000><b>Asset build failed.</b></color>");
				
					// Flag the overall build status as failed
                    buildSucceeded = false;

                    // Remove tracking state for this asset, it needs to be re-built
                    buildData.DependencyTracker.RemoveAsset(asset);

                    // Update the asset BuildStatus to "Failed"
					SetAssetBuildStatus(asset, BuildStatus.Failed);
				}
				else
				{
					// No errors!
		            context.Log("\n<b>Completed asset build.</b>");
	
					// Log the outputs
		            context.Log("\n<b><u>Build Outputs:</u></b>");
		            foreach (string buildOutput in context.BuildOutputs)
		            {
		                string fullOutputPath = Path.Combine(buildOutputFolder, buildOutput).Replace('\\','/');
						string displayName = Path.Combine(bundle.Name, buildOutput).Replace('\\', '/');
						
						// Write the build output to the log
		                context.Log("<link:gdkasset:" + fullOutputPath + ">" + displayName + "</link>");
						
						// Raise the OuputGenerated message for this output
						if(this.OutputGenerated != null)
							this.OutputGenerated(asset, displayName, fullOutputPath);
		            }
		            context.Log("");

					// Did we encounter any warnings?
					if(context.NumWarnings > 0)
					{
						// Update the asset BuildStatus to "SuccessWithWarnings"
						SetAssetBuildStatus(asset, BuildStatus.SuccessWithWarnings);
					}
					else
					{
						// Update the asset BuildStatus to "Success"
						SetAssetBuildStatus(asset, BuildStatus.Success);
					}
				}
            }

            // Save the intermediate build file data
            buildData.Save();
			
			// Raise the BuildCompleted event
			if(BuildCompleted != null)
				BuildCompleted(buildSucceeded);
				
            return buildSucceeded;
        }
		
		/// <summary>
		/// Sets the build status for the given asset
		/// </summary>
        public void SetAssetBuildStatus(Asset asset, BuildStatus buildStatus)
        {
			if(BuildStatusChanged != null)
				BuildStatusChanged(asset, buildStatus);
		}
		
		/// <summary>
		/// Raises an error for the given asset
		/// </summary>
        public void GenerateError(Asset asset, string message)
        {
			if(ErrorGenerated != null)
				ErrorGenerated(asset, message);
		}
		
		/// <summary>
		/// Raises a warning for the given asset
		/// </summary>
        public void GenerateWarning(Asset asset, string message)
        {
			if(WarningGenerated != null)
				WarningGenerated(asset, message);
		}
		
		/// <summary>
		/// Writes a new log line for the given asset
		/// </summary>
        public void LogMessage(Asset asset, string html)
        {
			if(NewLogMessage != null)
				NewLogMessage(asset, html);
		}
    }
}
