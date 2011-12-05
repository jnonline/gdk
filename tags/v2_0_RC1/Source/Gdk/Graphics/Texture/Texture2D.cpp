/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#include "BasePCH.h"
#include "Texture2D.h"

using namespace Gdk;

// *****************************************************************
/// @brief
///     Constructs a new texture of the given size and pixel format
/// @param name
///     Name of the resource
/// @remarks
///     GDK Internal Use Only
// *****************************************************************
Texture2D::Texture2D()
{
    this->GLTextureId = GL_INVALID_VALUE;
}

// *****************************************************************
/// @brief
///     Destructor
// *****************************************************************
Texture2D::~Texture2D()
{
    // Do we have a GL texture?
    if(this->GLTextureId != GL_INVALID_VALUE)
    {
        // Destroy the GL texture
        glDeleteTextures(1, &(this->GLTextureId));
    }
}

// *****************************************************************
/// @brief
///     Sets the entire image data for the texture
/// @param pixelData
///     A buffer of pixel data with the same size & pixel format as the texture.
// *****************************************************************
void Texture2D::SetImageData(void* pixelData)
{
    // Bind the texture
    Graphics::BindTexture(this->GLTextureId);
    
    // Determine the GL texture format & pixel type 
	GLuint glTextureFormat = PixelFormat::GetGLTextureFormat(this->Format);
	GLuint glPixelType = PixelFormat::GetGLPixelType(this->Format);
    
    // Load the image data into the GL texture
	glTexImage2D(GL_TEXTURE_2D, 0, glTextureFormat, this->Width, this->Height, 0, glTextureFormat, glPixelType, (void*)pixelData);
}

// *****************************************************************
/// @brief
///     Generates mip maps for the texture
/// @note
///     See the OpenGL documentation for glGenerateMipmap() for more information
// *****************************************************************
void Texture2D::GenerateMipMaps()
{
    // Bind the texture
    Graphics::BindTexture(this->GLTextureId);
    
    // Generate mip maps
    glGenerateMipmap(GL_TEXTURE_2D);
}

// *****************************************************************
/// @brief
///     Sets the wrapping mode for this texture
/// @param value
///     The wrapping mode
// *****************************************************************
void Texture2D::SetWrapMode(TextureWrapMode::Enum wrapMode)
{
	// Bind this texture
	Graphics::BindTexture(this->GLTextureId);
    
    // Check if the user is trying to set an invalid wrap mode
	if(wrapMode != TextureWrapMode::Clamp &&
       (Math::IsPowerOfTwo((UInt32)this->Width) == false || Math::IsPowerOfTwo((UInt32)this->Height) == false) )
    {
        LOG_WARN(L"Attempt to set invalid wrapping mode on a Non-PoT texture.  Reverting to Clamped");
        wrapMode = TextureWrapMode::Clamp;
    }
	
	// Set the wrap mode for this texture
    GLenum glValue = TextureWrapMode::GetGLenum(wrapMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, glValue);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, glValue);

	this->wrapMode = wrapMode;
}

// *****************************************************************
/// @brief
///     Sets the filtering mode for the texture
/// @param value
///     The filtering mode
// *****************************************************************
void Texture2D::SetFilterMode(TextureFilterMode::Enum value)
{
	// Bind this texture
	Graphics::BindTexture(this->GLTextureId);
	
	// Which type of filtering?
	switch(value)
	{
		case TextureFilterMode::Nearest:
			{
				// Setup Nearest (non) filtering
				if(this->hasMipMaps)
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);
				else
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			}
			break;
		
		case TextureFilterMode::Bilinear:
			{
				// Setup linear filtering
				if(this->hasMipMaps)
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
				else
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			}
			break;

		default:
			ASSERT(false, L"Unknown texture filter format...");
	}
}

