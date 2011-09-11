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
    public class FloatOrParam : ColladaBase
    {
        // Type enum
        public enum FloatOrParamType
        {
            Float,
            Param,
        }

        // Child Objects
        public FloatOrParamType Type;
        public Float1 Float;
        public Param Param;

        /// <summary>
        /// CTor
        /// </summary>
        public FloatOrParam(XmlNode xmlFloatOrParam, ColladaDocument ownerDocument)
            : base(ownerDocument)
        {
            // Load the child element
            // ------------------------

            XmlNode xmlFloat = xmlFloatOrParam.SelectSingleNode("float");
            XmlNode xmlParam = xmlFloatOrParam.SelectSingleNode("param");

            // Process the child elements
            if (xmlFloat != null)
            {
                Type = FloatOrParamType.Float;
                this.Float = new Float1(xmlFloat, ownerDocument);
            }
            else if (xmlParam != null)
            {
                Type = FloatOrParamType.Param;
                this.Param = new Param(xmlParam, ownerDocument);
            }
        }
    }

    // ============================================================
    public class ColorOrTexture : ColladaBase
    {
        // Type enum
        public enum ColorOrTextureType
        {
            Color,
            Param,
            Texture,
        }

        // Attributes
        public string Opaque;

        // Child Objects
        public ColorOrTextureType Type;
        public Color Color;
        public FXParam Param;
        public Texture Texture;
        
        /// <summary>
        /// CTor
        /// </summary>
        public ColorOrTexture(XmlNode xmlColorOrTexture, ColladaDocument ownerDocument)
            : base(ownerDocument)
        {
            // Load the attributes
            // ------------------------

            this.Opaque = XmlUtilities.GetAttribute(xmlColorOrTexture, "opaque", "A_ONE");

            // Load the child element
            // ------------------------

            XmlNode xmlColor = xmlColorOrTexture.SelectSingleNode("color");
            XmlNode xmlParam = xmlColorOrTexture.SelectSingleNode("param");
            XmlNode xmlTexture = xmlColorOrTexture.SelectSingleNode("texture");

            // Process the child elements
            if (xmlColor != null)
            {
                this.Type = ColorOrTextureType.Color;
                this.Color = new Color(xmlColor, ownerDocument);
            }
            else if (xmlParam != null)
            {
                this.Type = ColorOrTextureType.Param;
                this.Param = new FXParam(xmlParam, ownerDocument);
            }
            else if (xmlTexture != null)
            {
                this.Type = ColorOrTextureType.Texture;
                this.Texture = new Texture(xmlTexture, ownerDocument);
            }
        }
    }

    // ============================================================
    public class Texture : ColladaBase
    {
        // Attributes
        public string TextureRef;
        public string TexCoord;

        /// <summary>
        /// CTor
        /// </summary>
        public Texture(XmlNode xmlTexture, ColladaDocument ownerDocument)
            : base(ownerDocument)
        {
            // Load the attributes
            this.TextureRef = XmlUtilities.GetAttribute(xmlTexture, "texture", "");
            this.TexCoord = XmlUtilities.GetAttribute(xmlTexture, "texcoord", "");
        }
    }

    // ============================================================
    public class Blinn : ColladaBase
    {
        // Child Objects
        public ColorOrTexture Emission;
        public ColorOrTexture Ambient;
        public ColorOrTexture Diffuse;
        public ColorOrTexture Specular;
        public FloatOrParam Shininess;
        public ColorOrTexture Reflective;
        public FloatOrParam Reflectivity;
        public ColorOrTexture Transparent;
        public FloatOrParam Transparency;
        public FloatOrParam IndexOfRefraction;

        /// <summary>
        /// CTor
        /// </summary>
        public Blinn(XmlNode xmlConstant, ColladaDocument ownerDocument)
            : base(ownerDocument)
        {
            // Get the child elements
            XmlNode xmlEmission = xmlConstant.SelectSingleNode("emission");
            XmlNode xmlAmbient = xmlConstant.SelectSingleNode("ambient");
            XmlNode xmlDiffuse = xmlConstant.SelectSingleNode("diffuse");
            XmlNode xmlSpecular = xmlConstant.SelectSingleNode("specular");
            XmlNode xmlShininess = xmlConstant.SelectSingleNode("shininess");
            XmlNode xmlReflective = xmlConstant.SelectSingleNode("reflective");
            XmlNode xmlReflectivity = xmlConstant.SelectSingleNode("reflectivity");
            XmlNode xmlTransparent = xmlConstant.SelectSingleNode("transparent");
            XmlNode xmlTransparency = xmlConstant.SelectSingleNode("transparency");
            XmlNode xmlIndexOfRefraction = xmlConstant.SelectSingleNode("index_of_refraction");

            // Load the child elements
            this.Emission = xmlEmission == null ? null : new ColorOrTexture(xmlEmission, ownerDocument);
            this.Ambient = xmlAmbient == null ? null : new ColorOrTexture(xmlAmbient, ownerDocument);
            this.Diffuse = xmlDiffuse == null ? null : new ColorOrTexture(xmlDiffuse, ownerDocument);
            this.Specular = xmlSpecular == null ? null : new ColorOrTexture(xmlSpecular, ownerDocument);
            this.Shininess = xmlShininess == null ? null : new FloatOrParam(xmlShininess, ownerDocument);
            this.Reflective = xmlReflective == null ? null : new ColorOrTexture(xmlReflective, ownerDocument);
            this.Reflectivity = xmlReflectivity == null ? null : new FloatOrParam(xmlReflectivity, ownerDocument);
            this.Transparent = xmlTransparent == null ? null : new ColorOrTexture(xmlTransparent, ownerDocument);
            this.Transparency = xmlTransparency == null ? null : new FloatOrParam(xmlTransparency, ownerDocument);
            this.IndexOfRefraction = xmlIndexOfRefraction == null ? null : new FloatOrParam(xmlIndexOfRefraction, ownerDocument);
        }
    }

    // ============================================================
    public class ConstantFX : ColladaBase
    {
        // Child Objects
        public ColorOrTexture Emission;
        public ColorOrTexture Reflective;
        public FloatOrParam Reflectivity;
        public ColorOrTexture Transparent;
        public FloatOrParam Transparency;
        public FloatOrParam IndexOfRefraction;

        /// <summary>
        /// CTor
        /// </summary>
        public ConstantFX(XmlNode xmlConstant, ColladaDocument ownerDocument)
            : base(ownerDocument)
        {
            // Get the child elements
            XmlNode xmlEmission = xmlConstant.SelectSingleNode("emission");
            XmlNode xmlReflective = xmlConstant.SelectSingleNode("reflective");
            XmlNode xmlReflectivity = xmlConstant.SelectSingleNode("reflectivity");
            XmlNode xmlTransparent = xmlConstant.SelectSingleNode("transparent");
            XmlNode xmlTransparency = xmlConstant.SelectSingleNode("transparency");
            XmlNode xmlIndexOfRefraction = xmlConstant.SelectSingleNode("index_of_refraction");

            // Load the child elements
            this.Emission = xmlEmission == null ? null : new ColorOrTexture(xmlEmission, ownerDocument);
            this.Reflective = xmlReflective == null ? null : new ColorOrTexture(xmlReflective, ownerDocument);
            this.Reflectivity = xmlReflectivity == null ? null : new FloatOrParam(xmlReflectivity, ownerDocument);
            this.Transparent = xmlTransparent == null ? null : new ColorOrTexture(xmlTransparent, ownerDocument);
            this.Transparency = xmlTransparency == null ? null : new FloatOrParam(xmlTransparency, ownerDocument);
            this.IndexOfRefraction = xmlIndexOfRefraction == null ? null : new FloatOrParam(xmlIndexOfRefraction, ownerDocument);
        }
    }

    // ============================================================
    public class Lambert : ColladaBase
    {
        // Child Objects
        public ColorOrTexture Emission;
        public ColorOrTexture Ambient;
        public ColorOrTexture Diffuse;
        public ColorOrTexture Reflective;
        public FloatOrParam Reflectivity;
        public ColorOrTexture Transparent;
        public FloatOrParam Transparency;
        public FloatOrParam IndexOfRefraction;

        /// <summary>
        /// CTor
        /// </summary>
        public Lambert(XmlNode xmlConstant, ColladaDocument ownerDocument)
            : base(ownerDocument)
        {
            // Get the child elements
            XmlNode xmlEmission = xmlConstant.SelectSingleNode("emission");
            XmlNode xmlAmbient = xmlConstant.SelectSingleNode("ambient");
            XmlNode xmlDiffuse = xmlConstant.SelectSingleNode("diffuse");
            XmlNode xmlReflective = xmlConstant.SelectSingleNode("reflective");
            XmlNode xmlReflectivity = xmlConstant.SelectSingleNode("reflectivity");
            XmlNode xmlTransparent = xmlConstant.SelectSingleNode("transparent");
            XmlNode xmlTransparency = xmlConstant.SelectSingleNode("transparency");
            XmlNode xmlIndexOfRefraction = xmlConstant.SelectSingleNode("index_of_refraction");

            // Load the child elements
            this.Emission = xmlEmission == null ? null : new ColorOrTexture(xmlEmission, ownerDocument);
            this.Ambient = xmlAmbient == null ? null : new ColorOrTexture(xmlAmbient, ownerDocument);
            this.Diffuse = xmlDiffuse == null ? null : new ColorOrTexture(xmlDiffuse, ownerDocument);
            this.Reflective = xmlReflective == null ? null : new ColorOrTexture(xmlReflective, ownerDocument);
            this.Reflectivity = xmlReflectivity == null ? null : new FloatOrParam(xmlReflectivity, ownerDocument);
            this.Transparent = xmlTransparent == null ? null : new ColorOrTexture(xmlTransparent, ownerDocument);
            this.Transparency = xmlTransparency == null ? null : new FloatOrParam(xmlTransparency, ownerDocument);
            this.IndexOfRefraction = xmlIndexOfRefraction == null ? null : new FloatOrParam(xmlIndexOfRefraction, ownerDocument);
        }
    }

    // ============================================================
    public class Phong : ColladaBase
    {
        // Child Objects
        public ColorOrTexture Emission;
        public ColorOrTexture Ambient;
        public ColorOrTexture Diffuse;
        public ColorOrTexture Specular;
        public FloatOrParam Shininess;
        public ColorOrTexture Reflective;
        public FloatOrParam Reflectivity;
        public ColorOrTexture Transparent;
        public FloatOrParam Transparency;
        public FloatOrParam IndexOfRefraction;

        /// <summary>
        /// CTor
        /// </summary>
        public Phong(XmlNode xmlConstant, ColladaDocument ownerDocument)
            : base(ownerDocument)
        {
            // Get the child elements
            XmlNode xmlEmission = xmlConstant.SelectSingleNode("emission");
            XmlNode xmlAmbient = xmlConstant.SelectSingleNode("ambient");
            XmlNode xmlDiffuse = xmlConstant.SelectSingleNode("diffuse");
            XmlNode xmlSpecular = xmlConstant.SelectSingleNode("specular");
            XmlNode xmlShininess = xmlConstant.SelectSingleNode("shininess");
            XmlNode xmlReflective = xmlConstant.SelectSingleNode("reflective");
            XmlNode xmlReflectivity = xmlConstant.SelectSingleNode("reflectivity");
            XmlNode xmlTransparent = xmlConstant.SelectSingleNode("transparent");
            XmlNode xmlTransparency = xmlConstant.SelectSingleNode("transparency");
            XmlNode xmlIndexOfRefraction = xmlConstant.SelectSingleNode("index_of_refraction");

            // Load the child elements
            this.Emission = xmlEmission == null ? null : new ColorOrTexture(xmlEmission, ownerDocument);
            this.Ambient = xmlAmbient == null ? null : new ColorOrTexture(xmlAmbient, ownerDocument);
            this.Diffuse = xmlDiffuse == null ? null : new ColorOrTexture(xmlDiffuse, ownerDocument);
            this.Specular = xmlSpecular == null ? null : new ColorOrTexture(xmlSpecular, ownerDocument);
            this.Shininess = xmlShininess == null ? null : new FloatOrParam(xmlShininess, ownerDocument);
            this.Reflective = xmlReflective == null ? null : new ColorOrTexture(xmlReflective, ownerDocument);
            this.Reflectivity = xmlReflectivity == null ? null : new FloatOrParam(xmlReflectivity, ownerDocument);
            this.Transparent = xmlTransparent == null ? null : new ColorOrTexture(xmlTransparent, ownerDocument);
            this.Transparency = xmlTransparency == null ? null : new FloatOrParam(xmlTransparency, ownerDocument);
            this.IndexOfRefraction = xmlIndexOfRefraction == null ? null : new FloatOrParam(xmlIndexOfRefraction, ownerDocument);
        }
    }
   
}
