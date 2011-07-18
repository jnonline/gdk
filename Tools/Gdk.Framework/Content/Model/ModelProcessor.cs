using System;
using System.Collections.Generic;
using System.Linq;
using System.IO;
using System.Text;
using System.Xml;
using Gdk;

namespace Gdk.Content.Model
{
    
    // ===============================================================
    public class ModelProcessor : Processor
    {
        // Source content (Collada)
        string ColladaSourceFile;
        string ColladaSourceFolder;
        COLLADA.ColladaDocument Source;

        // Destination content (Gdk.Model)
        Model ResultModel;

        // Intermediate objects used during the processing
        public Dictionary<string, Material> MaterialsByColladaId = new Dictionary<string, Material>();
        public Dictionary<string, Mesh> MeshesByColladaId = new Dictionary<string, Mesh>();
        public Dictionary<string, Animation> AnimationsByColladaId = new Dictionary<string, Animation>();

        /// <summary>
        /// Gets the descriptive details of this processor
        /// </summary>
        public override ProcessorDetails GetDetails()
        {
            ProcessorDetails details = new ProcessorDetails(
                "Processes all the data from a COLLADA file into a Gdk Model",
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
            Context.LogVerbose(" - Nodes: " + Source.Nodes.Count);
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
            // Create the new model that we are building
            this.ResultModel = new Model();

            // Setup the root transforms for the model
            // -------------------------------------

            Context.LogInfo("<b> - Setting up root transforms</b>");

            // Construct the root transform for this model; 
            Matrix rootTransform = Matrix.Identity;

            // Alter the root node to compensate for the UP Axis of the collada data
            switch (this.Source.Asset.UpAxis)
            {
                case COLLADA.UpAxisTypes.X_UP:
                    throw new BuildException("X_UP up-axis models are not currently supported");

                case COLLADA.UpAxisTypes.Z_UP:
                    Context.LogInfo(" +-- Converting collada data from Z_UP to Y_UP");
                    rootTransform *= Matrix.CreateRotationX(3.0f * MathUtility.PiOver2);
                    break;

                case COLLADA.UpAxisTypes.Y_UP:
                    Context.LogInfo(" +-- Collada data is already in Y_UP format");
                    break;
            }

            // Apply scaling to get the collada data into a 1unit = 1meter scale
            if (this.Source.Asset.UnitMeter != 1.0f)
            {
                Context.LogInfo(" +-- Scaling collada data to match [1-Unit = 1-Meter].  Current scale [1-Unit = " + this.Source.Asset.UnitMeter + "-Meters]");
                rootTransform *= Matrix.CreateScale(this.Source.Asset.UnitMeter);
            }
            else
                Context.LogInfo(" +-- Collada data is already in the default scale [1-Unit = 1-Meter].");

            // Get the processor's "Transform" parameters
            float transformScale = Context.Parameters.GetValue("Scale", 1.0f);
            Vector3 transformRotate = Context.Parameters.GetValue("Rotate", Vector3.Zero);
            Vector3 transformPreTranslate = Context.Parameters.GetValue("Pre-Translate", Vector3.Zero);
            Vector3 transformPostTranslate = Context.Parameters.GetValue("Post-Translate", Vector3.Zero);

            // Apply the processor's "Transform" parameters to the root transform
            rootTransform = rootTransform
                * Matrix.CreateTranslation(transformPreTranslate)
                * Matrix.CreateScale(transformScale)
                * Matrix.CreateRotationX(MathUtility.DegreesToRadians(transformRotate.X))
                * Matrix.CreateRotationY(MathUtility.DegreesToRadians(transformRotate.Y))
                * Matrix.CreateRotationZ(MathUtility.DegreesToRadians(transformRotate.Z))
                * Matrix.CreateTranslation(transformPostTranslate)
                ;

            // Setup the root node of the model
            Node rootNode = new Node();
            rootNode.PoseTransform = rootTransform;
            rootNode.Name = "GDK.Model.Root";

            this.ResultModel.RootNode = rootNode;
            this.ResultModel.Nodes.Add(rootNode);

            // Process the Materials
            // -------------------------------------

            Context.LogInfo("<br/><b> - Processing Materials</b>");

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

            // Process the Controllers
            // -------------------------------------

            Context.LogInfo("<br/><b> - Processing Controllers</b>");

            // Loop through the collada controllers
            foreach (KeyValuePair<string, COLLADA.Controller> controllerEntry in Source.Controllers)
            {
                // Process this controller
                ProcessColladaController(controllerEntry.Value);
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

            // Process animations
            // -------------------------------------

            Context.LogInfo("<br/><b> - Processing Animations</b>");

            // Loop through the collada animations
            foreach (KeyValuePair<string, COLLADA.Animation> animationEntry in Source.Animations)
            {
                ProcessColladaAnimation(animationEntry.Value, null, string.Empty);
            }

            // Process animation clips
            // -------------------------------------

            Context.LogInfo("<br/><b> - Processing Animation Clips</b>");

            // Loop through the collada animation clips
            foreach (KeyValuePair<string, COLLADA.AnimationClip> animationClipEntry in Source.AnimationClips)
            {
                ProcessColladaAnimationClip(animationClipEntry.Value);
            }

        }

        /// <summary>
        /// Processes a COLLADA.AnimationClip object
        /// </summary>
        public void ProcessColladaAnimationClip(COLLADA.AnimationClip colladaAnimationClip)
        {
            // Create a new Gdk AnimationClip
            AnimationClip animationClip = new AnimationClip();

            // Get the animation clip Name & Id
            animationClip.Name = colladaAnimationClip.Name;
            if (string.IsNullOrEmpty(animationClip.Name))
                animationClip.Name = colladaAnimationClip.Id;

            Context.LogInfo(" +- Animation Clip: " + animationClip.Name + " [Collada ID: " + colladaAnimationClip.Id + "]");

            // Get the clip start/end times
            animationClip.StartTime = (float)colladaAnimationClip.Start;
            animationClip.EndTime = (float)colladaAnimationClip.End;

            // Loop through the animations that are instanced in this clip
            foreach(COLLADA.InstanceAnimation animationInstance in colladaAnimationClip.AnimationInstances)
            {
                // Get the ID of this instanced animation
                string animationId = animationInstance.Url;
                if(animationId.StartsWith("#"))
                    animationId = animationId.Substring(1);

                // Lookup the animation & add it to our clip
                Animation animation = this.AnimationsByColladaId[animationId];
                animationClip.Animations.Add(animation);
            }

            // Add the animation clip to the model
            this.ResultModel.AnimationClips.Add(animationClip);
        }

        /// <summary>
        /// Processes a COLLADA.Animation object
        /// </summary>
        public void ProcessColladaAnimation(COLLADA.Animation colladaAnimation, Animation parentAnimation, string indent)
        {
            // Create a new Gdk Animation
            Animation animation = new Animation();

            // Get the animation Name & Id
            animation.ColladaID = colladaAnimation.Id;
            animation.Name = colladaAnimation.Name;
            if(string.IsNullOrEmpty(animation.Name))
                animation.Name = colladaAnimation.Id;

            Context.LogInfo(" +-" + indent + " Animation: " + animation.Name + " [Collada ID: " + colladaAnimation.Id + "]");

            // If the animation has an ID, add it to our Id list
            if(string.IsNullOrEmpty(colladaAnimation.Id) == false)
            {
                this.AnimationsByColladaId.Add(colladaAnimation.Id, animation);
            }

            // Add the anim to the parent
            if(parentAnimation != null)
                parentAnimation.Children.Add(animation);

            // Add the animation to the model
            this.ResultModel.Animations.Add(animation);

            // Process child animations
            // -----------------------------

            foreach (COLLADA.Animation childColladaAnimation in colladaAnimation.Animations)
            {
                ProcessColladaAnimation(childColladaAnimation, animation, indent + "--");
            }

            // Process the node/matrix animations within this animation
            // -----------------------------

            ProcessNodeAnimations(colladaAnimation, animation, indent);

        }

        /// <summary>
        /// Processes the node animations from a collada animation
        /// </summary>
        public void ProcessNodeAnimations(COLLADA.Animation colladaAnimation, Animation animation, string indent)
        {
            // Loop through the channels in the animation
            foreach (COLLADA.Channel channel in colladaAnimation.Channels)
            {
                // Find the target of the channel
                // --------------------------------

                // TODO(P2): This should really use the full COLLADA Addressing scheme to find the object being animated..
                //   but for now, we hard code the animation addressing to support "NODE/Matrix"

                // Split the address path into the Id & Child element  (and make sure we have both)
                string[] targetParts = channel.Target.Split('/');
                if (targetParts.Length != 2)
                    return;

                // Make sure the target is pointing at a collada node
                COLLADA.Node colladaNode = this.Source.GetObjectByUrl("#" + targetParts[0]) as COLLADA.Node; 
                if(colladaNode == null)
                    return; // The channel does not target a node

                // Make sure the target points at a matrix transform within the node
                COLLADA.TransformBase transform = colladaNode.Transforms.First(n => n.Sid == targetParts[1]);
                if (transform == null || (transform is COLLADA.Matrix == false))
                    return; // This animation is not applied to a node matrix

                // Find the Node referenced by the address ID
                Node node = this.ResultModel.RootNode.FindNodeByColladaId(targetParts[0]);
                if (node == null)
                    throw new BuildException("Animation references a node that was not processed.");

                // Create a node animation for this node
                NodeAnimation nodeAnimation = new NodeAnimation();
                nodeAnimation.Node = node;

                // Process the Channel Source
                // ------------------------------

                // Get the source
                COLLADA.Sampler sampler = this.Source.GetObjectByUrl(channel.Source) as COLLADA.Sampler;
                if (sampler == null)
                    throw new Exception("Animation channel references an unknown [sampler@id=\"" + channel.Source + "\"]");

                // Get the 3 sources..  INPUT / OUTPUT / INTERPOLATION
                string inputSourceUrl = sampler.Inputs.Single(n => n.Semantic == "INPUT").Source;
                string outputSourceUrl = sampler.Inputs.Single(n => n.Semantic == "OUTPUT").Source;
                string interpolationSourceUrl = sampler.Inputs.Single(n => n.Semantic == "INTERPOLATION").Source;

                COLLADA.Source inputSource = this.Source.GetObjectByUrl(inputSourceUrl) as COLLADA.Source;
                COLLADA.Source outputSource = this.Source.GetObjectByUrl(outputSourceUrl) as COLLADA.Source;
                COLLADA.Source interpolationSource = this.Source.GetObjectByUrl(interpolationSourceUrl) as COLLADA.Source;

                // Get the input & output float values
                List<float> inputValues = (inputSource.ArrayData as COLLADA.FloatArray).Values;
                List<float> outputValues = (outputSource.ArrayData as COLLADA.FloatArray).Values;

                // Loop through the keyframes
                for(int index=0; index < inputValues.Count; index++)
                {
                    // Create this node animation keyframe
                    TransformKeyFrame keyFrame = new TransformKeyFrame();
                    nodeAnimation.KeyFrames.Add(keyFrame);

                    // Set the keyframe time
                    keyFrame.Time = inputValues[index];

                    // Extract the output TRANSFORM matrix
                    List<float> matrixValues = outputValues.GetRange(index * 16, 16);
                    Matrix transformMatrix = new Gdk.Matrix(
                        matrixValues[0], matrixValues[4], matrixValues[8], matrixValues[12],
                        matrixValues[1], matrixValues[5], matrixValues[9], matrixValues[13],
                        matrixValues[2], matrixValues[6], matrixValues[10], matrixValues[14],
                        matrixValues[3], matrixValues[7], matrixValues[11], matrixValues[15]
                        );

                    // Decompose the output transform into the keyframe
                    transformMatrix.Decompose(out keyFrame.Scale, out keyFrame.Rotation, out keyFrame.Translation);
                }

                // Did the node have any keyframes?
                if(nodeAnimation.KeyFrames.Count > 0)
                {
                    // Put this node animation into the animation
                    animation.NodeAnimations.Add(nodeAnimation);

                    Context.LogInfo(" +---" + indent + " Node [" + node.Name + "] [Keyframes: " + nodeAnimation.KeyFrames.Count + "]");
                }
            }
        }

        /// <summary>
        /// Processes a COLLADA.Node object
        /// </summary>
        public void ProcessColladaNode(COLLADA.Node colladaNode, Node parentNode, Gdk.Matrix parentTransform, string logIndent)
        {
            Context.LogInfo(" +-" + logIndent + " Node: " + colladaNode.Id);

            // Get the local & full transform for this node
            Gdk.Matrix localTransform = COLLADA.TransformBase.GetCombinedTransforms(colladaNode.Transforms);
            Gdk.Matrix absoluteTransform = localTransform * parentTransform;

            // Create the Model.Node for this node
            Node node = new Node();
            node.PoseTransform = localTransform;
            node.ColladaSID = colladaNode.Sid;
            node.ColladaID = colladaNode.Id;

            // Get the node name
            node.Name = colladaNode.Name;
            if (string.IsNullOrEmpty(node.Name))
                node.Name = node.ColladaID;

            // Add this node to the model
            node.ParentNode = parentNode;
            if(parentNode != null)
                parentNode.ChildNodes.Add(node);
            this.ResultModel.Nodes.Add(node);

            // Loop though the instanced geometry on this node
            foreach (COLLADA.InstanceGeometry geometryInstance in colladaNode.GeometryInstances)
            {
                // Process this geometry instance
                ProcessColladaInstanceGeometry(geometryInstance, node, logIndent);
            }

            // Loop though the instanced controllers on this node
            foreach (COLLADA.InstanceController controllerInstance in colladaNode.ControllerInstances)
            {
                // Process this controller instance
                ProcessColladaInstanceController(controllerInstance, node, logIndent);
            }

            // Loop though the instanced nodes on this node
            foreach (COLLADA.InstanceNode nodeInstance in colladaNode.NodeInstances)
            {
                // Process this controller instance
                ProcessColladaInstanceNode(nodeInstance, node, absoluteTransform, logIndent);
            }

            // Loop through the child nodes
            foreach (COLLADA.Node colladaChildNode in colladaNode.ChildNodes)
            {
                // Recurse process this node
                ProcessColladaNode(colladaChildNode, node, absoluteTransform, logIndent + "--");
            }
        }

        /// <summary>
        /// Processes a COLLADA.InstanceNode object, at the given node
        /// </summary>
        public void ProcessColladaInstanceNode(COLLADA.InstanceNode nodeInstance, Node parentNode, Gdk.Matrix parentTransform, string logIndent)
        {
            // Get the node that is being instanced
            COLLADA.Node colladaNode = this.Source.GetObjectByUrl(nodeInstance.Url) as COLLADA.Node;
            if (colladaNode == null)
                throw new BuildException("Unable to instance a node at the url \"" + nodeInstance.Url + "\", node not found");

            // Process the collada node
            ProcessColladaNode(colladaNode, parentNode, parentTransform, logIndent + "--");
        }

        /// <summary>
        /// Processes a COLLADA.InstanceController object, at the given node
        /// </summary>
        public void ProcessColladaInstanceController(COLLADA.InstanceController controllerInstance, Node node, string logIndent)
        {
            // Get the controller that is being referenced
            COLLADA.Controller controller = this.Source.GetObjectByUrl(controllerInstance.Url) as COLLADA.Controller;
            if (controller == null)
                throw new BuildException("Unable to instance a controller at the url \"" + controllerInstance.Url + "\", controller not found");

            // Skin controllers
            // ============================================
            
            if(controller.Skin != null)
            {
                // Get the Mesh that was built from this skin controller
                Mesh mesh = this.MeshesByColladaId[controller.Id];
                if (mesh == null)
                    throw new BuildException("Unable to instance a skin controller, no mesh was processed for the skin source [mesh.id=\"" + controller.Skin.Source + "\"]");
 
                // Create a skeletal mesh instance for this skin controller instance
                MeshInstance meshInstance = new MeshInstance();
                meshInstance.Mesh = mesh;
                meshInstance.Node = node;
                this.ResultModel.MeshInstances.Add(meshInstance);

                // Build a map of all the nodes used by this skeleton, by node SID
                // ---------------------------------------------------------------------------

                Dictionary<string, Node> SkeletonNodesBySID = new Dictionary<string, Node>();

                // Is this node using specific skeleton roots?
                if (controllerInstance.Skeletons.Count > 0)
                {
                    // Loop through the skeletons used by this instance
                    foreach (COLLADA.Skeleton skeleton in controllerInstance.Skeletons)
                    {
                        // Get the node at the root of this skeleton
                        string skeletonNodeId = skeleton.URI;
                        if (skeletonNodeId.StartsWith("#"))
                            skeletonNodeId = skeletonNodeId.Substring(1);
                        Node skeletonRootNode = this.ResultModel.RootNode.FindNodeByColladaId(skeletonNodeId);

                        if(skeletonRootNode == null)
                            throw new BuildException("Unable to find the node \"" + skeletonNodeId + "\" referenced in an [instance_controller/skeleton] element");

                        // Add all the nodes with a SID to our dictionary
                        RecurseCollectNodesBySID(skeletonRootNode, SkeletonNodesBySID);
                    }
                }
                else
                {
                    // Add all the nodes with a SID to our dictionary
                    RecurseCollectNodesBySID(this.ResultModel.RootNode, SkeletonNodesBySID);
                }

                // Assign each joint to a node using the SkeletalMesh's Joint SID mappings
                // ---------------------------------------------------------------------------

                // Loop through the joint SIDs
                foreach(string jointSID in mesh.JointSIDs)
                {
                    // Find the node with this SID
                    Node jointNode = SkeletonNodesBySID[jointSID];

                    // Assign this node to this joint index in the skeletal mesh instance
                    meshInstance.JointToNodeMap.Add(jointNode);

                    // Flag the node as InUse
                    jointNode.InUse = true;
                }

                // Load the Material Bindings for the instance
                // ------------------------------------------------

                // Does the instance have any material bindings?
                if (controllerInstance.BindMaterial != null)
                {
                    // Process the material bindings for this instance
                    foreach (COLLADA.InstanceMaterial materialInstance in controllerInstance.BindMaterial.MaterialInstances)
                    {
                        // Does the mesh have this material symbol?
                        if (mesh.TrianglesByMaterialSymbol.ContainsKey(materialInstance.Symbol) == false)
                        {
                            Context.LogWarn("An [instance_controller] in the node id=\"" + node.Name + "\" contains a material binding for a material symbol \"" + materialInstance.Symbol + "\" that doesnt exist on the mesh.  (this is usually safe to ignore)");
                            continue;
                        }

                        // Get the material that is targetted by this binding
                        string materialId = materialInstance.Target;
                        if (materialId.StartsWith("#"))
                            materialId = materialId.Substring(1);
                        Material material;
                        if (this.MaterialsByColladaId.TryGetValue(materialId, out material) == false)
                        {
                            throw new BuildException("The node id=\"" + node.Name + "\" contains a controller instance binding to an unknown material \"" + materialId + "\"");
                        }

                        // Add this binding to the mesh instance
                        meshInstance.MaterialBindings.Add(materialInstance.Symbol, material);
                    }
                }

                // If the mesh has an unassigned material symbol, bind the default material to it
                if (mesh.TrianglesByMaterialSymbol.ContainsKey("GDK-Unassigned"))
                {
                    Material defaultMaterial = this.MaterialsByColladaId["GDK-Default-Material"];
                    meshInstance.MaterialBindings.Add("GDK-Unassigned", defaultMaterial);
                }

                // Verify the instance has a material binding for every material symbol in the mesh
                foreach (string materialSymbol in mesh.TrianglesByMaterialSymbol.Keys)
                    if (meshInstance.MaterialBindings.Keys.Contains(materialSymbol) == false)
                        throw new BuildException("An [instance_controller] for the controller \"" + controller.Id + "\"] does not contains a binding for the material symbol \"" + materialSymbol + "\"");

                if (mesh.TrianglesByMaterialSymbol.Count != meshInstance.MaterialBindings.Count)
                    throw new BuildException("An [instance_controller] for the controller \"" + controller.Id + "\"] does not have a matching number of material bindings.  [Expected:" + mesh.TrianglesByMaterialSymbol.Count + "] [Actual:" + meshInstance.MaterialBindings.Count + "]");

                // -------------------------------------

                // Log the skeletal mesh instance
                Context.LogInfo(" +---" + logIndent + " Mesh Instance (Skinned): " + mesh.ColladaID);

                // Set the node as 'inUse'
                node.InUse = true;
            }

            // ============================================
        }

        /// <summary>
        /// Recursively adds the node & all its children to the dictionary (by their SID)
        /// </summary>
        /// <param name="node"></param>
        /// <param name="nodesBySID"></param>
        public void RecurseCollectNodesBySID(Node node, Dictionary<string, Node> nodesBySID)
        {
            // If this node has a Sid, add it to the dictionary
            if (string.IsNullOrEmpty(node.ColladaSID) == false)
                nodesBySID.Add(node.ColladaSID, node);

            // Recurse to the child nodes
            foreach (Node childNode in node.ChildNodes)
                RecurseCollectNodesBySID(childNode, nodesBySID);
        }

        /// <summary>
        /// Processes a COLLADA.InstanceGeometry object, at the given node
        /// </summary>
        public void ProcessColladaInstanceGeometry(COLLADA.InstanceGeometry geometryInstance, Node node, string logIndent)
        {
            // Get the Mesh this instance is using
            string meshId = geometryInstance.Url;
            if (meshId.StartsWith("#"))
                meshId = meshId.Substring(1);
            Mesh mesh;
            if (this.MeshesByColladaId.TryGetValue(meshId, out mesh) == false)
            {
                Context.LogWarn("The node id=\"" + node.Name + "\" contains an instance of an unknown/unrecognized mesh \"" + meshId + "\"");
                return;
            }

            // Create the GDK MeshInstance
            MeshInstance meshInstance = new MeshInstance();
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
                    if (mesh.TrianglesByMaterialSymbol.ContainsKey(materialInstance.Symbol) == false)
                    {
                        Context.LogWarn("An [instance_geometry] in the node id=\"" + node.Name + "\" contains a material binding for a material symbol \"" + materialInstance.Symbol + "\" that doesnt exist on the mesh.  (this is usually safe to ignore)");
                        continue;
                    }

                    // Get the material that is targetted by this binding
                    string materialId = materialInstance.Target;
                    if (materialId.StartsWith("#"))
                        materialId = materialId.Substring(1);
                    Material material;
                    if (this.MaterialsByColladaId.TryGetValue(materialId, out material) == false)
                    {
                        throw new BuildException("The node id=\"" + node.Name + "\" contains a geometry instance binding to an unknown material \"" + materialId + "\"");
                    }

                    // Add this binding to the mesh instance
                    meshInstance.MaterialBindings.Add(materialInstance.Symbol, material);
                }
            }

            // If the mesh has an unassigned material symbol, bind the default material to it
            if (mesh.TrianglesByMaterialSymbol.ContainsKey("GDK-Unassigned"))
            {
                Material defaultMaterial = this.MaterialsByColladaId["GDK-Default-Material"];
                meshInstance.MaterialBindings.Add("GDK-Unassigned", defaultMaterial);
            }

            // Verify the instance has a material binding for every material symbol in the mesh
            foreach (string materialSymbol in mesh.TrianglesByMaterialSymbol.Keys)
                if (meshInstance.MaterialBindings.Keys.Contains(materialSymbol) == false)
                    throw new BuildException("An [instance_geometry] for the geometry \"" + meshId + "\"] does not contains a binding for the material symbol \"" + materialSymbol + "\"");

            if (mesh.TrianglesByMaterialSymbol.Count != meshInstance.MaterialBindings.Count)
                throw new BuildException("An [instance_geometry] for the geometry \"" + meshId + "\"] does not have a matching number of material bindings.  [Expected:" + mesh.TrianglesByMaterialSymbol.Count + "] [Actual:" + meshInstance.MaterialBindings.Count + "]");

            // Log the mesh instance
            Context.LogInfo(" +---" + logIndent + " Mesh Instance: " + mesh.ColladaID);

            // Set the node as 'inUse'
            node.InUse = true;
        }

