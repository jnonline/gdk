

#pragma once

#include "GdkLIB.h"
#include "../../Math/2D/Vector2.h"
#include "../../Assets/Asset.h"
#include "../Texture2D.h"


namespace Gdk
{
	// ----------------
	class AtlasSheet;

	// ============================================================================
	// AtlasImage

	class AtlasImage
	{
	public:
		
		// Properties
		string Name;				// Name of the image
		int Width;					// Width of the image (in pixels)
		int Height;					// Height of the image (in pixels)
		int Index;					// Index of the image within the atlas

		int CroppedImageX;			// Position of the reduced image relative to the original
		int CroppedImageY;			
		int CroppedImageWidth;		// Size of the reduced image
		int CroppedImageHeight;	

		Vector2 TopLeftTexCoord;	// Texture coordinate location of the image
		Vector2 BottomRightTexCoord;
		
		Vector2 ReferencePoint;	// Reference point of the image (this is the location the image is oriented from)
		vector<Vector2> ChildPoints;

		AtlasSheet* Sheet;

		// Deconstructor
		~AtlasImage();

		// Methods
		Vector2 GetSize() {return Vector2((float)Width, (float)Height); }

	private:
		
		// Private constructor
		AtlasImage();

		friend class Atlas;
	};

	typedef StringHashMap<AtlasImage*> AtlasImageNameMap;

	// ============================================================================
	// AtlasAnimation

	class AtlasAnimation
	{
	public:

		// Properties
		string Name;
		float FPS;
		vector<AtlasImage*> Images;

		// Destructor
		~AtlasAnimation();

	private:

		// Private constructor
		AtlasAnimation();

		friend class Atlas;
	};

	typedef StringHashMap<AtlasAnimation*> AtlasAnimationNameMap;

	// ============================================================================
	// AtlasSheet

	class AtlasSheet
	{
	public:
		
		// Properties
		int Width;
		int Height;
		Texture2D* Texture;

		// Destructor
		~AtlasSheet();

	private:

		// Constructor
		AtlasSheet();

		friend class Atlas;
	};

	// ============================================================================
	// Atlas
	
	class Atlas
	{
	public:
		// Constructor
		~Atlas();

		// Properties
		string Name;

		// Child objects
		vector<AtlasImage*> Images;
		vector<AtlasSheet*> Sheets;
		vector<AtlasAnimation*> Animations;

		// Maps
		AtlasImageNameMap ImagesByName;
		AtlasAnimationNameMap AnimationsByName;

		// Accessor Methods
		AtlasImage* GetImage(const char* imageName);
		AtlasAnimation* GetAnimation(const char* animationName);

		// Creation Methods
		static Atlas* FromFile(const char *atlasFilePath);
		
		// Asset Interfaces
		static const AssetType::Enum ASSET_TYPE = AssetType::Atlas;
		static Atlas* FromAsset(AssetLoadContext* context);
		void IgnoreChildAssets() {}
	
	public:

		// Internal types
		class AtlasChildLoader
		{
		public:
			virtual Texture2D* LoadTexture(const char *textureName) = 0;
		};

	private:

		// private methods
		Atlas();
		static Atlas* FromStream(const char *atlasName, Stream* stream, class AtlasChildLoader* childLoader);
		
	};

	// ----------------------------------------------------------------------------

} // namespace

