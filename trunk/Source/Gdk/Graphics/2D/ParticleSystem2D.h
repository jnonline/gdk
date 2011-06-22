/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#pragma once

#include "GdkLIB.h"
#include "../../Math/Maths.h"
#include "../../Math/2D/Vector2.h"
#include "../../Math/2D/Matrix2D.h"
#include "../Color.h"
#include "../Graphics.h"
#include "Atlas.h"

namespace Gdk
{
	// ================================================
	struct Particle2D
	{
		Particle2D*		Next;
		Vector2			Position;
		float			Orientation;
        Gdk::Color		Color;
		float			Size;
		float			Lifetime;
		AtlasImage*		Image;
	};

	// ================================================
	struct Particle2DEx : Particle2D
	{
		Vector2			Velocity;
		float			SpinVelocity;
	};

	// ================================================
	class ParticleSystem2D
	{
	public:

		// Properties
		// ---------------------------------

		// Emitter properties
		bool EmitterEnabled;					// Is the system emitting particles?  (can be forced by calling EmitBy***)
		bool ParticlesAreRelativeToEmitter;		// Are the particles rendered relative to the emitter transform?
		Vector2 Position;						// The Position of the emitter 
		float Orientation;						// The Orientation of the emitter
		float Z;								// Z coordinate for all emitted particles/images
		float SpawnRate;						// Number of particles to spawn per-second.   (if you wish to turn off default emission, set EmitterEnabled = false)

		// Defaults for new particles  - (usually the OnNewParticle() override would set the properties on the particle)
		float		DefaultLifetime;
		float		DefaultSize;
		Color		DefaultColor;
		AtlasImage*	DefaultImage;
		
		// System properties
		bool Paused;							// Is the update cycle of the entire system paused?
		
		// Render State
		Gdk::BlendMode::Enum		BlendMode;
		Gdk::Shader*				Shader;

	public:
		// Method
		// ---------------------------------

		// CTor/DTor Methods
		ParticleSystem2D();
		virtual ~ParticleSystem2D();

		// Init / Update / Render
		void Init(UInt32 maxParticles, bool prePopulate = false);
		void Update(float elapsedSeconds);
		void Draw();

		// Custom Emission
		void Clear();
		void EmitByNumber(int numParticles);			// Immediately emits X particles
		void EmitByTimeSpan(float elapsedSeconds);		// Immediately emits ElapsedSeconds/SpawnRate particles
		
		// Get statistical information
		int GetNumActiveParticles();
		int GetMaxParticles();
		bool AreAllParticlesActive();
		Particle2D *GetActivePoolHead() {return this->activePool;}

	protected:
		// Virtual methods for derived classes
		virtual size_t GetParticleStride();
		virtual void OnNewParticle(Particle2D *newParticle) {}
		virtual void OnUpdateParticles(float elapsedSeconds, Particle2D *pool) {}

	private:

		// Flat particles buffer
		int maxParticles;
		void *particlesBuffer;

		// Active & Inactive pools
		Particle2D *activePool;
		Particle2D *inactivePool;
		int activePoolCount;

		// Spawn timer
		float spawnTimer;

		// Internal Methods
		void ReleaseMemory();
	};

} // namespace