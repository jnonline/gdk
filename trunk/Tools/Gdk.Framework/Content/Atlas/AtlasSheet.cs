using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Gdk;

namespace Gdk.Content
{
    class AtlasSheet
    {
        // Content properties
        public int Width;
        public int Height;
        public PixelFormats PixelFormat;
        public List<AtlasImage> Images = new List<AtlasImage>();

        // Runtime properties
        public int Index;
        public Surface Texture;
    }
}
