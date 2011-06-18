
#pragma once

#include "GdkLIB.h"

namespace Gdk
{
	// ===================================================
	class ModelInstance
	{
	public:
		// Properties
		class Model*					ParentModel;
		vector<class ModelMaterial*>	Materials;

		// Methods
		~ModelInstance();
		void Draw();

	private:
		
		// Private CTor
		ModelInstance();
		friend class Model;
	};

} // namespace