using System;
using System.Collections.Generic;

namespace Gdk
{
    /// <summary>
    /// A quaternion!
    /// </summary>
    public struct Quaternion
    {
        // Member Fields
        public float X, Y, Z, W;

        /// <summary>
        /// Constructor
        /// </summary>
        public Quaternion(float x, float y, float z, float w)
        {
            X = x; 
            Y = y;
            Z = z;
            W = w;
        }

        /// <summary>
        /// Constructor
        /// </summary>
        public Quaternion(float[] values)
        {
            X = values[0]; 
            Y = values[1]; 
            Z = values[2]; 
            W = values[3];
        }

        #region Properties & Accessors

        // Static Quaternions
        // -------------------------------

        /// <summary>
        /// Identity Quaternion
        /// </summary>
        public static Quaternion Identity
        {
            get
            {
                return new Quaternion(0, 0, 0, 1);
            }
        }

        /// <summary>
        /// Zero Quaternion
        /// </summary>
        public static Quaternion Zero
        {
            get
            {
                return new Quaternion(0, 0, 0, 0);
            }
        }

        #endregion

        #region Operators

        /// <summary>
        /// Unary - Negation
        /// </summary>
        public static Quaternion operator -(Quaternion a)
        {
            return new Quaternion(-a.X, -a.Y, -a.Z, -a.W);
        }

        /// <summary>
        /// Binary - Addition
        /// </summary>
        public static Quaternion operator +(Quaternion a, Quaternion b)
        {
            return new Quaternion(a.X + b.X, a.Y + b.Y, a.Z + b.Z, a.W + b.W);
        }

        /// <summary>
        /// Binary - Subtraction
        /// </summary>
        public static Quaternion operator -(Quaternion a, Quaternion b)
        {
            return new Quaternion(a.X - b.X, a.Y - b.Y, a.Z - b.Z, a.W - b.W);
        }

        /// <summary>
        /// Binary - Multiply (Quaternion * Scalar)
        /// </summary>
        public static Quaternion operator *(Quaternion a, float scalar)
        {
            return new Quaternion(a.X * scalar, a.Y * scalar, a.Z * scalar, a.W * scalar);
        }

        /// <summary>
        /// Binary - Multiply (Quaternion * Quaternion)
        /// </summary>
        public static Quaternion operator *(Quaternion a, Quaternion b)
        {
            return new Quaternion(
                a.W * b.X + a.X * b.W + a.Y * b.Z - a.Z * b.Y,
                a.W * b.Y + a.Y * b.W + a.Z * b.X - a.X * b.Z,
                a.W * b.Z + a.Z * b.W + a.X * b.Y - a.Y * b.X,
                a.W * b.W - a.X * b.X - a.Y * b.Y - a.Z * b.Z
                );
        }

        /// <summary>
        /// Binary - Divide (Quaternion / scalar)
        /// </summary>
        public static Quaternion operator /(Quaternion a, float divisor)
        {
            if (divisor != 0.0f)
            {
                float scalar = 1.0f / divisor;
                return new Quaternion(a.X * scalar, a.Y * scalar, a.Z * scalar, a.W * scalar);
            }
            else
            {
                return new Quaternion(float.MaxValue, float.MaxValue, float.MaxValue, float.MaxValue);
            }
        }

        #endregion

        #region Instance Methods
        /*
        // Instance Methods
		Quaternion Inverse() const;
		Quaternion Conjugate() const;

		// Creation / Transformation
		float ToAxisAngle(Vector3& axis) const;
		static Quaternion FromAxisAngle(const Vector3& axis, float angle);
		class Matrix3D ToRotationMatrix() const;
		static Quaternion FromRotationMatrix(const class Matrix3D& matrix);
		static Quaternion FromPitchYawRoll(float pitch, float yaw, float roll);
		static Quaternion FromAlignment(const Vector3& start, const Vector3& end);	// Creates a quaternion that rotates from V1 to V2

		// Interpolation
		static Quaternion Slerp(float t, const Quaternion& start, const Quaternion& end);
		static Quaternion SlerpExtraSpins (float t, const Quaternion& start, const Quaternion& end, int extraSpins);
         */

        /// <summary>
        /// Length
        /// </summary>
	    public float Length()
	    {
		    return (float) Math.Sqrt(W * W + X * X + Y * Y + Z * Z);
	    }

        /// <summary>
        /// Length Squared
        /// </summary>
        public float LengthSquared()
        {
            return (float) W * W + X * X + Y * Y + Z * Z;
        }
	    
        /// <summary>
        /// Dot Product
        /// </summary>
        public float Dot(Quaternion a)
        {
            return W * a.W + X * a.X + Y * a.Y + Z * a.Z;
        }

