#include "TitleButton.h"

#include "AttachUI.h"

void TitleButton::Awake()
{
	if (button == nullptr)
	{
		button = GetGameObject()->GetComponent<Button>();
	}
}

void TitleButton::Start()
{
	if (button != nullptr)
	{
		button->AddButtonEvent([this]()
			{
				if (!button->GetGameObject()->GetRootActive())
				return;

				// 일시정지 풀기
				TimeController::GetInstance().SetTimeScale(1.0f);
				Input::GetInstance().SetKeyActive(true);
				Input::GetInstance().SetMouseActive(true);

				SoundManager::GetInstance().PlaySFX(SoundManager::Button::Select);
				SoundManager::GetInstance().StopAll();

				SoundManager::GetInstance().PlayBGM(SoundManager::BGM::Opening);
				LoadScene(L"MainScene");
			}
		);
	}
}

void TitleButton::OnMouseEnter()
{
	mouseEnter = true;
}

void TitleButton::OnMouseExit()
{
	mouseExit = true;
}

void TitleButton::SetAttachUI(const AttachUI& attachUI)
{
	this->attachUI = const_cast<AttachUI*>(&attachUI);
}

void TitleButton::SetUISize(const Vector2D& size)
{
	UISize = size;
}

void TitleButton::SetAttachUISize(const Vector2D& size)
{
	attachUISize = size;
}

void TitleButton::PreSerialize(json& jsonData) const
{
	PRESERIALIZE_BASE(TitleButton);

	PRESERIALIZE_SERVALUE(UISize);
	PRESERIALIZE_SERVALUE(attachUISize);
}

void TitleButton::PreDeserialize(const json& jsonData)
{
	PREDESERIALIZE_BASE(TitleButton);

	PREDESERIALIZE_SERVALUE(UISize);
	PREDESERIALIZE_SERVALUE(attachUISize);
}

void TitleButton::PostSerialize(json& jsonData) const
{
	POSTSERIALIZE_BASE();

	POSTSERIALIZE_PTR(button);
	POSTSERIALIZE_PTR(attachUI);
}

void TitleButton::PostDeserialize(const json& jsonData)
{
	POSTDESERIALIZE_BASE();

	POSTDESERIALIZE_PTR(button);
	POSTDESERIALIZE_PTR(attachUI);
}
