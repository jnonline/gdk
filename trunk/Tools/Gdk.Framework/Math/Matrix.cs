using System;
using System.Collections.Generic;

namespace Gdk
{
    /// <summary>
    /// A row/column ordered 4x4 matrix class
    /// </summary>
    public struct Matrix
    {
        // Member Fields
        public float M11, M12, M13, M14;
        public float M21, M22, M23, M24;
        public float M31, M32, M33, M34;
        public float M41, M42, M43, M44;

        /// <summary>
        /// Constructor
        /// </summary>
        public Matrix(
            float m11, float m12, float m13, float m14,
            float m21, float m22, float m23, float m24,
            float m31, float m32, float m33, float m34,
            float m41, float m42, float m43, float m44
            )
        {
            M11 = m11; M12 = m12; M13 = m13; M14 = m14;
            M21 = m21; M22 = m22; M23 = m23; M24 = m24;
            M31 = m31; M32 = m32; M33 = m33; M34 = m34;
            M41 = m41; M42 = m42; M43 = m43; M44 = m44;
        }        

        /// <summary>
        /// Constructor
        /// </summary>
        public Matrix(float[] values)
        {
            M11 = values[0]; M12 = values[1]; M13 = values[2]; M14 = values[3];
            M21 = values[4]; M22 = values[5]; M23 = values[6]; M24 = values[7];
            M31 = values[8]; M32 = values[9]; M33 = values[10]; M34 = values[11];
            M41 = values[12]; M42 = values[13]; M43 = values[14]; M44 = values[15];
        }

        /// <summary>
        /// Constructor
        /// </summary>
        public Matrix(Matrix original)
        {
            M11 = original.M11; M12 = original.M12; M13 = original.M13; M14 = original.M14;
            M21 = original.M21; M22 = original.M22; M23 = original.M23; M24 = original.M24;
            M31 = original.M31; M32 = original.M32; M33 = original.M33; M34 = original.M34;
            M41 = original.M41; M42 = original.M42; M43 = original.M43; M44 = original.M44;
        }    

        #region Properties & Accessors
        
        // Named Cell accessors
        // -------------------------------

        public float TX { get { return M14; } set { M14 = value; } }
        public float TY { get { return M24; } set { M24 = value; } }
        public float TZ { get { return M34; } set { M34 = value; } }
        
        public float SX { get { return M11; } set { M11 = value; } }
        public float SY { get { return M22; } set { M22 = value; } }
        public float SZ { get { return M33; } set { M33 = value; } }
        public float W  { get { return M44; } set { M44 = value; } }

        public float D0 { get { return M11; } set { M11 = value; } }
        public float D1 { get { return M22; } set { M22 = value; } }
        public float D2 { get { return M33; } set { M33 = value; } }
        public float D3 { get { return M44; } set { M44 = value; } }

        // Named Vector accessors
        // -------------------------------

        public Vector3 Right
        {
            get { return new Vector3(this.M11, this.M12, this.M13); }
            set { this.M11 = value.X; this.M12 = value.Y; this.M13 = value.Z; }
        }

        // **********************************************************
        public Vector3 Left
        {
            get { return new Vector3(-this.M11, -this.M12, -this.M13); }
            set { this.M11 = -value.X; this.M12 = -value.Y; this.M13 = -value.Z; }
        }

        // **********************************************************
        public Vector3 Up
        {
            get { return new Vector3(this.M21, this.M22, this.M23); }
            set { this.M21 = value.X; this.M22 = value.Y; this.M23 = value.Z; }
        }

        // **********************************************************
        public Vector3 Down
        {
            get { return new Vector3(-this.M21, -this.M22, -this.M23); }
            set { this.M21 = -value.X; this.M22 = -value.Y; this.M23 = -value.Z; }
        }

        // **********************************************************
        public Vector3 Backward
        {
            get { return new Vector3(this.M31, this.M32, this.M33); }
            set { this.M31 = value.X; this.M32 = value.Y; this.M33 = value.Z; }
        }

