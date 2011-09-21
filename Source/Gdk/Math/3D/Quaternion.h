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
    ///     Represents arbitrary rotations and orientations in 3D space
    /// @remarks
    ///     Quaternions are 4D vectors (w,x,y,z) which are used to efficiently rotate about a vector (x, y, z)
    ///     by the angle theta, where w = cos(theta/2).
    // =================================================================================
	class Quaternion
	{
	public:
		
        // Public Properties
		// =====================================================
        
		float W, X, Y, Z;

        // Public Methods
		// =====================================================
        
        // ---------------------------------
        /// @name Constructors
        /// @{

		Quaternion();
		Quaternion(float w, float x, float y, float z);
		Quaternion(const Quaternion& input);

        /// @}
        // ---------------------------------
        /// @name Operators
        /// @{
        
		inline Quaternion& operator= (const Quaternion& input);

		inline bool operator== (const Quaternion& input) const;
		inline bool operator!= (const Quaternion& input) const;

		inline Quaternion operator+ (const Quaternion& input) const;
		inline Quaternion operator- (const Quaternion& input) const;
		inline Quaternion operator* (const Quaternion& input) const;
		inline Quaternion operator* (float scalar) const;
		inline Quaternion operator/ (float scalar) const;
		inline Quaternion operator- () const;

		inline Quaternion& operator+= (const Quaternion& input);
		inline Quaternion& operator-= (const Quaternion& input);
		inline Quaternion& operator*= (float scalar);
		inline Quaternion& operator/= (float scalar);

        /// @}
        // ---------------------------------
        /// @name Common Quaternion Methods
        /// @{
        
		inline float Length() const;
		inline float LengthSquared() const;
		inline float Normalize();
		inline float Dot(const Quaternion& input) const;
		Quaternion Inverse() const;
		Quaternion Conjugate() const;

		float ToAxisAngle(Vector3& axis) const;
		static Quaternion FromAxisAngle(const Vector3& axis, float angle);
		class Matrix3D ToRotationMatrix() const;
		static Quaternion FromRotationMatrix(const class Matrix3D& matrix);
		static Quaternion FromPitchYawRoll(float pitch, float yaw, float roll);
		static Quaternion FromAlignment(const Vector3& start, const Vector3& end);	// Creates a quaternion that rotates from V1 to V2

        /// @}
        // ---------------------------------
        /// @name Interpolation Methods
        /// @{
        
		static Quaternion Slerp(float t, const Quaternion& start, const Quaternion& end);
		static Quaternion SlerpExtraSpins (float t, const Quaternion& start, const Quaternion& end, int extraSpins);
		
        /// @}
        
        // Public Constants
		// =====================================================
        
        // ---------------------------------
        /// @name Static Constants
        /// @{
        
		/// Zero quaternion:  All 0s
		static const Quaternion ZERO;
        
        /// Identity quaternion
		static const Quaternion IDENTITY;	// (1)(0,0,0)
        
        /// @}
	};

	/// @}
    /// @}
    
    // Inline Implementations
	// ====================================
    
	// *****************************************************************
    /// @brief
    ///     Assignement operator: copies the values of the given quaternion to this quaternion
    // *****************************************************************
	inline Quaternion& Quaternion::operator= (const Quaternion& input)
	{
		W = input.W;
		X = input.X;
		Y = input.Y;
		Z = input.Z;
		return *this;
	}

	// *****************************************************************
    /// @brief
    ///     Equality operator: returns true if two quaternions have the same values
    // *****************************************************************
	inline bool Quaternion::operator== (const Quaternion& input) const
	{
		return
			W == input.W &&
			X == input.X &&
			Y == input.Y &&
			Z == input.Z;
	}

	// *****************************************************************
    /// @brief
    ///     Inequality operator: returns true if two quaternionss have different values
    // *****************************************************************
	inline bool Quaternion::operator!= (const Quaternion& input) const
	{
		return 
			W != input.W ||
			X != input.X ||
			Y != input.Y ||
			Z != input.Z;
	}

	// *****************************************************************
    /// @brief
    ///     Adds two quaterions together
    // *****************************************************************
	inline Quaternion Quaternion::operator+ (const Quaternion& input) const
	{
		return Quaternion(
			W + input.W,
			X + input.X,
			Y + input.Y,
			Z + input.Z
			);
	}

	// *****************************************************************
    /// @brief
    ///     Subtracts one quaternion from another
    // *****************************************************************
	inline Quaternion Quaternion::operator- (const Quaternion& input) const
	{
		return Quaternion(
			W - input.W,
			X - input.X,
			Y - input.Y,
			Z - input.Z
			);
	}

	// *****************************************************************
    /// @brief
    ///     Multiplies two quaternions.
    // *****************************************************************
	inline Quaternion Quaternion::operator* (const Quaternion& input) const
	{
		return Quaternion(
			W * input.W - X * input.X - Y * input.Y - Z * input.Z,
			W * input.X + X * input.W + Y * input.Z - Z * input.Y,
			W * input.Y + Y * input.W + Z * input.X - X * input.Z,
			W * input.Z + Z * input.W + X * input.Y - Y * input.X
			);
	}

	// *****************************************************************
    /// @brief
    ///     Multiples a quaternion by a scalar
    // *****************************************************************
	inline Quaternion Quaternion::operator* (float scalar) const
	{
		return Quaternion(
			W * scalar,
			X * scalar,
			Y * scalar,
			Z * scalar
			);
	}

	// *****************************************************************
    /// @brief
    ///     Divides a quaternion by a scalar
    // *****************************************************************
	inline Quaternion Quaternion::operator/ (float scalar) const
	{
		if(scalar != 0.0f)
		{
			float inverse = 1.0f / scalar;
			return Quaternion(
				W * inverse,
				X * inverse,
				Y * inverse,
				Z * inverse
				);
		}
		else
		{
			return Quaternion(
				FLT_MAX,
				FLT_MAX,
				FLT_MAX,
				FLT_MAX
				);
		}
	}

	// *****************************************************************
    /// @brief
    ///     Negates a quaternion
    // *****************************************************************
	inline Quaternion Quaternion::operator- () const
	{
		return Quaternion(
			-W,
			-X,
			-Y,
			-Z
			);
	}

	// *****************************************************************
    /// @brief
    ///     Multiples a scalar and a quaternion
    // *****************************************************************
	inline Quaternion operator* (float scalar, const Quaternion& input)
	{
		return Quaternion(
			scalar * input.W,
			scalar * input.X,
			scalar * input.Y,
			scalar * input.Z
			);
	}

	// *****************************************************************
    /// @brief
    ///     Adds a quaternion to this quaternion
    // *****************************************************************
	inline Quaternion& Quaternion::operator+= (const Quaternion& input)
	{
		W += input.W;
		X += input.X;
		Y += input.Y;
		Z += input.Z;
		return *this;
	}

    // *****************************************************************
    /// @brief
    ///     Subtracts a quaternion from this quaternion
    // *****************************************************************
	inline Quaternion& Quaternion::operator-= (const Quaternion& input)
	{
		W -= input.W;
		X -= input.X;
		Y -= input.Y;
		Z -= input.Z;
		return *this;
	}

    // *****************************************************************
    /// @brief
    ///     Multiples this quaternion by a scalar
    // *****************************************************************
	inline Quaternion& Quaternion::operator*= (float scalar)
	{
		W *= scalar;
		X *= scalar;
		Y *= scalar;
		Z *= scalar;
		return *this;
	}

    // *****************************************************************
    /// @brief
    ///     Divides this quaternion by a scalar
    // *****************************************************************
	inline Quaternion& Quaternion::operator/= (float scalar)
	{
		if(scalar != 0.0f)
		{
			float inverse = 1.0f / scalar;
			W *= inverse;
			X *= inverse;
			Y *= inverse;
			Z *= inverse;
		}
		else
		{
			W = FLT_MAX;
			X = FLT_MAX;
			Y = FLT_MAX;
			Z = FLT_MAX;
		}
		return *this;
	}

	// *****************************************************************
    /// @brief
    ///     Calculates the length of a quaterion
    // *****************************************************************
	inline float Quaternion::Length() const
	{
		return Math::Sqrt(
			W * W +
			X * X +
			Y * Y +
			Z * Z
			);
	}

    // *****************************************************************
    /// @brief
    ///     Calculates the length squared of a quaterion
    // *****************************************************************
	inline float Quaternion::LengthSquared() const
	{
		return
			W * W +
			X * X +
			Y * Y +
			Z * Z
			;
	}

    // *****************************************************************
    /// @brief
    ///     Calculates the Dot product of this quaternion and a given quaterion
    // *****************************************************************
	inline float Quaternion::Dot(const Quaternion& input) const
	{
		return 
			W * input.W +
			X * input.X +
			Y * input.Y +
			Z * input.Z
			;
	}

	// *****************************************************************
    /// @brief
    ///     Normalizes this quaternion
    // *****************************************************************
	inline float Quaternion::Normalize()
	{
		float length = Length();

		if (length > Math::ZERO_TOLERANCE)
		{
			// Is the length > Zero tolerance?
			float inverseLength = 1.0f / length;
			W *= inverseLength;
			X *= inverseLength;
			Y *= inverseLength;
			Z *= inverseLength;
		}
		else
		{
			// The length is approximately 0
			length = 0.0f;
			W = 0.0f;
			X = 0.0f;
			Y = 0.0f;
			Z = 0.0f;
		}

		return length;
	}

} // namespace
