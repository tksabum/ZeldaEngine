#pragma once

#include "ZeldaEngine.h"

// 편하게 쓰려면 using namespace 2개는 고정으로 해주는게 좋을것임
using namespace ZeldaEngine;
using namespace ZeldaEngine::ZeldaMath;

class BaseUI;
class MovableUI;
class CoverUI;
class NumUI;

class CoolTimeUI : public Component
{
public:
	enum class CountType
	{
		NUM,
		SLIDE,
	};

	/// 플레이어가 아니라도 쓸 수 있을 것으로 보여서 우선 GameObject 로 처리
	virtual void SetTarget(const GameObject& target);
	virtual void SetBaseUI(const BaseUI& base);
	virtual void SetMovableUI(const MovableUI& moveCover);
	virtual void SetCoverUI(const CoverUI& cover);
	virtual void SetNumUI(const NumUI& numUI);
	virtual void SetUISize(const Vector2D& size);
	virtual void SetType(const CountType& type);

protected:
	GameObject* target = nullptr;
	float maxCoolTime = 0;
	Vector2D UISize = Vector2D::zero;
	BaseUI* base = nullptr;
	MovableUI* moveCover = nullptr;
	NumUI* numUI = nullptr;
	CoverUI* cover = nullptr;

	Transform* moveCoverTransform = nullptr;

	CountType countType = CountType::SLIDE;

	// Component을(를) 통해 상속됨
	virtual void PreSerialize(json& jsonData) const = 0;
	virtual void PreDeserialize(const json& jsonData) = 0;
	virtual void PostSerialize(json& jsonData) const = 0;
	virtual void PostDeserialize(const json& jsonData) = 0;
};