using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml;

namespace Gdk.Content.COLLADA
{
    // ============================================================
    public enum ControllerType
    {
        Skin,
        Morph
    }

    // ============================================================
    public class Controller : ColladaBase, IColladaObjectWithId
    {
        // Attributes
        public string Id { get; set; }
        public string Name;

        // Child Objects
        public ControllerType Type;
        public Skin Skin;
        public Morph Morph;

        /// <summary>
        /// CTor
        /// </summary>
        public Controller(XmlNode xmlController, ColladaDocument ownerDocument)
            : base(ownerDocument)
        {
            // Load the attributes
            // --------------------------

            this.Id = XmlUtilities.GetAttribute(xmlController, "id", "");
            this.Name = XmlUtilities.GetAttribute(xmlController, "name", "");

            // Register this object by its Id
            this.OwnerDocument.RegisterObjectWithId(this);

            // Load the child elements
            // --------------------------

            XmlNode xmlSkin = xmlController.SelectSingleNode("skin");
            XmlNode xmlMorph = xmlController.SelectSingleNode("morph");

            // Select & Handle the controller type based on the main child element
            if (xmlSkin != null)
            {
                this.Type = ControllerType.Skin;
                this.Skin = new Skin(xmlSkin, ownerDocument);
            }
            else if (xmlMorph != null)
            {
                this.Type = ControllerType.Morph;
                this.Morph = new Morph(xmlMorph, ownerDocument);
            }
            else
                throw new ColladaLoadException("[controller] node is missing a child [skin] or [morph] element");
        }
    }

    // ============================================================
    public class InstanceController : ColladaBase
    {
        // Attributes
        public string Sid;
        public string Name;
        public string Url;

        // Child Objects
        public List<Skeleton> Skeletons = new List<Skeleton>();
        public BindMaterial BindMaterial;

        /// <summary>
        /// CTor
        /// </summary>
        public InstanceController(XmlNode xmlInstanceController, ColladaDocument ownerDocument)
            : base(ownerDocument)
        {
            // Load the attributes
            // --------------------------

            this.Sid = XmlUtilities.GetAttribute(xmlInstanceController, "sid", "");
            this.Name = XmlUtilities.GetAttribute(xmlInstanceController, "name", "");
            this.Url = XmlUtilities.GetAttribute(xmlInstanceController, "url", "");

            // Load the child elements
            // -------------------------------

            // <skeleton>
            foreach (XmlNode xmlSkeleton in xmlInstanceController.SelectNodes("skeleton"))
                this.Skeletons.Add(new Skeleton(xmlSkeleton, ownerDocument));

            // <bind_material>
            XmlNode xmlBindMaterial = xmlInstanceController.SelectSingleNode("bind_material");
            if (xmlBindMaterial != null)
                this.BindMaterial = new BindMaterial(xmlBindMaterial, ownerDocument);
        }
    }

    // ============================================================
    public class Skin : ColladaBase
    {
        // Attributes
        public string Source;

        // Child Objects
        public Gdk.Matrix BindShapeMatrix;
        public List<Source> Sources = new List<Source>();
        public Joints Joints;
        public VertexWeights VertexWeights;

        /// <summary>
        /// CTor
        /// </summary>
        public Skin(XmlNode xmlSkin, ColladaDocument ownerDocument)
            : base(ownerDocument)
        {
            // Load the attributes
            // --------------------------

            this.Source = XmlUtilities.GetAttribute(xmlSkin, "source", "");

            // Load the child elements
            // --------------------------

            // <bind_shape_matrix>
            XmlNode xmlBindShapeMatrix = xmlSkin.SelectSingleNode("bind_shape_matrix");
            if (xmlBindShapeMatrix != null)
            {
                // Get the float values & build a matrix from them
                List<float> values = StringUtilities.ParseFloats(xmlBindShapeMatrix.InnerText);
                this.BindShapeMatrix = new Gdk.Matrix(
                    values[0], values[4], values[8], values[12],
                    values[1], values[5], values[9], values[13],
                    values[2], values[6], values[10], values[14],
                    values[3], values[7], values[11], values[15]
                    );
            }
            else
                this.BindShapeMatrix = Gdk.Matrix.Identity;

            // <source>
            foreach (XmlNode xmlSource in xmlSkin.SelectNodes("source"))
                this.Sources.Add(new Source(xmlSource, ownerDocument));

            // <joint>
            XmlNode xmlJoints = xmlSkin.SelectSingleNode("joints");
            if (xmlJoints == null)
                throw new ColladaLoadException("[skin] element is missing a child [joints] element");
            this.Joints = new Joints(xmlJoints, ownerDocument);

            // <vertex_weights>
            XmlNode xmlVertexWeights = xmlSkin.SelectSingleNode("vertex_weights");
            if (xmlVertexWeights == null)
                throw new ColladaLoadException("[skin] element is missing a child [vertex_weights] element");
            this.VertexWeights = new VertexWeights(xmlVertexWeights, ownerDocument);
        }
    }

    // ============================================================
    public class Joints : ColladaBase
    {

        // Child Objects
        public List<Input> Inputs = new List<Input>();

