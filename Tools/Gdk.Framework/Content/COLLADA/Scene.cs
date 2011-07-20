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
    public class VisualScene : ColladaBase, IColladaObjectWithId
    {
        // Attributes
        public string Id { get; set; }
        public string Name;

        // Child Objects
        public List<Node> Nodes = new List<Node>();

        /// <summary>
        /// CTor
        /// </summary>
        public VisualScene(XmlNode xmlVisualScene, ColladaDocument ownerDocument)
            : base(ownerDocument)
        {
            // Load the attributes
            // --------------------------

            this.Id = XmlUtilities.GetAttribute(xmlVisualScene, "id", "");
            this.Name = XmlUtilities.GetAttribute(xmlVisualScene, "name", "");

            // Register this object by its Id
            this.OwnerDocument.RegisterObjectWithId(this);

            // Load the child elements
            // --------------------------

            // Process any <node> elements
            foreach(XmlNode xmlNode in xmlVisualScene.SelectNodes("node"))
                this.Nodes.Add(new Node(xmlNode, ownerDocument));

            // Process any <evaluate_scene> elements
            foreach (XmlNode xmlEvaluateScene in xmlVisualScene.SelectNodes("evaluate_scene"))
            {
                // TODO(P3): Not Implemented
            }
        }
    }

    // ============================================================
    public class InstanceVisualScene : ColladaBase
    {
        // Attributes
        public string Sid;
        public string Name;
        public string Url;

        /// <summary>
        /// CTor
        /// </summary>
        public InstanceVisualScene(XmlNode xmlInstanceVisualScene, ColladaDocument ownerDocument)
            : base(ownerDocument)
        {
            // Load the attributes
            // --------------------------

            this.Sid = XmlUtilities.GetAttribute(xmlInstanceVisualScene, "sid", "");
            this.Name = XmlUtilities.GetAttribute(xmlInstanceVisualScene, "name", "");
            this.Url = XmlUtilities.GetAttribute(xmlInstanceVisualScene, "url", "");
        }
    }

    // ============================================================
    public enum NodeType
    {
        NODE,
        JOINT
    }

    // ============================================================
    public class Node : ColladaBase, IColladaObjectWithId
    {
        // Attributes
        public string Id { get; set; }
        public string Name;
        public string Sid;
        public NodeType Type;
        public List<string> Layers = new List<string>();

        // Child Objects
        public List<TransformBase> Transforms = new List<TransformBase>();
        public List<InstanceController> ControllerInstances = new List<InstanceController>();
        public List<InstanceGeometry> GeometryInstances = new List<InstanceGeometry>();
        public List<InstanceNode> NodeInstances = new List<InstanceNode>();
        public List<Node> ChildNodes = new List<Node>();

        /// <summary>
        /// CTor
        /// </summary>
        public Node(XmlNode xmlNode, ColladaDocument ownerDocument)
            : base(ownerDocument)
        {
            // Load the attributes
            // --------------------------

            this.Id = XmlUtilities.GetAttribute(xmlNode, "id", "");
            this.Name = XmlUtilities.GetAttribute(xmlNode, "name", "");
            this.Sid = XmlUtilities.GetAttribute(xmlNode, "sid", "");
            this.Type = XmlUtilities.GetAttributeEnumValue<NodeType>(xmlNode, "type", NodeType.NODE);
            this.Layers = XmlUtilities.GetAttribute(xmlNode, "layer", "").Trim().Split(' ').ToList();

            // Register this object by its Id
            this.OwnerDocument.RegisterObjectWithId(this);

            // Load the child elements
            // ----------------------------------

            // Load the transforms
            this.Transforms = TransformBase.ParseTransforms(xmlNode.ChildNodes, ownerDocument);

            // Load the controller instances
            foreach (XmlNode xmlInstanceController in xmlNode.SelectNodes("instance_controller"))
                this.ControllerInstances.Add(new InstanceController(xmlInstanceController, ownerDocument));

            // Load the geometry instances
            foreach (XmlNode xmlInstanceGeometry in xmlNode.SelectNodes("instance_geometry"))
                this.GeometryInstances.Add(new InstanceGeometry(xmlInstanceGeometry, ownerDocument));

            // Load the node instances
            foreach (XmlNode xmlInstanceNode in xmlNode.SelectNodes("instance_node"))
                this.NodeInstances.Add(new InstanceNode(xmlInstanceNode, ownerDocument));

            // Load the child nodes
            foreach (XmlNode xmlChildNode in xmlNode.SelectNodes("node"))
                this.ChildNodes.Add(new Node(xmlChildNode, ownerDocument));
        }
    }

    // ============================================================
    public class InstanceNode : ColladaBase
    {
        // Attributes
        public string Sid;
        public string Name;
        public string Url;

        /// <summary>
        /// CTor
        /// </summary>
        public InstanceNode(XmlNode xmlInstanceNode, ColladaDocument ownerDocument)
            : base(ownerDocument)
        {
            // Load the attributes
            // --------------------------

            this.Sid = XmlUtilities.GetAttribute(xmlInstanceNode, "sid", "");
            this.Name = XmlUtilities.GetAttribute(xmlInstanceNode, "name", "");
            this.Url = XmlUtilities.GetAttribute(xmlInstanceNode, "url", "");
        }

    }

    // ============================================================
    public class Scene : ColladaBase
    {
        // Child Objects
        public InstanceVisualScene VisualSceneInstance;

        /// <summary>
        /// CTor
        /// </summary>
        public Scene(XmlNode xmlScene, ColladaDocument ownerDocument)
            : base(ownerDocument)
        {
            // Load the <instance_visual_scene> element
            XmlNode xmlInstanceVisualScene = xmlScene.SelectSingleNode("instance_visual_scene");
            if(xmlInstanceVisualScene != null)
                this.VisualSceneInstance = new InstanceVisualScene(xmlInstanceVisualScene, ownerDocument);
        }
    }
}
