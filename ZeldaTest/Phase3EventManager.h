#pragma once

#include "ZeldaEngine.h"

#include <queue>
#include <random>
#include <cmath>
#include <list>

using namespace std;
using namespace ZeldaEngine;
using namespace ZeldaEngine::ZeldaMath;

class GameManager;

class Phase3EventManager : public Component
{
public:
	Phase3EventManager();

	void Awake();
	void Start();
	void Update();

	void SetGameManager(GameManager* gameManager);

private:
	GameManager* gameManager;

	float eventDelay;					// 이벤트 시작 지연시간

	float centerRadiusDecrease;			// 중앙으로 인식하는 범위가 좁아지는 속도	(감소량/초)
	float centerRadius;					// 중앙 범위

	int bossFirstAttackCount;			// 보스가 시작하자 마자 미리 공격해두는 횟수
	float bossFirstAttackCycle;			// 보스의 첫 공격의 주기
	float bossAttackCycle;				// 보스의 일반적인 공격 주기

	int playerNoHandicapCount;			// 플레이어가 처음에 핸디캡없이 공격할 수 있는 수
	float playerHandicap;				// 플레이어가 공격을 위해 키입력을 해야하는 횟수 (키입력 횟수/공격 횟수)
	float playerHitRate;				// 플레이어의 공격이 보스의 공격을 완전히 덮을 확률

	float playerHitRateIncrease;		// 플레이어의 HitRate가 증가하는 속도 (증가량/초)
	float playerHitRateMax;				// 플레이어의 최대 HitRate
	float playerHandicapDecrease;		// 플레이어의 Hadicap가 감소하는 속도 (감소량/초)
	float playerHandicapMin;			// 플레이어의 최소 Handicap

	float animationPlaySpeed;			// 애니메이션 재생속도 (프레임 수/초)


	bool finalEvent;
	float finalElapsed;

	float playerAttackElapsed;

	float elapsedTime;
	int attackLayer;
	float imageWidth;
	float imageHeight;

	int deathCount;
	int keyCount;

	bool isGameOver;

	GameObject* hithitObject;

	list<float> removeBossImageTimerList;
	queue<pair<SpriteAnimator*, Image*>> bossAttackQueue;
	queue<pair<SpriteAnimator*, Image*>> bossAttackErasedQueue;
	mt19937 gen;
	uniform_real_distribution<float> dis;

	pair<Vector2D, float> CreateRandomPosRot();
	void BossAttack(Vector2D pos, float rot, float delay = 0.0f);
	void PlayerAttack(bool coverBossAttack, Vector2D pos = Vector2D::zero, float rot = 0.0f);

private:
	queue<pair<SpriteAnimator*, Image*>> animatorPool;

	pair<SpriteAnimator*, Image*> PopFromPool();
	void ReturnToPool(pair<SpriteAnimator*, Image*> obj);

	const static vector<wstring> bossSpriteList;
	const static vector<wstring> playerSpriteList;

private:
	// Component을(를) 통해 상속됨
	virtual void PreSerialize(json& jsonData) const override;
	virtual void PreDeserialize(const json& jsonData) override;
	virtual void PostSerialize(json& jsonData) const override;
	virtual void PostDeserialize(const json& jsonData) override;

};

