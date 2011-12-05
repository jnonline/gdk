/*
  Copyright (c) 2011, Raincity Games LLC
  Licensed under the MIT license: http://www.opensource.org/licenses/mit_license.php
*/

using System;
using System.Collections.Generic;
using System.Linq;
using System.IO;
using System.Text;
using System.Xml;
using Gdk.Framework;

namespace Gdk.Framework.Content.Model
{
    // ========================================================
    // This is the DOM for GDK Model objects
    //

    // ========================================================
    public class Model
    {
        // Content Properties
        // ------------------------

        // Root node of the model node graph        
        public Node RootNode;

        // Basic objects
        public List<Node> Nodes = new List<Node>();
        public List<Material> Materials = new List<Material>();
        public List<Mesh> Meshes = new List<Mesh>();
        public List<Animation> Animations = new List<Animation>();
        public List<AnimationClip> AnimationClips = new List<AnimationClip>();

        // Instances
        public List<MeshInstance> MeshInstances = new List<MeshInstance>();

        // Methods
        // ------------------------

        /// <summary>
        /// Writes the model binary data to a stream
        /// </summary>
        public void WriteToStream(BinaryWriter writer)
        {
            // Version & Flags
            UInt16 version = 3;
            UInt16 headerFlags = 0;
            writer.Write(version);
            writer.Write(headerFlags);

            // Sub-object counts
            writer.Write((UInt16)this.Nodes.Count);
            writer.Write((UInt16)this.Materials.Count);
            writer.Write((UInt16)this.Meshes.Count);
            writer.Write((UInt16)this.MeshInstances.Count);
            writer.Write((UInt16)this.Animations.Count);
            writer.Write((UInt16)this.AnimationClips.Count);

            // Assign indices to the child objects
            for (int index = 0; index < this.Nodes.Count; index++)
                this.Nodes[index].Index = index;
            for (int index = 0; index < this.Materials.Count; index++)
                this.Materials[index].Index = index;
            for (int index = 0; index < this.Meshes.Count; index++)
                this.Meshes[index].Index = index;
            for (int index = 0; index < this.Animations.Count; index++)
                this.Animations[index].Index = index;

            // Nodes
            // ----------------------

            // Loop through the nodes
            foreach (Node node in this.Nodes)
            {
                writer.WriteString(node.Name);
                writer.Write((UInt16)(node.ParentNode == null ? 65535 : node.ParentNode.Index));
                writer.WriteMatrix(node.PoseTransform);
            }

            // Materials
            // -----------------------

            // Loop through the materials
            foreach (Material material in this.Materials)
            {
                // Write out the material
                writer.WriteString(material.Name);
                writer.Write((UInt16)material.Flags);
                writer.WriteColor(material.Emissive);
                writer.WriteColor(material.Ambient);
                writer.WriteColor(material.Diffuse);
                writer.WriteColor(material.Specular);
                writer.Write((float)material.Shininess);

                if ((material.Flags & MaterialFlags.DiffuseTextured) > 0)
                    writer.WriteString(material.DiffuseTexture);
                if ((material.Flags & MaterialFlags.BumpTextured) > 0)
                    writer.WriteString(material.BumpTexture);
            }

            // Meshes
            // -----------------------

            // Loop through the meshes
            foreach (Mesh mesh in this.Meshes)
            {
                WriteMesh(writer, mesh);
            }

            // Mesh Instances
            // -----------------------

            // Loop through the mesh instances
            foreach (MeshInstance meshInstance in this.MeshInstances)
            {
                // Write the instanced object indices
                writer.Write((UInt16)meshInstance.Node.Index);
                writer.Write((UInt16)meshInstance.Mesh.Index);

                // Write the material assignment table for the mesh

                // Loop through the mesh parts of the mesh
                foreach (MeshPart meshPart in meshInstance.Mesh.MeshParts)
                {
                    // Get the material this mesh part's material symbol is bound to
                    Material material = meshInstance.MaterialBindings[meshPart.MaterialSymbol];

                    // Write the material index for the material this part is bound to
                    writer.Write((UInt16)material.Index);
                }

                // Write the joint to node mappings
                if (meshInstance.JointToNodeMap.Count != meshInstance.Mesh.NumJoints)
                    throw new BuildException("MeshInstance does not contain a matching number of joint to node assignments");
                foreach (Node jointNode in meshInstance.JointToNodeMap)
                {
                    // Write the index of the node used at this joint
                    writer.Write((UInt16)jointNode.Index);
                }
            }
        }