        // **********************************************************
        public Vector3 Forward
        {
            get { return new Vector3(-this.M31, -this.M32, -this.M33); }
            set { this.M31 = -value.X; this.M32 = -value.Y; this.M33 = -value.Z; }
        }

        // **********************************************************
        public Vector3 Translation
        {
            get { return new Vector3(this.M41, this.M42, this.M43); }
            set { this.M41 = value.X; this.M42 = value.Y; this.M43 = value.Z; }
        }

        // Static Matrices
        // -------------------------------

        /// <summary>
        /// Gets the Identity matrix
        /// </summary>
        public static Matrix Identity
        {
            get
            {
                return new Matrix(
                    1, 0, 0, 0,
                    0, 1, 0, 0,
                    0, 0, 1, 0,
                    0, 0, 0, 1
                    );
            }
        }

        /// <summary>
        /// Gets the Zero matrix
        /// </summary>
        public static Matrix Zero
        {
            get
            {
                return new Matrix(
                    0, 0, 0, 0,
                    0, 0, 0, 0,
                    0, 0, 0, 0,
                    0, 0, 0, 0
                    );
            }
        }

        #endregion

        #region Operators

        // **********************************************************
        // Unary - Negative
        public static Matrix operator -(Matrix a)
        {
            return new Matrix(
                -a.M11, -a.M12, -a.M13, -a.M14,
                -a.M21, -a.M22, -a.M23, -a.M24,
                -a.M31, -a.M32, -a.M33, -a.M34,
                -a.M41, -a.M42, -a.M43, -a.M44
                );
        }

        // **********************************************************
        // Binary - Addition
        public static Matrix operator +(Matrix a, Matrix b)
        {
            return new Matrix(
                a.M11 + b.M11, a.M12 + b.M12, a.M13 + b.M13, a.M14 + b.M14,
                a.M21 + b.M21, a.M22 + b.M22, a.M23 + b.M23, a.M24 + b.M24,
                a.M31 + b.M31, a.M32 + b.M32, a.M33 + b.M33, a.M34 + b.M34,
                a.M41 + b.M41, a.M42 + b.M42, a.M43 + b.M43, a.M44 + b.M44
                );
        }

        // **********************************************************
        // Binary - Subtraction
        public static Matrix operator -(Matrix a, Matrix b)
        {
            return new Matrix(
                a.M11 - b.M11, a.M12 - b.M12, a.M13 - b.M13, a.M14 - b.M14,
                a.M21 - b.M21, a.M22 - b.M22, a.M23 - b.M23, a.M24 - b.M24,
                a.M31 - b.M31, a.M32 - b.M32, a.M33 - b.M33, a.M34 - b.M34,
                a.M41 - b.M41, a.M42 - b.M42, a.M43 - b.M43, a.M44 - b.M44
                );
        }

        // **********************************************************
        // Binary - Multiply (Matrix * Scalar)
        public static Matrix operator *(Matrix a, float scalar)
        {
            return new Matrix(
                a.M11 * scalar, a.M12 * scalar, a.M13 * scalar, a.M14 * scalar,
                a.M21 * scalar, a.M22 * scalar, a.M23 * scalar, a.M24 * scalar,
                a.M31 * scalar, a.M32 * scalar, a.M33 * scalar, a.M34 * scalar,
                a.M41 * scalar, a.M42 * scalar, a.M43 * scalar, a.M44 * scalar
                );
        }

