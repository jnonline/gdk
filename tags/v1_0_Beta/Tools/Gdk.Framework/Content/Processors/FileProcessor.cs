/*
  Copyright (c) 2011, Raincity Games LLC
  Licensed under the MIT license: http://www.opensource.org/licenses/mit_license.php
*/

using System;
using System.Collections.Generic;
using System.Linq;
using System.IO;
using System.Text;
using Gdk;

namespace Gdk.Content
{
    public class FileProcessor : Processor    
    {
        /// <summary>
        /// Gets the descriptive details of this processor
        /// </summary>
        /// <returns></returns>
        public override ProcessorDetails GetDetails()
        {
            ProcessorDetails details = new ProcessorDetails(
                "Copies files from the source asset to the output folder",
                Icons.FileIcon
                );
            details.FileExtensions.Add("*");
            return details;
        }

        /// <summary>
        /// Processes the given asset
        /// </summary>
        public override void Process()
        {
            // Get the full paths to the source & destination files
            string fullSourcePath = Path.Combine(Context.ContentFolder, Asset.Path);
            string fullDestPath = Path.Combine(Context.OutputFolder, Asset.Path);

            // Make sure the destination folder exists
            Directory.CreateDirectory(Path.GetDirectoryName(fullDestPath));

            // Copy the file
            File.Copy(fullSourcePath, fullDestPath, true);

            // Track the destination file
            Context.AddOutputDependency(Asset.Path);
        }
    }
}
