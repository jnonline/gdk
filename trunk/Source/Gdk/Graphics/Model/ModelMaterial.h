
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

		ModelMaterialFlags::Enum	 Flags;

        // Simple Properties
        float	Emissive[4];
        float	Ambient[4];
        float	Diffuse[4];
        float	Specular[4];
        float	Shininess;
       
        // Textures
        Texture2D*	DiffuseTexture;
        Texture2D*	BumpTexture;

		// Shaders
		Shader*		MeshShader;
		//Shader*		SkinnedMeshShader;

		// Public Methods
		// --------------------

		// CTor
		ModelMaterial();

		// Clones this material & it's values
		ModelMaterial* Clone();
	};

} // namespace