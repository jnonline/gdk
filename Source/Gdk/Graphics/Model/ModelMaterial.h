/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#pragma once

#include "GdkLIB.h"
#include "../Color.h"
#include "../Texture2D.h"

namespace Gdk
{
	// ===================================================
	namespace ModelMaterialFlags
	{
		enum Enum
		{
			DiffuseTextured     = 0x0001,
			BumpTextured        = 0x0002,
		};
	}

	// ===================================================
	class ModelMaterial
	{
	public:
		
		// Public Properties
		// --------------------

        // Simple Properties
        string	Name;
		float	Emissive[4];
        float	Ambient[4];
        float	Diffuse[4];
        float	Specular[4];
        float	Shininess;
       
		ModelMaterialFlags::Enum	 Flags;

        // Textures
        Texture2D*	DiffuseTexture;
        Texture2D*	BumpTexture;

		// Shaders
		//Shader*		MeshShader;
		//Shader*		SkinnedMeshShader;

		// Public Methods
		// --------------------

		// CTor / DTor
		ModelMaterial();
		~ModelMaterial();

		// Clones this material & it's values
		//ModelMaterial* Clone();
	};

} // namespace