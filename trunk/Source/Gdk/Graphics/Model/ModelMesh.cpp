/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#include "BasePCH.h"
#include "ModelMesh.h"

using namespace Gdk;

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