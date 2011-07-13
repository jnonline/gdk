using System;
using System.Collections.Generic;
using System.Linq;
using System.IO;
using System.Text;
using System.Xml;
using Gdk;

namespace Gdk.Content.Model
{
    // ========================================================
    public class Model
    {
        // Run-time Properties (these are written out to the binary)
        // ----------------------------------------------------

        public Node RootNode;

        public List<Node> Nodes = new List<Node>();
        public List<MeshInstance> MeshInstances = new List<MeshInstance>();
        public List<SkeletalMeshInstance> SkeletalMeshInstances = new List<SkeletalMeshInstance>();

        public Dictionary<string, Material> MaterialsById = new Dictionary<string, Material>();
        public Dictionary<string, Mesh> MeshesById = new Dictionary<string, Mesh>();
        public Dictionary<string, SkeletalMesh> SkeletalMeshesById = new Dictionary<string, SkeletalMesh>();

        // Compile-time Properties (these are only used during the compilation/processing process)
        // ----------------------------------------------------
    }

    // ========================================================
    public class Node
    {
        // Run-time Properties (these are written out to the binary)
        // ----------------------------------------------------

        public string Name;
        public Gdk.Matrix Transform;

        // Compile-time Properties (these are only used during the compilation/processing process)
        // ----------------------------------------------------

        public string Sid;
        public int Index;
        public Node ParentNode;
        public List<Node> ChildNodes = new List<Node>();
        public bool InUse;

        public Gdk.Matrix AbsoluteTransform;

        /// <summary>
        /// Finds a node with the given Id by recursively walking the node heirarchy
        /// </summary>
        /// <param name="nodeId"></param>
        /// <returns></returns>
        public Node FindNodeById(string nodeId)
        {
            // Does this node have the given Id?
            if (this.Name == nodeId)
                return this;

            // Recurse to the children
            foreach(Node childNode in this.ChildNodes)
            {
                Node result = childNode.FindNodeById(nodeId);
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
        // Run-time Properties (these are written out to the binary)
        // ----------------------------------------------------

        public string Id;
        public MaterialFlags Flags;
        public Color Emissive;
        public Color Ambient;
        public Color Diffuse;
        public Color Specular;
        public float Shininess;

        // Texture References
        public string DiffuseTexture = null;
        public string BumpTexture = null;

        // Compile-time Properties (these are only used during the compilation/processing process)
        // ----------------------------------------------------

        public int Index;
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
        public Vector3 Position;
        public Vector3 Normal;
        public Color Color;
        public Vector2 TexCoords1;
        public Vector2 TexCoords2;
        public UInt16[] BoneIndices = new UInt16[4];
        public float[] BoneWeights = new float[4];

        // These flags define the data that is relevant in the vertex
        public MeshFlags Flags;
        public int Index;

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
        /// Creates a duplicate of this mesh (only the pre-finalized triangle soup data is cloned, none of the meshpart/vertex/index data is cloned)
        /// </summary>
        /// <returns></returns>
        public Vertex Clone()
        {
            Vertex clone = new Vertex();
            clone.Position = this.Position;
            clone.Normal = this.Normal;
            clone.Color = this.Color;
            clone.TexCoords1 = this.TexCoords1;
            clone.TexCoords2 = this.TexCoords2;
            for (int i = 0; i < 4; i++)
            {
                clone.BoneIndices[i] = this.BoneIndices[i];
                clone.BoneWeights[i] = this.BoneWeights[i];
            }
            clone.Flags = this.Flags;
            clone.Index = this.Index;
            return clone;
        }
    }

    // ========================================================
    public class Mesh
    {
        // Run-time Properties (these are written out to the binary)
        // ----------------------------------------------------

        public string Id;
        public MeshFlags Flags;

        public List<Vertex> Vertices = new List<Vertex>();
        public List<int> Indices = new List<int>();
        public List<MeshPart> MeshParts = new List<MeshPart>();
        public Sphere3 BoundingSphere;

        // Compile-time Properties (these are only used during the compilation/processing process)
        // ----------------------------------------------------

        public int Index;
        public Dictionary<string, TriangleSoup> TriangleSoupsByMaterialSymbol = new Dictionary<string, TriangleSoup>();

    }

    // ========================================================
    public class SkeletalMesh : Mesh
    {
        // Run-time Properties (these are written out to the binary)
        // ----------------------------------------------------

        public UInt16 NumJoints;
        public List<Matrix> JointInvBindMatrices = new List<Matrix>();

        // Compile-time Properties (these are only used during the compilation/processing process)
        // ----------------------------------------------------

        public List<string> JointSIDs = new List<string>();
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
    public class TriangleSoup
    {
        // This class is ONLY used during compile time
        public List<Triangle> Triangles = new List<Triangle>();

        /// <summary>
        /// Creates a duplicate of this object
        /// </summary>
        /// <returns></returns>
        public TriangleSoup Clone()
        {
            TriangleSoup clone = new TriangleSoup();
            foreach(Triangle triangle in this.Triangles)
            {
                clone.Triangles.Add(triangle.Clone());
            }
            return clone;
        }
    }

    // ========================================================
    public class Triangle
    {
        // This class is ONLY used during compile time
        public Vertex[] Vertices = new Vertex[3];

        /// <summary>
        /// Creates a duplicate of this object
        /// </summary>
        /// <returns></returns>
        public Triangle Clone()
        {
            Triangle clone = new Triangle();
            for (int i = 0; i < 3; i++)
            {
                clone.Vertices[i] = this.Vertices[i].Clone();
            }
            return clone;
        }
    }

    // ========================================================
    public class MeshInstance
    {
        // Run-time Properties (these are written out to the binary)
        // ----------------------------------------------------

        public Mesh Mesh;
        public Node Node;
        
        // Compile-time Properties (these are only used during the compilation/processing process)
        // ----------------------------------------------------

        public Dictionary<string, Material> MaterialBindings = new Dictionary<string, Material>();
    }

    // ========================================================
    public class SkeletalMeshInstance : MeshInstance
    {
        // Run-time Properties (these are written out to the binary)
        // ----------------------------------------------------

        public List<Node> JointToNodeMap = new List<Node>();    // This list contains a reference to a node for each joint of the skeletal mesh

        // Compile-time Properties (these are only used during the compilation/processing process)
        // ----------------------------------------------------

    }
    

}

