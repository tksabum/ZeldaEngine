#pragma once

#include "ZeldaEngine.h"

// ���ϰ� ������ using namespace 2���� �������� ���ִ°� ��������
using namespace ZeldaEngine;
using namespace ZeldaEngine::ZeldaMath;

class SettingButton : public Component
{
public:
	virtual void Awake() override;
	virtual void Start() override;
	virtual void Update() override;

	virtual void OnMouseEnter() override;
	virtual void OnMouseExit() override;

	void SetPanel(const Transform& panel);
	void OpenSettingUI();
	void CloseSettingUI();

private:
	Transform* panel = nullptr;
	Button* button = nullptr;
	bool isOpen = false;

	// Component��(��) ���� ��ӵ�
	virtual void PreSerialize(json& jsonData) const override;
	virtual void PreDeserialize(const json& jsonData) override;
	virtual void PostSerialize(json& jsonData) const override;
	virtual void PostDeserialize(const json& jsonData) override;
};