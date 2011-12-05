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
    ///     Represents a 3D oriented box.
    /// @remarks
    ///     Oriented boxes are defined by a center point, an orthonormal basis for axes, 
    ///     and extents (half-sizes) along the axes
    // =================================================================================
	class Box3
	{
	public:
		
        // Public Properties
		// =====================================================
        
        /// Center of the box
		Vector3 Center;
        
        /// Three orthonormalized axes of the box
		Vector3 Axis[3];
        
        /// Extent (half-size) of the box along each axis
		float Extent[3];

        // Public Methods
		// =====================================================
        
        // ---------------------------------
        /// @name Constructors
        /// @{

		Box3 ();
		Box3 (const Vector3& center, const Vector3& axis1, const Vector3& axis2, const Vector3& axis3, float extent1, float extent2, float extent3);
		Box3 (const Box3& input);

        /// @}
        // ---------------------------------
        /// @name Operators
        /// @{
        
		// Asssignment Operator
		inline Box3& operator= (const Box3& input);

		// Comparison Operators
		inline bool operator== (const Box3& input) const;
		inline bool operator!= (const Box3& input) const;

        /// @}
        // ---------------------------------
        /// @name Containment Methods
        /// @{
        
		// Containment Methods
        inline bool Contains(float x, float y, float z) const;
		inline bool Contains(const Vector3& point) const;
        
        /// @}
        
	};

	/// @}
    /// @}
    
    // Inline Implementations
	// ====================================
    
	// *****************************************************************
    /// @brief
    ///     Assignment operator: copies the values of the given box to this box
    // *****************************************************************
	inline Box3& Box3::operator= (const Box3& input)
	{
		Axis[0] = input.Axis[0];
		Axis[1] = input.Axis[1];
		Axis[2] = input.Axis[2];
		Extent[0] = input.Extent[0];
		Extent[1] = input.Extent[1];
		Extent[2] = input.Extent[2];
		Center = input.Center;
		return *this;
	}

	// *****************************************************************
    /// @brief
    ///     Equality operator:  Returns true if two boxes have the same values
    // *****************************************************************
	inline bool Box3::operator== (const Box3& input) const
	{
		return Axis[0] == input.Axis[0] 
			&& Axis[1] == input.Axis[1] 
			&& Axis[2] == input.Axis[2]
			&& Extent[0] == input.Extent[0] 
			&& Extent[1] == input.Extent[1] 
			&& Extent[2] == input.Extent[2]
			&& Center == Center;
	}

	// *****************************************************************
    /// @brief
    ///     Inequality operator:  Returns true if two boxes have different values
    // *****************************************************************
	inline bool Box3::operator!= (const Box3& input) const
	{
		return Axis[0] != input.Axis[0] 
			|| Axis[1] != input.Axis[1] 
			|| Axis[2] != input.Axis[2]
			|| Extent[0] != input.Extent[0] 
			|| Extent[1] != input.Extent[1] 
			|| Extent[2] != input.Extent[2]
			|| Center != Center;
	}

	// *****************************************************************
    /// @brief
    ///     Checks if the box contains a given point
    // *****************************************************************
    inline bool Box3::Contains(float x, float y, float z) const
	{
		return Contains(Vector3(x,y,z));
	}
    
	// *****************************************************************
    /// @brief
    ///     Checks if the box contains a given point
    // *****************************************************************
	inline bool Box3::Contains(const Vector3& point) const
	{
		// Get the vector from the center of the box to the point
		Vector3 diff = point - this->Center;
		
		// Loop through the axis
		for(int i=0; i<3; i++)
		{
			// Get the distance from the point to the center along this axis
			float dist = diff.Dot(this->Axis[i]);

			// Is the point outside the extent of this axis?
			if(Math::Abs(dist) > this->Extent[i])
				return false;
		}

		return true;
	}


} // namespace

