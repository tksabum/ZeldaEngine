#pragma once

#include "ZeldaEngineAPI.h"

#include "EngineSetting.h"

#include "ZeldaMath.h"

#include "CoreSystem.h"
#include "SceneManager.h"
#include "GameObject.h"

// util
#include "TimeController.h"
#include "Input.h"
#include "SoundManager.h"

// factory
#include "ComponentFactory.h"

// resource
#include "ResourceManager.h"

// component
#include "Component.h"
#include "Transform.h"
#include "Camera.h"
#include "Rigidbody.h"
#include "Animator.h"
#include "SpriteRenderer.h"
#include "SpriteAnimator.h"

// UI
#include "ZeldaUI.h"
#include "Image.h"
#include "Button.h"

// collider
#include "CubeCollider.h"
#include "SphereCollider.h"
#include "CylinderCollider.h"
#include "CapsuleCollider.h"
#include "Collider2D.h"

// light
#include "DirectionalLight.h"

// renderer
#include "MeshRenderer.h"
#include "SkinnedMeshRenderer.h"
#include "ParticleRenderer.h"

// test
#include "TestComponent.h"
#include "TestComponent2.h"

namespace ZeldaEngine
{
	ZELDAENGINE_API void Initialize(_In_ HINSTANCE hInstance, LPCWSTR gamename, _In_ int nCmdShow, unsigned int width, unsigned int height, bool screenresizeable = true, bool notitlebar = false, bool minimizable = true, bool maximizable = true);
	ZELDAENGINE_API void Finalize();
	ZELDAENGINE_API void Run();

	ZELDAENGINE_API void SaveScene();
	ZELDAENGINE_API void LoadScene(std::wstring fileName);

	ZELDAENGINE_API void SaveSetting();
	ZELDAENGINE_API void LoadSetting(std::wstring fileName);

	ZELDAENGINE_API void SaveResourceData(std::wstring fileName);
	ZELDAENGINE_API void LoadResourceData(std::wstring fileName);

	ZELDAENGINE_API GameObject* CreateGameObjectFromFBX(std::wstring fbxName);
	ZELDAENGINE_API FF::FFAnimationClip& GetAnimationClip(std::wstring animationName);
}