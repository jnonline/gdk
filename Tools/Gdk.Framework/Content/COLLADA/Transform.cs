using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml;

namespace Gdk.Content.COLLADA
{
    // ============================================================
    public abstract class TransformBase : ColladaBase
    {
        // Attributes
        public string Sid;

        /// <summary>
        /// CTor
        /// </summary>
        public TransformBase(XmlNode xmlTransform, ColladaDocument ownerDocument)
            : base(ownerDocument)
        {
            // Load the attributes
            // --------------------------

            this.Sid = XmlUtilities.GetAttribute(xmlTransform, "sid", "");
        }

        /// <summary>
        /// Parses all the transform elements from a list of xml nodes
        /// </summary>
        /// <param name="xmlNodeList"></param>
        /// <returns></returns>
        static public List<TransformBase> ParseTransforms(XmlNodeList xmlNodeList, ColladaDocument ownerDocument)
        {
            List<TransformBase> transforms = new List<TransformBase>();

            // Loop through all the child nodes
            foreach (XmlNode xmlChild in xmlNodeList)
            {
                // If this is a transform node, parse it
                switch (xmlChild.Name)
                {
                    case "lookat": transforms.Add(new LookAt(xmlChild, ownerDocument)); break;
                    case "matrix": transforms.Add(new Matrix(xmlChild, ownerDocument)); break;
                    case "rotate": transforms.Add(new Rotate(xmlChild, ownerDocument)); break;
                    case "scale": transforms.Add(new Scale(xmlChild, ownerDocument)); break;
                    case "scew": transforms.Add(new Skew(xmlChild, ownerDocument)); break;
                    case "translate": transforms.Add(new Translate(xmlChild, ownerDocument)); break;
                }
            }

            return transforms;
        }

        /// <summary>
        /// Gets the Gdk.Matrix that performs the transformation defined in the derived class
        /// </summary>
        public abstract Gdk.Matrix GetTransformMatrix();

        /// <summary>
        /// Combines all the transforms in the given list and returns the resultant Gdk.Matrix that performs the full transform stack
        /// </summary>
        /// <param name="transforms"></param>
        /// <returns></returns>
        static public Gdk.Matrix GetCombinedTransforms(List<TransformBase> transforms)
        {
            // Start with the identity matrix
            Gdk.Matrix result = Gdk.Matrix.Identity;

            // Loop through the transforms
            foreach (TransformBase transform in transforms)
            {
                // Get this transform's matrix equivalent, and multiply it through the transform stack
                Gdk.Matrix transformMatrix = transform.GetTransformMatrix();
                result *= transformMatrix;
            }

            return result;
        }
    }

    // ============================================================
    public class LookAt : TransformBase
    {
        // Values
        Vector3 Position;
        Vector3 Target;
        Vector3 Up;

        /// <summary>
        /// CTor
        /// </summary>
        public LookAt(XmlNode xmlLookAt, ColladaDocument ownerDocument)
            : base(xmlLookAt, ownerDocument)
        {
            // Get the float values
            List<float> floatValues = StringUtilities.ParseFloats(xmlLookAt.InnerText, ' ');

            // Get the vectors from the floats
            Position.X = floatValues[0];
            Position.Y = floatValues[1];
            Position.Z = floatValues[2];
            Target.X = floatValues[3];
            Target.Y = floatValues[4];
            Target.Z = floatValues[5];
            Up.X = floatValues[6];
            Up.Y = floatValues[7];
            Up.Z = floatValues[8];
        }

        /// <summary>
        /// Gets the Gdk.Matrix that performs this transformation
        /// </summary>
        public override Gdk.Matrix GetTransformMatrix()
        {
            return Gdk.Matrix.CreateLookAt(this.Position, this.Target, this.Up);
        }
    }

    // ============================================================
    public class Matrix : TransformBase
    {
        // Values
        Gdk.Matrix Value;

        /// <summary>
        /// CTor
        /// </summary>
        public Matrix(XmlNode xmlMatrix, ColladaDocument ownerDocument)
            : base(xmlMatrix, ownerDocument)
        {
            // Get the float values
            List<float> floatValues = StringUtilities.ParseFloats(xmlMatrix.InnerText, ' ');

            // Load the matrix
            this.Value = new Gdk.Matrix(floatValues.ToArray());
        }

