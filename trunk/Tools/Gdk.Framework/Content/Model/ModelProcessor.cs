using System;
using System.Collections.Generic;
using System.Linq;
using System.IO;
using System.Text;
using System.Xml;
using Gdk;

namespace Gdk.Content
{
    
    // ===============================================================
    public class ModelProcessor : Processor
    {
        XmlNode ColladaXml;
        string ColladaSourceFile;
        string ColladaSourceFolder;
        Matrix MasterTransform;

        List<string> MaterialsToParse = new List<string>();

        // High-level parsed objects
        List<ModelMeshContent> ModelMeshes = new List<ModelMeshContent>();
        Dictionary<string, ModelMaterialContent> MaterialsByName = new Dictionary<string, ModelMaterialContent>();
        

        /// <summary>
        /// Gets the descriptive details of this processor
        /// </summary>
        /// <returns></returns>
        public override ProcessorDetails GetDetails()
        {
            ProcessorDetails details = new ProcessorDetails(
                "Processes all the mesh data from a single COLLADA file into a GdkModel output",
                Icons.ModelIcon
                );
            details.FileExtensions.Add("dae");

            // Add the processor parameters
            details.AddParameter("Diffuse PixelFormat", "Destination pixel format for the diffuse texture", "Textures", typeof(PixelFormats), PixelFormats.RGBA_5551);

            details.AddParameter("Scale", "Scales the mesh by a given factor", "Transform", typeof(float), 1.0f);
            details.AddParameter("Rotate", "Rotates the mesh by the given vector (per axis, in degrees)", "Transform", typeof(Vector3), Vector3.Zero);
            details.AddParameter("Pre-Translate", "Translates the mesh by a given vector (before scaling & rotating)", "Transform", typeof(Vector3), Vector3.Zero);
            details.AddParameter("Post-Translate", "Translates the mesh by a given vector (after scaling & rotating)", "Transform", typeof(Vector3), Vector3.Zero);
            
            return details;
        }

        /// <summary>
        /// Processes the given asset
        /// </summary>
        public override void Process()
        {
            // Open & Validate the source collada
            // -------------------------------------

            // Get the full path to the source file
            ColladaSourceFile = Path.Combine(Context.ContentFolder, Asset.Path);
            ColladaSourceFolder = Path.GetDirectoryName(ColladaSourceFile);

            // Read in the collada source file
            string xml = System.IO.File.ReadAllText(ColladaSourceFile);

            // Rip the collada xmlns out of the xml
            xml = xml.Replace("xmlns=\"http://www.collada.org/2005/11/COLLADASchema\"", "");

            // Load the collada xml
            XmlDocument xmlDoc = new XmlDocument();
            xmlDoc.LoadXml(xml);

            // Get the root <COLLADA> element
            ColladaXml = xmlDoc.DocumentElement;
            if (ColladaXml.Name != "COLLADA")
            {
                throw new BuildException("Root <COLLADA> element not found");
            }

            // Parse the collada data
            // ------------------------------

            // Parse the Meshes
            // ------------------------------

            Context.LogInfo("==== Parsing Collada Data ====");

            ParseCollada();

            // Process any dependent assets.  (IE: textures)
            // -----------------------------

            Context.LogInfo("==== Processing Dependent Assets ====");

            ProcessDependencies();

            // Write the binary model
            // --------------------------

            Context.LogInfo("==== Generating Model Asset ====");

            WriteModel();
        }

