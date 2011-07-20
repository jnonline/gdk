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
    public class Geometry : ColladaBase, IColladaObjectWithId
    {
        // Attributes
        public string Id { get; set; }  
        public string Name;

        // Child Objects
        public Mesh Mesh;

        /// <summary>
        /// CTor - from a <geometry/> node
        /// </summary>
        public Geometry(XmlNode xmlGeometry, ColladaDocument ownerDocument)
            : base(ownerDocument)
        {
            // Load the attributes
            // --------------------------

            this.Id = XmlUtilities.GetAttribute(xmlGeometry, "id", "");
            this.Name = XmlUtilities.GetAttribute(xmlGeometry, "name", "");

            // Register this object by its Id
            this.OwnerDocument.RegisterObjectWithId(this);

            // Load the child nodes
            // --------------------------
            
            // Process a <mesh> node
            XmlNode xmlMesh = xmlGeometry.SelectSingleNode("mesh");
            if(xmlMesh != null)
                this.Mesh = new Mesh(xmlMesh, ownerDocument);

            // Unsupported child nodes
            //      convex_mesh
            //      spline
        }
    }

    // ============================================================
    public class InstanceGeometry : ColladaBase
    {
        // Attributes
        public string Sid;
        public string Name;
        public string Url;

        // Child Objects
        public BindMaterial BindMaterial;

        /// <summary>
        /// CTor
        /// </summary>
        public InstanceGeometry(XmlNode xmlInstanceGeometry, ColladaDocument ownerDocument)
            : base(ownerDocument)
        {
            // Load the attributes
            // --------------------------

            this.Sid = XmlUtilities.GetAttribute(xmlInstanceGeometry, "sid", "");
            this.Name = XmlUtilities.GetAttribute(xmlInstanceGeometry, "name", "");
            this.Url = XmlUtilities.GetAttribute(xmlInstanceGeometry, "url", "");

            // Load the child elements
            // -------------------------------

            // <bind_material>
            XmlNode xmlBindMaterial = xmlInstanceGeometry.SelectSingleNode("bind_material");
            if (xmlBindMaterial != null)
                this.BindMaterial = new BindMaterial(xmlBindMaterial, ownerDocument);
        }
    }

    // ============================================================
    public class Mesh : ColladaBase
    {
        // Child Objects
        public Dictionary<string, Source> Sources = new Dictionary<string, Source>();
        public Vertices Vertices;

        public List<Triangles> TrianglesSets = new List<Triangles>();
        public List<TriStrips> TriStripsSets = new List<TriStrips>();
        public List<TriFans> TriFansSets = new List<TriFans>();
        public List<PolyList> PolyListSets = new List<PolyList>();

        /// <summary>
        /// CTor - from a <mesh/> node
        /// </summary>
        public Mesh(XmlNode xmlMesh, ColladaDocument ownerDocument)
            : base(ownerDocument)
        {
            // Load the <source> child elements
            // --------------------------

            // Loop through the <source> child elements
            foreach(XmlNode xmlSource in xmlMesh.SelectNodes("source"))
            {
                // Load this source & put it in our dictionary
                Source source = new Source(xmlSource, ownerDocument);
                this.Sources.Add(source.Id, source);
            }

            // Verify we got at least 1 source
            if (this.Sources.Count == 0)
                throw new ColladaLoadException("[mesh] element is missing at least one child [source] element");

            // Load the <vertices> child element
            // ---------------------------------

            // Load the vertices from it's xml element
            XmlNode xmlVertices = xmlMesh.SelectSingleNode("vertices");
            if (xmlVertices == null)
                throw new ColladaLoadException("[mesh] element is missing a child [vertices] element");
            this.Vertices = new Vertices(xmlVertices, ownerDocument);

            // Load all the primitive child elements
            // -------------------------------------

            // Triangles
            foreach (XmlNode xmlTriangles in xmlMesh.SelectNodes("triangles"))
                this.TrianglesSets.Add(new Triangles(xmlTriangles, ownerDocument));

            // TriStrips
            foreach (XmlNode xmlTriStrips in xmlMesh.SelectNodes("tristrips"))
                this.TriStripsSets.Add(new TriStrips(xmlTriStrips, ownerDocument));

            // TriFans
            foreach (XmlNode xmlTriFans in xmlMesh.SelectNodes("trifans"))
                this.TriFansSets.Add(new TriFans(xmlTriFans, ownerDocument));

            // PolyList
            foreach (XmlNode xmlPolyList in xmlMesh.SelectNodes("polylist"))
                this.PolyListSets.Add(new PolyList(xmlPolyList, ownerDocument));

            // Unsupported child nodes
            //      lines
            //      linestrips
            //      polygons
        }
    }
    
    // ============================================================
    public class Vertices : ColladaBase, IColladaObjectWithId
    {
        // Attributes
        public string Id { get; set; }  
        public string Name;

        // Child Objects
        public List<Input> Inputs = new List<Input>();
        public Input PositionInput;

        /// <summary>
        /// CTor - from a <source/> node
        /// </summary>
        public Vertices(XmlNode xmlVertices, ColladaDocument ownerDocument)
            : base(ownerDocument)
        {
            // Load the attributes
            // --------------------------

            this.Id = XmlUtilities.GetAttribute(xmlVertices, "id", "");
            this.Name = XmlUtilities.GetAttribute(xmlVertices, "name", "");

            // Register this object by its Id
            this.OwnerDocument.RegisterObjectWithId(this);

            // Load the <input> child nodes
            // --------------------------

            // Loop through the <input> nodes
            foreach (XmlNode xmlInput in xmlVertices.SelectNodes("input"))
            {
                Input input = new Input(xmlInput, ownerDocument);
                this.Inputs.Add(input);
            }

            // Verify we have at least 1 input
            if (this.Inputs.Count == 0)
                throw new ColladaLoadException("[vertices] element must have at least one child [input] element");
            
            // Verify we have 1 input with the "POSITION" semantic
            foreach (Input input in this.Inputs)
                if (input.Semantic == "POSITION")
                    this.PositionInput = input;
            if (this.PositionInput == null)
                throw new ColladaLoadException("[vertices] element has no child [input] elements with the 'POSITION' semantic");
        }
    }

    // ============================================================
    public class IndexSet : List<int>
    {
        public IndexSet() { }
        public IndexSet(List<int> source)
        {
            // Copy the source
            foreach (int value in source)
                this.Add(value);
        }
    }

    // ============================================================
    public abstract class PrimitiveSetBase : ColladaBase
    {
        // Attributes
        public int Count;
        public string Material;
        public string Name;

        // Child Objects
        public List<SharedInput> Inputs = new List<SharedInput>();
        public Input VertexInput;
        public List<IndexSet> PrimitiveIndexSets = new List<IndexSet>();

        /// <summary>
        /// CTor
        /// </summary>
        public PrimitiveSetBase(XmlNode xmlData, ColladaDocument ownerDocument)
            : base(ownerDocument)
        {
            // Load the attributes
            // --------------------------

            // Verify the required attributes exists
            if (XmlUtilities.VerifyAttributesExist(xmlData, false, "count") == false)
                throw new ColladaLoadException("[" + xmlData.Name + "] element is missing the [count] attribute");

            this.Count = XmlUtilities.GetAttribute(xmlData, "count", 0);
            this.Material = XmlUtilities.GetAttribute(xmlData, "material", "");
            this.Name = XmlUtilities.GetAttribute(xmlData, "name", "");

            // Load the <input> child nodes (optional)
            // --------------------------

            // Loop through the <input> nodes
            foreach (XmlNode xmlInput in xmlData.SelectNodes("input"))
            {
                SharedInput input = new SharedInput(xmlInput, ownerDocument);
                this.Inputs.Add(input);
            }

            // Find the "VERTEX" semantic input
            foreach (SharedInput input in this.Inputs)
                if (input.Semantic == "VERTEX")
                    this.VertexInput = input;

            // Load the <p> child nodes (optional)
            // --------------------------

            // Loop through the <p> child elements
            foreach (XmlNode xmlP in xmlData.SelectNodes("p"))
            {
                // Turn the string of space delimited indices into an int list
                List<int> listOfInts = StringUtilities.ParseInts(xmlP.InnerText.Trim());
                IndexSet indexSet = new IndexSet(listOfInts);
                
                // Add this index set to our list
                this.PrimitiveIndexSets.Add(indexSet);
            }
        }
    }

    // ============================================================
    public class Triangles : PrimitiveSetBase
    {

        /// <summary>
        /// CTor
        /// </summary>
        public Triangles(XmlNode xmlTriangles, ColladaDocument ownerDocument)
            : base(xmlTriangles, ownerDocument)
        {
        }
    }

    // ============================================================
    public class TriStrips : PrimitiveSetBase
    {

        /// <summary>
        /// CTor
        /// </summary>
        public TriStrips(XmlNode xmlTriStrips, ColladaDocument ownerDocument)
            : base(xmlTriStrips, ownerDocument)
        {
        }
    }

    // ============================================================
    public class TriFans : PrimitiveSetBase
    {

        /// <summary>
        /// CTor
        /// </summary>
        public TriFans(XmlNode xmlTriFans, ColladaDocument ownerDocument)
            : base(xmlTriFans, ownerDocument)
        {
        }
    }

    // ============================================================
    public class PolyList : PrimitiveSetBase
    {
        // Child Objects
        public List<int> VCounts = new List<int>();

        /// <summary>
        /// CTor
        /// </summary>
        public PolyList(XmlNode xmlPolylist, ColladaDocument ownerDocument)
            : base(xmlPolylist, ownerDocument)
        {
            // Load the <vcount> child node
            // --------------------------

            // Get the vcount node
            XmlNode xmlVCount = xmlPolylist.SelectSingleNode("vcount");

            // Turn the string of space delimited indices into an int list
            this.VCounts = StringUtilities.ParseInts(xmlVCount.InnerText.Trim());
        }
    }
}
