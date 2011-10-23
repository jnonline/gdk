/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#pragma once



namespace Gdk
{
	// ============================================================================
	// Critical Sections
	//	Usage:
	//
	//		GDK_BEGIN_CRITICAL_SECTION( mySection );
	//      ... do work
	//		GDK_END_CRITICAL_SECTION( mySection );
	//

    /// @addtogroup System
    /// @{
    /// @addtogroup Threading
    /// @{

    // *****************************************************************
    /// @brief
    ///     Starts a critical section
    /// @param SECTION_NAME
    ///     A unique identifier for the section name
    // *****************************************************************
	#define GDK_BEGIN_CRITICAL_SECTION( SECTION_NAME ) \
		static pthread_mutex_t SECTION_NAME = PTHREAD_MUTEX_INITIALIZER; \
		pthread_mutex_lock( &SECTION_NAME );

    // *****************************************************************
    /// @brief
    ///     Ends a critical section
    /// @param SECTION_NAME
    ///     A unique identifier for the section name
    // *****************************************************************
	#define GDK_END_CRITICAL_SECTION( SECTION_NAME ) \
		pthread_mutex_unlock( &SECTION_NAME ) \
        ;

    /// @}
    /// @}
    
} // namespace Gdk