        /// <summary>
        /// Writes out the binary model data
        /// </summary>
        public void WriteModel()
        {
            // Create the relative & full path to the destination .gdkmodel file
            string relativeDestPath = Path.ChangeExtension(Asset.Path, ".gdkmodel");
            string fullDestPath = Path.Combine(Context.OutputFolder, relativeDestPath);

            // Make sure the destination folder exists
            Directory.CreateDirectory(Path.GetDirectoryName(fullDestPath));

            // Track the destination file
            Context.AddOutputDependency(relativeDestPath);

            // Header
            // -----------------------

            // Create the model file
            FileStream file = File.Create(fullDestPath, 2048, FileOptions.None);
            BinaryWriter writer = new BinaryWriter(file);

            // Version & Flags
            UInt16 version = 0;
            UInt16 headerFlags = 0;
            writer.Write(version);
            writer.Write(headerFlags);

            // Sub-object counts
            writer.Write((UInt16) this.MaterialsByName.Count);
            writer.Write((UInt16) this.ModelMeshes.Count);

            // Materials
            // -----------------------
            
            // Loop through the materials
            foreach (ModelMaterialContent material in this.MaterialsByName.Values)
            {
                // Write out the material
                writer.Write((UInt16)material.Flags);
                WriteColor(writer, material.Emissive);
                WriteColor(writer, material.Ambient);
                WriteColor(writer, material.Diffuse);
                WriteColor(writer, material.Specular);
                writer.Write((float)material.Shininess);
                
                if ((material.Flags & ModelMaterialFlags.DiffuseTextured) > 0)
                    StringUtilities.WriteString(writer, material.DiffuseTexture);
                if ((material.Flags & ModelMaterialFlags.BumpTextured) > 0)
                    StringUtilities.WriteString(writer, material.BumpTexture);
            }

            // Meshes
            // -----------------------
            
            // Loop through the meshes
            foreach(ModelMeshContent mesh in this.ModelMeshes)
            {
                // Build the mesh flags
                UInt16 meshFlags = 0;

                // Check for huge meshes..
                if (mesh.Vertices.Count > UInt16.MaxValue)
                    throw new BuildException("Mesh contains too many vertices:  " + mesh.Indices.Count);
                if (mesh.Indices.Count > UInt16.MaxValue)
                    throw new BuildException("Mesh contains too many indices:  " + mesh.Indices.Count);

                // Write out the mesh header
                writer.Write((UInt16)meshFlags);
                writer.Write((UInt16)mesh.Vertices.Count);
                writer.Write((UInt16)mesh.Indices.Count);
                writer.Write((UInt16)mesh.MeshParts.Count);

                // Write the bounding sphere
                WriteVector3(writer, mesh.BoundingSphere.Center);
                writer.Write(mesh.BoundingSphere.Radius);

                // Write the vertex data
                foreach(ModelMeshVertex vertex in mesh.Vertices)
                {
                    WriteVector3(writer, vertex.Position);
                    WriteVector3(writer, vertex.Normal);
                    WriteVector2(writer, vertex.TexCoord);
                }

                // Write the indices
                foreach (int index in mesh.Indices)
                {
                    writer.Write((UInt16)index);
                }

                // Loop through the mesh parts
                foreach (ModelMeshPartContent meshPart in mesh.MeshParts)
                {
                    writer.Write((UInt16)meshPart.IndexStart);
                    writer.Write((UInt16)meshPart.IndexCount);

                    // Lookup the material & write its index
                    ModelMaterialContent material = this.MaterialsByName[meshPart.MaterialName];
                    writer.Write((UInt16)material.Index);
                }
            }

            // Finished
            // -----------------------

            writer.Flush();
            writer.Close();
        }

        /// <summary>
        /// Writes a Vector3 to the given BinaryWriter
        /// </summary>
        public void WriteVector3(BinaryWriter writer, Vector3 v)
        {
            writer.Write(v.X);
            writer.Write(v.Y);
            writer.Write(v.Z);
        }

        /// <summary>
        /// Writes a Vector2 to the given BinaryWriter
        /// </summary>
        public void WriteVector2(BinaryWriter writer, Vector2 v)
        {
            writer.Write(v.X);
            writer.Write(v.Y);;
        }

        /// <summary>
        /// Writes a Color to the given BinaryWriter
        /// </summary>
        public void WriteColor(BinaryWriter writer, Color color)
        {
            writer.Write(color.R);
            writer.Write(color.G);
            writer.Write(color.B);
            writer.Write(color.A);
        }

        /// <summary>
        /// Processes any dependent assets, such as referenced materials & textures
        /// </summary>
        public void ProcessDependencies()
        {
            // Get the pixel format processor settings
            PixelFormats diffusePixelFormat = Context.Parameters.GetEnumValue<PixelFormats>("Diffuse PixelFormat", PixelFormats.RGBA_5551);

            // Loop through the materials
            foreach (KeyValuePair<string, ModelMaterialContent> entry in this.MaterialsByName)
            {
                ModelMaterialContent material = entry.Value;

                // If this material has a diffuse texture, then process it
                if (string.IsNullOrEmpty(material.DiffuseTexture) == false)
                {
                    ProcessDependentTexture(ref material.DiffuseTexture, diffusePixelFormat);
                }
            }
        }

