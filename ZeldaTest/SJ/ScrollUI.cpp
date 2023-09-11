#include "ScrollUI.h"

void ScrollUI::Awake()
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

void ScrollUI::PreSerialize(json& jsonData) const
{
	PRESERIALIZE_BASE(ScrollUI);
}

void ScrollUI::PreDeserialize(const json& jsonData)
{
	PREDESERIALIZE_BASE(ScrollUI);
}

void ScrollUI::PostSerialize(json& jsonData) const
{
	POSTSERIALIZE_BASE();

	POSTSERIALIZE_PTR(ui);
	POSTSERIALIZE_PTR(image);
}

void ScrollUI::PostDeserialize(const json& jsonData)
{
	POSTDESERIALIZE_BASE();

	POSTDESERIALIZE_PTR(ui);
	POSTDESERIALIZE_PTR(image);
}
