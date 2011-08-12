/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#pragma once



namespace Gdk
{
	// ============================================================================
	class ModelNode
	{
	public:
		
		// Properties 
		string		Name;
		UInt16		Index;
		ModelNode*	ParentNode;
		vector<ModelNode*>	ChildNodes;

		Matrix3D	LocalTransform;
		Matrix3D	AbsoluteTransform;		// Transform from the root node to this node

		// CTor / DTor
		ModelNode();
		~ModelNode();

		// Utility Methods
		void UpdateAbsoluteTransforms();

	};

} // namespace