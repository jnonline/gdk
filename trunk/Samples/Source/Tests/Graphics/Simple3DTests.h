/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

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
    
	// Camera/View properties
    float viewLongitudeAngle;
    float viewLatitudeAngle;
    float viewDistance;
    
    // calculated camera values
    Vector3 cameraPosition;
    Vector3 cameraLookAt;
    Vector3 cameraDirection;
    Matrix3D projection;
    Matrix3D view;

	// Assets
	ResourcePool resourcePool;
	Model* skeletalModel;
    
	// Input event handlers
    void OnTouchMoved(Touch* touch);
    void OnMouseMove(MouseMoveArgs* args);
	void OnMouseWheelScroll(float deltaX, float deltaY);
};