        // **********************************************************
        // Binary - Multiply (Matrix * Matrix)
        public static Matrix operator *(Matrix a, Matrix b)
        {
            return new Matrix(
                a.M11 * b.M11 + a.M12 * b.M21 + a.M13 * b.M31 + a.M14 * b.M41,
                a.M11 * b.M12 + a.M12 * b.M22 + a.M13 * b.M32 + a.M14 * b.M42,
                a.M11 * b.M13 + a.M12 * b.M23 + a.M13 * b.M33 + a.M14 * b.M43,
                a.M11 * b.M14 + a.M12 * b.M24 + a.M13 * b.M34 + a.M14 * b.M44,

                a.M21 * b.M11 + a.M22 * b.M21 + a.M23 * b.M31 + a.M24 * b.M41,
                a.M21 * b.M12 + a.M22 * b.M22 + a.M23 * b.M32 + a.M24 * b.M42,
                a.M21 * b.M13 + a.M22 * b.M23 + a.M23 * b.M33 + a.M24 * b.M43,
                a.M21 * b.M14 + a.M22 * b.M24 + a.M23 * b.M34 + a.M24 * b.M44,

                a.M31 * b.M11 + a.M32 * b.M21 + a.M33 * b.M31 + a.M34 * b.M41,
                a.M31 * b.M12 + a.M32 * b.M22 + a.M33 * b.M32 + a.M34 * b.M42,
                a.M31 * b.M13 + a.M32 * b.M23 + a.M33 * b.M33 + a.M34 * b.M43,
                a.M31 * b.M14 + a.M32 * b.M24 + a.M33 * b.M34 + a.M34 * b.M44,

                a.M41 * b.M11 + a.M42 * b.M21 + a.M43 * b.M31 + a.M44 * b.M41,
                a.M41 * b.M12 + a.M42 * b.M22 + a.M43 * b.M32 + a.M44 * b.M42,
                a.M41 * b.M13 + a.M42 * b.M23 + a.M43 * b.M33 + a.M44 * b.M43,
                a.M41 * b.M14 + a.M42 * b.M24 + a.M43 * b.M34 + a.M44 * b.M44
                );
        }

        // **********************************************************
        // Binary - Divide (Matrix / scalar)
        public static Matrix operator /(Matrix a, float divisor)
        {
            float scalar = 1f / divisor;
            return new Matrix(
                a.M11 * scalar, a.M12 * scalar, a.M13 * scalar, a.M14 * scalar,
                a.M21 * scalar, a.M22 * scalar, a.M23 * scalar, a.M24 * scalar,
                a.M31 * scalar, a.M32 * scalar, a.M33 * scalar, a.M34 * scalar,
                a.M41 * scalar, a.M42 * scalar, a.M43 * scalar, a.M44 * scalar
                );
        }

        // **********************************************************
        // Binary - Divide (Matrix / Matrix)
        public static Matrix operator /(Matrix a, Matrix b)
        {
            return new Matrix(
                a.M11 / b.M11, a.M12 / b.M12, a.M13 / b.M13, a.M14 / b.M14,
                a.M21 / b.M21, a.M22 / b.M22, a.M23 / b.M23, a.M24 / b.M24,
                a.M31 / b.M31, a.M32 / b.M32, a.M33 / b.M33, a.M34 / b.M34,
                a.M41 / b.M41, a.M42 / b.M42, a.M43 / b.M43, a.M44 / b.M44
                );
        }

        #endregion

        #region Instance Methods

        /// <summary>
        /// Adds a translation vector to this transformation matrix
        /// </summary>
        public void Translate(Vector3 vector)
        {
            TX += vector.X;
            TY += vector.Y;
            TZ += vector.Z;
        }

        /// <summary>
        /// Converts the Matrix to a user readable string
        /// </summary>
        public override string ToString()
        {
            return 
                "{ " + 
                "{M11:" + this.M11 + " M12:" + this.M12 + " M13:" + this.M13 + " M14:" + this.M14 + "} " +
                "{M21:" + this.M21 + " M22:" + this.M22 + " M23:" + this.M23 + " M24:" + this.M24 + "} " +
                "{M31:" + this.M31 + " M32:" + this.M32 + " M33:" + this.M33 + " M34:" + this.M34 + "} " +
                "{M41:" + this.M41 + " M42:" + this.M42 + " M43:" + this.M43 + " M44:" + this.M44 + "} " +
                "}";
        }
        
