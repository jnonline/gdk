using System;
using System.Collections.Generic;
using System.Linq;
using System.IO;
using System.Text;
using System.Xml;
using Gdk;

using Gdk.Forms;
using Tao.OpenGl;

namespace Gdk.Content
{

    // ===============================================================
    public class ShaderProcessor : Processor    
    {
        // Properties
        XmlElement xmlShader;
        ShaderContent shader = new ShaderContent();
        string shaderFolder;

        Dictionary<int, string> AttributeBindings = new Dictionary<int, string>();
        Dictionary<string, ShaderUniform> UniqueUniforms = new Dictionary<string,ShaderUniform>();

        /// <summary>
        /// Gets the descriptive details of this processor
        /// </summary>
        /// <returns></returns>
        public override ProcessorDetails GetDetails()
        {
            ProcessorDetails details = new ProcessorDetails(
                "Takes an input shader file (.Shader) and generates an output gdkshader file for use as a Shader",
                Icons.TextureIcon
                );
            details.FileExtensions.Add("shader");

            //details.AddParameter("Pre-Compile", "Causes the shader to be pre-compiled into binary in the resultant asset", "Shader", typeof(bool), false);
            
            return details;
        }

        /// <summary>
        /// Processes the given asset
        /// </summary>
        public override void Process()
        {
            // Load the shader file
            // --------------------------

            // Get the full path to the source file
            string fullSourcePath = Path.Combine(Context.ContentFolder, Asset.Path);
            shaderFolder = Path.GetDirectoryName(fullSourcePath);

            // Load the shader xml
            XmlDocument xmlDoc = new XmlDocument();
            xmlDoc.Load(fullSourcePath);

            // Get the root <Shader> element
            xmlShader = xmlDoc.DocumentElement;
            if (xmlShader.Name != "Shader")
            {
                throw new BuildException("Root <Shader> element not found");
            }

            // Parse the Shader XML
            // ----------------------------------------------

            Context.LogInfo("Parsing Shader XML");

            ParseShader();

            // Compile the shaders
            // ----------------------------------------------

            Context.LogInfo("Compiling Shader Programs");

            CompileShaders();

            // Verify the shader
            // ----------------------------------------------

            Context.LogInfo("Verifying Shader");

            VerifyShader();

            // Write out the binary shader
            // ----------------------------------------------

            Context.LogInfo("Writing Binary Shader");

            // Create the relative & full path to the destination .gdkshader file
            string relativeDestPath = Path.ChangeExtension(Asset.Path, ".gdkshader");
            string fullDestPath = Path.Combine(Context.OutputFolder, relativeDestPath);

            // Make sure the destination folder exists
            Directory.CreateDirectory(Path.GetDirectoryName(fullDestPath));

            // Track the destination file
            Context.AddOutputDependency(relativeDestPath);

            // Write out the binary
            WriteShader(fullDestPath, shader);
        }

        #region Parsing

        /// <summary>
        /// Parses the <Shader> XML into a ShaderContent blob
        /// </summary>
        private void ParseShader()
        {
            // Parse <RenderState> element
            Context.LogVerbose(" - Parsing RenderState");
            XmlNode xmlRenderState = xmlShader.SelectSingleNode("RenderState");
            ParseRenderState(xmlRenderState);

            // Parse the <AttributeBindings> element
            Context.LogVerbose(" - Parsing AttributeBindings");
            XmlNode xmlAttributeBindings = xmlShader.SelectSingleNode("AttributeBindings");
            ParseAttributeBindings(xmlAttributeBindings);

            // Parse the <Techniques> element
            Context.LogVerbose(" - Parsing Techniques");
            XmlNode xmlTechniques = xmlShader.SelectSingleNode("Techniques");
            ParseTechniques(xmlTechniques);

            // Parse the <Parameters> element
            Context.LogVerbose(" - Parsing Parameters");
            XmlNode xmlParameters = xmlShader.SelectSingleNode("Parameters");
            ParseParameters(xmlParameters);
        }

