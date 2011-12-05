/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#pragma once



namespace Gdk
{
    /// @addtogroup Math
    /// @{
    /// @addtogroup _2D
    /// @{
    
    // =================================================================================
    /// @brief
    ///     A vector in 2D space
    // =================================================================================
	class Vector2
	{
	public:
		
        // Public Properties
		// =====================================================
        
		float X, Y;

        // Public Methods
		// =====================================================

        // ---------------------------------
        /// @name Constructors
        /// @{
        
		Vector2 ();
		Vector2 (float x, float y);
		Vector2 (const Vector2& input);

        /// @}
        // ---------------------------------
        /// @name Operators
        /// @{
        
		inline Vector2& operator= (const Vector2& input);
		inline bool operator== (const Vector2& input) const;
		inline bool operator!= (const Vector2& input) const;

		inline Vector2 operator+ (const Vector2& input) const;
		inline Vector2 operator- (const Vector2& input) const;
		inline Vector2 operator* (float scalar) const;
		inline Vector2 operator/ (float scalar) const;
		inline Vector2 operator- () const;

		inline Vector2& operator+= (const Vector2& input);
		inline Vector2& operator-= (const Vector2& input);
		inline Vector2& operator*= (float scalar);
		inline Vector2& operator/= (float scalar);
        
        /// @}
        // ---------------------------------
        /// @name Common Vector Methods
        /// @{

		inline float Length () const;
		inline float LengthSquared () const;
		inline float Normalize ();
        inline Vector2 GetNormalized() const;
		inline float Dot (const Vector2& input) const;
        inline Vector2 GetPerpendicular() const;
		inline float GetRotationAngle() const;
		inline static Vector2 FromAngle(float angle);
        inline static float Distance(const Vector2& v1, const Vector2& v2);
        
        /// @}
        // ---------------------------------
        /// @name Coordinate System Conversion Methods
        /// @{
        
        void ToPolarCoordinate(float& r, float& theta);
        static Vector2 FromPolarCoordinate(float r, float theta);
        
        /// @}
        // ---------------------------------
        /// @name Interpolation Methods
        /// @{
        
		static Vector2 Lerp(const Vector2& start, const Vector2& end, float t);
		static Vector2 Hermite(const Vector2& point1, const Vector2& tangent1, const Vector2& point2, const Vector2& tangent2, float t);
		static Vector2 Bezier(const Vector2& point1, const Vector2& point2, const Vector2& point3, const Vector2& point4, float t);
		static Vector2 CatmullRom(const Vector2& point1, const Vector2& point2, const Vector2& point3, const Vector2& point4, float t);	// 4 Control Points, t=0=CP2, t=1=CP3
		static Vector2 Barycentric(const Vector2& point1, const Vector2& point2, const Vector2& point3, float t1, float t2);

        /// @}
        // ---------------------------------
        /// @name Utility Methods
        /// @{
        
		static Vector2 Parse(const char *str);
		string ToString();

		static Vector2 ReadFromStream(class Stream* stream);
		void WriteToStream(class Stream* stream);

        /// @}
        
        // Public Constants
		// =====================================================
        
        // ---------------------------------
        /// @name Static Constants
        /// @{

        /// Vector of zeros:  (0,0)
		static const Vector2 ZERO;
        
        /// Unit length vector along the +X axis:  (1,0)
		static const Vector2 UNIT_X;
        
        /// Unit length vector along the +Y axis:  (0,1)
		static const Vector2 UNIT_Y;
        
        /// Vector of ones:  (1,1)
		static const Vector2 ONE;
        
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
	inline Vector2& Vector2::operator= (const Vector2& input)
	{
		X = input.X;
		Y = input.Y;
		return *this;
	}

	// *****************************************************************
    /// @brief
    ///     Equality operator: returns true if two vectors have the same values
    // *****************************************************************
    inline bool Vector2::operator== (const Vector2& input) const
	{
		return 
			X == input.X &&
			Y == input.Y;
	}

	// *****************************************************************
    /// @brief
    ///     Inequality operator: returns true if two vectors have different values
    // *****************************************************************
	inline bool Vector2::operator!= (const Vector2& input) const
	{
		return 
			X != input.X ||
			Y != input.Y;
	}

	// *****************************************************************
    /// @brief
    ///     Adds two vectors
    // *****************************************************************
    inline Vector2 Vector2::operator+ (const Vector2& input) const
	{
		return Vector2(
			X + input.X,
			Y + input.Y
			);
	}

	// *****************************************************************
    /// @brief
    ///     Subtracts one vector from another
    // *****************************************************************
	inline Vector2 Vector2::operator- (const Vector2& input) const
	{
		return Vector2(
			X - input.X,
			Y - input.Y
			);
	}

	// *****************************************************************
    /// @brief
    ///     Multiples a vector by a scalar
    // *****************************************************************
	inline Vector2 Vector2::operator* (float scalar) const
	{
		return Vector2(
			X * scalar,
			Y * scalar
			);
	}

