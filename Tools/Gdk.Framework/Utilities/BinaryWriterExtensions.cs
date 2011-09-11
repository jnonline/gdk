/*
  Copyright (c) 2011, Raincity Games LLC
  Licensed under the MIT license: http://www.opensource.org/licenses/mit_license.php
*/

using System;
using System.Collections.Generic;
using System.Reflection;
using System.Text;
using System.IO;

namespace Gdk.Framework
{
	/// <summary>
	/// This utility class provides properties & methods for writing various gdk objects
	/// </summary>
    static public class BinaryWriterExtensions
	{
        /// <summary>
        /// Writes a Vector2
        /// </summary>
        static public void WriteVector2(this BinaryWriter writer, Vector2 v)
        {
            writer.Write(v.X);
            writer.Write(v.Y);
        }

        /// <summary>
        /// Writes a Vector3
        /// </summary>
        static public void WriteVector3(this BinaryWriter writer, Vector3 v)
        {
            writer.Write(v.X);
            writer.Write(v.Y);
            writer.Write(v.Z);
        }

        /// <summary>
        /// Writes a Color
        /// </summary>
        static public void WriteColor(this BinaryWriter writer, Color color)
        {
            writer.Write(color.R);
            writer.Write(color.G);
            writer.Write(color.B);
            writer.Write(color.A);
        }

        /// <summary>
        /// Writes a Matrix
        /// </summary>
        static public void WriteMatrix(this BinaryWriter writer, Matrix m)
        {
            writer.Write(m.M11); writer.Write(m.M12); writer.Write(m.M13); writer.Write(m.M14);
            writer.Write(m.M21); writer.Write(m.M22); writer.Write(m.M23); writer.Write(m.M24);
            writer.Write(m.M31); writer.Write(m.M32); writer.Write(m.M33); writer.Write(m.M34);
            writer.Write(m.M41); writer.Write(m.M42); writer.Write(m.M43); writer.Write(m.M44);
        }

        /// <summary>
        /// Writes a string, in a gdk readable format
        /// </summary>
        static public void WriteString(this BinaryWriter writer, string value)
        {
            writer.Write((ushort)value.Length);
            writer.Write((char[])value.ToCharArray());
        }
	}
}