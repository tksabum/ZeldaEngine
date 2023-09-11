#include "pch.h"
#include "CoreSystem.h"

#include "TimeController.h"
#include "Input.h"
#include "GraphicsSystem.h"
#include "EngineSetting.h"
#include "SceneManager.h"
#include "PhysicsSystem.h"
#include "PhysicsSystem2D.h"
#include "UISystem.h"
#include "AnimationSystem.h"

#include "SoundManager.h"

#include <fstream>

namespace ZeldaEngine
{
	using namespace GraphicsEngine;
	using namespace ZeldaMath;

	void CoreSystem::Initialize(_In_ HINSTANCE hInstance, LPCWSTR gamename, unsigned int width, unsigned int height, bool screenresizeable, bool notitlebar, bool minimizable, bool maximizable)
	{
		// ���� �� �����츦 ���
		WNDCLASSEXW wcex;
		wcex.cbSize = sizeof(WNDCLASSEX);

		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = WndProc; // ���ν���
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = 0;
		wcex.hInstance = hInstance;
		wcex.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
		wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
		wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		wcex.lpszMenuName = NULL;
		wcex.lpszClassName = gamename;
		wcex.hIconSm = NULL;

		RegisterClassExW(&wcex);

		// �� �����츦 ����
		hWnd = CreateWindowW(
			gamename,
			gamename,
			screenresizeable ? WS_OVERLAPPEDWINDOW : (notitlebar ? (WS_POPUP) : (WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | (minimizable ? WS_MINIMIZEBOX : 0L) | (maximizable ? WS_MAXIMIZEBOX : 0L))), // WS_EX_TOPMOST | WS_POPUP,
			0,
			0,
			width,
			height,
			NULL,
			NULL,
			hInstance,
			NULL);

		SetMenu(hWnd, NULL);

		resolutionX = width;
		resolutionY = height;
		lastWidth = width;
		lastHeight = height;

		isFullScreenMode = false;
		resizable = screenresizeable;

		TimeController::GetInstance().Initialize("INITIALIZATION_START", "PHYSICS_LOOP_START");

		SceneManager::GetInstance().Initialize();

		GraphicsSystem::GetInstance().Initialize(hWnd);

		Input::GetInstance().RegisterGameKey(VK_CONTROL);
		Input::GetInstance().RegisterGameKey(VK_F12);
		Input::GetInstance().RegisterGameKey('Z');
		Input::GetInstance().RegisterGameKey('X');
		Input::GetInstance().RegisterGameKey('C');
		Input::GetInstance().Initialize(hWnd);

		UISystem::GetInstance().Initialize(hWnd);

		PhysicsSystem::GetInstance().Initialize();
		PhysicsSystem2D::GetInstance().Initialize();
	}

	void CoreSystem::Finalize()
	{
		GraphicsSystem::GetInstance().Finalize();
	}

	void CoreSystem::Run(_In_ int nCmdShow)
	{
		// ������ �ڵ��� ���� �߿�
		if (!hWnd)
		{
			return;
		}

		ShowWindow(hWnd, nCmdShow);
		UpdateWindow(hWnd);

		// ���ѷ��� (�ȿ� �޽��� ������ ���� ���� ���� ���Ե�)
		MSG msg;

		isRun = true;

		while (TRUE)
		{
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				if (msg.message == WM_QUIT) break;
				DispatchMessage(&msg);
			}
			else
			{
				// ���� ����
				run();
			}
		}

