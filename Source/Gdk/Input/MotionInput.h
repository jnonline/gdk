/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#pragma once

#include "GdkLIB.h"

namespace Gdk
{
	// ============================================================================

	class MotionInput
	{
	public: 

        // Methods
        // --------------------------------------------

        // Checks if the motion input is currently 'shaking' (IE: the user is literally shaking the device)
        static bool IsShaking()  { return isShaking; }
        
        // Gets the current accelerometer acceleration data.
        //  +X = Right, +Y = Up, +Z = Towards the user
        static Vector3 GetAccelerometerAcceleration()  { return accelerometerAcceleration; }
        
        // Events
        // --------------------------------------------
        
        // Events for device motion
        static MulticastDelegate0<void>  ShakingBegan;
        static MulticastDelegate0<void>  ShakingEnded;
        
    public:
        
        
        // Public Platform Interfaces
		// ----------------------------------
        
		// Input processing
		static void Platform_ShakingBegan();
		static void Platform_ShakingEnded();
        static void Platform_UpdateAccelerometer(Vector3 acceleration);
        
	private:

		// Internals
		// --------------------------------------------
        
        static bool isShaking;
        static Vector3 accelerometerAcceleration;
		
		// Hidden CTor
		MotionInput() {}
	};


} // namespace Gdk
