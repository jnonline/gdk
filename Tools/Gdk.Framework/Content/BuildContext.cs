/*
  Copyright (c) 2011, Raincity Games LLC
  Licensed under the MIT license: http://www.opensource.org/licenses/mit_license.php
*/

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Gdk;

namespace Gdk.Content
{
    /// <summary>
    /// Context object that is passed to a Processor during an Asset build
    /// </summary>
    public class BuildContext
    {
        private string bundle;
        private ParameterSet parameters;
        private string contentFolder;
        private string outputFolder;
        private Asset asset;
        private DependencyTracker dependencyTracker;
        private Logger logger;
        private int numWarnings = 0;
        private int numErrors = 0;

        /// <summary>
        /// Accessor for the output folder property
        /// </summary>
        public string ContentFolder { get { return contentFolder; } }

        /// <summary>
        /// Accessor for the output folder property
        /// </summary>
        public string OutputFolder { get { return outputFolder; } }

        /// <summary>
        /// Accessor for the asset property
        /// </summary>
        public Asset Asset { get { return asset; } }

        /// <summary>
        /// Accessor for the asset property
        /// </summary>
        public ParameterSet Parameters { get { return parameters; } }

        /// <summary>
        /// Gets the number of warnings that were logged during the asset build
        /// </summary>
        public int NumWarnings { get { return numWarnings; } }

        /// <summary>
        /// Gets the number of errors that were logged during the asset build
        /// </summary>
        public int NumErrors { get { return numErrors; } }

        /// <summary>
        /// Returns a list of all the files that were output during an asset build
        /// </summary>
        public List<String> BuildOutputs = new List<string>();

        /// <summary>
        /// Constructor
        /// </summary>
        public BuildContext(string contentFolder, string outputFolder, string bundle, Asset asset, ParameterSet parameters, DependencyTracker dependencyTracker, Logger logger)
        {
            this.contentFolder = contentFolder;
            this.outputFolder = outputFolder;
            this.bundle = bundle;
            this.asset = asset;
            this.parameters = parameters;
            this.dependencyTracker = dependencyTracker;
            this.logger = logger;
        }

        #region Logging

        public void LogError(string message)
        {
            logger.Write(LogLevel.Error, message, this.asset);
            numErrors++;
        }

        public void LogWarn(string message)
        {
            logger.Write(LogLevel.Warning, message, this.asset);
            numWarnings++;
        }

        public void LogInfo(string message)
        {
            logger.Write(LogLevel.Info, message, this.asset);
        }

        public void LogVerbose(string message)
        {
            logger.Write(LogLevel.Verbose, message, this.asset);
        }

        public void LogException(Exception ex, string message)
        {
            logger.WriteException(message, ex, this.asset);
        }

        #endregion // Logging

        #region Dependency Tracking

        /// <summary>
        /// Adds a dependency on an input file.  (Relative to the content root folder)
        /// </summary>
        /// <param name="inputFilePath"></param>
        public void AddInputDependency(string inputFilePath)
        {
            dependencyTracker.AddInputDependency(this.asset, inputFilePath);
        }

        /// <summary>
        /// Adds a dependency on an output file.  (Relative to the content destination folder)
        /// </summary>
        public void AddOutputDependency(string outputFilePath)
        {
            // Add this output to the dependency tracker
            dependencyTracker.AddOutputDependency(this.asset, System.IO.Path.Combine(this.bundle, outputFilePath));

            // Store a list of outputs for this build
            this.BuildOutputs.Add(outputFilePath);
        }

        #endregion
    }
}
