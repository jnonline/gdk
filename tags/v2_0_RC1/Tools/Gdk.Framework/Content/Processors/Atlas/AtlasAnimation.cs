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
    class AtlasAnimation
    {
        // Content Properties
        public string Name;
        public float FPS;
        public List<AtlasImage> Images = new List<AtlasImage>();
    }
}
