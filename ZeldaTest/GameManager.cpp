#include "GameManager.h"
#include "Guilty.h"

const int GameManager::DefaultLayer = 900;

void GameManager::Awake()
{
	mainAnimator->SetEventAnimation(true);
	subAnimator->SetEventAnimation(true);
	if (thirdAnimator != nullptr)
	{
		thirdAnimator->SetEventAnimation(true);
	}

	elapsedTime = 0.0f;
	eventTime = 0.0f;

	nextSceneName = L"";

	isPushedSceneEvent = false;
}

void GameManager::Start()
{
	if (SceneManager::GetInstance().GetName() == L"MainScene")
	{
		SoundManager::GetInstance().PlayBGM(SoundManager::BGM::Opening);
	}
	else if (SceneManager::GetInstance().GetName() == L"Directing Scene 0")
	{
		SoundManager::GetInstance().PlayBGM(SoundManager::BGM::Phase1);
	}
	else if (SceneManager::GetInstance().GetName() == L"Directing Scene 2")
	{
		// 2~3 연출 여기서 바꿈

		SoundManager::GetInstance().PlayBGM(SoundManager::BGM::Phase3);
		ShowPhase3CutScene();
	}
	else if (SceneManager::GetInstance().GetName() == L"GameOverScene")
	{
		SoundManager::GetInstance().PlayBGM(SoundManager::BGM::GameOver);
	}
	if (SceneManager::GetInstance().GetName() == L"MainScene")
	{
		SoundManager::GetInstance().PlayBGM(SoundManager::BGM::Opening);
	}

	mainAnimator->SetFadeOutAfterPlayEnds(true, 0.5f);
	mainAnimator->SetPlaySpeed(0.5f);
	mainAnimator->SetRetainAfterPlayEnds(false);
	mainAnimator->SetTexture
	(
		{
			L"Black"
		}
	);

	subAnimator->SetRetainAfterPlayEnds(false);
	subAnimator->SetTexture
	(
		{
			L"Black"
		}
	);

	thirdAnimator->SetRetainAfterPlayEnds(false);
	thirdAnimator->SetTexture
	(
		{
			L"Black"
		}
	);

	imageMap.insert({ mainAnimator, mainAnimator->GetGameObject()->GetComponent<Image>() });
	imageMap.insert({ subAnimator, subAnimator->GetGameObject()->GetComponent<Image>() });
	imageMap.insert({ thirdAnimator, thirdAnimator->GetGameObject()->GetComponent<Image>() });
	
	if (SceneManager::GetInstance().GetName() == L"IntroScene")
	{
		mainAnimator->Play();
	}
	else if (SceneManager::GetInstance().GetName() == L"Directing Scene 0")
	{
		mainAnimator->Play();
	}
	else if (SceneManager::GetInstance().GetName() == L"StaffRollScene")
	{
		LoadMainScene();
	}
	else if (SceneManager::GetInstance().GetName() == L"GameScene")
	{
		Guilty::GetInstance().bossPhase = 1;
	}
	else if (SceneManager::GetInstance().GetName() == L"BossPhase2Scene")
	{
		Guilty::GetInstance().bossPhase = 2;
	}
	else if (SceneManager::GetInstance().GetName() == L"GameSceneBossPhase3")
	{
		Guilty::GetInstance().bossPhase = 3;
		ShowPhase3StartEvent();
	}
}

