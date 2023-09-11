#pragma once

#include "ZeldaEngine.h"

// 편하게 쓰려면 using namespace 2개는 고정으로 해주는게 좋을것임
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

	// Component을(를) 통해 상속됨
	virtual void PreSerialize(json& jsonData) const override;
	virtual void PreDeserialize(const json& jsonData) override;
	virtual void PostSerialize(json& jsonData) const override;
	virtual void PostDeserialize(const json& jsonData) override;
};