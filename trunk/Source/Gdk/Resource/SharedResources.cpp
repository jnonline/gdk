/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#include "BasePCH.h"
#include "SharedResources.h"

using namespace Gdk;

// Static instantiations
ResourcePool                    SharedResources::Pool;
GLushort*                       SharedResources::QuadIndexBuffer = NULL;

SharedResources::_Models        SharedResources::Models;
SharedResources::_Fonts         SharedResources::Fonts;
SharedResources::_Atlases       SharedResources::Atlases;
SharedResources::_Shaders       SharedResources::Shaders;

SharedResources::_AtlasImages		SharedResources::AtlasImages;
SharedResources::_AtlasAnimations	SharedResources::AtlasAnimations;

// ***********************************************************************
void SharedResources::Init()
{
    // Shaders   (Note: These must be first)
	// --------------------------------
    
	// Renderer2D
	{
		Shaders.Renderer2D.NonTextured = LoadShader("Shaders/Renderer2D/NonTextured");
		Shaders.Renderer2D.Textured = LoadShader("Shaders/Renderer2D/Textured");
	}
    
	// Billboard
	{
		Shaders.Billboard.Textured = LoadShader("Shaders/Billboard/Textured");
	}
    
	// Models
	{
		// Mesh
		Shaders.Model.Mesh.DiffuseTextured = LoadShader("Shaders/Model/Mesh/DiffuseTextured");
		Shaders.Model.Mesh.NonTextured = LoadShader("Shaders/Model/Mesh/NonTextured");

		// SkeletalMesh B4
		Shaders.Model.SkeletalMeshB4.DiffuseTextured = LoadShader("Shaders/Model/SkeletalMeshB4/DiffuseTextured");
	}

	// Models
	// ------------------------------------

	Models.TestAxis = LoadModel("Models/TestAxis");
	
	// Fonts
	// --------------------------------

	Fonts.Arial20 = LoadBMFont("Fonts/Arial20");

	// Atlases
	// --------------------------------

	Atlases.Test = LoadAtlas("Atlas/Test");

	// Atlas Images & Animations
	AtlasImages.Test1 = Atlases.Test->GetImage("Test/Test1");
	AtlasImages.Particle1 = Atlases.Test->GetImage("Test/Particle1");
	AtlasImages.Particle2 = Atlases.Test->GetImage("Test/Particle2");
	AtlasImages.Fence = Atlases.Test->GetImage("Test/Fence");
	AtlasAnimations.Chopper = Atlases.Test->GetAnimation("Chopper");
	
	// Shared Buffers
	// ---------------------------------

	// Allocate the shared quad index buffer
	QuadIndexBuffer = (GLushort*) GdkAlloc( sizeof(GLushort) * GDK_MAX_QUADS * 6);

	// Populate the index buffer
	GLushort *indexPtr = QuadIndexBuffer;
	for(int i=0; i<GDK_MAX_QUADS; i++)
	{
		*indexPtr++ = i * 4;
		*indexPtr++ = i * 4 + 1;
		*indexPtr++ = i * 4 + 2;
		*indexPtr++ = i * 4 + 1;
		*indexPtr++ = i * 4 + 2;
		*indexPtr++ = i * 4 + 3;
	}
}

// ***********************************************************************
void SharedResources::Shutdown()
{
	// Free the Shared Buffers
	if(QuadIndexBuffer != NULL)
		GdkFree(QuadIndexBuffer);
	QuadIndexBuffer = NULL;

	// Release the Resources pool
	Pool.Release();
}

// ***********************************************************************
Shader* SharedResources::LoadShader(const char* name)
{
    Shader* shader = ShaderManager::FromAsset(name);
    SharedResources::Pool.Add(shader);
    return shader;
}

// ***********************************************************************
BMFont* SharedResources::LoadBMFont(const char* name)
{
    BMFont* font = BMFontManager::FromAsset(name);
    SharedResources::Pool.Add(font);
    return font;
}

// ***********************************************************************
Atlas* SharedResources::LoadAtlas(const char* name)
{
    Atlas* atlas = AtlasManager::FromAsset(name);
    SharedResources::Pool.Add(atlas);
    return atlas;
}

// ***********************************************************************
Model* SharedResources::LoadModel(const char* name)
{
    Model* model = ModelManager::FromAsset(name);
    SharedResources::Pool.Add(model);
    return model;
}