        /// <summary>
        /// Parses a <RenderState> node
        /// </summary>
        void ParseRenderState(XmlNode xmlRenderState)
        {
            if (xmlRenderState == null)
                return;

            // Get a quick reference to the shader's render state
            ShaderRenderState renderState = shader.RenderState;

            // ColorMask 
            // ---------------------

            XmlNode xmlColorMask = xmlRenderState.SelectSingleNode("ColorMask");
            if (xmlColorMask != null)
            {
                renderState.ColorMaskApply = true;
                renderState.ColorRedWriteEnabled = XmlUtilities.GetAttribute(xmlColorMask, "RedWriteEnabled", renderState.ColorRedWriteEnabled);
                renderState.ColorGreenWriteEnabled = XmlUtilities.GetAttribute(xmlColorMask, "GreenWriteEnabled", renderState.ColorGreenWriteEnabled);
                renderState.ColorBlueWriteEnabled = XmlUtilities.GetAttribute(xmlColorMask, "BlueWriteEnabled", renderState.ColorBlueWriteEnabled);
                renderState.ColorAlphaWriteEnabled = XmlUtilities.GetAttribute(xmlColorMask, "AlphaWriteEnabled", renderState.ColorAlphaWriteEnabled);
            }

            // Blending
            // ---------------------

            XmlNode xmlBlending = xmlRenderState.SelectSingleNode("Blending");
            if (xmlBlending != null)
            {
                renderState.BlendingApply = true;
                renderState.BlendingEnabled = XmlUtilities.GetAttribute(xmlBlending, "Enabled", renderState.BlendingEnabled);
                renderState.BlendConstantColor = XmlUtilities.GetAttribute(xmlBlending, "ConstantColor", renderState.BlendConstantColor);
                renderState.BlendMode = XmlUtilities.GetAttributeEnumValue<BlendMode>(xmlBlending, "BlendMode", renderState.BlendMode);

                renderState.SourceRGBFactor = renderState.SourceAlphaFactor = XmlUtilities.GetAttributeEnumValue<BlendFactor>(xmlBlending, "SourceFactor", renderState.SourceRGBFactor);
                renderState.DestRGBFactor = renderState.DestAlphaFactor = XmlUtilities.GetAttributeEnumValue<BlendFactor>(xmlBlending, "DestFactor", renderState.DestRGBFactor);
                renderState.RGBBlendEquation = renderState.AlphaBlendEquation = XmlUtilities.GetAttributeEnumValue<BlendEquation>(xmlBlending, "BlendEquation", renderState.RGBBlendEquation);

                renderState.SourceRGBFactor = XmlUtilities.GetAttributeEnumValue<BlendFactor>(xmlBlending, "SourceRGBFactor", renderState.SourceRGBFactor);
                renderState.DestRGBFactor = XmlUtilities.GetAttributeEnumValue<BlendFactor>(xmlBlending, "DestRGBFactor", renderState.DestRGBFactor);
                renderState.RGBBlendEquation = XmlUtilities.GetAttributeEnumValue<BlendEquation>(xmlBlending, "RGBBlendEquation", renderState.RGBBlendEquation);
                renderState.SourceAlphaFactor = XmlUtilities.GetAttributeEnumValue<BlendFactor>(xmlBlending, "SourceAlphaFactor", renderState.SourceAlphaFactor);
                renderState.DestAlphaFactor = XmlUtilities.GetAttributeEnumValue<BlendFactor>(xmlBlending, "DestAlphaFactor", renderState.DestAlphaFactor);
                renderState.AlphaBlendEquation = XmlUtilities.GetAttributeEnumValue<BlendEquation>(xmlBlending, "AlphaBlendEquation", renderState.AlphaBlendEquation);

                // If the "BlendMode" attribute existed, we use simple blending
                if (XmlUtilities.AttributeExists(xmlBlending, "BlendMode") == true)
                    renderState.UseSimpleBlendMode = true;
            }

            // Depth
            // --------------------

            XmlNode xmlDepth = xmlRenderState.SelectSingleNode("Depth");
            if (xmlDepth != null)
            {
                renderState.DepthApply = true;
                renderState.DepthWriteEnabled = XmlUtilities.GetAttribute(xmlDepth, "WriteEnabled", renderState.DepthWriteEnabled);
                renderState.DepthTestEnabled = XmlUtilities.GetAttribute(xmlDepth, "TestEnabled", renderState.DepthTestEnabled);
                renderState.DepthTestFunction = XmlUtilities.GetAttributeEnumValue<CompareFunction>(xmlBlending, "TestFunction", renderState.DepthTestFunction);
                renderState.DepthBiasEnabled = XmlUtilities.GetAttribute(xmlDepth, "BiasEnabled", renderState.DepthBiasEnabled);
                renderState.DepthBiasFactor = XmlUtilities.GetAttribute(xmlDepth, "BiasFactor", renderState.DepthBiasFactor);
                renderState.DepthBiasUnits = XmlUtilities.GetAttribute(xmlDepth, "BiasUnits", renderState.DepthBiasUnits);
            }

            // Culling
            // --------------------

            XmlNode xmlCulling = xmlRenderState.SelectSingleNode("Culling");
            if (xmlCulling != null)
            {
                renderState.CullingApply = true;
                renderState.CullingMode = XmlUtilities.GetAttributeEnumValue<CullingMode>(xmlCulling, "CullMode", renderState.CullingMode);
                renderState.CullingMode = XmlUtilities.GetAttributeEnumValue<CullingMode>(xmlCulling, "CullingMode", renderState.CullingMode);
            }
        }

        /// <summary>
        /// Parses an <AttributeBindings> node
        /// </summary>
        void ParseAttributeBindings(XmlNode xmlAttributeBindings)
        {
            if (xmlAttributeBindings == null)
            {
                throw new BuildException("No <AttributeBindings> element was found in the shader");
            }

            // Loop through the child nodes
            foreach (XmlNode xmlChildNode in xmlAttributeBindings.ChildNodes)
            {
                // Is this an <Attribute> node?
                if (xmlChildNode.Name == "Attribute")
                {
                    // Get the parameter attributes
                    string name = XmlUtilities.GetAttribute(xmlChildNode, "Name", string.Empty);
                    int location = XmlUtilities.GetAttribute(xmlChildNode, "Location", -1);

                    // Make sure the parameter has a name & a value or binding
                    if (string.IsNullOrEmpty(name))
                        throw new BuildException("Attribute binding found with no Name attribute");
                    if (location == -1)
                        throw new BuildException("Attribute binding found with no Location attribute");

                    // Make sure this location isnt already bound
                    if (this.AttributeBindings.ContainsKey(location))
                        throw new BuildException("Attribute Bindings node contains more than one binding for location " + location);

                    // Add the binding
                    this.AttributeBindings.Add(location, name);
                }
            }
        }

        /// <summary>
        /// Parses a <Techniques> nodes
        /// </summary>
        private void ParseTechniques(XmlNode xmlTechniques)
        {
            if(xmlTechniques == null)
            {
                throw new BuildException("No <Techniques> element was found in the shader");
            }

            // Loop through all the <Technique> nodes within this group
            foreach (XmlNode xmlTechnique in xmlTechniques.SelectNodes("Technique"))
            {
                // Parse this technique
                ParseTechnique(xmlTechnique);
            }

            // Did we not find any techniques?
            if (shader.Techniques.Count == 0)
                throw new BuildException("No child <Technique> nodes were found in the <Techniques> element");
        }

