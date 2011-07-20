/*
  Copyright (c) 2011, Raincity Games LLC
  Licensed under the MIT license: http://www.opensource.org/licenses/mit_license.php
*/

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Gdk;

namespace Gdk.Content
{
    class AtlasImage
    {
        // Content Properties
        public string Name;
        public int Width;
        public int Height;
        public int CroppedWidth;
        public int CroppedHeight;
        public int CroppedImageOffsetX;
        public int CroppedImageOffsetY;
        public Vector2 TopLeftTexCoord;
        public Vector2 BottomRightTexCoord;
        

        // Normalized in Image space
        public Vector2 ReferencePoint;
        public List<Vector2> ChildPoints = new List<Vector2>();

        // Runtime properties
        public AtlasSheet Sheet;
        internal int Index;
        internal Surface Texture;
        internal Vector2 SheetPosition;
    }
}
