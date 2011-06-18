using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;

using Gdk;
using Gdk.Content;

namespace Gdk.BuildContent
{
    class Program
    {
        static object lockObj = new object();
        static int errorCount = 0;
        static int warningCount = 0;

        static void Main(string[] args)
        {
            // Parse the command line arguments
            // --------------------------------------

            // Make sure there are enough args
            if (args.Length < 2)
            {
                Console.WriteLine("USAGE:  BuildContent.exe  <SourceContent.xml>  <C:\\OutputFolder> [Param:Value]");
                return;
            }

            // Get the required args
            string contentFile = args[0];
            string outputFolder = args[1];

            // Check the input file exists
            if(File.Exists(contentFile) == false)
            {
                Console.WriteLine("ERROR: Unable to find the source content file: " + contentFile);
                return;
            }

            // Process any other arguments as custom parameters
            Parameters parameters = new Parameters();
            for (int index = 2; index < args.Length; index++)
            {
                string arg = args[index];
                int splitIndex = arg.IndexOf(':');
                if (splitIndex >= 0)
                {
                    string paramName = arg.Substring(0, splitIndex);
                    string paramValue = arg.Substring(splitIndex + 1);
                    parameters[paramName] = paramValue;
                }
            }

            // If there is no Platform defined, default to WIN
            if (parameters.ContainsKey("Platform") == false)
            {
                parameters["Platform"] = "WIN";
                Console.WriteLine("No platform specified, defaulting to: [" + parameters["Platform"] + "]");
            }
            else
            {
                Console.WriteLine("Building platform: [" + parameters["Platform"] + "]");
            }

            // Ensure the destination folder exists
            Directory.CreateDirectory(outputFolder);

            // Build the content
            // -------------------------------------

            Console.WriteLine("Build Starting");
            Console.WriteLine("--------------------------------------------");

            try
            {
                // Load the content file
                Assets assets = Assets.FromFile(contentFile);
            
                // Create a Logger to track the build
                Logger logger = new Logger();
                logger.NewLogMessage += new LogMessageHandler(logger_NewLogMessage);

                // Do the build
                Builder builder = new Builder();
                bool success = builder.Build(outputFolder, assets, parameters, logger, false);

                // Done!
                Console.WriteLine("--------------------------------------------");
                if (success)
                    Console.WriteLine("Build Successfull");
                else
                    Console.WriteLine("Build Failed (Error: " + errorCount + ")");

            }
            catch (Exception ex)
            {
                Console.Write(ExceptionUtilities.GetExceptionText(ex));
                return;
            }
        }

        /// <summary>
        /// This method is called every time a log is written
        /// </summary>
        static void logger_NewLogMessage(LogLevel level, string message, Asset asset)
        {
            lock(lockObj)
            {
                // Prepend the asset tag to the log
                string text = "[" + asset.Path + "]: ";

                // Count Errors & Warnings
                if(level == LogLevel.Error)
                {
                    errorCount++;
                    text = "[ERROR]" + text;
                }
                else if(level == LogLevel.Warning)
                {
                    warningCount++;
                    text = "[WARNING]" + text;
                }

                // Add the message to the log
                text += message;

                // Write the line
                Console.WriteLine(text);
            }
        }
    }
}
