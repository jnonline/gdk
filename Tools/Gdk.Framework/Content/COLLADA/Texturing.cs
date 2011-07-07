using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml;

namespace Gdk.Content.COLLADA
{
    // ============================================================
    public enum ImageType
    {
        File,
        Data
    }

    // ============================================================
    public class Image : ColladaBase, IColladaObjectWithId
    {
        // Attributes
        public string Id { get; set; }
        public string Name;
        public string Format;
        public int Width;
        public int Height;
        public int Depth;

        public ImageType Type;
        public string FilePath;
        public byte[] Data;

        /// <summary>
        /// CTor
        /// </summary>
        public Image(XmlNode xmlImage, ColladaDocument ownerDocument)
            : base(ownerDocument)
        {
            // Load the attributes
            // --------------------------

            this.Id = XmlUtilities.GetAttribute(xmlImage, "id", "");
            this.Name = XmlUtilities.GetAttribute(xmlImage, "name", "");
            this.Format = XmlUtilities.GetAttribute(xmlImage, "Format", "");
            this.Width = XmlUtilities.GetAttribute(xmlImage, "width", 0);
            this.Height = XmlUtilities.GetAttribute(xmlImage, "height", 0);
            this.Depth = XmlUtilities.GetAttribute(xmlImage, "depth", 1);

            // Register this object by its Id
            this.OwnerDocument.RegisterObjectWithId(this);

            // Load the init* / create* node
            // ------------------------------

            // Note, only one of these nodes will exist
            XmlNode xmlInitFrom = xmlImage.SelectSingleNode("init_from");
            XmlNode xmlData = xmlImage.SelectSingleNode("data");

            // Does this image contain the <init_from> child node
            if (xmlInitFrom != null)
            {
                // This is a file reference type of image
                Type = ImageType.File;
                FilePath = xmlInitFrom.InnerText;
            }
            // Does this image contain the <data> child node
            else if (xmlData != null)
            {
                // This is a actual image
                Type = ImageType.Data;
                throw new NotImplementedException("loading of [image/data] nodes is not currently implemented");
            }
            // Neither of the expected child nodes exist
            else
                throw new ColladaLoadException("[image] node does not contain a child [init_from] or [data] node)");

        }
    }

    // ============================================================
    public class Surface : ValueTypeBase
    {
        // Attributes
        public string Type;

        // Child objects
        public string Format;
        public int MipLevels;
        public bool MipMapGenerate;

        // Init method
        public SurfaceInitMethodBase InitMethod;

        /// <summary>
        /// CTor
        /// </summary>
        public Surface(XmlNode xmlSurface, ColladaDocument ownerDocument)
            : base(xmlSurface, ownerDocument)
        {
            // Load the attributes
            // --------------------------

            this.Type = XmlUtilities.GetAttribute(xmlSurface, "type", "");
            
            // Load the child info elements
            // --------------------------

            // <format>
            XmlNode xmlFormat = xmlSurface.SelectSingleNode("format");
            if (xmlFormat != null)
                this.Format = xmlFormat.InnerText.Trim();

            // <mip_levels>
            XmlNode xmlMipLevels = xmlSurface.SelectSingleNode("mip_levels");
            if (xmlMipLevels != null)
                this.MipLevels = int.Parse(xmlMipLevels.InnerText.Trim());

            // <mipmap_generate>
            XmlNode xmlMipMapGenerate = xmlSurface.SelectSingleNode("mipmap_generate");
            if (xmlMipMapGenerate != null)
                this.MipMapGenerate = bool.Parse(xmlMipMapGenerate.InnerText.Trim());

            // Load the child initialization element
            // ---------------------------------------

            XmlNode xmlInitFrom = xmlSurface.SelectSingleNode("init_from");

            // Process the initialization based on the the init type
            if (xmlInitFrom != null)
                this.InitMethod = new SurfaceInitFrom(xmlInitFrom);

            // No known initialization method
            else
                throw new NotImplementedException("[surface] element does not contain a supported [init_*] element");
        }
    }

    // ============================================================
    public abstract class SurfaceInitMethodBase
    {
    }