        /// <summary>
        /// Writes all the data in a Mesh to the given BinaryWriter
        /// </summary>
        private void WriteMesh(BinaryWriter writer, Mesh mesh)
        {
            // Write out the mesh header
            writer.WriteString(mesh.Name);
            writer.Write((UInt16)mesh.Flags);
            writer.Write((UInt16)mesh.Vertices.Count);
            writer.Write((UInt16)mesh.Indices.Count);
            writer.Write((UInt16)mesh.MeshParts.Count);

            // Write the number of joints used by this skeletal mesh
            writer.Write((UInt16)mesh.NumJoints);

            // Write out the InvBindMatrices for the joints
            for (UInt16 jointIndex = 0; jointIndex < mesh.NumJoints; jointIndex++)
            {
                writer.WriteMatrix(mesh.JointInvBindMatrices[jointIndex]);
            }

            // Write the bounding sphere
            writer.WriteVector3(mesh.BoundingSphere.Center);
            writer.Write(mesh.BoundingSphere.Radius);

            // Write the vertex data
            foreach (Vertex vertex in mesh.Vertices)
            {
                // Write the position & normal
                writer.WriteVector3(vertex.Position);
                writer.WriteVector3(vertex.Normal);

                // Write the optional vertex properties
                if ((mesh.Flags & MeshFlags.VertexHasColor) > 0)
                    writer.WriteColor(vertex.Color);
                if ((mesh.Flags & MeshFlags.VertexHasTexCoords) > 0)
                    writer.WriteVector2(vertex.TexCoords1);
                if ((mesh.Flags & MeshFlags.VertexHasTexCoords2) > 0)
                    writer.WriteVector2(vertex.TexCoords2);

                // Write the bone index/weight propeties
                if ((mesh.Flags & MeshFlags.SKINNING_TYPE_MASK) == MeshFlags.VertexHasSingleBone)
                    writer.Write(vertex.BoneIndices[0]);
                else if ((mesh.Flags & MeshFlags.SKINNING_TYPE_MASK) == MeshFlags.VertexHas2WeightedBones)
                {
                    writer.Write(vertex.BoneIndices[0]);
                    writer.Write(vertex.BoneIndices[1]);
                    writer.Write(vertex.BoneWeights[0]);
                    writer.Write(vertex.BoneWeights[1]);
                }
                else if ((mesh.Flags & MeshFlags.SKINNING_TYPE_MASK) == MeshFlags.VertexHas4WeightedBones)
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
            foreach (MeshPart meshPart in mesh.MeshParts)
            {
                writer.Write((UInt16)meshPart.IndexStart);
                writer.Write((UInt16)meshPart.IndexCount);
                writer.WriteString(meshPart.MaterialSymbol);
            }
        }
    }

    // ========================================================
    public class Node
    {
        // Content Properties
        // ------------------------

        public string Name;
        public Gdk.Framework.Matrix PoseTransform;

        public Node ParentNode;
        public List<Node> ChildNodes = new List<Node>();

        // Internal Properties
        // ------------------------

        internal int Index;
        internal string ColladaID;
        internal string ColladaSID;
        internal bool InUse;

        // Utility Methods
        // ------------------------

        public Node FindNodeByColladaId(string colladaId)
        {
            // Do we have this id?
            if (this.ColladaID == colladaId)
                return this;
            
            // Recurse to the children
            foreach(Node childNode in this.ChildNodes)
            {
                Node result = childNode.FindNodeByColladaId(colladaId);
                if (result != null)
                    return result;
            }

            return null;
        }
    }

