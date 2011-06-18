
#include "BasePCH.h"
#include "Texture2D.h"

using namespace Gdk;

// Static instantiations
int Texture2D::totalMemoryUsed = 0;

// GDK Image Flags
#define GDKIMAGE_FLAG_MIPMAP		0x01

// -----------------------------------------
Texture2D::Texture2D(int width, int height, PixelFormat::Enum pixelFormat, void* pixelData, bool generateMipMap)
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
	this->hasMipMaps = generateMipMap;
	
	// Bind the texture into GL
	glBindTexture(GL_TEXTURE_2D, this->GLTextureId);

	// Determine the GL texture format & type 
	GLuint glImageFormat = GL_RGBA;
	GLuint glImageType = GL_UNSIGNED_BYTE;

	switch(pixelFormat)
	{
		case PixelFormat::RGB_565:			glImageFormat = GL_RGB;				glImageType = GL_UNSIGNED_SHORT_5_6_5;		break;
		case PixelFormat::RGB_888:			glImageFormat = GL_RGB;				glImageType = GL_UNSIGNED_BYTE;				break;
		case PixelFormat::RGBA_5551:		glImageFormat = GL_RGBA;			glImageType = GL_UNSIGNED_SHORT_5_5_5_1;	break;
		case PixelFormat::RGBA_4444:		glImageFormat = GL_RGBA;			glImageType = GL_UNSIGNED_SHORT_4_4_4_4;	break;
		case PixelFormat::RGBA_8888:		glImageFormat = GL_RGBA;			glImageType = GL_UNSIGNED_BYTE;				break;
		case PixelFormat::LUMINANCE_ALPHA:	glImageFormat = GL_LUMINANCE_ALPHA;	glImageType = GL_UNSIGNED_BYTE;				break;
		case PixelFormat::LUMINANCE:		glImageFormat = GL_LUMINANCE;		glImageType = GL_UNSIGNED_BYTE;				break;
		case PixelFormat::ALPHA:			glImageFormat = GL_ALPHA;			glImageType = GL_UNSIGNED_BYTE;				break;
	}

	// Load the image data into the GL texture
	glTexImage2D(GL_TEXTURE_2D, 0, glImageFormat, width, height, 0, glImageFormat, glImageType, (void*)pixelData);

	// Does this texture need mip maps?
	if(generateMipMap)
	{
		// Generate mip maps
		glGenerateMipmap(GL_TEXTURE_2D);
	}

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

// ***********************************************************************
Texture2D::~Texture2D()
{
	// Destroy the texture
	glDeleteTextures(1, &(this->GLTextureId));

	// Decrement the used texture memory counter
	Texture2D::totalMemoryUsed -= this->memoryUsed;
}

// ***********************************************************************
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

// ***********************************************************************
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

// ***********************************************************************
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
	Texture2D *texture = GdkNew Texture2D(width, height, pixelFormat, imageData, (flags & GDKIMAGE_FLAG_MIPMAP) > 0);
	
	return texture;
}

// -----------------------------------------
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

// -----------------------------------------
Texture2D* Texture2D::FromAsset(AssetLoadContext* context)
{
	// Load the texture from the stream
	return FromStream(context->AssetStream);
}