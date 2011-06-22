/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#pragma once

#include "GdkLIB.h"

namespace Gdk
{
	// ============================================================================
	namespace ModelMeshFlags
	{
		enum Enum
		{
			None = 0,
		};
	}

	// ============================================================================
	class ModelMesh
	{
	public:
		
		// Properties 
		ModelMeshFlags::Enum Flags;
		UInt16 NumVertices;
		UInt16 NumIndices;
		Sphere3 BoundingSphere;

		GLuint VertexBuffer;
		GLuint IndexBuffer;

		vector<class ModelMeshPart*> MeshParts;

		// Methods
		~ModelMesh();

	private:
		// Private CTor
		ModelMesh();
		friend class Model;
	};

	// ===================================================

	class ModelMeshPart
	{
	public:
		
		// Properties 
		UInt16 MaterialIndex;	// Index into the model's materials
		UInt16 IndexStart;
		UInt16 IndexCount;

		// Methods
		~ModelMeshPart();

	private:
		// Private CTor
		ModelMeshPart();
		friend class Model;
	};

} // namespace