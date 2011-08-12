/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#pragma once


#include "../../Math/Maths.h"
#include "../../Math/3D/Vector3.h"
#include "../../Math/3D/Matrix3D.h"
#include "../Color.h"
#include "../Graphics.h"
#include "../2D/Atlas.h"
#include "BillboardRenderer.h"

namespace Gdk
{
	// ================================================
	struct BillboardParticle
	{
		BillboardParticle* Next;
		Vector3 Position;
		Vector3 Velocity;
        Gdk::Color Color;
		float Size;
		float Lifetime;
		float Spin;
		float SpinVelocity;
		AtlasImage *Image;
	};

	// ================================================
	class BillboardParticleSystem
	{
	public:

		// Properties
		// ---------------------------------

		// Emitter properties
		bool EmitterEnabled;					// Is the system emitting particles?  (can be forced by calling EmitBy***)
		bool ParticlesAreRelativeToEmitter;		// Are the particles rendered relative to the emitter transform?
		Vector3 Position;						// The Position of the emitter 
		// Quaternion Orientation;					// The Orientation of the emitter
		float SpawnRate;						// Number of particles to spawn per-second.   (if you wish to turn off default emission, set EmitterEnabled = false)
		BillboardFacing::Enum Facing;
		Vector3	CustomFacing;

		float DefaultParticleLifetime;			// Default for each new particle
		float DefaultParticleSize;				//   (usually the OnNewParticle() override would set the properties on the particle)
		Color DefaultParticleColor;
		
		// System properties
		bool Paused;							// Is the update cycle of the entire system paused?
		
		// Render State
		BlendMode::Enum BlendingMode;

	public:
		// Method
		// ---------------------------------

		// CTor/DTor Methods
		BillboardParticleSystem();
		virtual ~BillboardParticleSystem();

		// Init / Update / Render
		void Init(int maxParticles, bool prePopulate);
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
		BillboardParticle *GetActivePoolHead() {return this->activePool;}

	protected:
		// Virtual methods for derived classes
		virtual void OnNewParticle(BillboardParticle *particle) {}
		virtual void OnUpdateParticles(float elapsedSeconds, BillboardParticle *pool) {}

	private:
		int maxParticles;
		int nextSpawnParticle;

		// Internal Properties
		BillboardParticle *particles;
		BillboardParticle *activePool;
		BillboardParticle *inactivePool;
		int activeParticleCount;
		float spawnTimer;

		// Internal Methods
		void ReleaseMemory();
	};

} // namespace