        /// <summary>
        /// Processes a texture file
        /// </summary>
        public void ProcessDependentTexture(ref string textureRelPath, PixelFormats pixelFormat)
        {
            // Build the relative & full path to the output gdkimage
            string outputRelPath = Path.Combine(Path.GetDirectoryName(Asset.Path), textureRelPath);
            outputRelPath = Path.ChangeExtension(outputRelPath, "gdkimage");
            string outputFullPath = Path.Combine(Context.OutputFolder, outputRelPath);

            // Add the dependency on the input texture file
            string textureFullPath = Path.Combine(ColladaSourceFolder, textureRelPath);
            string texturePathRelativeToContent = textureFullPath.Substring(Context.ContentFolder.Length + 1);
            Context.AddInputDependency(texturePathRelativeToContent);

            // Add the dependency to this output gdkimage file
            Context.AddOutputDependency(outputRelPath);

            // Load the texture & save it back out as a gdkimage
            Surface texture = Surface.FromFile(textureFullPath);
            Directory.CreateDirectory(Path.GetDirectoryName(outputFullPath));
            texture.SaveToGdkImage(outputFullPath, pixelFormat);

            // Remove the file extension from the original path
            int index = textureRelPath.LastIndexOf('.');
            if (index > 0)
                textureRelPath = textureRelPath.Substring(0, index); 
        }

        /// <summary>
        /// Parses the root collada data from the given collada source file
        /// </summary>
        public void ParseCollada()
        {   
            // Get the "Transform" parameters
            float transformScale = Context.Parameters.GetValue("Scale", 1.0f);
            Vector3 transformRotate = Context.Parameters.GetValue("Rotate", Vector3.Zero);
            Vector3 transformPreTranslate = Context.Parameters.GetValue("Pre-Translate", Vector3.Zero);
            Vector3 transformPostTranslate = Context.Parameters.GetValue("Post-Translate", Vector3.Zero);

            // Setup a transform matrix for the processor "Transform" parameters
            MasterTransform =
                Matrix.CreateTranslation(transformPreTranslate)
                * Matrix.CreateRotationY(MathUtility.DegreesToRadians(transformRotate.Y))
                * Matrix.CreateRotationY(MathUtility.DegreesToRadians(transformRotate.Z))
                * Matrix.CreateRotationY(MathUtility.DegreesToRadians(transformRotate.X))
                * Matrix.CreateScale(transformScale)
                * Matrix.CreateTranslation(transformPostTranslate)
                ;


            // Parse the Meshes
            // ------------------------------

            Context.LogInfo("Processing Meshes");

            // Loop through the scenes
            foreach (XmlNode xmlVisualScene in ColladaXml.SelectNodes("library_visual_scenes/visual_scene"))
            {
                // Loop through the nodes in the scene
                foreach (XmlNode xmlNode in xmlVisualScene.SelectNodes("node"))
                {
                    // Loop through the instanced geometries in this node
                    foreach (XmlNode xmlInstanceGeometry in xmlNode.SelectNodes("instance_geometry"))
                    {
                        // Get the geometry URL
                        if (XmlUtilities.AttributeExists(xmlInstanceGeometry, "url") == true)
                        {
                            string instanceGeometryUrl = xmlInstanceGeometry.Attributes["url"].Value;

                            // Parse this mesh
                            ParseModelMesh(instanceGeometryUrl);
                        }
                    }
                }
            }

            // Parse the Materials we need
            // ------------------------------

            Context.LogInfo("Processing Materials");

            // Loop through the materials we need to parse
            foreach(string materialName in this.MaterialsToParse)
            {
                // Parse this material
                ParseMaterial(materialName);
            }
        }

