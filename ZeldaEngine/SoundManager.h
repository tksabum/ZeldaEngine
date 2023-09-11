#pragma once

#include "ZeldaEngineAPI.h"

#include "fmod.hpp"

#include <string>
#include <map>

namespace ZeldaEngine
{
	class ZELDAENGINE_API SoundManager
	{
	public:
        enum class SoundType
        {
            BGM,
            EFFECT
        };

        enum class BGM
        {
            Opening,
            Phase1,
            // Phase2 는 1이 반복
            Phase3,
            Ending,
            GameOver
        };

        enum class Button
        {
            Hover,
            Select,
            Option
        };

        enum class Player
        {
            Walk,
            Damaged,
            Dead,
            Dash,
            BasicAttack,
            SpecialAttack
        };

        enum class Boss
        {
            Walk,
            Damaged,
            Dead,
            Telepo,
            Pattern1,   // 가지
            Pattern2,   // 돌진
            Pattern3,   // 근접 평타
            Pattern4,   // 2D 장애물
            Pattern5,   // 원거리 평타
            Pattern6,   // 밀격
            Pattern7,   // 2D 획
            Roar
        };

        enum class Effect
        {
            CutScene
        };


        void StopAll();
        void StopAllBGM();
        void StopAllSFX();  /// 추가, 김상준
        float GetVolume();
        float GetBGMVolume();
        void SetVolume(float v);
        void SetBGM(float v);
        void UpdateVolume(float v);
        void UpdateAllVolume();

        void Play(const std::wstring& soundName);
        void LoadSound(const std::wstring& soundName, const std::wstring& filePath, SoundType type);

        void PlayBGM(BGM sound);
        void PlaySFX(Button sound);
        void PlaySFX(Player sound);
        void PlaySFX(Boss sound);
        void PlaySFX(Effect sound);


	private:
        FMOD::System* system;
        std::map<std::wstring, FMOD::Sound*> bgmSounds;
        std::map<std::wstring, FMOD::Sound*> effectSounds;
        FMOD::Channel* bgmChannel;
        FMOD::ChannelGroup* effectChannelGroup;
        float effectVolume;
        float bgmVolume;

        std::wstring lastBGMNumber;

        void Update();

        friend class CoreSystem;

        //singleton
    public:
        static SoundManager& GetInstance();

    private:
        SoundManager();
        ~SoundManager();
        SoundManager(const SoundManager& ref) = delete;
        SoundManager& operator=(const SoundManager& ref) = delete;
	};
}