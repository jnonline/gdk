/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#pragma once

namespace Gdk
{
    /// @addtogroup Graphics
    /// @{
    
    // =================================================================================
    ///	@brief
    ///		Contains information for a single BMFont character
    // =================================================================================
	class BMFontCharacter
	{
	public:
        
        // Publics
		// =====================================================
        
        // ---------------------------------
        /// @name Read-only Properties
        /// @{
        
        /// X Coordinate of the character's image within the texture.  [Read-only]
		unsigned short	X;
        
        /// X Coordinate of the character's image within the texture.  [Read-only]
		unsigned short	Y;
        
        /// Width of the character's image within the texture.  [Read-only]
		unsigned short	Width;
        
        /// Width of the character's image within the texture.  [Read-only]
		unsigned short	Height;
        
        /// The X-offset of the destination position when blitting the character's image.  [Read-only]
		short			XOffset;
        
        /// The Y-offset of the destination position when blitting the character's image.  [Read-only]
		short			YOffset;
        
        /// The number of pixels to advance the screen position after drawing this character.  [Read-only]
		unsigned short	XAdvance;
        
        /// Index of the texture page that contains this character image (within the BMFont.pages).  [Read-only]
		unsigned char	Page;	

		/// @}
        // ---------------------------------
        /// @name Constructors
        /// @{
        
		BMFontCharacter();
		BMFontCharacter(unsigned short x, unsigned short y, unsigned short width, unsigned short height, 
			unsigned short xOffset, unsigned short yOffset, unsigned short xAdvance, unsigned char page);

        /// @}
        // ---------------------------------
        /// @name Operators
        /// @{
        
		inline BMFontCharacter& operator= (const BMFontCharacter& input);
        
        /// @}
	};

    // =================================================================================
    ///	@brief
    ///		A map of BMFontCharacter instances for the corresponding wide character
    // =================================================================================
    typedef map<char, BMFontCharacter> BMFontCharacterMap;
    
	
    // =================================================================================
    ///	@brief
    ///		Manages a font resource created by the BMFont tool. 
    // =================================================================================
	class BMFont : public Resource
	{
	public:
		
        // Publics
		// =====================================================
        
        // ---------------------------------
        /// @name Methods
        /// @{
        
		~BMFont();

		Vector2 GetTextSize(const char *text);

        /// @}
        
    protected:
        
        // Protected Methods
        // =====================================================
        
        // ---------------------------------
        /// @name Virtuals from Resouce
        /// @{
        
        /// @} 
        
	private:
        
        // Private Properties
		// =====================================================
        
		unsigned short lineHeight;
		unsigned short base;
		unsigned short scaleW;
		unsigned short scaleH;
		BMFontCharacterMap characters;
		vector<Texture2D*> pages;
		
        // Private Methods
        // =====================================================
        
        friend class Renderer2D;
        friend class Drawing2D;
        friend class BMFontManager;
        
		BMFont();
        
        void LoadFromAsset();
	};
    
    /// @}
    
    
    // *****************************************************************
    /// @brief
    ///     Assignment operator: copies the values of the given character to this character
    // *****************************************************************
	inline BMFontCharacter& BMFontCharacter::operator= (const BMFontCharacter& input)
	{
		X = input.X;
		Y = input.Y;
		Width = input.Width;
		Height = input.Height;
		XOffset = input.XOffset;
		YOffset = input.YOffset;
		XAdvance = input.XAdvance;
		Page = input.Page;
		return *this;
	}

} // namespace