        /// <summary>
        /// Parses a <Technique> nodes
        /// </summary>
        private void ParseTechnique(XmlNode xmlTechnique)
        {
            // Create a new technique
            ShaderTechnique technique = new ShaderTechnique();

            // Get and Verify the technique's name
            technique.Name = XmlUtilities.GetAttribute(xmlTechnique, "Name", string.Empty);
            if (string.IsNullOrEmpty(technique.Name))
                throw new BuildException("<Technique> is missing a Name attribute");

            Context.LogInfo(" +-- Parsing Technique: " + technique.Name);

            // Make sure this isnt a duplicate name
            foreach (ShaderTechnique existingTechnique in shader.Techniques)
            {
                if (String.Compare(existingTechnique.Name, technique.Name, true) == 0)
                    throw new BuildException("A technique with this name was already parsed");
            }

            // Add this technique to the shader
            shader.Techniques.Add(technique);            

            // Parse the Supported/Excluded Platforms & Devices
            // -----------------------------------------------------

            // Loop through all the <SupportedPlatform> elements
            foreach (XmlNode xmlSupportedPlatform in xmlTechnique.SelectNodes("SupportedPlatform"))
            {
                // Get the Name of the platform & verify it's valid
                PlatformType platformType = XmlUtilities.GetAttributeEnumValue<PlatformType>(xmlSupportedPlatform, "Name", PlatformType.Unknown);
                if (platformType == PlatformType.Unknown)
                    throw new BuildException("SupportedPlatform element found with an invalid or unknown Name attribute:  " + xmlSupportedPlatform.OuterXml);

                // Add the platform to our supported list
                if(technique.SupportedPlatforms.Contains(platformType) == false)
                    technique.SupportedPlatforms.Add(platformType);
            }

            // Loop through all the <SupportedDevice> elements
            foreach (XmlNode xmlSupportedDevice in xmlTechnique.SelectNodes("SupportedDevice"))
            {
                // Get the Name of the device & verify it's valid
                DeviceType deviceType = XmlUtilities.GetAttributeEnumValue<DeviceType>(xmlSupportedDevice, "Name", DeviceType.Unknown);
                if (deviceType == DeviceType.Unknown)
                    throw new BuildException("SupportedDevice element found with an invalid or unknown Name attribute:  " + xmlSupportedDevice.OuterXml);

                // Add the device to our supported list
                if (technique.SupportedDevices.Contains(deviceType) == false)
                    technique.SupportedDevices.Add(deviceType);
            }

            // Loop through all the <ExcludedPlatform> elements
            foreach (XmlNode xmlExcludedPlatform in xmlTechnique.SelectNodes("ExcludedPlatform"))
            {
                // Get the Name of the platform & verify it's valid
                PlatformType platformType = XmlUtilities.GetAttributeEnumValue<PlatformType>(xmlExcludedPlatform, "Name", PlatformType.Unknown);
                if (platformType == PlatformType.Unknown)
                    throw new BuildException("ExcludedPlatform element found with an invalid or unknown Name attribute:  " + xmlExcludedPlatform.OuterXml);

                // Add the platform to our excluded list
                if (technique.ExcludedPlatforms.Contains(platformType) == false)
                    technique.ExcludedPlatforms.Add(platformType);
            }

            // Loop through all the <ExcludedDevice> elements
            foreach (XmlNode xmlExcludedDevice in xmlTechnique.SelectNodes("ExcludedDevice"))
            {
                // Get the Name of the device & verify it's valid
                DeviceType deviceType = XmlUtilities.GetAttributeEnumValue<DeviceType>(xmlExcludedDevice, "Name", DeviceType.Unknown);
                if (deviceType == DeviceType.Unknown)
                    throw new BuildException("ExcludedDevice element found with an invalid or unknown Name attribute:  " + xmlExcludedDevice.OuterXml);

                // Add the device to our excluded list
                if (technique.ExcludedDevices.Contains(deviceType) == false)
                    technique.ExcludedDevices.Add(deviceType);
            }

            // Parse the Vertex & Fragment shader text
            // -----------------------------------------
            
            // Verify the VertexShader & FragmentShader elements exist
            XmlNode xmlVertexShader = xmlTechnique.SelectSingleNode("VertexShader");
            XmlNode xmlFragmentShader = xmlTechnique.SelectSingleNode("FragmentShader");

            if (xmlVertexShader == null)
                throw new BuildException("The technique [" + technique.Name + "] is missing a <VertexShader> element");
            if (xmlFragmentShader == null)
                throw new BuildException("The technique [" + technique.Name + "] is missing a <FragmentShader> element");

            // Parse the vertex shader
            Context.LogVerbose(" +---- Parsing Vertex Shader");
            technique.VertexShaderText = ParseShaderProgramNode(xmlVertexShader, shaderFolder);

            // Parse the fragment shader
            Context.LogVerbose(" +---- Parsing Fragment Shader");
            technique.FragmentShaderText = ParseShaderProgramNode(xmlFragmentShader, shaderFolder);
        }

        /// <summary>
        /// Processes a VertexShader or FragmentShader node & returns the full shader text
        /// </summary>
        public string ParseShaderProgramNode(XmlNode xmlShaderProgramNode, string parentFolder)
        {
            string shaderText = null;

            // Get the initial shader text
            // ---------------------------------------

            // Is the shader a file reference
            if (XmlUtilities.AttributeExists(xmlShaderProgramNode, "File") == true)
            {
                // Get the file path
                string file = XmlUtilities.GetAttribute(xmlShaderProgramNode, "File", "");
                string fullPath = Path.Combine(parentFolder, file);

                // Verify the file exists
                if (File.Exists(fullPath) == false)
                    throw new BuildException("Cannot find the GLSL file: " + fullPath);

                // Load the file text
                shaderText = File.ReadAllText(fullPath);
            }
            else
            {
                // Does the shader contain the direct CDATA blob?
                foreach (XmlNode childNode in xmlShaderProgramNode.ChildNodes)
                {
                    if (childNode.NodeType == XmlNodeType.CDATA)
                    {
                        shaderText = ((XmlCDataSection)childNode).Value;
                        break;
                    }
                }
            }

            // Make sure we got some shader text
            if (shaderText == null)
                throw new BuildException("***Shader nodes must have either a File attribute referencing a glsl shader text file, or a CDATA blob containing the shader code");

            // Do Shader pre-processing
            // --------------------------------------------

            // Recursively process include statements
            shaderText = RecurseProcessIncludes(shaderText, parentFolder);

            // Do the define substitutions
            shaderText = ProcessSubstitutions(shaderText);

            // return the final shader text
            return shaderText;
        }

        /// <summary>
        /// Recursively processes any [#include file] statements in the text blob
        /// </summary>
        string RecurseProcessIncludes(string input, string parentFolder)
        {
            string text = input;

            // Loop while the text has #include statements
            int index = -1;
            while ((index = text.IndexOf("#include")) >= 0)
            {
                int lineEnd = index;

                // Walk forward to the end of the line
                while (lineEnd < text.Length && text[lineEnd] != '\n')
                    lineEnd++;

                // Extract the line from the input text
                string includeLine = text.Substring(index, lineEnd - index);
                text = text.Remove(index, lineEnd - index - 1);

                // Get the file name
                string relativeFilePath = includeLine.Trim().Substring(8).Trim();
                relativeFilePath = relativeFilePath.Trim('\"');

                // Load the file
                string includeFilePath = Path.Combine(parentFolder, relativeFilePath);
                if (File.Exists(includeFilePath) == false)
                    throw new BuildException("GLSL: cannot fine include file [" + relativeFilePath + "].  Full Path: " + includeFilePath);
                Context.LogVerbose(" +-- Processing include: " + includeFilePath);
                string includeFileContents = File.ReadAllText(includeFilePath);

                // Recursively process the includes
                string includeFileFolder = Path.GetDirectoryName(includeFilePath);
                RecurseProcessIncludes(includeFileContents, includeFileFolder);

                // Insert the include contents into the original text
                text = text.Insert(index, includeFileContents);
            }

            return text;
        }

