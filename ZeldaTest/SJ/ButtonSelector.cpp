#include "ButtonSelector.h"

#include "../StartButton.h"
#include "../ExitButton.h"
#include "../RestartButton.h"
#include "ContinueButton.h"
#include "TitleButton.h"
#include "SettingButton.h"
#include "AttachUI.h"
#include "../Guilty.h"

#include <algorithm>

void ButtonSelector::Awake()
{
	if (buttonList.size() != 0 && firstButton == nullptr)
	{
		firstButton = buttonList[0];
	}
}

void ButtonSelector::Start()
{
	if (firstButton)
	{
		currentButton = firstButton;
		SelectButton(*currentButton);
	}
}

void ButtonSelector::LateUpdate()
{
	MouseEnterUpdate();

	if (SceneManager::GetInstance().GetName() == L"GameOverScene")
	{
		if (Input::GetInstance().PressedKeyNow(VK_RIGHT, true) && currentButton != *(--buttonList.end()))
		{
			DeselectButton(*currentButton);
			currentButton = GetNextButton(*currentButton);
			SelectButton(*currentButton);

		}
		else if (Input::GetInstance().PressedKeyNow(VK_LEFT, true) && currentButton != firstButton)
		{
			DeselectButton(*currentButton);
			currentButton = GetBeforeButton(*currentButton);
			SelectButton(*currentButton);
		}
	}
	else
	{
		if (Input::GetInstance().PressedKeyNow(VK_DOWN, true) && currentButton != *(--buttonList.end()))
		{
			DeselectButton(*currentButton);
			currentButton = GetNextButton(*currentButton);
			SelectButton(*currentButton);

		}
		else if (Input::GetInstance().PressedKeyNow(VK_UP, true) && currentButton != firstButton)
		{
			DeselectButton(*currentButton);
			currentButton = GetBeforeButton(*currentButton);
			SelectButton(*currentButton);
		}
	}

	if (Input::GetInstance().PressedKeyNow(VK_RETURN, true))
	{
		InvokeButton(*currentButton);
	}
}

void ButtonSelector::AddButton(const GameObject& obj)
{
	buttonList.push_back(const_cast<GameObject*>(&obj));
}

bool ButtonSelector::SetFirstButton(const GameObject& obj)
{
	auto itr = std::find(buttonList.begin(), buttonList.end(), const_cast<GameObject*>(&obj));

	if (itr == buttonList.end())
	{
		return false;
	}
	else
	{
		buttonList.erase(itr);
		buttonList.insert(buttonList.begin(), const_cast<GameObject*>(&obj));
		firstButton = const_cast<GameObject*>(&obj);
		return true;
	}
}

void ButtonSelector::SelectButton(GameObject& obj)
{
	switch (GetButtonType(obj))
	{
		case ButtonType::Start:
		{
			SoundManager::GetInstance().StopAllSFX();
			SoundManager::GetInstance().PlaySFX(SoundManager::Button::Hover);

			auto image = obj.GetComponent<Image>();
			image->SetTexture(L"Start_Enter");

			auto ui = obj.GetComponent<StartButton>()->GetAttachUI();
			if (ui)
			{
				ui->GetGameObject()->SetActive(true);
			}
		}
		break;
		case ButtonType::Exit:
		{
			SoundManager::GetInstance().StopAllSFX();
			SoundManager::GetInstance().PlaySFX(SoundManager::Button::Hover);

			auto image = obj.GetComponent<Image>();
			if (SceneManager::GetInstance().GetName() == L"MainScene")
			{
				image->SetTexture(L"Exit_Enter");
			}
			else
			{
				image->SetTexture(L"Exit_Ingame_Enter");
			}

			auto ui = obj.GetComponent<ExitButton>()->GetAttachUI();
			if (ui)
			{
				ui->GetGameObject()->SetActive(true);
			}
		}
		break;
		case ButtonType::Continue:
		{
			SoundManager::GetInstance().StopAllSFX();
			SoundManager::GetInstance().PlaySFX(SoundManager::Button::Hover);

			auto image = obj.GetComponent<Image>();
			image->SetTexture(L"Continue_Enter");

			auto ui = obj.GetComponent<ContinueButton>()->GetAttachUI();
			if (ui)
			{
				ui->GetGameObject()->SetActive(true);
			}
		}
		break;
		case ButtonType::Title:
		{
			SoundManager::GetInstance().StopAllSFX();
			SoundManager::GetInstance().PlaySFX(SoundManager::Button::Hover);

			auto image = obj.GetComponent<Image>();
			if (SceneManager::GetInstance().GetName() == L"GameOverScene")
			{
				image->SetTexture(L"Title_GameOver_Enter");
			}
			else
			{
				image->SetTexture(L"Title_Enter");
			}

			auto ui = obj.GetComponent<TitleButton>()->GetAttachUI();
			if (ui)
			{
				ui->GetGameObject()->SetActive(true);
			}
		}
		break;
		case ButtonType::Restart:
		{
			SoundManager::GetInstance().StopAllSFX();
			SoundManager::GetInstance().PlaySFX(SoundManager::Button::Hover);

			auto image = obj.GetComponent<Image>();
			image->SetTexture(L"Restart_Enter");

			auto ui = obj.GetComponent<RestartButton>()->GetAttachUI();
			if (ui)
			{
				ui->GetGameObject()->SetActive(true);
			}
		}
		break;
	}
}

