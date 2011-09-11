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
    public enum LogLevel
    {
        Error = 1,
        Warning = 2,
        Info = 3,
        Verbose = 4
    }

    public enum BuildStatus
    {
        Waiting,
        Building,
        Skipped,
        Success,
        SuccessWithWarning,
        Failed,
    }

    // Delegates
    public delegate void LogMessageHandler(LogLevel level, string message, Asset asset);
    public delegate void StatusUpdateHandler(BuildStatus status, Asset asset);
    
    /// <summary>
    /// Logger class
    /// </summary>
    public class Logger
    {
        /// <summary>
        /// This event is raised each time a log is written from the content builder
        /// </summary>
        public event LogMessageHandler NewLogMessage;

        /// <summary>
        /// This event is raised when the status of an asset is changed
        /// </summary>
        public event StatusUpdateHandler AssetStatusUpdated;

        /// <summary>
        /// Write a log message
        /// </summary>
        public void Write(LogLevel logLevel, string message, Asset asset)
        {
            if (NewLogMessage != null)
                NewLogMessage(logLevel, message, asset);
        }

        /// <summary>
        /// Write the full details of an exception to the log
        /// </summary>
        public void WriteException(string message, Exception ex, Asset asset)
        {
            message += System.Environment.NewLine;
            message += StringUtilities.EncodeHtml(ExceptionUtilities.GetExceptionText(ex));
            Write(LogLevel.Error, message, asset);
        }

        /// <summary>
        /// Updates the status of the given asset
        /// </summary>
        public void SetAssetStatus(BuildStatus status, Asset asset)
        {
            // Raise the status message
            if (AssetStatusUpdated != null)
                AssetStatusUpdated(status, asset);
        }
    }
}
