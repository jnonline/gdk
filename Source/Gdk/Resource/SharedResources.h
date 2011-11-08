/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#pragma once


#include "ResourcePool.h"

// Max number of quads supported by the gdk in a single batch
#define GDK_MAX_QUADS 2048

namespace Gdk
{
	/// @addtogroup Resources & Assets
    /// @{
    
    // =================================================================================
    ///	@brief
    ///	    This static class contains references to all resources used internally by the GDK.
    // =================================================================================
    class SharedResources
	{
	public:

        // Public Properties
		// ================================
		
        // -----------------------------------
        /// @name Resources
        /// @{
        
        /// This resource pool contains all the managed resources used by the GDK
		static ResourcePool Pool;

        /// @}
        // -----------------------------------
        /// @name Atlases, Images, and Animations
        /// @{
		
		static struct _Atlases
		{
			class Atlas* Test;
		} Atlases;
		
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

		/// @}
        // -----------------------------------
        /// @name Fonts
        /// @{
        
		static struct _Fonts
		{
			class BMFont* Arial20;
		} Fonts;
		
		/// @}
        // -----------------------------------
        /// @name Models
        /// @{
        
		static struct _Models
		{
			class Model* TestAxis;
		} Models;

		/// @}
        // -----------------------------------
        /// @name Shaders
        /// @{
        
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

		/// @}
        // -----------------------------------
        /// @name Global Buffers
        /// @{
        
		// Quad index buffer
		static GLushort* QuadIndexBuffer;

        /// @}
        
	private:
		
		// Internal Methods
		// ================================
		
		// CTor/DTor
		SharedResources() {};

        // Utilities
        static Shader* LoadShader(const char* name);
        static BMFont* LoadBMFont(const char* name);
        static Atlas* LoadAtlas(const char* name);
        static Model* LoadModel(const char* name);
        
		// Init / Shutdown
		friend class Application;
		static void Init();
		static void Shutdown();
		
	};
    
    /// @}

} // namespace 
