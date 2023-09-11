#pragma once
#include <functional>
#include "ZeldaEngine.h"

class Player;
class Boss1;

class BossCollisionManager : public ZeldaEngine::Component
{
private:
	Player* m_playerComponent;
	Boss1* m_bossComponent;


private:
	double m_damage;		// 패턴 별 데미지는 여기에
	bool isBranchMoveStart = false;
	
	
public:
	BossCollisionManager();
	void Awake();
	void OnTriggerEnter(ZeldaEngine::Collider* other);
	void OnTriggerStay(ZeldaEngine::Collider* other);
	
	void SetPlayerComponent(Player* playerComponent);
	void SetBossComponent(Boss1* bossComponent);
	void SetPatternDamage(double damage);
	void DetectFanRange();
	void IsBranchMoveStart(bool isStart);

private:
	// Component을(를) 통해 상속됨
	virtual void PreSerialize(json& jsonData) const override;
	virtual void PreDeserialize(const json& jsonData) override;
	virtual void PostSerialize(json& jsonData) const override;
	virtual void PostDeserialize(const json& jsonData) override;
};