        /// <summary>
        /// Normalizes the quaternion to unit length
        /// </summary>
        /// <returns></returns>
	    public float Normalize()
        {
            // Get the length of the quaternion
            float length = Length();

            // Is the length non-zero?
            if(length > float.Epsilon)
            {
                // The length is greater than 0
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

        /// <summary>
        /// Gets the inverse of this Quaternion
        /// </summary>
        public Quaternion Inverse()
        {
	        float lengthSquared = this.LengthSquared();
	        if(lengthSquared > 0.0f)
	        {
		        float inverseLengthSquared = 1.0f / lengthSquared;
		        return new Quaternion(
			        -X * inverseLengthSquared, 
			        -Y * inverseLengthSquared,
			        -Z * inverseLengthSquared,
                     W * inverseLengthSquared
			        );
	        }

	        // Return the zero quaternion
	        return Quaternion.Zero;
        }

        /// <summary>
        /// Gets the Conjugate of the Quaternion
        /// </summary>
        public Quaternion Conjugate()
        {
	        return new Quaternion(-X, -Y, -Z, W);
        }

        /// <summary>
        /// Gets the axis & angle of rotation for this quaternion
        /// </summary>
        public float ToAxisAngle(out Vector3 axis)
        {
	        // The quaternion representing the rotation is
            //   q = cos(A/2)+sin(A/2)*(x*i+y*j+z*k)
            axis = new Vector3();
	        float angle = 0.0f;
	        float lengthSquared = X*X + Y*Y + Z*Z;
	        if(lengthSquared > float.Epsilon)
	        {
		        angle = (float)(2.0f * Math.Acos(W));
		        float inverseLength = 1.0f / (float)Math.Sqrt(lengthSquared);
		        axis.X = X * inverseLength;
		        axis.Y = Y * inverseLength;
		        axis.Z = Z * inverseLength;
	        }
            else
            {
		        // Angle is 0, so axis doesnt matter
		        angle = 0.0f;
		        axis.X = 1.0f;
		        axis.Y = 0.0f;
		        axis.Z = 0.0f;
            }
	        return angle;
        }

        /// <summary>
        /// Gets a rotation matrix that contains this Quaternion's rotation
        /// </summary>
        public Matrix ToRotationMatrix()
        {
	        // Compute a bunch of intermediate float vales
	        float x2 = 2.0f * X;
	        float y2 = 2.0f * Y;
	        float z2 = 2.0f * Z;
	        float wx2 = x2 * W;
	        float wy2 = y2 * W;
	        float wz2 = z2 * W;
	        float xx2 = x2 * X;
	        float xy2 = y2 * X;
	        float xz2 = z2 * X;
	        float yy2 = y2 * Y;
	        float yz2 = z2 * Y;
	        float zz2 = z2 * Z;

	        // Setup the the rotation matrix
	        return new Matrix(
		        1.0f - (yy2 + zz2),		xy2 - wz2,				xz2 + wy2,				0.0f,
		        xy2 + wz2,				1.0f - (xx2 + zz2),		yz2 - wx2,				0.0f,
		        xz2 - wy2,				yz2 + wx2,				1.0f - (xx2 + yy2),		0.0f,
		        0.0f,					0.0f,					0.0f,					1.0f
		        );
        }

        #endregion

        #region Static Methods

        /// <summary>
        /// Creates a Quaternion that represents a rotation around an axis
        /// </summary>
        static public Quaternion FromAxisAngle(Vector3 axis, float angle)
        {
	        float halfAngle = angle * 0.5f;
	        float sinAngle = (float) Math.Sin(halfAngle);

	        // W = cos(angle),  X,Y,Z = n * sin(angle)
	        return new Quaternion(
		        axis.X * sinAngle,
		        axis.Y * sinAngle,
		        axis.Z * sinAngle,
                (float) Math.Cos(halfAngle)
		        );
        }

        /// <summary>
        /// Creates a quaternion that represents the rotation within a rotation matrix
        /// </summary>
        static public Quaternion FromRotationMatrix(Matrix matrix)
        {
	        float trace = matrix.M11 + matrix.M22 + matrix.M33;
	        Quaternion quaternion = new Quaternion();

            if (trace > 0.0f)
            {
		        float root = (float)Math.Sqrt(trace + 1.0f);
                quaternion.W = root * 0.5f;
                root = 0.5f / root;
                quaternion.X = (matrix.M23 - matrix.M32) * root;
                quaternion.Y = (matrix.M31 - matrix.M13) * root;
                quaternion.Z = (matrix.M12 - matrix.M21) * root;
                return quaternion;
            }

            if ((matrix.M11 >= matrix.M22) && (matrix.M11 >= matrix.M33))
            {
                float root = (float)Math.Sqrt(1.0f + matrix.M11 - matrix.M22 - matrix.M33);
                float root2 = 0.5f / root;
                quaternion.X = 0.5f * root;
                quaternion.Y = (matrix.M12 + matrix.M21) * root2;
                quaternion.Z = (matrix.M13 + matrix.M31) * root2;
                quaternion.W = (matrix.M23 - matrix.M32) * root2;
                return quaternion;
            }

            if (matrix.M22 > matrix.M33)
            {
                float root = (float)Math.Sqrt(1.0f + matrix.M22 - matrix.M11 - matrix.M33);
                float root2 = 0.5f / root;
                quaternion.X = (matrix.M21 + matrix.M12) * root2;
                quaternion.Y = 0.5f * root;
                quaternion.Z = (matrix.M32 + matrix.M23) * root2;
                quaternion.W = (matrix.M31 - matrix.M13) * root2;
                return quaternion;
            }

            float root3 = (float)Math.Sqrt(1.0f + matrix.M33 - matrix.M11 - matrix.M22);
            float root4 = 0.5f / root3;
            quaternion.X = (matrix.M31 + matrix.M13) * root4;
            quaternion.Y = (matrix.M32 + matrix.M23) * root4;
            quaternion.Z = 0.5f * root3;
            quaternion.W = (matrix.M12 - matrix.M21) * root4;
            return quaternion;

        }

        /// <summary>
        /// Creates a Quaternion which represents rotations around the X, Y, and Z axes
        /// </summary>
        static public Quaternion FromPitchYawRoll(float pitch, float yaw, float roll)
        {
	        float halfPitch = pitch * 0.5f;
	        float halfYaw = yaw * 0.5f;
	        float halfRoll = roll * 0.5f;
            float sinPitch = (float)Math.Sin(halfPitch);
            float cosPitch = (float)Math.Cos(halfPitch);
            float sinYaw = (float)Math.Sin(halfYaw);
            float cosYaw = (float)Math.Cos(halfYaw);
            float sinRoll = (float)Math.Sin(halfRoll);
            float cosRoll = (float)Math.Cos(halfRoll);

	        return new Quaternion(
		        cosYaw * cosPitch * sinRoll - sinYaw * sinPitch * cosRoll,
                cosYaw * cosPitch * cosRoll - sinYaw * sinPitch * sinRoll,
		        cosYaw * sinPitch * cosRoll + sinYaw * cosPitch * sinRoll,
		        sinYaw * cosPitch * cosRoll - cosYaw * sinPitch * sinRoll
		        );
        }

        /// <summary>
        /// Creates a Quaternion which represents a rotation between two vectors
        /// </summary>
        static public Quaternion FromAlignment(Vector3 start, Vector3 end)
        {
	        Vector3 bisector = start + end;
	        bisector.Normalize();

	        float cosHalfAngle = start.Dot(bisector);
	        Vector3 cross;

            Quaternion quaternion = new Quaternion();
	        quaternion.W = cosHalfAngle;

	        if(cosHalfAngle != 0.0f)
	        {
		        cross = start.Cross(bisector);
		        quaternion.X = cross.X;
		        quaternion.Y = cross.Y;
		        quaternion.Z = cross.Z;
	        }
	        else
	        {
		        float inverseLength;
		        if(Math.Abs(start.X) >= Math.Abs(start.Y))
		        {
                    inverseLength = 1.0f / (float)Math.Sqrt(start.Y * start.Y + start.X * start.X);
			        quaternion.X = -start.Z * inverseLength;
			        quaternion.Y = 0.0f;
			        quaternion.Z = start.X * inverseLength;
		        }
                else
                {
                    inverseLength = 1.0f / (float)Math.Sqrt(start.Y * start.Y + start.Z * start.Z);
			        quaternion.X = 0.0f;
			        quaternion.Y = start.Z * inverseLength;
			        quaternion.Y = -start.Y * inverseLength;
                }
            }

	        return quaternion;
        }

        /// <summary>
        /// Creates a quaternion that represents a Spherical Linear Interpolation between two input quaternions
        /// </summary>
        /// <param name="t"></param>
        /// <param name="?"></param>
        /// <returns></returns>
        static public Quaternion Slerp(float t, Quaternion start, Quaternion end)
        {
	        float cos = start.Dot(end);
            float angle = (float)Math.Acos(cos);

	        if(Math.Abs(angle) >= float.Epsilon)
	        {
                float sin = (float)Math.Sin(angle);
		        float invSin = 1.0f / sin;
		        float tAngle = t * angle;
                float a = (float)Math.Sin(angle - tAngle) * invSin;
                float b = (float)Math.Sin(tAngle) * invSin;

		        return new Quaternion(
			        a * start.X + b * end.X,
			        a * start.Y + b * end.Y,
			        a * start.Z + b * end.Z,
                    a * start.W + b * end.W
			        );
	        }
            
            return start;
        }

        /// <summary>
        /// Creates a quaternion that represents a Spherical Linear Interpolation between two input quaternions + a number of extra spins
        /// </summary>
        static public Quaternion SlerpExtraSpins(float t, Quaternion start, Quaternion end, int extraSpins)
        {
	        float cos = start.Dot(end);
            float angle = (float)Math.Acos(cos);

	        if(Math.Abs(angle) >= float.Epsilon)
	        {
                float sin = (float)Math.Sin(angle);
                float phase = (float)Math.PI * extraSpins * t;
		        float invSin = 1.0f / sin;
                float a = (float)Math.Sin((1.0f - t) * angle - phase) * invSin;
                float b = (float)Math.Sin(t * angle + phase) * invSin;

		        return new Quaternion(
			        a * start.X + b * end.X,
			        a * start.Y + b * end.Y,
			        a * start.Z + b * end.Z,
                    a * start.W + b * end.W
			        );
	        }

            return start;
        }
	   
        

        #endregion

    }
}