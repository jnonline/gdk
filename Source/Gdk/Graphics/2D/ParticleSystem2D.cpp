/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#include "BasePCH.h"
#include "ParticleSystem2D.h"

using namespace Gdk;

// ***********************************************************************
ParticleSystem2D::ParticleSystem2D()
{
	this->maxParticles = 0;
	this->particlesBuffer = NULL;
	this->spawnTimer = 0.0f;
	this->activePoolCount = 0;

	// Emitter properties
	this->Position = Vector2::ZERO;
	this->Z = 0;
	this->Orientation = 0.0f;
	this->Paused = false;
	this->EmitterEnabled = true;
	this->ParticlesAreRelativeToEmitter = true;
	this->SpawnRate = 5.0f;
	this->BlendMode = BlendMode::Normal;
	this->Shader = NULL;

	this->DefaultLifetime = 5.0f;
	this->DefaultSize = 1.0f;
	this->DefaultColor = Color::WHITE;
	this->DefaultImage = Gdk::SharedResources::AtlasImages.Test1;
}

// ***********************************************************************
ParticleSystem2D::~ParticleSystem2D()
{
	// Free allocated memory
	ReleaseMemory();
}

// ***********************************************************************
void ParticleSystem2D::ReleaseMemory()
{
	// Free allocated buffers
	if(this->particlesBuffer != NULL)
	{
		GdkFree(this->particlesBuffer);
		this->particlesBuffer = NULL;
	}
}

// ***********************************************************************
void ParticleSystem2D::Init(UInt32 maxParticles, bool prePopulate)
{
	// Free any already allocated memory
	ReleaseMemory();

	// Allocate the particles arrays
	size_t stride = GetParticleStride();
	particlesBuffer = (Particle2D*) GdkAlloc(maxParticles * stride);

	// Setup the pools
	this->maxParticles = maxParticles;
	this->activePool = NULL;
	this->inactivePool = (Particle2D*) particlesBuffer;
	this->activePoolCount = 0;

	// Link together all the particles
	Particle2D* particle = this->inactivePool;
	for(size_t i=0; i<maxParticles-1; i++)
	{
		particle->Next = (Particle2D*)((size_t)particlesBuffer + (i+1) * stride);
		particle = particle->Next;
	}
	particle->Next = NULL;

	// Do we need to do a pre-populate pass?
	if(prePopulate && this->Paused == false)
	{
		// Spawn all the particles
		while(this->inactivePool != NULL)
		{
			Update(SpawnRate);
		}
	}
}

// ***********************************************************************
size_t ParticleSystem2D::GetParticleStride()
{
	// By default, particle systems use the Particle2D struct stride
	return sizeof(Particle2D);
}

// ***********************************************************************
void ParticleSystem2D::Update(float elapsedSeconds)
{
	// Are we paused?
	if(this->Paused == true)
		return;

	// Run external particle updates
	// ------------------------------

	// Call the virtual update method
	OnUpdateParticles(elapsedSeconds, this->activePool);

	// Update the particle lifetimes
	// ---------------------------------

	// Loop through the particles
	Particle2D *particle = this->activePool;
	Particle2D *lastParticle = NULL;
	while(particle != NULL)
	{
		// Update the particle's lifetime
		particle->Lifetime -= elapsedSeconds;

		// Has this particle life expired?
		if(particle->Lifetime <= 0.0f)
		{
			// Remove the particle from the active pool
			this->activePoolCount--;
			if(particle == this->activePool)
				// This particle is the head of the active pool, so move the active pool head to the next particle
				this->activePool = particle->Next;
			else
				// Remove this particle from the chain (by linking the last particle to the next particle)
				lastParticle->Next = particle->Next;

			// Put the particle into the inactive pool
			if(this->inactivePool == NULL)
			{
				// The inactive pool is empty, so put this particle as the first entry in the active pool
				this->inactivePool = particle;
				particle->Next = NULL;
			}
			else
			{
				// Put this particle as the head of the inactive pool
				particle->Next = this->inactivePool;
				this->inactivePool = particle;
			}
			
			// Get the next particle --

			// Are we still at the head of the active pool?
			if(lastParticle == NULL)
				// Get the head of the active pool
				particle = this->activePool;
			else
				// Get the next particle in the active pool chain
				particle = lastParticle->Next;
		}
		else 
		{
			// The particle is still alive

			// Move to the next particle in the chain
			lastParticle = particle;
			particle = particle->Next;
		}
	}

	// Spawn new Particles
	// ------------------------------

	// Are we actively emitting?
	if(this->EmitterEnabled == true)
	{
		// Emit particles over the given time span
		EmitByTimeSpan(elapsedSeconds);
	}

	// --------------------------------------
}

// ***************************************************************
void ParticleSystem2D::Clear()
{
	// Loop through the active particles
	while(this->activePool != NULL)
	{
		// Stick this particle on the inactive list
		Particle2D* next = this->activePool->Next;
		this->activePool->Next = this->inactivePool;
		this->inactivePool = this->activePool;
		this->activePool = next;
	}

	// Reset the active particle count
	this->activePoolCount = 0;
}

