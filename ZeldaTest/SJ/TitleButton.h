#pragma once

#include "ZeldaEngine.h"

// 편하게 쓰려면 using namespace 2개는 고정으로 해주는게 좋을것임
using namespace ZeldaEngine;
using namespace ZeldaEngine::ZeldaMath;

class AttachUI;

class TitleButton : public Component
{
public:
	virtual void Awake() override;
	virtual void Start() override;

	virtual void OnMouseEnter() override;
	virtual void OnMouseExit() override;

	void SetAttachUI(const AttachUI& attachUI);
	AttachUI* GetAttachUI() { return attachUI; }
	void SetUISize(const Vector2D& size);
	void SetAttachUISize(const Vector2D& size);
	bool GetMouseEnter() { return mouseEnter; }
	void SetMouseEnter(bool val) { mouseEnter = val; }
	bool GetMouseExit() { return mouseExit; }
	void SetMouseExit(bool val) { mouseExit = val; }

private:
	Button* button = nullptr;
	AttachUI* attachUI = nullptr;
	Vector2D UISize = Vector2D::zero;
	Vector2D attachUISize = Vector2D::zero;
	bool mouseEnter = false;
	bool mouseExit = false;

	// Component을(를) 통해 상속됨
	virtual void PreSerialize(json& jsonData) const override;
	virtual void PreDeserialize(const json& jsonData) override;
	virtual void PostSerialize(json& jsonData) const override;
	virtual void PostDeserialize(const json& jsonData) override;
};