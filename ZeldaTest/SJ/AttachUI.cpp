#include "AttachUI.h"

void AttachUI::Awake()
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

void AttachUI::SetTexture(const std::wstring& imageName)
{
	if (image)
	{
		image->SetTexture(imageName);
	}
}

void AttachUI::PreSerialize(json& jsonData) const
{
	PRESERIALIZE_BASE(AttachUI);
}

void AttachUI::PreDeserialize(const json& jsonData)
{
	PREDESERIALIZE_BASE(AttachUI);
}

void AttachUI::PostSerialize(json& jsonData) const
{
	POSTSERIALIZE_BASE();

	POSTSERIALIZE_PTR(ui);
	POSTSERIALIZE_PTR(image);
}

void AttachUI::PostDeserialize(const json& jsonData)
{
	POSTDESERIALIZE_BASE();

	POSTDESERIALIZE_PTR(ui);
	POSTDESERIALIZE_PTR(image);
}
