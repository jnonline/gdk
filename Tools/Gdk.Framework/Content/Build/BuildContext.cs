/*
  Copyright (c) 2011, Raincity Games LLC
  Licensed under the MIT license: http://www.opensource.org/licenses/mit_license.php
*/

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Gdk.Framework;

namespace Gdk.Framework.Content
{
    /// <summary>
    /// Context object that is passed to a Processor during an Asset build
    /// </summary>
    public class BuildContext
    {
		private ContentBuilder contentBuilder;
        private ParameterSet parameters;
        private string contentFolder;
        private string outputFolder;
		private Bundle bundle;
        private Asset asset;
        private DependencyTracker dependencyTracker;
        
        private int numWarnings = 0;
        private int numErrors = 0;

        /// <summary>
        /// Source folder where the content is coming from (The Asset path is relative to this folder)
        /// </summary>
        public string ContentFolder { get { return contentFolder; } }

        /// <summary>
        /// The output folder for all processed assets
        /// </summary>
        public string OutputFolder { get { return outputFolder; } }

        /// <summary>
        /// The asset being processed
        /// </summary>
        public Asset Asset { get { return asset; } }
		
		/// <summary>
        /// The bundle that contains the asset
        /// </summary>
        public Bundle Bundle { get { return bundle; } }
		
		/// <summary>
        /// The ContentBuilder that is performng the build
        /// </summary>
        public ContentBuilder ContentBuilder { get { return contentBuilder; } }
		
        /// <summary>
        /// Parameters for the processor
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
        public BuildContext(ContentBuilder contentBuilder, string contentFolder, string outputFolder, Bundle bundle, Asset asset, ParameterSet parameters, DependencyTracker dependencyTracker)
        {
			this.contentBuilder = contentBuilder;
            this.contentFolder = contentFolder;
            this.outputFolder = outputFolder;
            this.bundle = bundle;
            this.asset = asset;
            this.parameters = parameters;
            this.dependencyTracker = dependencyTracker;
        }

        #region Logging
		
		/// <summary>
		/// Generates an error during the asset processing.  The message should be plain text
		/// </summary>
        public void GenerateError(string message)
        {
            // Send the error message upstream
			contentBuilder.GenerateError(this.asset, message);
			
			// Write the error to the log
			Log("<color:#B00000>" + message + "</color>");
			
            numErrors++;
        }
		
		/// <summary>
		/// Generates a warning during the asset processing.  The message should be plain text
		/// </summary>
        public void GenerateWarning(string message)
        {
			// Send the warning message upstream
			contentBuilder.GenerateWarning(this.asset, message);
			
			// Write the warning to the log
			Log("<color:#DB8700>" + message + "</color>");
			
            numWarnings++;
        }
		
		/// <summary>
		/// Logs a general message about the asset's build process.  GTML markup is allowed 
		/// </summary>
        public void Log(string gtml)
        {
            // Send the log message upstream
			contentBuilder.LogMessage(this.asset, gtml);
        }
		
		/// <summary>
		/// This utility method writes the exception details to the asset's build log. 
		/// Optionally, the method can generate a build error for the exception.
		/// </summary>
        public void LogException(Exception ex, bool generateError)
        {
			// First write the exception details to the asset log
			Log(
		        "<color:#B00000>" +
				ExceptionUtilities.GetExceptionText(ex) +
				"</color>"
				);
			
			// Generate an error (if requested)
			if(generateError)
			{
				// Send the exception message upstream
				string error = "[" + ex.GetType().Name + "]: " + ex.Message;
				contentBuilder.GenerateError(this.asset, error);
				
				numErrors++;
			}
        }
		
		/// <summary>
		/// This utility method writes the exception details to the asset's build log.  (This does not generate an error)
		/// </summary>
		public void LogException(Exception ex)
		{
			LogException(ex, false);
		}

        #endregion

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
            dependencyTracker.AddOutputDependency(this.asset, outputFilePath);

            // Store a list of outputs for this build
            this.BuildOutputs.Add(outputFilePath);
        }

        #endregion
    }
}
