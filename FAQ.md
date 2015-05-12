# GDK FAQ #

  * [What is the GDK?](FAQ#What_is_the_GDK?.md)
  * [Why should i use it?](FAQ#Why_should_I_use_it?.md)
  * [What platforms are supported?](FAQ#What_platforms_are_supported?.md)
  * [Feature set?](FAQ#Feature_set?.md)
  * [Requirements?](FAQ#Requirements?.md)
  * [Shipped games?](FAQ#Shipped_games?.md)

### What is the GDK? ###
The GDK is a "Game SDK" or Software Development Kit for games. It is composed of two distinct parts, the first is an automated content pipeline that takes game assets (images, fonts, models, etc) and optimizes them for use by the GDK API. The second part of the GDK is an API library of cross platform C++ code for managing input, displaying graphics, and performing many of the functions most used by developers when programming games. The output of the automated content pipeline works hand in hand with the C++ API to deliver high performance and quick responsiveness in games developed with the GDK.

### Why should I use it? ###
The primary purpose of the GDK, is to get a game project running from scratch **as fast as possible**.  By simply downloading the GDK package & running a quick command line tool, you will have a building/running game on multiple platforms in under five minutes.

The secondary purpose of the GDK is to streamline the process of getting content into a game and rendered on screen **as easy as possible**.  The GDK's Content Studio tool provides a simple interface for managing, configuring, and building your game assets (Images, Textures, Fonts, Animations, Models, Shaders, Materials, etc).   The GDK C++ layer provides classes for loading & displaying these assets.

Lastly GDK is licensed under the **[MIT License](http://www.opensource.org/licenses/mit-license.php)** meaning it is free for personal or commercial use.

### What platforms are supported? ###
Fully functional
  * Windows XP / Vista / 7
  * MAC OS X 10.5+
  * iOS 3.2+   (iPhone / iPad / iPod)

In the works
  * Android   (Phone & Tablet)
  * Google NaCl

### Feature set? ###
Content:
  * Full suite of content management / configuration / building tools
  * **All popular image formats.**  Images are processed & compressed in their native texture pixel formats for super fast decompression & loading.
  * **Atlased sprite sheets:**  Put loads of images onto 1 sheet , create animations from them, and use custom pixel formats, image scaling, etc.   All configurable in easy-to-understand xml .atlas files.
  * **COLLADA Mesh support**.   (Currently only static meshes are supported, skinning & more are in works)
  * **BM Fonts:**  convert true type fonts into bitmap fonts and use them into your game.

C++ Layer
  * **Platform Projects:**   Pre-configured projects for every supported platform.
  * **Core Systems:**  Multithreading, C++ Multicast Delegates, File IO,  OS Interfaces
  * **Math Library** (2D & 3D), Transforms, Shapes, Intersection Tests, Collision Tests, etc
  * **Debugging tools:**   Logging, Memory tracking, Assertions, Debug breaks, etc
  * **Localizable:**  UTF8 / UTF16 logging, text rendering, and font support.
  * **2D Graphics:**  Images, Animations, Sprites, Primitives (circles, rects, lines, etc), Font Text, etc
  * **3D Graphics:**  Models, Meshes, Textures, primtiives (spheres, cones, boxes, etc).
  * **Input:**   Keyboard, Mouse, Touch, Gestures, Motion, Vibration, Accelerometers, Device interfaces, etc
  * **Asset Management:**  Background loading, Compressed assets, Asset Pools, etc


### Requirements? ###
For Development:
  * **Windows:**   Visual Studio 2008+ (Express will work just fine)
  * **MAC / iOS:**   XCode 4

For Running:
  * OpenGL 2.1 or OpenGL ES 2.0

### Shipped games? ###
Finished Games:

  * **[Little Soldiers HD](http://itunes.apple.com/us/app/little-soldiers-hd/id388360005?mt=8)** - (iPhone & iPod)
  * **[The Incredible Machine](http://itunes.apple.com/us/app/the-incredible-machine/id440297659?mt=8)** - (iPad, iPhone, & iPod)

Miscellaneous Projects:

  * **[GunHack](http://www.youtube.com/watch?v=5sK4S-WxsCQ)** - Made in 72-hours - Ludum Dare #20 JAM Entry   (Windows, Mac, iOS 3.2)