        /// <summary>
        /// Gets the Gdk.Matrix that performs this transformation
        /// </summary>
        public override Gdk.Matrix GetTransformMatrix()
        {
            return this.Value;
        }
    }

    // ============================================================
    public class Rotate : TransformBase
    {
        // Values
        Vector3 Axis;
        float Angle;        // In Degrees

        /// <summary>
        /// CTor
        /// </summary>
        public Rotate(XmlNode xmlRotate, ColladaDocument ownerDocument)
            : base(xmlRotate, ownerDocument)
        {
            // Get the float values
            List<float> floatValues = StringUtilities.ParseFloats(xmlRotate.InnerText, ' ');

            // Load the rotation values
            Axis.X = floatValues[0];
            Axis.Y = floatValues[1];
            Axis.Z = floatValues[2];
            Angle = floatValues[3];
        }

        /// <summary>
        /// Gets the Gdk.Matrix that performs this transformation
        /// </summary>
        public override Gdk.Matrix GetTransformMatrix()
        {
            return Gdk.Matrix.CreateRotationAroundAxis(this.Axis, MathUtility.DegreesToRadians(this.Angle));
        }
    }

    // ============================================================
    public class Scale : TransformBase
    {
        // Values
        Vector3 Value;

        /// <summary>
        /// CTor
        /// </summary>
        public Scale(XmlNode xmlScale, ColladaDocument ownerDocument)
            : base(xmlScale, ownerDocument)
        {
            // Get the float values
            List<float> floatValues = StringUtilities.ParseFloats(xmlScale.InnerText, ' ');

            // Load the rotation values
            Value.X = floatValues[0];
            Value.Y = floatValues[1];
            Value.Z = floatValues[2];
        }

        /// <summary>
        /// Gets the Gdk.Matrix that performs this transformation
        /// </summary>
        public override Gdk.Matrix GetTransformMatrix()
        {
            return Gdk.Matrix.CreateScale(this.Value);
        }
    }

    // ============================================================
    public class Skew : TransformBase
    {
        // Values
        float Angle;
        Vector3 AxisOfRotation;
        Vector3 AxisOfTranslation;

        /// <summary>
        /// CTor
        /// </summary>
        public Skew(XmlNode xmlSkew, ColladaDocument ownerDocument)
            : base(xmlSkew, ownerDocument)
        {
            // Get the float values
            List<float> floatValues = StringUtilities.ParseFloats(xmlSkew.InnerText, ' ');

            // Load the rotation values
            Angle = floatValues[0];
            AxisOfRotation.X = floatValues[1];
            AxisOfRotation.Y = floatValues[2];
            AxisOfRotation.Z = floatValues[3];
            AxisOfTranslation.X = floatValues[4];
            AxisOfTranslation.Y = floatValues[5];
            AxisOfTranslation.Z = floatValues[6];
        }

        /// <summary>
        /// Gets the Gdk.Matrix that performs this transformation
        /// </summary>
        public override Gdk.Matrix GetTransformMatrix()
        {
            throw new NotImplementedException("<skew> transforms are currently not implemented");
        }
    }

    // ============================================================
    public class Translate : TransformBase
    {
        // Values
        Vector3 Value;

        /// <summary>
        /// CTor
        /// </summary>
        public Translate(XmlNode xmlTranslate, ColladaDocument ownerDocument)
            : base(xmlTranslate, ownerDocument)
        {
            // Get the float values
            List<float> floatValues = StringUtilities.ParseFloats(xmlTranslate.InnerText, ' ');

            // Load the rotation values
            Value.X = floatValues[0];
            Value.Y = floatValues[1];
            Value.Z = floatValues[2];
        }

        /// <summary>
        /// Gets the Gdk.Matrix that performs this transformation
        /// </summary>
        public override Gdk.Matrix GetTransformMatrix()
        {
            return Gdk.Matrix.CreateTranslation(this.Value);
        }
    }
}
