/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#pragma once


namespace Gdk
{
    /// @addtogroup Math
    /// @{
    /// @addtogroup _3D
    /// @{
    
    // =================================================================================
    /// @brief
    ///     Represents a 3D sphere.
    // =================================================================================
	class Sphere3
	{
	public:
        
        // Public Properties
		// =====================================================
        
		/// Center of the sphere
		Vector3 Center;
        
        /// Radius of the sphere
		float Radius;
        
        // Public Methods
		// =====================================================

		// ---------------------------------
        /// @name Constructors
        /// @{
        
		Sphere3();
		Sphere3(float x, float y, float z, float radius);
		Sphere3(const Vector3& center, float radius);
		Sphere3(const Sphere3& input);

        /// @}
        // ---------------------------------
        /// @name Operators
        /// @{
        
		inline Sphere3& operator= (const Sphere3& input);

		inline bool operator== (const Sphere3& input) const;
		inline bool operator!= (const Sphere3& input) const;

        /// @}
        // ---------------------------------
        /// @name Containment Methods
        /// @{
        
        inline bool Contains(float x, float y, float z) const;
		inline bool Contains(const Vector3& point) const;

        /// @}
        // ---------------------------------
        /// @name Geometric Methods
        /// @{
        
        static Sphere3 FromPoints(size_t numPoints, const Vector3* points);
		static Sphere3 Merge(const Sphere3& sphere1, const Sphere3& sphere2);
        
        /// @}
	};

	/// @}
    /// @}
    
    // Inline Implementations
	// ====================================
    
	// *****************************************************************
    /// @brief
    ///     Assignment operator: copies values of a given sphere to this sphere.
    // *****************************************************************
	inline Sphere3& Sphere3::operator= (const Sphere3& input)
	{
		Center = input.Center;
		Radius = input.Radius;
		return *this;
	}

	// *****************************************************************
    /// @brief
    ///     Equality operator:  Returns true if two spheres have the same values
    // *****************************************************************
	inline bool Sphere3::operator== (const Sphere3& input) const
	{
		return 
			Center == input.Center && Radius == input.Radius;
	}

	// *****************************************************************
    /// @brief
    ///     Inequality operator:  Returns true if two spheres have different values
    // *****************************************************************
	inline bool Sphere3::operator!= (const Sphere3& input) const
	{
		return 
			Center != input.Center || Radius != input.Radius;
	}

	// *****************************************************************
    /// @brief
    ///     Checks if the sphere contains a given point
    // *****************************************************************
	inline bool Sphere3::Contains(float x, float y, float z) const
	{
		return Contains(Vector3(x,y,z));
	}
    
	// *****************************************************************
    /// @brief
    ///     Checks if the sphere contains a given point
    // *****************************************************************
	inline bool Sphere3::Contains(const Vector3& point) const
	{
		// Get the distance squared between the center & the point
		float distanceSquared = (this->Center - point).LengthSquared();
		return distanceSquared <= this->Radius * this->Radius;
	}

} // namespace
