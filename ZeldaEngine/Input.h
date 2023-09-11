#pragma once

#include "ZeldaEngineAPI.h"

#include <Windows.h>

#include "Vector2D.h"

namespace ZeldaEngine
{
	class CoreSystem;

	class ZELDAENGINE_API Input
	{
	public:
		void Initialize(HWND hwnd);

		void Update();

		int GetWheelDelta(bool ignoreActive = false);

		// 키의 눌렸는지를 리턴한다.
		bool PressedKey(int keycode, bool ignoreActive = false);
		// 키가 지금 눌렸는지를 리턴한다.
		bool PressedKeyNow(int keycode, bool ignoreActive = false);
		// 키가 떨어졌는지 리턴한다.
		bool ReleasedKey(int keycode, bool ignoreActive = false);
		// 키가 지금 떨어졌는지 리턴한다.
		bool ReleasedKeyNow(int keycode, bool ignoreActive = false);

		// 눌린키가 하나이상이면 true
		bool PressedAnyKey();
		// 지금 눌린키가 하나 이상이면 true
		bool PressedAnyKeyNow();

		// 마우스 왼쪽 버튼을 마지막으로 누른 위치
		ZeldaMath::Vector2D GetMouseLeftPressedPosition();
		// 마우스 왼쪽 버튼을 마지막으로 뗀 위치
		ZeldaMath::Vector2D GetMouseLeftReleasedPosition();
		// 마우스 오른쪽 버튼을 마지막으로 누른 위치
		ZeldaMath::Vector2D GetMouseRightPressedPosition();
		// 마우스 오른쪽 버튼을 마지막으로 뗀 위치
		ZeldaMath::Vector2D GetMouseRightReleasedPosition();
		// 마우스 위치
		ZeldaMath::Vector2D GetMousePosition();

		void SetKeyActive(bool value);
		bool GetKeyActive();

		void SetMouseActive(bool value);
		bool GetMouseActive();

		// 게임에서 사용할 키를 등록한다. Initialize가 실행되기 전에만 등록 가능
		void RegisterGameKey(int keycode);

	private:
		void updateWheelDelta(SHORT wheeldelta);

		bool usingKeyList[0xFF];
		std::vector<int> gameKeyList;

		bool currentAnyKeyDown;
		bool lastAnyKeyDown;
		bool currentKeyState[0xFF];
		bool lastKeyState[0xFF];
		ZeldaMath::Vector2D currentMousePosition;
		ZeldaMath::Vector2D mouseLeftPressedPosition;
		ZeldaMath::Vector2D mouseLeftReleasedPosition;
		ZeldaMath::Vector2D mouseRightPressedPosition;
		ZeldaMath::Vector2D mouseRightReleasedPosition;

		int wheelDelta;
		int currentWheel;

		bool isKeyActive;
		bool isMouseActive;

		HWND hWnd;


		//singleton
	private:
		Input();
		~Input();
		Input(const Input& ref);
		Input& operator=(const Input& ref) = delete;

	public:
		static Input& GetInstance();

		friend CoreSystem;
	};
}