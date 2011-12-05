/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#pragma once

#include "Segment3.h"
#include "Sphere3.h"
#include "Box3.h"

namespace Gdk
{
    /// @addtogroup Math
    /// @{
    /// @addtogroup _3D
    /// @{
    
    // =================================================================================
    /// @brief
    ///     Represents a 3D capsule.
    // =================================================================================
	class Capsule3
	{
	public:
        
        // Public Properties
		// =====================================================
        
		/// Line segment between the end caps of the capsule
		Segment3 Segment;
        
        /// Radius of the capsule
		float Radius;
        
        // Public Methods
		// =====================================================

		// ---------------------------------
        /// @name Constructors
        /// @{
        
		Capsule3();
		Capsule3(const Segment3& segment, float radius);
		Capsule3(const Vector3& endpoint1, const Vector3& endpoint2, float radius);
		Capsule3(const Capsule3& input);

        /// @}
        // ---------------------------------
        /// @name Operators
        /// @{
        
		inline Capsule3& operator= (const Capsule3& input);

		inline bool operator== (const Capsule3& input) const;
		inline bool operator!= (const Capsule3& input) const;

        /// @}
        // ---------------------------------
        /// @name Containment Methods
        /// @{
        
        bool Contains(float x, float y, float z) const;
        bool Contains(const Vector3& point) const;

        /// @}
        // ---------------------------------
        /// @name Geometric Methods
        /// @{
        
        Sphere3 GetBoundingSphere() const;
        Box3 GetBoundingBox() const;
        
        /// @}
	};

	/// @}
    /// @}
    
    // Inline Implementations
	// ====================================
    
	// *****************************************************************
    /// @brief
    ///     Assignment operator: copies values of a given capsule to this capsule.
    // *****************************************************************
	inline Capsule3& Capsule3::operator= (const Capsule3& input)
	{
		Segment = input.Segment;
		Radius = input.Radius;
		return *this;
	}

	// *****************************************************************
    /// @brief
    ///     Equality operator:  Returns true if two capsules have the same values
    // *****************************************************************
	inline bool Capsule3::operator== (const Capsule3& input) const
	{
		return Segment == input.Segment && Radius == input.Radius;
	}

	// *****************************************************************
    /// @brief
    ///     Inequality operator:  Returns true if two capsules have different values
    // *****************************************************************
	inline bool Capsule3::operator!= (const Capsule3& input) const
	{
		return Segment != input.Segment || Radius != input.Radius;
	}

} // namespace
