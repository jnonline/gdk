/*
  Copyright (c) 2011, Raincity Games LLC
  Licensed under the MIT license: http://www.opensource.org/licenses/mit_license.php
*/

using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Xml;

namespace Gdk.Framework.Content.COLLADA
{
    // Notes:
    //      This loader only loads the COMMON profile & techniques
    //

    // ============================================================
    public class ColladaDocument
    {
        // Attributes
        // ----------------------------------------------------
        public string Version;
        
        // Child objects
        // ----------------------------------------------------

        public Asset Asset;
        public Scene Scene;

        // Libraries
        public Dictionary<string, Geometry> Geometries = new Dictionary<string, Geometry>();
        public Dictionary<string, Image> Images = new Dictionary<string, Image>();
        public Dictionary<string, Material> Materials = new Dictionary<string, Material>();
        public Dictionary<string, Effect> Effects = new Dictionary<string, Effect>();
        public Dictionary<string, VisualScene> VisualScenes = new Dictionary<string, VisualScene>();
        public Dictionary<string, Animation> Animations = new Dictionary<string, Animation>();
        public Dictionary<string, AnimationClip> AnimationClips = new Dictionary<string, AnimationClip>();
        public Dictionary<string, Controller> Controllers = new Dictionary<string, Controller>();
        public Dictionary<string, Node> Nodes = new Dictionary<string, Node>();

        // Object / Id lookup table
        private Dictionary<string, IColladaObjectWithId> objectsById = new Dictionary<string, IColladaObjectWithId>();
        public Dictionary<string, IColladaObjectWithId> ObjectsById { get { return objectsById; } }

        // Methods
        // ----------------------------------------------------

        /// <summary>
        /// CTor from a file path
        /// </summary>
        /// <param name="path"></param>
        public ColladaDocument(string path)
        {
            // Read in the collada source file
            string xml = System.IO.File.ReadAllText(path);

            // Rip the collada xmlns out of the xml
            xml = xml.Replace("xmlns=\"http://www.collada.org/2005/11/COLLADASchema\"", "");

            // Load the collada xml
            XmlDocument xmlDoc = new XmlDocument();
            xmlDoc.LoadXml(xml);

            // Load from the xml
            LoadFromXml(xmlDoc);
        }

