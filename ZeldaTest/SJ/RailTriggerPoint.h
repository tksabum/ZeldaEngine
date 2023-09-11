#pragma once

#include "ZeldaEngine.h"

// 편하게 쓰려면 using namespace 2개는 고정으로 해주는게 좋을것임
using namespace ZeldaEngine;
using namespace ZeldaEngine::ZeldaMath;

class EffectPlayer;

class RailTriggerPoint : public Component
{
public:
	virtual void Awake() override;
	virtual void Update() override;

	virtual void OnTriggerEnter() override;

	void SetIndex(int index);
	int GetIndex() const;

	void SetBoss1(GameObject* input) { boss = input; }
	GameObject* GetBoss1() { return boss; }

	void SetPlayer(GameObject* input) { player = input; }
	GameObject* GetPlayer() { return player; }

	void SetCollider(Collider* input) { collider = input; }
	Collider* GetCollider() { return collider; }

private:
	GameObject* boss = nullptr;
	GameObject* player = nullptr;
	Transform* cam = nullptr;
	Collider* collider = nullptr;
	std::wstring sceneName = L"";
	int index = -1;

	GameObject* effectObj = nullptr;
	EffectPlayer* teleportEffectPlayer = nullptr;
	bool effectPlay = false;

	bool dynamic_cam = false;
	bool zoom = false;
	float radius = 22.84f;
	float angV = 120.0f;	// degree
	float lineV = 0.08f;	// line
	int count = 0;

	// Component을(를) 통해 상속됨
	virtual void PreSerialize(json& jsonData) const override;
	virtual void PreDeserialize(const json& jsonData) override;
	virtual void PostSerialize(json& jsonData) const override;
	virtual void PostDeserialize(const json& jsonData) override;
};