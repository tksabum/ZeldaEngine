#include "MovableUI.h"

void MovableUI::Awake()
{
	if (ui == nullptr)
	{
		ui = GetGameObject()->GetComponent<ZeldaUI>();
	}

	if (image == nullptr)
	{
		image = GetGameObject()->GetComponent<Image>();
	}
}

void MovableUI::SetTexture(const std::wstring& imageName)
{
	if (image)
	{
		image->SetTexture(imageName);
	}
}

void MovableUI::PreSerialize(json& jsonData) const
{
	PRESERIALIZE_BASE(MovableUI);
}

void MovableUI::PreDeserialize(const json& jsonData)
{
	PREDESERIALIZE_BASE(MovableUI);
}

void MovableUI::PostSerialize(json& jsonData) const
{
	POSTSERIALIZE_BASE();

	POSTSERIALIZE_PTR(ui);
	POSTSERIALIZE_PTR(image);
}

void MovableUI::PostDeserialize(const json& jsonData)
{
	POSTDESERIALIZE_BASE();

	POSTDESERIALIZE_PTR(ui);
	POSTDESERIALIZE_PTR(image);
}
