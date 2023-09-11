#pragma once

#include <Windows.h>

#include "Vector2D.h"

#include <vector>

namespace ZeldaEngine
{
	class IGraphics;
	class CoreSystem;
	class GraphicsResourceManager;
	class Renderer;
	class UIRenderer;
	class Camera;
	class PhysicsSystem;
	class Light;

	class GraphicsSystem
	{
	public:
		void Initialize(HWND hwnd);
		void Finalize();

		ZeldaMath::Vector2D GetWindowSize();

		// 그래픽 시스템의 창크기를 변경한다. (실제 창크기를 바꾸는 함수가 아님)
		void ResizeWindow(unsigned int width, unsigned int height);

	private:
		void Render();
		void AddRenderer(Renderer* renderer);
		void RemoveRenderer(Renderer* renderer);

		void AddUIRenderer(UIRenderer* uirenderer);
		void RemoveUIRenderer(UIRenderer* uirenderer);

		void ChangedUILayer();

		void AddLight(Light* light);
		void RemoveLight(Light* light);

		void ShowFrameDebugInfo();

		IGraphics* graphics;

		bool isRendererSorted;
		bool isUIRendererSorted;

		std::vector<Light*> lightList;
		std::vector<Renderer*> rendererList;
		std::vector<UIRenderer*> uiRendererList;

		ZeldaMath::Vector2D windowSize;

		friend GraphicsResourceManager;
		friend CoreSystem;
		friend Renderer;
		friend UIRenderer;
		friend PhysicsSystem;
		friend Light;
		//friend GraphicsEngine::Direct2DSpriteSheet;


		//singleton
	public:
		static GraphicsSystem& GetInstance();

	private:
		GraphicsSystem();
		~GraphicsSystem();
		GraphicsSystem(const GraphicsSystem& ref);
		GraphicsSystem& operator=(const GraphicsSystem& ref) {}
	};
}