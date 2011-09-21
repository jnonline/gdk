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
    ///     A matrix for doing transforms in 3D space
    /// @remarks
    ///     This 4x4 matrix correctly performs various 3D space transforms such as translation,
    ///     rotation, scaling, skewing, etc...
    ///   @par
    ///     All look-at and orientation matrices assume the following basis:
    ///     @li - +X = Right
    ///     @li - +Y = Up
    ///     @li - +Z = Out the front of the screen
    // =================================================================================
	class Matrix3D
	{
	public:
        
		// Public Properties
		// =====================================================
        
		float M11, M12, M13, M14;
		float M21, M22, M23, M24;
		float M31, M32, M33, M34;
		float M41, M42, M43, M44;

        // Public Methods
		// =====================================================
        
        // ---------------------------------
        /// @name Constructors
        /// @{
        
		Matrix3D(bool identity = true);
		Matrix3D(float m11, float m12, float m13, float m14, 
                 float m21, float m22, float m23, float m24, 
                 float m31, float m32, float m33, float m34, 
                 float m41, float m42, float m43, float m44);
		Matrix3D(float *components);
		Matrix3D(const Matrix3D& input);

        /// @}
        // ---------------------------------
        /// @name Operators
        /// @{

		inline Matrix3D& operator= (const Matrix3D& input);

		inline bool operator== (const Matrix3D& input) const;
		inline bool operator!= (const Matrix3D& input) const;

		inline Matrix3D operator* (const Matrix3D& input) const;
		inline Matrix3D operator* (float scalar) const;
		inline Matrix3D operator/ (float scalar) const;
		inline Matrix3D operator- () const;

		inline Matrix3D& operator*= (const Matrix3D& input);
		inline Matrix3D& operator*= (float scalar);
		inline Matrix3D& operator/= (float scalar);

        /// @}
        // ---------------------------------
        /// @name Vector Transformation Methods
        /// @{
        
		inline Vector3 TransformPoint(const Vector3& point);
		inline Vector3 TransformDirection(const Vector3& direction);
		inline void TransformPointInPlace(float& x, float& y, float& z);
		inline void TransformDirectionInPlace(float& x, float& y, float& z);
		
        /// @}
        // ---------------------------------
        /// @name In-Place Matrix Transformation Methods
        /// @{
        
		Matrix3D& MakeZero();
		Matrix3D& MakeIdentity();
		Matrix3D& Multiply(const Matrix3D& input);
		Matrix3D& Multiply(float m11, float m12, float m13, float m14, 
                           float m21, float m22, float m23, float m24, 
                           float m31, float m32, float m33, float m34, 
                           float m41, float m42, float m43, float m44);
        Matrix3D& PreMultiply(const Matrix3D& input);
        Matrix3D& PreMultiply(float m11, float m12, float m13, float m14, 
                              float m21, float m22, float m23, float m24, 
                              float m31, float m32, float m33, float m34, 
                              float m41, float m42, float m43, float m44);
        
		Matrix3D& Translate(const Vector3& translation);
		Matrix3D& Translate(float x, float y, float z);
		Matrix3D& Scale(float scalar);
		Matrix3D& Scale(float sx, float sy, float sz);
		Matrix3D& RotateX(float angle);
		Matrix3D& RotateY(float angle);
		Matrix3D& RotateZ(float angle);
		Matrix3D& RotateOnAxis(const Vector3& axis, float angle);
		Matrix3D& RotateQuaternion(const class Quaternion& quat);
		Matrix3D& Reflect(const class Plane3& plane);
        
        /// @}
        // ---------------------------------
        /// @name Matrix Creation Methods
        /// @{
        
		static Matrix3D CreateTranslation(const Vector3& translation);
		static Matrix3D CreateTranslation(float x, float y, float z);
		static Matrix3D CreateScale(float scalar);
		static Matrix3D CreateScale(float sx, float sy, float sz);
		static Matrix3D CreateRotationX(float angle);
		static Matrix3D CreateRotationY(float angle);
		static Matrix3D CreateRotationZ(float angle);
		static Matrix3D CreateRotationOnAxis(const Vector3& axis, float angle);
		static Matrix3D CreateRotationFromQuaternion(const class Quaternion& quat);
		static Matrix3D CreateReflection(const class Plane3& plane);
		static Matrix3D CreateLookAt(const Vector3& position, const Vector3& target, const Vector3& up);
		static Matrix3D CreatePerspectiveFOV(float fov, float aspectRatio, float near, float far);
		static Matrix3D CreateOrtho(float width, float height, float nearPlane, float farPlane);
		static Matrix3D CreateOrthoOffCenter(float left, float right, float bottom, float top, float nearPlane, float farPlane);

        /// @}
        // ---------------------------------
        /// @name Utility Methods
        /// @{
        
		Matrix3D GetTranspose();
		Matrix3D GetInverse();
		float GetDeterminant();

		static Matrix3D ReadFromStream(class Stream* stream);
		void WriteToStream(class Stream* stream);

		/// @}
        
        // Public Constants
		// =====================================================
        
        // ---------------------------------
        /// @name Static Constants
        /// @{

        
		/// The Zero Matrix (all values are 0)
		static const Matrix3D ZERO;
        
        /// The Identity Matrix
		static const Matrix3D IDENTITY;
        
        /// @}
	};
	
	/// @}
    /// @}
    
    // Inline Implementations
	// ====================================
    
	// *****************************************************************
    /// @brief
    ///     Assignement operator: copies the values of the given matrix to this matrix
    // *****************************************************************
	inline Matrix3D& Matrix3D::operator= (const Matrix3D& input)
	{
		M11 = input.M11; M12 = input.M12; M13 = input.M13; M14 = input.M14;
		M21 = input.M21; M22 = input.M22; M23 = input.M23; M24 = input.M24;
		M31 = input.M31; M32 = input.M32; M33 = input.M33; M34 = input.M34;
		M41 = input.M41; M42 = input.M42; M43 = input.M43; M44 = input.M44;
		return *this;
	}

	// *****************************************************************
    /// @brief
    ///     Equality operator: returns true if two matrices have the same values
    // *****************************************************************
	inline bool Matrix3D::operator== (const Matrix3D& input) const
	{
		return M11 == input.M11 && M12 == input.M12 && M13 == input.M13 && M14 == input.M14
			&& M21 == input.M21 && M22 == input.M22 && M23 == input.M23 && M24 == input.M24
			&& M31 == input.M31 && M32 == input.M32 && M33 == input.M33 && M34 == input.M34
			&& M41 == input.M41 && M42 == input.M42 && M43 == input.M43 && M44 == input.M44;
	}

	// *****************************************************************
    /// @brief
    ///     Inequality operator: returns true if two matrices have different values
    // *****************************************************************
	inline bool Matrix3D::operator!= (const Matrix3D& input) const
	{
		return M11 != input.M11 || M12 != input.M12 || M13 != input.M13 || M14 != input.M14
			|| M21 != input.M21 || M22 != input.M22 || M23 != input.M23 || M24 != input.M24
			|| M31 != input.M31 || M32 != input.M32 || M33 != input.M33 || M34 != input.M34
			|| M41 != input.M41 || M42 != input.M42 || M43 != input.M43 || M44 != input.M44;
	}

	// *****************************************************************
    /// @brief
    ///     Multiplies two matrices together
    // *****************************************************************
	inline Matrix3D Matrix3D::operator* (const Matrix3D& input) const
	{
		return Matrix3D(
			M11 * input.M11 + M12 * input.M21 + M13 * input.M31 + M14 * input.M41,
			M11 * input.M12 + M12 * input.M22 + M13 * input.M32 + M14 * input.M42,
			M11 * input.M13 + M12 * input.M23 + M13 * input.M33 + M14 * input.M43,
			M11 * input.M14 + M12 * input.M24 + M13 * input.M34 + M14 * input.M44,

			M21 * input.M11 + M22 * input.M21 + M23 * input.M31 + M24 * input.M41,
			M21 * input.M12 + M22 * input.M22 + M23 * input.M32 + M24 * input.M42,
			M21 * input.M13 + M22 * input.M23 + M23 * input.M33 + M24 * input.M43,
			M21 * input.M14 + M22 * input.M24 + M23 * input.M34 + M24 * input.M44,

			M31 * input.M11 + M32 * input.M21 + M33 * input.M31 + M34 * input.M41,
			M31 * input.M12 + M32 * input.M22 + M33 * input.M32 + M34 * input.M42,
			M31 * input.M13 + M32 * input.M23 + M33 * input.M33 + M34 * input.M43,
			M31 * input.M14 + M32 * input.M24 + M33 * input.M34 + M34 * input.M44,

			M41 * input.M11 + M42 * input.M21 + M43 * input.M31 + M44 * input.M41,
			M41 * input.M12 + M42 * input.M22 + M43 * input.M32 + M44 * input.M42,
			M41 * input.M13 + M42 * input.M23 + M43 * input.M33 + M44 * input.M43,
			M41 * input.M14 + M42 * input.M24 + M43 * input.M34 + M44 * input.M44
			);
	}

	// *****************************************************************
    /// @brief
    ///     Multiplies a matrix by a scalar
    // *****************************************************************
	inline Matrix3D Matrix3D::operator* (float scalar) const
	{
		return Matrix3D(
			M11 * scalar, M12 * scalar, M13 * scalar, M14 * scalar,
			M21 * scalar, M22 * scalar, M23 * scalar, M24 * scalar,
			M31 * scalar, M32 * scalar, M33 * scalar, M34 * scalar,
			M41 * scalar, M42 * scalar, M43 * scalar, M44 * scalar
			);
	}

	// *****************************************************************
    /// @brief
    ///     Divides a matrix by a scalar
    // *****************************************************************
	inline Matrix3D Matrix3D::operator/ (float scalar) const
	{
		if(scalar != 0.0f)
		{
			float inverse = 1.0f / scalar;
			return Matrix3D(
				M11 * inverse, M12 * inverse, M13 * inverse, M14 * inverse,
				M21 * inverse, M22 * inverse, M23 * inverse, M24 * inverse,
				M31 * inverse, M32 * inverse, M33 * inverse, M34 * inverse,
				M41 * inverse, M42 * inverse, M43 * inverse, M44 * inverse
				);
		}
		else
		{
			return Matrix3D(
				FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX,
				FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX,
				FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX,
				FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX
				);
		}
	}

	// *****************************************************************
    /// @brief
    ///     Negates a matrix
    // *****************************************************************
	inline Matrix3D Matrix3D::operator- () const
	{
		return Matrix3D(
			-M11, -M12, -M13, -M14,
			-M21, -M22, -M23, -M24,
			-M31, -M32, -M33, -M43,
			-M41, -M42, -M43, -M44
			);
	}

	// *****************************************************************
    /// @brief
    ///     Multiplies a vector by a matrix, effectively transforming the vector as a point.
    // *****************************************************************

	inline Vector3 operator* (const Vector3& v, const Matrix3D& m)
	{
		return Vector3(
			v.X * m.M11 + v.Y * m.M21 + v.Z * m.M31 + m.M41,
			v.X * m.M12 + v.Y * m.M22 + v.Z * m.M32 + m.M42,
			v.X * m.M13 + v.Y * m.M23 + v.Z * m.M33 + m.M43
			);
	}

	// *****************************************************************
    /// @brief
    ///     Multiplies a given matrix into this matrix
    // *****************************************************************
	inline Matrix3D& Matrix3D::operator*= (const Matrix3D& input)
	{
		float m11 = M11 * input.M11 + M12 * input.M21 + M13 * input.M31 + M14 * input.M41;
		float m12 = M11 * input.M12 + M12 * input.M22 + M13 * input.M32 + M14 * input.M42;
		float m13 = M11 * input.M13 + M12 * input.M23 + M13 * input.M33 + M14 * input.M43;
		float m14 = M11 * input.M14 + M12 * input.M24 + M13 * input.M34 + M14 * input.M44;

		float m21 = M21 * input.M11 + M22 * input.M21 + M23 * input.M31 + M24 * input.M41;
		float m22 = M21 * input.M12 + M22 * input.M22 + M23 * input.M32 + M24 * input.M42;
		float m23 = M21 * input.M13 + M22 * input.M23 + M23 * input.M33 + M24 * input.M43;
		float m24 = M21 * input.M14 + M22 * input.M24 + M23 * input.M34 + M24 * input.M44;

		float m31 = M31 * input.M11 + M32 * input.M21 + M33 * input.M31 + M34 * input.M41;
		float m32 = M31 * input.M12 + M32 * input.M22 + M33 * input.M32 + M34 * input.M42;
		float m33 = M31 * input.M13 + M32 * input.M23 + M33 * input.M33 + M34 * input.M43;
		float m34 = M31 * input.M14 + M32 * input.M24 + M33 * input.M34 + M34 * input.M44;

		float m41 = M41 * input.M11 + M42 * input.M21 + M43 * input.M31 + M44 * input.M41;
		float m42 = M41 * input.M12 + M42 * input.M22 + M43 * input.M32 + M44 * input.M42;
		float m43 = M41 * input.M13 + M42 * input.M23 + M43 * input.M33 + M44 * input.M43;
		float m44 = M41 * input.M14 + M42 * input.M24 + M43 * input.M34 + M44 * input.M44;
		
		M11 = m11; M12 = m12; M13 = m13; M13 = m14;
		M21 = m21; M22 = m22; M23 = m23; M24 = m24;
		M31 = m31; M32 = m32; M33 = m33; M34 = m34;
		M41 = m41; M42 = m42; M43 = m43; M44 = m44;
		return *this;
	}


	// *****************************************************************
    /// @brief
    ///     Multiplies this matrix by a scalar
    // *****************************************************************
	inline Matrix3D& Matrix3D::operator*= (float scalar)
	{
		M11 *= scalar; M12 *= scalar; M13 *= scalar; M14 *= scalar;
		M21 *= scalar; M22 *= scalar; M23 *= scalar; M24 *= scalar;
		M31 *= scalar; M32 *= scalar; M33 *= scalar; M34 *= scalar;
		M41 *= scalar; M42 *= scalar; M43 *= scalar; M44 *= scalar;
		return *this;
	}

	// *****************************************************************
    /// @brief
    ///     Divides this matrix by a scalar
    // *****************************************************************
	inline Matrix3D& Matrix3D::operator/= (float scalar)
	{
		if(scalar != 0.0f)
		{
			float inverse = 1.0f / scalar;
			M11 *= inverse; M12 *= inverse; M13 *= inverse; M14 *= inverse; 
			M21 *= inverse; M22 *= inverse; M23 *= inverse; M24 *= inverse; 
			M31 *= inverse; M32 *= inverse; M33 *= inverse; M34 *= inverse; 
			M41 *= inverse; M42 *= inverse; M43 *= inverse; M44 *= inverse; 
		}
		else
		{
			M11 = FLT_MAX; M12 = FLT_MAX; M13 = FLT_MAX; M14 = FLT_MAX;
			M21 = FLT_MAX; M22 = FLT_MAX; M23 = FLT_MAX; M24 = FLT_MAX;
			M31 = FLT_MAX; M32 = FLT_MAX; M33 = FLT_MAX; M34 = FLT_MAX;
			M41 = FLT_MAX; M42 = FLT_MAX; M43 = FLT_MAX; M44 = FLT_MAX;
		}
		return *this;
	}

	// *****************************************************************
    /// @brief
    ///     Transforms a point vector by this matrix.
    /// @remarks
    ///     The complete transformation of the matrix is applied to the 
    ///     point.
    // *****************************************************************
	inline Vector3 Matrix3D::TransformPoint(const Vector3& point)
	{
		return Vector3(
			point.X * M11 + point.Y * M21 + point.Z * M31 + M41,
			point.X * M12 + point.Y * M22 + point.Z * M32 + M42,
			point.X * M13 + point.Y * M23 + point.Z * M33 + M43
			);
	}

    // *****************************************************************
    /// @brief
    ///     Transforms a direction vector by this matrix (ignoring translation).
    /// @remarks
    ///     Only the rotation and scale factors of the matrix are applied to 
    ///     the direction.  The translation component of the matrix will NOT be applied
    // *****************************************************************
	inline Vector3 Matrix3D::TransformDirection(const Vector3& direction)
	{
		return Vector3(
			direction.X * M11 + direction.Y * M21 + direction.Z * M31,
			direction.X * M12 + direction.Y * M22 + direction.Z * M32,
			direction.X * M13 + direction.Y * M23 + direction.Z * M33
			);
	}
    
    // *****************************************************************
    /// @brief
    ///     Transforms an (x,y) point by this matrix.
    /// @remarks
    ///     The complete transformation of the matrix is applied to the 
    ///     point directly.
    /// @param[in,out] x
    ///     X coordinate of the point to be transformed
    /// @param[in,out] y
    ///     Y coordinate of the point to be transformed
    /// @param[in,out] z
    ///     Z coordinate of the point to be transformed
    // *****************************************************************
	inline void Matrix3D::TransformPointInPlace(float &x, float &y, float &z)
	{
		float tempX = x * M11 + y * M21 + z * M31 + M41;
		float tempY = x * M12 + y * M22 + z * M32 + M42;
		float tempZ = x * M13 + y * M23 + z * M33 + M43;

		x = tempX; y = tempY; z = tempZ;
	}

    // *****************************************************************
    ///  @brief
    ///     Transforms an (x,y) direction by this matrix (ignoring translation).
    /// @remarks
    ///     Only the rotation and scale factors of the matrix are applied to 
    ///     the direction.  The translation component of the matrix will NOT be applied
    /// @param[in,out] x
    ///     X component of the direction to be transformed
    /// @param[in,out] y
    ///     Y component of the direction to be transformed
    /// @param[in,out] z
    ///     Z component of the direction to be transformed
    // *****************************************************************
	inline void Matrix3D::TransformDirectionInPlace(float &x, float &y, float &z)
	{
		float tempX = x * M11 + y * M21 + z * M31;
		float tempY = x * M12 + y * M22 + z * M32;
		float tempZ = x * M13 + y * M23 + z * M33;

		x = tempX; y = tempY; z = tempZ;
	}


} // namespace
