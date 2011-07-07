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
        string ColladaSourceFile;
        string ColladaSourceFolder;

        // The source COLLADA & result MODEL objects 
        COLLADA.ColladaDocument Source;
        Model.Model ResultModel;
        
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

            details.AddParameter("Remove Unused Nodes", "Removes any nodes that do not contribute to the rendered model.  (Turn this off if you have ghost nodes you will use for binding)", "Optimization", typeof(bool), true);
            details.AddParameter("Remove Unused Meshes", "Removes any meshes that are not instanced in the model.  (Turn this off if you will be dynamically instancing meshes within a model)", "Optimization", typeof(bool), true);
            details.AddParameter("Remove Unused Materials", "Removes any materials that are not instanced in the model.  (Turn this off if you will be dynamically switching materials on a mesh, such as swapping skins)", "Optimization", typeof(bool), true);

            details.AddParameter("Default Ambient", "The diffuse channel for the default material assigned to any meshes that do not have a material binding in the COLLADA source.", "Default Material", typeof(Color), new Color(0, 0, 0));
            details.AddParameter("Default Diffuse", "The ambient channel for the default material assigned to any meshes that do not have a material binding in the COLLADA source.", "Default Material", typeof(Color), new Color(196, 0, 196));
            details.AddParameter("Default Specular", "The specular channel for the default material assigned to any meshes that do not have a material binding in the COLLADA source.", "Default Material", typeof(Color), new Color(255,255,255));
            details.AddParameter("Default Shininess", "The shininess for the default material assigned to any meshes that do not have a material binding in the COLLADA source.", "Default Material", typeof(float), 0.0f);
            details.AddParameter("Default Emissive", "The emissive channel for the default material assigned to any meshes that do not have a material binding in the COLLADA source.", "Default Material", typeof(Color), new Color(0, 0, 0));

            return details;
        }

        /// <summary>
        /// Processes the given asset
        /// </summary>
        public override void Process()
        {

            // Open & Validate the source collada
            // -------------------------------------

            // Get the full path to the source file & folder
            ColladaSourceFile = Path.Combine(Context.ContentFolder, Asset.Path);
            ColladaSourceFolder = Path.GetDirectoryName(ColladaSourceFile);

            Context.LogInfo("<br/><b>==== Parsing Collada File ====</b>");

            // Load the collada source
            Source = new Gdk.Content.COLLADA.ColladaDocument(ColladaSourceFile);

            // Log some collada stats
            Context.LogVerbose(" - COLLADA Version: " + Source.Version);
            Context.LogVerbose(" - Geometries: " + Source.Geometries.Count);
            Context.LogVerbose(" - Effects: " + Source.Effects.Count);
            Context.LogVerbose(" - Images: " + Source.Images.Count);
            Context.LogVerbose(" - Materials: " + Source.Materials.Count);
            Context.LogVerbose(" - Animation Clips: " + Source.AnimationClips.Count);
            Context.LogVerbose(" - Animations: " + Source.Animations.Count);
            Context.LogVerbose(" - Controllers: " + Source.Controllers.Count);
            Context.LogVerbose(" - Visual Scenes: " + Source.VisualScenes.Count);
            Context.LogInfo(" - Root Scene: " + Source.Scene.VisualSceneInstance.Url);
           
            // Build the Gdk Model DOM from the Collada DOM
            // --------------------------------------------

            Context.LogInfo("<br/><b>==== Generating Raw Model ====</b>");
            GenerateModel();

            Context.LogInfo("<br/><b>==== Optimizing Model ====</b>");
            OptimizeModel();

            Context.LogInfo("<br/><b>==== Finalizing Model ====</b>");
            FinalizeModel();

            // Process any dependent assets.  (IE: textures)
            // -----------------------------

            Context.LogInfo("<br/><b>==== Processing Dependent Assets ====</b>");
            ProcessDependencies();

            // Write the binary model
            // --------------------------

            Context.LogInfo("<br/><b>==== Writing Binary Model ====</b>");
            WriteModel();
        }

        #region Model Generation

        /// <summary>
        /// This method builds a raw GDK Model from the Source collada data
        /// </summary>
        public void GenerateModel()
        {   
            // Create the model
            // -------------------------------------

            // Create the new model that we are building
            this.ResultModel = new Model.Model();

            // Get the "Transform" parameters
            float transformScale = Context.Parameters.GetValue("Scale", 1.0f);
            Vector3 transformRotate = Context.Parameters.GetValue("Rotate", Vector3.Zero);
            Vector3 transformPreTranslate = Context.Parameters.GetValue("Pre-Translate", Vector3.Zero);
            Vector3 transformPostTranslate = Context.Parameters.GetValue("Post-Translate", Vector3.Zero);

            // Construct the root transform for this model, from the processor's "Transform" parameters
            Gdk.Matrix rootTransform =
                Matrix.CreateTranslation(transformPreTranslate)
                * Matrix.CreateRotationY(MathUtility.DegreesToRadians(transformRotate.Y))
                * Matrix.CreateRotationY(MathUtility.DegreesToRadians(transformRotate.Z))
                * Matrix.CreateRotationY(MathUtility.DegreesToRadians(transformRotate.X))
                * Matrix.CreateScale(transformScale)
                * Matrix.CreateTranslation(transformPostTranslate)
                ;

            // Setup the root node of the model
            Model.Node rootNode = new Model.Node();
            rootNode.Transform = rootTransform;
            rootNode.Id = "GDK.Model.Root";

            this.ResultModel.RootNode = rootNode;
            this.ResultModel.NodesById.Add(rootNode.Id, rootNode);

            // Process the Materials
            // -------------------------------------

            Context.LogInfo("<b> - Processing Materials</b>");

            // Loop through the collada materials
            foreach (KeyValuePair<string, COLLADA.Material> materialEntry in Source.Materials)
            {
                // Process this material
                ProcessColladaMaterial(materialEntry.Value);
            }

            // Process the Geometries
            // -------------------------------------

            Context.LogInfo("<br/><b> - Processing Geometries</b>");

            // Loop through the collada geometries
            foreach (KeyValuePair<string, COLLADA.Geometry> geometryEntry in Source.Geometries)
            {
                // Process this geometry
                ProcessColladaGeometry(geometryEntry.Value);   
            }

            // Process the scene node graph
            // -------------------------------------

            Context.LogInfo("<br/><b> - Processing Scene</b>");

            // Get the URL of the root VisualScene in the collada doc
            if (Source.Scene == null || Source.Scene.VisualSceneInstance == null)
                throw new BuildException("The collada data does not contain a root scene.");
            string rootSceneUrl = Source.Scene.VisualSceneInstance.Url;

            // Get the root VisualScene
            COLLADA.VisualScene rootScene = (COLLADA.VisualScene)Source.GetObjectByUrl(rootSceneUrl);

            // Loop through the nodes in the root scene
            foreach (COLLADA.Node colladaNode in rootScene.Nodes)
            {
                // Process this node
                ProcessColladaNode(colladaNode, this.ResultModel.RootNode, rootTransform, "");
            }
        }

        /// <summary>
        /// Processes a COLLADA.Node object
        /// </summary>
        public void ProcessColladaNode(COLLADA.Node colladaNode, Model.Node parentNode, Gdk.Matrix parentTransform, string logIndent)
        {
            Context.LogInfo(" +-" + logIndent + " Node: " + colladaNode.Id);

            // Get the full transform for this node
            Gdk.Matrix localTransform = COLLADA.TransformBase.GetCombinedTransforms(colladaNode.Transforms);
            Gdk.Matrix fullTransform = parentTransform * localTransform;

            // Create the Model.Node for this node
            Model.Node node = new Gdk.Content.Model.Node();
            node.Transform = localTransform;
            node.Id = colladaNode.Id;

            // Add this node to the parent & the global nodes lookup
            parentNode.ChildNodes.Add(node);
            this.ResultModel.NodesById.Add(node.Id, node);

            // Loop though the instanced geometry on this node
            foreach (COLLADA.InstanceGeometry geometryInstance in colladaNode.GeometryInstances)
            {
                // Process this geometry instance
                ProcessColladaInstanceGeometry(geometryInstance, node, logIndent);
            }

            // Loop through the child nodes
            foreach (COLLADA.Node colladaChildNode in colladaNode.ChildNodes)
            {
                // Recurse process this node
                ProcessColladaNode(colladaChildNode, node, fullTransform, logIndent + "--");
            }
        }

        /// <summary>
        /// Processes a COLLADA.InstanceGeometry object, at the given node
        /// </summary>
        public void ProcessColladaInstanceGeometry(COLLADA.InstanceGeometry geometryInstance, Model.Node node, string logIndent)
        {
            // Get the Mesh this instance is using
            string meshId = geometryInstance.Url;
            if(meshId.StartsWith("#"))
                meshId = meshId.Substring(1);
            Model.Mesh mesh;
            if(this.ResultModel.MeshesById.TryGetValue(meshId, out mesh) == false)
            {
                Context.LogWarn("The node id=\"" + node.Id + "\" contains an instance of an unknown/unrecognized mesh \"" + meshId + "\"");
                return;
            }

            // Create the GDK MeshInstance
            Model.MeshInstance meshInstance = new Model.MeshInstance();
            meshInstance.Mesh = mesh;
            meshInstance.Node = node;
            this.ResultModel.MeshInstances.Add(meshInstance);

            // Does the instance have any material bindings?
            if (geometryInstance.BindMaterial != null)
            {
                // Process the material bindings for this instance
                foreach (COLLADA.InstanceMaterial materialInstance in geometryInstance.BindMaterial.MaterialInstances)
                {
                    // Does the mesh have this material symbol?
                    if (mesh.TriangleSoupsByMaterialSymbol.ContainsKey(materialInstance.Symbol) == false)
                    {
                        Context.LogWarn("An [instance_geometry] in the node id=\"" + node.Id + "\" contains a material binding for a material symbol \"" + materialInstance.Symbol + "\" that doesnt exist on the mesh.  (this is usually safe to ignore)");
                        continue;
                    }

                    // Get the material that is targetted by this binding
                    string materialId = materialInstance.Target;
                    if (materialId.StartsWith("#"))
                        materialId = materialId.Substring(1);
                    Model.Material material;
                    if (this.ResultModel.MaterialsById.TryGetValue(materialId, out material) == false)
                    {
                        throw new BuildException("The node id=\"" + node.Id + "\" contains a geometry instance binding to an unknown material \"" + materialId + "\"");
                    }

                    // Add this binding to the mesh instance
                    meshInstance.MaterialBindings.Add(materialInstance.Symbol, material);
                }
            }

            // If the mesh has an unassigned material symbol, bind the default material to it
            if (mesh.TriangleSoupsByMaterialSymbol.ContainsKey("GDK-Unassigned"))
            {
                Model.Material defaultMaterial = this.ResultModel.MaterialsById["GDK-Default-Material"];
                meshInstance.MaterialBindings.Add("GDK-Unassigned", defaultMaterial);
            }

            // Verify the instance has a material binding for every material symbol in the mesh
            foreach(string materialSymbol in mesh.TriangleSoupsByMaterialSymbol.Keys)
                if(meshInstance.MaterialBindings.Keys.Contains(materialSymbol) == false)
                    throw new BuildException("An [instance_geometry] for the geometry \"" + mesh.Id + "\"] does not contains a binding for the material symbol \"" + materialSymbol + "\"");

            if(mesh.TriangleSoupsByMaterialSymbol.Count != meshInstance.MaterialBindings.Count)
                throw new BuildException("An [instance_geometry] for the geometry \"" + mesh.Id + "\"] does not have a matching number of material bindings.  [Expected:" + mesh.TriangleSoupsByMaterialSymbol.Count + "] [Actual:" + meshInstance.MaterialBindings.Count + "]");

            // Log the mesh instance
            Context.LogInfo(" +---" + logIndent + " Mesh Instance: " + mesh.Id);

            // Set the node as 'inUse'
            node.InUse = true;
        }

        /// <summary>
        /// Processes a COLLADA.Geometry object
        /// </summary>
        public void ProcessColladaGeometry(COLLADA.Geometry colladaGeometry)
        {
            // Is this a 'Mesh' Geometry?
            if (colladaGeometry.Mesh != null)
            {
                Context.LogInfo(" +- Mesh: " + colladaGeometry.Id);

                // Create a new Gdk Mesh
                Model.Mesh mesh = new Gdk.Content.Model.Mesh();

                // Copy the Id
                mesh.Id = colladaGeometry.Id;

                // Parse the triangle soups from the COLLADA data
                // ----------------------------------------------------

                // Process the <triangles> elements in the mesh
                foreach (COLLADA.Triangles triangles in colladaGeometry.Mesh.TrianglesSets)
                {
                    ProcessColladaTriangles(triangles, mesh);
                }

                // Process the <polylist> elements in the mesh
                foreach (COLLADA.PolyList polylist in colladaGeometry.Mesh.PolyListSets)
                {
                    ProcessColladaPolyList(polylist, mesh);
                }

                // TODO(P2): Support TriFans & TriStrips
                if (colladaGeometry.Mesh.TriFansSets.Count > 0)
                    throw new BuildException("The mesh \"" + mesh.Id + "\" contains a <trifans> element, which isnt currently supported");
                if (colladaGeometry.Mesh.TriStripsSets.Count > 0)
                    throw new BuildException("The mesh \"" + mesh.Id + "\" contains a <tristrips> element, which isnt currently supported");

                // Did we not find any triangles?
                if(mesh.TriangleSoupsByMaterialSymbol.Count == 0)
                {
                    Context.LogWarn(" +--- The mesh \"" + mesh.Id + "\" contains no triangle data, skipping");
                    return;
                }

                // Determine the mesh vertex format
                // ----------------------------------------------------

                // Get the first vertex in the first triangle of the first triangle soup :D
                Model.Vertex firstVertex = mesh.TriangleSoupsByMaterialSymbol.First().Value.Triangles[0].Vertices[0];

                // Verify every vertex within this mesh uses the exact same vertex data format
                foreach (Model.TriangleSoup triangleSoup in mesh.TriangleSoupsByMaterialSymbol.Values)
                    foreach (Model.Triangle triangle in triangleSoup.Triangles)
                        foreach (Model.Vertex vertex in triangle.Vertices)
                            if (firstVertex.Flags != vertex.Flags)
                                throw new BuildException("The geometry mesh \"" + colladaGeometry.Id + "\" contains mixed vertex types, which is currently un-supported by the GDK ModelProcessor");
                
                // Set the mesh flags
                mesh.Flags = firstVertex.Flags;

                // Log the mesh data
                // ----------------------------------------------------

                // Vertex Format
                string vertexFormat = 
                    "[Position: YES] " +
                    "[Normal: YES] " +
                    "[Color: " + ((mesh.Flags & Gdk.Content.Model.MeshFlags.VertexHasColor) > 0 ? "YES" : " NO") + "] " +
                    "[TexCoords1: " + ((mesh.Flags & Gdk.Content.Model.MeshFlags.VertexHasTexCoords) > 0 ? "YES" : " NO") + "] " +
                    "[TexCoords2: " + ((mesh.Flags & Gdk.Content.Model.MeshFlags.VertexHasTexCoords2) > 0 ? "YES" : " NO") + "] ";

                // Skinning info
                switch (mesh.Flags & Gdk.Content.Model.MeshFlags.SKINNING_TYPE_MASK)
                {
                    case Gdk.Content.Model.MeshFlags.VertexHasNoSkinning: vertexFormat += "[Skinning: NONE] ";  break;
                    case Gdk.Content.Model.MeshFlags.VertexHasSingleBone: vertexFormat += "[Skinning: 1 BONE] ";  break;
                    case Gdk.Content.Model.MeshFlags.VertexHas2WeightedBones: vertexFormat += "[Skinning: TWO BONES] ";  break;
                    case Gdk.Content.Model.MeshFlags.VertexHas4WeightedBones: vertexFormat += "[Skinning: FOUR BONES] ";  break;
                }

                Context.LogVerbose(" +--- Vertex Format: " + vertexFormat);

                // Log the triangle soup data
                foreach (KeyValuePair<string, Model.TriangleSoup> soupByMaterial in mesh.TriangleSoupsByMaterialSymbol)
                {
                    Context.LogVerbose(" +--- Triangle Soup:  [Material: " + soupByMaterial.Key + "] [#Triangles: " + soupByMaterial.Value.Triangles.Count + "]");
                }

                // Add the mesh to the model
                ResultModel.MeshesById.Add(mesh.Id, mesh);
            }
            else
            {
                Context.LogWarn(" +- Unrecognized geometry: " + colladaGeometry.Id + " [skipping]");
            }
        }

        /// <summary>
        /// Ensures that a material by the name "GDK-Default-Material" exists in the model, creating it if necessary.
        /// This material is what triangle soups / mesh parts are automatically assigned to, if the collada data has no material binding 
        /// </summary>
        public void EnsureDefaultMaterialExists()
        {
            // Does the model have the default material?
            if (this.ResultModel.MaterialsById.ContainsKey("GDK-Default-Material") == false)
            {
                // Create the default material
                Model.Material defaultMaterial = new Model.Material();
                defaultMaterial.Id = "GDK-Default-Material";
                defaultMaterial.Ambient = Context.Parameters.GetValue("Default Ambient", Color.Black);
                defaultMaterial.Diffuse = Context.Parameters.GetValue("Default Diffuse", new Color(196, 0, 196));
                defaultMaterial.Specular = Context.Parameters.GetValue("Default Specular", Color.White);
                defaultMaterial.Shininess = Context.Parameters.GetValue("Default Shininess", 0.0f);
                defaultMaterial.Emissive = Context.Parameters.GetValue("Default Emissive", Color.Black);
                defaultMaterial.Flags = 0;

                // Add the default material to the model
                this.ResultModel.MaterialsById.Add("GDK-Default-Material", defaultMaterial);
            }
        }

        /// <summary>
        /// Gets the triangle soup for the given material symbol, from the mesh.  If no soup exists for the given
        /// material, a new one is created and added to the mesh
        /// </summary>
        /// <param name="mesh"></param>
        Model.TriangleSoup GetTriangleSoupForMaterialSymbol(Model.Mesh mesh, string materialSymbol)
        {
            // If the material symbol is blank, use the builtin 'GDK-No-Material' material
            if (string.IsNullOrEmpty(materialSymbol))
            {
                Context.LogWarn("The mesh[id=\"" + mesh.Id + "\"] does not have a material symbol, defaulting to the \"GDK-Unassigned\" symbol");
                materialSymbol = "GDK-Unassigned";

                // Ensure the default material exists
                EnsureDefaultMaterialExists();
            }

            // Does the mesh have an existing triangle soup for this material symbol?
            if (mesh.TriangleSoupsByMaterialSymbol.ContainsKey(materialSymbol) == true)
            {
                // Use this soup
                return mesh.TriangleSoupsByMaterialSymbol[materialSymbol];
            }
            
            // Create a new soup for this material symbol
            Model.TriangleSoup triangleSoup = new Model.TriangleSoup();
            mesh.TriangleSoupsByMaterialSymbol.Add(materialSymbol, triangleSoup);

            return triangleSoup;
        }

        /// <summary>
        /// Processes a single COLLADA <triangles/> element and adds the triangle/vertex data to a Mesh
        /// </summary>
        void ProcessColladaTriangles(COLLADA.Triangles triangles, Model.Mesh mesh)
        {
            // Get a triangle soup in the mesh for this <triangles>'s material symbol
            Model.TriangleSoup triangleSoup = GetTriangleSoupForMaterialSymbol(mesh, triangles.Material);

            // Loop through the primitives sets (<p> elements)
            foreach(COLLADA.IndexSet indexSet in triangles.PrimitiveIndexSets)
            {
                int vertexSourceIndex = 0;

                // Loop through the triangles
                for (int triangleIndex = 0; triangleIndex < triangles.Count; triangleIndex++)
                {
                    // Create a triangle in the soup
                    Model.Triangle triangle = new Gdk.Content.Model.Triangle();
                    triangleSoup.Triangles.Add(triangle);

                    // Loop through the 3 vertices of this triangle
                    for (int i = 0; i < 3; i++)
                    {
                        // Sample the named data of this vertex from the source/accessor/inputs model of the face group
                        Dictionary<string, object> namedVertexData = new Dictionary<string,object>();
                        vertexSourceIndex += SampleColladaSharedInputs(vertexSourceIndex, indexSet, triangles.Inputs, namedVertexData, string.Empty);

                        // Parse the named data into a Model Vertex
                        Model.Vertex vertex = BuildModelVertex(namedVertexData);

                        // Add the vertex to the triangle
                        triangle.Vertices[i] = vertex;
                    } 
                }
            }
        }

        /// <summary>
        /// Processes a single COLLADA <polylist/> element and adds the triangle/vertex data to a Mesh
        /// </summary>
        /// <param name="mesh"></param>
        void ProcessColladaPolyList(COLLADA.PolyList polylist, Model.Mesh mesh)
        {
            // Get a triangle soup in the mesh for this <polylist>'s material symbol
            Model.TriangleSoup triangleSoup = GetTriangleSoupForMaterialSymbol(mesh, polylist.Material);

            // Verify the poly list has polygon vertices
            if (polylist.PrimitiveIndexSets.Count != 1)
                throw new BuildException("The mesh[id=\"" + mesh.Id + "\"]/polylist contains an invalid number of [p] child elements (should be 1)");

            int vertexSourceIndex = 0;

            // Get the index set for the polylist (<p>)
            COLLADA.IndexSet indexSet = polylist.PrimitiveIndexSets[0];

            // Loop through the poly vertex counts
            foreach (int polyVertexCount in polylist.VCounts)
            {
                // Make sure this poly has at least 3 vertices
                if (polyVertexCount < 3)
                    throw new BuildException("The mesh[id=\"" + mesh.Id + "\"]/polylist contains a poly with less than 3 vertices");

                // Sample the first vertex
                Dictionary<string, object> namedVertexData1 = new Dictionary<string, object>();
                vertexSourceIndex += SampleColladaSharedInputs(vertexSourceIndex, indexSet, polylist.Inputs, namedVertexData1, string.Empty);
                Model.Vertex vertex1 = BuildModelVertex(namedVertexData1);

                // Sample the 3rd vertex
                Dictionary<string, object> namedVertexData3 = new Dictionary<string, object>();
                vertexSourceIndex += SampleColladaSharedInputs(vertexSourceIndex, indexSet, polylist.Inputs, namedVertexData3, string.Empty);
                Model.Vertex vertex3 = BuildModelVertex(namedVertexData3);
                Model.Vertex vertex2;

                // Loop through the triangles
                for (int triangleIndex = 0; triangleIndex < polyVertexCount - 2; triangleIndex++)
                {
                    // Move V3 -> V2
                    vertex2 = vertex3;

                    // ReSample V3
                    namedVertexData3.Clear();
                    vertexSourceIndex += SampleColladaSharedInputs(vertexSourceIndex, indexSet, polylist.Inputs, namedVertexData3, string.Empty);
                    vertex3 = BuildModelVertex(namedVertexData3);

                    // Build a triangle out of the 3 vertices
                    Model.Triangle triangle = new Gdk.Content.Model.Triangle();
                    triangle.Vertices[0] = vertex1;
                    triangle.Vertices[1] = vertex2;
                    triangle.Vertices[2] = vertex3;
                    triangleSoup.Triangles.Add(triangle);
                }
            }
        }


        /// <summary>
        /// This method samples a set of collada <input/> (Shared) nodes and popualtes a dictionary of the named values that result from the lookup
        /// </summary>
        /// <param name="index">Current index into the indices table.</param>
        /// <param name="indices">Table of indices to use when doing the input lookups</param>
        /// <param name="sharedInputs">List of SharedInput(s) to use for the sampling process</param>
        /// <returns>The number of shared inputs that were processed.  This value + index = next index to sample</returns>
        int SampleColladaSharedInputs(int index, List<int> indices, IEnumerable<COLLADA.SharedInput> sharedInputs, Dictionary<string, object> results, string prefix)
        {
            int highestOffset = 0;

            // Loop through the inputs
            foreach(COLLADA.SharedInput sharedInput in sharedInputs)
            {
                // Get the index of this input
                int inputIndex = index + sharedInput.Offset;
                int sourceIndex = indices[inputIndex];

                // Track the highest offset used
                if (sharedInput.Offset > highestOffset)
                    highestOffset = sharedInput.Offset;

                // Is this a custom lookup semantic?
                if (sharedInput.Semantic == "VERTEX")
                {
                    // Get the Vertices object that this input's source references
                    COLLADA.Vertices vertices = Source.GetObjectByUrl(sharedInput.Source) as COLLADA.Vertices;
                    if (vertices == null)
                        throw new BuildException("Unable to find a [vertices] at the url: " + sharedInput.Source);

                    // Sample the inputs in the Vertices
                    SampleColladaInputs(sourceIndex, vertices.Inputs, results, prefix);
                }
                else
                {
                    // Get the Source object that this input references
                    COLLADA.Source source = Source.GetObjectByUrl(sharedInput.Source) as COLLADA.Source;
                    if (source == null)
                        throw new BuildException("Unable to find a [source] at the url: " + sharedInput.Source);

                    // Sample the source
                    SampleColladaSource(sourceIndex, source, results, prefix + sharedInput.Semantic + ".");
                }
            }

            // Return the highest offset + 1, as this is the stride of this lookup through the indices table
            return highestOffset + 1;
        }

        /// <summary>
        /// This method samples a set of collada <input/> (Unshared) nodes and popualtes a dictionary of the named values that result from the lookup
        /// </summary>
        /// <param name="index">Index to use when indexing the sources.</param>
        /// <param name="inputs">List of Input(s) to use for the sampling process</param>
        /// <param name="results">This dictionary is populated with the results</param>
        /// <param name="prefix">String to prefix to the result names in the results dictionary</param>
        void SampleColladaInputs(int index, IEnumerable<COLLADA.Input> inputs, Dictionary<string, object> results, string prefix)
        {
            // Loop through the inputs
            foreach (COLLADA.Input input in inputs)
            {
                // Get the Source object that this input references
                COLLADA.Source source = Source.GetObjectByUrl(input.Source) as COLLADA.Source;
                if (source == null)
                    throw new BuildException("Unable to find a [source] at the url: " + input.Source);

                // Sample the source
                SampleColladaSource(index, source, results, prefix + input.Semantic + ".");
            }
        }

        /// <summary>
        /// This method samples a <souce/> node and populates a dictionary with the named values that result from the lookup
        /// </summary>
        /// <param name="index">Index to use when indexing the source.</param>
        /// <param name="source">Source to be sampledf</param>
        /// <param name="results">This dictionary is populated with the results</param>
        /// <param name="prefix">String to prefix to the result names in the results dictionary</param>
        void SampleColladaSource(int index, COLLADA.Source source, Dictionary<string, object> results, string prefix)
        {
            // Get the value array that this source accesses
            COLLADA.ValueArrayBase valueArray = Source.GetObjectByUrl(source.Accessor.Source) as COLLADA.ValueArrayBase;
            if (valueArray == null)
                throw new BuildException("Unable to find a [xxx_array] at the url: " + source.Accessor.Source);

            // Loop through the params in the source
            int paramIndex = 0;
            foreach (COLLADA.Param param in source.Accessor.Params)
            {
                // Does this param have a name?
                if (string.IsNullOrEmpty(param.Name) == false)
                {
                    // Determine the index into the array for this param
                    int arrayIndex = (index * source.Accessor.Stride) + source.Accessor.Offset + paramIndex;

                    // Get the value from the array 
                    object value = valueArray.GetValue(arrayIndex);

                    // Add this value to the results
                    results.Add(prefix + param.Name, value);
                }

                // Next param index
                paramIndex++;
            }
        }

        /// <summary>
        /// Extracts a Model.Vertex from the given named vertex data blob (as parsed from collada data)
        /// </summary>
        public Model.Vertex BuildModelVertex(Dictionary<string, object> namedVertexData)
        {
            // Create a default vertex
            Model.Vertex vertex = new Model.Vertex();

            // Loop through the vertex data items
            foreach (KeyValuePair<string, object> dataItem in namedVertexData)
            {
                switch (dataItem.Key)
                {
                    // Position
                    case "POSITION.X": vertex.Position.X = GetObjectAsFloat(dataItem.Value); break;
                    case "POSITION.Y": vertex.Position.Y = GetObjectAsFloat(dataItem.Value); break;
                    case "POSITION.Z": vertex.Position.Z = GetObjectAsFloat(dataItem.Value); break;

                    // Normal
                    case "NORMAL.X": vertex.Normal.X = GetObjectAsFloat(dataItem.Value); break;
                    case "NORMAL.Y": vertex.Normal.Y = GetObjectAsFloat(dataItem.Value); break;
                    case "NORMAL.Z": vertex.Normal.Z = GetObjectAsFloat(dataItem.Value); break;

                    // TexCoord1
                    case "TEXCOORD.S": 
                        vertex.TexCoords1.X = GetObjectAsFloat(dataItem.Value);
                        vertex.Flags |= Model.MeshFlags.VertexHasTexCoords;
                        break;
                    case "TEXCOORD.T": 
                        vertex.TexCoords1.Y = GetObjectAsFloat(dataItem.Value);
                        vertex.Flags |= Model.MeshFlags.VertexHasTexCoords;

                        // Collada texture coordinates are based on 0,0 = bottom left
                        // GDK's are based on 0,0 = top left
                        // so let's fix them :D
                        vertex.TexCoords1.Y = 1.0f - vertex.TexCoords1.Y;

                        break;

                    // Color
                    case "COLOR.R":
                        vertex.Color.R = (byte)(Math.Max(Math.Min(GetObjectAsFloat(dataItem.Value), 1.0f), 0.0f) * 255.0f);
                        vertex.Flags |= Model.MeshFlags.VertexHasColor;
                        break;
                    case "COLOR.G":
                        vertex.Color.G = (byte)(Math.Max(Math.Min(GetObjectAsFloat(dataItem.Value), 1.0f), 0.0f) * 255.0f);
                        vertex.Flags |= Model.MeshFlags.VertexHasColor;
                        break;
                    case "COLOR.B":
                        vertex.Color.B = (byte)(Math.Max(Math.Min(GetObjectAsFloat(dataItem.Value), 1.0f), 0.0f) * 255.0f);
                        vertex.Flags |= Model.MeshFlags.VertexHasColor;
                        break;
                    case "COLOR.A":
                        vertex.Color.A = (byte)(Math.Max(Math.Min(GetObjectAsFloat(dataItem.Value), 1.0f), 0.0f) * 255.0f);
                        vertex.Flags |= Model.MeshFlags.VertexHasColor;
                        break;

                    // Vertex Data we ignore
                    case "TEXCOORD.P":
                    case "TEXCOORD.Q":
                        break;

                    // Unknown Vertex Data
                    default:
                        Context.LogWarn("Unprocessed Vertex Data Item: " + dataItem.Key);
                        break;
                }
            }

            return vertex;
        }

        /// <summary>
        /// Utility method to convert an object result from a collada value array, to a float
        /// </summary>
        public float GetObjectAsFloat(object value)
        {
            if (value is float)
                return (float)value;
            else if (value is bool)
                return ((bool)value) ? 1.0f : 0.0f;
            else if (value is int)
                return (float)((int)value);

            return float.Parse(value.ToString().Trim());
        }

        /// <summary>
        /// Processes a COLLADA.Material object
        /// </summary>
        public void ProcessColladaMaterial(COLLADA.Material colladaMaterial)
        {
            Context.LogInfo(" +- Material: " + colladaMaterial.Id);

            // Create a new Gdk Material
            Model.Material material = new Gdk.Content.Model.Material();

            // Copy the Id
            material.Id = colladaMaterial.Id;

            // Get the Effect & Effect-Instance that this material uses
            COLLADA.InstanceEffect effectInstance = colladaMaterial.InstanceEffect;
            COLLADA.Effect effect = (COLLADA.Effect)Source.GetObjectByUrl(effectInstance.Url);
            if (effect == null)
                throw new BuildException("The collada [material id=\"" + colladaMaterial.Id + "\"] is instancing a non existing effect \"" + effectInstance.Url + "\"");

            // Verify the common profile exists
            if (effect.Profile_COMMON == null)
                throw new BuildException("The collada [effect id=\"" + effect.Id + "\"] does NOT have a [profile_COMMON]");

            // Temp variables to be set when processing the technique
            string diffuseTextureRef = null;

            // Which type of technique is this effect using?
            switch (effect.Profile_COMMON.Technique.Type)
            {
                case Gdk.Content.COLLADA.TechniqueType.Blinn:
                    {
                        // Parse the Blinn settings into the Material
                        // -----------------------------------------

                        COLLADA.Blinn blinn = effect.Profile_COMMON.Technique.Blinn;

                        // Emissive
                        if (blinn.Emission.Type != COLLADA.ColorOrTexture.ColorOrTextureType.Color)
                            throw new BuildException("The collada [effect id=\"" + effect.Id + "\"] must have a [blinn/emission] value of type [color]");
                        material.Emissive = blinn.Emission.Color.ToGdkColor();

                        // Ambient
                        if (blinn.Ambient.Type != COLLADA.ColorOrTexture.ColorOrTextureType.Color)
                            throw new BuildException("The collada [effect id=\"" + effect.Id + "\"] must have a [blinn/ambient] value of type [color]");
                        material.Ambient = blinn.Ambient.Color.ToGdkColor();

                        // Diffuse
                        if (blinn.Diffuse.Type == COLLADA.ColorOrTexture.ColorOrTextureType.Color)
                        {
                            material.Diffuse = blinn.Diffuse.Color.ToGdkColor();
                        }
                        else if (blinn.Diffuse.Type == COLLADA.ColorOrTexture.ColorOrTextureType.Texture)
                        {
                            diffuseTextureRef = blinn.Diffuse.Texture.TextureRef;
                        }
                        else
                            throw new BuildException("The collada [effect id=\"" + effect.Id + "\"] must have a [blinn/diffuse] value of type [color] or [texture]");

                        // Specular
                        if (blinn.Specular.Type != COLLADA.ColorOrTexture.ColorOrTextureType.Color)
                            throw new BuildException("The collada [effect id=\"" + effect.Id + "\"] must have a [blinn/specular] value of type [color]");
                        material.Specular = blinn.Specular.Color.ToGdkColor();

                        // Shininess
                        if (blinn.Shininess.Type != COLLADA.FloatOrParam.FloatOrParamType.Float)
                            throw new BuildException("The collada [effect id=\"" + effect.Id + "\"] must have a [blinn/shininess] value of type [float]");
                        material.Shininess = blinn.Shininess.Float.Value;

                    }
                    break;

                case Gdk.Content.COLLADA.TechniqueType.Phong:
                    {
                        // Parse the Phong settings into the Material
                        // -----------------------------------------

                        COLLADA.Phong phong = effect.Profile_COMMON.Technique.Phong;

                        // Emissive
                        if (phong.Emission.Type != COLLADA.ColorOrTexture.ColorOrTextureType.Color)
                            throw new BuildException("The collada [effect id=\"" + effect.Id + "\"] must have a [phong/emission] value of type [color]");
                        material.Emissive = phong.Emission.Color.ToGdkColor();

                        // Ambient
                        if (phong.Ambient.Type != COLLADA.ColorOrTexture.ColorOrTextureType.Color)
                            throw new BuildException("The collada [effect id=\"" + effect.Id + "\"] must have a [phong/ambient] value of type [color]");
                        material.Ambient = phong.Ambient.Color.ToGdkColor();

                        // Diffuse
                        if (phong.Diffuse.Type == COLLADA.ColorOrTexture.ColorOrTextureType.Color)
                        {
                            material.Diffuse = phong.Diffuse.Color.ToGdkColor();
                        }
                        else if (phong.Diffuse.Type == COLLADA.ColorOrTexture.ColorOrTextureType.Texture)
                        {
                            diffuseTextureRef = phong.Diffuse.Texture.TextureRef;
                        }
                        else
                            throw new BuildException("The collada [effect id=\"" + effect.Id + "\"] must have a [phong/diffuse] value of type [color] or [texture]");

                        // Specular
                        if (phong.Specular.Type != COLLADA.ColorOrTexture.ColorOrTextureType.Color)
                            throw new BuildException("The collada [effect id=\"" + effect.Id + "\"] must have a [phong/specular] value of type [color]");
                        material.Specular = phong.Specular.Color.ToGdkColor();

                        // Shininess
                        if (phong.Shininess.Type != COLLADA.FloatOrParam.FloatOrParamType.Float)
                            throw new BuildException("The collada [effect id=\"" + effect.Id + "\"] must have a [phong/shininess] value of type [float]");
                        material.Shininess = phong.Shininess.Float.Value;
                    }
                    break;

                case Gdk.Content.COLLADA.TechniqueType.Lambert:
                    {
                        // Parse the Lambert settings into the Material
                        // -----------------------------------------

                        COLLADA.Lambert lambert = effect.Profile_COMMON.Technique.Lambert;

                        // Emissive
                        if (lambert.Emission.Type != COLLADA.ColorOrTexture.ColorOrTextureType.Color)
                            throw new BuildException("The collada [effect id=\"" + effect.Id + "\"] must have a [lambert/emission] value of type [color]");
                        material.Emissive = lambert.Emission.Color.ToGdkColor();

                        // Ambient
                        if (lambert.Ambient.Type != COLLADA.ColorOrTexture.ColorOrTextureType.Color)
                            throw new BuildException("The collada [effect id=\"" + effect.Id + "\"] must have a [lambert/ambient] value of type [color]");
                        material.Ambient = lambert.Ambient.Color.ToGdkColor();

                        // Diffuse
                        if (lambert.Diffuse.Type == COLLADA.ColorOrTexture.ColorOrTextureType.Color)
                        {
                            material.Diffuse = lambert.Diffuse.Color.ToGdkColor();
                        }
                        else if (lambert.Diffuse.Type == COLLADA.ColorOrTexture.ColorOrTextureType.Texture)
                        {
                            diffuseTextureRef = lambert.Diffuse.Texture.TextureRef;
                        }
                        else
                            throw new BuildException("The collada [effect id=\"" + effect.Id + "\"] must have a [lambert/diffuse] value of type [color] or [texture]");


                        // Lambert materials have no specular or shininess component
                        material.Specular = material.Diffuse;
                        material.Shininess = 0.0f;
                    }
                    break;

                default:
                    throw new BuildException("The effect technique type \"" + effect.Profile_COMMON.Technique.Type.ToString() + "\" is not supported");
            }

            // Is the diffuse component a texture?
            if(string.IsNullOrEmpty(diffuseTextureRef) == false)
            {
                // The material is diffuse textured, so set the material flag for diffuse texturing
                material.Diffuse = Gdk.Color.White;
                material.Flags |= Model.MaterialFlags.DiffuseTextured;

                // Lookup the sampler by the given sid
                COLLADA.Sampler2D sampler = (COLLADA.Sampler2D) GetFXParamValue(effect, effectInstance, diffuseTextureRef);

                // Lookup the source surface for the sampler
                COLLADA.Surface surface = (COLLADA.Surface) GetFXParamValue(effect, effectInstance, sampler.Source);

                // Verify this is an image based surface
                COLLADA.SurfaceInitFrom surfaceInitFrom = surface.InitMethod as COLLADA.SurfaceInitFrom;
                if (surfaceInitFrom == null)
                    throw new BuildException("The surface param \"" + sampler.Source + "\" has an unsupported init method");

                // Get the image this surface is initialized from
                COLLADA.Image image = Source.GetObjectByUrl("#" + surfaceInitFrom.ImageReference) as COLLADA.Image;
                
                // Store the diffuse texture relative path in the material
                material.DiffuseTexture = image.FilePath;
            }

            // Add the material to the model
            ResultModel.MaterialsById.Add(material.Id, material);
        }
     

        /// <summary>
        /// Finds the value of a given FX parameter, for the given effect / instance
        /// </summary>
        public COLLADA.ValueTypeBase GetFXParamValue(COLLADA.Effect effect, COLLADA.InstanceEffect effectInstance, string paramSid)
        {
            // Does the InstanceEffect have an override for this parameter?
            foreach(COLLADA.SetParam setParam in effectInstance.SetParams)
                if(setParam.Ref == paramSid)
                    return setParam.Value;

            // Does the Effect have a NewParam value for this parameter?
            foreach(COLLADA.NewParam loopNewParam in effect.NewParams)
                if(loopNewParam.Sid == paramSid)
                    return loopNewParam.Value;

            // Does the Effect/profile have a NewParam value for this parameter?
            foreach(COLLADA.NewParam loopNewParam in effect.Profile_COMMON.NewParams)
                if(loopNewParam.Sid == paramSid)
                    return loopNewParam.Value;

            // Does the Effect/profile/technique have a NewParam value for this parameter?
            foreach(COLLADA.NewParam loopNewParam in effect.Profile_COMMON.Technique.NewParams)
                if(loopNewParam.Sid == paramSid)
                    return loopNewParam.Value;

            // We cant find the parameter...
            throw new BuildException("Unable to resolve the FX parameter \"" + paramSid + "\".  It doesnt exist in any [newparam] nodes within the effect \"" + effect.Id + "\"");
        }

        #endregion

        #region Model Optimization

        /// <summary>
        /// Optimizes the gdk model
        /// </summary>
        public void OptimizeModel()
        {
            // Remove unused meshes
            // =================================

            bool removeUnusedMeshes = Context.Parameters.GetValue("Remove Unused Meshes", true);
            if (removeUnusedMeshes)
            {
                Context.LogInfo(" - Checking for unused meshes");

                // Loop through the meshes by id
                List<string> meshIds = new List<string>(this.ResultModel.MeshesById.Keys);
                foreach (string meshId in meshIds)
                {
                    // Get this mesh
                    Model.Mesh mesh = this.ResultModel.MeshesById[meshId];

                    // Are there any instances of this mesh?
                    bool instanceFound = false;
                    foreach (Model.MeshInstance meshInstance in this.ResultModel.MeshInstances)
                        if (meshInstance.Mesh == mesh)
                        {
                            instanceFound = true;
                            break;
                        }

                    // Did we not find any instances of this mesh?
                    if (instanceFound == false)
                    {
                        // Remove the mesh from the model
                        this.ResultModel.MeshesById.Remove(meshId);
                        Context.LogInfo(" +- Removing unused mesh: \"" + meshId + "\"");
                    }
                }
            }

            // Remove unused materials
            // =================================

            bool removeUnusedMaterials = Context.Parameters.GetValue("Remove Unused Materials", true);
            if (removeUnusedMaterials)
            {
                Context.LogInfo(" - Checking for unused materials");

                // Loop through the meshes by id
                List<string> materialIds = new List<string>(this.ResultModel.MaterialsById.Keys);
                foreach (string materialId in materialIds)
                {
                    // Get this material
                    Model.Material material = this.ResultModel.MaterialsById[materialId];

                    // Are there any instances of this material?
                    bool instanceFound = false;
                    foreach (Model.MeshInstance meshInstance in this.ResultModel.MeshInstances)
                        foreach(KeyValuePair<string, Model.Material> materialBinding in meshInstance.MaterialBindings)
                            if (materialBinding.Value == material)
                            {
                                instanceFound = true;
                                break;
                            }

                    // Did we not find any instances of this material?
                    if (instanceFound == false)
                    {
                        // Remove the material from the model
                        this.ResultModel.MaterialsById.Remove(materialId);
                        Context.LogInfo(" +- Removing unused material: \"" + materialId + "\"");
                    }
                }
            }

            // Remove unused nodes
            // =================================

            bool removeUnusedNodes = Context.Parameters.GetValue("Remove Unused Nodes", true);
            if (removeUnusedMaterials)
            {
                Context.LogInfo(" - Checking for unused nodes");

                // Recursively update the node tree so all ancestors of an InUse node, are also InUse
                this.ResultModel.RootNode.InUse = RecurseUpdateNodeUsage(this.ResultModel.RootNode);

                // Loop through the nodes by id
                List<string> nodeIds = new List<string>(this.ResultModel.NodesById.Keys);
                foreach (string nodeId in nodeIds)
                {
                    // Get this node
                    Model.Node node = this.ResultModel.NodesById[nodeId];

                    // Is this node Not in use?
                    if(node.InUse == false)
                    {
                        // Remove the node from the model
                        this.ResultModel.NodesById.Remove(nodeId);
                        Context.LogInfo(" +- Removing unused node: \"" + nodeId + "\"");
                    }
                }
            }

            // Combine like meshes
            // =================================

            // TODO(P2) - Technically, it could improve performance to flatten all meshes through the node scenegraph transforms
            //   This would only be possible if there is no animation.
            //   Only non-skinned meshes
            //   Only combine like-vertex data

            // Order data for optimal rendering
            // =================================

            // TODO(P1) - Sort the mesh instances for optimal performance


        }

        /// <summary>
        /// Recursively walks a node graph and sets the InUse flag for a node, if any of its decendents have the InUse flag set
        /// </summary>
        /// <param name="node"></param>
        public bool RecurseUpdateNodeUsage(Model.Node node)
        {
            // Recurse to the children.  If any of them are in use, then this node will be too!
            foreach (Model.Node childNode in node.ChildNodes)
                node.InUse |= RecurseUpdateNodeUsage(childNode);
            return node.InUse;
        }

        #endregion

        #region Model Finalization

        /// <summary>
        /// Finalizes the gdk model
        /// </summary>
        public void FinalizeModel()
        {
            // Assign Object Indices
            // ==================================

            // Nodes
            int nodeIndex = 0;
            RecurseSetNodeIndex(this.ResultModel.RootNode, ref nodeIndex, 0);

            // Materials
            int materialIndex = 0;
            foreach(Model.Material material in this.ResultModel.MaterialsById.Values)
            {
                material.Index = materialIndex;
                materialIndex++;
            }

            // Meshes
            int meshIndex = 0;
            foreach (Model.Mesh mesh in this.ResultModel.MeshesById.Values)
            {
                mesh.Index = meshIndex;
                meshIndex++;
            }

            // Finalize the Meshes
            // ==================================

            Context.LogInfo(" - Finalizing Meshes");

            int originalVertices = 0;
            int reducedVertices = 0;

            // Loop through the meshes
            foreach (Model.Mesh mesh in this.ResultModel.MeshesById.Values)
            {

                // Calculate missing normals
                // -------------------------------------

                bool autoCalculatedNormals = false;

                // Loop through the triangle soups in this mesh
                foreach (KeyValuePair<string, Model.TriangleSoup> triangleSoupByMaterialSymbol in mesh.TriangleSoupsByMaterialSymbol)
                {
                    Model.TriangleSoup triangleSoup = triangleSoupByMaterialSymbol.Value;

                    // Loop through the triangles
                    foreach (Model.Triangle triangle in triangleSoup.Triangles)
                    {
                        // Does this triangle need normals?
                        if (triangle.Vertices[0].Normal.Equals(Vector3.Zero))
                        {
                            // Calculate a face normal for this triangle
                            Vector3 p1 = triangle.Vertices[0].Position;
                            Vector3 p2 = triangle.Vertices[1].Position;
                            Vector3 p3 = triangle.Vertices[2].Position;
                            Vector3 normal = Vector3.Normalize(Vector3.Cross(p2 - p1, p2 - p3));
                            triangle.Vertices[0].Normal = normal;
                            triangle.Vertices[1].Normal = normal;
                            triangle.Vertices[2].Normal = normal;

                            autoCalculatedNormals = true;
                        }
                    }
                }

                // Log the work, if we did it
                if (autoCalculatedNormals == true)
                {
                    Context.LogVerbose(" +- Mesh \"" + mesh.Id + "\" is missing normals.  They will be auto-generated");
                }

                // Process the triangle soups into vertex/index buffer'd mesh parts
                // ------------------------------------------------------------------

                // Loop through the triangle soups
                foreach (KeyValuePair<string, Model.TriangleSoup> triangleSoupByMaterialSymbol in mesh.TriangleSoupsByMaterialSymbol)
                {
                    // Get the soup & the material symbol
                    string materialSymbol = triangleSoupByMaterialSymbol.Key;
                    Model.TriangleSoup triangleSoup = triangleSoupByMaterialSymbol.Value;

                    // Create a mesh part for this soup
                    Model.MeshPart meshPart = new Gdk.Content.Model.MeshPart();
                    mesh.MeshParts.Add(meshPart);
                    meshPart.IndexStart = mesh.Indices.Count;
                    meshPart.IndexCount = triangleSoup.Triangles.Count * 3;
                    meshPart.MaterialSymbol = triangleSoupByMaterialSymbol.Key;

                    // Convert triangle soup to vertices/indices   (removing duplicate vertices)
                    foreach (Model.Triangle triangle in triangleSoup.Triangles)
                    {
                        AddIndexedVertexToMesh(mesh, triangle.Vertices[0]);
                        AddIndexedVertexToMesh(mesh, triangle.Vertices[1]);
                        AddIndexedVertexToMesh(mesh, triangle.Vertices[2]);
                    }

                    // Track the vertex counts
                    originalVertices += triangleSoup.Triangles.Count * 3;
                }
                reducedVertices += mesh.Vertices.Count;

                // Check for huge meshes..
                if (mesh.Vertices.Count > UInt16.MaxValue)
                    throw new BuildException("Mesh contains too many vertices:  " + mesh.Indices.Count);
                if (mesh.Indices.Count > UInt16.MaxValue)
                    throw new BuildException("Mesh contains too many indices:  " + mesh.Indices.Count);

                // Calculate the Mesh Bounding Volume
                // -------------------------------------

                // Start with a 0 radius sphere at the first vertex
                mesh.BoundingSphere = new Sphere3(mesh.Vertices[0].Position, 0.0f);

                // Loop through the rest of the vertices
                for (int vertexIndex = 1; vertexIndex < mesh.Vertices.Count; vertexIndex++)
                {
                    mesh.BoundingSphere.GrowToContain(mesh.Vertices[vertexIndex].Position);
                }
            }

            // Log the final object counts
            // ====================================

            Context.LogInfo(" - Final Model Details");
            Context.LogInfo(" +- Nodes: " + this.ResultModel.NodesById.Count);
            Context.LogInfo(" +- Materials: " + this.ResultModel.MaterialsById.Count);
            Context.LogInfo(" +- Meshes: " + this.ResultModel.MeshesById.Count);
            Context.LogInfo(" +--- Vertex Count: [Original: " + originalVertices + "] [Reduced: " + reducedVertices + "] - " + (int)((1.0f - reducedVertices / (float)originalVertices) * 100) + "% reduction");
            Context.LogInfo(" +- Mesh Instances: " + this.ResultModel.MeshInstances.Count);
        }

        /// <summary>
        /// Adds a vertex & its index to the mesh.  If a like vertex already exists, that vertex is indexed instead.
        /// </summary>
        public int AddIndexedVertexToMesh(Model.Mesh mesh, Model.Vertex vertex)
        {
            // Loop through the existing vertices
            for(int vertexIndex=0; vertexIndex < mesh.Vertices.Count; vertexIndex++)
            {
                // Is this existing vertex a match or the vertex we're adding?
                if (mesh.Vertices[vertexIndex].Equals(vertex))
                {
                    // Use the index of the existing vertex
                    mesh.Indices.Add(vertexIndex);
                    return vertexIndex;
                }
            }

            // No match found, add this vertex
            int newIndex = mesh.Vertices.Count;
            mesh.Vertices.Add(vertex);
            mesh.Indices.Add(newIndex);
            return newIndex;
        }

        /// <summary>
        /// Finalizes the gdk model
        /// </summary>
        public void RecurseSetNodeIndex(Model.Node node, ref int index, int parentIndex)
        {
            // Set this node's index & parent index
            node.Index = index;
            node.ParentIndex = parentIndex;

            // Increment the index
            index++;

            // Recurse to the child nodes
            foreach (Model.Node childNode in node.ChildNodes)
                RecurseSetNodeIndex(childNode, ref index, node.Index);
        }

        #endregion

        #region Dependency Processing

        /// <summary>
        /// Processes any dependent assets, such as referenced textures
        /// </summary>
        public void ProcessDependencies()
        {
            // Get the pixel format processor settings
            PixelFormats diffusePixelFormat = Context.Parameters.GetEnumValue<PixelFormats>("Diffuse PixelFormat", PixelFormats.RGBA_5551);

            // Loop through the materials
            foreach (KeyValuePair<string, Model.Material> entry in this.ResultModel.MaterialsById)
            {
                Model.Material material = entry.Value;

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

        #endregion

        #region Binary Writer

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
            UInt16 version = 2;
            UInt16 headerFlags = 0;
            writer.Write(version);
            writer.Write(headerFlags);

            // Sub-object counts
            writer.Write((UInt16) this.ResultModel.NodesById.Count);
            writer.Write((UInt16) this.ResultModel.MaterialsById.Count);
            writer.Write((UInt16) this.ResultModel.MeshesById.Count);
            writer.Write((UInt16) this.ResultModel.MeshInstances.Count);

            // Nodes
            // -----------------------

            // Loop through the nodes in index order
            foreach (Model.Node node in this.ResultModel.NodesById.Values.OrderBy(n => n.Index))
            {
                // Write out the node info
                BinaryWriterUtilities.WriteString(writer, node.Id);
                writer.Write((UInt16)node.ParentIndex);
                BinaryWriterUtilities.WriteMatrix(writer, node.Transform);
            }

            // Materials
            // -----------------------
            
            // Loop through the materials in index order
            foreach (Model.Material material in this.ResultModel.MaterialsById.Values.OrderBy(n => n.Index))
            {
                // Write out the material
                BinaryWriterUtilities.WriteString(writer, material.Id);
                writer.Write((UInt16)material.Flags);
                BinaryWriterUtilities.WriteColor(writer, material.Emissive);
                BinaryWriterUtilities.WriteColor(writer, material.Ambient);
                BinaryWriterUtilities.WriteColor(writer, material.Diffuse);
                BinaryWriterUtilities.WriteColor(writer, material.Specular);
                writer.Write((float)material.Shininess);
                
                if ((material.Flags & Model.MaterialFlags.DiffuseTextured) > 0)
                    BinaryWriterUtilities.WriteString(writer, material.DiffuseTexture);
                if ((material.Flags & Model.MaterialFlags.BumpTextured) > 0)
                    BinaryWriterUtilities.WriteString(writer, material.BumpTexture);
            }

            // Meshes
            // -----------------------
            
            // Loop through the meshes in index order
            foreach (Model.Mesh mesh in this.ResultModel.MeshesById.Values.OrderBy(n => n.Index))
            {
                // Write out the mesh header
                BinaryWriterUtilities.WriteString(writer, mesh.Id);
                writer.Write((UInt16)mesh.Flags);
                writer.Write((UInt16)mesh.Vertices.Count);
                writer.Write((UInt16)mesh.Indices.Count);
                writer.Write((UInt16)mesh.MeshParts.Count);

                // Write the bounding sphere
                BinaryWriterUtilities.WriteVector3(writer, mesh.BoundingSphere.Center);
                writer.Write(mesh.BoundingSphere.Radius);

                // Write the vertex data
                foreach(Model.Vertex vertex in mesh.Vertices)
                {
                    // Write the position & normal
                    BinaryWriterUtilities.WriteVector3(writer, vertex.Position);
                    BinaryWriterUtilities.WriteVector3(writer, vertex.Normal);
                    
                    // Write the optional vertex properties
                    if ((mesh.Flags & Model.MeshFlags.VertexHasColor) > 0)
                        BinaryWriterUtilities.WriteColor(writer, vertex.Color);
                    if ((mesh.Flags & Model.MeshFlags.VertexHasTexCoords) > 0)
                        BinaryWriterUtilities.WriteVector2(writer, vertex.TexCoords1);
                    if ((mesh.Flags & Model.MeshFlags.VertexHasTexCoords2) > 0)
                        BinaryWriterUtilities.WriteVector2(writer, vertex.TexCoords2);

                    // Write the bone index/weight propeties
                    if ((mesh.Flags & Model.MeshFlags.SKINNING_TYPE_MASK) == Model.MeshFlags.VertexHasSingleBone)
                        writer.Write(vertex.BoneIndices[0]);
                    else if ((mesh.Flags & Model.MeshFlags.SKINNING_TYPE_MASK) == Model.MeshFlags.VertexHas2WeightedBones)
                    {
                        writer.Write(vertex.BoneIndices[0]);
                        writer.Write(vertex.BoneIndices[1]);
                        writer.Write(vertex.BoneWeights[0]);
                        writer.Write(vertex.BoneWeights[1]);
                    }
                    else if ((mesh.Flags & Model.MeshFlags.SKINNING_TYPE_MASK) == Model.MeshFlags.VertexHas4WeightedBones)
                    {
                        writer.Write(vertex.BoneIndices[0]);
                        writer.Write(vertex.BoneIndices[1]);
                        writer.Write(vertex.BoneIndices[2]);
                        writer.Write(vertex.BoneIndices[3]);
                        writer.Write(vertex.BoneWeights[0]);
                        writer.Write(vertex.BoneWeights[1]);
                        writer.Write(vertex.BoneWeights[2]);
                        writer.Write(vertex.BoneWeights[3]);
                    }
                }

                // Write the indices
                foreach (int index in mesh.Indices)
                {
                    writer.Write((UInt16)index);
                }

                // Loop through the mesh parts
                foreach (Model.MeshPart meshPart in mesh.MeshParts)
                {
                    writer.Write((UInt16)meshPart.IndexStart);
                    writer.Write((UInt16)meshPart.IndexCount);
                    BinaryWriterUtilities.WriteString(writer, meshPart.MaterialSymbol);
                }
            }

            // Mesh Instances
            // -----------------------
            
            // Loop through the mesh instances
            foreach (Model.MeshInstance meshInstance in this.ResultModel.MeshInstances)
            {
                // Write the instanced object indices
                writer.Write((UInt16)meshInstance.Node.Index);
                writer.Write((UInt16)meshInstance.Mesh.Index);

                // Write the material assignment table for the mesh

                // Loop through the mesh parts of the mesh
                foreach (Model.MeshPart meshPart in meshInstance.Mesh.MeshParts)
                {
                    // Get the material this mesh part's material symbol is bound to
                    Model.Material material = meshInstance.MaterialBindings[meshPart.MaterialSymbol];

                    // Write the material index for the material this part is bound to
                    writer.Write((UInt16)material.Index);
                }
            }

            // Finished
            // -----------------------

            writer.Flush();
            writer.Close();
        }

        #endregion

    } // class
} // namespace