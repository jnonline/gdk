/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#pragma once

// Includes
#include "../Module.h"

// -------------------------
typedef struct _Star
{
	Vector2 Position;
	UInt8 Intensity;
} Star;

// ============================================================================
// This Starfield sample draws a 2d point starfield

// *******************************************************************
class StarfieldSample : public Module
{
public:
	// Methods
	StarfieldSample();
	~StarfieldSample();

	// Module overrides
	virtual void OnUpdate(float elapsedSeconds);
	virtual void OnDraw();

private:
	// Private Properties
	int numStars;
	Star *stars;
};