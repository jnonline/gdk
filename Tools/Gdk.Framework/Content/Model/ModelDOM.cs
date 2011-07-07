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
        public Dictionary<string, Node> NodesById = new Dictionary<string, Node>();
        public Dictionary<string, Material> MaterialsById = new Dictionary<string, Material>();
        public Dictionary<string, Mesh> MeshesById = new Dictionary<string, Mesh>();
        public List<MeshInstance> MeshInstances = new List<MeshInstance>();

        // Compile-time Properties (these are only used during the compilation/processing process)
        // ----------------------------------------------------
    }

    // ========================================================
    public class Node
    {
        // Run-time Properties (these are written out to the binary)
        // ----------------------------------------------------

        public string Id;
        public Gdk.Matrix Transform;
        public int ParentIndex;

        // Compile-time Properties (these are only used during the compilation/processing process)
        // ----------------------------------------------------
        
        public int Index;
        public List<Node> ChildNodes = new List<Node>();
        public bool InUse;
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
		SKINNING_TYPE_MASK			  = 0x0300,
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

        /// <summary>
        /// Equals()
        /// </summary>
        public bool Equals(Vertex v)
        {
            return v.Position.Equals(this.Position)
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
    }

    // ========================================================
    public class Triangle
    {
        // This class is ONLY used during compile time
        public Vertex[] Vertices = new Vertex[3];
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
    

}

