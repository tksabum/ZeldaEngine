#include "NumUI.h"

void NumUI::Awake()
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

void NumUI::SetTexture(const std::wstring& imageName)
{
	if (image)
	{
		image->SetTexture(imageName);
	}
}

void NumUI::PreSerialize(json& jsonData) const
{
	PRESERIALIZE_BASE(NumUI);
}

void NumUI::PreDeserialize(const json& jsonData)
{
	PREDESERIALIZE_BASE(NumUI);
}

void NumUI::PostSerialize(json& jsonData) const
{
	POSTSERIALIZE_BASE();

	POSTSERIALIZE_PTR(ui);
	POSTSERIALIZE_PTR(image);
}

void NumUI::PostDeserialize(const json& jsonData)
{
	POSTDESERIALIZE_BASE();

	POSTDESERIALIZE_PTR(ui);
	POSTDESERIALIZE_PTR(image);
}
