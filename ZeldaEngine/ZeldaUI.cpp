#include "pch.h"
#include "ZeldaUI.h"

#include "GameObject.h"
#include "Transform.h"

#include "UISystem.h"

namespace ZeldaEngine
{
	void ZeldaUI::PostInitialize_()
	{
		GetGameObject()->GetTransform()->AddUIComponent();
		UISystem::GetInstance().AddUI(this);
	}

	ZeldaUI::~ZeldaUI()
	{
		GetGameObject()->GetTransform()->RemoveUIComponent();
		UISystem::GetInstance().RemoveUI(this);
	}

	void ZeldaUI::PreSerialize(json& jsonData) const
	{
		PRESERIALIZE_BASE(ZeldaUI);
	}

	void ZeldaUI::PreDeserialize(const json& jsonData)
	{
		PREDESERIALIZE_BASE(ZeldaUI);
	}

	void ZeldaUI::PostSerialize(json& jsonData) const
	{
		POSTSERIALIZE_BASE();
	}

	void ZeldaUI::PostDeserialize(const json& jsonData)
	{
		POSTDESERIALIZE_BASE();
	}
}