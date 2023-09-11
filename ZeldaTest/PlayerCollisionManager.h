#pragma once
#include "ZeldaEngine.h"

using namespace ZeldaEngine;
using namespace ZeldaEngine::ZeldaMath;

class Boss1;

class PlayerCollisionManager : public Component
{
private:
	CubeCollider* m_Collider;
	CapsuleCollider* m_bossCollider;
	Transform* m_bossTransform;

	Boss1* m_bossComponent;

	float m_damage;
public:
	void Awake();
	void SetDamage(float damage);
	void SetBossComponent(Boss1* bossComponent);

	void OnTriggerEnter(Collider* other);

private:
	// Component을(를) 통해 상속됨
	virtual void PreSerialize(json& jsonData) const override;
	virtual void PreDeserialize(const json& jsonData) override;
	virtual void PostSerialize(json& jsonData) const override;
	virtual void PostDeserialize(const json& jsonData) override;
};

