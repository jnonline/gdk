/*
  Copyright (c) 2011, Raincity Games LLC
  Licensed under the MIT license: http://www.opensource.org/licenses/mit_license.php
*/

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Gdk.Framework
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