void GameManager::Update()
{
	if (Input::GetInstance().PressedKeyNow('1') && EngineSetting::GetInstance().GetShowDebugInfo())
	{
		LoadScene(L"MainScene");
	}

	if (Input::GetInstance().PressedKeyNow('2') && EngineSetting::GetInstance().GetShowDebugInfo())
	{
		LoadScene(L"IntroScene");
	}

	if (Input::GetInstance().PressedKeyNow('3') && EngineSetting::GetInstance().GetShowDebugInfo())
	{
		LoadScene(L"GameScene");
	}

	if (Input::GetInstance().PressedKeyNow('4') && EngineSetting::GetInstance().GetShowDebugInfo())
	{
		LoadScene(L"GameSceneBossPhase3");
	}

	if (Input::GetInstance().PressedKeyNow('5') && EngineSetting::GetInstance().GetShowDebugInfo())
	{
		LoadScene(L"Directing Scene 0");
	}

	if (Input::GetInstance().PressedKeyNow('6') && EngineSetting::GetInstance().GetShowDebugInfo())
	{
		// 2~3 연출 여기서 바꿈

		LoadScene(L"Directing Scene 2");
	}

	if (Input::GetInstance().PressedKeyNow('7') && EngineSetting::GetInstance().GetShowDebugInfo())
	{
		LoadScene(L"UnnecessaryScene");
	}

	if (Input::GetInstance().PressedKeyNow('8') && EngineSetting::GetInstance().GetShowDebugInfo())
	{
		LoadScene(L"UnnecessaryScene2");
	}

	if (Input::GetInstance().PressedKeyNow('9') && EngineSetting::GetInstance().GetShowDebugInfo())
	{
		LoadScene(L"UnnecessaryScene3");
	}

	if (Input::GetInstance().PressedKeyNow('0') && EngineSetting::GetInstance().GetShowDebugInfo())
	{
		LoadScene(L"SampleScene");
	}

	if (Input::GetInstance().PressedKeyNow(VK_F1) && EngineSetting::GetInstance().GetShowDebugInfo())
	{
		ShowPhase2CutScene1();
	}

	if (Input::GetInstance().PressedKeyNow(VK_F2) && EngineSetting::GetInstance().GetShowDebugInfo())
	{
		ShowPhase2CutScene2();
	}

	if (Input::GetInstance().PressedKeyNow(VK_F3) && EngineSetting::GetInstance().GetShowDebugInfo())
	{
		ShowPhase3CutScene();
	}

	if (Input::GetInstance().PressedKeyNow(VK_F4) && EngineSetting::GetInstance().GetShowDebugInfo())
	{
		ShowPhase2EndEvent();
	}


	if (Input::GetInstance().PressedKeyNow(VK_F5) && EngineSetting::GetInstance().GetShowDebugInfo())
	{
		ShowEndingCutSceneEndEvent();
	}

	if (Input::GetInstance().PressedKeyNow(VK_F11) && EngineSetting::GetInstance().GetShowDebugInfo())
	{
		LoadScene(L"BossPhase2Scene");
	}

	if (TimeController::GetInstance().GetTimeScale() == 0.0f && isStoppedByMe == false)
	{
		mainAnimator->SetEventAnimation(false);
		subAnimator->SetEventAnimation(false);
		thirdAnimator->SetEventAnimation(false);
	}
	else
	{
		mainAnimator->SetEventAnimation(true);
		subAnimator->SetEventAnimation(true);
		thirdAnimator->SetEventAnimation(true);
	}

	float eventDeltaTime = TimeController::GetInstance().GetEventDeltaTime();

	if (TimeController::GetInstance().GetTimeScale() != 0.0f || isStoppedByMe)
	{
		// 시간 흐름
		for (auto iter = eventList.begin(); iter != eventList.end(); iter++)
		{
			(*iter).first -= eventDeltaTime;
		}

		// 이벤트 실행
		while (!eventList.empty() && (*eventList.begin()).first <= 0.0)
		{
			std::pair<float, EventInfo> currentEvent = eventList.front();
			eventList.pop_front();

			EventInfo eventInfo = currentEvent.second;

			if (eventInfo.type == Animation)
			{
				eventInfo.animator->SetPreDelay(eventInfo.preDelay);
				eventInfo.animator->SetFadeInBeforePlayBegins(true, eventInfo.fadeinTime);
				eventInfo.animator->SetPlaySpeed(eventInfo.sprites.size() / eventInfo.playTime);
				eventInfo.animator->SetPostDelay(eventInfo.PostDelay);
				eventInfo.animator->SetFadeOutAfterPlayEnds(true, eventInfo.fadeoutTime);
				eventInfo.animator->SetTexture(eventInfo.sprites);
				imageMap[eventInfo.animator]->SetLayer(eventInfo.layer);
				eventInfo.animator->Play();
			}
			else if (eventInfo.type == Scene)
			{
				LoadScene(eventInfo.sceneName);
			}
			else if (eventInfo.type == Pause)
			{
				if (TimeController::GetInstance().GetTimeScale() != 0.0f)
				{
					TimeController::GetInstance().SetTimeScale(0.0f);
					Input::GetInstance().SetKeyActive(false);
					Input::GetInstance().SetMouseActive(false);
					isStoppedByMe = true;
				}
			}
			else if (eventInfo.type == Resume)
			{
				if (isStoppedByMe == true && TimeController::GetInstance().GetTimeScale() == 0.0f)
				{
					TimeController::GetInstance().SetTimeScale(1.0f);
					Input::GetInstance().SetKeyActive(true);
					Input::GetInstance().SetMouseActive(true);
					isStoppedByMe = false;
				}
			}
			else
			{
				assert(0);
			}
		}
	}
}

