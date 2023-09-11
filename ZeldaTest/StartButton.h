#pragma once

#include "ZeldaEngine.h"

// ���ϰ� ������ using namespace 2���� �������� ���ִ°� ��������
using namespace ZeldaEngine;
using namespace ZeldaEngine::ZeldaMath;

class AttachUI;

class StartButton : public Component
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
	Vector2D UISize = Vector2D::zero;
	Vector2D attachUISize = Vector2D::zero;
	Button* button = nullptr;
	AttachUI* attachUI = nullptr;
	bool mouseEnter = false;
	bool mouseExit = false;

	// Component��(��) ���� ��ӵ�
	virtual void PreSerialize(json& jsonData) const override;
	virtual void PreDeserialize(const json& jsonData) override;
	virtual void PostSerialize(json& jsonData) const override;
	virtual void PostDeserialize(const json& jsonData) override;
};