#include "RestartButton.h"
#include "Guilty.h"

void RestartButton::Awake()
{
	if (button == nullptr)
	{
		button = GetGameObject()->GetComponent<Button>();
	}
}

void RestartButton::Start()
{
	if (button != nullptr)
	{
		button->AddButtonEvent([this]()
			{
				if (!button->GetGameObject()->GetRootActive())
					return;
				SoundManager::GetInstance().PlaySFX(SoundManager::Button::Select);
				
				if (Guilty::GetInstance().bossPhase == 1)
				{
					SoundManager::GetInstance().PlayBGM(SoundManager::BGM::Phase1);
					LoadScene(L"GameScene");
				}
				else if (Guilty::GetInstance().bossPhase == 2)
				{
					SoundManager::GetInstance().PlayBGM(SoundManager::BGM::Phase1);
					LoadScene(L"BossPhase2Scene");
				}
				else if (Guilty::GetInstance().bossPhase == 3)
				{
					LoadScene(L"GameSceneBossPhase3");
				}
				else
				{
					assert(0);
				}
			}
		);
	}
}

void RestartButton::OnMouseEnter()
{
	mouseEnter = true;
}

void RestartButton::OnMouseExit()
{
	mouseExit = true;
}

void RestartButton::SetAttachUI(const AttachUI& attachUI)
{
	this->attachUI = const_cast<AttachUI*>(&attachUI);
}

void RestartButton::SetUISize(const Vector2D& size)
{
	UISize = size;
}

void RestartButton::SetAttachUISize(const Vector2D& size)
{
	attachUISize = size;
}

void RestartButton::PreSerialize(json& jsonData) const
{
	PRESERIALIZE_BASE(RestartButton);

	PRESERIALIZE_SERVALUE(UISize);
	PRESERIALIZE_SERVALUE(attachUISize);
}

void RestartButton::PreDeserialize(const json& jsonData)
{
	PREDESERIALIZE_BASE(RestartButton);

	PREDESERIALIZE_SERVALUE(UISize);
	PREDESERIALIZE_SERVALUE(attachUISize);
}

void RestartButton::PostSerialize(json& jsonData) const
{
	POSTSERIALIZE_BASE();

	POSTSERIALIZE_PTR(button);
	POSTSERIALIZE_PTR(attachUI);
}

void RestartButton::PostDeserialize(const json& jsonData)
{
	POSTDESERIALIZE_BASE();

	POSTDESERIALIZE_PTR(button);
	POSTDESERIALIZE_PTR(attachUI);
}
