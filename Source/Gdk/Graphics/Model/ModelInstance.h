/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#pragma once



namespace Gdk
{
	// ===================================================
	class ModelInstance
	{
	public:
		// Properties
		class Model*			ParentModel;

		// Instanced Properties
		Matrix3D				World;
		vector<Matrix3D>		AbsoluteTransforms;
		

		//vector<class ModelMaterial*>	Materials;

		// Methods
		void Draw();
		~ModelInstance();

	private:
		
		// Private CTor
		ModelInstance();
		friend class Model;
	};

} // namespace