/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#include "BasePCH.h"
#include "ModelNode.h"

using namespace Gdk;

// ***********************************************************************
ModelNode::ModelNode()
{
	Index = 0;
	ParentNode = NULL;
	LocalTransform = Matrix3D::IDENTITY;
}

// ***********************************************************************
ModelNode::~ModelNode()
{
}

// ***********************************************************************
void ModelNode::UpdateAbsoluteTransforms()
{
	// This method updates the full transform tree from the given node and down.
	// This method should be called whenever the LocalTransform of a node is changed

	// Does this node have a parent?
	if(this->ParentNode != NULL)
	{
		// Our Absolute = Our Local * Parent Absolute
		this->AbsoluteTransform = this->LocalTransform * this->ParentNode->AbsoluteTransform;
	}
	else
	{
		// We dont have a parent, so Our Absolute = Our Local
		this->AbsoluteTransform = this->LocalTransform;
	}

	// Recurse update the child nodes
	for(vector<ModelNode*>::iterator iter = this->ChildNodes.begin(); iter != this->ChildNodes.end(); iter++)
	{
		(*iter)->UpdateAbsoluteTransforms();
	}
}