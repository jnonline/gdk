/*
  Copyright (c) 2011, Raincity Games LLC
  Licensed under the MIT license: http://www.opensource.org/licenses/mit_license.php
*/

using System;
using System.Collections.Generic;
 
namespace Gdk.Framework
{
    public struct Sphere3
    {
        // Member Fields
        public Vector3 Center;
        public float Radius;

        /// <summary>
        /// Constructor
        /// </summary>
        public Sphere3(Vector3 center, float radius)
        {
            this.Center = center;
            this.Radius = radius;
        }

        /// <summary>
        /// Grows this sphere to contain itself & the given point
        /// </summary>
        /// <param name="point"></param>
        public void GrowToContain(Vector3 point)
        {
            // Build a vector from the sphere center to the point
            Vector3 centerToPoint = point - this.Center;

            // Get a vector from the sphere center, to the spehre edge (inverse direction of the sphere->point vector)
            Vector3 centerToEdge = centerToPoint;
            centerToEdge.Normalize();
            centerToEdge *= -Radius;

            // Calculate the new sphere center
            Vector3 newCenter = ((this.Center + centerToEdge) + (this.Center + centerToPoint)) * 0.5f;
            float newRadius = (centerToEdge - centerToPoint).Length() * 0.5f;
            
            this.Center = newCenter;
            this.Radius = newRadius;
        }
    }
}