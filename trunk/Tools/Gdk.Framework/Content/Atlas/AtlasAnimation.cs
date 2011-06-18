using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Gdk;

namespace Gdk.Content
{
    class AtlasAnimation
    {
        // Content Properties
        public string Name;
        public float FPS;
        public List<AtlasImage> Images = new List<AtlasImage>();
    }
}
