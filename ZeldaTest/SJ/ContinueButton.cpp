#include "ContinueButton.h"

#include "SettingButton.h"

void ContinueButton::Awake()
{
	if (button == nullptr)
	{
		button = GetGameObject()->GetComponent<Button>();
	}
}

void ContinueButton::Start()
{
	if (button != nullptr)
	{
		button->AddButtonEvent([this]()
			{
				if (!button->GetGameObject()->GetRootActive())
				return;

				SoundManager::GetInstance().PlaySFX(SoundManager::Button::Select);

				if (settingButton)
				{
					settingButton->CloseSettingUI();
				}
			}
		);
	}
}


void ContinueButton::OnMouseEnter()
{
	mouseEnter = true;
}

void ContinueButton::OnMouseExit()
{
	mouseExit = true;
}

void ContinueButton::SetSettingButton(const SettingButton& settingButton)
{
	this->settingButton = const_cast<SettingButton*>(&settingButton);
}

void ContinueButton::SetAttachUI(const AttachUI& attachUI)
{
	this->attachUI = const_cast<AttachUI*>(&attachUI);
}

void ContinueButton::SetUISize(const Vector2D& size)
{
	UISize = size;
}

void ContinueButton::SetAttachUISize(const Vector2D& size)
{
	attachUISize = size;
}

void ContinueButton::PreSerialize(json& jsonData) const
{
	PRESERIALIZE_BASE(ContinueButton);

	PRESERIALIZE_SERVALUE(UISize);
	PRESERIALIZE_SERVALUE(attachUISize);
}

void ContinueButton::PreDeserialize(const json& jsonData)
{
	PREDESERIALIZE_BASE(ContinueButton);

	PREDESERIALIZE_SERVALUE(UISize);
	PREDESERIALIZE_SERVALUE(attachUISize);
}

void ContinueButton::PostSerialize(json& jsonData) const
{
	POSTSERIALIZE_BASE();

	POSTSERIALIZE_PTR(button);
	POSTSERIALIZE_PTR(settingButton);
	POSTSERIALIZE_PTR(attachUI);
}

void ContinueButton::PostDeserialize(const json& jsonData)
{
	POSTDESERIALIZE_BASE();

	POSTDESERIALIZE_PTR(button);
	POSTDESERIALIZE_PTR(settingButton);
	POSTDESERIALIZE_PTR(attachUI);
}
