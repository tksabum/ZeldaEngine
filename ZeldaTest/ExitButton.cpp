#include "ExitButton.h"

#include "SJ/AttachUI.h"

void ExitButton::Awake()
{
	if (button == nullptr)
	{
		button = GetGameObject()->GetComponent<Button>();
	}
}

void ExitButton::Start()
{
	if (button != nullptr)
	{
		button->AddButtonEvent([this]()
			{
				if (!button->GetGameObject()->GetRootActive())
				return;
				SoundManager::GetInstance().PlaySFX(SoundManager::Button::Select);
				PostQuitMessage(0);
			}
		);
	}
}

void ExitButton::OnMouseEnter()
{
	/// 마우스 이벤트 막음
	//if (attachUI != nullptr)
	//{
	//	attachUI->GetGameObject()->SetActive(true);
	//}

	mouseEnter = true;
}

void ExitButton::OnMouseExit()
{
	/// 마우스 이벤트 막음
	//if (attachUI != nullptr)
	//{
	//	attachUI->GetGameObject()->SetActive(false);
	//}

	mouseExit = true;
}

void ExitButton::SetAttachUI(const AttachUI& attachUI)
{
	this->attachUI = const_cast<AttachUI*>(&attachUI);
}

void ExitButton::SetUISize(const Vector2D& size)
{
	UISize = size;
}

void ExitButton::SetAttachUISize(const Vector2D& size)
{
	attachUISize = size;
}

void ExitButton::PreSerialize(json& jsonData) const
{
	PRESERIALIZE_BASE(ExitButton);

	PRESERIALIZE_SERVALUE(UISize);
	PRESERIALIZE_SERVALUE(attachUISize);
}

void ExitButton::PreDeserialize(const json& jsonData)
{
	PREDESERIALIZE_BASE(ExitButton);

	PREDESERIALIZE_SERVALUE(UISize);
	PREDESERIALIZE_SERVALUE(attachUISize);
}

void ExitButton::PostSerialize(json& jsonData) const
{
	POSTSERIALIZE_BASE();

	POSTSERIALIZE_PTR(button);
	POSTSERIALIZE_PTR(attachUI);
}

void ExitButton::PostDeserialize(const json& jsonData)
{
	POSTDESERIALIZE_BASE();

	POSTDESERIALIZE_PTR(button);
	POSTDESERIALIZE_PTR(attachUI);
}
