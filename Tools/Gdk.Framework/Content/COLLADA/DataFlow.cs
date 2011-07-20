/*
  Copyright (c) 2011, Raincity Games LLC
  Licensed under the MIT license: http://www.opensource.org/licenses/mit_license.php
*/

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml;

namespace Gdk.Content.COLLADA
{
    // ============================================================
    public class Source : ColladaBase, IColladaObjectWithId
    {
        // Attributes
        public string Id { get; set; }  
        public string Name;

        // Child Objects
        public ValueArrayBase ArrayData;
        public Accessor Accessor;

        /// <summary>
        /// CTor - from a <source/> node
        /// </summary>
        public Source(XmlNode xmlSource, ColladaDocument ownerDocument)
            : base(ownerDocument)
        {
            // Load the attributes
            // --------------------------

            this.Id = XmlUtilities.GetAttribute(xmlSource, "id", "");
            this.Name = XmlUtilities.GetAttribute(xmlSource, "name", "");

            // Register this object by its Id
            this.OwnerDocument.RegisterObjectWithId(this);

            // Load the data array child node (optional)
            // --------------------------

            // Get the child array element (there may be 0 or 1 of these)
            XmlNode xmlBoolArray = xmlSource.SelectSingleNode("bool_array");
            XmlNode xmlFloatArray = xmlSource.SelectSingleNode("float_array");
            XmlNode xmlIntArray = xmlSource.SelectSingleNode("int_array");
            XmlNode xmlNameArray = xmlSource.SelectSingleNode("Name_array");
            XmlNode xmlIDREFArray = xmlSource.SelectSingleNode("IDREF_array");

            // Handle whichever array exists
            if (xmlBoolArray != null)
                this.ArrayData = new BoolArray(xmlBoolArray, ownerDocument);
            else if (xmlFloatArray != null)
                this.ArrayData = new FloatArray(xmlFloatArray, ownerDocument);
            else if (xmlIntArray != null)
                this.ArrayData = new IntArray(xmlIntArray, ownerDocument);
            else if (xmlNameArray != null)
                this.ArrayData = new StringArray(xmlNameArray, ownerDocument);
            else if (xmlIDREFArray != null)
                this.ArrayData = new StringArray(xmlIDREFArray, ownerDocument);

            // Load the acessor (optional)
            // --------------------------

            // Get the accessor from the common technique
            XmlNode xmlAccessor = xmlSource.SelectSingleNode("technique_common/accessor");
            if (xmlAccessor != null)
                this.Accessor = new Accessor(xmlAccessor, ownerDocument);

        }
    }

    // ============================================================
    public abstract class ValueArrayBase : ColladaBase, IColladaObjectWithId
    {
        // Attributes
        public string Id { get; set; }  
        public string Name;
        public int Count;

        /// <summary>
        /// CTor
        /// </summary>
        public ValueArrayBase(XmlNode xmlData, ColladaDocument ownerDocument)
            : base(ownerDocument)
        { 
            // Load the attributes
            // --------------------------

            // Verify the "count" attribute exists
            if (XmlUtilities.VerifyAttributesExist(xmlData, false, "count") == false)
                throw new ColladaLoadException("[" + xmlData.Name  + "] element is missing the [count] attribute");

            this.Id = XmlUtilities.GetAttribute(xmlData, "id", "");
            this.Name = XmlUtilities.GetAttribute(xmlData, "name", "");
            this.Count = XmlUtilities.GetAttribute(xmlData, "count", 0);
            
            // Register this object by its Id
            this.OwnerDocument.RegisterObjectWithId(this);
        }

        /// <summary>
        /// Lookup by index method to be implemented by all derived classes
        /// </summary>
        public abstract object GetValue(int index);
    }

    // ============================================================
    public class FloatArray : ValueArrayBase
    {
        // Attributes
        public int Digits;
        public int Magnitude;

        // Data
        public List<float> Values = new List<float>();

        /// <summary>
        /// CTor - from <float_array/> element
        /// </summary>
        public FloatArray(XmlNode xmlFloatArray, ColladaDocument ownerDocument)
            : base(xmlFloatArray, ownerDocument)
        {
            // Load the attributes
            // --------------------------

            this.Digits = XmlUtilities.GetAttribute(xmlFloatArray, "digits", 6);
            this.Magnitude = XmlUtilities.GetAttribute(xmlFloatArray, "magnitude", 38);

            // Load the array data
            // --------------------------

            this.Values = StringUtilities.ParseFloats(xmlFloatArray.InnerText.Trim());
        }

        /// <summary>
        /// Gets the indexed value as an object
        /// </summary>
        public override object GetValue(int index)
        {
            return this.Values[index];
        }
    }

    // ============================================================
    public class BoolArray : ValueArrayBase
    {
        // Data
        public List<bool> Values = new List<bool>();

        /// <summary>
        /// CTor - from <bool_array/> element
        /// </summary>
        public BoolArray(XmlNode xmlBoolArray, ColladaDocument ownerDocument)
            : base(xmlBoolArray, ownerDocument)
        {
            // Load the array data
            // --------------------------

            this.Values = StringUtilities.ParseBools(xmlBoolArray.InnerText.Trim());
        }

        /// <summary>
        /// Gets the indexed value as an object
        /// </summary>
        public override object GetValue(int index)
        {
            return this.Values[index];
        }
    }

    // ============================================================
    public class IntArray : ValueArrayBase
    {
        // Attributes
        public int MinInclusive;
        public int MaxInclusive;

        // Data
        public List<int> Values = new List<int>();

