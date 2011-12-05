/*
  Copyright (c) 2011, Raincity Games LLC
  Licensed under the MIT license: http://www.opensource.org/licenses/mit_license.php
*/

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Gdk.Framework;

namespace Gdk.Framework.Content
{
    class Atlas
    {
        // Content Properties - get saved into the built atlas
        // ---------------------------------------------------
        // Name of the atlas
        public string Name;

        // Sheets & Animations
        public List<AtlasSheet> Sheets = new List<AtlasSheet>();
        public List<AtlasImage> Images = new List<AtlasImage>();
        public List<AtlasAnimation> Animations = new List<AtlasAnimation>();

        // Runtime Properties - only used during build time
        // --------------------------------------------------

        // Path of the atlas file being built
        public string ContentFolder;

        // Runtime properties
        internal List<string> UniqueNames = new List<string>();
        internal Dictionary<string, AtlasImage> ImagesByName = new Dictionary<string, AtlasImage>();
        internal Dictionary<PixelFormats, List<AtlasImage>> ImagesByPixelFormat = new Dictionary<PixelFormats, List<AtlasImage>>();
    }
}