        /// <summary>
        /// Processes a COLLADA.Controller object
        /// </summary>
        public void ProcessColladaController(COLLADA.Controller controller)
        {
            // Handle the controller based on it's type

            // Skin Controller
            // ===============================================

            if (controller.Type == COLLADA.ControllerType.Skin)
            {
                Context.LogInfo(" +- Skin: " + controller.Id);

                // Clone the source mesh
                // -------------------------------------------

                // Get the source mesh this controller uses
                COLLADA.Geometry skinSourceGeometry = this.Source.GetObjectByUrl(controller.Skin.Source) as COLLADA.Geometry;
                Mesh sourceMesh = this.MeshesByColladaId[skinSourceGeometry.Id];

                // Create a skeletal mesh 
                Mesh skinnedMesh = new Mesh();
                skinnedMesh.ColladaID = controller.Id;
                skinnedMesh.Name = controller.Name;
                if (string.IsNullOrEmpty(skinnedMesh.Name) == true)
                    skinnedMesh.Name = controller.Id;

                // Clone the source mesh flags & primitives
                skinnedMesh.Flags = sourceMesh.Flags;
                foreach (KeyValuePair<string, List<Triangle>> triangleBlobEntry in sourceMesh.TrianglesByMaterialSymbol)
                {
                    // Clone the triangles assigned to this material
                    List<Triangle> clonedTriangleBlob = new List<Triangle>();
                    foreach(Triangle sourceTri in triangleBlobEntry.Value)
                        clonedTriangleBlob.Add(new Triangle(sourceTri));
                    skinnedMesh.TrianglesByMaterialSymbol.Add(triangleBlobEntry.Key, clonedTriangleBlob);
                } 

                // Add the new skinned mesh to the processed set
                this.MeshesByColladaId.Add(skinnedMesh.ColladaID, skinnedMesh);

                // Add the mesh to the model
                this.ResultModel.Meshes.Add(skinnedMesh);

                Context.LogVerbose(" +--- Source Mesh: " + sourceMesh.ColladaID);

                // Load the joint data for this skeletal mesh
                // -------------------------------------------

                // Get the skin's joint inputs for the JOINT & INV_BIND_MATRIX semantics
                COLLADA.Input jointInput = controller.Skin.Joints.Inputs.Single(n => n.Semantic == "JOINT");
                COLLADA.Input invBindMatrixInput = controller.Skin.Joints.Inputs.Single(n => n.Semantic == "INV_BIND_MATRIX");

                // Get the source arrays for the joint data
                COLLADA.Source jointSource = this.Source.GetObjectByUrl(jointInput.Source) as COLLADA.Source;
                COLLADA.Source invBindMatrixSource = this.Source.GetObjectByUrl(invBindMatrixInput.Source) as COLLADA.Source;

                // Loop through the joints used by this skin
                skinnedMesh.NumJoints = (UInt16)jointSource.ArrayData.Count;
                for(int jointIndex = 0; jointIndex < jointSource.ArrayData.Count; jointIndex++)
                {
                    // Get the SID for this joint
                    string jointSID = jointSource.ArrayData.GetValue(jointIndex) as string;
                    skinnedMesh.JointSIDs.Add(jointSID); ;

                    // Get the InvBindMatrix for this joint
                    List<float> ibmValues = (invBindMatrixSource.ArrayData as COLLADA.FloatArray).Values;
                    int offset = jointIndex * 16;
                    Matrix jointInvBindMatrix = new Matrix(
                        ibmValues[offset + 0], ibmValues[offset + 4], ibmValues[offset + 8], ibmValues[offset + 12],
                        ibmValues[offset + 1], ibmValues[offset + 5], ibmValues[offset + 9], ibmValues[offset + 13],
                        ibmValues[offset + 2], ibmValues[offset + 6], ibmValues[offset + 10], ibmValues[offset + 14],
                        ibmValues[offset + 3], ibmValues[offset + 7], ibmValues[offset + 11], ibmValues[offset + 15]
                        );
                    skinnedMesh.JointInvBindMatrices.Add(jointInvBindMatrix);

                }

                Context.LogVerbose(" +--- Joints: " + skinnedMesh.NumJoints);

                // Load the vertex bone indices & weights from the collada skin
                // -------------------------------------------------------

                int sampleIndex = 0;
                List<SkinVertexData> skinVertices = new List<SkinVertexData>();

                int highestSingleVertexBoneCount = 0;
                float[] verticesBonesCounter = new float[16];   // Keeps a count of the number of vertices that use 1, 2, 3, + bones

                // Loop through the vertices in the collada skin
                for(int vertexIndex = 0; vertexIndex < controller.Skin.VertexWeights.Count; vertexIndex++)
                {
                    // Create a VertexBoneData for this vertex
                    SkinVertexData skinVertexData = new SkinVertexData();
                    skinVertices.Add(skinVertexData);

                    // Get the number of bones that affect this vertex
                    skinVertexData.NumBones = controller.Skin.VertexWeights.VCount[vertexIndex];

                    // Update the bones per vertex counter & track the highest number of bones per single vertex
                    verticesBonesCounter[skinVertexData.NumBones]++;
                    if(skinVertexData.NumBones > highestSingleVertexBoneCount)
                        highestSingleVertexBoneCount = skinVertexData.NumBones;

                    // Loop through the number of joints affecting this vertex
                    for (int affectorIndex = 0; affectorIndex < skinVertexData.NumBones; affectorIndex++)
                    {
                        // Sample the named data of this vertex affector from the source/accessor/inputs model of the skin
                        Dictionary<string, object> namedVertexData = new Dictionary<string, object>();
                        sampleIndex += SampleColladaSharedInputs(sampleIndex, controller.Skin.VertexWeights.V, controller.Skin.VertexWeights.Inputs, namedVertexData, "");

                        // Parse the joint index & weight into the vertex bone data struct
                        int jointIndex = (int)namedVertexData["JOINT.INDEX"];
                        float weight = GetObjectAsFloat(namedVertexData.First(n => n.Key.StartsWith("WEIGHT")).Value);
                        skinVertexData.Indices.Add(jointIndex);
                        skinVertexData.Weights.Add(weight);

                        // For now, we dont support -1 bone indices (which means no skinning matrix)
                        //   This could be implmented in the future by pushing all bone indices out by 1 & making bone0 = identity...
                        if(jointIndex < 0)
                            throw new BuildException("Skinning joint indices of -1 are not supported.  Please bind all of you skinned mesh vertices to a bone structure");
                    }
                }

                // Convert the skinning data to consistent 1, 2, or 4 bone indices
                // -----------------------------------------------------------------------

                // TODO: Determine the type of skinning to use
                //   For now, everything forces 4-Bone skinning

                int fixedBonesPerVertex = 4;

                // Loop through the skinning vertex data
                for(int vertexIndex = 0; vertexIndex < skinVertices.Count; vertexIndex++)
                {
                    SkinVertexData skinVertex = skinVertices[vertexIndex];

                    // Does this vertex have MORE bones than we need?
                    while(skinVertex.NumBones > fixedBonesPerVertex)
                    {
                        // Find the bone with the weakest weight
                        float weakestWeight = 10000.0f;
                        int weakestBoneIndex = 0;
                        for(int boneIndex=0; boneIndex < skinVertex.NumBones; boneIndex++)
                        {
                            // Is this the weaker bone?
                            if(skinVertex.Weights[boneIndex] <= weakestWeight)
                            {
                                weakestWeight = skinVertex.Weights[boneIndex];
                                weakestBoneIndex = boneIndex;
                            }
                        }

                        // Remove the weak bone
                        skinVertex.Indices.RemoveAt(weakestBoneIndex);
                        skinVertex.Weights.RemoveAt(weakestBoneIndex);
                        skinVertex.NumBones--;
                    }

                    // Does this vertex have LESS bones than we need?
                    while(skinVertex.NumBones < fixedBonesPerVertex)
                    {
                        // Lets add a new bone with matrix 0 & weight 0
                        skinVertex.Indices.Add(0);
                        skinVertex.Weights.Add(0.0f);
                        skinVertex.NumBones++;
                    }
                }

                Context.LogVerbose(" +--- Joints Per Vertex: " + fixedBonesPerVertex);

                // Normalize the bone weights
                // ------------------------------------------

                // Loop through the skinning vertex data
                bool didWeightNormalization = false;
                for(int vertexIndex = 0; vertexIndex < skinVertices.Count; vertexIndex++)
                {
                    SkinVertexData skinVertex = skinVertices[vertexIndex];

                    // Get the total of all the bone weights in this vertex
                    float totalWeight = 0.0f;
                    for(int boneIndex=0; boneIndex < skinVertex.NumBones; boneIndex++)
                    {
                        totalWeight += skinVertex.Weights[boneIndex];
                        if (totalWeight != 1.0f)
                            didWeightNormalization = true;
                    }

                    // If the total weight isnt 0, then normalize all the weights to a total weight of 1.0
                    if(totalWeight != 0.0f)
                    {
                        for(int boneIndex=0; boneIndex < skinVertex.NumBones; boneIndex++)
                        {
                            skinVertex.Weights[boneIndex] /= totalWeight;
                        }
                    }
                }

                // Log if we normalized the weights
                if(didWeightNormalization == true)
                    Context.LogVerbose(" +--- Joint Weights Normalized");

                // Apply the skin vertex data to the mesh
                // --------------------------------------------

                // Set the mesh flags for this type of skinning
                switch(fixedBonesPerVertex)
                {
                    case 1: skinnedMesh.Flags |= MeshFlags.VertexHasSingleBone; break;
                    case 2: skinnedMesh.Flags |= MeshFlags.VertexHas2WeightedBones; break;
                    case 4: skinnedMesh.Flags |= MeshFlags.VertexHas4WeightedBones; break;
                    default: throw new BuildException("Skeletal mesh processing is using an unsupported number of bones per vertex: " + fixedBonesPerVertex);
                }

                // Loop through the triangle sets in the mesh
                foreach (List<Triangle> triangleSet in skinnedMesh.TrianglesByMaterialSymbol.Values)
                {
                    // Loop through the triangles in this se
                    foreach (Triangle triangle in triangleSet)
                    {
                        // Loop through the vertices in this triangle
                        foreach (Vertex vertex in triangle.Vertices)
                        {
                            // Lookup the skin vertex data for this vertex
                            SkinVertexData skinVertex = skinVertices[vertex.Index];

                            // Copy the bone indices & weights to this vertex
                            for(int boneIndex=0; boneIndex < fixedBonesPerVertex; boneIndex++)
                            {
                                vertex.BoneIndices[boneIndex] = (byte)skinVertex.Indices[boneIndex];
                                vertex.BoneWeights[boneIndex] = skinVertex.Weights[boneIndex];
                            }
                        }
                    }
                }
            }

            // Morph Controller
            // ===============================================

            if (controller.Type == COLLADA.ControllerType.Morph)
            {
                // Currently Unsupported
                Context.LogWarn("Morph controller detected \"" + controller.Id + "\".  Skipping processing as morph controllers are unsupported");
            }
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
                Mesh mesh = new Mesh();

                // Copy the Id
                mesh.ColladaID = colladaGeometry.Id;
                mesh.Name = colladaGeometry.Name;
                if (string.IsNullOrEmpty(mesh.Name))
                    mesh.Name = colladaGeometry.Id;

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
                    throw new BuildException("The mesh \"" + mesh.ColladaID + "\" contains a <trifans> element, which isnt currently supported");
                if (colladaGeometry.Mesh.TriStripsSets.Count > 0)
                    throw new BuildException("The mesh \"" + mesh.ColladaID + "\" contains a <tristrips> element, which isnt currently supported");

                // Did we not find any triangles?
                if(mesh.TrianglesByMaterialSymbol.Count == 0)
                {
                    Context.LogWarn(" +--- The mesh \"" + mesh.ColladaID + "\" contains no triangle data, skipping");
                    return;
                }

                // Determine the mesh vertex format
                // ----------------------------------------------------

                // Get the first vertex in the first triangle of the first triangle soup :D
                Vertex firstVertex = mesh.TrianglesByMaterialSymbol.First().Value[0].Vertices[0];

                // Verify every vertex within this mesh uses the exact same vertex data format
                foreach (List<Triangle> triangleSet in mesh.TrianglesByMaterialSymbol.Values)
                    foreach (Triangle triangle in triangleSet)
                        foreach (Vertex vertex in triangle.Vertices)
                            if (firstVertex.Flags != vertex.Flags)
                                throw new BuildException("The geometry mesh \"" + colladaGeometry.Id + "\" contains mixed vertex types, which is currently un-supported by the GDK ModelProcessor");
                
                // Set the mesh flags
                mesh.Flags = firstVertex.Flags;

                // Log the mesh data
                // ----------------------------------------------------

                // Vertex Format
                string vertexFormat = "P3 N3 ";
                if ((mesh.Flags & Gdk.Content.Model.MeshFlags.VertexHasColor) > 0)
                    vertexFormat += "C4 ";
                if ((mesh.Flags & Gdk.Content.Model.MeshFlags.VertexHasTexCoords) > 0)
                    vertexFormat += "TC2a ";
                if ((mesh.Flags & Gdk.Content.Model.MeshFlags.VertexHasTexCoords2) > 0)
                    vertexFormat += "TC2b ";

                Context.LogVerbose(" +--- Vertex Format: " + vertexFormat);

                // Log the triangle soup data
                foreach (KeyValuePair<string, List<Triangle>> trianglesByMaterial in mesh.TrianglesByMaterialSymbol)
                {
                    Context.LogVerbose(" +--- Triangle Soup:  [Material: " + trianglesByMaterial.Key + "] [#Triangles: " + trianglesByMaterial.Value.Count + "]");
                }

                // Add the mesh to the processed set
                this.MeshesByColladaId.Add(mesh.ColladaID, mesh);

                // Add the mesh to the model
                this.ResultModel.Meshes.Add(mesh);
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
            if (this.MaterialsByColladaId.ContainsKey("GDK-Default-Material") == false)
            {
                // Create the default material
                Material defaultMaterial = new Material();
                defaultMaterial.Name = "GDK-Default-Material";
                defaultMaterial.ColladaID = "GDK-Default-Material";
                defaultMaterial.Ambient = Context.Parameters.GetValue("Default Ambient", Color.Black);
                defaultMaterial.Diffuse = Context.Parameters.GetValue("Default Diffuse", new Color(196, 0, 196));
                defaultMaterial.Specular = Context.Parameters.GetValue("Default Specular", Color.White);
                defaultMaterial.Shininess = Context.Parameters.GetValue("Default Shininess", 0.0f);
                defaultMaterial.Emissive = Context.Parameters.GetValue("Default Emissive", Color.Black);
                defaultMaterial.Flags = 0;

                // Add the default material to the model
                this.MaterialsByColladaId.Add(defaultMaterial.ColladaID, defaultMaterial);
                this.ResultModel.Materials.Add(defaultMaterial);
            }
        }

