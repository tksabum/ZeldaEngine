#pragma once

#include <vector>

namespace ZeldaEngine
{
	class ZeldaUI;

	class UISystem
	{
	public:
		void AddUI(ZeldaUI* zeldaUI);
		void RemoveUI(ZeldaUI* zeldaUI);

	private:
		std::vector<ZeldaUI*> uiList;

		std::vector<bool> lastMouseOver;
		std::vector<bool> currentMouseOver;

		HWND hWnd;

		void Initialize(HWND hwnd);
		void UIEvent();

		friend class CoreSystem;

		//singleton
	public:
		static UISystem& GetInstance();

	private:
		UISystem();
		~UISystem();
		UISystem(const UISystem& ref) = delete;
		UISystem& operator=(const UISystem& ref) = delete;
	};
}