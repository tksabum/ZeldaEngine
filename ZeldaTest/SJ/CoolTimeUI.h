#pragma once

#include "ZeldaEngine.h"

// ���ϰ� ������ using namespace 2���� �������� ���ִ°� ��������
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

	/// �÷��̾ �ƴ϶� �� �� ���� ������ ������ �켱 GameObject �� ó��
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

	// Component��(��) ���� ��ӵ�
	virtual void PreSerialize(json& jsonData) const = 0;
	virtual void PreDeserialize(const json& jsonData) = 0;
	virtual void PostSerialize(json& jsonData) const = 0;
	virtual void PostDeserialize(const json& jsonData) = 0;
};