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
    public class Effect : ColladaBase, IColladaObjectWithId
    {
        // Attributes
        public string Id { get; set; }     
        public string Name;
        
        // Child Objects
        public List<Image> Images = new List<Image>();
        public List<NewParam> NewParams = new List<NewParam>();
        public ProfileCommon Profile_COMMON;

        /// <summary>
        /// CTor
        /// </summary>
        public Effect(XmlNode xmlEffect, ColladaDocument ownerDocument)
            : base(ownerDocument)
        {
            // Load the attributes
            // --------------------------

            this.Id = XmlUtilities.GetAttribute(xmlEffect, "id", "");
            this.Name = XmlUtilities.GetAttribute(xmlEffect, "name", "");

            // Register this object by its Id
            this.OwnerDocument.RegisterObjectWithId(this);

            // Load the child nodes
            // ------------------------------

            // Load all the <image> nodes
            foreach (XmlNode xmlImage in xmlEffect.SelectNodes("image"))
                this.Images.Add(new Image(xmlImage, ownerDocument));

            // Load all the <newparam> nodes
            foreach (XmlNode xmlNewParam in xmlEffect.SelectNodes("newparam"))
                this.NewParams.Add(new NewParam(xmlNewParam, ownerDocument));

            // <profile_COMMON>
            XmlNode xmlProfileCommon = xmlEffect.SelectSingleNode("profile_COMMON");
            if (xmlProfileCommon != null)
                this.Profile_COMMON = new ProfileCommon(xmlProfileCommon, ownerDocument);
        }
    }

    // ============================================================
    public class ProfileCommon : ColladaBase, IColladaObjectWithId
    {
        // Attributes
        public string Id { get; set; }   

        // Child objects
        public List<Image> Images = new List<Image>();
        public List<NewParam> NewParams = new List<NewParam>();
        public TechniqueFX Technique;

        /// <summary>
        /// CTor
        /// </summary>
        public ProfileCommon(XmlNode xmlProfileCommon, ColladaDocument ownerDocument)
            : base(ownerDocument)
        {
            // Load the attributes
            // --------------------------

            this.Id = XmlUtilities.GetAttribute(xmlProfileCommon, "id", "");

            // Register this object by its Id
            this.OwnerDocument.RegisterObjectWithId(this);

            // Load the child elements
            // --------------------------

            // Load all the <image> nodes
            foreach (XmlNode xmlImage in xmlProfileCommon.SelectNodes("image"))
                this.Images.Add(new Image(xmlImage, ownerDocument));

            // Load all the <newparam> nodes
            foreach (XmlNode xmlNewParam in xmlProfileCommon.SelectNodes("newparam"))
                this.NewParams.Add(new NewParam(xmlNewParam, ownerDocument));

            // Load the technique
            XmlNode xmlTechnique = xmlProfileCommon.SelectSingleNode("technique");
            if(xmlTechnique == null)
                throw new ColladaLoadException("[profile_COMMON] node is missing a child [technique] node");
            this.Technique = new TechniqueFX(xmlTechnique, ownerDocument);
        }
    }

    // ============================================================
    public enum TechniqueType
    {
        Constant,
        Blinn,
        Phong,
        Lambert,
    }

    // ============================================================
    public class TechniqueFX : ColladaBase, IColladaObjectWithId
    {
        // Attributes
        public string Id { get; set; }
        public string Sid;

        // Child Objects
        public List<Image> Images = new List<Image>();
        public List<NewParam> NewParams = new List<NewParam>();

        public TechniqueType Type;
        public ConstantFX Constant;
        public Blinn Blinn;
        public Phong Phong;
        public Lambert Lambert;

        /// <summary>
        /// CTor
        /// </summary>
        public TechniqueFX(XmlNode xmlTechnique, ColladaDocument ownerDocument)
            : base(ownerDocument)
        {
            // Load the attributes
            // --------------------------

            this.Id = XmlUtilities.GetAttribute(xmlTechnique, "id", "");
            this.Sid = XmlUtilities.GetAttribute(xmlTechnique, "sid", "");

            // Register this object by its Id
            this.OwnerDocument.RegisterObjectWithId(this);

            // Load the child elements
            // --------------------------

            // Load all the <image> nodes
            foreach (XmlNode xmlImage in xmlTechnique.SelectNodes("image"))
                this.Images.Add(new Image(xmlImage, ownerDocument));

            // Load all the <newparam> nodes
            foreach (XmlNode xmlNewParam in xmlTechnique.SelectNodes("newparam"))
                this.NewParams.Add(new NewParam(xmlNewParam, ownerDocument));
            
            // Load the render type/setting element
            // -------------------------------

            XmlNode xmlBlinn = xmlTechnique.SelectSingleNode("blinn");
            XmlNode xmlConstant = xmlTechnique.SelectSingleNode("constant");
            XmlNode xmlLambert = xmlTechnique.SelectSingleNode("lambert");
            XmlNode xmlPhong = xmlTechnique.SelectSingleNode("phong");

            if (xmlBlinn != null)
            {
                this.Type = TechniqueType.Blinn;
                this.Blinn = new Blinn(xmlBlinn, ownerDocument);
            }
            if (xmlConstant != null)
            {
                this.Type = TechniqueType.Constant;
                this.Constant = new ConstantFX(xmlConstant, ownerDocument);
            }
            if (xmlLambert != null)
            {
                this.Type = TechniqueType.Lambert;
                this.Lambert = new Lambert(xmlLambert, ownerDocument);
            }
            if (xmlPhong != null)
            {
                this.Type = TechniqueType.Phong;
                this.Phong = new Phong(xmlPhong, ownerDocument);
            }
        }
    }

    // ============================================================
    public class InstanceEffect : ColladaBase
    {
        // Attributes
        public string Sid;
        public string Name;
        public string Url;

        // Child Objects
        public List<TechniqueHint> TechniqueHints = new List<TechniqueHint>();
        public List<SetParam> SetParams = new List<SetParam>();

        /// <summary>
        /// CTor
        /// </summary>
        public InstanceEffect(XmlNode xmlInstanceEffect, ColladaDocument ownerDocument)
            : base(ownerDocument)
        {
            // Load the attributes
            // --------------------------

            this.Sid = XmlUtilities.GetAttribute(xmlInstanceEffect, "sid", "");
            this.Name = XmlUtilities.GetAttribute(xmlInstanceEffect, "name", "");
            this.Url = XmlUtilities.GetAttribute(xmlInstanceEffect, "url", "");

            // Load the child elements
            // -------------------------------

            // <technique_hint>
            foreach(XmlNode xmlTechniqueHint in xmlInstanceEffect.SelectNodes("technique_hint"))
                this.TechniqueHints.Add(new TechniqueHint(xmlTechniqueHint, ownerDocument));

            // <setparam>
            foreach (XmlNode xmlSetParam in xmlInstanceEffect.SelectNodes("setparam"))
                this.SetParams.Add(new SetParam(xmlSetParam, ownerDocument));
        }
    }
    
    // ============================================================
    public class TechniqueHint : ColladaBase
    {
        // Attributes
        public string Platform;
        public string Ref;
        public string Profile;

        // Child Objects
        List<TechniqueHint> TechniqueHints = new List<TechniqueHint>();

        /// <summary>
        /// CTor
        /// </summary>
        public TechniqueHint(XmlNode xmlInstanceEffect, ColladaDocument ownerDocument)
            : base(ownerDocument)
        {
            // Load the attributes
            // --------------------------

            this.Platform = XmlUtilities.GetAttribute(xmlInstanceEffect, "platform", "");
            this.Ref = XmlUtilities.GetAttribute(xmlInstanceEffect, "ref", "");
            this.Profile = XmlUtilities.GetAttribute(xmlInstanceEffect, "profile", "");
        }
    }
}
