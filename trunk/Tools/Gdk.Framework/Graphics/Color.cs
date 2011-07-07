using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Reflection;

namespace Gdk
{
    [TypeConverter(typeof(ColorTypeConverter))]
    public struct Color
    {
        // Member Fields
        public byte R;
        public byte G;
        public byte B;
        public byte A;

        /// <summary>
        /// Constructor
        /// </summary>
        public Color(byte r, byte g, byte b)
        {
            this.R = r;
            this.G = g;
            this.B = b;
            this.A = 255;
        }

        /// <summary>
        /// Constructor
        /// </summary>
        public Color(byte r, byte g, byte b, byte a)
        {
            this.R = r;
            this.G = g;
            this.B = b;
            this.A = a;
        }

        #region Operators

        // **********************************************************
        public static bool operator ==(Color a, Color b)
        {
            return a.R == b.R && 
                   a.G == b.G && 
                   a.B == b.B && 
                   a.A == b.A;
        }

        // **********************************************************
        public static bool operator !=(Color a, Color b)
        {
            return !(a == b);
        }

        // **********************************************************
        public static Color operator +(Color c1, Color c2)
        {
            byte r = (byte)Math.Min((int)c1.R + (int)c2.R, 255);
            byte g = (byte)Math.Min((int)c1.G + (int)c2.G, 255);
            byte b = (byte)Math.Min((int)c1.B + (int)c2.B, 255);
            byte a = (byte)Math.Min((int)c1.A + (int)c2.A, 255);
            return new Color(r, g, b, a);
        }

        // **********************************************************
        public static Color operator -(Color c1, Color c2)
        {
            byte r = (byte)Math.Max((int)c1.R - (int)c2.R, 0);
            byte g = (byte)Math.Max((int)c1.G - (int)c2.G, 0);
            byte b = (byte)Math.Max((int)c1.B - (int)c2.B, 0);
            byte a = (byte)Math.Max((int)c1.A - (int)c2.A, 0);
            return new Color(r, g, b, a);
        }

        // **********************************************************
        public static Color operator *(Color a, float scalar)
        {
            // Clamp the scalar
            if (scalar < 0f) scalar = 0f;
            else if (scalar > 1.0f) scalar = 1f;

            // Scale the color
            return new Color((byte)(a.R * scalar), (byte)(a.G * scalar), (byte)(a.B * scalar), (byte)(a.A * scalar));
        }

        // **********************************************************
        public static Color operator *(float scalar, Color a)
        {
            // Clamp the scalar
            if (scalar < 0f) scalar = 0f;
            else if (scalar > 1.0f) scalar = 1f;

            // Scale the color
            return new Color((byte)(a.R * scalar), (byte)(a.G * scalar), (byte)(a.B * scalar), (byte)(a.A * scalar));
        }


        #endregion

        #region Methods

        /// <summary>
        /// GetHashCode()
        /// </summary>
        public override int GetHashCode()
        {
            return (Int32)((this.R << 24) + (this.G << 16) + (this.B << 8) + this.A);
        }

        /// <summary>
        /// Equals(object)
        /// </summary>
        public override bool Equals(object obj)
        {
            if (obj == null)
                return false;
            Color c = (Color)obj;
            if (c == null)
                return false;

            return this.Equals(c);
        }

        /// <summary>
        /// Equals(Color)
        /// </summary>
        public bool Equals(Color c)
        {
            return this.R == c.R
                && this.G == c.G
                && this.B == c.B
                && this.A == c.A;
        } 

        /// <summary>
        /// Parses a Color value from an input string.
        /// </summary>
        /// <param name="input">Color values in the format (R, G, B [, A]) (0-255)  </param>
        /// <returns></returns>
        static public Color Parse(string input)
        {
            // Split up the string by commas
            string[] parts = input.Split(',');
            if (parts.GetUpperBound(0) == 2)
                return new Color(byte.Parse(parts[0].Trim()), byte.Parse(parts[1].Trim()), byte.Parse(parts[2].Trim()));
            else if (parts.GetUpperBound(0) == 3)
                return new Color(byte.Parse(parts[0].Trim()), byte.Parse(parts[1].Trim()), byte.Parse(parts[2].Trim()), byte.Parse(parts[3].Trim()));

            throw new ArgumentException("Input string is not a valid color value: " + input, "input");
        }

        /// <summary>
        /// Tries to parse the given string into a Color.  Returns false if the color cannot be parsed
        /// </summary>
        static public bool TryParse(string input, out Color color)
        {
            try
            {
                color = Parse(input);
            }
            catch (Exception)
            {
                color = Color.TransparentBlack;
                return false;
            }
            return true;
        }

        /// <summary>
        /// Returns a string value that represents this color
        /// </summary>
        public override string ToString()
        {
            return string.Format("{0},{1},{2},{3}", R, G, B, A);
        }

        /// <summary>
        /// Returns a System.Drawing.Color value that corresponds to this color
        /// </summary>
        public System.Drawing.Color ToSystemColor()
        {
            return System.Drawing.Color.FromArgb(this.A, this.R, this.G, this.B);
        }

        /// <summary>
        /// Returns a the color that corresponds to the givenSystem.Drawing.Color value 
        /// </summary>
        static public Color FromSystemColor(System.Drawing.Color color)
        {
            return new Color(color.R, color.G, color.B, color.A);
        }

        /// <summary>
        /// Returns a the color that corresponds to the given normalized (0-1) float color values
        /// </summary>
        static public Color FromNormalized(float r, float g, float b, float a)
        {
            return new Color((byte)(255 * r), (byte)(255 * g), (byte)(255 * b), (byte)(255 * a));
        }


        #endregion

        // Static definitions of pre-defined colors
        public static Color TransparentBlack = new Color(0, 0, 0, 0);
        public static Color TransparentWhite = new Color(255, 255, 255, 0);
        public static Color Black = new Color(0, 0, 0);
        public static Color White = new Color(255, 255, 255);
        public static Color Red = new Color(255, 0, 0);
        public static Color Green = new Color(0, 255, 0);
        public static Color Blue = new Color(0, 0, 255);
        public static Color Purple = new Color(255, 0, 255);
        public static Color Cyan = new Color(0, 255, 255);
        public static Color Yellow = new Color(255, 255, 0);
        
    }

    /// <summary>
    /// TypeConverter for the Color class
    /// </summary>
    public class ColorTypeConverter : TypeConverter
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
            if(destinationType == typeof(string))
                return value.ToString();

            return base.ConvertTo(context, culture, value, destinationType);
        }
        
        public override object ConvertFrom(ITypeDescriptorContext context, System.Globalization.CultureInfo culture, object value)
        {
            if (value is string)
                return Color.Parse(value as string);

            return base.ConvertFrom(context, culture, value);
        }
    }
}