        /// <summary>
        /// Greps through the input and does substituations for any #sub statements
        /// </summary>
        /// <param name="input"></param>
        /// <returns></returns>
        string ProcessSubstitutions(string input)
        {
            string text = input;

            // Loop while the text has #sub statements
            int index = -1;
            while ((index = text.IndexOf("#sub")) >= 0)
            {
                int lineEnd = index;

                // Walk forward to the end of the line
                while (lineEnd < text.Length && text[lineEnd] != '\n')
                    lineEnd++;

                // Extract the line from the input text
                string subLine = text.Substring(index, lineEnd - index).Trim();
                text = text.Remove(index, lineEnd - index);

                // Trim off the #sub command at the front
                subLine = subLine.Substring(4).Trim();

                // Get the identifier & the substitution text
                int seperatorIndex = subLine.IndexOfAny(new char[] { ' ', '\t' });
                if (subLine.Length == 0 || seperatorIndex <= 0)
                    throw new BuildException("#sub command found with no matching \"IDENTIFIER TEXT\" blob");

                // Get the identifier & substitution text
                string identifier = subLine.Substring(0, seperatorIndex).Trim();
                string subText = subLine.Substring(seperatorIndex).Trim();

                // Do the substitutions 
                text = text.Replace(identifier, subText);
            }

            return text;
        }

        /// <summary>
        /// Parses a <Parameters> node
        /// </summary>
        void ParseParameters(XmlNode xmlParameters)
        {
            if (xmlParameters == null)
            {
                Context.LogWarn("No <Parameters> element was found in the shader.. is this intentional?");
                return;
            }

            // Loop through the child nodes
            foreach (XmlNode xmlChildNode in xmlParameters.ChildNodes)
            {
                // Is this a <Parameter> node?
                if (xmlChildNode.Name == "Parameter")
                {
                    // Get the parameter attributes
                    string name = XmlUtilities.GetAttribute(xmlChildNode, "Name", string.Empty);
                    string bindToGlobal = XmlUtilities.GetAttribute(xmlChildNode, "BindToGlobal", string.Empty);
                    string value = XmlUtilities.GetAttribute(xmlChildNode, "Value", string.Empty);

                    // Make sure the parameter has a name, type, and a (value or binding)
                    if (string.IsNullOrEmpty(name))
                        throw new BuildException("Parameter found with no valid Name attribute");
                    if (string.IsNullOrEmpty(value) && string.IsNullOrEmpty(bindToGlobal))
                        throw new BuildException("Parameter [" + name + "] has no valid Value or BindToShared attribute");

                    // Do we already have this parameter?
                    if (shader.Parameters.ContainsKey(name))
                        throw new BuildException("Parameter [" + name + "] is defined more than once");

                    // Create the parameter
                    ShaderParameter parameter = new ShaderParameter(name, value, false);

                    // Is this a binding?
                    if (string.IsNullOrEmpty(bindToGlobal) == false)
                    {
                        parameter.IsBindToGlobal = true;
                        parameter.Value = bindToGlobal;
                    }

                    // Add the parameter to the shader content
                    shader.Parameters.Add(parameter.Name, parameter);
                }
            }
        }

        #endregion

        #region Compiling

        /// <summary>
        /// Compiles the shaders
        /// </summary>
        private void CompileShaders()
        {
            // Create a GL Window with a GL context
            GLForm glForm = new Gdk.Forms.GLForm();
            glForm.Visible = false;
            glForm.InitGL();

            try
            {
                // Loop through the techniques
                foreach (ShaderTechnique technique in shader.Techniques)
                {
                    Context.LogInfo(" - Compiling Technique: " + technique.Name);

                    // Compile the shaders in this techniqe
                    CompileTechnique(technique);
                }
            }
            finally
            {
                // Make sure GL is shut down
                glForm.Close();
            }
        }

        /// <summary>
        /// Compiles the shaders within the given technique
        /// </summary>
        private void CompileTechnique(ShaderTechnique technique)
        {
            int vertexShaderId = 0;
            int fragmentShaderId = 0;
            int programId = 0;

            try
            {
                // Compile & Link the shaders for this technique
                // ------------------------------------------------

                // Compile the vertex shader
                Context.LogInfo(" +-- Compiling Vertex Shader");
                vertexShaderId = CompileGLShader(technique.VertexShaderText, Gl.GL_VERTEX_SHADER);

                // Compile the fragment shader
                Context.LogInfo(" +-- Compiling Fragment Shader");
                fragmentShaderId = CompileGLShader(technique.FragmentShaderText, Gl.GL_FRAGMENT_SHADER);

                // Did we fail to compile either of the shaders?
                if (vertexShaderId == 0 || fragmentShaderId == 0)
                {
                    // Fail 
                    throw new BuildException("Failed to compile shaders");
                }

                // Create a program
                programId = Gl.glCreateProgram();

                // Attach the Vertex & Fragment shaders to the program
                Gl.glAttachShader(programId, vertexShaderId);
                Gl.glAttachShader(programId, fragmentShaderId);

                // Link the shader program
                Context.LogInfo(" +-- Linking Shader Program");
                if (LinkGLProgram(programId) == false)
                {
                    // Fail 
                    throw new BuildException("Failed to link the shader program");
                }

                Gl.glUseProgram(programId);

                // Get the shader program Attributes 
                // ---------------------------------------------------
                
                Context.LogInfo(" +-- Retrieving Attributes");

                // Get the # of attributes
                int numAttributes;
                Gl.glGetProgramiv(programId, Gl.GL_ACTIVE_ATTRIBUTES, out numAttributes);

                // Are there any attributes?
                if (numAttributes > 0)
                {
                    // Get the max size of the attribute name string
                    int maxAttributeNameLength;
                    Gl.glGetProgramiv(programId, Gl.GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, out maxAttributeNameLength);

                    // Create a string builder to hold the attribute names
                    StringBuilder nameBuilder = new StringBuilder(maxAttributeNameLength + 1);
                    
                    // Loop through the attributes
                    for (int attrIndex = 0; attrIndex < numAttributes; attrIndex++)
                    {
                        // Get the attribute name, size, and type
                        int len, size, type;
                        Gl.glGetActiveAttrib(programId, attrIndex, maxAttributeNameLength, out len, out size, out type, nameBuilder);

                        // Store this attribute in the technique info
                        string attrName = nameBuilder.ToString();
                        ShaderAttributeType attrType = (ShaderAttributeType)type;
                        technique.Attributes.Add(attrName, new ShaderAttribute(attrName, attrType, size));

                        Context.LogVerbose(" +---- Attribute [Name: " + attrName + "] [Type: " + attrType + "] [Size: " + size + "]");
                    }
                }

                // Get the shader program Uniforms
                // ---------------------------------------------------

                Context.LogInfo(" +-- Retrieving Uniforms");

                // Get the # of uniforms
                int numUniforms;
                Gl.glGetProgramiv(programId, Gl.GL_ACTIVE_UNIFORMS, out numUniforms);

                // Are there any uniforms?
                if (numUniforms > 0)
                {
                    // Get the max size of the uniform name string
                    int maxUniformNameLength;
                    Gl.glGetProgramiv(programId, Gl.GL_ACTIVE_UNIFORM_MAX_LENGTH, out maxUniformNameLength);

                    // Create a string builder to hold the uniform names
                    StringBuilder nameBuilder = new StringBuilder(maxUniformNameLength + 1);

                    // Loop through the uniforms
                    for (int uniformIndex = 0; uniformIndex < numUniforms; uniformIndex++)
                    {
                        // Get the uniform name, size, and type
                        int len, size, type;
                        Gl.glGetActiveUniform(programId, uniformIndex, maxUniformNameLength, out len, out size, out type, nameBuilder);

                        // Store this uniform in the technique info
                        string uniformName = nameBuilder.ToString();
                        ShaderUniformType uniformType = (ShaderUniformType)type;
                        technique.Uniforms.Add(uniformName, new ShaderUniform(uniformName, uniformType, size));

                        Context.LogVerbose(" +---- Uniform [Name: " + uniformName + "] [Type: " + uniformType + "] [Size: " + size + "]");
                    }
                }

                // -------------------------------
            }
            finally
            {
                // Release any GL objects we created
                if (programId != 0)
                    Gl.glDeleteProgram(programId);
                if (vertexShaderId != 0)
                    Gl.glDeleteShader(vertexShaderId);
                if (fragmentShaderId != 0)
                    Gl.glDeleteShader(fragmentShaderId);
            }
        }

