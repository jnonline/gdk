using System;
using System.Collections.Generic;
using System.Reflection;

namespace Gdk
{
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


        #endregion

        #region Methods

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
}