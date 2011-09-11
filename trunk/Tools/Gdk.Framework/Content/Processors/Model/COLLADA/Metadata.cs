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
    public enum UpAxisTypes
    {
                    //  Right Axis          Up Axis             In Axis (towards the camera)
                    //  --------------      ---------------     -------------------
        X_UP,       //  Negative Y          Positive X          Positive Z
        Y_UP,       //  Positive X          Positive Y          Positive Z
        Z_UP,       //  Positive X          Positive Z          Negative Y
    }

    // ============================================================
    public class Asset : ColladaBase
    {
        // Child Objects
        public List<Contributor> Contributors = new List<Contributor>();
        public string Created = "";
        public string Keywords = "";
        public string Modified = "";
        public string Revision = "";
        public string Subject = "";
        public string Title = "";

        public string UnitName = "meter";
        public float UnitMeter = 1.0f;
        public UpAxisTypes UpAxis = UpAxisTypes.Y_UP;


        /// <summary>
        /// CTor
        /// </summary>
        public Asset(XmlNode xmlAsset, ColladaDocument ownerDocument)
            : base(ownerDocument)
        {
            // Load the child elements
            // --------------------------

            // <contributor>
            foreach (XmlNode xmlContributor in xmlAsset.SelectNodes("contributor"))
                this.Contributors.Add(new Contributor(xmlContributor, ownerDocument));

            // <created> 
            XmlNode xmlCreated = xmlAsset.SelectSingleNode("created");
            if(xmlCreated != null)
                this.Created = xmlCreated.InnerText.Trim();

            // <keywords> 
            XmlNode xmlKeywords = xmlAsset.SelectSingleNode("keywords");
            if (xmlKeywords != null)
                this.Keywords = xmlKeywords.InnerText.Trim();

            // <modified> 
            XmlNode xmlModified = xmlAsset.SelectSingleNode("modified");
            if (xmlModified != null)
                this.Modified = xmlModified.InnerText.Trim();

            // <revision> 
            XmlNode xmlRevision = xmlAsset.SelectSingleNode("revision");
            if (xmlRevision != null)
                this.Revision = xmlRevision.InnerText.Trim();

            // <subject> 
            XmlNode xmlSubject = xmlAsset.SelectSingleNode("subject");
            if (xmlSubject != null)
                this.Subject = xmlSubject.InnerText.Trim();

            // <title> 
            XmlNode xmlTitle = xmlAsset.SelectSingleNode("title");
            if (xmlTitle != null)
                this.Title = xmlTitle.InnerText.Trim();

            // <unit> 
            XmlNode xmlUnit = xmlAsset.SelectSingleNode("unit");
            if (xmlUnit != null)
            {
                this.UnitName = XmlUtilities.GetAttribute(xmlUnit, "name", this.UnitName); 
                this.UnitMeter = XmlUtilities.GetAttribute(xmlUnit, "meter", this.UnitMeter); 
            }

            // <up_axis>
            XmlNode xmlUpAxis = xmlAsset.SelectSingleNode("up_axis");
            if (xmlUpAxis != null)
            {
                switch (xmlUpAxis.InnerText.Trim().ToUpper())
                {
                    case "X_UP": this.UpAxis = UpAxisTypes.X_UP; break;
                    case "Z_UP": this.UpAxis = UpAxisTypes.Z_UP; break;
                    default: this.UpAxis = UpAxisTypes.Y_UP; break;
                }
            }
            
        }
    }

    // ============================================================
    public class Contributor : ColladaBase
    {
        // Child Objects
        public string Author;
        public string AuthoringTool;
        public string Comments;
        public string Copyright;
        public string SourceData;

        /// <summary>
        /// CTor
        /// </summary>
        public Contributor(XmlNode xmlContributor, ColladaDocument ownerDocument)
            : base(ownerDocument)
        {
            // Load the child elements
            // --------------------------

            // <author>
            XmlNode xmlAuthor = xmlContributor.SelectSingleNode("author");
            if (xmlAuthor != null)
                this.Author = xmlAuthor.InnerText.Trim();

            // <authoring_tool>
            XmlNode xmlAuthoringTool = xmlContributor.SelectSingleNode("authoring_tool");
            if (xmlAuthoringTool != null)
                this.AuthoringTool = xmlAuthoringTool.InnerText.Trim();

            // <comments>
            XmlNode xmlComments = xmlContributor.SelectSingleNode("comments");
            if (xmlComments != null)
                this.Comments = xmlComments.InnerText.Trim();

            // <copyright>
            XmlNode xmlCopyright = xmlContributor.SelectSingleNode("copyright");
            if (xmlCopyright != null)
                this.Copyright = xmlCopyright.InnerText.Trim();

            // <source_data>
            XmlNode xmlSourceData = xmlContributor.SelectSingleNode("source_data");
            if (xmlSourceData != null)
                this.SourceData = xmlSourceData.InnerText.Trim();
        }
    }
}
