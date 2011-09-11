/*
  Copyright (c) 2011, Raincity Games LLC
  Licensed under the MIT license: http://www.opensource.org/licenses/mit_license.php
*/

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml;

namespace Gdk.Framework.Content.COLLADA
{
    // ============================================================
    public enum FXParameterTypes
    {
        Float,
        Float2,
        Float3,
        Float4,
        Sampler2D,
        Surface,
    }

    // ============================================================
    public enum FXParameterModifiers
    {
        CONST,
        UNIFORM,
        VARYING,
        STATIC,
        VOLATILE,
        EXTERN,
        SHARED
    }

    // ============================================================
    public class FXParam : ColladaBase
    {
        // Attributes
        public string Ref;

        /// <summary>
        /// CTor - from a <accessor/> node
        /// </summary>
        public FXParam(XmlNode xmlParam, ColladaDocument ownerDocument)
            : base(ownerDocument)
        {
            // Load the attributes
            // --------------------------

            // <param> elements inside a <texture*> element, do not use the "ref" attribute, instead the 
            // parameter Sid is in the text of the element.
            this.Ref = XmlUtilities.GetAttribute(xmlParam, "ref", xmlParam.InnerText.Trim());
        }
    }

    // ============================================================
    public class NewParam : ColladaBase
    {
        // Attributes
        public string Sid;

        // Child Objects
        public string Semantic;
        public FXParameterModifiers Modifier;
        public ValueTypeBase Value;

        /// <summary>
        /// CTor - from a <source/> node
        /// </summary>
        public NewParam(XmlNode xmlNewParam, ColladaDocument ownerDocument)
            : base(ownerDocument)
        {
            // Load the attributes
            // --------------------------

            this.Sid = XmlUtilities.GetAttribute(xmlNewParam, "sid", "");

            // Load the simple child nodes
            // --------------------------

            // Load the <semantic> node
            XmlNode xmlSemantic = xmlNewParam.SelectSingleNode("semantic");
            if (xmlSemantic != null)
                this.Semantic = xmlSemantic.InnerText.Trim();

            // Load the <modifier> node
            XmlNode xmlModifier = xmlNewParam.SelectSingleNode("modifier");
            if (xmlModifier != null)
                this.Modifier = (FXParameterModifiers)Enum.Parse(typeof(FXParameterModifiers), xmlModifier.InnerText.Trim());

            // Load the value type
            // ---------------------------------------

            // Get the value type from child nodes
            List<ValueTypeBase> values = ValueTypeBase.FromXmlNodeList(xmlNewParam.ChildNodes, ownerDocument);
            
            // Verify only 1 existed
            if (values.Count == 0)
                throw new ColladaLoadException("[newparam] node does not contain a child value node");
            if (values.Count > 1)
                throw new ColladaLoadException("[newparam] node contains more than one child value node");

            this.Value = values[0];
        }
    }

    // ============================================================
    public class SetParam : ColladaBase
    {
        // Attributes
        public string Ref;
        public string Program;

        // Child Objects
        public ValueTypeBase Value;

        /// <summary>
        /// CTor - from a <source/> node
        /// </summary>
        public SetParam(XmlNode xmlSetParam, ColladaDocument ownerDocument)
            : base(ownerDocument)
        {
            // Load the attributes
            // --------------------------

            this.Ref = XmlUtilities.GetAttribute(xmlSetParam, "ref", "");
            this.Program = XmlUtilities.GetAttribute(xmlSetParam, "program", "");

            // Load the value type
            // ---------------------------------------

            // Get the value type from child nodes
            List<ValueTypeBase> values = ValueTypeBase.FromXmlNodeList(xmlSetParam.ChildNodes, ownerDocument);

            // Verify only 1 existed
            if (values.Count == 0)
                throw new ColladaLoadException("[newparam] node does not contain a child value node");
            if (values.Count > 1)
                throw new ColladaLoadException("[newparam] node contains more than one child value node");

            this.Value = values[0];
        }
    }

    // ============================================================
    public class ValueTypeBase : ColladaBase
    {
        /// <summary>
        /// CTor
        /// </summary>
        public ValueTypeBase(XmlNode xmlData, ColladaDocument ownerDocument)
            : base(ownerDocument)
        {
        }

