/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#include "BasePCH.h"

using namespace Gdk;

// *****************************************************************
/// @brief
///     Constructs a new AtlasImage 
// *****************************************************************
AtlasImage::AtlasImage()
{
}

// *****************************************************************
/// @brief
///     Destructor
// *****************************************************************
AtlasImage::~AtlasImage()
{
}

// *****************************************************************
/// @brief
///     Gets a 4 vertex quad that can be used to render the atlas image.
/// @param vertices
///     An array of 4 vertices that will be initialized with the vertex data for drawing this atlas image
/// @param color
///     The color that will be set for each vertex
// *****************************************************************
void AtlasImage::GetQuad(VertexP2T2C4* vertices, const Color& color)
{
    // Calculate the extents of the quad, relative to the reference point
	float extentLeft = (this->CroppedImageX - this->Width * this->ReferencePoint.X);
	float extentRight = extentLeft + this->CroppedImageWidth;
	float extentTop = (this->CroppedImageY - this->Height * this->ReferencePoint.Y);
	float extentBottom = extentTop + this->CroppedImageHeight;
    
	float dx1 = extentLeft,		dy1 = extentTop;
	float dx2 = extentRight,	dy2 = extentTop;
	float dx3 = extentLeft,		dy3 = extentBottom;
	float dx4 = extentRight,	dy4 = extentBottom;
    
    vertices[0] = VertexP2T2C4(dx1, dy1, this->TopLeftTexCoord.X, this->TopLeftTexCoord.Y, color.R, color.G, color.B, color.A);
    vertices[1] = VertexP2T2C4(dx2, dy2, this->BottomRightTexCoord.X, this->TopLeftTexCoord.Y, color.R, color.G, color.B, color.A);
    vertices[2] = VertexP2T2C4(dx3, dy3, this->TopLeftTexCoord.X, this->BottomRightTexCoord.Y, color.R, color.G, color.B, color.A);
    vertices[3] = VertexP2T2C4(dx4, dy4, this->BottomRightTexCoord.X, this->BottomRightTexCoord.Y, color.R, color.G, color.B, color.A);
}

// *****************************************************************
/// @brief
///     Gets a 4 vertex quad that can be used to render the atlas image into a given rectangle
/// @param vertices
///     An array of 4 vertices that will be initialized with the vertex data for drawing this atlas image
/// @param color
///     The color that will be set for each vertex
// *****************************************************************
void AtlasImage::GetFittedQuad(VertexP2T2C4* vertices, const Rectangle2& fitToRect, const Color& color)
{
	// Calculate the positions of the cropped image within the given destination rectangle
	float left = fitToRect.Position.X + fitToRect.Size.X * this->CroppedImageX / this->Width;
	float right = left + fitToRect.Size.X * this->CroppedImageWidth / this->Width;
	float top = fitToRect.Position.Y + fitToRect.Size.Y * this->CroppedImageY / this->Height;
	float bottom = top + fitToRect.Size.Y * this->CroppedImageHeight / this->Height;
    
	float dx1 = left,		dy1 = top;
	float dx2 = right,		dy2 = top;
	float dx3 = left,		dy3 = bottom;
	float dx4 = right,		dy4 = bottom;
    
    vertices[0] = VertexP2T2C4(dx1, dy1, this->TopLeftTexCoord.X, this->TopLeftTexCoord.Y, color.R, color.G, color.B, color.A);
    vertices[1] = VertexP2T2C4(dx2, dy2, this->BottomRightTexCoord.X, this->TopLeftTexCoord.Y, color.R, color.G, color.B, color.A);
    vertices[2] = VertexP2T2C4(dx3, dy3, this->TopLeftTexCoord.X, this->BottomRightTexCoord.Y, color.R, color.G, color.B, color.A);
    vertices[3] = VertexP2T2C4(dx4, dy4, this->BottomRightTexCoord.X, this->BottomRightTexCoord.Y, color.R, color.G, color.B, color.A);
}


