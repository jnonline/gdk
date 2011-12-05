/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#pragma once


#include "../Enums.h"
#include "../../Resource/Resource.h"

namespace Gdk
{
	/// @addtogroup Graphics
    /// @{
    
    // =================================================================================
    ///	@brief
    ///		Manages a 2D OpenGL texture.
    // =================================================================================
    class Texture2D : public Resource
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
        
        /// OpenGL ID of the texture.  [Read-only]
		GLuint GLTextureId;

        /// @}
        // ---------------------------------
        /// @name Methods
        /// @{
        
        virtual ~Texture2D();
        
        void SetImageData(void* pixelData);
        
        void GenerateMipMaps();

        TextureWrapMode::Enum GetWrapMode()			{ return wrapMode; }
		void SetWrapMode(TextureWrapMode::Enum value);
		TextureFilterMode::Enum GetFilterMode()		{ return filterMode; }
		void SetFilterMode(TextureFilterMode::Enum value);

        /// @}
        
    protected:
        
        // Protected Methods
        // =====================================================
        
        // ---------------------------------
        /// @name Virtuals from Resouce
        /// @{
        
        virtual UInt32 GetMemoryUsed();
        
        /// @}
        
	private:

        // Private Properties
		// =====================================================
        
		// Private Properties
		bool hasMipMaps;
		TextureFilterMode::Enum filterMode;
		TextureWrapMode::Enum wrapMode;

        // Private Methods
        // =====================================================
        
        friend class Texture2DManager;
        
        Texture2D();
        
        void Initialize(int width, int height, PixelFormat::Enum pixelFormat);
        void LoadFromAsset();
	};
    
    /// @}

} // namespace 

