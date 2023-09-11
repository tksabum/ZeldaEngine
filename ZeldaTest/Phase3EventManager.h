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

	float eventDelay;					// �̺�Ʈ ���� �����ð�

	float centerRadiusDecrease;			// �߾����� �ν��ϴ� ������ �������� �ӵ�	(���ҷ�/��)
	float centerRadius;					// �߾� ����

	int bossFirstAttackCount;			// ������ �������� ���� �̸� �����صδ� Ƚ��
	float bossFirstAttackCycle;			// ������ ù ������ �ֱ�
	float bossAttackCycle;				// ������ �Ϲ����� ���� �ֱ�

	int playerNoHandicapCount;			// �÷��̾ ó���� �ڵ�ĸ���� ������ �� �ִ� ��
	float playerHandicap;				// �÷��̾ ������ ���� Ű�Է��� �ؾ��ϴ� Ƚ�� (Ű�Է� Ƚ��/���� Ƚ��)
	float playerHitRate;				// �÷��̾��� ������ ������ ������ ������ ���� Ȯ��

	float playerHitRateIncrease;		// �÷��̾��� HitRate�� �����ϴ� �ӵ� (������/��)
	float playerHitRateMax;				// �÷��̾��� �ִ� HitRate
	float playerHandicapDecrease;		// �÷��̾��� Hadicap�� �����ϴ� �ӵ� (���ҷ�/��)
	float playerHandicapMin;			// �÷��̾��� �ּ� Handicap

	float animationPlaySpeed;			// �ִϸ��̼� ����ӵ� (������ ��/��)


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
	// Component��(��) ���� ��ӵ�
	virtual void PreSerialize(json& jsonData) const override;
	virtual void PreDeserialize(const json& jsonData) override;
	virtual void PostSerialize(json& jsonData) const override;
	virtual void PostDeserialize(const json& jsonData) override;

};