        /// <summary>
        /// Gets the triangle soup for the given material symbol, from the mesh.  If no soup exists for the given
        /// material, a new one is created and added to the mesh
        /// </summary>
        /// <param name="mesh"></param>
        List<Triangle> GetTriangleSetForMaterialSymbol(Mesh mesh, string materialSymbol)
        {
            // If the material symbol is blank, use the builtin 'GDK-No-Material' material
            if (string.IsNullOrEmpty(materialSymbol))
            {
                Context.LogWarn("The mesh[id=\"" + mesh.ColladaID + "\"] does not have a material symbol, defaulting to the \"GDK-Unassigned\" symbol");
                materialSymbol = "GDK-Unassigned";

                // Ensure the default material exists
                EnsureDefaultMaterialExists();
            }

            // Does the mesh have an existing triangle soup for this material symbol?
            if (mesh.TrianglesByMaterialSymbol.ContainsKey(materialSymbol) == true)
            {
                // Use this soup
                return mesh.TrianglesByMaterialSymbol[materialSymbol];
            }
            
            // Create a new soup for this material symbol
            List<Triangle> newTriangleSet = new List<Triangle>();
            mesh.TrianglesByMaterialSymbol.Add(materialSymbol, newTriangleSet);

            return newTriangleSet;
        }

