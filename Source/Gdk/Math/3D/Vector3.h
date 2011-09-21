/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#pragma once


#include "../2D/Vector2.h"

namespace Gdk
{
    /// @addtogroup Math
    /// @{
    /// @addtogroup _3D
    /// @{
    
    // =================================================================================
    /// @brief
    ///     A vector in 3D space
    // =================================================================================
	class Vector3
	{
	public:
		
        // Public Properties
		// =====================================================
        
		float X, Y, Z;

        // Public Methods
		// =====================================================
        
        // ---------------------------------
        /// @name Constructors
        /// @{

		Vector3 ();
		Vector3 (float x, float y, float z);
        Vector3 (const Vector2& xy, float z);
		Vector3 (const Vector3& input);

        /// @}
        // ---------------------------------
        /// @name Operators
        /// @{
        
		inline Vector3& operator= (const Vector3& input);
		inline bool operator== (const Vector3& input) const;
		inline bool operator!= (const Vector3& input) const;

		inline Vector3 operator+ (const Vector3& input) const;
		inline Vector3 operator- (const Vector3& input) const;
		inline Vector3 operator* (float scalar) const;
		inline Vector3 operator/ (float scalar) const;
		inline Vector3 operator- () const;

		inline Vector3& operator+= (const Vector3& input);
		inline Vector3& operator-= (const Vector3& input);
		inline Vector3& operator*= (float scalar);
		inline Vector3& operator/= (float scalar);

        /// @}
        // ---------------------------------
        /// @name Common Vector Methods
        /// @{
        
        inline Vector2 GetXY()    { return Vector2(X,Y); }
        inline Vector2 GetYZ()    { return Vector2(Y,Z); }
        inline Vector2 GetXZ()    { return Vector2(X,Z); }
        
		inline float Length() const;
		inline float LengthSquared() const;
		inline float Normalize();               // normalizes in place
        inline Vector3 GetNormalized() const;   // returns a normalized copy
		inline float Dot (const Vector3& input) const;
		inline Vector3 Cross (const Vector3& input) const;
		inline Vector3 UnitCross (const Vector3& input) const;
        inline static float Distance(const Vector3& v1, const Vector3& v2);
        
        /// @}
        // ---------------------------------
        /// @name Interpolation Methods
        /// @{
        
		static Vector3 Lerp(const Vector3& start, const Vector3& end, float t);
		static Vector3 Hermite(const Vector3& point1, const Vector3& tangent1, const Vector3& point2, const Vector3& tangent2, float t);
		static Vector3 Bezier(const Vector3& point1, const Vector3& point2, const Vector3& point3, const Vector3& point4, float t);
		static Vector3 CatmullRom(const Vector3& point1, const Vector3& point2, const Vector3& point3, const Vector3& point4, float t);	// 4 Control Points, t=0=CP2, t=1=CP3
		static Vector3 Barycentric(const Vector3& point1, const Vector3& point2, const Vector3& point3, float t1, float t2);

        /// @}
        // ---------------------------------
        /// @name Utility Methods
        /// @{
        
		static Vector3 Parse(const char *str);
		string ToString();

		static Vector3 ReadFromStream(class Stream* stream);
		void WriteToStream(class Stream* stream);

		/// @}
        
        // Public Constants
		// =====================================================
        
        // ---------------------------------
        /// @name Static Constants
        /// @{
        
        /// Vector of zeros:  (0,0,0)
		static const Vector3 ZERO;
        
        /// Unit length vector along the +X axis:  (1,0,0)
		static const Vector3 UNIT_X;
        
        /// Unit length vector along the +Y axis:  (0,1,0)
		static const Vector3 UNIT_Y;
        
        /// Unit length vector along the +Z axis:  (0,0,1)
		static const Vector3 UNIT_Z;
        
        /// Vector of ones:  (1,1,1)
		static const Vector3 ONE;
        
        /// @}

	};

	/// @}
    /// @}
    
    // Inline Implementations
	// ====================================
    
	// *****************************************************************
    /// @brief
    ///     Assignement operator: copies the values of the given vector to this vector
    // *****************************************************************
	inline Vector3& Vector3::operator= (const Vector3& input)
	{
		X = input.X;
		Y = input.Y;
		Z = input.Z;
		return *this;
	}

	// *****************************************************************
    /// @brief
    ///     Equality operator: returns true if two vectors have the same values
    // *****************************************************************
	inline bool Vector3::operator== (const Vector3& input) const
	{
		return 
			X == input.X &&
			Y == input.Y &&
			Z == input.Z;
	}

	// *****************************************************************
    /// @brief
    ///     Inequality operator: returns true if two vectors have different values
    // *****************************************************************
	inline bool Vector3::operator!= (const Vector3& input) const
	{
		return 
			X != input.X ||
			Y != input.Y ||
			Z != input.Z;
	}

	// *****************************************************************
    /// @brief
    ///     Adds two vectors
    // *****************************************************************
	inline Vector3 Vector3::operator+ (const Vector3& input) const
	{
		return Vector3(
			X + input.X,
			Y + input.Y,
			Z + input.Z
			);
	}

	// *****************************************************************
    /// @brief
    ///     Subtracts one vector from another
    // *****************************************************************
	inline Vector3 Vector3::operator- (const Vector3& input) const
	{
		return Vector3(
			X - input.X,
			Y - input.Y,
			Z - input.Z
			);
	}

	// *****************************************************************
    /// @brief
    ///     Multiples a vector by a scalar
    // *****************************************************************
	inline Vector3 Vector3::operator* (float scalar) const
	{
		return Vector3(
			X * scalar,
			Y * scalar,
			Z * scalar
			);
	}