        /// <summary>
        /// Parses a material
        /// </summary>
        public void ParseMaterial(string materialName)
        {
            // Create the material
            ModelMaterialContent material = new ModelMaterialContent();
            material.Name = materialName;

            // Add the material to the dictionary
            material.Index = MaterialsByName.Count;
            MaterialsByName.Add(material.Name, material);

            // Log 
            Context.LogInfo(" - Material: " + materialName);

            /*
            <library_materials>
                <material id="TopMaterial" name="TopMaterial">
                    <instance_effect url="#TopMaterial-effect" />
             */

            // Get the instance_effect element of the material
            XmlNode xmlInstanceEffect = ColladaXml.SelectSingleNode("library_materials/material[@id='" + materialName + "']/instance_effect");
            if (xmlInstanceEffect == null)
                throw new BuildException("Unable to find the material or instance_effect elements for the material: " + material);
            string effectUrl = xmlInstanceEffect.Attributes["url"].Value.TrimStart('#');

            /*
            <library_effects>
                <effect id="TopMaterial-effect" name="TopMaterial-effect">
                    <profile_COMMON>
             */
             
            // Get the effect
            XmlNode xmlEffectProfile = ColladaXml.SelectSingleNode("library_effects/effect[@id='" + effectUrl + "']/profile_COMMON");
            if (xmlEffectProfile == null)
                throw new BuildException("Unable to find the effect or its profile_COMMON for the effect: " + effectUrl);

            /*
            <technique sid="common">
                <phong>
                    <emission>
                        <color>0.000000 0.000000 0.000000 1.000000</color>
                    </emission>
                    <ambient>
                        <color>0.290196 0.109804 0.113725 1.000000</color>
                    </ambient>
                    <diffuse>
                        <texture texture="TopMaterial-image-sampler" texcoord="" />
                    </diffuse>
                    <specular>
                        <color>0.000000 0.000000 0.000000 1.000000</color>
                    </specular>
                    <shininess>
                        <float>0.000000</float>
                    </shininess>
            */

            // Get the technique
            XmlNode xmlTechniquePhong = xmlEffectProfile.SelectSingleNode("technique/phong");

            // Parse the emissive
            // --------------------
            XmlNode xmlEmissionColor = xmlTechniquePhong.SelectSingleNode("emission/color");
            if(xmlEmissionColor != null)
            {
                material.Emissive = ParseColor(xmlEmissionColor.InnerText);
                Context.LogVerbose(" +-- Emissive: " + material.Emissive.ToString());
            }
            else
                Context.LogVerbose(" +-- Emissive: [default] " + material.Emissive.ToString());

            // Parse the ambient
            // --------------------
            XmlNode xmlAmbientColor = xmlTechniquePhong.SelectSingleNode("ambient/color");
            if(xmlAmbientColor != null)
            {
                material.Ambient = ParseColor(xmlAmbientColor.InnerText);
                Context.LogVerbose(" +-- Ambient: " + material.Ambient.ToString());
            }
            else
                Context.LogVerbose(" +-- Ambient: [default] " + material.Ambient.ToString());

            // Parse the diffuse
            // --------------------
            XmlNode xmlDiffuseColor = xmlTechniquePhong.SelectSingleNode("diffuse/color");
            XmlNode xmlDiffuseTexture = xmlTechniquePhong.SelectSingleNode("diffuse/texture");
            if(xmlDiffuseTexture != null)
            {
                material.Flags |= ModelMaterialFlags.DiffuseTextured;
                string samplerSid = xmlDiffuseTexture.Attributes["texture"].Value;
                material.DiffuseTexture = LookupTextureFromSampler(xmlEffectProfile, samplerSid);
                material.Diffuse = Color.White;
                Context.LogVerbose(" +-- Diffuse: [Texture]: " + material.DiffuseTexture);
            }
            else if(xmlDiffuseColor != null)
            {
                material.Diffuse = ParseColor(xmlDiffuseColor.InnerText);
                Context.LogVerbose(" +-- Diffuse: [Color] " + material.Diffuse.ToString());
            }
            else
                Context.LogVerbose(" +-- Diffuse: [Color] [default] " + material.Diffuse.ToString());

            // Parse the specular
            // --------------------
            XmlNode xmlSpecularColor = xmlTechniquePhong.SelectSingleNode("specular/color");
            if (xmlSpecularColor != null)
            {
                material.Specular = ParseColor(xmlSpecularColor.InnerText);
                Context.LogVerbose(" +-- Specular: " + material.Specular.ToString());
            }
            else
                Context.LogVerbose(" +-- Specular: [default] " + material.Specular.ToString());

            // Parse the shininess
            // --------------------
            XmlNode xmlShininess = xmlTechniquePhong.SelectSingleNode("shininess/float");
            if (xmlShininess != null)
            {
                material.Shininess = float.Parse(xmlShininess.InnerText);
                Context.LogVerbose(" +-- Shininess: " + material.Shininess.ToString());
            }
            else
                Context.LogVerbose(" +-- Shininess: [default] " + material.Shininess.ToString());

            // TODO: Bump mapping??
        }

        /// <summary>
        /// Parses a Color value from an input string.
        /// </summary>
        /// <param name="input">Color values in the format (R G B [A]) (0-1)  </param>
        /// <returns></returns>
        public Color ParseColor(string input)
        {
            Color result = Color.Black;

            // Split up the string by commas
            string[] parts = input.Split(' ');
            if (parts.GetUpperBound(0) < 2)
                throw new ArgumentException("Input string is not a valid color value: " + input, "input");
            
            result.R = (byte)(float.Parse(parts[0].Trim()) * 255.0f);
            result.G = (byte)(float.Parse(parts[1].Trim()) * 255.0f);
            result.B = (byte)(float.Parse(parts[2].Trim()) * 255.0f);
            
            if (parts.GetUpperBound(0) >= 3)
                result.A = (byte)(float.Parse(parts[3].Trim()) * 255.0f);
            return result;
        }

