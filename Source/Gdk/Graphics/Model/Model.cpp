/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#include "BasePCH.h"
#include "../VertexFormats.h"
#include "Model.h"
#include "ModelNode.h"
#include "ModelMaterial.h"
#include "ModelMesh.h"
#include "ModelMeshInstance.h"
#include "ModelInstance.h"

using namespace Gdk;

// ***********************************************************************
Model::Model()
{
}

// ***********************************************************************
Model::~Model()
{
	// Delete all the nodes
	for(vector<ModelNode*>::iterator iter = this->Nodes.begin(); iter != this->Nodes.end(); iter++)
	{
		GdkDelete(*iter);
	}

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

	// Delete all the mesh instances
	for(vector<ModelMeshInstance*>::iterator iter = this->MeshInstances.begin(); iter != this->MeshInstances.end(); iter++)
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
	instance->World = this->World;

	// Copy the absolute transforms
	instance->AbsoluteTransforms.reserve(this->Nodes.size());
	for(size_t nodeIndex=0; nodeIndex < this->Nodes.size(); nodeIndex++)
		instance->AbsoluteTransforms.push_back(this->Nodes[nodeIndex]->AbsoluteTransform);

	// Clone the materials
	/*
	instance->Materials.resize( this->Materials.size() );
	for(size_t materialIndex=0; materialIndex < this->Materials.size(); materialIndex++)
	{
		instance->Materials[materialIndex] = this->Materials[materialIndex]->Clone();
	}
	*/

	// Return the instance
	return instance;
}

// ***********************************************************************
Model* Model::FromAsset(AssetLoadContext* context)
{
	// Get the model file stream
	Stream *stream = context->AssetStream;

	// Header
	// ---------------

	// Read the version & flags
	UInt16 version = stream->ReadUInt16();
	UInt16 headerFlags = stream->ReadUInt16();
    
    GDK_NOT_USED(headerFlags);

	// Verify the version
	int requiredVersion = 2;
	if(version != requiredVersion)
	{
		LOG_ERROR(L"The model asset \"%hs\" is an unsupported version: %d [Expected: %d]", context->AssetName, version, requiredVersion);
		return NULL;
	}

	// Read the sub-object counts
	UInt16 numNodes = stream->ReadUInt16();
	UInt16 numMaterials = stream->ReadUInt16();
	UInt16 numMeshes = stream->ReadUInt16();
	UInt16 numMeshInstances = stream->ReadUInt16();

	// Create the model
	Model* model = GdkNew Model();

	// Reserve space for the child objects
	model->Nodes.reserve(numNodes);
	model->Materials.reserve(numMaterials);
	model->Meshes.reserve(numMeshes);
	model->MeshInstances.reserve(numMeshInstances);

	// Nodes
	// -------------------

	// Loop through the number of nodes
	for(UInt16 nodeIndex=0; nodeIndex < numNodes; nodeIndex++)
	{
		// Create a ModelNode
		ModelNode* node = GdkNew ModelNode();

		// Load the stream data
		node->Id = stream->ReadString();
		UInt16 parentNodeIndex = stream->ReadUInt16();
		node->LocalTransform = Matrix3D::ReadFromStream(stream);

		// Setup the node index
		node->Index = nodeIndex;

		// Is this the root node?
		if(nodeIndex == 0)
		{
			// This is the root node
			node->ParentNode = NULL;
			model->RootNode = node;
		}
		else
		{
			// Verify the parent node index is valid
			ASSERT(parentNodeIndex < model->Nodes.size(), L"The model asset \"%hs\" contains a node with an invalid parent node index [Expected: <%d] [Actual: %d]", context->AssetName, model->Nodes.size(), parentNodeIndex);

			// Set this node's parent node
			node->ParentNode = model->Nodes[parentNodeIndex];
			node->ParentNode->ChildNodes.push_back(node);
		}

		// Add the node to the model
		model->Nodes.push_back(node);
	}

	// Update the absoulate transforms of the node tree
	model->RootNode->UpdateAbsoluteTransforms();

	// Materials
	// -------------------
	
	// Loop through the materials
	for(UInt16 materialIndex=0; materialIndex < numMaterials; materialIndex++)
	{
		// Create a ModelMaterial
		ModelMaterial* material = GdkNew ModelMaterial();
		
		// Read in the basic properties
		material->Id = stream->ReadString();
		material->Flags = (ModelMaterialFlags::Enum) stream->ReadUInt16();
		Color::ReadFromStream(stream).Normalize(material->Emissive);
		Color::ReadFromStream(stream).Normalize(material->Ambient);
		Color::ReadFromStream(stream).Normalize(material->Diffuse);
		Color::ReadFromStream(stream).Normalize(material->Specular);
		material->Shininess = stream->ReadFloat();

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

		// Read the mesh properties
		mesh->Id = stream->ReadString();
		mesh->Flags = stream->ReadUInt16();
		mesh->NumVertices = stream->ReadUInt16();
		mesh->NumIndices = stream->ReadUInt16();
		UInt16 numMeshParts = stream->ReadUInt16();

		// Read the Bounding sphere of the mesh
		mesh->BoundingSphere.Center = Vector3::ReadFromStream(stream);
		mesh->BoundingSphere.Radius = stream->ReadFloat();

		// Create GL buffers for the vertex & index data
		glGenBuffers(1, &(mesh->VertexBuffer));
		glGenBuffers(1, &(mesh->IndexBuffer));

		// Read in the vertex data
		size_t vertexStride = ModelMeshFlags::GetVertexStrideFromFlags(mesh->Flags);
		size_t vertexDataSize = mesh->NumVertices * vertexStride;
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
		
		// Pre-size the mesh parts vector
		mesh->MeshParts.reserve(numMeshParts);

		// Loop through the mesh parts
		for(int meshPartIndex=0; meshPartIndex < numMeshParts; meshPartIndex++)
		{
			// Create a Mesh Part
			ModelMeshPart* meshPart = GdkNew ModelMeshPart();

			// Read the mesh part properties
			meshPart->IndexStart = stream->ReadUInt16();
			meshPart->IndexCount = stream->ReadUInt16();
			meshPart->MaterialSymbol = stream->ReadString();

			// Add the mesh part to the mesh
			mesh->MeshParts.push_back(meshPart);
		}

		// Add the mesh to the model
		model->Meshes.push_back(mesh);
	}

	// MeshInstances
	// -------------------

	// Loop through the mesh instances
	for(UInt16 meshInstanceIndex=0; meshInstanceIndex < numMeshInstances; meshInstanceIndex++)
	{
		// Create the mesh instance
		ModelMeshInstance* meshInstance = GdkNew ModelMeshInstance();

		// Get the mesh & node indices
		meshInstance->NodeIndex = stream->ReadUInt16();
		meshInstance->MeshIndex = stream->ReadUInt16();
	
		// Get the mesh used by this instance
		ModelMesh* mesh = model->Meshes[meshInstance->MeshIndex];

		// Pre-size the material bindings vector to the number of mesh parts
		size_t meshPartCount = mesh->MeshParts.size();
		meshInstance->MaterialBindings.reserve(meshPartCount);

		// Loop through the mesh parts
		for(size_t meshPartIndex=0; meshPartIndex < meshPartCount; meshPartIndex++)
		{
			// Get the material binding for this mesh part
			UInt16 materialIndex = stream->ReadUInt16();
			meshInstance->MaterialBindings.push_back(materialIndex);
		}

		// Add the mesh instance to the model
		model->MeshInstances.push_back(meshInstance);
	}

	// -------------------

	// Return the loaded model
	return model;
}	

