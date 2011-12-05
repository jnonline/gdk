/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#pragma once



namespace Gdk
{
	/// @addtogroup Input
    /// @{
    
    // =================================================================================
    ///	@brief
    ///		Provides access to Motion, Accelerometer, and Gyroscope based input.
    ///	@remarks 
    ///		This static-only class contains methods and properties for accessing motion based input.
    ///     Including inputs from the device accelerometer, gyroscope, compass, etc.
    // =================================================================================
	class MotionInput
	{
	public: 

        // Public Methods
		// =====================================================
        
        // ---------------------------------
        /// @name Methods
        /// @{
        
        static bool IsShaking();
        static Vector3 GetAccelerometerAcceleration();
        
        /// @}
        
        // Public Events
		// =====================================================
        
        /// @name Events
        /// @{
        
		/// @brief
        ///     This event is raised when the device begins to shake.
        static MulticastDelegate0<void>  ShakingBegan;
        
        /// @brief
        ///     This event is raised when the device stops shaking.
        static MulticastDelegate0<void>  ShakingEnded;
        
        /// @}
        
    public:
        
        // INTERNAL ONLY - Platform Interfaces
		// =====================================================
		
        /// @cond INTERNAL
        
        // ---------------------------------
        /// @name Internal Platform Methods
        /// @{
        
		static void Platform_ShakingBegan();
		static void Platform_ShakingEnded();
        static void Platform_UpdateAccelerometer(Vector3 acceleration);
        
        /// @}
        
        /// @endcond
        
	private:

		// Internal Properties
		// ================================
        
        static bool isShaking;
        static Vector3 accelerometerAcceleration;
	};

    /// @} // Input

} // namespace Gdk
