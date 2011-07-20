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
    public class Material : ColladaBase, IColladaObjectWithId
    {
        // Attributes
        public string Id { get; set; }
        public string Name;

        // Child Objects
        public InstanceEffect InstanceEffect;

        /// <summary>
        /// CTor
        /// </summary>
        public Material(XmlNode xmlMaterial, ColladaDocument ownerDocument)
            : base(ownerDocument)
        {
            // Load the attributes
            // --------------------------

            this.Id = XmlUtilities.GetAttribute(xmlMaterial, "id", "");
            this.Name = XmlUtilities.GetAttribute(xmlMaterial, "name", "");

            // Register this object by its Id
            this.OwnerDocument.RegisterObjectWithId(this);

            // Load the <instance_effect> node
            // ------------------------------

            XmlNode xmlInstanceEffect = xmlMaterial.SelectSingleNode("instance_effect");
            if (xmlInstanceEffect == null)
                throw new ColladaLoadException("[material] node is missing a child [instance_effect] node");

            this.InstanceEffect = new InstanceEffect(xmlInstanceEffect, ownerDocument);

        }
    }

    // ============================================================
    public class InstanceMaterial : ColladaBase
    {
        // Attributes
        public string Sid;
        public string Name;
        public string Target;
        public string Symbol;

        // Child Objects
        List<Bind> Binds = new List<Bind>();
        List<BindVertexInput> BindVertexInputs = new List<BindVertexInput>();

        /// <summary>
        /// CTor
        /// </summary>
        public InstanceMaterial(XmlNode xmlInstanceMaterial, ColladaDocument ownerDocument)
            : base(ownerDocument)
        {
            // Load the attributes
            // --------------------------

            this.Sid = XmlUtilities.GetAttribute(xmlInstanceMaterial, "sid", "");
            this.Name = XmlUtilities.GetAttribute(xmlInstanceMaterial, "name", "");
            this.Target = XmlUtilities.GetAttribute(xmlInstanceMaterial, "target", "");
            this.Symbol = XmlUtilities.GetAttribute(xmlInstanceMaterial, "symbol", "");

            // Load the child elements
            // -------------------------------

            // <bind>
            foreach (XmlNode xmlBind in xmlInstanceMaterial.SelectNodes("bind"))
                this.Binds.Add(new Bind(xmlBind, ownerDocument));

            // <bind_vertex_input>
            foreach (XmlNode xmlBindVertexInput in xmlInstanceMaterial.SelectNodes("bind_vertex_input"))
                this.BindVertexInputs.Add(new BindVertexInput(xmlBindVertexInput, ownerDocument));
        }
    }

    // ============================================================
    public class Bind : ColladaBase
    {
        // Attributes
        public string Semantic;
        public string Target;

        // Child Objects

        /// <summary>
        /// CTor
        /// </summary>
        public Bind(XmlNode xmlBind, ColladaDocument ownerDocument)
            : base(ownerDocument)
        {
            // Load the attributes
            // --------------------------

            this.Semantic = XmlUtilities.GetAttribute(xmlBind, "semantic", "");
            this.Target = XmlUtilities.GetAttribute(xmlBind, "target", "");
        }
    }

    // ============================================================
    public class BindVertexInput : ColladaBase
    {
        // Attributes
        public string Semantic;
        public string InputSemantic;
        public int InputSet;

        // Child Objects

        /// <summary>
        /// CTor
        /// </summary>
        public BindVertexInput(XmlNode xmlBind, ColladaDocument ownerDocument)
            : base(ownerDocument)
        {
            // Load the attributes
            // --------------------------

            this.Semantic = XmlUtilities.GetAttribute(xmlBind, "semantic", "");
            this.InputSemantic = XmlUtilities.GetAttribute(xmlBind, "input_semantic", "");
            this.InputSet = XmlUtilities.GetAttribute(xmlBind, "input_set", -1);
        }
    }

    // ============================================================
    public class BindMaterial : ColladaBase
    {
        // Child Objects
        public List<Param> Params = new List<Param>();
        public List<InstanceMaterial> MaterialInstances = new List<InstanceMaterial>();

        /// <summary>
        /// CTor
        /// </summary>
        public BindMaterial(XmlNode xmlBindMaterial, ColladaDocument ownerDocument)
            : base(ownerDocument)
        {
            // Load the <param> nodes
            foreach (XmlNode xmlParam in xmlBindMaterial.SelectNodes("Param"))
                this.Params.Add(new Param(xmlParam, ownerDocument));

            // Load <instance_material> elements in the common technique
            foreach (XmlNode xmlInstanceMaterial in xmlBindMaterial.SelectNodes("technique_common/instance_material"))
                this.MaterialInstances.Add(new InstanceMaterial(xmlInstanceMaterial, ownerDocument));
        }
    }
}
