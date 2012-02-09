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
    ///		Contains information for a single image within an Atlas
    // =================================================================================
    class AtlasImage
	{
	public:
		
        // Publics
		// =====================================================
        
        // ---------------------------------
        /// @name Read-only Properties
        /// @{
        
        /// Name of the image
		string Name;
		
		/// Width of the image (in pixels)
        int Width;
        
        /// Height of the image (in pixels)
        int Height;
        
        /// Index of the image within the atlas
		int Index;
        
        /// X-Position of the cropped image relative to the original
		int CroppedImageX;
        
        /// Y-Position of the cropped image relative to the original
		int CroppedImageY;
        
        /// Width of the cropped image
		int CroppedImageWidth;
        
        /// Height of the cropped image
		int CroppedImageHeight;

        /// Top-left Texture coordinate of the image within the atlas sheet
		Vector2 TopLeftTexCoord;
        
        /// Bottom-right Texture coordinate of the image within the atlas sheet
        Vector2 BottomRightTexCoord;
		
        /// Reference point of the image (this is the location the image is oriented from when drawn as a 2d position)
		Vector2 ReferencePoint;
        
        /// Child points of the image, as assigned by the atlas builder tool
		vector<Vector2> ChildPoints;

        /// The sheet that contains the actual image
		class AtlasSheet* Sheet;

        /// @}
        // ---------------------------------
        /// @name Methods
        /// @{
        
        AtlasImage();
        ~AtlasImage();

        // *****************************************************************
        /// @brief
        ///     Gets a Vector2 containing the image width & height in pixels.
        // *****************************************************************
		Vector2 GetSize() {return Vector2((float)Width, (float)Height); }
        
        void GetQuad(VertexP2T2C4* vertices, const Color& color = Color::WHITE);
        void GetFittedQuad(VertexP2T2C4* vertices, const Rectangle2& fitToRect, const Color& color = Color::WHITE);
        
        /// @}
	};

    // =================================================================================
    ///	@brief
    ///		A map of AtlasImage pointers by the image name
    // =================================================================================
	typedef StringHashMap<AtlasImage*> AtlasImageNameMap;

	// =================================================================================
    ///	@brief
    ///		Contains information for an animation within an Atlas
    // =================================================================================
    class AtlasAnimation
	{
	public:

        // Publics
		// =====================================================
        
        // ---------------------------------
        /// @name Read-only Properties
        /// @{
        
		/// Name of the animation
		string Name;
        
        /// Frames-per-second that the animation should be played at
		float FPS;
        
        /// Set of images that make up the animation
		vector<AtlasImage*> Images;
        
        /// @}
        // ---------------------------------
        /// @name Methods
        /// @{

        AtlasAnimation();
        ~AtlasAnimation();
        
        /// @}
        
	};

    // =================================================================================
    ///	@brief
    ///		A map of AtlasAnimation pointers by the animation name
    // =================================================================================
	typedef StringHashMap<AtlasAnimation*> AtlasAnimationNameMap;

	// =================================================================================
    ///	@brief
    ///		Contains a sheet of atlas images on a texture.
    // =================================================================================
    class AtlasSheet
	{
	public:
        
        // Publics
		// =====================================================
        
        // ---------------------------------
        /// @name Read-only Properties
        /// @{
    
		
		/// Width of the sheet (in pixels)
		int Width;
        
        /// Height of the sheet (in pixels)
		int Height;
        
        /// Texture that contains the sheet data
		Texture2D* Texture;
        
        /// @}
        // ---------------------------------
        /// @name Methods
        /// @{
        
        AtlasSheet();
        ~AtlasSheet();
        
        /// @}

	};

	// =================================================================================
    ///	@brief
    ///		Contains a set of images and animations compressed into a few atlas textures
    // =================================================================================
	class Atlas : public Resource
	{
	public:
        
        // Publics
		// =====================================================
        
        // ---------------------------------
        /// @name Read-only Properties
        /// @{

        /// All the AtlasImage's in this Atlas
		vector<AtlasImage*> Images;
        
        /// All the AtlasSheet's in this Atlas
		vector<AtlasSheet*> Sheets;
        
        /// All the AtlasAnimation's in this Atlas
		vector<AtlasAnimation*> Animations;
        
		/// A map of AtlasImages by name in this atlas
		AtlasImageNameMap ImagesByName;
        
        /// A map of AtlasAnimations by name in this atlas
		AtlasAnimationNameMap AnimationsByName;

        /// @}
        // ---------------------------------
        /// @name Methods
        /// @{
        
        ~Atlas();
        
		AtlasImage* GetImage(const char* imageName);
		AtlasAnimation* GetAnimation(const char* animationName);
        
        /// @}
        
    protected:
        
        // Protected Methods
        // =====================================================
        
        // ---------------------------------
        /// @name Virtuals from Resouce
        /// @{
        
        /// @} 

    private:
        
        // Private Methods
        // =====================================================
        
        friend class AtlasManager;
        
		Atlas();
        
        void LoadFromAsset();
	};
    
    /// @}
    
} // namespace