// *****************************************************************
/// @brief
///     Constructs a new ~AtlasAnimation 
// *****************************************************************
AtlasAnimation::AtlasAnimation()
{
}

// *****************************************************************
/// @brief
///     Destructor
// *****************************************************************
AtlasAnimation::~AtlasAnimation()
{
}

// *****************************************************************
/// @brief
///     Constructs a new AtlasSheet 
// *****************************************************************
AtlasSheet::AtlasSheet()
{
	Texture = NULL;
}

// *****************************************************************
/// @brief
///     Destructor
// *****************************************************************
AtlasSheet::~AtlasSheet()
{
	// Releaes the texture
	if(Texture != NULL)
	{
		Texture->Release();
	}
}

// *****************************************************************
/// @brief
///     Constructs a new Atlas 
/// @remarks
///     GDK Internal Use Only
// *****************************************************************
Atlas::Atlas()
{
}

// *****************************************************************
/// @brief
///     Destructor
// *****************************************************************
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

// *****************************************************************
/// @brief
///     Gets the AtlasImage with the given name.
/// @param name
///     Name of the image to retrieve
// *****************************************************************
AtlasImage* Atlas::GetImage(const char* imageName)
{
	// Get the image with the given name
	AtlasImageNameMap::Iterator iter = this->ImagesByName.Find(imageName);
	if(iter == this->ImagesByName.End())
	{
		LOG_WARN("Unable to find the image [%s] in the atlas [%s]", imageName, this->GetName().c_str());
		return NULL;
	}

	return iter->second;
}

// *****************************************************************
/// @brief
///     Gets the AtlasAnimation with the given name.
/// @param name
///     Name of the animation to retrieve
// *****************************************************************
AtlasAnimation* Atlas::GetAnimation(const char* animationName)
{
	// Get the animation with the given name
	AtlasAnimationNameMap::Iterator iter = this->AnimationsByName.Find(animationName);
	if(iter == this->AnimationsByName.End())
	{
		LOG_WARN("Unable to find the animation [%s] in the atlas [%s]", animationName, this->GetName().c_str());
		return NULL;
	}

	return iter->second;
}

// *****************************************************************
/// @brief
///     This method is called by the resource manager when the resource data is to be loaded from an asset
/// @remarks
///     GDK Internal Use Only
// *****************************************************************
void Atlas::LoadFromAsset()
{
    // Get a stream to the asset
    char assetPath[256];
    GDK_SPRINTF(assetPath, 256, "%s.gdkatlas", GetName().c_str());
    Stream* stream = AssetManager::GetAssetStream(assetPath);

	// Load the sheets
	//----------------------------

	// Get the number of sheets
	short numSheets = stream->ReadInt16();
	this->Sheets.reserve(numSheets);

	// Get the atlas flags
	UInt16 atlasFlags = stream->ReadUInt16();
    GDK_NOT_USED(atlasFlags);

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
		GDK_SPRINTF(sheetName, 256, "%s_sheet_%d", GetName().c_str(), sheetIndex);
		
		// Load the sheet texture
        sheet->Texture = Texture2DManager::FromAsset(sheetName);

		// Add the sheet to the atlas
		this->Sheets.push_back(sheet);
	}

	// Load the images
	// -------------------------------

	// Get the number of images
	short numImages = stream->ReadInt16();
	this->Images.reserve(numImages);

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
		image->Sheet = this->Sheets[sheetIndex];

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
		this->Images.push_back(image);
		this->ImagesByName.Add(image->Name.c_str(), image);
	}

	// Load the animations
	// -------------------------------

	// Get the number of animations
	short numAnimations = stream->ReadInt16();
	this->Animations.reserve(numAnimations);

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
			animation->Images.push_back( this->Images[animImageIndex] );
		}

		// Add the animation to the atlas
		this->Animations.push_back(animation);
		this->AnimationsByName.Add(animation->Name.c_str(), animation);
	}
}

