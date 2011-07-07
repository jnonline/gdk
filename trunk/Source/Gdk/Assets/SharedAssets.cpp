/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#include "BasePCH.h"
#include "SharedAssets.h"

using namespace Gdk;

// Static instantiations
AssetPool				SharedAssets::Pool;
GLushort*				SharedAssets::QuadIndexBuffer = NULL;

SharedAssets::_Models	SharedAssets::Models;
SharedAssets::_Fonts	SharedAssets::Fonts;
SharedAssets::_Atlases	SharedAssets::Atlases;
SharedAssets::_Shaders	SharedAssets::Shaders;

SharedAssets::_AtlasImages		SharedAssets::AtlasImages;
SharedAssets::_AtlasAnimations	SharedAssets::AtlasAnimations;

// ***********************************************************************
void SharedAssets::Init()
{
	AssetManager* assetManager = AssetManager::GetSingleton();

    // Shaders   (Note: These must be first)
	// --------------------------------
    
	// Renderer2D
	{
		SharedAssets::Shaders.Renderer2D.NonTextured = assetManager->Load<Shader>("Shaders/Renderer2D/NonTextured", &Pool, NULL);
		SharedAssets::Shaders.Renderer2D.Textured = assetManager->Load<Shader>("Shaders/Renderer2D/Textured", &Pool, NULL);
	}
    
	// Billboard
	{
		SharedAssets::Shaders.Billboard.Textured = assetManager->Load<Shader>("Shaders/Billboard/Textured", &Pool, NULL);
	}
    
	// Models
	{
		SharedAssets::Shaders.Model.Mesh.DiffuseTextured = assetManager->Load<Shader>("Shaders/Model/Mesh/DiffuseTextured", &Pool, NULL);
		SharedAssets::Shaders.Model.Mesh.NonTextured = assetManager->Load<Shader>("Shaders/Model/Mesh/NonTextured", &Pool, NULL);
	}

	// Models
	// ------------------------------------

	SharedAssets::Models.TestAxis = assetManager->Load<Model>("Models/TestAxis", &Pool, NULL);
	
	// Fonts
	// --------------------------------

	SharedAssets::Fonts.Arial20 = assetManager->Load<BMFont>("Fonts/Arial20", &Pool, NULL);

	// Atlases
	// --------------------------------

	SharedAssets::Atlases.Test = assetManager->Load<Atlas>("Atlas/Test", &Pool, NULL);

	// Atlas Images & Animations
	SharedAssets::AtlasImages.Test1 = SharedAssets::Atlases.Test->GetImage("Test\\Test1");
	SharedAssets::AtlasImages.Particle1 = SharedAssets::Atlases.Test->GetImage("Test\\Particle1");
	SharedAssets::AtlasImages.Particle2 = SharedAssets::Atlases.Test->GetImage("Test\\Particle2");
	SharedAssets::AtlasImages.Fence = SharedAssets::Atlases.Test->GetImage("Test\\Fence");
	SharedAssets::AtlasAnimations.Chopper = SharedAssets::Atlases.Test->GetAnimation("Chopper");
	
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
void SharedAssets::Shutdown()
{
	// Free the Shared Buffers
	if(QuadIndexBuffer != NULL)
		GdkFree(QuadIndexBuffer);
	QuadIndexBuffer = NULL;

	// Release the assets pool
	Pool.Release();
}
