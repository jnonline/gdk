/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#include "BasePCH.h"

using namespace Gdk;

/// @cond INTERNAL

// ===============================
// .FNT file structures

typedef struct _BMFFcommon
{
	unsigned short	LineHeight;
	unsigned short	Base;
	unsigned short	ScaleW;
	unsigned short	ScaleH;
	unsigned short	Pages;
	unsigned char	BitField;
	unsigned char	AlphaChannel;
	unsigned char	RedChannel;
	unsigned char	GreenChannel;
	unsigned char	BlueChannel;
} BMFFcommon;


typedef struct _BMFFchar
{
	unsigned int	Id;
	unsigned short	X;
	unsigned short	Y;
	unsigned short	Width;
	unsigned short	Height;
	unsigned short	XOffset;
	unsigned short	YOffset;
	unsigned short	XAdvance;
	unsigned char	Page;
	unsigned char	Channel;	
} BMFFchar;

/// @endcond

// *****************************************************************
/// @brief
///     Default Constructor
/// @remarks
///     GDK Internal Use Only
// *****************************************************************
BMFontCharacter::BMFontCharacter()
{
	X = Y = Width = Height = XOffset = YOffset = XAdvance = Page = 0;
}


// *****************************************************************
/// @brief
///     Constructs a new instance with the given property values
/// @remarks
///     GDK Internal Use Only
// *****************************************************************
BMFontCharacter::BMFontCharacter(unsigned short x, unsigned short y, unsigned short width, unsigned short height, 
		unsigned short xOffset, unsigned short yOffset, unsigned short xAdvance, unsigned char page)
{
	X = x;
	Y = y;
	Width = width;
	Height = height;
	XOffset = xOffset;
	YOffset = yOffset;
	XAdvance = xAdvance;
	Page = page;
}

// *****************************************************************
/// @brief
///     Constructs a new BMFont 
/// @remarks
///     GDK Internal Use Only
// *****************************************************************
BMFont::BMFont()
{
}

// *****************************************************************
/// @brief
///     Destructor
// *****************************************************************
BMFont::~BMFont()
{
	// Release the textures used by the pages
	for(vector<Texture2D*>::iterator iter = pages.begin(); iter != pages.end(); iter++)
	{
		Texture2D *texture = (*iter);
		texture->Release();
	}
}

// *****************************************************************
/// @brief
///     Gets the size of the given string in pixel coordinates.
/// @param text
///     Text to get the size of
// *****************************************************************
Vector2 BMFont::GetTextSize(const wchar_t *text)
{
	int maxXSize = 0;
	int xSize = 0;
	int ySize = 0;
	
	// Is there any text at all?
	if(text == 0 || *text == 0)
	{
		return Vector2(0,0);
	}

	// Set the initial y size
	ySize = this->lineHeight;

	// Walk through the input text
	const wchar_t *current = text;
	while(*current != 0)
	{
        wchar_t ch = *current;
		if(ch == L'\n')
		{
			// Move to the next line
			if(xSize > maxXSize)
				maxXSize = xSize;
			xSize = 0;
			ySize += this->lineHeight;
		}
		else
		{
			// Get the BMFontCharacter associated with this character
            BMFontCharacterMap::iterator iter = this->characters.find(ch);
            if(iter != this->characters.end())
            {
                BMFontCharacter &fontChar = iter->second;

                // Move the x position to after the character
                xSize += fontChar.XAdvance;
            }
            else
            {
                // Character doesnt exist
                LOG_WARN(L"The character [%lc][%u] doesn't exist in the character map for the font [%hs]", ch, (UInt32)ch, this->GetName().c_str()); 
            }
		}

		// next character
		current++;
	}

	// Is this last line the longest?
	if(xSize > maxXSize)
		maxXSize = xSize;

	return Vector2((float)maxXSize, (float)ySize);
}

// *****************************************************************
/// @brief
///     This method is called by the resource manager when the resource data is to be loaded from an asset
/// @remarks
///     GDK Internal Use Only
// *****************************************************************
void BMFont::LoadFromAsset()
{
    // Get a stream to the asset
    char assetPath[256];
    GDK_SPRINTF(assetPath, 256, "%s.gdkfont", GetName().c_str());
    Stream* stream = AssetManager::GetAssetStream(assetPath);
    
	// Read & parse the font file
	//----------------------------

	short numPages = 1;

	// Skip the first 4 bytes (first 3 = "BMF", last one = Version)
	char bmid[4];
	stream->Read(bmid, 4);

	// While not at the end of the stream
	while(stream->AtEnd() == false)
	{
		// Read the next block type & size
		char blockType = stream->ReadInt8();
		int blockSize = stream->ReadInt32();

		// Process the block type
		if(blockType == 2) // Common
		{
			// Read in the common block
			BMFFcommon *commonBlock = (BMFFcommon*)GdkAlloc(blockSize);
			stream->Read(commonBlock, blockSize);

			// Set the font properties
			this->lineHeight = commonBlock->LineHeight;
			this->base = commonBlock->Base;
			this->scaleW = commonBlock->ScaleW;
			this->scaleH = commonBlock->ScaleH;
			numPages = commonBlock->Pages;
			this->pages.reserve(numPages);

			// Free the commonBlock
			GdkFree(commonBlock);
		}
		else if(blockType == 3) // Pages
		{
			// Read in the pages block
			char *pages = (char*)GdkAlloc(blockSize);
			stream->Read(pages, blockSize);

			// Parse out the page names
			short nameLen = blockSize / numPages;
			for(int page = 0; page < numPages; page++)
			{
				// Extract the page file name (and remove the .png extension)
				char* pageFile = &pages[page * nameLen];
				string pageFileName = Path::GetFileNameWithoutExtension(pageFile);

				// Prepend the asset path
                string assetFolder = Path::GetDirectory(GetName().c_str());
				string pageAssetPath = Path::Combine(assetFolder.c_str(), pageFileName.c_str());

				// Load this page's texture
                Texture2D *pageTexture = Texture2DManager::FromAsset(pageAssetPath.c_str());

				// Add the page to the font
				this->pages.push_back(pageTexture);
			}
			
			// Free the pages block
			GdkFree(pages);
		}
		else if(blockType == 4) // Chars
		{
			// Read in the char block(s)
			int numChars = blockSize / sizeof(BMFFchar);
			BMFFchar *charBlock = (BMFFchar*)GdkAlloc(blockSize);
			stream->Read(charBlock, blockSize);

			// Processes the chars
			for(int i=0; i<numChars; i++)
			{
				BMFontCharacter fontChar(
					charBlock[i].X,
					charBlock[i].Y,
					charBlock[i].Width,
					charBlock[i].Height,
					charBlock[i].XOffset,
					charBlock[i].YOffset,
					charBlock[i].XAdvance,
					charBlock[i].Page
					);

				// Add the char to the character map
				this->characters[charBlock[i].Id] = fontChar;
			}

			// Free the charBlock
			GdkFree(charBlock);
		}
		else
		{
			// Skip any other block
			stream->Seek(blockSize, SeekOrigin::Current);
		}
	}
}