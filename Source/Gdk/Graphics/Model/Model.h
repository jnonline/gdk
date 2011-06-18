
#pragma once

#include "GdkLIB.h"
#include "../../Assets/AssetType.h"

namespace Gdk
{
	// ===================================================
	class Model
	{
	public:
		// Properties
		vector<class ModelMaterial*>	Materials;
		vector<class ModelMesh*>		Meshes;

		// Methods
		~Model();
		class ModelInstance* CreateInstance();

		// Asset Interfaces
		static const AssetType::Enum ASSET_TYPE = AssetType::Model;
		static Model* FromAsset(AssetLoadContext* context);
		void IgnoreChildAssets();

	private:
		// Private CTor
		Model();

		// References to assets that this model uses (Textures, etc)
		vector<class AssetBase*>  ownedAssets;
	};

} // namespace