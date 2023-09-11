#include "pch.h"
#include "GraphicsSystem.h"

#include <algorithm>

#include "EngineSetting.h"
#include "IGraphics.h"

#include "FFGraphics.h"

#include "GameObject.h"
#include "Renderer.h"
#include "UIRenderer.h"
#include "Transform.h"
#include "Camera.h"

#include "CoreSystem.h"
#include "PhysicsSystem.h"
#include "SceneManager.h"
#include "Light.h"
#include "ResourceManager.h"

#include "Input.h"
#include "TimeController.h"

#include <algorithm>

using namespace std;

namespace ZeldaEngine
{
	using namespace ZeldaMath;

	void GraphicsSystem::Initialize(HWND hwnd)
	{
		EngineSetting* engineSetting = &EngineSetting::GetInstance();

		if (graphics == nullptr)
		{
			if (engineSetting->graphicsEngine == EngineSetting::GraphicsEngine::FF_GRAPHICS)
			{
				graphics = new FFGraphics();
			}
			// default
			else
			{
				graphics = new FFGraphics();
			}
		}

		RECT rect;
		GetWindowRect(hwnd, &rect);
		windowSize = Vector2D(static_cast<float>(rect.right - rect.left), static_cast<float>(rect.bottom - rect.top));

		// long long 말고 void*로 바꿔줘....
		graphics->Initialize((long long)(hwnd), windowSize.x, windowSize.y, !CoreSystem::GetInstance().IsFullScreenMode());

		ResourceManager::GetInstance().Initialize(graphics);
	}

	void GraphicsSystem::Finalize()
	{
		graphics->Finalize();
	}

	Vector2D GraphicsSystem::GetWindowSize()
	{
		return windowSize;
	}

	void GraphicsSystem::ResizeWindow(unsigned int width, unsigned int height)
	{
		windowSize.x = static_cast<float>(width);
		windowSize.y = static_cast<float>(height);

		if (graphics != nullptr)
		{
			graphics->Resize(width, height);
		}
	}

	void GraphicsSystem::Render()
	{
		graphics->BeginDraw(TimeController::GetInstance().GetDeltaTime());

		Camera* maincamera = SceneManager::GetInstance().GetMainCamera();
		// 카메라가 없다면 아무것도 그리지 않음
		assert(maincamera != nullptr);
		if (maincamera == nullptr) return;

		Matrix4x4 cameraWorldTM = maincamera->GetGameObject()->GetTransform()->GetWorldMatrix();

		graphics->SetCameraInfo(maincamera->GetCameraID(), maincamera->GetCameraNear(), maincamera->GetCameraFar(), maincamera->GetCameraFOV(), maincamera->GetCameraScale(), windowSize.x, windowSize.y, cameraWorldTM);

		LightParams lightParams = {};
		for (size_t i = 0; i < lightList.size(); i++)
		{
			lightList[i]->SetLightIndex(lightParams.lightCount);
			lightParams.lights[lightParams.lightCount] = lightList[i]->GetLightInfo();
			lightParams.lightCount++;
		}
		FF::LightParams parmas = lightParams.ToFFLightParams();

		graphics->SetLightData(&parmas, sizeof(parmas));

		for (int i = 0; i < rendererList.size(); i++)
		{
			if (rendererList[i]->GetGameObject()->GetRootActive())
			{
				rendererList[i]->Render(graphics);
			}
		}

		for (int i = 0; i < lightList.size(); i++)
		{
			if (lightList[i]->GetGameObject()->GetRootActive())
			{
				lightList[i]->Render(graphics);
			}
		}

		if (EngineSetting::GetInstance().GetShowDebugInfo() == true)
		{
			PhysicsSystem::GetInstance().RenderDebuggingInfo(graphics);
		}

		if (isUIRendererSorted == false)
		{
			isUIRendererSorted = true;
			sort(uiRendererList.begin(), uiRendererList.end(), [](UIRenderer* r1, UIRenderer* r2) { return r1->GetLayer() < r2->GetLayer(); });
		}

		for (int i = 0; i < uiRendererList.size(); i++)
		{
			if (uiRendererList[i]->GetGameObject()->GetRootActive())
			{
				uiRendererList[i]->Render(graphics);
			}
		}

		//graphics->DrawTest();
		//
		//if (EngineSetting::GetInstance().GetShowDebugInfo())
		//{
		//	ShowFrameDebugInfo();
		//}

		if (EngineSetting::GetInstance().GetShowDebugInfo() == true)
		{
			graphics->DrawDeferredDebug();
		}

		graphics->EndDraw();
	}

	void GraphicsSystem::AddRenderer(Renderer* renderer)
	{
		isRendererSorted = false;

		rendererList.push_back(renderer);
	}

