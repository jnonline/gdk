/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#include "BasePCH.h"
#include "Texture2D.h"

using namespace Gdk;

// Static instantiations
int Texture2D::totalMemoryUsed = 0;

// GDK Image Flags
#define GDKIMAGE_FLAG_MIPMAP		0x01

// *****************************************************************
/// @brief
///     Constructs a new texture of the given size and pixel format
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
// *****************************************************************
Texture2D::Texture2D(int width, int height, PixelFormat::Enum pixelFormat)
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
        SetWrapMode(TextureWrapMode::Repeat);
	else
        SetWrapMode(TextureWrapMode::Clamp);
    SetFilterMode(TextureFilterMode::Linear);
	
	// Track the memory used by this texture
	int numBytes = width * height * PixelFormat::GetBytesPerPixel(pixelFormat);
	this->memoryUsed = numBytes;
	Texture2D::totalMemoryUsed += this->memoryUsed;
}

// *****************************************************************
/// @brief
///     Destructor
// *****************************************************************
Texture2D::~Texture2D()
{
	// Destroy the texture
	glDeleteTextures(1, &(this->GLTextureId));

	// Decrement the used texture memory counter
	Texture2D::totalMemoryUsed -= this->memoryUsed;
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
void Texture2D::SetWrapMode(TextureWrapMode::Enum value)
{
	// Bind this texture
	Graphics::BindTexture(this->GLTextureId);
    
    // Check if the user is trying to set an invalid wrap mode
	if(value != TextureWrapMode::Clamp &&
       (Math::IsPowerOfTwo((UInt32)this->Width) == false || Math::IsPowerOfTwo((UInt32)this->Height) == false) )
    {
        LOG_WARN(L"Attempt to set invalid wrapping mode on a Non-PoT texture.  Reverting to Clamped");
        value = TextureWrapMode::Clamp;
    }
	
	// Set the wrap mode for this texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (GLenum)value);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (GLenum)value);

	this->wrapMode = value;
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
		
		case TextureFilterMode::Linear:
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
///     Reads a GDK Image from a stream and creates a texture from the image data
/// @param stream
///     A stream containing a GDK image the current stream position
// *****************************************************************
Texture2D* Texture2D::FromStream(Stream *stream)
{
	// Step 1: Load the image header & image data from the file
	// ---------------------------------------------------------

	// Read the image header
	UInt16 width = stream->ReadUInt16();
	UInt16 height = stream->ReadUInt16();
	PixelFormat::Enum pixelFormat = (PixelFormat::Enum) stream->ReadUInt16();
	UInt16 flags = stream->ReadUInt16();

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

	// Create the texture 
	Texture2D *texture = GdkNew Texture2D(width, height, pixelFormat);
    
    // Apply the image data to the texture
    texture->SetImageData(imageData);
    
    // Generate mipmaps if we're supposed to
    if((flags & GDKIMAGE_FLAG_MIPMAP) > 0)
        texture->GenerateMipMaps();
    
    // Release the image data buffer
    GdkFree(imageData);
	
	return texture;
}

// *****************************************************************
/// @brief
///     Creates a texture from a GDK Image file
/// @param imageFilePath
///     Path to the GDK Image file
// *****************************************************************
Texture2D* Texture2D::FromFile(const char *imageFilePath)
{
	// Open the file as a stream
	FileStream fileStream(imageFilePath, FileMode::Read);

	// Load the data from the stream
	Texture2D* texture = FromStream(&fileStream);

	// Close the stream
	fileStream.Close();

	return texture;
}

// *****************************************************************
/// @brief
///     Creates a texture from a given asset
/// @param context
///     A buffer of pixel data with the same size & pixel format as the texture.
// *****************************************************************
Texture2D* Texture2D::FromAsset(AssetLoadContext* context)
{
	// Load the texture from the stream
	return FromStream(context->AssetStream);
}