        /// <summary>
        /// CTor - from <int_array/> element
        /// </summary>
        public IntArray(XmlNode xmlIntArray, ColladaDocument ownerDocument)
            : base(xmlIntArray, ownerDocument)
        {
            // Load the attributes
            // --------------------------

            this.MinInclusive = XmlUtilities.GetAttribute(xmlIntArray, "minInclusive", int.MinValue);
            this.MaxInclusive = XmlUtilities.GetAttribute(xmlIntArray, "maxInclusive", int.MaxValue);

            // Load the array data
            // --------------------------

            this.Values = StringUtilities.ParseInts(xmlIntArray.InnerText.Trim());
        }

        /// <summary>
        /// Gets the indexed value as an object
        /// </summary>
        public override object GetValue(int index)
        {
            return this.Values[index];
        }
    }

    // ============================================================
    public class StringArray : ValueArrayBase
    {
        // Data
        public List<string> Values = new List<string>();

        /// <summary>
        /// CTor - from <Name_array/>, <IDREF_array/>, or <SIDREF_array/> element
        /// </summary>
        public StringArray(XmlNode xmlStringArray, ColladaDocument ownerDocument)
            : base(xmlStringArray, ownerDocument)
        {
            // Load the array data
            // --------------------------

            // Get the list of string values
            string stringOfValues = xmlStringArray.InnerText;

            // Split up each value by the seperating space 
            this.Values = stringOfValues.Split(' ').ToList();
        }

        /// <summary>
        /// Gets the indexed value as an object
        /// </summary>
        public override object GetValue(int index)
        {
            return this.Values[index];
        }
    }

    // ============================================================
    public class Accessor : ColladaBase
    {
        // Attributes
        public int Count;
        public int Offset;
        public int Stride;
        public string Source;

        // Children
        public List<Param> Params = new List<Param>();

        /// <summary>
        /// CTor - from a <accessor/> node
        /// </summary>
        public Accessor(XmlNode xmlAccessor, ColladaDocument ownerDocument)
            : base(ownerDocument)
        {
            // Load the attributes
            // --------------------------

            // Verify the "count" attribute exists
            if (XmlUtilities.VerifyAttributesExist(xmlAccessor, false, "count", "source") == false)
                throw new ColladaLoadException("[accessor] element is missing a [count] or [source] attribute");

            this.Count = XmlUtilities.GetAttribute(xmlAccessor, "count", 0);
            this.Offset = XmlUtilities.GetAttribute(xmlAccessor, "offset", 0);
            this.Stride = XmlUtilities.GetAttribute(xmlAccessor, "stride", 1);
            this.Source = XmlUtilities.GetAttribute(xmlAccessor, "source", "");

            // Load the <param> child nodes
            // --------------------------

            // Loop through the <param> nodes
            foreach (XmlNode xmlParam in xmlAccessor.SelectNodes("param"))
            {
                // Load the Param & store it in this accessor
                Param param = new Param(xmlParam, ownerDocument);
                this.Params.Add(param);
            }
        }
    }

    // ============================================================
    public class Param : ColladaBase
    {
        // Attributes
        public string Name;
        public string Sid;
        public string Type;
        public string Semantic;

        /// <summary>
        /// CTor - from a <accessor/> node
        /// </summary>
        public Param(XmlNode xmlParam, ColladaDocument ownerDocument)
            : base(ownerDocument)
        {
            // Load the attributes
            // --------------------------

            // Verify the required attributes exists
            if (XmlUtilities.VerifyAttributesExist(xmlParam, false, "type") == false)
                throw new ColladaLoadException("[param] element is missing the [type] attribute");

            this.Name = XmlUtilities.GetAttribute(xmlParam, "name", "");
            this.Sid = XmlUtilities.GetAttribute(xmlParam, "sid", "");
            this.Type = XmlUtilities.GetAttribute(xmlParam, "type", "");
            this.Semantic = XmlUtilities.GetAttribute(xmlParam, "semantic", "");
        }
    }
    
    // ============================================================
    public class Input : ColladaBase
    {
        // Attributes
        public string Semantic;
        public string Source;

        /// <summary>
        /// CTor - from a <source/> node
        /// </summary>
        public Input(XmlNode xmlInput, ColladaDocument ownerDocument)
            : base(ownerDocument)
        {
            // Load the attributes
            // --------------------------

            // Verify the required attributes exists
            if (XmlUtilities.VerifyAttributesExist(xmlInput, false, "semantic", "source") == false)
                throw new ColladaLoadException("[input] element is missing the [semantic] or [source] attribute");

            this.Semantic = XmlUtilities.GetAttribute(xmlInput, "semantic", "");
            this.Source = XmlUtilities.GetAttribute(xmlInput, "source", "");
        }
    }

    // ============================================================
    public class SharedInput : Input
    {
        // Attributes
        public int Offset;
        public int Set;

        /// <summary>
        /// CTor - from a <source/> node
        /// </summary>
        public SharedInput(XmlNode xmlInput, ColladaDocument ownerDocument)
            : base(xmlInput, ownerDocument)
        {
            // Load the attributes
            // --------------------------

            // Verify the required attributes exists
            if (XmlUtilities.VerifyAttributesExist(xmlInput, false, "offset") == false)
                throw new ColladaLoadException("[input] element is missing the [offset] attribute");

            this.Offset = XmlUtilities.GetAttribute(xmlInput, "offset", 0);
            this.Set = XmlUtilities.GetAttribute(xmlInput, "set", -1);
        }
    }
}
