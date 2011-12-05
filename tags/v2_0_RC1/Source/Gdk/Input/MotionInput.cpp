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

// *****************************************************************
/// @brief
///     Returns true if the device is currently being physically shaken
/// @remarks
///     The amount of force of the shake is device dependent, as the state of being
///     physically shaken is determined differently for each type of device.  
// *****************************************************************
bool MotionInput::IsShaking()
{ 
    return isShaking; 
}

// *****************************************************************
/// @brief
///     Gets the current 3-axis accelerometer value of the device.
/// @remarks
///     The accelerometer value is measured in G-forces, with 1-G being standard gravity.
/// @return
///     The 3-axis vector accelerometer values as defined:
///     @li +X Axis - Right side of the device
///     @li +Y Axis - Top of the device
///     @li +Z Axis - Front of the device
// *****************************************************************
Vector3 MotionInput::GetAccelerometerAcceleration()  
{ 
    return accelerometerAcceleration; 
}

// *****************************************************************
/// @brief
///     Tells the GDK that the device has started shaking
/// @note
///     GDK Internal Use Only
// *****************************************************************
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

// *****************************************************************
/// @brief
///     Tells the GDK that the device has stopped shaking
/// @note
///     GDK Internal Use Only
// *****************************************************************
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

// *****************************************************************
/// @brief
///     Tells the GDK about an accelerometer data update
/// @note
///     GDK Internal Use Only
// *****************************************************************
void MotionInput::Platform_UpdateAccelerometer(Vector3 acceleration)
{
	accelerometerAcceleration = acceleration;
}

