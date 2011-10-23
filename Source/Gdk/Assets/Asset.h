/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#pragma once


#include "AssetType.h"

namespace Gdk
{
	// ============================================================================
	
	// NOTE:  Every class that will be used as an Asset<T> type, must expose the following public static methods:
	//
	//	static const AssetType::Enum ASSET_TYPE = AssetType::Texture2D;		// Specifies the type ID of the asset
	//	static TYPE* FromAsset(AssetLoadContext* context)					// Creates a new instance of the asset from the given context
	//  void ReleaseChildAssets();											// Tells the asset to release any child assets it loaded

	// ============================================================================

	namespace AssetState
	{
		enum Enum
		{
			Created,
			Loading,
			Ready,
			LoadFailed,
		};
	}

	// ============================================================================

	class AssetLoadContext
	{
	public:
		// Properties
		const char*				AssetName;			// Name (and path) of the asset
		Stream*					AssetStream;		// Stream containing the asset data
		class AssetBase*		Asset;				// The asset to load 'into'
		class AssetProvider*	Provider;			// Provider that is serving the asset
		class AssetManager*		Manager;			// Asset Manager that will manage the asset (and MUST be used for child assets)

		// CTor
		AssetLoadContext() {}
		AssetLoadContext(AssetBase* asset, const char* name, Stream* assetStream, AssetProvider* provider, AssetManager* manager)
		{
			Asset = asset;
			AssetName = name;
			AssetStream = assetStream;
			Provider = provider;
			Manager = manager;
		}
	};

	// ============================================================================

	class AssetBase
	{
		// Only AssetManager can create Asset instances
		friend class AssetManager;

	protected:
		
		// Internals
		// ------------------------

		class AssetManager*		manager;
		AssetType::Enum			type;
		AssetState::Enum		state;
		string					name;
		int						referenceCount;
		class AssetProvider*	provider;

		// CTor
		AssetBase(AssetManager* assetManager, const char* assetName, AssetType::Enum type);

		// Virtuals
		virtual void InternalLoadAsset(AssetLoadContext* context) = 0;
		virtual void ReleaseChildAssets() = 0;

	public:

		// Publics
		// ------------------------

		AssetType::Enum GetType()			{ return this->type; }
		AssetState::Enum GetState()			{ return this->state; }
		const string& GetName()				{ return this->name; }
		class AssetProvider* GetProvider()	{ return this->provider; }
		class AssetManager* GetManager()	{ return this->manager; }
		bool IsLoaded()						{ return this->state == AssetState::Ready; }

		// Reference counting
		int GetReferenceCount()		{ return this->referenceCount; }
		int AddRef();
		int Release();

		// Hashing 
		UInt32 GetHashCode()		{ return StringUtilities::FastHash(this->name.c_str()); }

		// Virtuals
		virtual void* GetUnknownInstance() = 0;
		
		// DTor
		virtual ~AssetBase() {}
	};

	// ============================================================================

	template<class T>
	class Asset : public AssetBase
	{
	protected:
		// Internals
		// --------------------

		T* instance;

		// Only AssetManager can create Asset<T> instances
		friend class AssetManager;

		// CTor
		Asset(AssetManager* assetManager, const char* assetName)
			: AssetBase(assetManager, assetName, T::ASSET_TYPE) 
		{
			this->instance = NULL;
		}

		// Causes the actual asset data to be loaded from the given stream
		void InternalLoadAsset(AssetLoadContext* context)
		{
			// Call the asset's FromAsset() method
			this->instance = T::FromAsset(context);
		}

		// Tells the asset to release it's child assets
		void ReleaseChildAssets()
		{
			this->instance->ReleaseChildAssets();
		}

	public:
		
		// Gets the proper typed pointer to the actual asset
		T* GetInstance() { return instance; }

		// Gets a void* to the actual asset
		virtual void* GetUnknownInstance() { return (void*) instance; }

		// DTor
		virtual ~Asset()
		{ 
			// Delete the instance
			GdkDelete(instance); 
		}
	};

	
} // namespace 