        /// <summary>
        /// Parses a mesh
        /// </summary>
        /// <param name="meshUrl"></param>
        public void ParseModelMesh(string meshUrl)
        {
            // Find the mesh with this URL
            string meshId = meshUrl.TrimStart('#');

            XmlNode xmlMesh = ColladaXml.SelectSingleNode("library_geometries/geometry[@id='" + meshId + "']/mesh");
            if (xmlMesh == null)
                throw new BuildException("Unable to find Mesh [ID=" + meshId + "] in the collada xml");

            Context.LogInfo(" - Processing Mesh: " + meshId);

            // Create the resultant mesh content object
            ModelMeshContent meshContent = new ModelMeshContent();

            // Parse the mesh vertex data streams & semantics
            Dictionary<string, ColladaFloatDataStream> streamsBySemantic = ParseSemanticStreams(xmlMesh);

            // Verify the mesh contains all the proper stream data
            // -----------------------------------------------

            // Verify the Position[3] semantic
            if (streamsBySemantic.ContainsKey("POSITION") == false)
                throw new BuildException("Mesh does not contain a POSITION semantic");
            ColladaFloatDataStream positionStream = streamsBySemantic["POSITION"];
            if (positionStream.Stride < 3)
                throw new BuildException("Mesh does not contain a Stride=3 POSITION semantic");

            // Verify the Normal[3] semantic
            if (streamsBySemantic.ContainsKey("NORMAL") == false)
                throw new BuildException("Mesh does not contain a NORMAL semantic");
            ColladaFloatDataStream normalStream = streamsBySemantic["NORMAL"];
            if (normalStream.Stride < 3)
                throw new BuildException("Mesh does not contain a Stride=3 NORMAL semantic");

            // Verify the TexCoord[2] semantic
            if (streamsBySemantic.ContainsKey("TEXCOORD") == false)
                throw new BuildException("Mesh does not contain a TEXCOORD semantic");
            ColladaFloatDataStream texCoordStream = streamsBySemantic["TEXCOORD"];
            if (texCoordStream.Stride < 2)
                throw new BuildException("Mesh does not contain a Stride=2 TEXCOORD semantic");

            // Parse the mesh parts
            // --------------------------------

            // Loop through the mesh parts in the mesh
            foreach (XmlNode xmlTriangles in xmlMesh.SelectNodes("triangles"))
            {
                // Create a mesh part for this triangles set
                ModelMeshPartContent meshPart = new ModelMeshPartContent();
                meshContent.MeshParts.Add(meshPart);

                // Set the start values for the mesh indices
                meshPart.IndexStart = meshContent.Indices.Count;

                // Get the material used by this mesh part
                string materialName = xmlTriangles.Attributes["material"].Value;
                meshPart.MaterialName = materialName;

                // Add this material to the list of materials we should parse
                if (MaterialsToParse.Contains(materialName) == false)
                    MaterialsToParse.Add(materialName);

                // Get the mesh part settings
                int triangleCount = int.Parse(xmlTriangles.Attributes["count"].Value);
                Context.LogVerbose(" +-- Processing Mesh Part");
                Context.LogVerbose(" +---- Material: " + meshPart.MaterialName);
                Context.LogVerbose(" +---- Faces: " + triangleCount);

                // Get the # of input semantics for this mesh part
                int numInputsPerVertex = xmlTriangles.SelectNodes("input").Count;

                // Get the offsets of the POSITION, NORMAL, and TEXCOORD semantics
                int positionOffset = XmlUtilities.GetAttribute(xmlTriangles.SelectSingleNode("input[@semantic='POSITION' or @semantic='VERTEX']"), "offset", 0);
                int normalOffset = XmlUtilities.GetAttribute(xmlTriangles.SelectSingleNode("input[@semantic='NORMAL']"), "offset", 1);
                int texCoordOffset = XmlUtilities.GetAttribute(xmlTriangles.SelectSingleNode("input[@semantic='TEXCOORD']"), "offset", 2);

                // Parse the vertex stream indices into an array
                int[] vertexStreamIndices = new int[triangleCount * 3 * numInputsPerVertex];
                string[] vertexStreamIndicesStrs = xmlTriangles.SelectSingleNode("p").InnerText.Split(' ');
                if (vertexStreamIndicesStrs.Length != vertexStreamIndices.Length)
                    throw new BuildException("triangles element contains an invalid number of vertex indices. [Expected=" + vertexStreamIndices.Length + "] [Actual=" + vertexStreamIndicesStrs.Length + "]");
                for (int index = 0; index < vertexStreamIndices.Length; index++)
                    vertexStreamIndices[index] = int.Parse(vertexStreamIndicesStrs[index]);

                // Determine if we need to build the normals from scratch
                bool calculateNormals = false;
                if (normalStream.Values.Length < 3 || (normalStream.Values[0] == 0.0f && normalStream.Values[1] == 0.0f && normalStream.Values[2] == 0.0f))
                {
                    calculateNormals = true;
                    Context.LogVerbose(" +---- Normals are missing or invalid, will auto-calculate");

                }

                // Loop through the triangles
                for (int triIndex = 0; triIndex < triangleCount; triIndex++)
                {
                    // 3 vertices per triangle
                    for (int vertIndex = 0; vertIndex < 3; vertIndex++)
                    {
                        int positionIndex = vertexStreamIndices[((triIndex * 3) + vertIndex) * numInputsPerVertex + positionOffset];
                        int normalIndex = vertexStreamIndices[((triIndex * 3) + vertIndex) * numInputsPerVertex + normalOffset];
                        int texCoordIndex = vertexStreamIndices[((triIndex * 3) + vertIndex) * numInputsPerVertex + texCoordOffset];

                        // Parse this vertex
                        ModelMeshVertex vertex = new ModelMeshVertex();
                        vertex.Position.X = positionStream.Values[positionStream.Stride * positionIndex + 0];
                        vertex.Position.Y = positionStream.Values[positionStream.Stride * positionIndex + 1];
                        vertex.Position.Z = positionStream.Values[positionStream.Stride * positionIndex + 2];
                        vertex.Normal.X = normalStream.Values[normalStream.Stride * normalIndex + 0];
                        vertex.Normal.Y = normalStream.Values[normalStream.Stride * normalIndex + 1];
                        vertex.Normal.Z = normalStream.Values[normalStream.Stride * normalIndex + 2];
                        vertex.TexCoord.X = texCoordStream.Values[texCoordStream.Stride * texCoordIndex + 0];
                        vertex.TexCoord.Y = texCoordStream.Values[texCoordStream.Stride * texCoordIndex + 1];

                        // Collada texture coordinates are based on 0,0 = bottom left
                        // Ours are based on 0,0 = top left
                        // so let's fix them :D
                        vertex.TexCoord.Y = 1.0f - vertex.TexCoord.Y;

                        // Add the vertex & its index to the mesh 
                        meshContent.Indices.Add(meshContent.Vertices.Count);
                        meshContent.Vertices.Add(vertex);
                    }

                    // Do we need to calculate the normals?
                    if (calculateNormals)
                    {
                        // Get the last 3 vertices we processed
                        ModelMeshVertex v1 = meshContent.Vertices[meshContent.Vertices.Count - 1];
                        ModelMeshVertex v2 = meshContent.Vertices[meshContent.Vertices.Count - 2];
                        ModelMeshVertex v3 = meshContent.Vertices[meshContent.Vertices.Count - 3];

                        // Calculate a face normal
                        Vector3 normal = Vector3.Normalize(Vector3.Cross(v2.Position - v1.Position, v2.Position - v3.Position));
                        v1.Normal = normal;
                        v2.Normal = normal;
                        v3.Normal = normal;
                    }
                }

                // Set the counts for the mesh indices of this part
                meshPart.IndexCount = meshContent.Indices.Count - meshPart.IndexStart;

                Context.LogVerbose(" +---- Indices: " + meshPart.IndexCount);
            }

            // Log the mesh data
            Context.LogInfo(" +-- Total Parts: " + meshContent.MeshParts.Count);
            Context.LogInfo(" +-- Total Indices: " + meshContent.Indices.Count);
            Context.LogInfo(" +-- Total Vertices: " + meshContent.Vertices.Count);

            // Process the mesh
            ProcessMesh(meshContent);

            // Add the mesh to the internal list
            ModelMeshes.Add(meshContent);
        }

