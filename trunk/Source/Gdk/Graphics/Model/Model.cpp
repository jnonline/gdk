/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#include "BasePCH.h"
#include "../VertexFormats.h"
#include "Model.h"
#include "ModelMaterial.h"
#include "ModelMesh.h"
#include "ModelInstance.h"

using namespace Gdk;

// ***********************************************************************
Model::Model()
{
}

// ***********************************************************************
Model::~Model()
{
	// Delete all the materials
	for(vector<ModelMaterial*>::iterator iter = this->Materials.begin(); iter != this->Materials.end(); iter++)
	{
		GdkDelete(*iter);
	}

	// Delete all the meshes
	for(vector<ModelMesh*>::iterator iter = this->Meshes.begin(); iter != this->Meshes.end(); iter++)
	{
		GdkDelete(*iter);
	}

	// Release any owned assets
	for(vector<AssetBase*>::iterator assetIter = this->ownedAssets.begin(); assetIter != this->ownedAssets.end(); assetIter++)
	{
		(*assetIter)->Release();
	}
}
	 
// ***********************************************************************
ModelInstance* Model::CreateInstance()
{
	// Create a new instance
	ModelInstance* instance = GdkNew ModelInstance();

	// Set this model as the instance parent
	instance->ParentModel = this;

	// Clone the materials
	instance->Materials.resize( this->Materials.size() );
	for(size_t materialIndex=0; materialIndex < this->Materials.size(); materialIndex++)
	{
		instance->Materials[materialIndex] = this->Materials[materialIndex]->Clone();
	}

	// Return the instance
	return instance;
}

// ***********************************************************************
Model* Model::FromAsset(AssetLoadContext* context)
{
	// Get the model file stream
	Stream *stream = context->AssetStream;

	// Create the model
	Model* model = GdkNew Model();

	// Header
	// ---------------

	// Read the version & flags
	UInt16 version = stream->ReadUInt16();
	UInt16 headerFlags = stream->ReadUInt16();
    
    GDK_NOT_USED(version);
    GDK_NOT_USED(headerFlags);

	// Read the sub-object counts
	UInt16 numMaterials = stream->ReadUInt16();
	UInt16 numMeshes = stream->ReadUInt16();

	// Materials
	// -------------------

	// Loop through the materials
	for(UInt16 materialIndex=0; materialIndex < numMaterials; materialIndex++)
	{
		// Create a ModelMaterial
		ModelMaterial* material = GdkNew ModelMaterial();
		
		// Read in the basic properties
		material->Flags = (ModelMaterialFlags::Enum) stream->ReadUInt16();
		Color::ReadFromStream(stream).Normalize(material->Emissive);
		Color::ReadFromStream(stream).Normalize(material->Ambient);
		Color::ReadFromStream(stream).Normalize(material->Diffuse);
		Color::ReadFromStream(stream).Normalize(material->Specular);
		material->Shininess = stream->ReadFloat();

		// Setup the default shaders
		material->MeshShader = SharedAssets::Shaders.Model.Mesh.NonTextured;

		// Do we need to load a Diffuse texture?
		if(material->Flags & ModelMaterialFlags::DiffuseTextured)
		{
			// Load the asset, track it, and store the texture reference
			string textureName = stream->ReadString();
			string assetFolder = Path::GetDirectory(context->AssetName);
			string textureAssetPath = Path::Combine(assetFolder.c_str(), textureName.c_str());
			Asset<Texture2D>* diffuseTextureAsset = context->Manager->Load<Texture2D>(textureAssetPath.c_str());
			model->ownedAssets.push_back(diffuseTextureAsset);
			material->DiffuseTexture = diffuseTextureAsset->GetInstance();

			// Switch to the diffuse textured default shaders
			material->MeshShader = SharedAssets::Shaders.Model.Mesh.DiffuseTextured;
		}

		// Do we need to load a Bump texture?
		if(material->Flags & ModelMaterialFlags::BumpTextured)
		{
			// Load the asset, track it, and store the texture reference
			string textureName = stream->ReadString();
			string assetFolder = Path::GetDirectory(context->AssetName);
			string textureAssetPath = Path::Combine(assetFolder.c_str(), textureName.c_str());
			Asset<Texture2D>* bumpTextureAsset = context->Manager->Load<Texture2D>(textureAssetPath.c_str());
			model->ownedAssets.push_back(bumpTextureAsset);
			material->BumpTexture = bumpTextureAsset->GetInstance();

			// Switch to the bump textured default shaders
			// TODO(P2): Bump textured shaders...
			//material->MeshShader = SharedAssets::Shaders.Model.Mesh.BumpTextured;
		}


		// Add the material to the model
		model->Materials.push_back(material);
	}

	// Meshes
	// -------------------

	// Loop through the meshes
	for(UInt16 meshIndex=0; meshIndex < numMeshes; meshIndex++)
	{
		// Create a Mesh
		ModelMesh* mesh = GdkNew ModelMesh();

		// Read the mesh flags
		mesh->Flags = (ModelMeshFlags::Enum) stream->ReadUInt16();

		// Read the sub-object counts
		mesh->NumVertices = stream->ReadUInt16();
		mesh->NumIndices = stream->ReadUInt16();
		UInt16 numMeshParts = stream->ReadUInt16();

		// Read the Bounding sphere of the mesh
		mesh->BoundingSphere.Center.X = stream->ReadFloat();
		mesh->BoundingSphere.Center.Y = stream->ReadFloat();
		mesh->BoundingSphere.Center.Z = stream->ReadFloat();
		mesh->BoundingSphere.Radius = stream->ReadFloat();

		// Create GL buffers for the vertex & index data
		glGenBuffers(1, &(mesh->VertexBuffer));
		glGenBuffers(1, &(mesh->IndexBuffer));

		// Read in the vertex data
		int vertexDataSize = mesh->NumVertices * sizeof(VertexP3N3T2);
		void* vertexData = GdkAlloc(vertexDataSize);
		stream->Read(vertexData, vertexDataSize);

		// Copy the vertex data into the vertex buffer
		Graphics::BindVertexBuffer(mesh->VertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, vertexDataSize, vertexData, GL_STATIC_DRAW); 
		GdkFree(vertexData);

		// Read in the index data
		int indexDataSize = mesh->NumIndices * sizeof(UInt16);
		void* indexData = GdkAlloc(indexDataSize);
		stream->Read(indexData, indexDataSize);

		// Copy the index data into the index buffer
		Graphics::BindIndexBuffer(mesh->IndexBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexDataSize, indexData, GL_STATIC_DRAW);
		GdkFree(indexData);
		
		// Loop through the mesh parts
		for(int meshPartIndex=0; meshPartIndex < numMeshParts; meshPartIndex++)
		{
			// Create a Mesh Part
			ModelMeshPart* meshPart = GdkNew ModelMeshPart();

			// Read the mesh part properties
			meshPart->IndexStart = stream->ReadUInt16();
			meshPart->IndexCount = stream->ReadUInt16();
			meshPart->MaterialIndex = stream->ReadUInt16();

			// Add the mesh part to the mesh
			mesh->MeshParts.push_back(meshPart);
		}

		// Add the mesh to the model
		model->Meshes.push_back(mesh);
	}

	// Return the loaded model
	return model;
}	

// ***********************************************************************
void Model::IgnoreChildAssets()
{
	// The asset manager is asking up to ignore assets we reference
	this->ownedAssets.clear();
}