// ***************************************************************
void ParticleSystem2D::EmitByTimeSpan(float elapsedSeconds)
{
	// Emits particles over a given time span.  The number of particles emitted
	// is relative to the SpawnRate

	// Advance the spawn timer
	spawnTimer += elapsedSeconds;
	
	// Is this system using a cusom Spawn Rate?
	float timeBetweenSpawns = 1.0f / this->SpawnRate;

	// Emit the number of particles that we should in the given time span
	while(spawnTimer > timeBetweenSpawns)
	{
		// Emit a particle
		EmitByNumber(1);

		spawnTimer -= timeBetweenSpawns;
	}
}

// ***************************************************************
void ParticleSystem2D::EmitByNumber(int numParticles)
{
	// Emits the specified number of particles!

	// Calculate the world transform
	Matrix2D world;
	if(this->ParticlesAreRelativeToEmitter == false)
	{
		world.RotateScaleTranslate(1.0f, 1.0f, this->Position.X, this->Position.Y, this->Orientation);
	}

	// While there are particles to emit!
	while(numParticles > 0 && this->inactivePool != NULL)
	{
		// Get the next particle from the inactive pool
		Particle2D *newParticle = this->inactivePool;
		this->inactivePool = newParticle->Next;
		newParticle->Next = NULL;

		// Setup the initial values of the particle
		newParticle->Position = Vector2(0,0);
		newParticle->Orientation = 0.0f;
		newParticle->Color = this->DefaultColor;
		newParticle->Size = this->DefaultSize;
		newParticle->Lifetime = this->DefaultLifetime;
		newParticle->Image = this->DefaultImage;

		// Call the virtual creation methods
		OnNewParticle(newParticle);

		// Are the particles NOT relative to the emitter?
		if(this->ParticlesAreRelativeToEmitter == false)
		{
			// Transform the particles into world space
			world.TransformPointInPlace(newParticle->Position.X, newParticle->Position.Y);
			newParticle->Orientation += this->Orientation;
		}

		// Put the new particle on the active pool
		newParticle->Next = this->activePool;
		this->activePool = newParticle;
		this->activePoolCount++;

		// 1 particle down
		numParticles--;
	}
}


// ***********************************************************************
int ParticleSystem2D::GetNumActiveParticles()
{
	return this->activePoolCount;
}

// ***********************************************************************
int ParticleSystem2D::GetMaxParticles()
{
	return this->maxParticles;
}

// ***********************************************************************
bool ParticleSystem2D::AreAllParticlesActive()
{
	return this->inactivePool == NULL;
}

// ***********************************************************************
void ParticleSystem2D::Draw()
{
	// Loop through the particles
	Particle2D *particle = this->activePool;

	// Get the shader used for rendering
	Gdk::Shader* shader = this->Shader;
	if(shader == NULL)
		shader = Renderer2D::GetTexturedShader();

	// Create a transform matrix for the emitter
	Matrix2D world;
	world.RotateScaleTranslate(1.0f, 1.0f, this->Position.X, this->Position.Y, this->Orientation);

	// We can only draw in batches small enough to fit the MAX Quads, so lets loop until we can draw all the batches
	int numParticlesLeft = this->activePoolCount;
	while(numParticlesLeft > 0)
	{
		// How many particles should we draw in this batch?
		int batchSize = Math::Min(GDK_MAX_QUADS, numParticlesLeft);
		numParticlesLeft -= batchSize;

		// Get a batch to draw the particles
		Renderer2DBatchP2T2C4 batch = Renderer2D::GetBatchP2T2C4(
			batchSize, 
			Renderer2DPrimitiveType::Quads, 
			particle->Image->Sheet->Texture->GLTextureId, 
			0, 
			this->BlendMode, 
			this->Z, 
			shader
			);

		// Are the particles relative to the emitter?
		if(this->ParticlesAreRelativeToEmitter)
		{
			// Draw all the particles, while applying the emitter world transform
			while(batchSize > 0)
			{
				// Transform the particle into world coordinates (from emitter local)
				Vector2 worldPosition = world.TransformPoint(particle->Position);

				// Draw this particle
				float scale = particle->Size  / particle->Image->Width;
				Renderer2D::DrawImage(
					batch,
					particle->Image,
					worldPosition,
					particle->Color, 
					particle->Orientation + this->Orientation,
					scale, scale
					);

				// Next particle
				particle = particle->Next;
				batchSize--;
			}
		}
		else
		{
			// Draw all the particles in world space
			while(batchSize > 0)
			{
				// Draw this particle
				float scale = particle->Size  / particle->Image->Width;
				Renderer2D::DrawImage(
					batch,
					particle->Image,
					particle->Position,
					particle->Color, 
					particle->Orientation,
					scale, scale
					);

				// Next particle
				particle = particle->Next;
				batchSize--;
			}
		}
	}
}