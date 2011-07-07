/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#include "BasePCH.h"
#include "ModelMesh.h"

using namespace Gdk;

// ***********************************************************************
size_t ModelMeshFlags::GetVertexStrideFromFlags(UInt16 flags)
{
	// All mesh vertices are assumed to have a float[3] Position + float[3] Normal
	size_t vertexStride = sizeof(float[6]);

	// Add more stride for the other vertex attribute
	if((flags & ModelMeshFlags::VertexHasColor) > 0)
		vertexStride += sizeof(UInt8[4]);
	if((flags & ModelMeshFlags::VertexHasTexCoords) > 0)
		vertexStride += sizeof(float[2]);
	if((flags & ModelMeshFlags::VertexHasTexCoords2) > 0)
		vertexStride += sizeof(float[2]);

	UInt16 skinningType = flags & ModelMeshFlags::SKINNING_TYPE_MASK;
	if(skinningType == ModelMeshFlags::VertexHasSingleBone)
		vertexStride += sizeof(UInt16);
	else if(skinningType == ModelMeshFlags::VertexHas2WeightedBones)
		vertexStride += sizeof(UInt16[2]) + sizeof(float[2]);
	else if(skinningType == ModelMeshFlags::VertexHas4WeightedBones)
		vertexStride += sizeof(UInt16[4]) + sizeof(float[4]);
	// else:   ModelMeshFlags::VertexHasNoSkinning

	return vertexStride;
}

// ***********************************************************************
ModelMesh::ModelMesh()
{
	VertexBuffer = 0;
	IndexBuffer = 0;
}

// ***********************************************************************
ModelMesh::~ModelMesh()
{
	// Delete any mesh parts
	for(vector<ModelMeshPart*>::iterator iter = this->MeshParts.begin(); iter != this->MeshParts.end(); iter++)
	{
		GdkDelete(*iter);
	}

	// Release the Vertex & Index buffers
	glDeleteBuffers(1, &VertexBuffer);
	glDeleteBuffers(1, &IndexBuffer);
}

// ***********************************************************************
ModelMeshPart::ModelMeshPart()
{
}


// ***********************************************************************
ModelMeshPart::~ModelMeshPart()
{
}