#pragma once

#include "ZeldaEngine.h"

#include "../SJ/CoolTimeUI.h"

// ���ϰ� ������ using namespace 2���� �������� ���ִ°� ��������
using namespace ZeldaEngine;
using namespace ZeldaEngine::ZeldaMath;

class StandoffCoolTimeUI : public CoolTimeUI
{
public:
	virtual void Awake() override;
	virtual void Start() override;
	virtual void Update() override;

public:
	// Component��(��) ���� ��ӵ�
	virtual void PreSerialize(json& jsonData) const override;
	virtual void PreDeserialize(const json& jsonData) override;
	virtual void PostSerialize(json& jsonData) const override;
	virtual void PostDeserialize(const json& jsonData) override;
};