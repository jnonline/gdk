/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#pragma once

#include "GdkLIB.h"

#include "../../Math/2D/Vector2.h"
#include "../../Assets/Asset.h"
#include "../Texture2D.h"

namespace Gdk
{
	// ============================================================================

	class BMFontCharacter
	{
	public:
		unsigned short	X;
		unsigned short	Y;
		unsigned short	Width;
		unsigned short	Height;
		short			XOffset;
		short			YOffset;
		unsigned short	XAdvance;
		unsigned char	Page;	

		// Constructor
		BMFontCharacter();
		BMFontCharacter(unsigned short x, unsigned short y, unsigned short width, unsigned short height, 
			unsigned short xOffset, unsigned short yOffset, unsigned short xAdvance, unsigned char page);

		// Assignment
		inline BMFontCharacter& operator= (const BMFontCharacter& input);
	};

    typedef map<wchar_t, BMFontCharacter> BMFontCharacterMap;
    
	// ***********************************************************************
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

	// ============================================================================

	class BMFont
	{
	public:
		
		// Constructor / Destructor
		~BMFont();

		// Properties
		string Name;

		// Utility Methods
		Vector2 GetTextSize(const wchar_t *text);

		// Creation Methods
		static BMFont* FromFile(const char *fontFilePath);

		// Asset Interfaces
		static const AssetType::Enum ASSET_TYPE = AssetType::BMFont;
		static BMFont* FromAsset(AssetLoadContext* context);
		void IgnoreChildAssets() {}

        
        // Internal types
		class BMFontChildLoader
		{
		public:
			virtual Texture2D* LoadTexture(const char *textureName) = 0;
		};
        
	private:
        friend class Renderer2D;

		// Private Properties
		unsigned short lineHeight;
		unsigned short base;
		unsigned short scaleW;
		unsigned short scaleH;
		BMFontCharacterMap characters;
		vector<Texture2D*> pages;
		
	private:

		// private methods
		BMFont();
		static BMFont* FromStream(const char *fontName, Stream* stream, class BMFontChildLoader* childLoader);

	};

} // namespace
