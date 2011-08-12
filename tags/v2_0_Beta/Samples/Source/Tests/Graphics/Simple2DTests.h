/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#pragma once

// Includes
#include "../../Module.h"

// *******************************************************************
class Simple2DTests : public Module
{
public:
	// CTor / DTor
    Simple2DTests();
	~Simple2DTests();

	// Module overrides
	virtual void OnUpdate(float elapsedSeconds);
	virtual void OnDraw();

protected:
	// View
	Vector2 ViewCenter;
	Vector2 ViewExtents;
    
	// Animation timers
    float totalElapsedSeconds;
	float rectTimer;
	float circleTimer;
	float diamondTimer;
	float textScaleTimer;
	float textSpinTimer;

	Sprite* sprite1;
	Sprite* sprite2;

	class Simple2DTestsParticleSystem* particleSystem1;
	class Simple2DTestsParticleSystem* particleSystem2;
};


// *******************************************************************
struct Simple2DTestsParticle : Particle2D
{
	Vector2			Velocity;
	float			SpinVelocity;
};

// *******************************************************************
class Simple2DTestsParticleSystem : public ParticleSystem2D
{
public:
	Simple2DTestsParticleSystem();

	// Virtual methods for derived classes
	virtual size_t GetParticleStride();
	virtual void OnNewParticle(Particle2D *particle);
	virtual void OnUpdateParticles(float elapsedSeconds, Particle2D *pool);
};