/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#pragma once




namespace Gdk
{
	/// @addtogroup System
    /// @{
    /// @addtogroup Time
    /// @{
    
    // =================================================================================
    ///	@brief
    ///		Provides static-methods to access High Resolution Timers
    // =================================================================================
	class HighResTimer
	{
	public:
        
        // Public Methods
		// =====================================================

        // *****************************************************************
        /// @brief
        ///     Gets a high-resolution time stamp, in seconds.
        // *****************************************************************
		static double GetSeconds();
	};
    
    /// @}
    /// @}

} // namespace Gdk