void GameManager::SetMainAnimator(SpriteAnimator* spriteAnimator)
{
	mainAnimator = spriteAnimator;
}

void GameManager::SetSubAnimator(SpriteAnimator* spriteAnimator)
{
	subAnimator = spriteAnimator;
}

void GameManager::SetThirdAnimator(SpriteAnimator* spriteAnimator)
{
	thirdAnimator = spriteAnimator;
}

void GameManager::ShowPhase2CutScene1()
{
	if (eventList.size() != 0) return;
	if (isPushedSceneEvent)
	{
		return;
	}

	PushPauseEvent(0.0f);
	PushAnimationEvent(0.0f, mainAnimator, 0.0f, 0.0f, 0.5f, 1.0f, 0.5f,
		{
			L"Phase2CutScene1_0",
			L"Phase2CutScene1_1",
			L"Phase2CutScene1_2",
			L"Phase2CutScene1_3",
			L"Phase2CutScene1_4",
			L"Phase2CutScene1_5",
			L"Phase2CutScene1_6"
		}
	);
	PushResumeEvent(2.0f);
}

void GameManager::ShowPhase2CutScene2()
{
	if (eventList.size() != 0) return;
	if (isPushedSceneEvent)
	{
		return;
	}

	PushPauseEvent(0.0f);
	PushAnimationEvent(0.0f, mainAnimator, 0.0f, 0.0f, 1.0f, 1.0f, 0.5f,
		{
			L"Phase2CutScene2_0",
			L"Phase2CutScene2_1",
			L"Phase2CutScene2_2",
			L"Phase2CutScene2_3",
			L"Phase2CutScene2_4",
			L"Phase2CutScene2_5",
			L"Phase2CutScene2_6",
			L"Phase2CutScene2_7",
			L"Phase2CutScene2_8",
			L"Phase2CutScene2_9",
		}
	);
	PushResumeEvent(2.5f);
}

void GameManager::ShowPhase3CutScene()
{
	if (isPushedSceneEvent)
	{
		return;
	}

	PushPauseEvent(0.0f);
	PushAnimationEvent(0.0f, subAnimator, 0.0f, 0.0f, 2.5f, 0.0f, 0.0f,
		{
			L"White"
		},
		DefaultLayer - 1
	);
	PushAnimationEvent(0.0f, mainAnimator, 0.0f, 0.0f, 1.0f, 1.0f, 0.5f,
		{
			L"Phase3CutScene0",
			L"Phase3CutScene1",
			L"Phase3CutScene2",
			L"Phase3CutScene3",
			L"Phase3CutScene4",
			L"Phase3CutScene5",
			L"Phase3CutScene6",
			L"Phase3CutScene7",
			L"Phase3CutScene8",
			L"Phase3CutScene9",
			L"Phase3CutScene10",
			L"Phase3CutScene11",
			L"Phase3CutScene12"
		}
	);
	PushResumeEvent(2.5f);
}

void GameManager::ShowPhase2EndEvent()
{
	if (isPushedSceneEvent)
	{
		return;
	}

	PushPauseEvent(0.0f);
	PushAnimationEvent(0.0f, mainAnimator, 0.0f, 1.5f, 2.0f, 2.f, 0.0f,
		{
			L"Black"
		}
	);
	PushResumeEvent(4.0f);

	// 2~3 연출 여기서 바꿈
	PushSceneEvent(4.0f, L"Directing Scene 2");
}

void GameManager::ShowPhase3StartEvent()
{
	PushAnimationEvent(0.0f, mainAnimator, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
		{
			L"White"
		},
		DefaultLayer
	);
}

