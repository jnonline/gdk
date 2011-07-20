/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#pragma once



namespace Gdk
{

	// ============================================================================
	class ModelMeshInstance
	{
	public:
		
		// Properties 
		UInt16 NodeIndex;
		UInt16 MeshIndex;

		vector<UInt16>	MaterialBindings;		// This vector has 1 entry per mesh part.  Each value is an index into the model materials.

		vector<UInt16>	JointNodes;				// This vector has 1 entry per joint.  Each value is the index of a Node that the joint is bound to

		// CTor/DTor
		ModelMeshInstance();
		~ModelMeshInstance();

	};

	

} // namespace