/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

// Includes
#include "BasePCH.h"
#include "StarfieldSample.h"

#define NUM_STARS			100
#define RANDOM_INTENSITY	(UInt8)Random::FromInterval(64, 196)

// ***********************************************************************
StarfieldSample::StarfieldSample()
{
	// Allocate the starfield
	stars = (Star*) GdkAlloc(NUM_STARS * sizeof(Star));

	// Assign a random position & intensity to each star
	for(size_t i=0; i<NUM_STARS; i++)
	{
		Star &star = stars[i];
		star.Position.X = (float) Gdk::Random::FromInterval(0, Application::GetWidth());
		star.Position.Y = (float) Gdk::Random::FromInterval(0, Application::GetHeight());
		star.Intensity = RANDOM_INTENSITY;
	}
}


// ***********************************************************************
StarfieldSample::~StarfieldSample()
{
	// Release the star field
	GdkFree(stars);
}

// ***********************************************************************
void StarfieldSample::OnUpdate(float elapsedSeconds)
{
	Vector2 velocity(Application::GetWidth() * -0.3f, 0);
	Vector2 screenBounds = Graphics::GetScreenBounds();

	// Loop through the stars
	for(size_t i=0; i<NUM_STARS; i++)
	{
		Star &star = stars[i];

		// Update this star's position
		star.Position += (star.Intensity / 255.0f) * elapsedSeconds * velocity;

		// If the star goes off the screen, move it to the other side & reset it's intensity
		//   (to simulate a new star coming into view)
		if(star.Position.X > screenBounds.X)
		{
			star.Position.X -= screenBounds.X;
			star.Intensity = RANDOM_INTENSITY;
		}
		else if(star.Position.X < 0)
		{
			star.Position.X += screenBounds.X;
			star.Intensity = RANDOM_INTENSITY;
		}

		if(star.Position.Y > screenBounds.Y)
		{
			star.Position.Y -= screenBounds.Y;
			star.Intensity = RANDOM_INTENSITY;
		}
		else if(star.Position.Y < 0)
		{
			star.Position.Y += screenBounds.Y;
			star.Intensity = RANDOM_INTENSITY;
		}
	}
}

// ***********************************************************************
void StarfieldSample::OnDraw()
{
	// Clear the background
	Graphics::Clear(
		true, false, false,
        Color::FromNormalized(0.1f, 0.1f, 0.1f, 1.0f),
		1.0f
		);
	
	// Setup a 2D projection matrix to draw in screen coordinates
    Matrix3D proj = Matrix3D::CreateOrthoOffCenter(
		0, (float) Application::GetWidth(),					// left / right
		(float) Application::GetHeight(), 0,				// bottom / top
		-1.0f, 1.0f											// far / near
		);
	Graphics::GlobalUniforms.Projection->SetMatrix4(proj);

	// For demonstration purposes, we will draw the less intense stars as points and the 
	// brigher stars as images (Textured Quads).   We seperate the drawing into two 
	// drawing passes to allow the render batcher to batch all points and all images.

	// Loop through the low intensity stars
	for(size_t i=0; i<NUM_STARS; i++)
	{
		if(stars[i].Intensity < 160)
		{
			// Draw this star as a point
			Drawing2D::DrawPoint(
				stars[i].Position, 
				Color(stars[i].Intensity, stars[i].Intensity, stars[i].Intensity, 255)
				);
		}
	}

	// Loop through the high intensity stars
	for(size_t i=0; i<NUM_STARS; i++)
	{
		if(stars[i].Intensity >= 160)
		{
			// Draw the star as an image
			Drawing2D::DrawAtlasImage(
				SharedResources::AtlasImages.Particle2, 
				stars[i].Position, 
				Color(stars[i].Intensity, stars[i].Intensity, stars[i].Intensity, 255),
				0.0f,			// rotation
				0.1f, 0.1f,		// scale
				BlendMode::Normal
				);
		}
	}

	// Flush 2d drawing
	Drawing2D::Flush();
}
