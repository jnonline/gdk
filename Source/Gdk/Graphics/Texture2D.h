/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#pragma once

#include "GdkLIB.h"
#include "Enums.h"
#include "../Assets/Asset.h"

namespace Gdk
{
	// ============================================================================

	class Texture2D
	{
	public:

		// Properties   (NOTE, users should NEVER change these values)
		int Width;
		int Height;
		float TexelWidth;
		float TexelHeight;
		PixelFormat::Enum Format;
		GLuint GLTextureId;

		// Methods
		Texture2D(int width, int height, PixelFormat::Enum pixelFormat, void* pixelData = NULL, bool generateMipMap = false);
		~Texture2D();

		TextureWrapMode::Enum GetWrapMode()			{ return wrapMode; }
		void SetWrapMode(TextureWrapMode::Enum value);
		TextureFilterMode::Enum GetFilterMode()		{ return filterMode; }
		void SetFilterMode(TextureFilterMode::Enum value);

		// Creation methods
		static Texture2D* FromFile(const char *imageFilePath);
		static Texture2D* FromStream(Stream* stream);

		// Asset Interfaces
		static const AssetType::Enum ASSET_TYPE = AssetType::Texture2D;
		static Texture2D* FromAsset(AssetLoadContext* context);
		void IgnoreChildAssets() {}

	private:

		// Private Properties
		bool hasMipMaps;
		TextureFilterMode::Enum filterMode;
		TextureWrapMode::Enum wrapMode;

		// Texture Memory tracking
		static int totalMemoryUsed;
		int memoryUsed;
	};

} // namespace 

