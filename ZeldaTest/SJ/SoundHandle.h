#pragma once

#include "ZeldaEngine.h"

// ���ϰ� ������ using namespace 2���� �������� ���ִ°� ��������
using namespace ZeldaEngine;
using namespace ZeldaEngine::ZeldaMath;

class SoundHandle : public Component
{
public:
	virtual void Awake() override;

	virtual void OnMouseLeftDown() override;

	bool IsMouseLeftDown();

private:
	bool nowLeftDown = false;

	ZeldaUI* ui = nullptr;
	Image* image = nullptr;

	// Component��(��) ���� ��ӵ�
	virtual void PreSerialize(json& jsonData) const override;
	virtual void PreDeserialize(const json& jsonData) override;
	virtual void PostSerialize(json& jsonData) const override;
	virtual void PostDeserialize(const json& jsonData) override;
};