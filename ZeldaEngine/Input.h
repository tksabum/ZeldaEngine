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

		// Ű�� ���ȴ����� �����Ѵ�.
		bool PressedKey(int keycode, bool ignoreActive = false);
		// Ű�� ���� ���ȴ����� �����Ѵ�.
		bool PressedKeyNow(int keycode, bool ignoreActive = false);
		// Ű�� ���������� �����Ѵ�.
		bool ReleasedKey(int keycode, bool ignoreActive = false);
		// Ű�� ���� ���������� �����Ѵ�.
		bool ReleasedKeyNow(int keycode, bool ignoreActive = false);

		// ����Ű�� �ϳ��̻��̸� true
		bool PressedAnyKey();
		// ���� ����Ű�� �ϳ� �̻��̸� true
		bool PressedAnyKeyNow();

		// ���콺 ���� ��ư�� ���������� ���� ��ġ
		ZeldaMath::Vector2D GetMouseLeftPressedPosition();
		// ���콺 ���� ��ư�� ���������� �� ��ġ
		ZeldaMath::Vector2D GetMouseLeftReleasedPosition();
		// ���콺 ������ ��ư�� ���������� ���� ��ġ
		ZeldaMath::Vector2D GetMouseRightPressedPosition();
		// ���콺 ������ ��ư�� ���������� �� ��ġ
		ZeldaMath::Vector2D GetMouseRightReleasedPosition();
		// ���콺 ��ġ
		ZeldaMath::Vector2D GetMousePosition();

		void SetKeyActive(bool value);
		bool GetKeyActive();

		void SetMouseActive(bool value);
		bool GetMouseActive();

		// ���ӿ��� ����� Ű�� ����Ѵ�. Initialize�� ����Ǳ� ������ ��� ����
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