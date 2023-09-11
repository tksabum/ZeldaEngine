#include "pch.h"
#include "SoundManager.h"

#include "Serializable.h"

namespace ZeldaEngine
{
	SoundManager& SoundManager::GetInstance()
	{
		static SoundManager instance;
		return instance;
	}

	SoundManager::SoundManager() :
		lastBGMNumber(L""),
		effectVolume(0.5f),
		bgmVolume(0.5f),
		system(nullptr),
		bgmChannel(nullptr),
		effectChannelGroup(nullptr)
	{
		FMOD::System_Create(&system);
		system->init(32, FMOD_INIT_NORMAL, nullptr);

		// 채널 그룹 생성
		system->createChannelGroup("effectChannelGroup", &effectChannelGroup);

		// volume 초기값으로 업데이트
		UpdateAllVolume();

		// BGM
		LoadSound(L"BGM_Opening",		L"../Sound/BGM/BGM_Opening.mp3",			SoundType::BGM);
		LoadSound(L"BGM_Phase1",		L"../Sound/BGM/BGM_Phase1.mp3",				SoundType::BGM);
		LoadSound(L"BGM_Phase3",		L"../Sound/BGM/BGM_Phase3.mp3",				SoundType::BGM);
		LoadSound(L"BGM_Ending",		L"../Sound/BGM/BGM_Ending.mp3",				SoundType::BGM);
		LoadSound(L"BGM_GameOver",		L"../Sound/BGM/BGM_GameOver.mp3",			SoundType::BGM);

		// EFECT
		LoadSound(L"M_ButtonHover",		L"../Sound/EFFECT/M_ButtonHover.mp3",		SoundType::EFFECT);
		LoadSound(L"M_ButtonSelect",	L"../Sound/EFFECT/M_ButtonSelect.mp3",		SoundType::EFFECT);
		LoadSound(L"M_Option",			L"../Sound/EFFECT/M_Option.mp3",			SoundType::EFFECT);

		LoadSound(L"P_Walk",			L"../Sound/EFFECT/Player/P_Walk.mp3",		SoundType::EFFECT);
		LoadSound(L"P_Damaged",			L"../Sound/EFFECT/Player/P_Damaged.mp3",	SoundType::EFFECT);
		LoadSound(L"P_Dead",			L"../Sound/EFFECT/Player/P_Dead.mp3",		SoundType::EFFECT);
		LoadSound(L"P_Dash",			L"../Sound/EFFECT/Player/P_Dash.mp3",		SoundType::EFFECT);
		LoadSound(L"P_Attack1",			L"../Sound/EFFECT/Player/P_Attack1.mp3",	SoundType::EFFECT);
		LoadSound(L"P_Attack2",			L"../Sound/EFFECT/Player/P_Attack2.mp3",	SoundType::EFFECT);

		LoadSound(L"B_Walk",			L"../Sound/EFFECT/Boss/B_Walk.mp3",			SoundType::EFFECT);
		LoadSound(L"B_Damaged",			L"../Sound/EFFECT/Boss/B_Damaged.mp3",		SoundType::EFFECT);
		LoadSound(L"B_Dead",			L"../Sound/EFFECT/Boss/B_Dead.mp3",			SoundType::EFFECT);
		LoadSound(L"B_Telepo",			L"../Sound/EFFECT/Boss/B_Telepo.mp3",		SoundType::EFFECT);
		LoadSound(L"B_Tree_Attack",		L"../Sound/EFFECT/Boss/B_Tree_Attack.mp3",	SoundType::EFFECT);
		LoadSound(L"B_Dash_Attack",		L"../Sound/EFFECT/Boss/B_Dash_Attack.mp3",	SoundType::EFFECT);
		LoadSound(L"B_Attack1",			L"../Sound/EFFECT/Boss/B_Attack1.mp3",		SoundType::EFFECT);
		LoadSound(L"B_2D_Block",		L"../Sound/EFFECT/Boss/B_2D_Block.mp3",		SoundType::EFFECT);
		LoadSound(L"B_Attack2",			L"../Sound/EFFECT/Boss/B_Attack2.mp3",		SoundType::EFFECT);
		LoadSound(L"B_Push_Attack",		L"../Sound/EFFECT/Boss/B_Push_Attack.mp3",	SoundType::EFFECT);
		LoadSound(L"B_2D_Attack",		L"../Sound/EFFECT/Boss/B_2D_Attack.mp3",	SoundType::EFFECT);
		LoadSound(L"B_Roar",			L"../Sound/EFFECT/Boss/B_Roar.mp3",			SoundType::EFFECT);

		LoadSound(L"S_Page",			L"../Sound/EFFECT/S_Page.mp3",				SoundType::EFFECT);
	}

	SoundManager::~SoundManager()
	{
		for (auto& sound : bgmSounds)
		{
			sound.second->release();
		}

		for (auto& sound : effectSounds)
		{
			sound.second->release();
		}

		effectChannelGroup->release();
		system->close();
		system->release();
	}

	void SoundManager::StopAll()
	{
		lastBGMNumber = L"";

		bgmChannel->stop();
		effectChannelGroup->stop();
	}

	void SoundManager::StopAllSFX()
	{
		effectChannelGroup->stop();
	}

	void SoundManager::StopAllBGM()
	{
		lastBGMNumber = L"";

		bgmChannel->stop();
	}

	float SoundManager::GetVolume()
	{
		return this->effectVolume;
	}

	float SoundManager::GetBGMVolume()
	{
		return this->bgmVolume;
	}

	void SoundManager::SetVolume(float v)
	{
		this->effectVolume = v;
		UpdateAllVolume();
	}

