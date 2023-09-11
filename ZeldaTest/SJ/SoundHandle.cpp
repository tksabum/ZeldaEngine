#include "SoundHandle.h"

void SoundHandle::Awake()
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

void SoundHandle::OnMouseLeftDown()
{
	nowLeftDown = true;
}

bool SoundHandle::IsMouseLeftDown()
{
	if (nowLeftDown)
	{
		nowLeftDown = false;
		return true;
	}

	return false;
}

void SoundHandle::PreSerialize(json& jsonData) const
{
	PRESERIALIZE_BASE(SoundHandle);
}

void SoundHandle::PreDeserialize(const json& jsonData)
{
	PREDESERIALIZE_BASE(SoundHandle);
}

void SoundHandle::PostSerialize(json& jsonData) const
{
	POSTSERIALIZE_BASE();

	POSTSERIALIZE_PTR(ui);
	POSTSERIALIZE_PTR(image);
}

void SoundHandle::PostDeserialize(const json& jsonData)
{
	POSTDESERIALIZE_BASE();

	POSTDESERIALIZE_PTR(ui);
	POSTDESERIALIZE_PTR(image);
}
