
#pragma once

#include "GdkLIB.h"

#include "../Shader/Shader.h"

namespace Gdk
{
	// ============================================================================
	class Material
	{
	public:

		// Public Properties
		// --------------------------------

		// Shader
		Shader*	ShaderRef;
		
		// Shader Parameters (Uniform Values)
		ShaderParameterSet* ShaderParameters;

		// Mapping of GL Texture IDs to specific shader uniform parameters
		map<ShaderParameter*, GLuint> TextureBindings;		// value = Texture GL ID

		// Public Methods
		// --------------------------------

		// CTor/DTor
		Material(Shader* shader);
		~Material();

		// Applys this material to the current graphics state
		void Apply();

		// Clones this material
		Material* Clone();

	private:

		// Internal Methods
		// --------------------------------
		
		// Internal CTor
		Material() {}
	};

} // namespace 



