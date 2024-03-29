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