        /// <summary>
        /// Returns a display friend string for a given shader type GLenum
        /// </summary>
        /// <param name="shaderType"></param>
        /// <returns></returns>
        private string ShaderTypeToString(int shaderType)
        {
            return (shaderType == Gl.GL_VERTEX_SHADER) ? "Vertex Shader" : "Fragment Shader";
        }

        /// <summary>
        /// Compiles the given shader text into a GL shader object
        /// </summary>
        private int CompileGLShader(string shaderText, int shaderType)
        {
            // Create the shader
            int shaderId = Gl.glCreateShader(shaderType);

            // Compile the shader
            string[] source = { shaderText };
            Gl.glShaderSource(shaderId, 1, source, null);
            Gl.glCompileShader(shaderId);

            // Verify the compile succeeded
            int[] status = new int[1];
            Gl.glGetShaderiv(shaderId, Gl.GL_COMPILE_STATUS, status);

            // Did the compile fail?
            if (status[0] != Gl.GL_TRUE)
            {
                // Get the length of the result log
                int[] infoLogLen = new int[1];
                Gl.glGetShaderiv(shaderId, Gl.GL_INFO_LOG_LENGTH, infoLogLen);

                // Is there a log
                if (infoLogLen[0] > 0)
                {
                    // Get the log
                    StringBuilder log = new StringBuilder(infoLogLen[0] + 1);
                    int len = 0;
                    Gl.glGetShaderInfoLog(shaderId, infoLogLen[0], out len, log);

                    // Write the log
                    string logMesage = log.ToString();
                    Context.LogError(
                        "Failed to compile " + ShaderTypeToString(shaderType) + "<br/>" +
                        StringUtilities.EncodeHtml(logMesage)
                        );
                }
                else
                {
                    // No Log
                    Context.LogError("Failed to compile " + ShaderTypeToString(shaderType) + ": Generic Error (no log)");
                }

                // Release the shader
                Gl.glDeleteShader(shaderId);
                return 0;
            }
            
            // Compile status == success
            Context.LogInfo(" +---- Successfully compiled " + ShaderTypeToString(shaderType));
            
            return shaderId;
        }

        /// <summary>
        /// Links a shader program
        /// </summary>
        private bool LinkGLProgram(int programId)
        {
            // Link
            Gl.glLinkProgram(programId);

            // Verify the link succeeded
            int[] status = new int[1];
            Gl.glGetProgramiv(programId, Gl.GL_LINK_STATUS, status);

            // Did the link fail?
            if (status[0] != Gl.GL_TRUE)
            {
                // Get the length of the result log
                int[] infoLogLen = new int[1];
                Gl.glGetProgramiv(programId, Gl.GL_INFO_LOG_LENGTH, infoLogLen);

                // Is there a log
                if (infoLogLen[0] > 0)
                {
                    // Get the log
                    StringBuilder log = new StringBuilder(infoLogLen[0] + 1);
                    int len = 0;
                    Gl.glGetProgramInfoLog(programId, infoLogLen[0], out len, log);

                    // Write the log
                    string logMesage = log.ToString();
                    Context.LogError(
                        "Failed to link the shader program.<br/>" +
                        StringUtilities.EncodeHtml(logMesage)
                        );
                }
                else
                {
                    // No Log
                    Context.LogError("Failed to link the shader program: Generic Error (no log)");
                }

                return false;
            }
            
            // Link status == success
            Context.LogInfo(" +---- Successfully linked the shader program");

            return true;
        }

        #endregion

        #region Verification

        /// <summary>
        /// Verify the final shader
        /// </summary>
        private void VerifyShader()
        {
            // Verify the Attributes
            VerifyAttributes();

            // Verify the Uniforms
            VerifyUniforms();

            // Verify the parameters
            VerifyParameters();
        }

