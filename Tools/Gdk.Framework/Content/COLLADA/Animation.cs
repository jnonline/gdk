using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml;

namespace Gdk.Content.COLLADA
{
    // ============================================================
    public class Animation : ColladaBase, IColladaObjectWithId
    {
        // Attributes
        public string Id { get; set; }
        public string Name;

        // Child Objects
        public List<Animation> Animations = new List<Animation>();
        public List<Source> Sources = new List<Source>();
        public List<Sampler> Samplers = new List<Sampler>();
        public List<Channel> Channels = new List<Channel>();

        /// <summary>
        /// CTor
        /// </summary>
        public Animation(XmlNode xmlAnimation, ColladaDocument ownerDocument)
            : base(ownerDocument)
        {
            // Load the attributes
            // --------------------------

            this.Id = XmlUtilities.GetAttribute(xmlAnimation, "id", "");
            this.Name = XmlUtilities.GetAttribute(xmlAnimation, "name", "");

            // Register this object by its Id
            this.OwnerDocument.RegisterObjectWithId(this);

            // Load the child elements
            // --------------------------

            // Process any <animation> elements
            foreach (XmlNode xmlChildAnimation in xmlAnimation.SelectNodes("animation"))
                this.Animations.Add(new Animation(xmlChildAnimation, ownerDocument));

            // Process any <source> elements
            foreach (XmlNode xmlSource in xmlAnimation.SelectNodes("source"))
                this.Sources.Add(new Source(xmlSource, ownerDocument));

            // Process any <sampler> elements
            foreach (XmlNode xmlSampler in xmlAnimation.SelectNodes("sampler"))
                this.Samplers.Add(new Sampler(xmlSampler, ownerDocument));

            // Process any <channel> elements
            foreach (XmlNode xmlChannel in xmlAnimation.SelectNodes("channel"))
                this.Channels.Add(new Channel(xmlChannel, ownerDocument));
        }
    }

    // ============================================================
    public class InstanceAnimation : ColladaBase
    {
        // Attributes
        public string Sid;
        public string Name;
        public string Url;

        /// <summary>
        /// CTor
        /// </summary>
        public InstanceAnimation(XmlNode xmlInstanceAnimation, ColladaDocument ownerDocument)
            : base(ownerDocument)
        {
            // Load the attributes
            // --------------------------

            this.Sid = XmlUtilities.GetAttribute(xmlInstanceAnimation, "sid", "");
            this.Name = XmlUtilities.GetAttribute(xmlInstanceAnimation, "name", "");
            this.Url = XmlUtilities.GetAttribute(xmlInstanceAnimation, "url", "");
        }
    }

    // ============================================================
    public class Sampler : ColladaBase, IColladaObjectWithId
    {
        // Attributes
        public string Id { get; set; }

        // Child Objects
        List<Input> Inputs = new List<Input>();

        /// <summary>
        /// CTor
        /// </summary>
        public Sampler(XmlNode xmlSampler, ColladaDocument ownerDocument)
            : base(ownerDocument)
        {
            // Load the attributes
            // --------------------------

            this.Id = XmlUtilities.GetAttribute(xmlSampler, "id", "");

            // Register this object by its Id
            this.OwnerDocument.RegisterObjectWithId(this);

            // Load the child elements
            // --------------------------

            foreach (XmlNode xmlInput in xmlSampler.SelectNodes("input"))
                this.Inputs.Add(new Input(xmlInput, ownerDocument));
        }
    }

    // ============================================================
    public class Channel : ColladaBase
    {
        // Attributes
        public string Source;
        public string Target;

        /// <summary>
        /// CTor
        /// </summary>
        public Channel(XmlNode xmlChannel, ColladaDocument ownerDocument)
            : base(ownerDocument)
        {
            // Load the attributes
            // --------------------------

            this.Source = XmlUtilities.GetAttribute(xmlChannel, "source", "");
            this.Target = XmlUtilities.GetAttribute(xmlChannel, "target", "");
        }
    }

    // ============================================================
    public class AnimationClip : ColladaBase, IColladaObjectWithId
    {
        // Attributes
        public string Id { get; set; }
        public string Name;
        public double Start;
        public double End;

        // Child Objects
        public List<InstanceAnimation> AnimationInstances = new List<InstanceAnimation>();

        /// <summary>
        /// CTor
        /// </summary>
        public AnimationClip(XmlNode xmlAnimationClip, ColladaDocument ownerDocument)
            : base(ownerDocument)
        {
            // Load the attributes
            // --------------------------

            this.Id = XmlUtilities.GetAttribute(xmlAnimationClip, "id", "");
            this.Name = XmlUtilities.GetAttribute(xmlAnimationClip, "name", "");
            this.Start = XmlUtilities.GetAttributeDouble(xmlAnimationClip, "start", 0.0);
            this.End = XmlUtilities.GetAttributeDouble(xmlAnimationClip, "end", 0.0);

            // Register this object by its Id
            this.OwnerDocument.RegisterObjectWithId(this);

            // Load the child elements
            // --------------------------

            // Process the <instance_animation> elements
            foreach (XmlNode xmlInstanceAnimation in xmlAnimationClip.SelectNodes("instance_animation"))
                this.AnimationInstances.Add(new InstanceAnimation(xmlInstanceAnimation, ownerDocument));
        }
    }
}