    // ========================================================
    [Flags]
    public enum MaterialFlags
    {
        DiffuseTextured = 0x0001,
        BumpTextured = 0x0002,
    }

    // ========================================================
    public class Material
    {
        // Content Properties
        // ------------------------

        public string Name;
        public MaterialFlags Flags;

        // Basic material properties
        public Color Emissive;
        public Color Ambient;
        public Color Diffuse;
        public Color Specular;
        public float Shininess;

        // Texture references
        public string DiffuseTexture = null;
        public string BumpTexture = null;

        // Internal Properties
        // ------------------------

        internal int Index;
        internal string ColladaID;
    }

    // ========================================================
    public enum AttributeChannels
    {
        Position = 0,
        Normal = 1,
        Color = 2,
        TexCoords1 = 3,
        TexCoords2 = 4,
        BoneIndices = 5,
        BoneWeights = 6,
    }

    // ========================================================
    [Flags]
    public enum MeshFlags
    {
        VertexHasColor			    = 0x0001,		// UByte[4]
		VertexHasTexCoords		    = 0x0002,		// float[2]
		VertexHasTexCoords2		    = 0x0004,		// float[2]

		// Bone skinning data types
		SKINNING_TYPE_MASK			= 0x0300,
		VertexHasNoSkinning		    = 0x0000,
		VertexHasSingleBone		    = 0x0100,		// float[1]				Single bone index
		VertexHas2WeightedBones	    = 0x0200,		// float[2] + float[2]	2 Bone indices & weights
		VertexHas4WeightedBones	    = 0x0300,		// float[4] + float[4]	4 Bone indices & weights
    }

    // ========================================================
    public class Vertex
    {
        // Content Properties
        // ------------------------

        public Vector3 Position;
        public Vector3 Normal;
        public Color Color;
        public Vector2 TexCoords1;
        public Vector2 TexCoords2;
        public byte[] BoneIndices = new byte[4];
        public float[] BoneWeights = new float[4];

        // Runtime Properties
        // -----------------------------

        public MeshFlags Flags;
        public int Index;

        // Methods
        // ------------------------

        /// <summary>
        /// Equals()
        /// </summary>
        public bool Equals(Vertex v)
        {
            return v.Flags == this.Flags
                && v.Position.Equals(this.Position)
                && v.Normal.Equals(this.Normal)
                && v.Color.Equals(this.Color)
                && v.TexCoords1.Equals(this.TexCoords1)
                && v.TexCoords2.Equals(this.TexCoords2)
                && v.BoneIndices[0] == this.BoneIndices[0]
                && v.BoneIndices[1] == this.BoneIndices[1]
                && v.BoneIndices[2] == this.BoneIndices[2]
                && v.BoneIndices[3] == this.BoneIndices[3]
                && v.BoneWeights[0] == this.BoneWeights[0]
                && v.BoneWeights[1] == this.BoneWeights[1]
                && v.BoneWeights[2] == this.BoneWeights[2]
                && v.BoneWeights[3] == this.BoneWeights[3]
                ;
        }

        /// <summary>
        /// Default Constructor
        /// </summary>
        public Vertex()
        {
        }

        /// <summary>
        /// Copy constructor
        /// </summary>
        public Vertex(Vertex input)
        {
            this.Position = input.Position;
            this.Normal = input.Normal;
            this.Color = input.Color;
            this.TexCoords1 = input.TexCoords1;
            this.TexCoords2 = input.TexCoords2;
            for (int i = 0; i < 4; i++)
            {
                this.BoneIndices[i] = input.BoneIndices[i];
                this.BoneWeights[i] = input.BoneWeights[i];
            }
            this.Flags = input.Flags;
            this.Index = input.Index;
        }
    }

    // ========================================================
    public class Mesh
    {
        // Content Properties
        // ------------------------

