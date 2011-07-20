/*
  Copyright (c) 2011, Raincity Games LLC
  Licensed under the MIT license: http://www.opensource.org/licenses/mit_license.php
*/

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Gdk
{

    public enum PlatformType
	{
        Unknown = 0,

		Windows = 1,
		Mac = 2,
		Linux = 3,
		iOS = 4,
		Android = 5,
		NaCl = 6,
	};

    public enum DeviceType
    {
        Unknown = 0,

        PC = 1,
        Web = 2,

        iPad = 101,
        iPhone = 102,
        iPodTouch = 103,

        AndroidPhone = 201,
        AndroidTablet = 202
    }

}