        /// <summary>
        /// Loads a collada blob into this DOM
        /// </summary>
        /// <param name="stream"></param>
        private void LoadFromXml(XmlDocument xmlDoc)
        {
            XmlNode xmlCollada = xmlDoc.DocumentElement;

            // Get the version
            this.Version = xmlCollada.Attributes["version"].Value;

            // Verify the version is 1.4
            if (this.Version.StartsWith("1.4") == false)
                throw new ColladaLoadException("Invalid collada version [" + this.Version + "].  Only 1.4 is supported by this document object");

            // Load the libraries
            // ==============================

            // root <asset>
            // -----------------------
            XmlNode xmlAsset = xmlCollada.SelectSingleNode("asset");
            if (xmlAsset == null)
                throw new ColladaLoadException("The root [COLLADA/asset] node is missing, unable to determine the asset information");
            this.Asset = new Asset(xmlAsset, this);
            
            // library_geometries
            // -----------------------

            // Loop through the <library_geometries> elements
            foreach (XmlNode xmlLibraryGeometries in xmlCollada.SelectNodes("library_geometries"))
            {
                // Loop through <geometry> elements
                foreach (XmlNode xmlGeometry in xmlLibraryGeometries.SelectNodes("geometry"))
                {
                    // Load this geometry
                    Geometry geometry = new Geometry(xmlGeometry, this);

                    // Add the geometry to our dictionary
                    this.Geometries.Add(geometry.Id, geometry);
                }
            }

            // library_images
            // -----------------------

            // Loop through the <library_images> elements
            foreach (XmlNode xmlLibraryImages in xmlCollada.SelectNodes("library_images"))
            {
                // Loop through <image> elements
                foreach (XmlNode xmlImage in xmlLibraryImages.SelectNodes("image"))
                {
                    // Load this image
                    Image image = new Image(xmlImage, this);

                    // Add the image to our dictionary
                    this.Images.Add(image.Id, image);
                }
            }

            // library_materials
            // -----------------------

            // Loop through the <library_materials> elements
            foreach (XmlNode xmlLibraryMaterials in xmlCollada.SelectNodes("library_materials"))
            {
                // Loop through <material> elements
                foreach (XmlNode xmlMaterial in xmlLibraryMaterials.SelectNodes("material"))
                {
                    // Load this material
                    Material material = new Material(xmlMaterial, this);

                    // Add the material to our dictionary
                    this.Materials.Add(material.Id, material);
                }
            }

            // library_effects
            // -----------------------

            // Loop through the <library_effects> elements
            foreach (XmlNode xmlLibraryEffects in xmlCollada.SelectNodes("library_effects"))
            {
                // Loop through <effect> elements
                foreach (XmlNode xmlEffect in xmlLibraryEffects.SelectNodes("effect"))
                {
                    // Load this effect
                    Effect effect = new Effect(xmlEffect, this);

                    // Add the effect to our dictionary
                    this.Effects.Add(effect.Id, effect);
                }
            }

            // library_visual_scenes
            // -----------------------

            // Loop through the <library_visual_scenes> elements
            foreach (XmlNode xmlLibraryVisualScenes in xmlCollada.SelectNodes("library_visual_scenes"))
            {
                // Loop through <visual_scene> elements
                foreach (XmlNode xmlVisualScene in xmlLibraryVisualScenes.SelectNodes("visual_scene"))
                {
                    // Load this visual_scene
                    VisualScene visualScene = new VisualScene(xmlVisualScene, this);

                    // Add the visual_scene to our dictionary
                    this.VisualScenes.Add(visualScene.Id, visualScene);
                }
            }

            // library_animations
            // -----------------------

            // Loop through the <library_animations> elements
            foreach (XmlNode xmlLibraryAnimations in xmlCollada.SelectNodes("library_animations"))
            {
                // Loop through <visual_scene> elements
                foreach (XmlNode xmlAnimation in xmlLibraryAnimations.SelectNodes("animation"))
                {
                    // Load this animation
                    Animation animation = new Animation(xmlAnimation, this);

                    // Add the animation to our dictionary
                    this.Animations.Add(animation.Id, animation);
                }
            }


            // library_animation_clips
            // -----------------------

            // Loop through the <library_animation_clips> elements
            foreach (XmlNode xmlLibraryAnimationClips in xmlCollada.SelectNodes("library_animation_clips"))
            {
                // Loop through <visual_scene> elements
                foreach (XmlNode xmlAnimationClip in xmlLibraryAnimationClips.SelectNodes("animation_clip"))
                {
                    // Load this animation clip
                    AnimationClip animationClip = new AnimationClip(xmlAnimationClip, this);

                    // Add the animation clip to our dictionary
                    this.AnimationClips.Add(animationClip.Id, animationClip);
                }
            }

            // library_controllers
            // -----------------------

            // Loop through the <library_controllers> elements
            foreach (XmlNode xmlLibraryControllers in xmlCollada.SelectNodes("library_controllers"))
            {
                // Loop through <controller> elements
                foreach (XmlNode xmlController in xmlLibraryControllers.SelectNodes("controller"))
                {
                    // Load this controller
                    Controller controller = new Controller(xmlController, this);

                    // Add the controller to our dictionary
                    this.Controllers.Add(controller.Id, controller);
                }
            }

            // library_nodes
            // -----------------------

            // Loop through the <library_nodes> elements
            foreach (XmlNode xmlLibraryNodes in xmlCollada.SelectNodes("library_nodes"))
            {
                // Loop through <node> elements
                foreach (XmlNode xmlNode in xmlLibraryNodes.SelectNodes("node"))
                {
                    // Load this node
                    Node node = new Node(xmlNode, this);

                    // Add the node to our dictionary
                    this.Nodes.Add(node.Id, node);
                }
            }

            // <scene>
            // -----------------------

            XmlNode xmlScene = xmlCollada.SelectSingleNode("scene");
            if (xmlScene != null)
                this.Scene = new Scene(xmlScene, this);
        }

        /// <summary>
        /// Registers an object with an Id to the collada document's Id lookup table
        /// </summary>
        /// <param name="objectWithId"></param>
        public void RegisterObjectWithId(IColladaObjectWithId objectWithId)
        {
            // Get the Id & check that it is valid
            string id = objectWithId.Id;
            if (string.IsNullOrEmpty(id) == false)
                this.objectsById.Add(id, objectWithId);
        }

        /// <summary>
        /// Gets the registered collada object in this document, with the given id
        /// </summary>
        public IColladaObjectWithId GetObjectByUrl(string url)
        {
            // Does the id/url have a # (document-local) prefix
            if(url.StartsWith("#"))
            {
                // Strip the # prefix off the url
                string id = url.Substring(1);
                    
                // Get the object
                return ObjectsById[id];
            }
            else
            {
                throw new NotImplementedException("The collada source has an external url reference, which is not supported: [Url: " + url + "]");
            }
        }
    }

    // ============================================================
    public interface IColladaObjectWithId
    {
        string Id { get; set; }
    }

    // ============================================================
    public abstract class ColladaBase
    {
        /// <summary>
        /// Owner ColladaDocument that this object belongs to
        /// </summary>
        public ColladaDocument OwnerDocument { get { return ownerDocument; } }
        private ColladaDocument ownerDocument;

        /// <summary>
        /// CTor
        /// </summary>
        public ColladaBase(ColladaDocument ownerDocument)
        {
            this.ownerDocument = ownerDocument;
        }
    }

    // ============================================================
    public class ColladaLoadException : ApplicationException
    {
        public ColladaLoadException()
            : base()
        { }

        public ColladaLoadException(string message)
            : base(message)
        { }

        public ColladaLoadException(string message, Exception innerException)
            : base(message, innerException)
        { }

    }
}
