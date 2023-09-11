#pragma once
#include "ZeldaEngine.h"

class Player;

using namespace ZeldaEngine;

class Player2DCollisionManager : public ZeldaEngine::Component
{
private:
	Player* m_playerComponent;

	float normal2DDamage;
	float ultimate2DDamage;

public:
	Player2DCollisionManager();
	void Awake();

	void OnCollisionEnter2D(Collider2D* other);
	void SetPlayerComponent(Player* playerComponent);

private:
	// Component을(를) 통해 상속됨
	virtual void PreSerialize(json& jsonData) const override;
	virtual void PreDeserialize(const json& jsonData) override;
	virtual void PostSerialize(json& jsonData) const override;
	virtual void PostDeserialize(const json& jsonData) override;
};

