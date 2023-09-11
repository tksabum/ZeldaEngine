#include "BaseUI.h"

void BaseUI::Awake()
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

void BaseUI::SetTexture(const std::wstring& imageName)
{
	if (image)
	{
		image->SetTexture(imageName);
	}
}

void BaseUI::PreSerialize(json& jsonData) const
{
	PRESERIALIZE_BASE(BaseUI);
}

void BaseUI::PreDeserialize(const json& jsonData)
{
	PREDESERIALIZE_BASE(BaseUI);
}

void BaseUI::PostSerialize(json& jsonData) const
{
	POSTSERIALIZE_BASE();

	POSTSERIALIZE_PTR(ui);
	POSTSERIALIZE_PTR(image);
}

void BaseUI::PostDeserialize(const json& jsonData)
{
	POSTDESERIALIZE_BASE();

	POSTDESERIALIZE_PTR(ui);
	POSTDESERIALIZE_PTR(image);
}