void GameManager::ShowEndingCutSceneEndEvent()
{
	if (isPushedSceneEvent)
	{
		return;
	}

	SoundManager::GetInstance().StopAll();
	SoundManager::GetInstance().PlayBGM(SoundManager::BGM::Ending);

	PushPauseEvent(0.0f);

	PushAnimationEvent(0.0f, mainAnimator, 0.0f, 1.5f, 2.0f, 0.0f, 1.5f,
		{
			L"White"
		},
		DefaultLayer
	);

	PushAnimationEvent(3.5f, subAnimator, 0.0f, 0.0f, 4.0f, 0.0f, 0.5f,
		{
			L"EndingCutScene0"
		},
		DefaultLayer - 1
	);

	PushAnimationEvent(7.5f, thirdAnimator, 0.0f, 0.0f, 4.0f, 0.0f, 0.5f,
		{
			L"EndingCutScene1"
		},
		DefaultLayer - 2
	);

	PushAnimationEvent(11.5f, mainAnimator, 0.0f, 0.0f, 4.5f, 0.5f, 0.0f,
		{
			L"EndingCutScene2"
		},
		DefaultLayer - 3
	);

	PushResumeEvent(15.5f);
	PushSceneEvent(15.5f, L"StaffRollScene");
}

void GameManager::ShowGameOverScene(float gameoverDelay)
{
	PushAnimationEvent(gameoverDelay, mainAnimator, 0.0f, 1.5f, 1.0f, 0.5f, 0.0f,
		{
			L"Black"
		}
	);

	PushSceneEvent(gameoverDelay + 2.5f, L"GameOverScene");
}

void GameManager::PushAnimationEvent(float startTime, SpriteAnimator* animator, float preDelay, float fadeinTime, float playTime, float postDelay, float fadeoutTime, std::vector<std::wstring> sprites, int layer)
{
	std::pair<float, EventInfo> temp =
	{
		startTime,
		{
			Animation,
			L"",
			animator,
			preDelay,
			fadeinTime,
			playTime,
			postDelay,
			fadeoutTime,
			sprites,
			layer
		}
	};

	eventList.emplace_back(temp);
}

void GameManager::PushSceneEvent(float startTime, std::wstring sceneName)
{
	isPushedSceneEvent = true;

	std::pair<float, EventInfo> temp =
	{
		startTime,
		{
			Scene,
			sceneName,
			nullptr,
			0.0f,
			0.0f,
			0.0f,
			0.0f,
			0.0f,
			{}
		}
	};

	eventList.emplace_back(temp);
}

void GameManager::PushPauseEvent(float time)
{
	std::pair<float, EventInfo> temp =
	{
		time,
		{
			Pause,
			L"",
			nullptr,
			0.0f,
			0.0f,
			0.0f,
			0.0f,
			0.0f,
			{}
		}
	};

	eventList.emplace_back(temp);
}

void GameManager::PushResumeEvent(float time)
{
	std::pair<float, EventInfo> temp =
	{
		time,
		{
			Resume,
			L"",
			nullptr,
			0.0f,
			0.0f,
			0.0f,
			0.0f,
			0.0f,
			{}
		}
	};

	eventList.emplace_back(temp);
}

void GameManager::LoadMainScene()
{
	PushSceneEvent(22.0f, L"MainScene");
}

void GameManager::PreSerialize(json& jsonData) const
{
	PRESERIALIZE_BASE(GameManager);
}

void GameManager::PreDeserialize(const json& jsonData)
{
	PREDESERIALIZE_BASE(GameManager);
}

void GameManager::PostSerialize(json& jsonData) const
{
	POSTSERIALIZE_BASE();
	POSTSERIALIZE_PTR(mainAnimator);
	POSTSERIALIZE_PTR(subAnimator);
	POSTSERIALIZE_PTR(thirdAnimator);
}

void GameManager::PostDeserialize(const json& jsonData)
{
	POSTDESERIALIZE_BASE();
	POSTDESERIALIZE_PTR(mainAnimator);
	POSTDESERIALIZE_PTR(subAnimator);
	POSTDESERIALIZE_PTR(thirdAnimator);
}