        /// <summary>
        /// Verifies all the attributes & attribute bindings are valid across the shader
        /// </summary>
        private void VerifyAttributes()
        {
            bool failed = false;

            Context.LogInfo(" - Verifying Attributes");

            // Get the first technique
            ShaderTechnique firstTechnique = shader.Techniques[0];

            // Setup the shader's global attributes
            // -------------------------------------------------

            // Copy the attributes of the first technique, to the shader
            foreach (KeyValuePair<string, ShaderAttribute> attributeByName in firstTechnique.Attributes)
            {
                ShaderAttribute attribute = attributeByName.Value;

                // Make a copy of this attribute
                ShaderAttribute newAttribute = new ShaderAttribute(attribute.Name, attribute.AttrType, attribute.ArraySize);

                // Add this global attribute to the shader
                shader.Attributes.Add(newAttribute.Name, newAttribute);
            }

            // Verify all techniques have the same attributes as the shader
            // -------------------------------------------------

            // Loop through the attributes of the shader
            foreach (KeyValuePair<string, ShaderAttribute> globalAttributeByName in shader.Attributes)
            {
                ShaderAttribute globalAttribute = globalAttributeByName.Value;

                // Loop through all the techniques
                foreach(ShaderTechnique technique in shader.Techniques)
                {
                    // Verify this technique contains the attribute
                    if (technique.Attributes.ContainsKey(globalAttribute.Name) == false)
                    {
                        Context.LogError("Attribute [" + globalAttribute.Name + "] is NOT defined in all the techniques");
                        failed = true;
                        break; // Next attribute
                    }

                    // Verify this technique's attribute exactly matches the global attribute
                    ShaderAttribute attribute = technique.Attributes[globalAttribute.Name];
                    if (attribute.AttrType != globalAttribute.AttrType ||
                        attribute.ArraySize != globalAttribute.ArraySize)
                    {
                        Context.LogError("Attribute [" + globalAttribute.Name + "] in the technique [" + technique.Name + "] does not match the Size & Type of the first technique");
                        failed = true;
                        continue; // Next technique
                    }
                }
            }

            // Loop through all the techniques
            foreach(ShaderTechnique technique in shader.Techniques)
            {
                // Loop through the attributes of this technique
                foreach (KeyValuePair<string, ShaderAttribute> attributeByName in technique.Attributes)
                {
                    // Verify this attribute exists in the shader's global attributes
                    if (shader.Attributes.ContainsKey(attributeByName.Key) == false)
                    {
                        Context.LogError("Attribute [" + attributeByName.Key + "] in the technique [" + technique.Name + "] is NOT defined in all the techniques");
                        failed = true;
                    }
                }
            }

            // Verify attribute bindings exist for all the attributes
            // ------------------------------------------------------

            Context.LogInfo(" - Verifying Attribute Bindings");

            // Loop through the bindings
            foreach (KeyValuePair<int, string> attributeBinding in this.AttributeBindings)
            {
                // Make sure this attribute in the shader
                if(shader.Attributes.ContainsKey(attributeBinding.Value) == false)
                {
                    Context.LogError("Cannot find the attribute [" + attributeBinding.Value + "] in all of the technique vertex shaders");
                    failed = true;
                }

                // Set the location for this attribute
                ShaderAttribute globalAttribute = shader.Attributes[attributeBinding.Value];
                globalAttribute.Location = attributeBinding.Key;
            }

            // Loop through the attributes of the shader
            foreach (KeyValuePair<string, ShaderAttribute> attributeByName in shader.Attributes)
            {
                // Verify the attribute bindings contains an entry for this attribute
                if (this.AttributeBindings.ContainsValue(attributeByName.Key) == false)
                {
                    Context.LogError("The attribute [" + attributeByName.Key + "] has no matching Attribute Binding specified");
                    failed = true;
                }
            }

            // Did we fail?
            if (failed)
                throw new BuildException("One or more attribute bindings could not be verified");
        }

        /// <summary>
        /// Verifies all the uniforms are valid across all the shaders
        /// </summary>
        private void VerifyUniforms()
        {
            bool failed = false;

            // First, Lets find all UNIQUE uniforms in this shader
            // ---------------------------------------------------------

            Context.LogInfo(" - Verifying Unique Uniforms");

            // Loop through all the techniques
            foreach(ShaderTechnique technique in shader.Techniques)
            {
                // Loop through all the uniforms in this technique
                foreach(KeyValuePair<string, ShaderUniform> uniformByName in technique.Uniforms)
                {
                    ShaderUniform uniform = uniformByName.Value;

                    // Do we already have a uniform by this name?
                    if(UniqueUniforms.ContainsKey(uniform.Name) == true)
                    {
                        // Existing Uniform
                        // ------------------------

                        ShaderUniform uniqueUniform = UniqueUniforms[uniform.Name];

                        // Verify the Type of the uniforms match
                        if(uniform.UniformType != uniqueUniform.UniformType)
                        {
                            Context.LogError("The Uniform [" + uniform.Name + "] has mis-matched types across multiple Techniques");
                            failed = true;
                        }

                        // Verify the Size of the uniforms match
                        if(uniform.ArraySize != uniqueUniform.ArraySize)
                        {
                            Context.LogError("The Uniform [" + uniform.Name + "] has mis-matched array sizes across multiple Techniques");
                            failed = true;
                        }
                    }
                    else
                    {
                        // New Unique Uniform
                        // ------------------------

                        // Create a unique uniform & add it to our dictionary
                        ShaderUniform newUniqueUniform = new ShaderUniform(uniform.Name, uniform.UniformType, uniform.ArraySize);
                        UniqueUniforms.Add(newUniqueUniform.Name, newUniqueUniform);
                    }
                }
            }

            // Did we fail?
            if (failed)
                throw new BuildException("One or more uniforms could not be verified");
        }

