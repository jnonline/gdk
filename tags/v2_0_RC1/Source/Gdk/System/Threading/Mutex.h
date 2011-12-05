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
    ///		A synchronization mechanism for guarding non-thread safe resources
    // =================================================================================
    class Mutex
	{
	public: 

		// Instance Methods
		// --------------------------------------------

        // Public Methods
		// =====================================================
        
        // ---------------------------------
        /// @name Methods
        /// @{
        
		~Mutex();
        void Lock();
        bool TryLock();
        void Unlock();
		static Mutex* Create();

        /// @}
        
	private:
        
		// Private Methods
		// =====================================================
		
        Mutex();

		// Properties
		pthread_mutex_t internalMutex;
	};

	// TODO(P2):  Timeout based locking : pthread_mutex_timedlock()
    
    /// @}
    /// @}

} // namespace Gdk
