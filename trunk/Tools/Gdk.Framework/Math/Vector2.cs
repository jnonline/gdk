/*
  Copyright (c) 2011, Raincity Games LLC
  Licensed under the MIT license: http://www.opensource.org/licenses/mit_license.php
*/

using System;
using System.Collections.Generic;
using System.ComponentModel;

namespace Gdk.Framework
{
    [TypeConverter(typeof(Vector2TypeConverter))]
    public struct Vector2
    {
        // Member Fields
        public float X;
        public float Y;

        /// <summary>
        /// Constructor
        /// </summary>
        public Vector2(float x, float y)
        {
            this.X = x;
            this.Y = y;
        }

        #region Operators

        // **********************************************************
        public static Vector2 operator -(Vector2 a)
        {
            return new Vector2(-a.X, -a.Y);
        }

        // **********************************************************
        public static Vector2 operator +(Vector2 a, Vector2 b)
        {
            return new Vector2(a.X + b.X, a.Y + b.Y);
        }

        // **********************************************************
        public static Vector2 operator -(Vector2 a, Vector2 b)
        {
            return new Vector2(a.X - b.X, a.Y - b.Y);
        }

        // **********************************************************
        public static Vector2 operator *(Vector2 a, float scalar)
        {
            return new Vector2(a.X * scalar, a.Y * scalar);
        }

        // **********************************************************
        public static Vector2 operator *(float scalar, Vector2 a)
        {
            return new Vector2(a.X * scalar, a.Y * scalar);
        }

        // **********************************************************
        public static Vector2 operator *(Vector2 a, Vector2 b)
        {
            return new Vector2(a.X * b.X, a.Y * b.Y);
        }

        #endregion

        #region Instance Methods

        /// <summary>
        /// Equals()
        /// </summary>
        public bool Equals(Vector2 v)
        {
            return v.X == this.X
                && v.Y == this.Y;
        } 

        /// <summary>
        /// Returns the length of the vector
        /// </summary>
        /// <returns></returns>
        public float Length()
        {
            return (float)Math.Sqrt(this.X * this.X + this.Y * this.Y);
        }

        /// <summary>
        /// Returns the length squared of the vector
        /// </summary>
        /// <returns></returns>
        public float LengthSquared()
        {
            return this.X * this.X + this.Y * this.Y;
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
        }

        /// <summary>
        /// Returns the dot product of this vector A and vector B: (A * B)
        /// </summary>
        /// <returns></returns>
        public float Dot(Vector2 b)
        {
            return this.X * b.X + this.Y * b.Y;
        }

        /// <summary>
        /// Returns the dot product of this vector A and vector B: (A * B)
        /// </summary>
        /// <returns></returns>
        public override string  ToString()
        {
            return string.Format("{0},{1}", X, Y);
        }

        #endregion

        #region Static Methods

        /// <summary>
        /// Normalizes the vector in place
        /// </summary>
        /// <returns></returns>
        public static Vector2 Normalize(Vector2 a)
        {
            a.Normalize();
            return a;
        }

        /// <summary>
        /// Returns the dot product of vector A and vector B: (A * B)
        /// </summary>
        /// <returns></returns>
        public static float Dot(Vector2 a, Vector2 b)
        {
            return a.X * b.X + a.Y * b.Y;
        }

        /// <summary>
        /// Parses a Vector2 from a string of the format "X,Y"  (or optionally "X")
        /// </summary>
        /// <param name="input">the input string</param>
        /// <param name="allowSingleValue">If the input only has 1 float, should we allow it & treat it as both the X & Y values</param>
        /// <returns></returns>
        static public Vector2 Parse(string input, bool allowSingleValue)
        {
			// Parse the input into a list of floats
			List<float> floats = Gdk.Framework.StringUtilities.ParseFloats(input);
			
            // Is this a single value?
            if (floats.Count == 1 && allowSingleValue)
                return new Vector2(floats[0], floats[0]);
            else if (floats.Count == 2)
                return new Vector2(floats[0], floats[1]);

            // Invalid number of vales
            throw new ArgumentException("Input string has an invalid format: " + input + " (Expected: \"X,Y\")", "input");
        }
		
		/// <summary>
        /// Parses a Vector2 from a string of the format "X,Y"  (or optionally "X")
        /// </summary>
        /// <param name="input">the input string</param>
        /// <param name="value">the output value</param>
        /// <param name="allowSingleValue">If the input only has 1 float, should we allow it & treat it as both the X & Y values</param>
        /// <returns></returns>
		static public bool TryParse(string input, out Vector2 value, bool allowSingleValue)
		{
			try
			{
				value = Parse(input, allowSingleValue);
			}
			catch(Exception)
			{
				value = Vector2.Zero;
				return false;
			}
			return true;
		}

        // Pre-defined vectors
        public static Vector2 Zero { get { return new Vector2(0, 0); } }
        public static Vector2 One { get { return new Vector2(1, 1); } }
        public static Vector2 Up { get { return new Vector2(0, 1); } }
        public static Vector2 Down { get { return new Vector2(0, -1); } }
        public static Vector2 Left { get { return new Vector2(-1, 0); } }
        public static Vector2 Right { get { return new Vector2(1, 0); } }

        #endregion
    }

    /// <summary>
    /// TypeConverter for the Vector2 class
    /// </summary>
    public class Vector2TypeConverter : TypeConverter
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
                return Vector2.Parse(value as string, false);

            return base.ConvertFrom(context, culture, value);
        }
    }
}