#pragma once

#include "ZeldaEngineAPI.h"

#include <Windows.h>

#include "Vector2D.h"
#include "Vector3D.h"

namespace ZeldaEngine
{
	class SceneManager;
	class TimeController;

	namespace GraphicsEngine
	{
		class Sprite;
		class SpriteSheet;
		class Graphics;
	}

	class CoreSystem
	{
	public:
		friend class GraphicsSystem;

		// Initialize Coresystem and Subsystem
		void Initialize(_In_ HINSTANCE hInstance, LPCWSTR gamename, unsigned int width, unsigned int height, bool screenresizeable = true, bool notitlebar = true, bool minimizable = true, bool maximizable = true);

		void Finalize();

		void Run(_In_ int nCmdShow);

		ZeldaMath::Vector2D GetCurrentWindowSize();
		ZeldaMath::Vector2D GetOriginWindowSize();

		void ChangeToFullScreenMode();
		void ChangeToWindowMode();
		void ChangeWindowSize(unsigned int width, unsigned int height);

		void SetResizable(bool value);

		unsigned int GetScreenWidth();
		unsigned int GetScreenHeight();
		bool IsFullScreenMode();

		bool IsRun();
		void LoadScene(std::wstring fileName);

	private:
		enum class RunningState
		{
			INITIALIZATION,
			DESTROY,
			PHYSICS,
			PHYSICSWAIT,
			INPUTEVENT,
			GAMELOGIC,
			GUIEVENT,
			RENDER
		};

		static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
		void run();
		void loadScene();

		std::wstring pendingSceneFile;

		HWND hWnd;

		bool isFullScreenMode;
		bool resizable;

		bool firstRun;
		bool isRun;
		RunningState runningState;

		// ÇØ»óµµ
		unsigned int resolutionX;
		unsigned int resolutionY;
		unsigned int lastWidth;
		unsigned int lastHeight;

		HCURSOR hCursor;

		//singleton
	public:
		static CoreSystem& GetInstance();

	private:
		CoreSystem();
		~CoreSystem();
		CoreSystem(const CoreSystem& ref);
		CoreSystem& operator=(const CoreSystem& ref) = delete;
	};
}