	// *****************************************************************
    /// @brief
    ///     Divides a vector by a scalar
    // *****************************************************************
	inline Vector3 Vector3::operator/ (float scalar) const
	{
		if(scalar != 0.0f)
		{
			float inverse = 1.0f / scalar;
			return Vector3(
				X * inverse,
				Y * inverse,
				Z * inverse
				);
		}
		else
		{
			return Vector3(
				FLT_MAX,
				FLT_MAX,
				FLT_MAX
				);
		}
	}

	// *****************************************************************
    /// @brief
    ///     Negates a vector
    // *****************************************************************
	inline Vector3 Vector3::operator- () const
	{
		return Vector3(
			-X,
			-Y,
			-Z
			);
	}

	// *****************************************************************
    /// @brief
    ///     Multiplies a vector by a scalar
    // *****************************************************************
	inline Vector3 operator* (float scalar, const Vector3& input)
	{
		return Vector3(
			scalar * input.X,
			scalar * input.Y,
			scalar * input.Z
			);
	}

	// *****************************************************************
    /// @brief
    ///     Adds a vector into this vector
    // *****************************************************************
	inline Vector3& Vector3::operator+= (const Vector3& input)
	{
		X += input.X;
		Y += input.Y;
		Z += input.Z;
		return *this;
	}

	// *****************************************************************
    /// @brief
    ///     Subtracts a vector from this vector
    // *****************************************************************
	inline Vector3& Vector3::operator-= (const Vector3& input)
	{
		X -= input.X;
		Y -= input.Y;
		Z -= input.Z;
		return *this;
	}

	// *****************************************************************
    /// @brief
    ///     Multiples this vector by a scalar
    // *****************************************************************
	inline Vector3& Vector3::operator*= (float scalar)
	{
		X *= scalar;
		Y *= scalar;
		Z *= scalar;
		return *this;
	}

	// *****************************************************************
    /// @brief
    ///     Divides this vector by a scalar
    // *****************************************************************
	inline Vector3& Vector3::operator/= (float scalar)
	{
		if(scalar != 0.0f)
		{
			float inverse = 1.0f / scalar;
			X *= inverse;
			Y *= inverse;
			Z *= inverse;
		}
		else
		{
			X = FLT_MAX;
			Y = FLT_MAX;
			Z = FLT_MAX;
		}
		return *this;
	}

	// *****************************************************************
    /// @brief
    ///     Calculates the length of this vector
    // *****************************************************************
	inline float Vector3::Length() const
	{
		return Math::Sqrt(
			X * X +
			Y * Y +
			Z * Z
			);
	}

	// *****************************************************************
    /// @brief
    ///     Calculates the length squared of this vector.
    /// @remarks 
    ///     The squared length of a vector is faster to calculate than the actual length.
    // *****************************************************************
	inline float Vector3::LengthSquared() const
	{
		return
			X * X +
			Y * Y +
			Z * Z
			;
	}

	// *****************************************************************
    /// @brief
    ///     Calculates the dot product of this vector and a given vector
    /// @param input
    ///     2nd vector in the dot product calculation
    // *****************************************************************
	inline float Vector3::Dot(const Vector3& input) const
	{
		return 
			X * input.X +
			Y * input.Y +
			Z * input.Z
			;
	}

	// *****************************************************************
    /// @brief
    ///     Normalizes this vector
    /// @return
    ///     The original length of the vector (before being normalized) is returned.
    // *****************************************************************
	inline float Vector3::Normalize()
	{
		float length = Length();

		if (length > Math::ZERO_TOLERANCE)
		{
			// Is the length > Zero tolerance?
			float inverseLength = 1.0f / length;
			X *= inverseLength;
			Y *= inverseLength;
			Z *= inverseLength;
		}
		else
		{
			// The length is approximately 0
			length = 0.0f;
			X = 0.0f;
			Y = 0.0f;
			Z = 0.0f;
		}

		return length;
	}

    // *****************************************************************
    /// @brief
    ///     Calculates a normalized version of this vector.  Without changing this vector
    // *****************************************************************
	inline Vector3 Vector3::GetNormalized() const
	{
        // Clone this vector
		Vector3 normalized(this->X, this->Y, this->Z);
        
        // Normalize the clone
        normalized.Normalize();
        
        return normalized;
    }

	// *****************************************************************
    /// @brief
    ///     Calculates a cross product of this vector and a given vector
    // *****************************************************************
	inline Vector3 Vector3::Cross(const Vector3& input) const
	{
		// Right handed-rule cross product
		return Vector3(
			Y * input.Z - Z * input.Y,
			Z * input.X - X * input.Z,
			X * input.Y - Y * input.X
			);
	}
    
    // *****************************************************************
    /// @brief
    ///     Calculates a normalized cross product of this vector and a given vector
    // *****************************************************************
	inline Vector3 Vector3::UnitCross(const Vector3& input) const
	{
		// Right handed-rule cross product
		Vector3 temp = Cross(input);
        temp.Normalize();
        return temp;
	}
    
    // *****************************************************************
    /// @brief
    ///     Calculates the distance between two points.
    // *****************************************************************
    inline float Vector3::Distance(const Vector3& v1, const Vector3& v2)
    {
        float x = v1.X - v2.X;
        float y = v1.Y - v2.Y;
        float z = v1.Z - v2.Z;
        return Math::Sqrt(x*x + y*y + z*z);
    }

} // namespace
