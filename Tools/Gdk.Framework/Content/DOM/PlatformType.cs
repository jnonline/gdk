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

        iPad1 = 101,
        iPad2 = 102,
        iPhone3GS = 113,
        iPhone4 = 114,
        iPod3 = 123,
        iPod4 = 124,

        AndroidPhone = 201,
        AndroidTablet = 202
    }

}
