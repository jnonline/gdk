/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#include "BasePCH.h"
#include "MotionInput.h"

using namespace Gdk;

// Static Instantiations
bool MotionInput::isShaking = false;
Vector3 MotionInput::accelerometerAcceleration(0.0f, 0.0f, 0.0f);

// Events
MulticastDelegate0<void> MotionInput::ShakingBegan;
MulticastDelegate0<void> MotionInput::ShakingEnded;

// ===================================================================================
void MotionInput::Platform_ShakingBegan()
{
    // Was the device not already shaking?
    if(isShaking == false)
    {
        // Fire the ShakingBegan event
        ShakingBegan.Invoke();
        isShaking = true;
    }
}

// ===================================================================================
void MotionInput::Platform_ShakingEnded()
{
	// Was the device not already shaking?
    if(isShaking == true)
    {
        // Fire the ShakingBegan event
        ShakingEnded.Invoke();
        isShaking = false;
    }
}

// ===================================================================================
void MotionInput::Platform_UpdateAccelerometer(Vector3 acceleration)
{
	accelerometerAcceleration = acceleration;
}