// ***********************************************************************
void Model::IgnoreChildAssets()
{
	// The asset manager is asking up to ignore assets we reference
	this->ownedAssets.clear();
}

// ***********************************************************************
void Model::SetupVertexAttributeChannels(ModelMesh* mesh)
{
	// This method sets up the GL vertex attribute arrays  (glVertexAttribPointer)
	// for the vertex data that the mesh contains.

	// Get the vertex stride for this mesh's vertices
	size_t vertexStride = ModelMeshFlags::GetVertexStrideFromFlags(mesh->Flags);
	size_t vertexOffset = 0;

	// Position
	Graphics::EnableVertexAttribArray(ModelMeshAttributeChannels::Position, true);
	glVertexAttribPointer(ModelMeshAttributeChannels::Position, 3, GL_FLOAT, GL_FALSE, vertexStride, (void*) vertexOffset);
	vertexOffset += sizeof(float[3]);

	// Normal
	Graphics::EnableVertexAttribArray(ModelMeshAttributeChannels::Normal, true);
	glVertexAttribPointer(ModelMeshAttributeChannels::Normal, 3, GL_FLOAT, GL_FALSE, vertexStride, (void*) vertexOffset);
	vertexOffset += sizeof(float[3]);

	// Color
	if((mesh->Flags & ModelMeshFlags::VertexHasColor) > 0)
	{
		Graphics::EnableVertexAttribArray(ModelMeshAttributeChannels::Color, true);
		glVertexAttribPointer(ModelMeshAttributeChannels::Color, 4, GL_UNSIGNED_BYTE, GL_TRUE, vertexStride, (void*) vertexOffset);
		vertexOffset += sizeof(UInt8[4]);
	}
	else
	{
		Graphics::EnableVertexAttribArray(ModelMeshAttributeChannels::Color, false);
	}
	
	// Tex Coords
	if((mesh->Flags & ModelMeshFlags::VertexHasTexCoords) > 0)
	{
		Graphics::EnableVertexAttribArray(ModelMeshAttributeChannels::TexCoords1, true);
		glVertexAttribPointer(ModelMeshAttributeChannels::TexCoords1, 2, GL_FLOAT, GL_FALSE, vertexStride, (void*) vertexOffset);
		vertexOffset += sizeof(float[2]);
	}
	else
	{
		Graphics::EnableVertexAttribArray(ModelMeshAttributeChannels::TexCoords1, false);
	}

	// Tex Coords 2
	if((mesh->Flags & ModelMeshFlags::VertexHasTexCoords2) > 0)
	{
		Graphics::EnableVertexAttribArray(ModelMeshAttributeChannels::TexCoords2, true);
		glVertexAttribPointer(ModelMeshAttributeChannels::TexCoords2, 2, GL_FLOAT, GL_FALSE, vertexStride, (void*) vertexOffset);
		vertexOffset += sizeof(float[2]);
	}
	else
	{
		Graphics::EnableVertexAttribArray(ModelMeshAttributeChannels::TexCoords2, false);
	}
	
	// Skinning
	UInt16 skinningMode = (mesh->Flags & ModelMeshFlags::SKINNING_TYPE_MASK);
	if(skinningMode == ModelMeshFlags::VertexHasNoSkinning)
	{
		// No Skinning
		Graphics::EnableVertexAttribArray(ModelMeshAttributeChannels::BoneIndices, false);
		Graphics::EnableVertexAttribArray(ModelMeshAttributeChannels::BoneWeights, false);
	}
	else if(skinningMode == ModelMeshFlags::VertexHasSingleBone)
	{
		// Single Bone Skinning
		Graphics::EnableVertexAttribArray(ModelMeshAttributeChannels::BoneIndices, true);
		Graphics::EnableVertexAttribArray(ModelMeshAttributeChannels::BoneWeights, false);
		glVertexAttribPointer(ModelMeshAttributeChannels::BoneIndices, 1, GL_UNSIGNED_SHORT, GL_FALSE, vertexStride, (void*) vertexOffset);
		vertexOffset += sizeof(UInt16);
	}
	else if(skinningMode == ModelMeshFlags::VertexHas2WeightedBones)
	{
		// 2 Bone Skinning
		Graphics::EnableVertexAttribArray(ModelMeshAttributeChannels::BoneIndices, true);
		Graphics::EnableVertexAttribArray(ModelMeshAttributeChannels::BoneWeights, true);
		glVertexAttribPointer(ModelMeshAttributeChannels::BoneIndices, 2, GL_UNSIGNED_SHORT, GL_FALSE, vertexStride, (void*) vertexOffset);
		vertexOffset += sizeof(UInt16[2]);
		glVertexAttribPointer(ModelMeshAttributeChannels::BoneWeights, 2, GL_FLOAT, GL_FALSE, vertexStride, (void*) vertexOffset);
		vertexOffset += sizeof(float[2]);
	}
	else if(skinningMode == ModelMeshFlags::VertexHas4WeightedBones)
	{
		// 4 Bone Skinning
		Graphics::EnableVertexAttribArray(ModelMeshAttributeChannels::BoneIndices, true);
		Graphics::EnableVertexAttribArray(ModelMeshAttributeChannels::BoneWeights, true);
		glVertexAttribPointer(ModelMeshAttributeChannels::BoneIndices, 4, GL_UNSIGNED_SHORT, GL_FALSE, vertexStride, (void*) vertexOffset);
		vertexOffset += sizeof(UInt16[4]);
		glVertexAttribPointer(ModelMeshAttributeChannels::BoneWeights, 4, GL_FLOAT, GL_FALSE, vertexStride, (void*) vertexOffset);
		vertexOffset += sizeof(float[4]);
	}
	
	// Turn off the other attribute arrays
	for(int i = ModelMeshAttributeChannels::FIRST_UNUSED; i < Graphics::MAX_VERTEX_ATTRIBUTES; i++)
	{
		Graphics::EnableVertexAttribArray(i, false);
	}

	// Make sure the stride matches the vertex offset (DEBUG CHECK)
	ASSERT(vertexStride == vertexOffset, L"Vertex Stride did not match calculated vertex data size");
}
	
