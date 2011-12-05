/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#pragma once


namespace Gdk
{
	// ============================================================================
	namespace AnimationDirection 
	{
		enum Enum 
		{
			Forward = 1,
			Backward = 2
		};
	}

	// ============================================================================
	namespace AnimationPlayback 
	{
		enum Enum 
		{
			Looping = 1,
			PingPong = 2,
			InactiveWhenDone = 3
		};
	}

	// ============================================================================
	class Sprite
	{
	public:

		// Public Properties
		// -----------------------

		// Position
		Vector2				Position;
		float				Z;

		// Transform
		float				Rotation;
		Vector2				Scale;

		// Visual 
		bool				Visible;
		Gdk::Color			Color;
		BlendMode::Enum		BlendMode;

		// Animation
		bool						Active;
		float						FPS;
		AnimationDirection::Enum	Direction;
		AnimationPlayback::Enum		Playback;
		
		// Extra User Data
		int					Id;
		void*				UserData;
	
		// Public Methods
		// -----------------------

		// Constructor / Destructor
		Sprite(AtlasImage *image);
		Sprite(AtlasImage **imagesArray, size_t numImages);
		Sprite(vector<AtlasImage*> images);
		Sprite(AtlasAnimation *image);
		~Sprite();

		// Change animation set
		void SetImage(AtlasImage *image);
		void SetAnimation(AtlasImage **imagesArray, size_t numImages);
		void SetAnimation(vector<AtlasImage*> images);
		void SetAnimation(AtlasAnimation *animation);

		// Animation control
		const vector<AtlasImage*>& GetAnimationSet();
		size_t GetNumFrames();
		short GetCurrentFrame();
		void SetCurrentFrame(short index);
		AtlasImage* GetCurrentFrameImage();

		// Accessors for mount points (of the active image)
		size_t GetNumChildMountPoints();
		Vector2 GetChildMountPoint(size_t index, bool applySpriteTransforms = true);

		// Updating  (This is required for the animation to play)
		void Update(float elapsedSeconds);

		// Public Types
		// -----------------------

		typedef Gdk::MulticastDelegate1<void, Sprite*>	SpriteDelegate;

		// Public Events
		// -----------------------

		SpriteDelegate AnimationComplete;
		SpriteDelegate FrameChanged;

	private:

		// Internal properties
		short currentFrameIndex;
		float secondsSinceLastFrame;
		vector<AtlasImage*> animationSet;

		// Internal methods
		void InternalInit();
	};

} // namespace