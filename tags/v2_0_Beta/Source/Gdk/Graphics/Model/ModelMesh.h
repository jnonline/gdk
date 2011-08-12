/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#pragma once



namespace Gdk
{
	// ============================================================================
	namespace ModelMeshAttributeChannels
	{
		// Notes:   
		//	All Model Shaders must use these channel indexes for attributes
		//	Vertex Data within the mesh must ALSO be in this order (though various attributes can be missing/excluded)
		
		const int Position		= 0;
		const int Normal		= 1;
		const int Color			= 2;
		const int TexCoords1	= 3;
		const int TexCoords2	= 4;
		const int BoneIndices	= 5;
		const int BoneWeights	= 6;

		const int FIRST_UNUSED		= 7; // This must be the last index, every attribute from this point on will be disabled

	}

	// ============================================================================
	namespace ModelMeshFlags
	{
		enum Enum
		{
			None = 0,

			VertexHasColor			= 0x0001,		// UInt8[4]
			VertexHasTexCoords		= 0x0002,		// float[2]
			VertexHasTexCoords2		= 0x0004,		// float[2]

			// Bone skinning data types
			SKINNING_TYPE_MASK		= 0x0300,
			VertexHasNoSkinning		= 0x0000,
			VertexHasSingleBone		= 0x0100,		// UInt16[1]				Single bone index
			VertexHas2WeightedBones	= 0x0200,		// UInt16[2] + float[2]		2 Bone indices & weights
			VertexHas4WeightedBones	= 0x0300,		// UInt16[4] + float[4]		4 Bone indices & weights
		};

		// Calculates the vertex stride from the mesh flags
		size_t GetVertexStrideFromFlags(UInt16 flags);
	}

	// ============================================================================
	class ModelMesh
	{
	public:
		
		// Properties 
		string	Name;
		UInt16	Flags;
		UInt16	NumVertices;
		UInt16	NumIndices;
		Sphere3	BoundingSphere;

		GLuint	VertexBuffer;
		GLuint	IndexBuffer;

		vector<class ModelMeshPart*> MeshParts;

		UInt16				NumJoints;
		vector<Matrix3D>	JointInvBindMatrices;

		// CTor / DTor
		ModelMesh();
		~ModelMesh();
	};

	// ===================================================

	class ModelMeshPart
	{
	public:
		
		// Properties 
		string	MaterialSymbol;	// Index into the model's materials
		UInt16	IndexStart;
		UInt16	IndexCount;

		// CTor / DTor
		ModelMeshPart();
		~ModelMeshPart();
	};

} // namespace