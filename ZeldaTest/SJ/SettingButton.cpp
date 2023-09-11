#include "SettingButton.h"

void SettingButton::Awake()
{
	if (button == nullptr)
	{
		button = GetGameObject()->GetComponent<Button>();
	}
}

void SettingButton::Start()
{
	if (button != nullptr)
	{
		button->AddButtonEvent([this]()
			{
				if (!button->GetGameObject()->GetRootActive())
				return;

				SoundManager::GetInstance().PlaySFX(SoundManager::Button::Option);

				if (isOpen)
				{
					CloseSettingUI();
				}
				else
				{
					OpenSettingUI();
				}
			}
		);
	}

	if (panel)
	{
		isOpen = panel->GetGameObject()->GetActive();
	}
}

void SettingButton::Update()
{
	if (Input::GetInstance().PressedKeyNow(VK_ESCAPE, true))
	{
		if (!isOpen && TimeController::GetInstance().GetTimeScale() != 0.0f)
		{
			OpenSettingUI();
		}
		else
		{
			CloseSettingUI();
		}
	}

	if (TimeController::GetInstance().GetDeltaTime() == 0)
	{
		GetGameObject()->GetComponent<Image>()->SetTexture(L"SettingUI_Idle");
	}
}

void SettingButton::OnMouseEnter()
{
	if (TimeController::GetInstance().GetDeltaTime() == 0)
	{
		return;
	}
	GetGameObject()->GetComponent<Image>()->SetTexture(L"SettingUI_Enter");
}

void SettingButton::OnMouseExit()
{
	GetGameObject()->GetComponent<Image>()->SetTexture(L"SettingUI_Idle");
}

void SettingButton::SetPanel(const Transform& panel)
{
	this->panel = const_cast<Transform*>(&panel);
}

void SettingButton::OpenSettingUI()
{
	if (!isOpen)
	{
		SoundManager::GetInstance().PlaySFX(SoundManager::Button::Select);
		TimeController::GetInstance().SetTimeScale(0.0f);
		Input::GetInstance().SetKeyActive(false);
		panel->GetGameObject()->SetActive(true);
		isOpen = true;
	}
}

void SettingButton::CloseSettingUI()
{
	if (isOpen)
	{
		SoundManager::GetInstance().PlaySFX(SoundManager::Button::Select);
		TimeController::GetInstance().SetTimeScale(1.0f);
		Input::GetInstance().SetKeyActive(true);
		panel->GetGameObject()->SetActive(false);
		isOpen = false;
	}
}

void SettingButton::PreSerialize(json& jsonData) const
{
	PRESERIALIZE_BASE(SettingButton);
}

void SettingButton::PreDeserialize(const json& jsonData)
{
	PREDESERIALIZE_BASE(SettingButton);
}

void SettingButton::PostSerialize(json& jsonData) const
{
	POSTSERIALIZE_BASE();

	POSTSERIALIZE_PTR(panel);
	POSTSERIALIZE_PTR(button);
}

void SettingButton::PostDeserialize(const json& jsonData)
{
	POSTDESERIALIZE_BASE();

	POSTDESERIALIZE_PTR(panel);
	POSTDESERIALIZE_PTR(button);
}