void ButtonSelector::DeselectButton(GameObject& obj)
{
	switch (GetButtonType(obj))
	{
		case ButtonType::Start:
		{
			auto image = obj.GetComponent<Image>();
			image->SetTexture(L"Start_Idle");

			auto ui = obj.GetComponent<StartButton>()->GetAttachUI();
			if (ui)
			{
				ui->GetGameObject()->SetActive(false);
			}
		}
		break;
		case ButtonType::Exit:
		{
			auto image = obj.GetComponent<Image>();
			if (SceneManager::GetInstance().GetName() == L"MainScene")
			{
				image->SetTexture(L"Exit_Idle");
			}
			else
			{
				image->SetTexture(L"Exit_Ingame_Idle");
			}

			auto ui = obj.GetComponent<ExitButton>()->GetAttachUI();
			if (ui)
			{
				ui->GetGameObject()->SetActive(false);
			}
		}
		break;
		case ButtonType::Continue:
		{
			auto image = obj.GetComponent<Image>();
			image->SetTexture(L"Continue_Idle");

			auto ui = obj.GetComponent<ContinueButton>()->GetAttachUI();
			if (ui)
			{
				ui->GetGameObject()->SetActive(false);
			}
		}
		break;
		case ButtonType::Title:
		{
			auto image = obj.GetComponent<Image>();
			if (SceneManager::GetInstance().GetName() == L"GameOverScene")
			{
				image->SetTexture(L"Title_GameOver_Idle");
			}
			else
			{
				image->SetTexture(L"Title_Idle");
			}

			auto ui = obj.GetComponent<TitleButton>()->GetAttachUI();
			if (ui)
			{
				ui->GetGameObject()->SetActive(false);
			}
		}
		break;
		case ButtonType::Restart:
		{
			auto image = obj.GetComponent<Image>();
			image->SetTexture(L"Restart_Idle");

			auto ui = obj.GetComponent<RestartButton>()->GetAttachUI();
			if (ui)
			{
				ui->GetGameObject()->SetActive(false);
			}
		}
		break;
	}
}

/// 해당 버튼 이벤트를 그대로 가져다가 실행하도록 함...
/// 버튼 이벤트 변경 시 같이 변경 필요...
/// 버튼 이벤트 발생 장치가 없기 때문
void ButtonSelector::InvokeButton(GameObject& obj)
{
	switch (GetButtonType(obj))
	{
		case ButtonType::Start:
		{
			SoundManager::GetInstance().PlaySFX(SoundManager::Button::Select);
			LoadScene(L"IntroScene");
		}
		break;
		case ButtonType::Exit:
		{
			SoundManager::GetInstance().PlaySFX(SoundManager::Button::Select);
			PostQuitMessage(0);
		}
		break;
		case ButtonType::Continue:
		{
			SoundManager::GetInstance().PlaySFX(SoundManager::Button::Select);

			auto settingButton = obj.GetComponent<ContinueButton>()->GetSettingButton();

			if (settingButton)
			{
				settingButton->CloseSettingUI();
			}
		}
		break;
		case ButtonType::Title:
		{
			// 일시정지 풀기
			TimeController::GetInstance().SetTimeScale(1.0f);
			Input::GetInstance().SetKeyActive(true);
			Input::GetInstance().SetMouseActive(true);

			SoundManager::GetInstance().PlaySFX(SoundManager::Button::Select);

			SoundManager::GetInstance().StopAllBGM();
			SoundManager::GetInstance().PlayBGM(SoundManager::BGM::Opening);
			LoadScene(L"MainScene");
		}
		break;
		case ButtonType::Restart:
		{
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
				SoundManager::GetInstance().PlayBGM(SoundManager::BGM::Phase3);
				LoadScene(L"GameSceneBossPhase3");
			}
			else
			{
				assert(0);
			}

			break;
		}
	}
}

GameObject* ButtonSelector::GetBeforeButton(GameObject& obj)
{
	auto itr = std::find(buttonList.begin(), buttonList.end(), const_cast<GameObject*>(&obj));
	if (itr != buttonList.begin())
	{
		itr--;
		return *itr;
	}
	else
	{
		return nullptr;
	}
}