	// *****************************************************************
    /// @brief
    ///     Divides a vector by a scalar
    // *****************************************************************
	inline Vector2 Vector2::operator/ (float scalar) const
	{
		if(scalar != 0.0f)
		{
			float inverse = 1.0f / scalar;
			return Vector2(
				X * inverse,
				Y * inverse
				);
		}
		else
		{
			return Vector2(
				FLT_MAX,
				FLT_MAX
				);
		}
	}

	// *****************************************************************
    /// @brief
    ///     Negates a vector
    // *****************************************************************
	inline Vector2 Vector2::operator- () const
	{
		return Vector2(
			-X,
			-Y
			);
	}

	// *****************************************************************
    /// @brief
    ///     Multiplies a vector by a scalar
    // *****************************************************************
	inline Vector2 operator* (float scalar, const Vector2& input)
	{
		return Vector2(
			scalar * input.X,
			scalar * input.Y
			);
	}

	// *****************************************************************
    /// @brief
    ///     Adds a vector into this vector
    // *****************************************************************
	inline Vector2& Vector2::operator+= (const Vector2& input)
	{
		X += input.X;
		Y += input.Y;
		return *this;
	}

	// *****************************************************************
    /// @brief
    ///     Subtracts a vector from this vector
    // *****************************************************************
	inline Vector2& Vector2::operator-= (const Vector2& input)
	{
		X -= input.X;
		Y -= input.Y;
		return *this;
	}

	// *****************************************************************
    /// @brief
    ///     Multiples this vector by a scalar
    // *****************************************************************
	inline Vector2& Vector2::operator*= (float scalar)
	{
		X *= scalar;
		Y *= scalar;
		return *this;
	}

	// *****************************************************************
    /// @brief
    ///     Divides this vector by a scalar
    // *****************************************************************
	inline Vector2& Vector2::operator/= (float scalar)
	{
		if(scalar != 0.0f)
		{
			float inverse = 1.0f / scalar;
			X *= inverse;
			Y *= inverse;
		}
		else
		{
			X = FLT_MAX;
			Y = FLT_MAX;
		}
		return *this;
	}

	// *****************************************************************
    /// @brief
    ///     Calculates the length of this vector
    // *****************************************************************
	inline float Vector2::Length() const
	{
		return Math::Sqrt(
			X * X +
			Y * Y
			);
	}

	// *****************************************************************
    /// @brief
    ///     Calculates the length squared of this vector.
    /// @remarks 
    ///     The squared length of a vector is faster to calculate than the actual length.
    // *****************************************************************
	inline float Vector2::LengthSquared() const
	{
		return
			X * X +
			Y * Y
			;
	}

	// *****************************************************************
    /// @brief
    ///     Calculates the dot product of this vector and a given vector
    /// @param input
    ///     2nd vector in the dot product calculation
    // *****************************************************************
	inline float Vector2::Dot(const Vector2& input) const
	{
		return 
			X * input.X +
			Y * input.Y
			;
	}

	// *****************************************************************
    /// @brief
    ///     Normalizes this vector
    /// @return
    ///     The original length of the vector (before being normalized) is returned.
    // *****************************************************************
	inline float Vector2::Normalize()
	{
		float length = Length();

		if (length > Math::ZERO_TOLERANCE)
		{
			// Is the length > Zero tolerance?
			float inverseLength = 1.0f / length;
			X *= inverseLength;
			Y *= inverseLength;
		}
		else
		{
			// The length is approximately 0
			length = 0.0f;
			X = 0.0f;
			Y = 0.0f;
		}

		return length;
	}
    
    // *****************************************************************
    /// @brief
    ///     Calculates a normalized version of this vector.  Without changing this vector
    // *****************************************************************
	inline Vector2 Vector2::GetNormalized() const
	{
        // Clone this vector
		Vector2 normalized(this->X, this->Y);
        
        // Normalize the clone
        normalized.Normalize();
        
        return normalized;
    }

    // *****************************************************************
    /// @brief
    ///     Calculates the perpendicular of this vector
    /// @remarks
    ///     The perpedicular of a 2D vector is (-Y, X)
    // *****************************************************************
	inline Vector2 Vector2::GetPerpendicular() const
    {
        return Vector2(-Y, X);
    }
    
	// *****************************************************************
    /// @brief
    ///     Calculates the angle this vector is facing. (in radians)
    /// @remarks
    ///     (1,0) = 0 radians
    ///     (0,1) = Pi/2 radians
    // *****************************************************************
	inline float Vector2::GetRotationAngle() const
	{
		return Math::ATan2(Y, X);	
	}
    
    // *****************************************************************
    /// @brief
    ///     Creates a unit vector facing the given angle (in radians)
    /// @remarks
    ///     0 radians = (1,0)
    ///     Pi/2 radians = (0,1)
    // *****************************************************************
    inline Vector2 Vector2::FromAngle(float angle)
    {
        return Vector2( 
            Math::Cos(angle), 
            Math::Sin(angle) 
            );
    }
    
	// *****************************************************************
    /// @brief
    ///     Calculates the distance between two points
    // *****************************************************************
    inline float Vector2::Distance(const Vector2& v1, const Vector2& v2)
    {
        float x = v1.X - v2.X;
        float y = v1.Y - v2.Y;
        return Math::Sqrt(x*x + y*y);
    }

} // namespace
