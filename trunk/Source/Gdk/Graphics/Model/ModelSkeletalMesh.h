/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#pragma once

#include "GdkLIB.h"
#include "ModelMesh.h"

namespace Gdk
{
	// ============================================================================
	class ModelSkeletalMesh : public ModelMesh
	{
	public:
		
		// Properties 
		UInt16				NumJoints;
		vector<Matrix3D>	JointInvBindMatrices;

		// CTor / DTor
		ModelSkeletalMesh();
		~ModelSkeletalMesh();
	};

} // namespace