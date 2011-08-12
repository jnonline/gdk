/*
  Copyright (c) 2011, Raincity Games LLC
  Licensed under the MIT license: http://www.opensource.org/licenses/mit_license.php
*/

using System;
using System.Collections.Generic;
using System.ComponentModel;

namespace Gdk
{
    [TypeConverter(typeof(Vector3TypeConverter))]
    public struct Vector3
    {
        // Member Fields
        public float X;
        public float Y;
        public float Z;

        /// <summary>
        /// Constructor
        /// </summary>
        public Vector3(float x, float y, float z)
        {
            this.X = x;
            this.Y = y;
            this.Z = z;
        }

        #region Operators

        // **********************************************************
        public static Vector3 operator -(Vector3 a)
        {
            return new Vector3(-a.X, -a.Y, -a.Z);
        }

        // **********************************************************
        public static Vector3 operator +(Vector3 a, Vector3 b)
        {
            return new Vector3(a.X + b.X, a.Y + b.Y, a.Z + b.Z);
        }

        // **********************************************************
        public static Vector3 operator -(Vector3 a, Vector3 b)
        {
            return new Vector3(a.X - b.X, a.Y - b.Y, a.Z - b.Z);
        }

        // **********************************************************
        public static Vector3 operator *(Vector3 a, float scalar)
        {
            return new Vector3(a.X * scalar, a.Y * scalar, a.Z * scalar);
        }

        // **********************************************************
        public static Vector3 operator *(float scalar, Vector3 a)
        {
            return new Vector3(a.X * scalar, a.Y * scalar, a.Z * scalar);
        }

        // **********************************************************
        public static Vector3 operator *(Vector3 a, Vector3 b)
        {
            return new Vector3(a.X * b.X, a.Y * b.Y, a.Z * b.Z);
        }

        #endregion

        #region Instance Methods

        /// <summary>
        /// Equals()
        /// </summary>
        public bool Equals(Vector3 v)
        {
            return v.X == this.X 
                && v.Y == this.Y 
                && v.Z == this.Z;
        } 

        /// <summary>
        /// Returns the length of the vector
        /// </summary>
        /// <returns></returns>
        public float Length()
        {
            return (float)Math.Sqrt(this.X * this.X + this.Y * this.Y + this.Z * this.Z);
        }

        /// <summary>
        /// Returns the length squared of the vector
        /// </summary>
        /// <returns></returns>
        public float LengthSquared()
        {
            return this.X * this.X + this.Y * this.Y + this.Z * this.Z;
        }

        /// <summary>
        /// Normalizes the vector in place
        /// </summary>
        /// <returns></returns>
        public void Normalize()
        {
            float inverseLength = 1.0f / this.Length();
            this.X *= inverseLength;
            this.Y *= inverseLength;
            this.Z *= inverseLength;
        }

        /// <summary>
        /// Returns the dot product of this vector A and vector B: (A * B)
        /// </summary>
        /// <returns></returns>
        public float Dot(Vector3 b)
        {
            return this.X * b.X + this.Y * b.Y + this.Z * b.Z;
        }

        /// <summary>
        /// Returns the cross product of this vector A and vector B: (A x B)
        /// </summary>
        /// <returns></returns>
        public Vector3 Cross(Vector3 b)
        {
            return new Vector3(
                this.Y * b.Z - this.Z * b.Y,
                this.Z * b.X - this.X * b.Z,
                this.X * b.Y - this.Y * b.X
                );
        }

        /// <summary>
        /// Converts the vector data to a user readable string
        /// </summary>
        public override string ToString()
        {
            return string.Format("{0}, {1}, {2}", this.X, this.Y, this.Z);
        }

        #endregion

        #region Static Methods

        /// <summary>
        /// Normalizes the vector in place
        /// </summary>
        /// <returns></returns>
        public static Vector3 Normalize(Vector3 a)
        {
            a.Normalize();
            return a;
        }

        /// <summary>
        /// Returns the dot product of vector A and vector B: (A * B)
        /// </summary>
        /// <returns></returns>
        public static float Dot(Vector3 a, Vector3 b)
        {
            return a.X * b.X + a.Y * b.Y + a.Z * b.Z;
        }

        /// <summary>
        /// Returns the cross product of this vector A and vector B: (A x B)
        /// </summary>
        /// <returns></returns>
        public static Vector3 Cross(Vector3 a, Vector3 b)
        {
            return new Vector3(
                a.Y * b.Z - a.Z * b.Y,
                a.Z * b.X - a.X * b.Z,
                a.X * b.Y - a.Y * b.X
                );
        }