        /// <summary>
        ///  Builds out all the shader parameters for this shader
        /// </summary>
        private void VerifyParameters()
        {
            Context.LogInfo(" - Verifying Parameters");
            bool failed = false;


            // Make sure every unique uniform has a corresponding shader parameter
            // -----------------------------------------------------------

            // Loop through all the unique uniforms
            foreach (KeyValuePair<string, ShaderUniform> uniformByName in UniqueUniforms)
            {
                ShaderUniform uniform = uniformByName.Value;

                // Do we have a shader parameter for this uniform yet?
                if (shader.Parameters.ContainsKey(uniform.Name) == false)
                {
                    // New Shader Parameter!
                    // -----------------------

                    // This is a uniform we need to build a new shader parameter for :)
                    ShaderParameter newParam = new ShaderParameter(uniform.Name, null, false);
                    newParam.ArraySize = uniform.ArraySize;
                    newParam.UniformType = uniform.UniformType;
                    shader.Parameters.Add(newParam.Name, newParam);

                    Context.LogWarn(" +-- Uniform without a matching parameter detected [" + uniform.Name + "].  (consider adding this parameter to the Shader XML)");
                }
                else
                {
                    // Existing Shader Parameter
                    // ---------------------------

                    ShaderParameter parameter = shader.Parameters[uniform.Name];

                    // Set the type & size of the parameter (which we get from the uniform)
                    parameter.ArraySize = uniform.ArraySize;
                    parameter.UniformType = uniform.UniformType;

                    // Is this NOT a binding?  (IOW: we need to parse the value)
                    if (parameter.IsBindToGlobal == false)
                    {
                        // Parse the parameter value based on the type
                        switch (parameter.UniformType)
                        {
                            case ShaderUniformType.Bool1: failed |= ParseBoolValue(parameter, 1); break;
                            case ShaderUniformType.Bool2: failed |= ParseBoolValue(parameter, 2); break;
                            case ShaderUniformType.Bool3: failed |= ParseBoolValue(parameter, 3); break;
                            case ShaderUniformType.Bool4: failed |= ParseBoolValue(parameter, 4); break;

                            case ShaderUniformType.Int1: failed |= ParseIntValue(parameter, 1); break;
                            case ShaderUniformType.Int2: failed |= ParseIntValue(parameter, 2); break;
                            case ShaderUniformType.Int3: failed |= ParseIntValue(parameter, 3); break;
                            case ShaderUniformType.Int4: failed |= ParseIntValue(parameter, 4); break;
                            case ShaderUniformType.Sampler2D: failed |= ParseIntValue(parameter, 1); break;
                            case ShaderUniformType.SamplerCube: failed |= ParseIntValue(parameter, 1); break;

                            case ShaderUniformType.Float1: failed |= ParseFloatValue(parameter, 1); break;
                            case ShaderUniformType.Float2: failed |= ParseFloatValue(parameter, 2); break;
                            case ShaderUniformType.Float3: failed |= ParseFloatValue(parameter, 3); break;
                            case ShaderUniformType.Float4: failed |= ParseFloatValue(parameter, 4); break;
                            case ShaderUniformType.Matrix2: failed |= ParseFloatValue(parameter, 4); break;
                            case ShaderUniformType.Matrix3: failed |= ParseFloatValue(parameter, 9); break;
                            case ShaderUniformType.Matrix4: failed |= ParseFloatValue(parameter, 16); break;

                            default:
                                Context.LogError("Parameter [" + parameter.Name + "] has an unknown type, cannot parse");
                                failed = true;
                                break;
                        }
                    } // is binding
                }
            }

            // Look for any shader parameters that dont have a corresponding uniform
            // ------------------------------------------------------------------------

            List<string> parametersToRemove = new List<string>();

            // Loop through the shader parameters
            foreach (KeyValuePair<string, ShaderParameter> parameter in shader.Parameters)
            {
                // Is there no uniform corresponding to the parameter?
                if (UniqueUniforms.ContainsKey(parameter.Key) == false)
                {
                    Context.LogWarn(" +-- Parameter [" + parameter.Key + "] has no matching uniforms and will be ignored.  (consider removing it from the Shader XML)");
                    parametersToRemove.Add(parameter.Key);
                }
            }

            // Remove any parameters we are ignoring
            foreach (string paramToRemove in parametersToRemove)
                shader.Parameters.Remove(paramToRemove);

            // -----------------------------------

            // Did we fail?
            if (failed)
                throw new BuildException("One or more shader parameters could not be verified");
        }

        /// <summary>
        /// Parses a vector of booleans from a shader parameter value string
        /// </summary>
        private bool ParseBoolValue(ShaderParameter parameter, int vectorSize)
        {
            // Determine the total number of values expected
            int totalElements = vectorSize * parameter.ArraySize;

            // Create a list to hold the final values
            List<int> valuesList = new List<int>();

            // Loop through each value (comma seperated) in the value string
            string[] strValues = parameter.Value.Split(',');
            foreach (string strValue in strValues)
            {
                // Convert this value to a boolean, and then an Int32
                valuesList.Add(bool.Parse(strValue.Trim()) == true ? 1 : 0);
            }

            // Make sure we got the expected number of elements
            if (totalElements != valuesList.Count)
            {
                Context.LogError("The Parameter [" + parameter.Name + "] contains an invalid number of elements for it's given array size & type.  [Expected: " + totalElements + "] [Actual: " + valuesList.Count + "]");
                return true;
            }

            // Store the parse values into the parameter
            parameter.IntValues = valuesList;

            return false;
        }

        /// <summary>
        /// Parses a vector of integers from a shader parameter value string
        /// </summary>
        private bool ParseIntValue(ShaderParameter parameter, int vectorSize)
        {
            // Determine the total number of values expected
            int totalElements = vectorSize * parameter.ArraySize;

            // Create a list to hold the final values
            List<int> valuesList = new List<int>();

            // Loop through each value (comma seperated) in the value string
            string[] strValues = parameter.Value.Split(',');
            foreach (string strValue in strValues)
            {
                // Convert this value to a boolean, and then an Int32
                valuesList.Add(int.Parse(strValue.Trim()));
            }

            // Make sure we got the expected number of elements
            if (totalElements != valuesList.Count)
            {
                Context.LogError("The Parameter [" + parameter.Name + "] contains an invalid number of elements for it's given array size & type.  [Expected: " + totalElements + "] [Actual: " + valuesList.Count + "]");
                return true;
            }

            // Store the parse values into the parameter
            parameter.IntValues = valuesList;

            return false;
        }

        /// <summary>
        /// Parses a vector of floats from a shader parameter value string
        /// </summary>
        private bool ParseFloatValue(ShaderParameter parameter, int vectorSize)
        {
            // Determine the total number of values expected
            int totalElements = vectorSize * parameter.ArraySize;

            // Create a list to hold the final values
            List<float> valuesList = new List<float>();

            // Loop through each value (comma seperated) in the value string
            string[] strValues = parameter.Value.Split(',');
            foreach (string strValue in strValues)
            {
                // Convert this value to a boolean, and then an Int32
                valuesList.Add(float.Parse(strValue.Trim()));
            }

            // Make sure we got the expected number of elements
            if (totalElements != valuesList.Count)
            {
                Context.LogError("The Parameter [" + parameter.Name + "] contains an invalid number of elements for it's given array size & type.  [Expected: " + totalElements + "] [Actual: " + valuesList.Count + "]");
                return true;
            }

            // Store the parse values into the parameter
            parameter.FloatValues = valuesList;

            return false;
        }

        #endregion

        #region Writing

