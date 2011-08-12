/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#include "BasePCH.h"
#include "ModelMaterial.h"

using namespace Gdk;

// ***********************************************************************
ModelMaterial::ModelMaterial()
{
	Flags = (ModelMaterialFlags::Enum) 0;
	DiffuseTexture = NULL;
	BumpTexture = NULL;
}

// ***********************************************************************
ModelMaterial::~ModelMaterial()
{
}

/*
// ***********************************************************************
ModelMaterial* ModelMaterial::Clone()
{
	// Create the new material
	ModelMaterial* newMaterial = GdkNew ModelMaterial();

	// Copy the material properties
	newMaterial->Flags = this->Flags;
	memcpy(newMaterial->Emissive, this->Emissive, sizeof(float[4]));
	memcpy(newMaterial->Ambient, this->Ambient, sizeof(float[4]));
	memcpy(newMaterial->Diffuse, this->Diffuse, sizeof(float[4]));
	memcpy(newMaterial->Specular, this->Specular, sizeof(float[4]));
	newMaterial->Shininess = this->Shininess;

	newMaterial->DiffuseTexture = this->DiffuseTexture;
	newMaterial->BumpTexture = this->BumpTexture;

	newMaterial->MeshShader = this->MeshShader;

	// Return the clone
	return newMaterial;
}
*/