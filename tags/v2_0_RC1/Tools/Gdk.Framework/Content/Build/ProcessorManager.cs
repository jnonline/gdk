/*
  Copyright (c) 2011, Raincity Games LLC
  Licensed under the MIT license: http://www.opensource.org/licenses/mit_license.php
*/

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using Gdk.Framework;

namespace Gdk.Framework.Content
{
    public class ProcessorParameter
    {
        public string Name;
        public string Description;
        public string Category;
        public Type Type;
        public object DefaultValue;

        /// <summary>
        /// Creates a new ProcessorParameter
        /// </summary>
        public ProcessorParameter(string name, string description, Type type, object defaultValue)
            : this(name, description, "Common", type, defaultValue)
        {
        }

        /// <summary>
        /// Creates a new ProcessorParameter
        /// </summary>
        public ProcessorParameter(string name, string description, string category, Type type, object defaultValue)
        {
            this.Name = name;
            this.Description = description;
            this.Category = category;
            this.Type = type;
            this.DefaultValue = defaultValue;
        }
    }

    /// <summary>
    /// This class contains the descriptive details of a processor.  It is used heavily by the Gdk.AssetManager UI tool for processor reflection
    /// </summary>
    public class ProcessorDetails
    {
        public string Name;
        public string Description;
        public Type Type;
        public Dictionary<string, ProcessorParameter> Parameters = new Dictionary<string, ProcessorParameter>();
        public Icons Icon;
        public List<string> FileExtensions = new List<string>();

        /// <summary>
        /// Creates a new ProcessorDetails
        /// </summary>
        public ProcessorDetails(string description, Icons icon)
        {
            this.Description = description;
            this.Icon = icon;
        }

        /// <summary>
        /// Utility method for quickly adding a new parameter to this processor details
        /// </summary>
        public ProcessorParameter AddParameter(string name, string description, string category, Type type, object defaultValue)
        {
            ProcessorParameter parameter = new ProcessorParameter(name, description, category, type, defaultValue);
            Parameters.Add(name, parameter);
            return parameter;
        }

        /// <summary>
        /// Utility method for quickly adding a new parameter to this processor details
        /// </summary>
        public ProcessorParameter AddParameter(string name, string description, Type type, object defaultValue)
        {
            ProcessorParameter parameter = new ProcessorParameter(name, description, type, defaultValue);
            Parameters.Add(name, parameter);
            return parameter;
        }

        /// <summary>
        /// Utility method for quickly adding a new parameter to this processor details
        /// </summary>
        public ProcessorParameter AddParameter(string name, string description)
        {
            ProcessorParameter parameter = new ProcessorParameter(name, description, null, null);
            Parameters.Add(name, parameter);
            return parameter;
        }

        /// <summary>
        /// Override of the ToString() method
        /// </summary>
        public override string ToString()
        {
            return Name;
        }
    }
    
    /// <summary>
    /// This class contains the global registry of all processors
    /// </summary>
    public class ProcessorManager
    {
        private static Dictionary<string, ProcessorDetails> processorDetailsByName = new Dictionary<string, ProcessorDetails>();

        // Static CTor
        static ProcessorManager()
        {
            RegisterProcessor(typeof(FileProcessor));
            RegisterProcessor(typeof(Texture2DProcessor));
            RegisterProcessor(typeof(AtlasProcessor));
            RegisterProcessor(typeof(BMFontProcessor));
            RegisterProcessor(typeof(ShaderProcessor));
            RegisterProcessor(typeof(Model.ColladaModelProcessor));
        }

        /// <summary>
        /// Call this method to register a processor
        /// </summary>
        public static void RegisterProcessor(Type processorType)
        {
            // Verify we dont already have this processor registered
            if (processorDetailsByName.ContainsKey(processorType.Name) == true)
                throw new ApplicationException("Processor already registered: " + processorType.Name);

            // Create an instance of the processor
            Processor processor = Activator.CreateInstance(processorType) as Processor;

            // Get the processor details
            ProcessorDetails details = processor.GetDetails();

            // Set the name & type
            details.Name = processorType.Name;
            details.Type = processorType;

            // Store this registered processor in our dictionary
            processorDetailsByName[details.Name] = details;
        }

        /// <summary>
        /// Gets an instance of the given processor
        /// </summary>
        public static Processor GetProcessor(string processorName)
        {
            // Verify the processor type is valid
            if (processorDetailsByName.ContainsKey(processorName) == false)
            {
                throw new BuildException("Unknown processor: " + processorName);
            }

            return Activator.CreateInstance(processorDetailsByName[processorName].Type) as Processor;
        }

        /// <summary>
        /// Gets an instance of the given processor
        /// </summary>
        public static ProcessorDetails GetProcessorDetails(string processorName)
        {
            // Verify the processor type is valid
            if (processorDetailsByName.ContainsKey(processorName) == false)
            {
                throw new BuildException("Unknown processor: " + processorName);
            }

            return processorDetailsByName[processorName];
        }

        /// <summary>
        /// Gets all the processors that support the given file type
        /// </summary>
        public static List<ProcessorDetails> GetProcessorsByFileExtension(string fileExtension)
        {
            List<ProcessorDetails> results = new List<ProcessorDetails>();

            // Loop through all the processors
            foreach (KeyValuePair<string, ProcessorDetails> pair in processorDetailsByName)
            {
                // Loop through the extensions this processor supports
                foreach(string supportedExtension in pair.Value.FileExtensions)
                {
                    string regExPattern = supportedExtension
                        .Replace("*", ".*")
                        .Replace("?", ".?")
                        ;

                    // Does this processor support the given file extension?
                    if (Regex.IsMatch(fileExtension, regExPattern, RegexOptions.IgnoreCase))
                    {
                        // If the supported extension has a wildcard, add it to the end of the list
                        if(supportedExtension.Contains('*') || supportedExtension.Contains('?'))
                            results.Add(pair.Value);
                        else // Exact matches go first :)
                            results.Insert(0, pair.Value);
                        break;
                    }
                }
            }

            return results;
        }
    }
}
