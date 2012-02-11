/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#include "BasePCH.h"
#include "Sprite.h"

using namespace Gdk;

// ***********************************************************************
void Sprite::InternalInit()
{
	// Initialize all the properties
	Position = Vector2::ZERO;
	Z = 0.0f;

	Rotation = 0.0f;
	Scale = Vector2::ONE;

	Visible = true;
	Color = Color::WHITE;
	BlendMode = BlendMode::Normal;
	
	Active = true;
	FPS = 10.0f;
	Direction = AnimationDirection::Forward;
	Playback = AnimationPlayback::Looping;

	Id = 0;
	UserData = NULL;
	
	// Setup the frame counters & timers
	currentFrameIndex = 0;
	secondsSinceLastFrame = 0.0f;
}

// ***********************************************************************
Sprite::Sprite(AtlasImage *image)
{
	InternalInit();
	SetImage(image);
}

// ***********************************************************************
Sprite::Sprite(AtlasImage **imagesArray, size_t numImages)
{
	InternalInit();
	SetAnimation(imagesArray, numImages);
}

// ***********************************************************************
Sprite::Sprite(vector<AtlasImage*> images)
{
	InternalInit();
	SetAnimation(images);
}

// ***********************************************************************
Sprite::Sprite(AtlasAnimation *animation)
{
	InternalInit();
	SetAnimation(animation);
}

// ***********************************************************************
Sprite::~Sprite()
{
}

// ***********************************************************************
void Sprite::SetImage(AtlasImage *image)
{
	// Add the image to the animation set
	animationSet.clear();
	animationSet.push_back(image);

	// Reset the animation
	SetCurrentFrame(0);
}

// ***********************************************************************
void Sprite::SetAnimation(AtlasImage **imagesArray, size_t numImages)
{
	// Clear the current animation set
	animationSet.clear();

	// Ensure the animation set is large enough to hold the animation
	if(animationSet.capacity() < numImages)
		animationSet.reserve(numImages);

	// Add the images to the animation set
	for(size_t i=0; i<numImages; i++)
		animationSet.push_back(imagesArray[i]);

	// Reset the animation
	SetCurrentFrame(0);
}

// ***********************************************************************
void Sprite::SetAnimation(vector<AtlasImage*> images)
{
	// Clear the current animation set
	animationSet.clear();

	// Ensure the animation set is large enough to hold the animation
	size_t numImages = images.size();
	if(animationSet.capacity() < numImages)
		animationSet.reserve(numImages);

	// Add the images to the animation set
	for(size_t i=0; i<numImages; i++)
		animationSet.push_back(images[i]);

	// Reset the animation
	SetCurrentFrame(0);
}

// ***********************************************************************
void Sprite::SetAnimation(AtlasAnimation *animation)
{
	// Clear the current animation set
	animationSet.clear();

	// Ensure the animation set is large enough to hold the animation
	size_t numImages = animation->Images.size();
	if(animationSet.capacity() < numImages)
		animationSet.reserve(numImages);

	// Add the images to the animation set
	for(size_t i=0; i<numImages; i++)
		animationSet.push_back(animation->Images[i]);

	// Set the FPS from the atlas animation
	this->FPS = animation->FPS;

	// Reset the animation
	SetCurrentFrame(0);
}
		void SetCurrentFrame(size_t index);

// ***********************************************************************
const vector<AtlasImage*>& Sprite::GetAnimationSet()
{
	return this->animationSet;
}

// ***********************************************************************
size_t Sprite::GetNumFrames()
{
	return this->animationSet.size();
}

// ***********************************************************************
short Sprite::GetCurrentFrame()
{
	return this->currentFrameIndex;
}

// ***********************************************************************
void Sprite::SetCurrentFrame(short index)
{
	ASSERT(index >= 0 && index < (short)animationSet.size(), "Index out of range of animation set");
	this->currentFrameIndex = index;
	this->secondsSinceLastFrame = 0.0f;
}

// ***********************************************************************
AtlasImage* Sprite::GetCurrentFrameImage()
{
	return this->animationSet[this->currentFrameIndex];
}

// ***********************************************************************
size_t Sprite::GetNumChildMountPoints()
{
	return animationSet[currentFrameIndex]->ChildPoints.size();
}

// ***********************************************************************
Vector2 Sprite::GetChildMountPoint(size_t index, bool applySpriteTransforms)
{
	// Get the mount point from the image
	Vector2 mountPoint = animationSet[currentFrameIndex]->ChildPoints[index];

	// Apply the sprite transforms?
	if(applySpriteTransforms)
	{
		Matrix2D transform(false);
		Matrix2D::CreateScaleRotateTranslate(this->Scale.X, this->Scale.Y, this->Position.X, this->Position.Y, this->Rotation);
		return transform.TransformPoint(mountPoint);
	}

	return mountPoint;
}

// ***********************************************************************
void Sprite::Update(float elapsedSeconds)
{
	// Step 1:  Update the animation
	// -----------------------------

	// Is the animation active?
	if(this->Active == true)
	{
		// Calculate the number of seconds to get to the next frame
		float secondsPerFrame = 1.0f / this->FPS;

		// Update the frame ticks counter
		this->secondsSinceLastFrame += elapsedSeconds;

		// Is it time for the next frame?
		while (this->secondsSinceLastFrame >= secondsPerFrame)
		{
			// Reset the frame ticks counter
			this->secondsSinceLastFrame -= secondsPerFrame;

			// Update the current frame based on the animation direction
			if (this->Direction == AnimationDirection::Forward)
				this->currentFrameIndex++;
			else
				this->currentFrameIndex--;

			// Fire the FrameChanged event
			this->FrameChanged.Invoke(this);

			// Did the animation play past its boundary?
			int totalFrames = this->animationSet.size();
			if (this->currentFrameIndex < 0 || this->currentFrameIndex >= totalFrames)
			{
				// What is the playback mode
				switch(this->Playback)
				{
					case AnimationPlayback::Looping:
						// Just move the frame index to it's next position
						if (this->currentFrameIndex < 0)
							this->currentFrameIndex = totalFrames - 1;
						else
							this->currentFrameIndex = 0;
						break;

					case AnimationPlayback::PingPong:
						// Move the current frame to it's proper position
						if (this->currentFrameIndex < 0)
						{
							// Is there more then 1 frame in this animation?
							if (totalFrames > 1)
								this->currentFrameIndex = 1;
							else
								this->currentFrameIndex = 0;
						}
						else
						{
							// Is there more then 1 frame in this image?
							if (totalFrames > 1)
								this->currentFrameIndex = totalFrames - 2;
							else
								this->currentFrameIndex = 0;
						}

						// Turn the animation direction around
						if (this->Direction == AnimationDirection::Forward)
							this->Direction = AnimationDirection::Backward;
						else
							this->Direction = AnimationDirection::Forward;

						break;

					case AnimationPlayback::InactiveWhenDone:
						// Just move the frame pointer to it's next position
						if (this->currentFrameIndex < 0)
							this->currentFrameIndex = 0;
						else
							this->currentFrameIndex = totalFrames - 1;

						// De-Activate the sprite
						this->Active = false;
						break;
				}

				// Do animation complete delegate dispatching
				this->AnimationComplete.Invoke(this);
			}
		}
	}
}