	void SoundManager::SetBGM(float v)
	{
		this->bgmVolume = v;
		UpdateAllVolume();
	}


	void SoundManager::UpdateVolume(float v)
	{
		effectVolume = v;
		UpdateAllVolume();
	}

	void SoundManager::UpdateAllVolume()
	{
		bgmChannel->setVolume(bgmVolume);
		effectChannelGroup->setVolume(effectVolume);
	}

	void SoundManager::Play(const std::wstring& soundName)
	{
		auto iter = bgmSounds.find(soundName);
		if (iter != bgmSounds.end())
		{
			FMOD::Sound* sound = iter->second;

			bool isPlaying;
			bgmChannel->isPlaying(&isPlaying);
			if (isPlaying)
			{
				bgmChannel->stop();
			}
			system->playSound(sound, 0, false, &bgmChannel);
			bgmChannel->setVolume(bgmVolume);
		}

		iter = effectSounds.find(soundName);
		if (iter != effectSounds.end())
		{
			FMOD::Sound* sound = iter->second;
			FMOD::Channel* channel;
			system->playSound(sound, effectChannelGroup, false, &channel);
			channel->setVolume(effectVolume);
		}
	}

	void SoundManager::LoadSound(const std::wstring& soundName, const std::wstring& filePath, SoundType type)
	{
		FMOD::Sound* sound;

		switch (type)
		{
		case SoundType::BGM:
		{
			std::string str = wstring_to_string(filePath);
			system->createSound(str.c_str(), FMOD_LOOP_NORMAL, 0, &sound);

			assert(bgmSounds.count(soundName) == 0);
			bgmSounds[soundName] = sound;

			break;
		}

		case SoundType::EFFECT:
		{
			system->createSound(wstring_to_string(filePath).c_str(), FMOD_DEFAULT, 0, &sound);

			assert(effectSounds.count(soundName) == 0);
			effectSounds[soundName] = sound;

			break;
		}
		default:
			break;
		}
	}

	void SoundManager::PlayBGM(BGM sound)
	{
		switch (sound)
		{
		case ZeldaEngine::SoundManager::BGM::Opening:
			Play(L"BGM_Opening");
			break;
		case ZeldaEngine::SoundManager::BGM::Phase1:
			Play(L"BGM_Phase1");
			break;
		case ZeldaEngine::SoundManager::BGM::Phase3:
			Play(L"BGM_Phase3");
			break;
		case ZeldaEngine::SoundManager::BGM::Ending:
			Play(L"BGM_Ending");
			break;
		case ZeldaEngine::SoundManager::BGM::GameOver:
			Play(L"BGM_GameOver");
			break;
		default:
			break;
		}
	}

	void SoundManager::PlaySFX(Button sound)
	{
		switch (sound)
		{
		case ZeldaEngine::SoundManager::Button::Hover:
			Play(L"M_ButtonHover");
			break;
		case ZeldaEngine::SoundManager::Button::Select:
			Play(L"M_ButtonSelect");
			break;
		case ZeldaEngine::SoundManager::Button::Option:
			Play(L"M_Option");
			break;
		default:
			break;
		}
	}

	void SoundManager::PlaySFX(Player sound)
	{
		switch (sound)
		{
		case ZeldaEngine::SoundManager::Player::Walk:
			Play(L"P_Walk");
			break;
		case ZeldaEngine::SoundManager::Player::Damaged:
			Play(L"P_Damaged");
			break;
		case ZeldaEngine::SoundManager::Player::Dead:
			Play(L"P_Dead");
			break;
		case ZeldaEngine::SoundManager::Player::Dash:
			Play(L"P_Dash");
			break;
		case ZeldaEngine::SoundManager::Player::BasicAttack:
			Play(L"P_Attack1");
			break;
		case ZeldaEngine::SoundManager::Player::SpecialAttack:
			Play(L"P_Attack2");
			break;
		default:
			break;
		}
	}

	void SoundManager::PlaySFX(Boss sound)
	{
		switch (sound)
		{
		case ZeldaEngine::SoundManager::Boss::Walk:
			Play(L"B_Walk");
			break;
		case ZeldaEngine::SoundManager::Boss::Damaged:
			Play(L"B_Damaged");
			break;
		case ZeldaEngine::SoundManager::Boss::Dead:
			Play(L"B_Dead");
			break;
		case ZeldaEngine::SoundManager::Boss::Telepo:
			Play(L"B_Telepo");
			break;
		case ZeldaEngine::SoundManager::Boss::Pattern1:
			Play(L"B_Tree_Attack");
			break;
		case ZeldaEngine::SoundManager::Boss::Pattern2:
			Play(L"B_Dash_Attack");
			break;
		case ZeldaEngine::SoundManager::Boss::Pattern3:
			Play(L"B_Attack1");
			break;
		case ZeldaEngine::SoundManager::Boss::Pattern4:
			Play(L"B_2D_Block");
			break;
		case ZeldaEngine::SoundManager::Boss::Pattern5:
			Play(L"B_Attack2");
			break;
		case ZeldaEngine::SoundManager::Boss::Pattern6:
			Play(L"B_Push_Attack");
			break;
		case ZeldaEngine::SoundManager::Boss::Pattern7:
			Play(L"B_2D_Attack");
			break;
		case ZeldaEngine::SoundManager::Boss::Roar:
			Play(L"B_Roar");
			break;
		default:
			break;
		}
	}

	void SoundManager::PlaySFX(Effect sound)
	{
		switch (sound)
		{
		case ZeldaEngine::SoundManager::Effect::CutScene:
			Play(L"S_Page");
			break;
		default:
			break;
		}
	}

	void SoundManager::Update()
	{
		system->update();
	}
}