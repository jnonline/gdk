/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#pragma once


#include "../Delegates.h"

namespace Gdk
{
	/// @addtogroup System
    /// @{
    /// @addtogroup Threading
    /// @{
    
	// =================================================================================
    ///	@brief
    ///		Provides methods to start and control execution threads.
    /// @remarks
    ///     Every thread must be either Detach()'d or Join()'d in order to properly cleanup the thread
    // =================================================================================
    class Thread
	{
	public:

		// Public Types
		// =====================================================

		// The type for valid thread Worker() functions/methods
		typedef		Delegate1<void*, void*>		Delegate;

		// Public Methods
		// =====================================================
        
        // ---------------------------------
        /// @name Instance Methods
        /// @{
        
		Thread(const Thread& input);
		Thread& operator= (const Thread& input);
            
        void* Join();
        void Detach();
        UInt32 GetId();
		operator UInt32 () { return GetId(); }
        
        /// @}
        // ---------------------------------
        /// @name Static Methods
        /// @{
        
		static Thread Create(Thread::Delegate* threadDelegate, void* argument = 0, bool createDetached = false);
        static Thread Create(void* threadFunction(void*), void* argument = 0, bool createDetached = false);
        static UInt32 GetCurrentThreadId();
        static void Exit(void* returnValue);
        static void Sleep(UInt32 milliseconds);
        
        /// @}

	private:

		// Private Methods
		// =====================================================
        
        Thread();


		// Properties
		pthread_t internalThreadId;
	};
    
    /// @}
    /// @}

} // namespace Gdk