        /// <summary>
        /// Processes a mesh
        /// </summary>
        public void ProcessMesh(ModelMeshContent mesh)
        {
            // Weld vertices in the mesh
            // ------------------------------

            Context.LogInfo(" +-- Optimizing Mesh");
            int oldVertexCount = mesh.Vertices.Count;

            // Loop through the vertices
            for (int vertexIndex1 = 0; vertexIndex1 < mesh.Vertices.Count; vertexIndex1++)
            {
                // Loop through the vertices again (from the currentIndex+1 and forward)
                for (int vertexIndex2 = vertexIndex1 + 1; vertexIndex2 < mesh.Vertices.Count; vertexIndex2++)
                {
                    // Check if the 2 vertices need to be welded
                    ModelMeshVertex vertex1 = mesh.Vertices[vertexIndex1];
                    ModelMeshVertex vertex2 = mesh.Vertices[vertexIndex2];
                    if (vertex1.IsEquivalent(ref vertex2) == false)
                        // No weld, continue
                        continue;

                    // Update all indices in this mesh
                    for (int index = 0; index < mesh.Indices.Count; index++)
                    {
                        // If this index is the vertex that was removed, then update it to the weld vertex
                        if (mesh.Indices[index] == vertexIndex2)
                            mesh.Indices[index] = vertexIndex1;

                        // If this index is after the vertex that was removed, then decrement it (to reflect the removed vertex)
                        else if (mesh.Indices[index] > vertexIndex2)
                            mesh.Indices[index]--;
                    }

                    // Remove the 2nd vertex & Backup the 2nd vertex loop counter 
                    mesh.Vertices.RemoveAt(vertexIndex2);
                    vertexIndex2--;
                }
            }

            int percentOfVertices = (oldVertexCount - mesh.Vertices.Count) * 100 / oldVertexCount;
            Context.LogInfo(" +---- New Total Vertices: " + mesh.Vertices.Count + "  [%" + percentOfVertices + " reduced]");

            // Apply global transforms to the mesh
            // -------------------------------------

            // Loop through the vertices
            foreach (ModelMeshVertex vertex in mesh.Vertices)
            {
                vertex.Position = Vector3.Transform(vertex.Position, MasterTransform);
                vertex.Normal = Vector3.TransformNormal(vertex.Normal, MasterTransform);
            }

            // Calculate the Mesh Bounding Volume
            // -------------------------------------

            // Start with a 0 radius sphere at the first vertex
            mesh.BoundingSphere = new Sphere3(mesh.Vertices[0].Position, 0.0f);

            // Loop through the rest of the vertices
            for (int vertexIndex = 1; vertexIndex < mesh.Vertices.Count; vertexIndex++)
            {
                mesh.BoundingSphere.GrowToContain(mesh.Vertices[vertexIndex].Position);
            }

            // Log the bounding sphere
            Context.LogInfo(" +-- Bounding Sphere - Center:[" + mesh.BoundingSphere.Center.ToString() + "] Radius:[" + mesh.BoundingSphere.Radius + "]");
        }

