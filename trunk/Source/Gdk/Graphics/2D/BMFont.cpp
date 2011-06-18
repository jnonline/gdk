
//#include <stdlib.h>

#include "BasePCH.h"
#include "BMFont.h"

using namespace Gdk;

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


// ***********************************************************************
BMFontCharacter::BMFontCharacter()
{
	X = Y = Width = Height = XOffset = YOffset = XAdvance = Page = 0;
}


// ***********************************************************************
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

// ***********************************************************************
BMFont::BMFont()
{
}

// ***********************************************************************
BMFont::~BMFont()
{
	// Destroy the textures used by the pages
	for(vector<Texture2D*>::iterator iter = pages.begin(); iter != pages.end(); iter++)
	{
		Texture2D *texture = (*iter);
		if(texture != NULL)
		{
			GdkDelete(texture);
		}
	}
}

// ********************************************************************
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
                LOG_WARN(L"The character [%lc][%u] doesn't exist in the character map for the font [%hs]", ch, (UInt32)ch, this->Name.c_str()); 
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

// **************************************************************************
class FileBMFontChildLoader : public BMFont::BMFontChildLoader
{
public:
	string Folder;
	FileBMFontChildLoader(const char* folder) : Folder(folder) {}
	Texture2D* LoadTexture(const char *textureName)
	{
		string fullPath = Path::Combine(Folder.c_str(), textureName);
		fullPath.append(".gdkimage");
		return Texture2D::FromFile(fullPath.c_str());
	}
};

// **************************************************************************
class ProviderBMFontChildLoader : public BMFont::BMFontChildLoader
{
public:
	AssetProvider* Provider;
	ProviderBMFontChildLoader(AssetProvider* provider) : Provider(provider) {}
	Texture2D* LoadTexture(const char *textureName)
	{
		Stream* textureStream = Provider->GetStream(textureName, AssetType::Texture2D);
		return Texture2D::FromStream(textureStream);
	}
};

// **************************************************************************
BMFont* BMFont::FromFile(const char *fontFilePath)
{
	// Get the name of the font
	string fontName = Path::GetFileNameWithoutExtension(fontFilePath);

	// Open the file as a stream
	FileStream fileStream(fontFilePath, FileMode::Read);

	// Create a loader for the child assets
	string fontFolder = Path::GetDirectory(fontFilePath);
	FileBMFontChildLoader childLoader(fontFolder.c_str());

	// Load the font from the stream
	BMFont* font = FromStream(fontName.c_str(), &fileStream, &childLoader);

	// Close the stream
	fileStream.Close();

	// return
	return font;
}

// **************************************************************************
BMFont* BMFont::FromAsset(AssetLoadContext* context)
{
	// Create a loader for the child assets
	ProviderBMFontChildLoader childLoader(context->Provider);

	// Load the font from the stream
	return FromStream(context->AssetName, context->AssetStream, &childLoader);
}


// ********************************************************************
BMFont* BMFont::FromStream(const char *fontName, Stream* stream, BMFontChildLoader* childLoader)
{
	// Create the new font
	BMFont *font = new BMFont();
	font->Name = fontName;

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
			font->lineHeight = commonBlock->LineHeight;
			font->base = commonBlock->Base;
			font->scaleW = commonBlock->ScaleW;
			font->scaleH = commonBlock->ScaleH;
			numPages = commonBlock->Pages;
			font->pages.reserve(numPages);

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
				string pageAssetPath = Path::Combine(Path::GetDirectory(fontName).c_str(), pageFileName.c_str());

				// Load this page's texture
				Texture2D *pageTexture = childLoader->LoadTexture(pageAssetPath.c_str());

				// Add the page to the font
				font->pages.push_back(pageTexture);
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
				font->characters[charBlock[i].Id] = fontChar;
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

	// Return the font!
	return font;
}