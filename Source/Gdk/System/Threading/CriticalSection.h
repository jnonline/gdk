/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#pragma once

#include "GdkLIB.h"

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


	// Starts a critical section
	#define GDK_BEGIN_CRITICAL_SECTION( SECTION_NAME ) \
		static pthread_mutex_t SECTION_NAME = PTHREAD_MUTEX_INITIALIZER; \
		pthread_mutex_lock( &SECTION_NAME );

	// Ends a critical section
	#define GDK_END_CRITICAL_SECTION( SECTION_NAME ) \
		pthread_mutex_unlock( &SECTION_NAME );

} // namespace Gdk
