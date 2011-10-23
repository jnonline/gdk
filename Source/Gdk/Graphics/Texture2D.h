/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#pragma once


#include "Enums.h"
#include "../Assets/Asset.h"

namespace Gdk
{
	/// @addtogroup Graphics
    /// @{
    
    // =================================================================================
    ///	@brief
    ///		Manages a 2D OpenGL texture.
    // =================================================================================
    class Texture2D
	{
	public:

        // Publics
		// =====================================================
        
        // ---------------------------------
        /// @name Read-only Properties
        /// @{
        
		/// Width of the texture, in pixels.  [Read-only]  
		int Width;
        
        /// Height of the texture, in pixels.  [Read-only]
		int Height;
        
        /// Width of a single texel, in normalized 0-1 texture space.  [Read-only]
		float TexelWidth;
        
        /// Height of a single texel, in normalized 0-1 texture space.  [Read-only]
		float TexelHeight;
        
        /// Pixel format of the texture.  [Read-only]
		PixelFormat::Enum Format;
        
        /// OpenGL id of the texture.  [Read-only]
		GLuint GLTextureId;

        /// @}
        // ---------------------------------
        /// @name Methods
        /// @{
        
		Texture2D(int width, int height, PixelFormat::Enum pixelFormat);
		~Texture2D();
        
        void SetImageData(void* pixelData);
        
        void GenerateMipMaps();

        TextureWrapMode::Enum GetWrapMode()			{ return wrapMode; }
		void SetWrapMode(TextureWrapMode::Enum value);
		TextureFilterMode::Enum GetFilterMode()		{ return filterMode; }
		void SetFilterMode(TextureFilterMode::Enum value);
        
		static Texture2D* FromFile(const char *imageFilePath);
		static Texture2D* FromStream(Stream* stream);

		static const AssetType::Enum ASSET_TYPE = AssetType::Texture2D;
		static Texture2D* FromAsset(AssetLoadContext* context);
		void ReleaseChildAssets() {}

        /// @}
        
	private:

        // Privates
		// =====================================================
        
		// Private Properties
		bool hasMipMaps;
		TextureFilterMode::Enum filterMode;
		TextureWrapMode::Enum wrapMode;

		// Texture Memory tracking
		static int totalMemoryUsed;
		int memoryUsed;
	};
    
    /// @}

} // namespace 