        /// <summary>
        /// Gets the determinant of this matrix
        /// </summary>
        public float GetDeterminant()
        {
	        float a0 = M11*M22 - M12*M21;
            float a1 = M11*M23 - M13*M21;
            float a2 = M11*M24 - M14*M21;
            float a3 = M12*M23 - M13*M22;
            float a4 = M12*M24 - M14*M22;
            float a5 = M13*M24 - M14*M23;
            float b0 = M31*M42 - M32*M41;
            float b1 = M31*M43 - M33*M41;
            float b2 = M31*M44 - M34*M41;
            float b3 = M32*M43 - M33*M42;
            float b4 = M32*M44 - M34*M42;
            float b5 = M33*M44 - M34*M43;

            return ( a0*b5 - a1*b4 + a2*b3 + a3*b2 - a4*b1 + a5*b0 );
        }

        /// <summary>
        /// Gets the transpose of this matrix
        /// </summary>
        Matrix GetTranspose()
        {
	        return new Matrix(
		        M11, M21, M31, M41, 
		        M12, M22, M32, M42, 
		        M13, M23, M33, M43, 
		        M14, M24, M34, M44
		        );
        }

        /// <summary>
        /// Gets the inverse of this matrix
        /// </summary>
        /// <returns></returns>
        Matrix GetInverse()
        {
            // Get the determinant
	        float a0 = M11*M22 - M12*M21;
            float a1 = M11*M23 - M13*M21;
            float a2 = M11*M24 - M14*M21;
            float a3 = M12*M23 - M13*M22;
            float a4 = M12*M24 - M14*M22;
            float a5 = M13*M24 - M14*M23;
            float b0 = M31*M42 - M32*M41;
            float b1 = M31*M43 - M33*M41;
            float b2 = M31*M44 - M34*M41;
            float b3 = M32*M43 - M33*M42;
            float b4 = M32*M44 - M34*M42;
            float b5 = M33*M44 - M34*M43;
            float det = a0*b5-a1*b4+a2*b3+a3*b2-a4*b1+a5*b0;

            // Verify the matrix is invertable
	        if (Math.Abs(det) <= float.Epsilon)
            {
                return Matrix.Zero;
            }

            // Generate the inverse
            Matrix result;
            result.M11 = + M22*b5 - M23*b4 + M24*b3;
            result.M21 = - M21*b5 + M23*b2 - M24*b1;
            result.M31 = + M21*b4 - M22*b2 + M24*b0;
            result.M41 = - M21*b3 + M22*b1 - M23*b0;
            result.M12 = - M12*b5 + M13*b4 - M14*b3;
            result.M22 = + M11*b5 - M13*b2 + M14*b1;
            result.M32 = - M11*b4 + M12*b2 - M14*b0;
            result.M42 = + M11*b3 - M12*b1 + M13*b0;
            result.M13 = + M42*a5 - M43*a4 + M44*a3;
            result.M23 = - M41*a5 + M43*a2 - M44*a1;
            result.M33 = + M41*a4 - M42*a2 + M44*a0;
            result.M43 = - M41*a3 + M42*a1 - M43*a0;
            result.M14 = - M32*a5 + M33*a4 - M34*a3;
            result.M24 = + M31*a5 - M33*a2 + M34*a1;
            result.M34 = - M31*a4 + M32*a2 - M34*a0;
            result.M44 = + M31*a3 - M32*a1 + M33*a0;

            float inverseDet = 1.0f / det;
            result *= inverseDet;
	        return result;
        }