        /// <summary>
        /// CTor
        /// </summary>
        public Joints(XmlNode xmlJoints, ColladaDocument ownerDocument)
            : base(ownerDocument)
        {
            // Parse the <input> child elements
            foreach (XmlNode xmlInput in xmlJoints.SelectNodes("input"))
                this.Inputs.Add(new Input(xmlInput, ownerDocument));

            // Verify we have at least 2 input
            if (this.Inputs.Count < 2)
                throw new ColladaLoadException("[joints] element does not have the minimum of two child [input] elements");

            // Verify we have a JOINT semantic input
            bool jointSemanticFound = false;
            foreach (Input input in this.Inputs)
                if (input.Semantic == "JOINT")
                    jointSemanticFound = true;
            if (jointSemanticFound == false)
                throw new ColladaLoadException("[joints] element does not have a child [input semantic=\"JOINT\"] element");
        }
    }

    // ============================================================
    public class VertexWeights : ColladaBase
    {
        // Attributes
        public int Count;

        // Child Objects
        public List<SharedInput> Inputs = new List<SharedInput>();
        public List<int> VCount = new List<int>();
        public List<int> V = new List<int>();

        /// <summary>
        /// CTor
        /// </summary>
        public VertexWeights(XmlNode xmlVertexWeights, ColladaDocument ownerDocument)
            : base(ownerDocument)
        {
            // Load the attributes
            // --------------------------

            this.Count = XmlUtilities.GetAttribute(xmlVertexWeights, "count", 0);

            // Load the <input> elements
            // --------------------------

            // Parse the <input> child elements
            foreach (XmlNode xmlInput in xmlVertexWeights.SelectNodes("input"))
                this.Inputs.Add(new SharedInput(xmlInput, ownerDocument));

            // Verify we have at least 2 input
            if (this.Inputs.Count < 2)
                throw new ColladaLoadException("[vertex_weights] element does not have the minimum of two child [input] elements");

            // Verify we have a JOINT semantic input
            bool jointSemanticFound = false;
            foreach (Input input in this.Inputs)
                if (input.Semantic == "JOINT")
                    jointSemanticFound = true;
            if (jointSemanticFound == false)
                throw new ColladaLoadException("[vertex_weights] element does not have a child [input semantic=\"JOINT\"] element");

            // Load the <vcount> & <v> elements
            // --------------------------

            // <vcount>
            XmlNode xmlVCount = xmlVertexWeights.SelectSingleNode("vcount");
            if (xmlVCount != null)
                this.VCount = StringUtilities.ParseInts(xmlVCount.InnerText.Trim());

            // <v>
            XmlNode xmlV = xmlVertexWeights.SelectSingleNode("v");
            if (xmlV != null)
                this.V = StringUtilities.ParseInts(xmlV.InnerText.Trim());

        }
    }

    // ============================================================
    public enum MorphMethods
    {
        NORMALIZED,
        RELATIVE
    }

    // ============================================================
    public class Morph : ColladaBase
    {
        // Attributes
        public string Sid;
        public MorphMethods Method;

        // Child Objects
        public List<Source> Sources = new List<Source>();
        public Targets Targets;

        /// <summary>
        /// CTor
        /// </summary>
        public Morph(XmlNode xmlMorph, ColladaDocument ownerDocument)
            : base(ownerDocument)
        {
            // Load the attributes
            // --------------------------

            this.Sid = XmlUtilities.GetAttribute(xmlMorph, "sid", "");
            this.Method = XmlUtilities.GetAttributeEnumValue<MorphMethods>(xmlMorph, "method", MorphMethods.NORMALIZED);

            // Load the child elements
            // --------------------------

            // <source>
            foreach (XmlNode xmlSource in xmlMorph.SelectNodes("source"))
                this.Sources.Add(new Source(xmlSource, ownerDocument));

            
            // <targets>
            XmlNode xmlTargets = xmlMorph.SelectSingleNode("vertex_weights");
            if (xmlTargets == null)
                throw new ColladaLoadException("[morph] element is missing a child [targets] element");
            this.Targets = new Targets(xmlTargets, ownerDocument);
        }
    }

    // ============================================================
    public class Targets : ColladaBase
    {

        // Child Objects
        List<Input> Inputs = new List<Input>();

        /// <summary>
        /// CTor
        /// </summary>
        public Targets(XmlNode xmlTargets, ColladaDocument ownerDocument)
            : base(ownerDocument)
        {
            // Parse the <input> child elements
            foreach (XmlNode xmlInput in xmlTargets.SelectNodes("input"))
                this.Inputs.Add(new Input(xmlInput, ownerDocument));

            // Verify we have at least 2 input
            if (this.Inputs.Count < 2)
                throw new ColladaLoadException("[joints] element does not have the minimum of two child [input] elements");

            // Verify we have MORPH_TARGET & MORPH_WEIGHT semantic inputs
            bool morphTargetFound = false;
            bool morphWeightFound = false;
            foreach (Input input in this.Inputs)
            {
                if (input.Semantic == "MORPH_TARGET")
                    morphTargetFound = true;
                else if (input.Semantic == "MORPH_WEIGHT")
                    morphWeightFound = true;
            }
            if (morphTargetFound == false)
                throw new ColladaLoadException("[target] element does not have a child [input semantic=\"MORPH_TARGET\"] element");
            if (morphWeightFound == false)
                throw new ColladaLoadException("[target] element does not have a child [input semantic=\"MORPH_WEIGHT\"] element");
        }
    }

    // ============================================================
    public class Skeleton : ColladaBase
    {
        // Child Objects
        public string URI;

        /// <summary>
        /// CTor
        /// </summary>
        public Skeleton(XmlNode xmlSkeleton, ColladaDocument ownerDocument)
            : base(ownerDocument)
        {
            this.URI = xmlSkeleton.InnerText.Trim();
        }
    }
}