        /// <summary>
        /// Writes the given shader in binary format to the given file path
        /// </summary>
        /// <param name="fullDestPath"></param>
        /// <param name="shader"></param>
        void WriteShader(string fullDestPath, ShaderContent shader)
        {
            // Create the file
            FileStream file = File.Create(fullDestPath, 2048, FileOptions.None);
            BinaryWriter writer = new BinaryWriter(file);

            // Write the shader version & flags
            writer.Write((UInt16)1);
            UInt16 shaderFlags = 0;
            shaderFlags |= (UInt16) ShaderFlags.ShaderIsText;      // Shader format: (0)Binary | (1)Text
            writer.Write(shaderFlags);

            // Render State
            // ------------------------

            UInt32 renderStateFlags = 0;

            if (shader.RenderState.ColorMaskApply)
                renderStateFlags |= (UInt32)ShaderRenderStateFlags.ColorMaskApply;
            if (shader.RenderState.BlendingApply)
                renderStateFlags |= (UInt32)ShaderRenderStateFlags.BlendingApply;
            if (shader.RenderState.DepthApply)
                renderStateFlags |= (UInt32)ShaderRenderStateFlags.DepthApply;
            if (shader.RenderState.CullingApply)
                renderStateFlags |= (UInt32)ShaderRenderStateFlags.CullingApply;

            if (shader.RenderState.ColorRedWriteEnabled)
                renderStateFlags |= (UInt32)ShaderRenderStateFlags.ColorRedWriteEnabled;
            if (shader.RenderState.ColorGreenWriteEnabled)
                renderStateFlags |= (UInt32)ShaderRenderStateFlags.ColorGreenWriteEnabled;
            if (shader.RenderState.ColorBlueWriteEnabled)
                renderStateFlags |= (UInt32)ShaderRenderStateFlags.ColorBlueWriteEnabled;
            if (shader.RenderState.ColorAlphaWriteEnabled)
                renderStateFlags |= (UInt32)ShaderRenderStateFlags.ColorAlphaWriteEnabled;

            if (shader.RenderState.BlendingEnabled)
                renderStateFlags |= (UInt32)ShaderRenderStateFlags.BlendingEnabled;
            if (shader.RenderState.UseSimpleBlendMode)
                renderStateFlags |= (UInt32)ShaderRenderStateFlags.UseSimpleBlendMode;
            if (shader.RenderState.DepthTestEnabled)
                renderStateFlags |= (UInt32)ShaderRenderStateFlags.DepthTestEnabled;
            if (shader.RenderState.DepthWriteEnabled)
                renderStateFlags |= (UInt32)ShaderRenderStateFlags.DepthWriteEnabled;
            if (shader.RenderState.DepthBiasEnabled)
                renderStateFlags |= (UInt32)ShaderRenderStateFlags.DepthBiasEnabled;

            // Write the render state flags
            writer.Write(renderStateFlags);

            // Write the Blending settings
            writer.Write(shader.RenderState.BlendConstantColor.R);
            writer.Write(shader.RenderState.BlendConstantColor.G);
            writer.Write(shader.RenderState.BlendConstantColor.B);
            writer.Write(shader.RenderState.BlendConstantColor.A);
            writer.Write((UInt16)shader.RenderState.BlendMode);

            writer.Write((UInt16)shader.RenderState.SourceRGBFactor);
            writer.Write((UInt16)shader.RenderState.DestRGBFactor);
            writer.Write((UInt16)shader.RenderState.RGBBlendEquation);
            writer.Write((UInt16)shader.RenderState.SourceAlphaFactor);
            writer.Write((UInt16)shader.RenderState.DestAlphaFactor);
            writer.Write((UInt16)shader.RenderState.AlphaBlendEquation);

            // Write the Depth settings
            writer.Write((UInt16)shader.RenderState.DepthTestFunction);
            writer.Write((float)shader.RenderState.DepthBiasFactor);
            writer.Write((float)shader.RenderState.DepthBiasUnits);

            // Write the Culling settings
            writer.Write((UInt16)shader.RenderState.CullingMode);

            // Attributes
            // -------------------------

            // Write the number of attributes
            writer.Write((UInt16) shader.Attributes.Count);

            // Loop through the attributes
            foreach(KeyValuePair<string, ShaderAttribute> attributeByName in shader.Attributes)
            {
                ShaderAttribute attribute = attributeByName.Value;

                // Write the attribute name
                BinaryWriterUtilities.WriteString(writer, attribute.Name);

                // Write the attribute type, size, and location
                writer.Write((UInt16)attribute.AttrType);
                writer.Write((UInt16)attribute.ArraySize);
                writer.Write((UInt16)attribute.Location);
            }

            // Parameters
            // -------------------------

            // Write the number of parameters
            writer.Write((UInt16)shader.Parameters.Count);

            // Loop through the parameters
            foreach (KeyValuePair<string, ShaderParameter> parameterByName in shader.Parameters)
            {
                ShaderParameter parameter = parameterByName.Value;

                // Write the parameter flags
                UInt16 parameterFlags = 0;
                if (parameter.IsBindToGlobal)
                    parameterFlags |= (UInt16)ShaderParameterFlags.BindToGlobal;
                writer.Write((UInt16)parameterFlags);

                // Write the parameter name
                BinaryWriterUtilities.WriteString(writer, parameter.Name);

                // Write the parameter size & type
                writer.Write((UInt16)parameter.UniformType);
                writer.Write((UInt16)parameter.ArraySize);
                
                // Is this a binding or a value parameter?
                if (parameter.IsBindToGlobal)
                {
                    // Write the binding
                    BinaryWriterUtilities.WriteString(writer, parameter.Value);
                }
                else
                {
                    // Write the value of the parameter
                    if (parameter.IntValues != null)
                    {
                        foreach (int value in parameter.IntValues)
                            writer.Write(value);
                    }
                    else if (parameter.FloatValues != null)
                    {
                        foreach (float value in parameter.FloatValues)
                            writer.Write(value);
                    }
                }
            }

            // Techniques
            // -------------------------

            // Write the number of techniques
            writer.Write((UInt16)shader.Techniques.Count);

            // Loop through the techniques
            foreach (ShaderTechnique technique in shader.Techniques)
            {
                // Write the supported platforms
                writer.Write((UInt16)technique.SupportedPlatforms.Count);
                foreach (PlatformType platformType in technique.SupportedPlatforms)
                    writer.Write((UInt16)platformType);

                // Write the supported devices
                writer.Write((UInt16)technique.SupportedDevices.Count);
                foreach (DeviceType deviceType in technique.SupportedDevices)
                    writer.Write((UInt16)deviceType);

                // Write the excluded platforms
                writer.Write((UInt16)technique.ExcludedPlatforms.Count);
                foreach (PlatformType platformType in technique.ExcludedPlatforms)
                    writer.Write((UInt16)platformType);

                // Write the excluded devices
                writer.Write((UInt16)technique.ExcludedDevices.Count);
                foreach (DeviceType deviceType in technique.ExcludedDevices)
                    writer.Write((UInt16)deviceType);

                // Write the technique name
                BinaryWriterUtilities.WriteString(writer, technique.Name);

                // Write the Vertex Shader
                writer.Write((UInt32)technique.VertexShaderText.Length);
                writer.Write((char[])technique.VertexShaderText.ToCharArray());

                // Write the Fragment Shader
                writer.Write((UInt32)technique.FragmentShaderText.Length);
                writer.Write((char[])technique.FragmentShaderText.ToCharArray());
            }

            // -------------------------

            // Flush & Close the stream
            file.Flush();
            file.Close();

        }

        #endregion

    } // class ShaderProcessor

} // namespace