        /// <summary>
        /// Decomposes a matrix into its Scale, Rotation, and Translational components.
        /// This only works if the matrix was originally created from a S*R*T matrix combination.
        /// </summary>
        public void Decompose(out Vector3 scale, out Quaternion rotation, out Vector3 translation)
        {
            // Make a copy of this matrix, to work with
            Matrix copy = new Matrix(this);

            // Extract the translation
            translation = new Vector3(copy.M41, copy.M42, copy.M43);

            // Remove the translational component from the matrix
            copy.M41 = 0.0f;
            copy.M42 = 0.0f;
            copy.M43 = 0.0f;

            // Extract the rotation.  (Using polar decomposition)
            float variance;
            int count = 0;
            Matrix rotationMatrix = new Matrix(copy);
            do 
            {
                Matrix nextRotationMatrix;
                Matrix rotationInverseTranspose = rotationMatrix.GetTranspose().GetInverse();
                
                // Get the next matrix for sampling
                nextRotationMatrix = (rotationMatrix + rotationInverseTranspose) * 0.5f;
                
                // Find the variances in the 3 columns
                float columnAVariance = 
                    Math.Abs(rotationMatrix.M11 - nextRotationMatrix.M11) + 
                    Math.Abs(rotationMatrix.M21 - nextRotationMatrix.M21) +
                    Math.Abs(rotationMatrix.M31 - nextRotationMatrix.M31);
                float columnBVariance = 
                    Math.Abs(rotationMatrix.M12 - nextRotationMatrix.M12) + 
                    Math.Abs(rotationMatrix.M22 - nextRotationMatrix.M22) +
                    Math.Abs(rotationMatrix.M32 - nextRotationMatrix.M32);
                float columnCVariance = 
                    Math.Abs(rotationMatrix.M13 - nextRotationMatrix.M13) + 
                    Math.Abs(rotationMatrix.M23 - nextRotationMatrix.M23) +
                    Math.Abs(rotationMatrix.M33 - nextRotationMatrix.M33);

                // find the largest of the variances
                variance = Math.Max(Math.Max(columnAVariance, columnBVariance), columnCVariance);

                // Move to the next rotation
                rotationMatrix = nextRotationMatrix;

                count++;

                // Loop until we run out of iterations or reach our approximate destination
            } while (count < 100 && variance > float.Epsilon);

            // Extract the scale from the matrix
            Matrix scaleMatrix = rotationMatrix.GetInverse() * copy;
            scale = new Vector3(scaleMatrix.M11, scaleMatrix.M22, scaleMatrix.M33);

            // Get the normalized orthagonal matrix
            Vector3 columnA = new Vector3(copy.M11, copy.M21, copy.M31);
            Vector3 columnB = new Vector3(copy.M12, copy.M22, copy.M32);
            Vector3 columnC = new Vector3(copy.M13, copy.M23, copy.M33);
            Matrix normalizedOrthagonalMatrix = new Matrix(
                columnA.X, columnB.X, columnC.X, 0,
                columnA.Y, columnB.Y, columnC.Y, 0,
                columnA.Z, columnB.Z, columnC.Z, 0,
                0, 0, 0, 1
                );

            // The determinate of the normalized matrix will tell us if there is a negative value in the scaling.
            //   Since 2 or 3 negative axis scales will be encoded as rotations, we simply have to handle the case of 1 negative axis
            if (normalizedOrthagonalMatrix.GetDeterminant() < 0.0f)
                scale.X = -scale.X;

            // Convert the rotation to a quaternion
            rotation = Quaternion.FromRotationMatrix(rotationMatrix);

        }
 

        #endregion

        #region Static Methods

        /// <summary>
        /// Creates a rotation matrix around the X axis by the given kber of radians
        /// </summary>
        public static Matrix CreateRotationX(float radians)
        {
            float cos = (float)Math.Cos(radians);
            float sin = (float)Math.Sin(radians);

            return new Matrix(
                1, 0, 0, 0,
                0, cos, sin, 0,
                0, -sin, cos, 0,
                0, 0, 0, 1
                );
        }

        /// <summary>
        /// Creates a rotation matrix around the Y axis by the given kber of radians
        /// </summary>
        public static Matrix CreateRotationY(float radians)
        {
            float cos = (float)Math.Cos(radians);
            float sin = (float)Math.Sin(radians);

            return new Matrix(
                cos, 0, -sin, 0,
                0, 1, 0, 0,
                sin, 0, cos, 0,
                0, 0, 0, 1
                );
        }

        /// <summary>
        /// Creates a rotation matrix around the Z axis by the given kber of radians
        /// </summary>
        public static Matrix CreateRotationZ(float radians)
        {
            float cos = (float)Math.Cos(radians);
            float sin = (float)Math.Sin(radians);

            return new Matrix(
                cos, sin, 0, 0,
                -sin, cos, 0, 0,
                0, 0, 1, 0,
                0, 0, 0, 1
                );
        }