		isRun = false;
	}

	Vector2D CoreSystem::GetCurrentWindowSize()
	{
		// ����Լ��� �Űܾ���
		bool fullscreen = false;

		if (fullscreen)
		{
			return Vector2D(static_cast<float>(GetSystemMetrics(SM_CXSCREEN)), static_cast<float>(GetSystemMetrics(SM_CYSCREEN)));
		}
		else
		{
			RECT r;
			GetClientRect(hWnd, &r);

			return Vector2D(static_cast<float>(r.right), static_cast<float>(r.bottom));
		}
	}

	Vector2D CoreSystem::GetOriginWindowSize()
	{
		return Vector2D(static_cast<float>(resolutionX), static_cast<float>(resolutionY));
	}

	void CoreSystem::ChangeToFullScreenMode()
	{
		if (isFullScreenMode == true)
		{
			return;
		}
		isFullScreenMode = true;

		RECT r;
		GetClientRect(hWnd, &r);
		lastWidth = r.right - r.left;
		lastHeight = r.bottom - r.top;

		SetWindowLongPtr(hWnd, GWL_STYLE, WS_EX_TOPMOST | WS_POPUP); //3d argument=style
		SetWindowPos(hWnd, NULL, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN), SWP_SHOWWINDOW);
	}

	void CoreSystem::ChangeToWindowMode()
	{
		if (isFullScreenMode == false)
		{
			return;
		}
		isFullScreenMode = false;

		SetWindowLongPtr(hWnd, GWL_STYLE, resizable ? WS_OVERLAPPEDWINDOW : (WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX)); //3d argument=style
		SetWindowPos(hWnd, NULL, (GetSystemMetrics(SM_CXSCREEN) - lastWidth + 16) / 2, (GetSystemMetrics(SM_CYSCREEN) - lastHeight + 39) / 2, lastWidth + 16, lastHeight + 39, SWP_SHOWWINDOW);
	}

	void CoreSystem::ChangeWindowSize(unsigned int width, unsigned int height)
	{
		if (isFullScreenMode == true || (lastWidth == width && lastHeight == height))
		{
			return;
		}
		lastWidth = width;
		lastHeight = height;

		SetWindowLongPtr(hWnd, GWL_STYLE, resizable ? WS_OVERLAPPEDWINDOW : (WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX)); //3d argument=style
		SetWindowPos(hWnd, NULL, (GetSystemMetrics(SM_CXSCREEN) - width + 16) / 2, (GetSystemMetrics(SM_CYSCREEN) - height + 39) / 2, width + 16, height + 39, SWP_SHOWWINDOW);
	}

	void CoreSystem::SetResizable(bool value)
	{
		if (resizable == value)
		{
			return;
		}

		resizable = value;

		if (isFullScreenMode == false)
		{
			ChangeToFullScreenMode();
			ChangeToWindowMode();
		}
	}

	unsigned int CoreSystem::GetScreenWidth()
	{
		return lastWidth;
	}

	unsigned int CoreSystem::GetScreenHeight()
	{
		return lastHeight;
	}

	bool CoreSystem::IsFullScreenMode()
	{
		return isFullScreenMode;
	}

	bool CoreSystem::IsRun()
	{
		return isRun;
	}

	void CoreSystem::LoadScene(std::wstring fileName)
	{
		pendingSceneFile = fileName;
	}

	void CoreSystem::run()
	{
		if (runningState == RunningState::INITIALIZATION)
		{
			loadScene();

			TimeController::GetInstance().Update("INITIALIZATION_START");

			SceneManager::GetInstance().InitializeEvent();

			runningState = RunningState::DESTROY;
		}
		else if (runningState == RunningState::DESTROY)
		{
			SceneManager::GetInstance().DestroyEvent();

			runningState = RunningState::PHYSICS;
		}
		else if (runningState == RunningState::PHYSICS)
		{
			TimeController::GetInstance().Update("PHYSICS_LOOP_START");

			PhysicsSystem::GetInstance().PrePhysicsEvent();
			PhysicsSystem::GetInstance().CollisionDetection();
			PhysicsSystem::GetInstance().PhysicsEvent();
			PhysicsSystem::GetInstance().CollisionEvent();

			PhysicsSystem2D::GetInstance().PrePhysicsEvent();
			PhysicsSystem2D::GetInstance().CollisionDetection();
			PhysicsSystem2D::GetInstance().PhysicsEvent();
			PhysicsSystem2D::GetInstance().CollisionEvent();

			if (firstRun) // || time
			{
				TimeController::GetInstance().Update("PHYSICS_LOOP_END");

				runningState = RunningState::INPUTEVENT;
			}
			else
			{
				float timedif = TimeController::GetInstance().GetTimeDif("PHYSICS_LOOP_END");
				float physicslooptime = TimeController::GetInstance().GetTimeDif("PHYSICS_LOOP_START");
				if (timedif + max(physicslooptime, TimeController::GetInstance().GetPhysicsLoopTime()) > (1.0f / (float)TimeController::GetInstance().GetFPS()))
				{
					TimeController::GetInstance().Update("PHYSICS_LOOP_END");

					runningState = RunningState::INPUTEVENT;
				}
				else
				{
					if (physicslooptime < TimeController::GetInstance().GetPhysicsLoopTime())
					{
						runningState = RunningState::PHYSICSWAIT;
					}
				}
			}
		}
		else if (runningState == RunningState::PHYSICSWAIT)
		{
			if (TimeController::GetInstance().GetTimeDif("PHYSICS_LOOP_START") >= TimeController::GetInstance().GetPhysicsLoopTime())
			{
				runningState = RunningState::PHYSICS;
			}
		}
		else if (runningState == RunningState::INPUTEVENT)
		{
			SoundManager::GetInstance().Update();
			Input::GetInstance().Update();

			if (EngineSetting::GetInstance().GetDebugToggleKeyAllowed() && Input::GetInstance().PressedKey(VK_CONTROL) && Input::GetInstance().PressedKeyNow(VK_F12))
			{
				EngineSetting::GetInstance().SetShowDebugInfo(!EngineSetting::GetInstance().GetShowDebugInfo());
			}

			runningState = RunningState::GAMELOGIC;
		}
		else if (runningState == RunningState::GAMELOGIC)
		{
			SceneManager::GetInstance().EarlyUpdateEvent();
			SceneManager::GetInstance().UpdateEvent();

			AnimationSystem::GetInstance().ExecuteAllAnimator();

			SceneManager::GetInstance().LateUpdateEvent();

			runningState = RunningState::GUIEVENT;
		}
		else if (runningState == RunningState::GUIEVENT)
		{
			UISystem::GetInstance().UIEvent();

			runningState = RunningState::RENDER;
		}
		else if (runningState == RunningState::RENDER)
		{
			GraphicsSystem::GetInstance().Render();

			firstRun = false;
			TimeController::GetInstance().CountFrameNumber();
			runningState = RunningState::INITIALIZATION;



			// �ӽ÷� Ÿ��Ʋ�� FPS�� ����.
			if (TimeController::GetInstance().GetDeltaTime() != 0.0f && TimeController::GetInstance().GetPhysicsDeltaTime() != 0.0f)
			{
				static float elapsedTime = 0.0f;
				elapsedTime += TimeController::GetInstance().GetDeltaTime();

				static float frameCount = 1.0f;
				frameCount += 1.0f;

				const static float period = 0.5f;

				if (elapsedTime > period)
				{
					//SetWindowText(hWnd, (L"FPS: " + std::to_wstring(frameCount / elapsedTime) + L"          PhysicsFPS: " + std::to_wstring(1.0f / TimeController::GetInstance().GetPhysicsDeltaTime())).c_str());

					elapsedTime = 0.0f;
					frameCount = 0.0f;
				}
			}
		}
	}

	void CoreSystem::loadScene()
	{
		if (pendingSceneFile == L"")
		{
			return;
		}

		// JSON ���Ͽ��� ������ �б�
		std::ifstream inputFile(EngineSetting::GetInstance().GetGameName() + L"//SceneData//" + pendingSceneFile + L".scene");
		if (inputFile.is_open())
		{
			// ��� �� ������ ã�µ� ������ ��쿡�� ���ʱ�ȭ
			SceneManager::GetInstance().DeleteAllGameObject();

			PhysicsSystem::GetInstance().Reset();
			PhysicsSystem2D::GetInstance().Reset();

			json jsonData;
			inputFile >> jsonData;

			// JSON ������ȭ
			SceneManager::GetInstance().Deserialize(jsonData);

			inputFile.close();
		}
		else
		{
			assert(0, L"���� ���� ����");
		}

		pendingSceneFile = L""; // ���ϸ� �ʱ�ȭ
	}

	LRESULT CoreSystem::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch (message)
		{
			case WM_MOUSEWHEEL:
			{
				Input::GetInstance().updateWheelDelta(GET_WHEEL_DELTA_WPARAM(wParam));
			}

			case WM_PAINT:
			{
				PAINTSTRUCT ps;
				HDC hdc = BeginPaint(hWnd, &ps);
				// TODO: ���⿡ hdc�� ����ϴ� �׸��� �ڵ带 �߰��մϴ�...
				EndPaint(hWnd, &ps);
				break;
			}

			case WM_ENTERSIZEMOVE:
			{
				TimeController::GetInstance().MoveWindow();

				break;
			}

			case WM_MOVE:
			{
				TimeController::GetInstance().MoveWindow();

				break;
			}

			case WM_EXITSIZEMOVE:
			{
				TimeController::GetInstance().MoveWindow();

				break;
			}

			case WM_DESTROY:
			{
				PostQuitMessage(0);
				break;
			}

			//case WM_SYSCOMMAND:
			//{
			//	if ((wParam & 0xFFF0) == SC_KEYMENU)
			//	{
			//		return 0;
			//	}
			//	break;
			//}

			case WM_SYSKEYDOWN:
			{
				if (wParam == VK_MENU)
				{

					return 0;
				}

				if ((wParam & 0xFFF0) == SC_KEYMENU)
				{
					return 0;
				}

				break;
			}

			case WM_SIZE:
			{
				UINT width = LOWORD(lParam);
				UINT height = HIWORD(lParam);

				GraphicsSystem::GetInstance().ResizeWindow(width, height);
			}

			default:
			{
				return DefWindowProc(hWnd, message, wParam, lParam);
			}
		}
		return 0;
	}

	CoreSystem& CoreSystem::GetInstance()
	{
		static CoreSystem instance;
		return instance;
	}

	CoreSystem::CoreSystem() : firstRun(true), runningState(RunningState::INITIALIZATION), isRun(false), pendingSceneFile(L"")
	{

	}

	CoreSystem::~CoreSystem()
	{

	}
}