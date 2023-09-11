#include "CoverUI.h"

void CoverUI::Awake()
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

void CoverUI::SetTexture(const std::wstring& imageName)
{
	if (image)
	{
		image->SetTexture(imageName);
	}
}

void CoverUI::PreSerialize(json& jsonData) const
{
	PRESERIALIZE_BASE(CoverUI);
}

void CoverUI::PreDeserialize(const json& jsonData)
{
	PREDESERIALIZE_BASE(CoverUI);
}

void CoverUI::PostSerialize(json& jsonData) const
{
	POSTSERIALIZE_BASE();

	POSTSERIALIZE_PTR(ui);
	POSTSERIALIZE_PTR(image);
}

void CoverUI::PostDeserialize(const json& jsonData)
{
	POSTDESERIALIZE_BASE();

	POSTDESERIALIZE_PTR(ui);
	POSTDESERIALIZE_PTR(image);
}