        /// <summary>
        /// Creates an appropriate FX parameter from the given xml.
        ///   Ex:  <float>1.0</float>,  <surface>...</surface>, etc
        /// </summary>
        static public ValueTypeBase FromXmlNode(XmlNode xmlData, ColladaDocument ownerDocument)
        {
            // Do the load based on the xml node name
            switch (xmlData.Name)
            {
                case "float": return new Float1(xmlData, ownerDocument);
                case "float2": return new Float2(xmlData, ownerDocument);
                case "float3": return new Float3(xmlData, ownerDocument);
                case "float4": return new Float4(xmlData, ownerDocument);
                case "surface": return new Surface(xmlData, ownerDocument);
                case "sampler2D": return new Sampler2D(xmlData, ownerDocument);
            }

            // Unrecognized node name
            return null;
        }

        /// <summary>
        /// Parses the given node list for any FX parameters & loads them
        ///   Ex:  <float>1.0</float>,  <surface>...</surface>, etc
        /// </summary>
        static public List<ValueTypeBase> FromXmlNodeList(XmlNodeList xmlNodes, ColladaDocument ownerDocument)
        {
            List<ValueTypeBase> results = new List<ValueTypeBase>();

            // Loop through the nodes
            foreach (XmlNode xmlNode in xmlNodes)
            {
                // Attempt to convert this node to a parameter (and store it if successfull)
                ValueTypeBase param = FromXmlNode(xmlNode, ownerDocument);
                if (param != null)
                    results.Add(param);
            }

            // Return the results
            return results;
        }
    }

    // ============================================================
    public class Float1 : ValueTypeBase
    {
        public float Value;

        // CTor
        public Float1(XmlNode xmlFloat, ColladaDocument ownerDocument)
            : base(xmlFloat, ownerDocument)
        {
            List<float> floatValues = StringUtilities.ParseFloats(xmlFloat.InnerText);
            this.Value = floatValues[0];
        }
    }

    // ============================================================
    public class Float2 : ValueTypeBase
    {
        public float[] Values = new float[2];
        public float this[int index] { get { return Values[index]; } set { this.Values[index] = value; } }
       
        // CTor
        public Float2(XmlNode xmlFloat2, ColladaDocument ownerDocument)
            : base(xmlFloat2, ownerDocument)
        {
            List<float> floatValues = StringUtilities.ParseFloats(xmlFloat2.InnerText);
            this.Values[0] = floatValues[0];
            this.Values[1] = floatValues[1];
        }
    }

    // ============================================================
    public class Float3 : ValueTypeBase
    {
        public float[] Values = new float[3];
        public float this[int index] { get { return Values[index]; } set { this.Values[index] = value; } }

        // CTor
        public Float3(XmlNode xmlFloat3, ColladaDocument ownerDocument)
            : base(xmlFloat3, ownerDocument)
        {
            List<float> floatValues = StringUtilities.ParseFloats(xmlFloat3.InnerText);
            this.Values[0] = floatValues[0];
            this.Values[1] = floatValues[1];
            this.Values[2] = floatValues[2];
        }
    }

    // ============================================================
    public class Float4 : ValueTypeBase
    {
        public float[] Values = new float[4];
        public float this[int index] { get { return Values[index]; } set { this.Values[index] = value; } }

        // CTor
        public Float4(XmlNode xmlFloat4, ColladaDocument ownerDocument)
            : base(xmlFloat4, ownerDocument)
        {
            List<float> floatValues = StringUtilities.ParseFloats(xmlFloat4.InnerText);
            this.Values[0] = floatValues[0];
            this.Values[1] = floatValues[1];
            this.Values[2] = floatValues[2];
            this.Values[3] = floatValues[3];
        }
    }

    // ============================================================
    public class Color : ColladaBase
    {
        public float R;
        public float G;
        public float B;
        public float A;

        // CTor
        public Color(XmlNode xmlColor, ColladaDocument ownerDocument)
            : base(ownerDocument)
        {
            List<float> floatValues = StringUtilities.ParseFloats(xmlColor.InnerText);
            R = floatValues[0];
            G = floatValues[1];
            B = floatValues[2];
            A = floatValues[3];
        }

        /// <summary>
        /// Utility to get the Gdk.Color equivalent of this collada color
        /// </summary>
        public Gdk.Framework.Color ToGdkColor()
        {
            return Gdk.Framework.Color.FromNormalized(this.R, this.G, this.B, this.A);
        }
    }
}