    // ============================================================
    public class SurfaceInitFrom : SurfaceInitMethodBase
    {
        // Attributes
        public int Mip;
        public int Slice;
        public string Face;

        public string ImageReference;

        /// <summary>
        /// CTor
        /// </summary>
        public SurfaceInitFrom(XmlNode xmlInitFrom)
        {
            // Load the attributes
            // --------------------------

            this.Mip = XmlUtilities.GetAttribute(xmlInitFrom, "mip", 0);
            this.Slice = XmlUtilities.GetAttribute(xmlInitFrom, "slice", 0);
            this.Face = XmlUtilities.GetAttribute(xmlInitFrom, "face", "POSITIVE_X");

            this.ImageReference = xmlInitFrom.InnerText;
        }
    }

    // ============================================================
    public enum SamplerFilterModes
    {
        NONE,
        NEAREST,
        LINEAR,
        NEAREST_MIPMAP_NEAREST,
        LINEAR_MIPMAP_NEAREST,
        NEAREST_MIPMAP_LINEAR,
        LINEAR_MIPMAP_LINEAR,
    }

    // ============================================================
    public enum SamplerWrapModes
    {
        WRAP,
        MIRROR,
        CLAMP,
        BORDER,
        NONE,
    }

    // ============================================================
    public class Sampler2D : ValueTypeBase
    {
        // Child objects
        public string Source;
        public SamplerWrapModes WrapS;
        public SamplerWrapModes WrapT;
        public SamplerFilterModes MinFilter;
        public SamplerFilterModes MagFilter;
        public SamplerFilterModes MipFilter;
        public int MipMapMaxLevel;
        public float MipMapBias;

        /// <summary>
        /// CTor
        /// </summary>
        public Sampler2D(XmlNode xmlSampler2D, ColladaDocument ownerDocument)
            : base(xmlSampler2D, ownerDocument)
        {
            // Load the child info elements
            // --------------------------

            // <source>
            XmlNode xmlSource = xmlSampler2D.SelectSingleNode("source");
            if (xmlSource != null)
                this.Source = xmlSource.InnerText.Trim();

            // <wrap_s>
            XmlNode xmlWrapS = xmlSampler2D.SelectSingleNode("wrap_s");
            if (xmlWrapS != null)
                this.WrapS = (SamplerWrapModes) Enum.Parse(typeof(SamplerWrapModes), xmlWrapS.InnerText.Trim());

            // <wrap_t>
            XmlNode xmlWrapT = xmlSampler2D.SelectSingleNode("wrap_t");
            if (xmlWrapT != null)
                this.WrapT = (SamplerWrapModes)Enum.Parse(typeof(SamplerWrapModes), xmlWrapT.InnerText.Trim());

            // <minfilter>
            XmlNode xmlMinFilter = xmlSampler2D.SelectSingleNode("minfilter");
            if (xmlMinFilter != null)
                this.MinFilter = (SamplerFilterModes)Enum.Parse(typeof(SamplerFilterModes), xmlMinFilter.InnerText.Trim());

            // <magfilter>
            XmlNode xmlMagFilter = xmlSampler2D.SelectSingleNode("magfilter");
            if (xmlMagFilter != null)
                this.MagFilter = (SamplerFilterModes)Enum.Parse(typeof(SamplerFilterModes), xmlMagFilter.InnerText.Trim());

            // <mipfilter>
            XmlNode xmlMipFilter = xmlSampler2D.SelectSingleNode("mipfilter");
            if (xmlMipFilter != null)
                this.MipFilter = (SamplerFilterModes)Enum.Parse(typeof(SamplerFilterModes), xmlMipFilter.InnerText.Trim());

            // <mipmap_maxlevel>
            XmlNode xmlMipMapMaxLevel = xmlSampler2D.SelectSingleNode("mipmap_maxlevel");
            if (xmlMipMapMaxLevel != null)
                this.MipMapMaxLevel = int.Parse(xmlMipMapMaxLevel.InnerText.Trim());

            // <mipmap_bias>
            XmlNode xmlMipMapBias = xmlSampler2D.SelectSingleNode("mipmap_bias");
            if (xmlMipMapBias != null)
                this.MipMapBias = float.Parse(xmlMipMapBias.InnerText.Trim());
        }
    }
}
