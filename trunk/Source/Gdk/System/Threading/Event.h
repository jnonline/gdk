/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#pragma once



namespace Gdk
{
    /// @addtogroup System
    /// @{
    /// @addtogroup Threading
    /// @{
    
	// =================================================================================
    ///	@brief
    ///		Used to communicate event signals across threads
    // =================================================================================
	class Event
	{
	public: 

		// Public Methods
		// =====================================================
        
        // ---------------------------------
        /// @name Methods
        /// @{

		~Event();

		void Wait(bool reset = true);
        void Set();
        void Reset();
		static Event* Create();
        
        /// @}

	private:

		// Private Methods
		// =====================================================
        
		// CTor
		Event();

		// Properties
		bool signaled;
		pthread_mutex_t internalMutex;
		pthread_cond_t internalCV;

	};
    
    /// @}
    /// @}

} // namespace Gdk