        /// <summary>
        /// Creates a rotation matrix around the given axis by the given amount of radians
        /// </summary>
        public static Matrix CreateRotationAroundAxis(Vector3 axis, float radians)
        {
            float cos = (float)Math.Cos(radians);
            float sin = (float)Math.Sin(radians);
            float xx = axis.X * axis.X;
            float yy = axis.Y * axis.Y;
            float zz = axis.Z * axis.Z;
            float xy = axis.X * axis.Y;
            float xz = axis.X * axis.Z;
            float yz = axis.Y * axis.Z;

            return new Matrix(
                xx + (cos * (1f - xx)),
                (xy - (cos * xy)) + (sin * axis.Z),
                (xz - (cos * xz)) - (sin * axis.Y),
                0f,
                (xy - (cos * xy)) - (sin * axis.Z),
                yy + (cos * (1f - yy)),
                (yz - (cos * yz)) + (sin * axis.X),
                0f,
                (xz - (cos * xz)) + (sin * axis.Y),
                (yz - (cos * yz)) - (sin * axis.X),
                zz + (cos * (1f - zz)),
                0f,
                0f, 0f, 0f, 1f
                );
        }

        /// <summary>
        /// Creates a rotation matrix that represents a quaternion orientation
        /// </summary>
        Matrix CreateRotationFromQuaternion(Quaternion quat)
        {
	        // Compute a bunch of intermediate float vales
	        float x2 = 2.0f * quat.X;
	        float y2 = 2.0f * quat.Y;
	        float z2 = 2.0f * quat.Z;
	        float wx2 = x2 * quat.W;
	        float wy2 = y2 * quat.W;
	        float wz2 = z2 * quat.W;
	        float xx2 = x2 * quat.X;
	        float xy2 = y2 * quat.X;
	        float xz2 = z2 * quat.X;
	        float yy2 = y2 * quat.Y;
	        float yz2 = z2 * quat.Y;
	        float zz2 = z2 * quat.Z;
            
	        // Multiply in the given matrix
	        return new Matrix(
                1.0f - (yy2 + zz2),		xy2 - wz2,				xz2 + wy2,				0.0f,
                xy2 + wz2,				1.0f - (xx2 + zz2),		yz2 - wx2,				0.0f,
                xz2 - wy2,				yz2 + wx2,				1.0f - (xx2 + yy2),		0.0f,
                0.0f,					0.0f,					0.0f,					1.0f
                );
        }

        /// <summary>
        /// Creates a scaling matrix by the given scalar
        /// </summary>
        public static Matrix CreateScale(float scale)
        {
            return new Matrix(
                scale, 0, 0, 0,
                0, scale, 0, 0,
                0, 0, scale, 0,
                0, 0, 0, 1
                );
        }

        /// <summary>
        /// Creates a scaling matrix by the given scalar factors
        /// </summary>
        public static Matrix CreateScale(Vector3 scale)
        {
            return new Matrix(
                scale.X, 0, 0, 0,
                0, scale.Y, 0, 0,
                0, 0, scale.Z, 0,
                0, 0, 0, 1
                );
        }

        /// <summary>
        /// Creates a translation matrix by the translation factors
        /// </summary>
        public static Matrix CreateTranslation(Vector3 translation)
        {
            return new Matrix(
                1, 0, 0, 0,
                0, 1, 0, 0,
                0, 0, 1, 0,
                translation.X, translation.Y, translation.Z, 1
                );
        }

        /// <summary>
        /// Creates a translation matrix by the translation factors
        /// </summary>
        public static Matrix CreateTranslation(float x, float y, float z)
        {
            return new Matrix(
                1, 0, 0, 0,
                0, 1, 0, 0,
                0, 0, 1, 0,
                x, y, z, 1
                );
        }

