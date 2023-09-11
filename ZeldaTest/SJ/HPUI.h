#pragma once

#include "ZeldaEngine.h"

// 편하게 쓰려면 using namespace 2개는 고정으로 해주는게 좋을것임
using namespace ZeldaEngine;
using namespace ZeldaEngine::ZeldaMath;

class Player;
class BaseUI;
class MovableUI;
class CoverUI;

class HPUI : public Component
{
public:
	virtual void Awake() override;
	virtual void Start() override;
	virtual void Update() override;

	void SetPlayer(const Player& player);
	void SetBaseUI(const BaseUI& base);
	void SetMovableUI(const MovableUI& moveBar);
	void SetCoverUI(const CoverUI& cover);
	void SetUISize(const Vector2D& size);
private:
	Player* player = nullptr;
	float maxHP = 0;
	Vector2D UISize = Vector2D::zero;
	BaseUI* base = nullptr;
	MovableUI* moveBar = nullptr;
	CoverUI* cover = nullptr;

	float padding = 7.6f;

	Transform* moveBarTransform = nullptr;

	// Component을(를) 통해 상속됨
	virtual void PreSerialize(json& jsonData) const override;
	virtual void PreDeserialize(const json& jsonData) override;
	virtual void PostSerialize(json& jsonData) const override;
	virtual void PostDeserialize(const json& jsonData) override;
};