	void GraphicsSystem::RemoveRenderer(Renderer* renderer)
	{
		for (vector<Renderer*>::iterator iter = rendererList.begin(); iter != rendererList.end(); iter++)
		{
			if (*iter == renderer)
			{
				rendererList.erase(iter);
				break;
			}
		}
	}

	void GraphicsSystem::AddUIRenderer(UIRenderer* uirenderer)
	{
		isUIRendererSorted = false;

		uiRendererList.push_back(uirenderer);
	}

	void GraphicsSystem::RemoveUIRenderer(UIRenderer* uirenderer)
	{
		for (vector<UIRenderer*>::iterator iter = uiRendererList.begin(); iter != uiRendererList.end(); iter++)
		{
			if (*iter == uirenderer)
			{
				uiRendererList.erase(iter);
				break;
			}
		}
	}

	void GraphicsSystem::ChangedUILayer()
	{
		isUIRendererSorted = false;
	}

	void GraphicsSystem::AddLight(Light* light)
	{
		lightList.push_back(light);
	}

	void GraphicsSystem::RemoveLight(Light* light)
	{
		for (vector<Light*>::iterator iter = lightList.begin(); iter != lightList.end(); iter++)
		{
			if (*iter == light)
			{
				lightList.erase(iter);
				break;
			}
		}
	}

	void GraphicsSystem::ShowFrameDebugInfo()
	{
		const Vector3D pos = Vector3D(0, 10);

		static std::list<float> frameList;

		frameList.push_back(TimeController::GetInstance().GetDeltaTime());
		while (frameList.size() > 121)
		{
			frameList.pop_front();
		}

		int frameNum = 0;

		for (auto iter = frameList.begin(); iter != frameList.end(); iter++)
		{
			auto iterend = frameList.end();
			iterend--;
			if (iter == iterend)
			{
				break;
			}

			auto iter2 = iter;
			iter2++;


			Vector3D a = Vector3D(0.1f * frameNum - 6, (*iter) * 50 - 15.0f, 50);
			Vector3D b = Vector3D(0.1f * (frameNum + 1) - 6, (*iter2) * 50 - 15.0f, 50);

			a += pos;
			b += pos;

			Vector3D normal = b - a;
			Vector3D position = a;

			Vector3D originLine = Vector3D(1, 0, 0);

			Vector3D axis = CrossProduct(originLine, normal).Normalize();
			float angle = std::acos(DotProduct(originLine, normal.Normalize()));

			Matrix4x4 matrix = Matrix4x4::Translation(position) * Matrix4x4::Rotation(axis, angle) * Matrix4x4::Scale(normal.Size());

			Matrix4x4 cameraTM = SceneManager::GetInstance().GetMainCamera()->GetGameObject()->GetTransform()->GetWorldMatrix();
			matrix = cameraTM * matrix;

			static std::wstring lineName = ResourceManager::GetInstance().GetMaterial(L"BoundaryDebug_Line")->materialName;

			graphics->DrawMesh(matrix, L"Line", *ResourceManager::GetInstance().GetMaterial(L"BoundaryDebug_Line"), lineName, 0);

			frameNum++;
		}

		// Base Line
		{
			static std::wstring baselineName = ResourceManager::GetInstance().GetMaterial(L"BoundaryDebug_Line")->materialName;
			static MaterialInfo baselineMaterial = *ResourceManager::GetInstance().GetMaterial(L"BoundaryDebug_Line");
			//baselineMaterial.color = ZeldaColor::White;

			Vector3D a = Vector3D(-6, -15.0f, 50);
			Vector3D b = Vector3D(6, -15.0f, 50);

			a += pos;
			b += pos;

			Vector3D normal = b - a;
			Vector3D position = a;

			Vector3D originLine = Vector3D(1, 0, 0);

			Vector3D axis = CrossProduct(originLine, normal).Normalize();
			float angle = std::acos(DotProduct(originLine, normal.Normalize()));

			Matrix4x4 matrix = Matrix4x4::Translation(position) * Matrix4x4::Rotation(axis, angle) * Matrix4x4::Scale(normal.Size());

			Matrix4x4 cameraTM = SceneManager::GetInstance().GetMainCamera()->GetGameObject()->GetTransform()->GetWorldMatrix();
			matrix = cameraTM * matrix;

			graphics->DrawMesh(matrix, L"Line", baselineMaterial, baselineName, 0);
		}
	}

	GraphicsSystem& GraphicsSystem::GetInstance()
	{
		static GraphicsSystem instance;
		return instance;
	}

	GraphicsSystem::GraphicsSystem() : graphics(nullptr), isRendererSorted(false), isUIRendererSorted(false), windowSize(Vector2D())
	{

	}

	GraphicsSystem::~GraphicsSystem()
	{

	}
}