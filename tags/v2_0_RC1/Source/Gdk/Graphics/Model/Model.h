/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#pragma once

namespace Gdk
{
	/// @addtogroup Graphics
    /// @{
    
    // =================================================================================
    ///	@brief
    ///		Contains an entire model, including meshes, materials, and scenegraph
    // =================================================================================	
	class Model : public Resource
	{
	public:
		// Model/Mesh/Material Properties
		class ModelNode*							RootNode;
		vector<class ModelNode*>					Nodes;
		vector<class ModelMaterial*>				Materials;
		vector<class ModelMesh*>					Meshes;
		vector<class ModelMeshInstance*>			MeshInstances;

		// Runtime properties
		Matrix3D	World;

		// Methods
		~Model();
		class ModelInstance* CreateInstance();
		void Draw(class ModelInstance* modelInstance = NULL);

    protected:
        
        // Protected Methods
        // =====================================================
        
        // ---------------------------------
        /// @name Virtuals from Resouce
        /// @{
        
        
        /// @}
        
        
    private:
        
        // Private Methods
        // =====================================================
        
        friend class ModelManager;
        
        Model();
        
        void LoadFromAsset();

        
		// Rendering Utilities
		void DrawMesh(class ModelMesh* mesh, class ModelMeshInstance* meshInstance, class ModelInstance* modelInstance);
		void SetupVertexAttributeChannels(class ModelMesh* mesh);
		class Shader* DetermineShader(class ModelMesh* mesh, class ModelMaterial* material);

		// References to assets that this model uses (Textures, etc)
		vector<class Resource*>  childResources;
	};

} // namespace