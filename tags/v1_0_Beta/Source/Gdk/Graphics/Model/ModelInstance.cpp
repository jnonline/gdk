/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#include "BasePCH.h"
#include "../VertexFormats.h"
#include "ModelInstance.h"
#include "Model.h"
#include "ModelMaterial.h"
#include "ModelMesh.h"

using namespace Gdk;

// ***********************************************************************
ModelInstance::ModelInstance()
{
	this->ParentModel = NULL;
}

// ***********************************************************************
ModelInstance::~ModelInstance()
{
	// Delete any material overrides
	/*
	for(vector<ModelMaterial*>::iterator iter = this->Materials.begin(); iter != this->Materials.end(); iter++)
	{
		ModelMaterial* material = *iter;
		if(material != NULL)
			GdkDelete(material);
	}
	*/
}

// ***********************************************************************
void ModelInstance::Draw()
{
	// Tell the model to draw, using this instance
	this->ParentModel->Draw(this);
}