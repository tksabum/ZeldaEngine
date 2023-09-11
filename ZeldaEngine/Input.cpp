#include "pch.h"
#include "Input.h"

#include "GraphicsSystem.h"

namespace ZeldaEngine
{
	using namespace ZeldaMath;

	Input::Input() : wheelDelta(0), currentWheel(0), isKeyActive(true), isMouseActive(true), currentAnyKeyDown(false), lastAnyKeyDown(false), hWnd(NULL)
	{
		for (size_t i = 0; i < 255; i++)
		{
			lastKeyState[i] = false;
			currentKeyState[i] = false;
		}

		usingKeyList[0x01] = true;
		usingKeyList[0x02] = true;

		for (int i = 0x03; i < 0xFF; i++)
		{
			usingKeyList[i] = false;
		}
	}

	Input::~Input()
	{

	}

	Input& Input::GetInstance()
	{
		static Input instance;
		return instance;
	}

	void Input::Initialize(HWND hwnd)
	{
		hWnd = hwnd;

		if (gameKeyList.size() == 0)
		{
			for (int i = 0; i < 0xFF; i++)
			{
				if (usingKeyList[i])
				{
					gameKeyList.push_back(i);
				}
			}
		}
	}

	void Input::Update()
	{
		lastAnyKeyDown = currentAnyKeyDown;
		currentAnyKeyDown = false;

		for (int i = 0; i < gameKeyList.size(); i++)
		{
			int keycode = gameKeyList[i];

			lastKeyState[keycode] = currentKeyState[keycode];

			currentKeyState[keycode] = GetAsyncKeyState(keycode) & 0x8000;

			currentAnyKeyDown = currentAnyKeyDown || (currentKeyState[keycode] && (keycode < 0x07) ? (isMouseActive) : (isKeyActive));

			// 창이 비활성화된 경우 키 입력을 무시한다.
			if (hWnd != GetFocus())
			{
				currentKeyState[keycode] = false;
			}
		}

		POINT mousepoint;
		GetCursorPos(&mousepoint);
		ScreenToClient(hWnd, &mousepoint);
		currentMousePosition = Vector2D(static_cast<float>(mousepoint.x), static_cast<float>(mousepoint.y));

		if (!lastKeyState[VK_LBUTTON] && currentKeyState[VK_LBUTTON])
		{
			mouseLeftPressedPosition = currentMousePosition;
		}

		if (lastKeyState[VK_LBUTTON] && !currentKeyState[VK_LBUTTON])
		{
			mouseLeftReleasedPosition = currentMousePosition;
		}

		if (!lastKeyState[VK_RBUTTON] && currentKeyState[VK_RBUTTON])
		{
			mouseRightPressedPosition = currentMousePosition;
		}

		if (lastKeyState[VK_RBUTTON] && !currentKeyState[VK_RBUTTON])
		{
			mouseRightReleasedPosition = currentMousePosition;
		}

		wheelDelta = currentWheel;
		currentWheel = 0;
	}

	int Input::GetWheelDelta(bool ignoreActive)
	{
		if (!ignoreActive && !isMouseActive)
		{
			return 0;
		}

		return wheelDelta;
	}

	bool Input::PressedKey(int keycode, bool ignoreActive)
	{
		if (!ignoreActive && ((keycode < 0x07 && !isMouseActive) || (0x07 <= keycode && !isKeyActive)))
		{
			return false;
		}

		return currentKeyState[keycode];
	}

	bool Input::PressedKeyNow(int keycode, bool ignoreActive)
	{
		if (!ignoreActive && ((keycode < 0x07 && !isMouseActive) || (0x07 <= keycode && !isKeyActive)))
		{
			return false;
		}

		return currentKeyState[keycode] && !lastKeyState[keycode];
	}

	bool Input::ReleasedKey(int keycode, bool ignoreActive)
	{
		if (!ignoreActive && ((keycode < 0x07 && !isMouseActive) || (0x07 <= keycode && !isKeyActive)))
		{
			return false;
		}

		return !currentKeyState[keycode];
	}

	bool Input::ReleasedKeyNow(int keycode, bool ignoreActive)
	{
		if (!ignoreActive && ((keycode < 0x07 && !isMouseActive) || (0x07 <= keycode && !isKeyActive)))
		{
			return false;
		}

		return !currentKeyState[keycode] && lastKeyState[keycode];
	}

	bool Input::PressedAnyKey()
	{
		return currentAnyKeyDown;
	}

	bool Input::PressedAnyKeyNow()
	{
		return currentAnyKeyDown && !lastAnyKeyDown;
	}

	ZeldaMath::Vector2D Input::GetMouseLeftPressedPosition()
	{
		return mouseLeftPressedPosition;
	}

	ZeldaMath::Vector2D Input::GetMouseLeftReleasedPosition()
	{
		return mouseLeftReleasedPosition;
	}

	ZeldaMath::Vector2D Input::GetMouseRightPressedPosition()
	{
		return mouseRightPressedPosition;
	}

	ZeldaMath::Vector2D Input::GetMouseRightReleasedPosition()
	{
		return mouseRightReleasedPosition;
	}

	ZeldaMath::Vector2D Input::GetMousePosition()
	{
		return currentMousePosition;
	}

	void Input::SetKeyActive(bool value)
	{
		isKeyActive = value;
	}

	bool Input::GetKeyActive()
	{
		return isKeyActive;
	}

	void Input::SetMouseActive(bool value)
	{
		isMouseActive = value;
	}

	bool Input::GetMouseActive()
	{
		return isMouseActive;
	}

	void Input::RegisterGameKey(int keycode)
	{
		usingKeyList[keycode] = true;
	}

	void Input::updateWheelDelta(SHORT wheeldelta)
	{
		// 창이 비활성화된 경우 키 입력을 무시한다.
		if (hWnd != GetFocus())
		{
			return;
		}

		currentWheel += wheeldelta;
	}
}