        /// <summary>
        /// Processes a single COLLADA <triangles/> element and adds the triangle/vertex data to a Mesh
        /// </summary>
        void ProcessColladaTriangles(COLLADA.Triangles triangles, Mesh mesh)
        {
            // Get a triangle soup in the mesh for this <triangles>'s material symbol
            List<Triangle> trianglesSet = GetTriangleSetForMaterialSymbol(mesh, triangles.Material);

            // Loop through the primitives sets (<p> elements)
            foreach(COLLADA.IndexSet indexSet in triangles.PrimitiveIndexSets)
            {
                int vertexSourceIndex = 0;

                // Loop through the triangles
                for (int triangleIndex = 0; triangleIndex < triangles.Count; triangleIndex++)
                {
                    // Create a triangle in the soup
                    Triangle triangle = new Triangle();
                    trianglesSet.Add(triangle);

                    // Loop through the 3 vertices of this triangle
                    for (int i = 0; i < 3; i++)
                    {
                        // Sample the named data of this vertex from the source/accessor/inputs model of the face group
                        Dictionary<string, object> namedVertexData = new Dictionary<string,object>();
                        vertexSourceIndex += SampleColladaSharedInputs(vertexSourceIndex, indexSet, triangles.Inputs, namedVertexData, string.Empty);

                        // Parse the named data into a Model Vertex
                        Vertex vertex = BuildModelVertex(namedVertexData);

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
        void ProcessColladaPolyList(COLLADA.PolyList polylist, Mesh mesh)
        {
            // Get a triangle soup in the mesh for this <polylist>'s material symbol
            List<Triangle> trianglesSet = GetTriangleSetForMaterialSymbol(mesh, polylist.Material);

            // Verify the poly list has polygon vertices
            if (polylist.PrimitiveIndexSets.Count != 1)
                throw new BuildException("The mesh[id=\"" + mesh.ColladaID + "\"]/polylist contains an invalid number of [p] child elements (should be 1)");

            int vertexSourceIndex = 0;

            // Get the index set for the polylist (<p>)
            COLLADA.IndexSet indexSet = polylist.PrimitiveIndexSets[0];

            // Loop through the poly vertex counts
            foreach (int polyVertexCount in polylist.VCounts)
            {
                // Make sure this poly has at least 3 vertices
                if (polyVertexCount < 3)
                    throw new BuildException("The mesh[id=\"" + mesh.ColladaID + "\"]/polylist contains a poly with less than 3 vertices");

                // Sample the first vertex
                Dictionary<string, object> namedVertexData1 = new Dictionary<string, object>();
                vertexSourceIndex += SampleColladaSharedInputs(vertexSourceIndex, indexSet, polylist.Inputs, namedVertexData1, string.Empty);
                Vertex vertex1 = BuildModelVertex(namedVertexData1);

                // Sample the 3rd vertex
                Dictionary<string, object> namedVertexData3 = new Dictionary<string, object>();
                vertexSourceIndex += SampleColladaSharedInputs(vertexSourceIndex, indexSet, polylist.Inputs, namedVertexData3, string.Empty);
                Vertex vertex3 = BuildModelVertex(namedVertexData3);
                Vertex vertex2;

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
                    Triangle triangle = new Triangle();
                    triangle.Vertices[0] = vertex1;
                    triangle.Vertices[1] = vertex2;
                    triangle.Vertices[2] = vertex3;
                    trianglesSet.Add(triangle);
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

                // For joint semantics, we also add the joint index
                if (sharedInput.Semantic == "JOINT")
                {
                    results.Add("JOINT.INDEX", sourceIndex);
                }

                // Is this a custom lookup semantic?
                if (sharedInput.Semantic == "VERTEX")
                {
                    // Add the vertex index to the results
                    results.Add("VERTEX.INDEX", sourceIndex);

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

                    // Make sure the index is valid before doing the lookup
                    if (sourceIndex >= 0 && sourceIndex < source.ArrayData.Count)
                    {
                        // Sample the source
                        SampleColladaSource(sourceIndex, source, results, prefix + sharedInput.Semantic + ".");
                    }
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
        public Vertex BuildModelVertex(Dictionary<string, object> namedVertexData)
        {
            // Create a default vertex
            Vertex vertex = new Vertex();

            // Loop through the vertex data items
            foreach (KeyValuePair<string, object> dataItem in namedVertexData)
            {
                switch (dataItem.Key)
                {
                    // Vertex Index
                    case "VERTEX.INDEX": vertex.Index = (int)dataItem.Value; break;

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
                        vertex.Flags |= MeshFlags.VertexHasTexCoords;
                        break;
                    case "TEXCOORD.T": 
                        vertex.TexCoords1.Y = GetObjectAsFloat(dataItem.Value);
                        vertex.Flags |= MeshFlags.VertexHasTexCoords;

                        // Collada texture coordinates are based on 0,0 = bottom left
                        // GDK's are based on 0,0 = top left
                        // so let's fix them :D
                        vertex.TexCoords1.Y = 1.0f - vertex.TexCoords1.Y;

                        break;

                    // Color
                    case "COLOR.R":
                        vertex.Color.R = (byte)(Math.Max(Math.Min(GetObjectAsFloat(dataItem.Value), 1.0f), 0.0f) * 255.0f);
                        vertex.Flags |= MeshFlags.VertexHasColor;
                        break;
                    case "COLOR.G":
                        vertex.Color.G = (byte)(Math.Max(Math.Min(GetObjectAsFloat(dataItem.Value), 1.0f), 0.0f) * 255.0f);
                        vertex.Flags |= MeshFlags.VertexHasColor;
                        break;
                    case "COLOR.B":
                        vertex.Color.B = (byte)(Math.Max(Math.Min(GetObjectAsFloat(dataItem.Value), 1.0f), 0.0f) * 255.0f);
                        vertex.Flags |= MeshFlags.VertexHasColor;
                        break;
                    case "COLOR.A":
                        vertex.Color.A = (byte)(Math.Max(Math.Min(GetObjectAsFloat(dataItem.Value), 1.0f), 0.0f) * 255.0f);
                        vertex.Flags |= MeshFlags.VertexHasColor;
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
            Material material = new Material();

            // Copy the Id
            material.ColladaID = colladaMaterial.Id;

            // Get the material name
            material.Name = colladaMaterial.Name;
            if (string.IsNullOrEmpty(material.Name))
                material.ColladaID = colladaMaterial.Id;

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
                material.Flags |= MaterialFlags.DiffuseTextured;

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
            this.MaterialsByColladaId.Add(material.ColladaID, material);
            this.ResultModel.Materials.Add(material);
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
            /*
            bool removeUnusedMeshes = Context.Parameters.GetValue("Remove Unused Meshes", true);
            if (removeUnusedMeshes)
            {
                // Meshes
                // ------------------------

                Context.LogInfo(" - Checking for unused meshes");

                // Loop through the meshes by id
                List<string> meshIds = new List<string>(this.MeshesByColladaId.Keys);
                foreach (string meshId in meshIds)
                {
                    // Get this mesh
                    Mesh mesh = this.ResultModel.MeshesByColladaId[meshId];

                    // Are there any instances of this mesh?
                    bool instanceFound = false;
                    foreach (MeshInstance meshInstance in this.ResultModel.MeshInstances)
                        if (meshInstance.Mesh == mesh)
                        {
                            instanceFound = true;
                            break;
                        }

                    // Did we not find any instances of this mesh?
                    if (instanceFound == false)
                    {
                        // Remove the mesh from the model
                        this.MeshesByColladaId.Remove(meshId);
                        this.ResultModel.Meshes.Remove(mesh);
                        Context.LogInfo(" +- Removing unused mesh: \"" + meshId + "\"");
                    }
                }

            // Remove unused materials
            // =================================

            bool removeUnusedMaterials = Context.Parameters.GetValue("Remove Unused Materials", true);
            if (removeUnusedMaterials)
            {
                Context.LogInfo(" - Checking for unused materials");

                // Loop through the meshes by id
                List<string> materialIds = new List<string>(this.ResultModel.MaterialsByColladaId.Keys);
                foreach (string materialId in materialIds)
                {
                    // Get this material
                    Material material = this.MaterialsByColladaId[materialId];

                    // Are there any instances of this material?
                    bool instanceFound = false;
                    foreach (MeshInstance meshInstance in this.ResultModel.MeshInstances)
                        foreach (KeyValuePair<string, Material> materialBinding in meshInstance.MaterialBindings)
                            if (materialBinding.Value == material)
                            {
                                instanceFound = true;
                                break;
                            }

                    // Did we not find any instances of this material?
                    if (instanceFound == false)
                    {
                        // Remove the material from the model
                        this.MaterialsById.Remove(materialId);
                        this.ResultModel.Materials.Remove(material);
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

                // Loop through the nodes by index
                for(int nodeIndex = 0; nodeIndex < this.ResultModel.Nodes.Count; nodeIndex++)
                {
                    // Get this node
                    Node node = this.ResultModel.Nodes[nodeIndex];

                    // Is this node Not in use?
                    if(node.InUse == false)
                    {
                        // Remove the node from the model
                        this.ResultModel.Nodes.RemoveAt(nodeIndex);
                        nodeIndex--;
                        Context.LogInfo(" +- Removing unused node: \"" + node.Name + "\"");

                        // Remove this node from the node heirarchy
                        if (node.ParentNode != null)
                            if (node.ParentNode.ChildNodes.Contains(node))
                                node.ParentNode.ChildNodes.Remove(node);
                        node.ParentNode = null;
                    }
                }
            }
*/
            // Combine like meshes
            // =================================

            // TODO(P2) - Technically, it could improve performance to flatten all meshes through the node scenegraph transforms
            //   This would only be possible if there is no animation.
            //   Only non-skinned meshes
            //   Only combine like-vertex data

            // Order data for optimal rendering
            // =================================

            // TODO(P1) - Sort the mesh instances for optimal performance
            //    Sort the mesh parts to minimize state changes
            //    Sort the mesh isntances to minimize state changes (instaces of the same mesh could be back2back)

        }

        /// <summary>
        /// Recursively walks a node graph and sets the InUse flag for a node, if any of its decendents have the InUse flag set
        /// </summary>
        /// <param name="node"></param>
        public bool RecurseUpdateNodeUsage(Node node)
        {
            // Recurse to the children.  If any of them are in use, then this node will be too!
            foreach (Node childNode in node.ChildNodes)
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
            // Finalize the Meshes
            // ==================================

            Context.LogInfo("<b> - Finalizing Meshes</b>");

            // Loop through the meshes
            foreach (Mesh mesh in this.ResultModel.Meshes)
            {
                FinalizeMesh(mesh);
            }

            // Log the final object counts
            // ====================================

            Context.LogInfo("<br/><b> - Final Model Details</b>");
            Context.LogInfo(" +- Nodes: " + this.ResultModel.Nodes.Count);
            Context.LogInfo(" +- Materials: " + this.ResultModel.Materials.Count);
            Context.LogInfo(" +- Meshes: " + this.ResultModel.Meshes.Count);
            Context.LogInfo(" +- Mesh Instances: " + this.ResultModel.MeshInstances.Count);
            Context.LogInfo(" +- Animations: " + this.ResultModel.Animations.Count);
            Context.LogInfo(" +- Animation Clips: " + this.ResultModel.AnimationClips.Count);

            // Check the final stats
            // ------------------------------------

            if (this.ResultModel.Nodes.Count == 0)
                throw new BuildException("The model has to final data, either the collada source is empty or invalid, or something went horribly wrong during processing");

            if (this.ResultModel.MeshInstances.Count == 0)
                Context.LogWarn("No meshes were instanced in this model, so the model data is empty..  This is likely a bad thing...");
        }

        /// <summary>
        /// Finalizes the data in the mesh
        /// </summary>
        private void FinalizeMesh(Mesh mesh)
        {
            Context.LogInfo(" +-- Mesh: " + mesh.ColladaID);

            // Calculate missing normals
            // -------------------------------------

            bool autoCalculatedNormals = false;

            // Loop through the triangle soups in this mesh
            foreach (KeyValuePair<string, List<Triangle>> trianglesByMaterialSymbol in mesh.TrianglesByMaterialSymbol)
            {
                // Loop through the triangles
                foreach (Triangle triangle in trianglesByMaterialSymbol.Value)
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
                Context.LogVerbose(" +---- Auto-generating normals");
            }

            // Process the triangle soups into vertex/index buffer'd mesh parts
            // ------------------------------------------------------------------

            int originalVertices = 0;
            int reducedVertices = 0;

            // Loop through the triangle soups
            foreach (KeyValuePair<string, List<Triangle>> trianglesByMaterialSymbol in mesh.TrianglesByMaterialSymbol)
            {
                // Get the soup & the material symbol
                string materialSymbol = trianglesByMaterialSymbol.Key;
                List<Triangle> triangles = trianglesByMaterialSymbol.Value;

                // Create a mesh part for this soup
                MeshPart meshPart = new MeshPart();
                mesh.MeshParts.Add(meshPart);
                meshPart.IndexStart = mesh.Indices.Count;
                meshPart.IndexCount = triangles.Count * 3;
                meshPart.MaterialSymbol = materialSymbol;

                // Convert triangle soup to vertices/indices   (removing duplicate vertices)
                foreach (Triangle triangle in triangles)
                {
                    AddIndexedVertexToMesh(mesh, triangle.Vertices[0]);
                    AddIndexedVertexToMesh(mesh, triangle.Vertices[1]);
                    AddIndexedVertexToMesh(mesh, triangle.Vertices[2]);
                }

                // Track the vertex counts
                originalVertices += triangles.Count * 3;
            }
            reducedVertices += mesh.Vertices.Count;

            // Log the vertex & index count
            if (originalVertices > 0)
                Context.LogVerbose(" +---- Vertices: [Original: " + originalVertices + "] [Reduced: " + reducedVertices + "] - " + (int)((1.0f - reducedVertices / (float)originalVertices) * 100) + "% reduction");
            Context.LogVerbose(" +---- Indices: " + mesh.Indices.Count);

            
            // Log the mesh part details
            foreach(MeshPart meshPart in mesh.MeshParts)
            {
                Context.LogVerbose(" +---- Mesh Part [Material: " + meshPart.MaterialSymbol + "] [IndexCount: " + meshPart.IndexCount + "] [IndexStart: " + meshPart.IndexStart + "]");
            }

            // Check for huge meshes..
            if (mesh.Vertices.Count > UInt16.MaxValue)
                throw new BuildException("Mesh contains too many vertices:  " + mesh.Vertices.Count);
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

        /// <summary>
        /// Adds a vertex & its index to the mesh.  If a like vertex already exists, that vertex is indexed instead.
        /// </summary>
        public int AddIndexedVertexToMesh(Mesh mesh, Vertex vertex)
        {
            // Loop through the existing vertices
            for(int vertexIndex=0; vertexIndex < mesh.Vertices.Count; vertexIndex++)
            {
                // Is this existing vertex a match for the vertex we're adding?
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
            foreach (Material material in this.ResultModel.Materials)
            {
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

            // Create the model file
            FileStream file = File.Create(fullDestPath, 2048, FileOptions.None);
            BinaryWriter writer = new BinaryWriter(file);

            // Write the model to the stream
            this.ResultModel.WriteToStream(writer);

            // Finished
            writer.Flush();
            writer.Close();
        }

        #endregion

        #region Utility Methods

        // Internals
        int nextUniqueId = 1;

        /// <summary>
        /// Returns a unique integer every time this method is called
        /// </summary>
        /// <returns></returns>
        public int GetUniqueId()
        {
            int id = nextUniqueId;
            nextUniqueId++;
            return id;
        }

        #endregion

    } // class

    /// <summary>
    /// Storage class used to store bone index & weight information
    /// </summary>
    class SkinVertexData
    {
        public int NumBones;
        public List<int> Indices = new List<int>();
        public List<float> Weights = new List<float>();
    }

} // namespace



//  TODO(P1)  Work Items
// ----------------------------------------

// Optimizer
//      For skeletal meshes > 24 bones, split the mesh into 2, with seperate bone structures
//
// Processor
//      Animation Data
//      <lines> / trifans / tristrips!!  some models use em...
// x    <library_nodes>
//      import sampler2d texture states...
//          if texture is NPoT, and wrapmode != clamp, throw warning..

// Options:
//      Scale textures, 1/2, 1/4, etc..
//      Color texture format override (Disabled, 5551, 565)
//      Alpha texture format override (Disabled, 4444, 5551)