﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.IO;
using System.Text;
using Gdk;

namespace Gdk.Content
{
    public class Builder
    {
        public bool RequestStop;

        /// <summary>
        /// Constructor
        /// </summary>
        public Builder()
        {
        }

        /// <summary>
        /// Builds the given set of assets
        /// </summary>
        public bool Build(Content content, List<Asset> assets, Logger logger, bool forceBuild)
        {
            RequestStop = false;
            bool buildSucceeded = true;

            // Load the tracked dependencies state
            DependencyTracker dependencyTracker = new DependencyTracker(content);

            // Flip all the assets to "Waiting" status
            foreach (Asset asset in assets)
            {
                logger.SetAssetStatus(BuildStatus.Waiting, asset);
            }

            // Loop through the assets
            foreach (Asset asset in assets)
            {
                // Is there a stop request?
                if (RequestStop == true)
                    break;

                // Build this asset
                try
                {
                    // Set the status for this asset to "Building"
                    logger.SetAssetStatus(BuildStatus.Building, asset);

                    // Does this asset need to be re-built?
                    if (!forceBuild && dependencyTracker.DoesAssetNeedRebuild(asset) == false)
                    {
                        logger.Write(LogLevel.Info, "Asset up-to-date, skipping build.", asset);

                        // Set the status for this asset to "Skipped"
                        logger.SetAssetStatus(BuildStatus.Skipped, asset);

                        continue;
                    }
                    
                    // Remove the dependency tracking info for this asset (so it is fresh with new build data)
                    dependencyTracker.RemoveAsset(asset);

                    // Add dependency tracking for the asset & its source file
                    dependencyTracker.AddAssetDependency(asset);
                    dependencyTracker.AddInputDependency(asset, asset.Path);

                    // Build the "Base" Bundle
                    // ----------------------------------

                    // Get the default parameters for the processor
                    ProcessorDetails processorDetails = ProcessorManager.GetProcessorDetails(asset.ProcessorType);
                    ParameterSet defaultParameters = new ParameterSet();
                    foreach (KeyValuePair<string, ProcessorParameter> processorParameter in processorDetails.Parameters)
                        defaultParameters.Add(processorParameter.Key, processorParameter.Value.DefaultValue.ToString());

                    // Clone default parameters & merge in the asset parameters
                    ParameterSet baseParameters = defaultParameters.Clone();
                    baseParameters.Merge(asset.ProcessorParameters);

                    // Build the asset
                    bool success = BuildAsset(content, asset, "Base", baseParameters, dependencyTracker, logger);

                    // Build the custom bundles
                    // ----------------------------------

                    foreach (KeyValuePair<string, AssetBundle> assetBundle in asset.BundlesByName)
                    {
                        // Is this a valid bundle in the content?
                        if (content.IsValidBundle(assetBundle.Key))
                        {
                            // Clone the base asset parameters & Merge in the bundle specific parameters
                            ParameterSet customParameters = baseParameters.Clone();
                            customParameters.Merge(assetBundle.Value.ProcessorParameters);

                            // Build the asset customization
                            success &= BuildAsset(content, asset, assetBundle.Value.Name, customParameters, dependencyTracker, logger);
                        }
                    }

                    // ------------------------------

                    // Was this a complete success?
                    if (success)
                    {
                        // Set the status for this asset to "Success"
                        logger.SetAssetStatus(BuildStatus.Success, asset);
                    }
                    else
                    {
                        // Set the status for this asset to "SuccessWithWarning"
                        logger.SetAssetStatus(BuildStatus.SuccessWithWarning, asset);
                    }
                }
                catch (Exception ex)
                {
                    logger.WriteException("Asset build failed.", ex, asset);
                    buildSucceeded = false;

                    // Building this asset failed, remove it's asset from the tracker state
                    dependencyTracker.RemoveAsset(asset);

                    // Set the status for this asset to "Failed"
                    logger.SetAssetStatus(BuildStatus.Failed, asset);
                }
            }

            // Save the dependency tracker info
            dependencyTracker.Save();

            return buildSucceeded;
        }

        /// <summary>
        /// Internal method for building a single asset
        /// </summary>
        internal bool BuildAsset(Content content, Asset asset, string bundle, ParameterSet parameters, DependencyTracker dependencyTracker, Logger logger)
        {
            // Get the output folder for the bundle
            logger.Write(LogLevel.Info, "<hr/><b>Bundle: " + bundle + "</b><br/>", asset);
            string outputFolder = content.GetBundleBuildFolder(bundle);

            // Log the processor parameters
            logger.Write(LogLevel.Verbose, "<b><u>Parameters</u></b>", asset);
            foreach (KeyValuePair<string, string> parameter in parameters)
            {
                logger.Write(LogLevel.Verbose, parameter.Key + " = " + parameter.Value, asset);
            }
            logger.Write(LogLevel.Verbose, "", asset);

            // Create a context for this asset build
            BuildContext context = new BuildContext(content.RootPath, outputFolder, bundle, asset, parameters, dependencyTracker, logger);

            // Create an instance of the processor for this asset
            Processor processor = ProcessorManager.GetProcessor(asset.ProcessorType);

            // Setup the processor
            processor.Asset = asset;
            processor.Context = context;

            // Log the "Start"
            logger.Write(LogLevel.Info, "<b><u>Starting asset build.</u></b>", asset);

            // Process the asset
            processor.Process();

            // Log the "End"
            logger.Write(LogLevel.Info, "<b>Completed asset build.</b>", asset);
            logger.Write(LogLevel.Info, "", asset);

            // Log the build outputs
            logger.Write(LogLevel.Info, "<b><u>Build Outputs:</u></b>", asset);
            foreach (string buildOutput in context.BuildOutputs)
            {
                string fullOutputPath = Path.Combine(outputFolder, buildOutput);
                logger.Write(LogLevel.Info, "<a href=\"gdkasset:" + fullOutputPath + "\">" + bundle + "/" + buildOutput + "</a>", asset);
            }
            logger.Write(LogLevel.Info, "", asset);

            // Did we encounter any warnings or errors during the build?
            if (context.NumErrors > 0 || context.NumWarnings > 0)
                return false;
            return true;
        }
    }
}