        public string Name;
        public MeshFlags Flags;

        // Vertex & Index data
        public List<Vertex> Vertices = new List<Vertex>();
        public List<int> Indices = new List<int>();

        // Joint data  (if mesh has skinning)
        public UInt16 NumJoints;
        public List<Matrix> JointInvBindMatrices = new List<Matrix>();

        // Mesh Parts
        public List<MeshPart> MeshParts = new List<MeshPart>();

        // Bounding volume
        public Sphere3 BoundingSphere;

        // Internal Properties
        // ------------------------

        internal int Index;
        internal Dictionary<string, List<Triangle>> TrianglesByMaterialSymbol = new Dictionary<string, List<Triangle>>();
        internal Dictionary<string, List<Line>> LinesByMaterialSymbol = new Dictionary<string, List<Line>>();

        internal List<string> JointSIDs = new List<string>();
        internal string ColladaID;
    }

    // ========================================================
    public class MeshPart
    {
        // Run-time Properties (these are written out to the binary)
        // ----------------------------------------------------

        public int IndexStart;
        public int IndexCount;
        public string MaterialSymbol;
    }

    // ========================================================
    public class MeshInstance
    {
        // Content Properties
        // ----------------------------------------------------

        public Mesh Mesh;
        public Node Node;
        
        // Bindings of material symbol to material object
        public Dictionary<string, Material> MaterialBindings = new Dictionary<string, Material>();

        // List of nodes assigned to joints  (if mesh has skinning data)
        public List<Node> JointToNodeMap = new List<Node>();
    }

    // ========================================================
    public class Animation
    {
        // Content Properties
        // ----------------------------------------------------

        public string Name;
        public List<Animation> Children = new List<Animation>();
        public List<NodeAnimation> NodeAnimations = new List<NodeAnimation>();

        // Runtime Properties
        // ----------------------------------------------------

        internal string ColladaID;
        internal int Index;
    }

    // ========================================================
    public class NodeAnimation
    {
        // Content Properties
        // ----------------------------------------------------

        public Node Node;
        public List<TransformKeyFrame> KeyFrames = new List<TransformKeyFrame>();
    }

    // ========================================================
    public class TransformKeyFrame
    {
        // Content Properties
        // ----------------------------------------------------

        public float Time;
        public Vector3 Translation;
        public Quaternion Rotation;
        public Vector3 Scale;
    }

    // ========================================================
    public class AnimationClip
    {
        // Content Properties
        // ----------------------------------------------------

        public string Name;
        public float StartTime;
        public float EndTime;

        public List<Animation> Animations = new List<Animation>();
    }




    // ==========================================
    public class Triangle
    {
        // This class is ONLY used during compile time
        public Vertex[] Vertices = new Vertex[3];

        /// <summary>
        /// Vertex Indexer
        /// </summary>
        public Vertex this[int index]
        {
            get { return this.Vertices[index]; }
            set { this.Vertices[index] = value; }
        }

        /// <summary>
        /// Default constructor
        /// </summary>
        public Triangle()
        {
        }

        /// <summary>
        /// Copy constructor
        /// </summary>
        public Triangle(Triangle input)
        {
            for (int i = 0; i < 3; i++)
            {
                this.Vertices[i] = new Vertex(input.Vertices[i]);
            }
        }
    }

    // ==========================================
    public class Line
    {
        // This class is ONLY used during compile time
        public Vertex[] Vertices = new Vertex[2];

        /// <summary>
        /// Vertex Indexer
        /// </summary>
        public Vertex this[int index]
        {
            get { return this.Vertices[index]; }
            set { this.Vertices[index] = value; }
        }

        /// <summary>
        /// Default constructor
        /// </summary>
        public Line()
        {
        }

        /// <summary>
        /// Copy constructor
        /// </summary>
        public Line(Line input)
        {
            for (int i = 0; i < 2; i++)
            {
                this.Vertices[i] = new Vertex(input.Vertices[i]);
            }
        }
    }
 }

