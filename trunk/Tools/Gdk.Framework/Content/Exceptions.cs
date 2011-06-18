using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Gdk
{

    public class BuildException : ApplicationException
    {
        public BuildException()
            : base()
        { }

        public BuildException(string message)
            : base(message)
        { }

        public BuildException(string message, Exception innerException)
            : base(message, innerException)
        { }

    }

}
