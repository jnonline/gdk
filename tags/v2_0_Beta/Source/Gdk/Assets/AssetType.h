/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#pragma once



namespace Gdk
{
	// ============================================================================

	namespace AssetType
	{
		enum Enum
		{
			// Do NOT change the numeric IDs of these constants

			// Textures & Images
			Texture1D = 100,
			Texture2D = 101,
			Texture3D = 102,
			TextureCube = 103,

			Shader = 110,
			Material = 111,

			// 2D assets
			Atlas = 120,
			BMFont = 121,

			// 3D assets
			Model = 130,
		};
	}

	// ============================================================================

} // namespace 