GameObject* ButtonSelector::GetNextButton(GameObject& obj)
{
	auto itr = std::find(buttonList.begin(), buttonList.end(), const_cast<GameObject*>(&obj));
	if (itr != (--buttonList.end()))
	{
		itr++;
		return *itr;
	}
	else
	{
		return nullptr;
	}
}

ButtonSelector::ButtonType ButtonSelector::GetButtonType(GameObject& obj)
{
	if (obj.GetComponent<StartButton>())
	{
		return ButtonType::Start;
	}
	else if (obj.GetComponent<ExitButton>())
	{
		return ButtonType::Exit;
	}
	else if (obj.GetComponent<ContinueButton>())
	{
		return ButtonType::Continue;
	}
	else if (obj.GetComponent<TitleButton>())
	{
		return ButtonType::Title;
	}
	else if (obj.GetComponent<RestartButton>())
	{
		return ButtonType::Restart;
	}

	return ButtonType::NONE;
}

void ButtonSelector::MouseEnterUpdate()
{
	// 해제
	for (auto each : buttonList)
	{
		switch (GetButtonType(*each))
		{
			case ButtonType::Start:
			{
				if (each->GetComponent<StartButton>()->GetMouseExit() && each == currentButton)
				{
					SelectButton(*each);
					each->GetComponent<StartButton>()->SetMouseExit(false);
				}
			}
			break;
			case ButtonType::Exit:
			{
				if (each->GetComponent<ExitButton>()->GetMouseExit() && each == currentButton)
				{
					SelectButton(*each);
					each->GetComponent<ExitButton>()->SetMouseExit(false);
				}
			}
			break;
			case ButtonType::Continue:
			{
				if (each->GetComponent<ContinueButton>()->GetMouseExit() && each == currentButton)
				{
					SelectButton(*each);
					each->GetComponent<ContinueButton>()->SetMouseExit(false);
				}
			}
			break;
			case ButtonType::Title:
			{
				if (each->GetComponent<TitleButton>()->GetMouseExit() && each == currentButton)
				{
					SelectButton(*each);
					each->GetComponent<TitleButton>()->SetMouseExit(false);
				}
			}
			break;
			case ButtonType::Restart:
			{
				if (each->GetComponent<RestartButton>()->GetMouseExit() && each == currentButton)
				{
					SelectButton(*each);
					each->GetComponent<RestartButton>()->SetMouseExit(false);
				}
			}
			break;
		}
	}

	// 선택
	for (auto each : buttonList)
	{
		switch (GetButtonType(*each))
		{
			case ButtonType::Start:
			{
				if (each->GetComponent<StartButton>()->GetMouseEnter())
				{
					DeselectButton(*currentButton);
					SelectButton(*each);
					currentButton = each;
					each->GetComponent<StartButton>()->SetMouseEnter(false);
				}
			}
			break;
			case ButtonType::Exit:
			{
				if (each->GetComponent<ExitButton>()->GetMouseEnter())
				{
					DeselectButton(*currentButton);
					SelectButton(*each);
					currentButton = each;
					each->GetComponent<ExitButton>()->SetMouseEnter(false);
				}
			}
			break;
			case ButtonType::Continue:
			{
				if (each->GetComponent<ContinueButton>()->GetMouseEnter())
				{
					DeselectButton(*currentButton);
					SelectButton(*each);
					currentButton = each;
					each->GetComponent<ContinueButton>()->SetMouseEnter(false);
				}
			}
			break;
			case ButtonType::Title:
			{
				if (each->GetComponent<TitleButton>()->GetMouseEnter())
				{
					DeselectButton(*currentButton);
					SelectButton(*each);
					currentButton = each;
					each->GetComponent<TitleButton>()->SetMouseEnter(false);
				}
			}
			break;
			case ButtonType::Restart:
			{
				if (each->GetComponent<RestartButton>()->GetMouseEnter())
				{
					DeselectButton(*currentButton);
					SelectButton(*each);
					currentButton = each;
					each->GetComponent<RestartButton>()->SetMouseEnter(false);
				}
			}
			break;
		}
	}
}

void ButtonSelector::PreSerialize(json& jsonData) const
{
	PRESERIALIZE_BASE(ButtonSelector);

	PRESERIALIZE_VECTOR_PTR(buttonList);
}

void ButtonSelector::PreDeserialize(const json& jsonData)
{
	PREDESERIALIZE_BASE(ButtonSelector);

	PREDESERIALIZE_VECTOR_PTR(buttonList);
}

void ButtonSelector::PostSerialize(json& jsonData) const
{
	POSTSERIALIZE_BASE();

	POSTSERIALIZE_VECTOR_PTR(buttonList);
	POSTSERIALIZE_PTR(firstButton);
}

void ButtonSelector::PostDeserialize(const json& jsonData)
{
	POSTDESERIALIZE_BASE();

	POSTDESERIALIZE_VECTOR_PTR(buttonList);
	POSTDESERIALIZE_PTR(firstButton);
}
