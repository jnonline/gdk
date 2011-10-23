/* 
 * Copyright (c) 2011, Raincity Games LLC
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.php
 */

#pragma once

// =====================================================================
// GDK Library Headers

// System
#include "System/Logging.h"
#include "System/Assert.h"
#include "System/Memory.h"
#include "System/Delegates.h"
#include "System/StringUtilities.h"

// System/Containers
#include "System/Containers/SortedVector.h"
#include "System/Containers/StringHashMap.h"

// System/Threading
#include "System/Threading/Thread.h"
#include "System/Threading/CriticalSection.h"
#include "System/Threading/Mutex.h"
#include "System/Threading/Event.h"

// System/Time
#include "System/Time/HighResTimer.h"

// Application
#include "Application/PlatformInterfaces.h"
#include "Application/Application.h"
#include "Application/Game.h"

// IO
#include "IO/Stream.h"
#include "IO/FileStream.h"
#include "IO/MemoryStream.h"
#include "IO/File.h"
#include "IO/Directory.h"
#include "IO/Path.h"

// Asset
#include "Assets/AssetType.h"
#include "Assets/Asset.h"
#include "Assets/AssetProvider.h"
#include "Assets/AssetManager.h"
#include "Assets/AssetPool.h"
#include "Assets/FileAssetProvider.h"
#include "Assets/SharedAssets.h"

// Math
#include "Math/Maths.h"

// Math / Random
#include "Math/Random/Random.h"
#include "Math/Random/MersenneTwister.h"
#include "Math/Random/PerlinNoise.h"

// Math / 2D
#include "Math/2D/Vector2.h"
#include "Math/2D/Matrix2D.h"
#include "Math/2D/Ray2.h"
#include "Math/2D/Segment2.h"
#include "Math/2D/Rectangle2.h"
#include "Math/2D/Box2.h"
#include "Math/2D/Triangle2.h"
#include "Math/2D/Circle2.h"
#include "Math/2D/Distance2.h"
#include "Math/2D/Intersection2.h"

// Math / 3D
#include "Math/3D/Vector3.h"
#include "Math/3D/Matrix3D.h"
#include "Math/3D/Quaternion.h"
#include "Math/3D/Ray3.h"
#include "Math/3D/Segment3.h"
#include "Math/3D/Plane3.h"
#include "Math/3D/Sphere3.h"
#include "Math/3D/Box3.h"
#include "Math/3D/Capsule3.h"
#include "Math/3D/Frustum3.h"
#include "Math/3D/Distance3.h"
#include "Math/3D/Intersection3.h"

// Input
#include "Input/Device.h"
#include "Input/Keys.h"
#include "Input/Keyboard.h"
#include "Input/Mouse.h"
#include "Input/Touch.h"
#include "Input/MotionInput.h"

// Graphics
#include "Graphics/Color.h"
#include "Graphics/Enums.h"
#include "Graphics/Texture2D.h"
#include "Graphics/Graphics.h"

// Graphics/Shader
#include "Graphics/Shader/UniformTypes.h"
#include "Graphics/Shader/UniformValue.h"
#include "Graphics/Shader/ShaderUniform.h"
#include "Graphics/Shader/ShaderAttribute.h"
#include "Graphics/Shader/SharedUniformValueSet.h"
#include "Graphics/Shader/ShaderTechnique.h"
#include "Graphics/Shader/Shader.h"

// Graphics/2D
#include "Graphics/2D/Atlas.h"
#include "Graphics/2D/BMFont.h"
#include "Graphics/2D/Sprite.h"
#include "Graphics/2D/ParticleSystem2D.h"
#include "Graphics/2D/Renderer2D.h"

// Graphics/3D
#include "Graphics/3D/BillboardRenderer.h"
#include "Graphics/3D/BillboardParticleSystem.h"

// Graphics/Model
#include "Graphics/Model/Model.h"
#include "Graphics/Model/ModelNode.h"
#include "Graphics/Model/ModelMaterial.h"
#include "Graphics/Model/ModelMesh.h"
#include "Graphics/Model/ModelMeshInstance.h"
#include "Graphics/Model/ModelInstance.h"