        /// <summary>
        /// Parses the collada xml definition for the given material & retrieves the diffuse texture applied to the material
        /// </summary>
        /// <param name="material"></param>
        /// <returns></returns>
        public string LookupTextureFromSampler(XmlNode xmlEffectProfile, string textureSampleId)
        {
            /*
            <newparam sid="TopMaterial-image-surface">
                <surface type="2D">
                    <init_from>TopMaterial-image</init_from>
                    <format>A8R8G8B8</format>
                </surface>
            </newparam>
            <newparam sid="TopMaterial-image-sampler">    <-- Texture Sampler ID (input)
                <sampler2D>
                    <source>TopMaterial-image-surface</source>
                    <minfilter>LINEAR_MIPMAP_LINEAR</minfilter>
                    <magfilter>LINEAR</magfilter>
                </sampler2D>
            </newparam>
            */

            // Lookup the surface sid from the sampler
            XmlNode xmlSamplerSource = xmlEffectProfile.SelectSingleNode("newparam[@sid='" + textureSampleId + "']/sampler2D/source");
            string surfaceSid = xmlSamplerSource.InnerText;

            // Lookup the image id from the surface
            XmlNode xmlSurfaceInitFrom = xmlEffectProfile.SelectSingleNode("newparam[@sid='" + surfaceSid + "']/surface/init_from");
            string imageId = xmlSurfaceInitFrom.InnerText;

            /*
            <library_images>
                <image id="TopMaterial-image" name="TopMaterial-image">
                    <init_from>TopTexture.png</init_from>
             */

            // Lookup the image 
            XmlNode xmlImageInitFrom = ColladaXml.SelectSingleNode("library_images/image[@id='" + imageId + "']/init_from");
            if(xmlImageInitFrom == null)
                throw new BuildException("Unable to find the image or its init_from child element for the image: " + imageId);

            // Return the texture file relative path
            return xmlImageInitFrom.InnerText;
        }

