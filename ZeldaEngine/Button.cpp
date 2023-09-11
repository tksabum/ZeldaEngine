#include "pch.h"
#include "Button.h"

#include "GameObject.h"

#include "Image.h"

namespace ZeldaEngine
{
	Button::Button() : isMouseLeftDown(false), image(nullptr), idleImage(L""), enterImage(L""), clickImage(L"")
	{

	}

	void Button::AddButtonEvent(std::function<void()> eventfunction)
	{
		buttonEventList.push_back(eventfunction);
	}

	void Button::Awake()
	{
		if (image == nullptr)
		{
			image = GetGameObject()->GetComponent<Image>();
		}
	}

	void Button::OnMouseLeftDown()
	{
		if (image != nullptr && clickImage != L"")
		{
			image->SetTexture(clickImage);
		}

		isMouseLeftDown = true;
	}

	void Button::OnMouseLeftUp()
	{
		if (image != nullptr && enterImage != L"")
		{
			image->SetTexture(enterImage);
		}

		if (isMouseLeftDown == true)
		{
			for (int i = 0; i < buttonEventList.size(); i++)
			{
				buttonEventList[i]();
			}
		}

		isMouseLeftDown = false;
	}

	void Button::OnMouseEnter()
	{
		if (image != nullptr && enterImage != L"")
		{
			image->SetTexture(enterImage);
		}
	}

	void Button::OnMouseExit()
	{
		if (image != nullptr && idleImage != L"")
		{
			image->SetTexture(idleImage);
		}

		isMouseLeftDown = false;
	}

	void Button::SetImage(std::wstring idleimage, std::wstring enterimage, std::wstring clickimage)
	{
		idleImage = idleimage;
		enterImage = enterimage;
		clickImage = clickimage;
	}

	void Button::PreSerialize(json& jsonData) const
	{
		PRESERIALIZE_BASE(Button);

		PRESERIALIZE_WSTRVALUE(idleImage);
		PRESERIALIZE_WSTRVALUE(enterImage);
		PRESERIALIZE_WSTRVALUE(clickImage);
	}

	void Button::PreDeserialize(const json& jsonData)
	{
		PREDESERIALIZE_BASE(Button);

		PREDESERIALIZE_WSTRVALUE(idleImage);
		PREDESERIALIZE_WSTRVALUE(enterImage);
		PREDESERIALIZE_WSTRVALUE(clickImage);
	}

	void Button::PostSerialize(json& jsonData) const
	{
		POSTSERIALIZE_BASE();

		POSTSERIALIZE_PTR(image);
	}

	void Button::PostDeserialize(const json& jsonData)
	{
		POSTDESERIALIZE_BASE();

		POSTDESERIALIZE_PTR(image);
	}
}