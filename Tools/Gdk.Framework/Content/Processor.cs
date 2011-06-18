using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Gdk;

namespace Gdk.Content
{
    /// <summary>
    /// Base Class for all content Processors
    /// </summary>
    public abstract class Processor
    {
        // The build context
        public Asset Asset;
        public BuildContext Context;

        // Process method
        public abstract void Process();

        // GetDetails method
        public abstract ProcessorDetails GetDetails();
    }
}
