#include "StartButton.h"

#include "SJ/AttachUI.h"

void StartButton::Awake()
{
	if (button == nullptr)
	{
		button = GetGameObject()->GetComponent<Button>();
	}
}

void StartButton::Start()
{
	if (button != nullptr)
	{
		button->AddButtonEvent([this]()
			{
				if (!button->GetGameObject()->GetRootActive())
				return;
				SoundManager::GetInstance().PlaySFX(SoundManager::Button::Select);
				LoadScene(L"IntroScene");
			}
		);
	}
}

void StartButton::OnMouseEnter()
{
	/// 마우스 이벤트 막음
	//if (attachUI != nullptr)
	//{
	//	attachUI->GetGameObject()->SetActive(true);
	//}

	mouseEnter = true;
}

void StartButton::OnMouseExit()
{
	/// 마우스 이벤트 막음
	//if (attachUI != nullptr)
	//{
	//	attachUI->GetGameObject()->SetActive(false);
	//}

	mouseExit = true;
}

void StartButton::SetAttachUI(const AttachUI& attachUI)
{
	this->attachUI = const_cast<AttachUI*>(&attachUI);
}

void StartButton::SetUISize(const Vector2D& size)
{
	UISize = size;
}

void StartButton::SetAttachUISize(const Vector2D& size)
{
	attachUISize = size;
}

void StartButton::PreSerialize(json& jsonData) const
{
	PRESERIALIZE_BASE(StartButton);

	PRESERIALIZE_SERVALUE(UISize);
	PRESERIALIZE_SERVALUE(attachUISize);
}

void StartButton::PreDeserialize(const json& jsonData)
{
	PREDESERIALIZE_BASE(StartButton);

	PREDESERIALIZE_SERVALUE(UISize);
	PREDESERIALIZE_SERVALUE(attachUISize);
}

void StartButton::PostSerialize(json& jsonData) const
{
	POSTSERIALIZE_BASE();

	POSTSERIALIZE_PTR(button);
	POSTSERIALIZE_PTR(attachUI);
}

void StartButton::PostDeserialize(const json& jsonData)
{
	POSTDESERIALIZE_BASE();

	POSTDESERIALIZE_PTR(button);
	POSTDESERIALIZE_PTR(attachUI);
}
