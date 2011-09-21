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
    ///     Represents a 3D ray.
    // =================================================================================
	class Ray3
	{
	public:
		
        // Public Properties
		// =====================================================
        
        /// Origin of the ray
		Vector3 Origin;
        
        /// Direction of the ray.  (Most GDK math operations assume this is normalized)
		Vector3 Direction;

        // Public Methods
		// =====================================================
        
		// ---------------------------------
        /// @name Constructors
        /// @{
        
		Ray3();
		Ray3(const Vector3& origin, const Vector3& direction);
		Ray3(const Ray3& input);

        /// @}
        // ---------------------------------
        /// @name Operators
        /// @{
        
		inline Ray3& operator= (const Ray3& input);

        /// @}
	};

	/// @}
    /// @}
    
    // Inline Implementations
	// ====================================
    
	// *****************************************************************
    /// @brief
    ///     Assignment operator: copies values of a given ray to this ray.
    // *****************************************************************
	inline Ray3& Ray3::operator= (const Ray3& input)
	{
		Origin = input.Origin;
		Direction = input.Direction;
		return *this;
	}


} // namespace
