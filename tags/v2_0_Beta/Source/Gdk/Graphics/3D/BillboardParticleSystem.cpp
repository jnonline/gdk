/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#include "BasePCH.h"
#include "BillboardParticleSystem.h"
#include "BillboardRenderer.h"

using namespace Gdk;

// ***********************************************************************
BillboardParticleSystem::BillboardParticleSystem()
{
	this->maxParticles = 0;
	this->particles = NULL;
	this->spawnTimer = 0.0f;
	this->activeParticleCount = 0;

	// Emitter properties
	this->Position = Vector3::ZERO;
	this->Paused = false;
	this->EmitterEnabled = true;
	this->ParticlesAreRelativeToEmitter = true;
	this->SpawnRate = 5.0f;
	this->BlendingMode = BlendMode::Normal;
	this->Facing = BillboardFacing::Camera;
	this->CustomFacing = Vector3::UNIT_Z;

	this->DefaultParticleLifetime = 5.0f;
	this->DefaultParticleSize = 1.0f;
	this->DefaultParticleColor = Color::WHITE;
}

// ***********************************************************************
BillboardParticleSystem::~BillboardParticleSystem()
{
	// Free allocated memory
	ReleaseMemory();
}

// ***********************************************************************
void BillboardParticleSystem::ReleaseMemory()
{
	// Free allocated buffers
	if(this->particles != NULL)
	{
		free(this->particles);
		this->particles = NULL;
	}
}

// ***********************************************************************
void BillboardParticleSystem::Init(int maxParticles, bool prePopulate)
{
	// Free any already allocated memory
	ReleaseMemory();

	// Allocate the particles arrays
	particles = (BillboardParticle*) malloc(maxParticles * sizeof(BillboardParticle));

	// Put all the particles in the inactive pool
	this->maxParticles = maxParticles;
	this->activePool = NULL;
	this->inactivePool = particles;
	for(int i=0; i<maxParticles-1; i++)
		particles[i].Next = &(particles[i+1]);
	particles[maxParticles-1].Next = NULL;

	// Do we need to do a pre-populate pass?
	if(prePopulate)
	{
		// Spawn all the particles
		while(this->inactivePool != NULL)
		{
			Update(SpawnRate);
		}
	}
}

// ***********************************************************************
void BillboardParticleSystem::Update(float elapsedSeconds)
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
	BillboardParticle *particle = this->activePool;
	BillboardParticle *lastParticle = NULL;
	while(particle != NULL)
	{
		// Update the particle's lifetime
		particle->Lifetime -= elapsedSeconds;

		// Has this particle life expired?
		if(particle->Lifetime <= 0.0f)
		{
			// Remove the particle from the active pool
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

	// Perform an update pass on all active particles
	// --------------------------------------

	// Loop through the particles
	particle = this->activePool;
	activeParticleCount = 0;
	while(particle != NULL)
	{	
		// Update the particle's properties
		particle->Position += (elapsedSeconds * particle->Velocity);
		particle->Spin += elapsedSeconds * particle->SpinVelocity;

		// Count the number of active particles
		activeParticleCount++;

		// Move to the next particle in the chain
		particle = particle->Next;
	}

	// --------------------------------------
}

// ***************************************************************
void BillboardParticleSystem::Clear()
{
	// Loop through the active particles
	while(this->activePool != NULL)
	{
		// Stick this particle on the inactive list
		BillboardParticle* next = this->activePool->Next;
		this->activePool->Next = this->inactivePool;
		this->inactivePool = this->activePool;
		this->activePool = next;
	}

	// Reset the active particle count
	this->activeParticleCount = 0;
}

// ***************************************************************
void BillboardParticleSystem::EmitByTimeSpan(float elapsedSeconds)
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
void BillboardParticleSystem::EmitByNumber(int numParticles)
{
	// Emits the specified number of particles!

	// While there are particles to emit!
	while(numParticles > 0 && this->inactivePool != NULL)
	{
		// Get the next particle from the inactive pool
		BillboardParticle *newParticle = this->inactivePool;
		this->inactivePool = newParticle->Next;
		newParticle->Next = NULL;

		// Setup the initial values of the particle
		newParticle->Position = Vector3::ZERO;
		newParticle->Velocity = Vector3::ZERO;
		newParticle->Spin = 0.0f;
		newParticle->SpinVelocity = 0.0f;
		newParticle->Color = this->DefaultParticleColor;
		newParticle->Size = this->DefaultParticleSize;
		newParticle->Lifetime = this->DefaultParticleLifetime;

		// Call the virtual creation methods
		OnNewParticle(newParticle);

		// Are the particles NOT relative to the emitter?
		if(this->ParticlesAreRelativeToEmitter == false)
		{
			// Transform the particles into world space
			newParticle->Position += this->Position;
		}

		// Put the new particle on the active pool
		newParticle->Next = this->activePool;
		this->activePool = newParticle;

		// 1 particle down
		numParticles--;
	}
}


// ***********************************************************************
int BillboardParticleSystem::GetNumActiveParticles()
{
	return this->activeParticleCount;
}

// ***********************************************************************
int BillboardParticleSystem::GetMaxParticles()
{
	return this->maxParticles;
}

// ***********************************************************************
bool BillboardParticleSystem::AreAllParticlesActive()
{
	return this->inactivePool == NULL;
}

// ***********************************************************************
void BillboardParticleSystem::Draw()
{
	// Loop through the particles
	BillboardParticle *particle = this->activePool;
	
	// Are the particles relative to the emitter?
	if(this->ParticlesAreRelativeToEmitter)
	{
		// Draw all the particles, while applying the emitter world transform
		while(particle != NULL)
		{
			// Transform the particle into world coordinates (from emitter local)
			// TODO(P2): Support quaternion transforms...
			Vector3 worldPosition = this->Position + particle->Position;

			// Draw this particle
			float scale = particle->Size  / particle->Image->Width;
			BillboardRenderer::DrawImage(
				particle->Image,
				worldPosition,
				particle->Color, 
				particle->Spin,
				scale, scale,
				this->Facing, 
				this->CustomFacing,
				this->BlendingMode
				);

			// Next particle
			particle = particle->Next;
		}
	}
	else
	{
		// Draw all the particles in world space
		while(particle != NULL)
		{
			// Draw this particle
			float scale = particle->Size  / particle->Image->Width;
			BillboardRenderer::DrawImage(
				particle->Image,
				particle->Position,
				particle->Color, 
				particle->Spin,
				scale, scale,
				this->Facing, 
				this->CustomFacing,
				this->BlendingMode
				);

			// Next particle
			particle = particle->Next;
		}
	}
}