// *****************************************************************
/// @brief
///     Initializes the texture with the given width, height, and pixel format
/// @param width
///     Width of the texture
/// @param height
///     Height of the texture 
/// @param pixelFormat
///     Pixel format of the texture
/// @remarks
///     The texture memory will be allocated in the devices video memory.
///     The filtering mode will default to Linear
///     The wrap mode will default to Repeat for PoT textures and Clamp for NPoT textures
///   @par
///     GDK Internal Use Only
// *****************************************************************
void Texture2D::Initialize(int width, int height, PixelFormat::Enum pixelFormat)
{
	// Create the GL texture
	GLuint glTextureId = -1;
	glGenTextures(1, &glTextureId);
	this->GLTextureId = glTextureId;
    
	// Store the texture properties
	this->Width = width;
	this->Height = height;
	this->TexelWidth = 1.0f / width;
	this->TexelHeight = 1.0f / height;
	this->Format = pixelFormat;
	
    // Bind the texture
    Graphics::BindTexture(this->GLTextureId);
    
	// Determine the GL texture format & pixel type 
	GLuint glTextureFormat = PixelFormat::GetGLTextureFormat(pixelFormat);
	GLuint glPixelType = PixelFormat::GetGLPixelType(pixelFormat);
    
	// Pre-allocate the texture buffers
	glTexImage2D(GL_TEXTURE_2D, 0, glTextureFormat, width, height, 0, glTextureFormat, glPixelType, NULL);
    
	// Setup default wrap & filter modes
    if(Math::IsPowerOfTwo((UInt32)this->Width) == true && Math::IsPowerOfTwo((UInt32)this->Height) == true)
        SetWrapMode(TextureWrapMode::Wrap);
	else
        SetWrapMode(TextureWrapMode::Clamp);
    SetFilterMode(TextureFilterMode::Bilinear);
}

// *****************************************************************
/// @brief
///     This method is called by the resource manager when the resource data is to be loaded from an asset
/// @remarks
///     GDK Internal Use Only
// *****************************************************************
void Texture2D::LoadFromAsset()
{
    // Get a stream to the asset
    char assetPath[256];
    GDK_SPRINTF(assetPath, 256, "%s.gdkimage", GetName().c_str());
    Stream* stream = AssetManager::GetAssetStream(assetPath);
    
	// Load the image header & image data from the file
	// ---------------------------------------------------------
    
	// Read the image header
	UInt16 width = stream->ReadUInt16();
	UInt16 height = stream->ReadUInt16();
	PixelFormat::Enum pixelFormat = (PixelFormat::Enum) stream->ReadUInt16();
	UInt16 flags = stream->ReadUInt16();
    
    // Process the image flags
    bool generateMipMaps = (flags & 0x0001) > 0;
    TextureWrapMode::Enum wrapMode = (TextureWrapMode::Enum) ((flags >> 1) & 0x3);
    TextureFilterMode::Enum filterMode = (TextureFilterMode::Enum) ((flags >> 3) & 0x3);
    
    // Initialize the texture
    Initialize(width, height, pixelFormat);
    
	// Create a buffer to hold the decompressed image data
	int numBytes = width * height * PixelFormat::GetBytesPerPixel(pixelFormat);
	UInt8* imageData = (UInt8*) GdkAlloc(numBytes);
    
	// Create a MemoryStream around our buffer
	MemoryStream memStream(imageData, numBytes);
    
	// Decompress the rest of the image data into the buffer
	bool result = stream->Decompress(&memStream, CompressionType::ZLib);
	ASSERT(result, 
           L"Failed to decompress the image data in a GdkImage"
           );
    
    // Apply the image data to the texture
    SetImageData(imageData);
    
    // Generate mipmaps if we're supposed to
    if(generateMipMaps)
        GenerateMipMaps();
    
    // Set wrap mode
    this->SetWrapMode(wrapMode);
    
    // Set filter mode
    this->SetFilterMode(filterMode);

    // Release the image data buffer
    GdkFree(imageData);
}

// *****************************************************************
/// @brief
///     This virtual is called by the resource manager to request the memory size of this asset
// *****************************************************************
UInt32 Texture2D::GetMemoryUsed()
{
    int numBytes = this->Width * this->Height * PixelFormat::GetBytesPerPixel(this->Format);
    return numBytes;
}

