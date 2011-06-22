/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#pragma once

// Includes
#include "../../Module.h"

// *******************************************************************
class Test2DModule : public Module
{
public:
	// CTor / DTor
    Test2DModule();
	~Test2DModule();

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

	class Test2DParticleSystem* particleSystem1;
	class Test2DParticleSystem* particleSystem2;
};


// *******************************************************************
struct Test2DParticle : Particle2D
{
	Vector2			Velocity;
	float			SpinVelocity;
};

// *******************************************************************
class Test2DParticleSystem : public ParticleSystem2D
{
public:
	Test2DParticleSystem();

	// Virtual methods for derived classes
	virtual size_t GetParticleStride();
	virtual void OnNewParticle(Particle2D *particle);
	virtual void OnUpdateParticles(float elapsedSeconds, Particle2D *pool);
};