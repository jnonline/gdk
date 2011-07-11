/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#pragma once

#include "GdkLIB.h"
#include "AssetPool.h"

// Max number of quads supported by the gdk
#define GDK_MAX_QUADS 1024

namespace Gdk
{
	// ============================================================================

	class SharedAssets
	{
	public:

		// Shared Assets Pool
		// -------------------------------

		static AssetManager Manager;
		static AssetPool Pool;

		// Atlases
		// -------------------------------

		static struct _Atlases
		{
			class Atlas* Test;
		} Atlases;

		// Atlas Images & Animations
		// -------------------------------
		
		static struct _AtlasImages
		{
			class AtlasImage* Fence;
			class AtlasImage* Particle1;
			class AtlasImage* Particle2;
			class AtlasImage* Test1;
		} AtlasImages;

		static struct _AtlasAnimations
		{
			class AtlasAnimation* Chopper;
		} AtlasAnimations;

		// Fonts
		// -------------------------------

		static struct _Fonts
		{
			class BMFont* Arial20;
		} Fonts;
		
		// Models
		// -------------------------------

		static struct _Models
		{
			class Model* TestAxis;
		} Models;

		// Shaders
		// -------------------------------

		static struct _Shaders
		{
			struct
			{
				class Shader* Textured;
				class Shader* NonTextured;
			} Renderer2D;

			struct
			{
				class Shader* Textured;
			} Billboard;

			struct 
			{
				struct
				{
					class Shader* DiffuseTextured;
					class Shader* NonTextured;
				} Mesh;

				struct
				{
					class Shader* DiffuseTextured;
				} SkeletalMeshB4;

			} Model;
		} Shaders;

		// Shared Buffers
		// -----------------------------

		// Quad index buffer
		static GLushort* QuadIndexBuffer;


	protected:
		
		// Internals
		// ------------------------------

		// CTor/DTor
		SharedAssets() {};

		// Init / Shutdown
		friend class Application;
		static void Init();
		static void Shutdown();
		
	};

} // namespace 