// ***********************************************************************
void Model::Draw(ModelInstance* modelInstance)
{
	// NOTE: The caller is responsible for the Global Shared Uniforms
	// - View / Projection transforms
	// - Lighting

	// TODO(P1) - update this list ^^ with uniforms used by the shaders...

	// TODO(P1) - need shader library, that we select from during rendering
	// TODO(P1) - need some way to specify custom material selection from the collada source  (Like: Use GlowShaderX)
	// TODO(P1) - export/import  bone indices/weights 
	// TODO(P1) - export/import  node animations

	// Temp variables
	Matrix3D world;

	// Render the Mesh Instances
	// -----------------------------

	// Loop through the mesh instances
	for(vector<ModelMeshInstance*>::iterator meshInstanceIter = this->MeshInstances.begin(); meshInstanceIter != this->MeshInstances.end(); meshInstanceIter++)
	{
		// Get this mesh instance
		ModelMeshInstance* meshInstance = *meshInstanceIter;
		ModelMesh* mesh = this->Meshes[meshInstance->MeshIndex];
		ModelNode* node = this->Nodes[meshInstance->NodeIndex];

		// Get the world matrix for rendering this mesh instance
		if(modelInstance == NULL)
		{
			// Use the model's world matrix & node tree
			world = node->AbsoluteTransform * this->World;
		}
		else
		{
			// Use the model instance's world matrix & node tree
			world = modelInstance->AbsoluteTransforms[meshInstance->NodeIndex] * modelInstance->World;
		}

		// Setup the world transform
		Graphics::GlobalUniforms.World->SetMatrix4(world);

		// Bind the vertex & index buffers 
		Graphics::BindVertexBuffer(mesh->VertexBuffer);
		Graphics::BindIndexBuffer(mesh->IndexBuffer);

		// Setup the vertex attributes for this mesh
		SetupVertexAttributeChannels(mesh);

		// Loop through the mesh instances
		for(size_t meshPartIndex = 0; meshPartIndex < mesh->MeshParts.size(); meshPartIndex++)
		{
			// Get this mesh part
			ModelMeshPart* meshPart = mesh->MeshParts[meshPartIndex];

			// Get the material index for this mesh part
			UInt16 materialIndex = meshInstance->MaterialBindings[meshPartIndex];

			// Get the material from the model
			ModelMaterial* material = this->Materials[materialIndex];

			// Setup the basic material properties
			Graphics::GlobalUniforms.Material.Emissive->SetFloat4(material->Emissive);
			Graphics::GlobalUniforms.Material.Ambient->SetFloat4(material->Ambient);
			Graphics::GlobalUniforms.Material.Diffuse->SetFloat4(material->Diffuse);
			Graphics::GlobalUniforms.Material.Specular->SetFloat4(material->Specular);
			Graphics::GlobalUniforms.Material.Shininess->SetFloat(material->Shininess);

			// Do we need to copy the diffuse color to the color vertex attribute channel?
			if((mesh->Flags & ModelMeshFlags::VertexHasColor) == 0)
				glVertexAttrib4fv(ModelMeshAttributeChannels::Color, material->Diffuse);

			// Bind textures
			if((material->Flags & ModelMaterialFlags::DiffuseTextured) > 0 && material->DiffuseTexture != NULL)
				Graphics::BindTexture(material->DiffuseTexture->GLTextureId, TextureUnit::Unit_0);
			if((material->Flags & ModelMaterialFlags::BumpTextured) > 0 && material->BumpTexture != NULL)
				Graphics::BindTexture(material->BumpTexture->GLTextureId, TextureUnit::Unit_1);

			// Get the shader that we should be using
			Shader* shader = DetermineShader(mesh, material);
			if(shader != NULL)
			{
				// Apply the shader
				shader->Apply();

				// Render the mesh part
				glDrawElements(GL_TRIANGLES, meshPart->IndexCount, GL_UNSIGNED_SHORT, (void*) meshPart->IndexStart);
			}

		} // foreach( MeshPart )
	} // foreach( Mesh )
}


// ***********************************************************************
Shader* Model::DetermineShader(ModelMesh* mesh, ModelMaterial* material)
{
	// Determine the shader to use based on the mesh & material passed in

	// What type of skinning data does the mesh contain
	UInt16 skinningType = (mesh->Flags & ModelMeshFlags::SKINNING_TYPE_MASK);
	if(skinningType == ModelMeshFlags::VertexHasNoSkinning)
	{
		// Does the material have no diffuse texture?
		if((material->Flags & ModelMaterialFlags::DiffuseTextured) == 0)
			return SharedAssets::Shaders.Model.Mesh.NonTextured;
		else
			return SharedAssets::Shaders.Model.Mesh.DiffuseTextured;
	}

	// No suitable shader found!
	ASSERT(false, L"Unable to find a suitable Shader for rendering the mesh \"%hs\" with the material \"%hs\"", mesh->Id.c_str(), material->Id.c_str());
	return NULL;
}