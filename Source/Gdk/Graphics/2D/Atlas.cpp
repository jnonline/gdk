
#include "BasePCH.h"
#include "Atlas.h"

using namespace Gdk;

// ************************************************************************************************
// AtlasImage
// ************************************************************************************************

AtlasImage::AtlasImage()
{
}

AtlasImage::~AtlasImage()
{
}

// ************************************************************************************************
// AtlasAnimation
// ************************************************************************************************

AtlasAnimation::AtlasAnimation()
{
}

AtlasAnimation::~AtlasAnimation()
{
}

// ************************************************************************************************
// AtlasSheet
// ************************************************************************************************

AtlasSheet::AtlasSheet()
{
	Texture = NULL;
}

AtlasSheet::~AtlasSheet()
{
	// Destroy the texture
	if(Texture != NULL)
	{
		GdkDelete(Texture);
	}
}

// ************************************************************************************************
// Atlas
// ************************************************************************************************

// -----------------------------------------
Atlas::Atlas()
{
}


// -----------------------------------------
Atlas::~Atlas()
{
	// Delete all the animations
	vector<AtlasAnimation*>::iterator animIter;
	for(animIter = Animations.begin(); animIter != Animations.end(); animIter++)
	{
		GdkDelete (*animIter);
	}
	Animations.clear();

	// Delete all the images
	vector<AtlasImage*>::iterator imageIter;
	for(imageIter = Images.begin(); imageIter != Images.end(); imageIter++)
	{
		GdkDelete (*imageIter);
	}
	Images.clear();

	// Delete all the sheets
	vector<AtlasSheet*>::iterator sheetIter;
	for(sheetIter = Sheets.begin(); sheetIter != Sheets.end(); sheetIter++)
	{
		GdkDelete (*sheetIter);
	}
	Sheets.clear();

	// Clear the maps
	ImagesByName.Clear();
	AnimationsByName.Clear();
}

// -----------------------------------------
AtlasImage* Atlas::GetImage(const char* imageName)
{
	// Get the image with the given name
	AtlasImageNameMap::Iterator iter = this->ImagesByName.Find(imageName);
	if(iter == this->ImagesByName.End())
	{
		LOG_WARN(L"Unable to find the image [%hs] in the atlas [%hs]", imageName, this->Name.c_str());
		return NULL;
	}

	return iter->second;
}

// -----------------------------------------
AtlasAnimation* Atlas::GetAnimation(const char* animationName)
{
	// Get the animation with the given name
	AtlasAnimationNameMap::Iterator iter = this->AnimationsByName.Find(animationName);
	if(iter == this->AnimationsByName.End())
	{
		LOG_WARN(L"Unable to find the animation [%hs] in the atlas [%hs]", animationName, this->Name.c_str());
		return NULL;
	}

	return iter->second;
}

// **************************************************************************
class FileAtlasChildLoader : public Atlas::AtlasChildLoader
{
public:
	string Folder;
	FileAtlasChildLoader(const char* folder) : Folder(folder) {}
	Texture2D* LoadTexture(const char *textureName)
	{
		string fullPath = Path::Combine(Folder.c_str(), textureName);
		fullPath.append(".gdkimage");
		return Texture2D::FromFile(fullPath.c_str());
	}
};

// **************************************************************************
class ProviderAtlasChildLoader : public Atlas::AtlasChildLoader
{
public:
	AssetProvider* Provider;
	ProviderAtlasChildLoader(AssetProvider* provider) : Provider(provider) {}
	Texture2D* LoadTexture(const char *textureName)
	{
		Stream* textureStream = Provider->GetStream(textureName, AssetType::Texture2D);
		return Texture2D::FromStream(textureStream);
	}
};

// **************************************************************************
Atlas* Atlas::FromFile(const char *atlasFilePath)
{
	// Get the name of the atlas
	string atlasName = Path::GetFileNameWithoutExtension(atlasFilePath);

	// Open the file as a stream
	FileStream fileStream(atlasFilePath, FileMode::Read);

	// Create a loader for the child assets
	string atlasFolder = Path::GetDirectory(atlasFilePath);
	FileAtlasChildLoader childLoader(atlasFolder.c_str());

	// Load the atlas from the stream
	Atlas* atlas = FromStream(atlasName.c_str(), &fileStream, &childLoader);

	// Close the stream
	fileStream.Close();

	// return
	return atlas;
}

// **************************************************************************
Atlas* Atlas::FromAsset(AssetLoadContext* context)
{
	// Create a loader for the child assets
	ProviderAtlasChildLoader childLoader(context->Provider);

	// Load the atlas from the stream
	return FromStream(context->AssetName, context->AssetStream, &childLoader);
}

// **************************************************************************
namespace AtlasFlags
{
	enum
	{
		FilterModeLinear = 0x0001,		// else Nearest
	};
}

