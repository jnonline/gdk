/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#pragma once

#include "GdkLIB.h"
#include "ModelMeshInstance.h"

namespace Gdk
{

	// ============================================================================
	class ModelSkeletalMeshInstance : public ModelMeshInstance
	{
	public:
		
		// Properties 
		vector<UInt16>	JointNodes;				// This vector has 1 entry per joint.  Each value is the index of a Node that the joint is bound to

		// CTor/DTor
		ModelSkeletalMeshInstance();
		~ModelSkeletalMeshInstance();
	};	

} // namespace