        /// <summary>
        /// Parses all the data streams in a mesh & builds a dictionary of them arranged by semantic
        /// </summary>
        public Dictionary<string, ColladaFloatDataStream> ParseSemanticStreams(XmlNode xmlMesh)
        {
            Dictionary<string, ColladaFloatDataStream> streamsBySemantic = new Dictionary<string, ColladaFloatDataStream>();

            // Get the first <triangles> node
            XmlNode xmlTriangles = xmlMesh.SelectSingleNode("triangles");

            // Loop through the <input> nodes
            foreach (XmlNode xmlInput in xmlTriangles.SelectNodes("input"))
            {
                // Get the semantic & source
                string semantic = xmlInput.Attributes["semantic"].Value;
                string source = xmlInput.Attributes["source"].Value.TrimStart('#');

                // If this is the "VERTEX" semantic, resolve it into the "POSITION" semantic
                if (semantic == "VERTEX")
                {
                    XmlNode xmlInputPosition = xmlMesh.SelectSingleNode("vertices[@id='" + source + "']/input[@semantic='POSITION']");
                    if (xmlInputPosition == null)
                        throw new BuildException("Mesh contains VERTEX semantic that does NOT resolve to a POSITION semantic");
                    source = xmlInputPosition.Attributes["source"].Value.TrimStart('#');
                    semantic = "POSITION";
                }

                // Get this semantic's vertex data stream
                XmlNode xmlSource = xmlMesh.SelectSingleNode("source[@id='" + source + "']");
                if (xmlSource == null)
                    throw new BuildException("Unable to find the vertex data source [" + source + "] for the semantic [" + semantic + "]");

                // Get the stride of this source
                XmlNode xmlAccessor = xmlSource.SelectSingleNode("technique_common/accessor");
                if (xmlAccessor == null)
                    throw new BuildException("Unable to find an accessor element in the vertex data source [" + source + "]");
                int stride = XmlUtilities.GetAttribute(xmlAccessor, "stride", 0);
                if (stride == 0)
                    throw new BuildException("accessor element has a missing or invalid stride attribute in source [" + source + "]");
                string accessorSource = xmlAccessor.Attributes["source"].Value.TrimStart('#');

                // Parse the actual source data
                XmlNode xmlFloatArray = xmlSource.SelectSingleNode("float_array[@id='" + accessorSource + "']");
                if (xmlFloatArray == null)
                    throw new BuildException("Source [" + source + "] is missing a float_array element");
                int floatArrayCount = XmlUtilities.GetAttribute(xmlFloatArray, "count", 0);
                if (floatArrayCount == 0)
                    throw new BuildException("float_array has an invalid or missing count attribute in the source [" + source + "]");

                ColladaFloatDataStream dataStream = new ColladaFloatDataStream();
                dataStream.Semantic = semantic;
                dataStream.Id = source;
                dataStream.Stride = stride;
                dataStream.Values = new float[floatArrayCount];

                // Split the source data up & put it into the array
                string[] strValues = xmlFloatArray.InnerText.Split(' ');
                for (int i = 0; i < strValues.Length; i++)
                    dataStream.Values[i] = float.Parse(strValues[i]);

                streamsBySemantic.Add(semantic, dataStream);
            }

            return streamsBySemantic;
        }
    }

    public class ColladaFloatDataStream
    {
        public string Semantic;
        public string Id;
        public int Stride;
        public float[] Values;
    }

    public class ModelMeshVertex
    {
        public Vector3 Position;
        public Vector3 Normal;
        public Vector2 TexCoord;

        /// <summary>
        /// Checks if this vertex & the input vertex are like enough that they can be welded
        /// </summary>
        public bool IsEquivalent(ref ModelMeshVertex input)
        {
            // Calculate the difference between the vertices
            float difference =
                Math.Abs(this.Position.X - input.Position.X) +
                Math.Abs(this.Position.Y - input.Position.Y) +
                Math.Abs(this.Position.Z - input.Position.Z) +
                Math.Abs(this.Normal.X - input.Normal.X) +
                Math.Abs(this.Normal.Y - input.Normal.Y) +
                Math.Abs(this.Normal.Z - input.Normal.Z) +
                Math.Abs(this.TexCoord.X - input.TexCoord.X) +
                Math.Abs(this.TexCoord.Y - input.TexCoord.Y)
                ;

            // Is the difference within the allowed error?
            if (difference < float.Epsilon)
                return true;
            return false;
        }
    }

    public class ModelMeshPartContent
    {
        public string MaterialName;
        public int IndexStart;
        public int IndexCount;
    }

    public class ModelMeshContent
    {
        public List<ModelMeshVertex> Vertices = new List<ModelMeshVertex>();
        public List<int> Indices = new List<int>();
        public List<ModelMeshPartContent> MeshParts = new List<ModelMeshPartContent>();
        public Sphere3 BoundingSphere;
    }

    [Flags]
    public enum ModelMaterialFlags
    {
        DiffuseTextured     = 0x0001,
        BumpTextured        = 0x0002,
    }

    public class ModelMaterialContent
    {
        public string Name;
        public int Index;
        public ModelMaterialFlags Flags;

        // Simple Properties
        public Color Emissive = Color.TransparentBlack;
        public Color Ambient = Color.Black;
        public Color Diffuse = Color.Purple;
        public Color Specular = Color.White;
        public float Shininess = 1.0f;
       
        // Texture References
        public string DiffuseTexture = null;
        public string BumpTexture = null;
    }
}