        /// <summary>
        /// Transforms a vertex using this matrix
        /// </summary>
        public static Vector3 Transform(Vector3 vertex, Matrix matrix)
        {
            Vector3 result;
            result.X = (vertex.X * matrix.M11) + (vertex.Y * matrix.M21) + (vertex.Z * matrix.M31) + matrix.M41;
            result.Y = (vertex.X * matrix.M12) + (vertex.Y * matrix.M22) + (vertex.Z * matrix.M32) + matrix.M42;
            result.Z = (vertex.X * matrix.M13) + (vertex.Y * matrix.M23) + (vertex.Z * matrix.M33) + matrix.M43;
            return result;
        }

        /// <summary>
        /// Transforms a normal using this matrix (does not apply translation)
        /// </summary>
        public static Vector3 TransformNormal(Vector3 normal, Matrix matrix)
        {
            Vector3 result;
            result.X = (normal.X * matrix.M11) + (normal.Y * matrix.M21) + (normal.Z * matrix.M31);
            result.Y = (normal.X * matrix.M12) + (normal.Y * matrix.M22) + (normal.Z * matrix.M32);
            result.Z = (normal.X * matrix.M13) + (normal.Y * matrix.M23) + (normal.Z * matrix.M33);
            return result;
        }

        /// <summary>
        /// Calculates the angle between two vectors
        /// </summary>
        public static float AngleBetween(Vector3 a, Vector3 b)
        {
            return (float)Math.Acos(a.Dot(b));
        }

        ///// <summary>
        ///// Un-projects the given coordinate from screen space to world space
        ///// </summary>
        //public static Vector3 UnProject(Vector3 vector,
        //    float viewportX, float viewportY, float viewportWidth, float viewportHeight,
        //    ref Matrix world, ref Matrix view, ref Matrix projection)
        //{
        //    Matrix worldViewProj = world * view * projection;
        //    Matrix inverseWVP = Matrix.Invert(worldViewProj);
        //    Vector3 temp;
        //    temp.X = 2.0f * (vector.X - viewportX) / viewportWidth - 1.0f;
        //    temp.Y = (1.0f - 2.0f * (vector.Y - viewportY) / viewportHeight) / proj;
        //    temp.Z = vector.Z;

        //    return Vector3.Transform(temp, inverseWVP);
        //}

        /// <summary>
        /// Parses a Vector3 from a string of the format "X,Y,Z"  (or optionally "X")
        /// </summary>
        /// <param name="input">the input string</param>
        /// <param name="allowSingleValue">If the input only has 1 float, should we allow it & treat it as both the X, Y, & Z values</param>
        /// <returns></returns>
        static public Vector3 Parse(string input, bool allowSingleValue)
        {
            // Split up the string by commas
            string[] parts = input.Split(',');
            int numValues = parts.GetUpperBound(0) + 1;

            // Is this a single value?
            if (numValues == 1 && allowSingleValue)
            {
                float x = float.Parse(parts[0]);
                return new Vector3(x, x, x);
            }
            else if (numValues == 3)
                return new Vector3(float.Parse(parts[0]), float.Parse(parts[1]), float.Parse(parts[2]));

            // Invalid number of vales
            throw new ArgumentException("Input string has an invalid format: " + input + " (Expected: \"X,Y,Z\")", "input");
        }

        // Pre-defined vectors
        public static Vector3 Zero { get { return new Vector3(0, 0, 0); }}
        public static Vector3 One { get { return new Vector3(1, 1, 1); } }
        public static Vector3 Up { get { return new Vector3(0, 1, 0); } }
        public static Vector3 Down { get { return new Vector3(0, -1, 0); } }
        public static Vector3 Forward { get { return new Vector3(0, 0, 1); } }
        public static Vector3 Backward { get { return new Vector3(0, 0, -1); } }
        public static Vector3 Left { get { return new Vector3(-1, 0, 0); } }
        public static Vector3 Right { get { return new Vector3(1, 0, 0); } }

        #endregion
    }

    /// <summary>
    /// TypeConverter for the Vector3 class
    /// </summary>
    public class Vector3TypeConverter : TypeConverter
    {
        public override bool CanConvertTo(ITypeDescriptorContext context, Type destinationType)
        {
            if (destinationType == typeof(string))
                return true;

            return base.CanConvertTo(context, destinationType);
        }

        public override bool CanConvertFrom(ITypeDescriptorContext context, Type sourceType)
        {
            if (sourceType == typeof(string))
                return true;

            return base.CanConvertFrom(context, sourceType);
        }

        public override object ConvertTo(ITypeDescriptorContext context, System.Globalization.CultureInfo culture, object value, Type destinationType)
        {
            if (destinationType == typeof(string))
                return value.ToString();

            return base.ConvertTo(context, culture, value, destinationType);
        }

        public override object ConvertFrom(ITypeDescriptorContext context, System.Globalization.CultureInfo culture, object value)
        {
            if (value is string)
                return Vector3.Parse(value as string, false);

            return base.ConvertFrom(context, culture, value);
        }
    }
}