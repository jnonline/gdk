/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#pragma once

#include "GdkLIB.h"

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

		// CTor/DTor
		ModelMeshInstance();
		~ModelMeshInstance();

	};

	

} // namespace