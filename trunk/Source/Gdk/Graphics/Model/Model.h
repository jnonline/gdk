/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#pragma once

#include "GdkLIB.h"
#include "../../Assets/AssetType.h"
#include "../../Math/3D/Matrix3D.h"

namespace Gdk
{
	// ===================================================
	class Model
	{
	public:
		// Model/Mesh/Material Properties
		class ModelNode*					RootNode;
		vector<class ModelNode*>			Nodes;
		vector<class ModelMaterial*>		Materials;
		vector<class ModelMesh*>			Meshes;
		vector<class ModelMeshInstance*>	MeshInstances;

		// Runtime properties
		Matrix3D	World;

		// Methods
		~Model();
		class ModelInstance* CreateInstance();
		void Draw(class ModelInstance* modelInstance = NULL);
		
		// Asset Interfaces
		static const AssetType::Enum ASSET_TYPE = AssetType::Model;
		static Model* FromAsset(AssetLoadContext* context);
		void IgnoreChildAssets();

	private:

		// Private CTor
		Model();

		// Utility methods
		void SetupVertexAttributeChannels(ModelMesh* mesh);
		Shader* DetermineShader(ModelMesh* mesh, ModelMaterial* material);

		// References to assets that this model uses (Textures, etc)
		vector<class AssetBase*>  ownedAssets;
	};

} // namespace