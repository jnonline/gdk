
#pragma once

// Includes
#include "../../Module.h"

// *******************************************************************
class Simple3DTests : public Module
{
public:
	// CTor / DTor
    Simple3DTests();
	~Simple3DTests();

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

};