        /// <summary>
        /// Creates a Look At matrix
        /// </summary>
        public static Matrix CreateLookAt(Vector3 position, Vector3 target, Vector3 cameraUp)
        {
            // Determine the Direction, Right, & real Up vectors
            Vector3 direction = (target - position); 
            direction.Normalize();
            Vector3 right = Vector3.Cross(cameraUp, direction); 
            right.Normalize();
            Vector3 up = Vector3.Cross(direction, right);

            return new Matrix(
                right.X, up.X, direction.X, 0,
                right.Y, up.Y, direction.Y, 0,
                right.Z, up.Z, direction.Z, 0,
                -Vector3.Dot(right, position),
                -Vector3.Dot(up, position),
                -Vector3.Dot(direction, position), 
                1f
                );
        }

        /// <summary>
        /// Creates an Orthographic projection matrix
        /// </summary>
        public static Matrix CreateOrthographic(float width, float height, float nearPlane, float farPlane)
        {
            return new Matrix(
                2f / width, 0, 0, 0,
                0, 2f / height, 0, 0,
                0, 0, 1f / (nearPlane - farPlane), 0,
                0, 0, nearPlane / (nearPlane - farPlane), 1
                );
        }

        /// <summary>
        /// Creates a Perspective projection matrix
        /// </summary>
        public static Matrix CreatePerspective(float width, float height, float nearPlane, float farPlane)
        {
            return new Matrix(
                (2f * nearPlane) / width, 0, 0, 0,
                0, (2f * nearPlane) / height, 0, 0,
                0, 0, farPlane / (nearPlane - farPlane), -1,
                0, 0, (nearPlane * farPlane) / (nearPlane - farPlane), 0f
                );
        }

        /// <summary>
        /// Creates a Perspective projection matrix
        /// </summary>
        public static Matrix CreatePerspectiveFOV(float fov, float aspectRatio, float nearPlane, float farPlane)
        {
            float tan = 1f / ((float)Math.Tan(fov * 0.5));

            return new Matrix(
                tan / aspectRatio, 0, 0, 0,
                0, tan, 0, 0,
                0, 0, farPlane / (nearPlane - farPlane), -1,
                0, 0, (nearPlane * farPlane) / (nearPlane - farPlane), 0f
                );
        }

