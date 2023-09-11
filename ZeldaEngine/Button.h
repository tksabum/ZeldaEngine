#pragma once

#include "ZeldaEngineAPI.h"

#include "Component.h"

#include <string>
#include <vector>
#include <functional>

namespace ZeldaEngine
{
	class Image;

	class ZELDAENGINE_API Button : public Component
	{
	public:
		Button();

		// �Լ����
		void AddButtonEvent(std::function<void()> eventfunction);

		void Awake();

		void OnMouseLeftDown();
		void OnMouseLeftUp();
		void OnMouseEnter();
		void OnMouseExit();

		void SetImage(std::wstring idleimage, std::wstring enterimage, std::wstring clickimage);

	private:
		Image* image;

		bool isMouseLeftDown;
		std::wstring idleImage;
		std::wstring enterImage;
		std::wstring clickImage;

		std::vector<std::function<void()>> buttonEventList;

		// Component��(��) ���� ��ӵ�
		virtual void PreSerialize(json& jsonData) const override;
		virtual void PreDeserialize(const json& jsonData) override;
		virtual void PostSerialize(json& jsonData) const override;
		virtual void PostDeserialize(const json& jsonData) override;
	};
}