// **************************************************************************
Atlas* Atlas::FromStream(const char *atlasName, Stream* stream, AtlasChildLoader* childLoader)
{
	// Create the new atlas
	Atlas *atlas = GdkNew Atlas();
	atlas->Name = atlasName;

	// Load the sheets
	//----------------------------

	// Get the number of sheets
	short numSheets = stream->ReadInt16();
	atlas->Sheets.reserve(numSheets);

	// Get the atlas flags
	UInt16 atlasFlags = stream->ReadUInt16();

	// Load the sheets
	for(int sheetIndex = 0; sheetIndex < numSheets; sheetIndex++)
	{
		// Load this sheet properties
		short sheetWidth = stream->ReadInt16();
		short sheetHeight = stream->ReadInt16();
		
        UInt8 sheetPixelFormat = stream->ReadUInt8();
        GDK_NOT_USED(sheetPixelFormat);
        
		// Create the sheet
		AtlasSheet* sheet = GdkNew AtlasSheet();
		sheet->Width = sheetWidth;
		sheet->Height = sheetHeight;

		// Build the name of the sheet texture
		char sheetName[256];
		sprintf(sheetName, "%s_sheet_%d", atlasName, sheetIndex);
		
		// Load the sheet texture
		sheet->Texture = childLoader->LoadTexture(sheetName);

		// Setup Texture Filtering
		if((atlasFlags & AtlasFlags::FilterModeLinear) > 0)
			sheet->Texture->SetFilterMode(TextureFilterMode::Linear);
		else
			sheet->Texture->SetFilterMode(TextureFilterMode::Nearest);


		// Add the sheet to the atlas
		atlas->Sheets.push_back(sheet);
	}

	// Load the images
	// -------------------------------

	// Get the number of images
	short numImages = stream->ReadInt16();
	atlas->Images.reserve(numImages);

	// Loop through the images
	for(int imageIndex = 0; imageIndex < numImages; imageIndex++)
	{
		// Create the image
		AtlasImage *image = GdkNew AtlasImage();
		image->Index = imageIndex;

		// Get the image name
		image->Name = stream->ReadString();

		// Get the image's associated sheet
		short sheetIndex = stream->ReadInt16();
		image->Sheet = atlas->Sheets[sheetIndex];

		// Get the size of the image
		image->Width = stream->ReadInt16();
		image->Height = stream->ReadInt16();

		// Get the size & position of the cropped image as it is embedded in the sheet
		image->CroppedImageWidth = stream->ReadInt16();
		image->CroppedImageHeight = stream->ReadInt16();
		image->CroppedImageX = stream->ReadInt16();
		image->CroppedImageY = stream->ReadInt16();

		// Get the Texture coordinates of the cropped image within the sheet texture
		image->TopLeftTexCoord.X = stream->ReadFloat();
		image->TopLeftTexCoord.Y = stream->ReadFloat();
		image->BottomRightTexCoord.X = stream->ReadFloat();
		image->BottomRightTexCoord.Y = stream->ReadFloat();
		
		// Get the reference point of the image
		image->ReferencePoint.X = stream->ReadFloat();
		image->ReferencePoint.Y = stream->ReadFloat();

		// Get the number of child points for this image
		short numChildPoints = stream->ReadInt16();
		image->ChildPoints.reserve(numChildPoints);

		// Load the child points
		for(short childPointIndex = 0; childPointIndex < numChildPoints; childPointIndex++)
		{
			Vector2 childPoint;
			childPoint.X = stream->ReadFloat();
			childPoint.Y = stream->ReadFloat();
			image->ChildPoints.push_back(childPoint);
		}

		// Add the image to the atlas
		atlas->Images.push_back(image);
		atlas->ImagesByName.Add(image->Name.c_str(), image);
	}

	// Load the animations
	// -------------------------------

	// Get the number of animations
	short numAnimations = stream->ReadInt16();
	atlas->Animations.reserve(numAnimations);

	// Loop through the animations
	for(int animIndex = 0; animIndex < numAnimations; animIndex++)
	{
		// Create the animation
		AtlasAnimation *animation = GdkNew AtlasAnimation();

		// Get the animation name
		animation->Name = stream->ReadString();

		// Get the FPS
		animation->FPS = stream->ReadFloat();

		// Get the number of animation frames
		short numFrames = stream->ReadInt16();
		animation->Images.reserve(numFrames);

		// Load the frames
		for(short frameIndex = 0; frameIndex < numFrames; frameIndex++)
		{
			short animImageIndex = stream->ReadInt16();
			animation->Images.push_back( atlas->Images[animImageIndex] );
		}

		// Add the animation to the atlas
		atlas->Animations.push_back(animation);
		atlas->AnimationsByName.Add(animation->Name.c_str(), animation);
	}

	// Return the atlas!
	return atlas;
}