        /// <summary>
        /// Inverts the given matrix
        /// </summary>
        public static Matrix Invert(Matrix matrix)
        {
            float a = (matrix.M33 * matrix.M44) - (matrix.M34 * matrix.M43);
            float b = (matrix.M32 * matrix.M44) - (matrix.M34 * matrix.M42);
            float c = (matrix.M32 * matrix.M43) - (matrix.M33 * matrix.M42);
            float d = (matrix.M31 * matrix.M44) - (matrix.M34 * matrix.M41);
            float e = (matrix.M31 * matrix.M43) - (matrix.M33 * matrix.M41);
            float f = (matrix.M31 * matrix.M42) - (matrix.M32 * matrix.M41);
            float g = ((matrix.M22 * a) - (matrix.M23 * b)) + (matrix.M24 * c);
            float h = -(((matrix.M21 * a) - (matrix.M23 * d)) + (matrix.M24 * e));
            float i = ((matrix.M21 * b) - (matrix.M22 * d)) + (matrix.M24 * f);
            float j = -(((matrix.M21 * c) - (matrix.M22 * e)) + (matrix.M23 * f));
            float k = 1f / ((((matrix.M11 * g) + (matrix.M12 * h)) + (matrix.M13 * i)) + (matrix.M14 * j));
            float l = (matrix.M23 * matrix.M44) - (matrix.M24 * matrix.M43);
            float m = (matrix.M22 * matrix.M44) - (matrix.M24 * matrix.M42);
            float n = (matrix.M22 * matrix.M43) - (matrix.M23 * matrix.M42);
            float o = (matrix.M21 * matrix.M44) - (matrix.M24 * matrix.M41);
            float p = (matrix.M21 * matrix.M43) - (matrix.M23 * matrix.M41);
            float q = (matrix.M21 * matrix.M42) - (matrix.M22 * matrix.M41);
            float r = (matrix.M23 * matrix.M34) - (matrix.M24 * matrix.M33);
            float s = (matrix.M22 * matrix.M34) - (matrix.M24 * matrix.M32);
            float t = (matrix.M22 * matrix.M33) - (matrix.M23 * matrix.M32);
            float u = (matrix.M21 * matrix.M34) - (matrix.M24 * matrix.M31);
            float v = (matrix.M21 * matrix.M33) - (matrix.M23 * matrix.M31);
            float w = (matrix.M21 * matrix.M32) - (matrix.M22 * matrix.M31);

            Matrix result;
            result.M11 = g * k;
            result.M21 = h * k;
            result.M31 = i * k;
            result.M41 = j * k;
            result.M12 = -(((matrix.M12 * a) - (matrix.M13 * b)) + (matrix.M14 * c)) * k;
            result.M22 = (((matrix.M11 * a) - (matrix.M13 * d)) + (matrix.M14 * e)) * k;
            result.M32 = -(((matrix.M11 * b) - (matrix.M12 * d)) + (matrix.M14 * f)) * k;
            result.M42 = (((matrix.M11 * c) - (matrix.M12 * e)) + (matrix.M13 * f)) * k;
            result.M13 = (((matrix.M12 * l) - (matrix.M13 * m)) + (matrix.M14 * n)) * k;
            result.M23 = -(((matrix.M11 * l) - (matrix.M13 * o)) + (matrix.M14 * p)) * k;
            result.M33 = (((matrix.M11 * m) - (matrix.M12 * o)) + (matrix.M14 * q)) * k;
            result.M43 = -(((matrix.M11 * n) - (matrix.M12 * p)) + (matrix.M13 * q)) * k;
            result.M14 = -(((matrix.M12 * r) - (matrix.M13 * s)) + (matrix.M14 * t)) * k;
            result.M24 = (((matrix.M11 * r) - (matrix.M13 * u)) + (matrix.M14 * v)) * k;
            result.M34 = -(((matrix.M11 * s) - (matrix.M12 * u)) + (matrix.M14 * w)) * k;
            result.M44 = (((matrix.M11 * t) - (matrix.M12 * v)) + (matrix.M13 * w)) * k;
            return result;
        }

        /// <summary>
        /// Creates a transpose of the given matrix
        /// </summary>
        public static Matrix Transpose(Matrix matrix)
        {
            return new Matrix(
                matrix.M11, matrix.M21, matrix.M31, matrix.M41,
                matrix.M12, matrix.M22, matrix.M32, matrix.M42,
                matrix.M13, matrix.M23, matrix.M33, matrix.M43,
                matrix.M14, matrix.M24, matrix.M34, matrix.M44
                );
        }

        /// <summary>
        /// Creates a linear interpolated transformation matrix between matrix A and matrix B.
        /// Formula:  ta + (1-t)b
        /// </summary>
        public static Matrix Lerp(Matrix a, Matrix b, float t)
        {
            return new Matrix(
                a.M11 + (b.M11 - a.M11) * t,
                a.M12 + (b.M12 - a.M12) * t,
                a.M13 + (b.M13 - a.M13) * t,
                a.M14 + (b.M14 - a.M14) * t,
                a.M21 + (b.M21 - a.M21) * t,
                a.M22 + (b.M22 - a.M22) * t,
                a.M23 + (b.M23 - a.M23) * t,
                a.M24 + (b.M24 - a.M24) * t,
                a.M31 + (b.M31 - a.M31) * t,
                a.M32 + (b.M32 - a.M32) * t,
                a.M33 + (b.M33 - a.M33) * t,
                a.M34 + (b.M34 - a.M34) * t,
                a.M41 + (b.M41 - a.M41) * t,
                a.M42 + (b.M42 - a.M42) * t,
                a.M43 + (b.M43 - a.M43) * t,
                a.M44 + (b.M44 - a.M44) * t
                );
        }

        #endregion

    }
}