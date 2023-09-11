#pragma once

#include "ZeldaEngine.h"

// ���ϰ� ������ using namespace 2���� �������� ���ִ°� ��������
using namespace ZeldaEngine;
using namespace ZeldaEngine::ZeldaMath;

class SoundHandle;
class ScrollUI;

class SoundBar : public Component
{
public:
	virtual void Awake() override;
	virtual void Start() override;

	virtual void Update() override;

	void SetHandle(const SoundHandle& handle);
	void SetUISize(const Vector2D& size);
	void SetPadding(float padding);

private:
	bool scrolling = false;
	Vector2D lastMousePos = Vector2D::zero;
	Vector2D currentMousePos = Vector2D::zero;
	Vector3D handlePos = Vector3D::zero;
	Vector2D UISize = Vector2D::zero;
	float padding = 0;
	SoundHandle* handle = nullptr;
	ScrollUI* scroll = nullptr;

	// Component��(��) ���� ��ӵ�
	virtual void PreSerialize(json& jsonData) const override;
	virtual void PreDeserialize(const json& jsonData) override;
	virtual void PostSerialize(json& jsonData) const override;
	virtual void PostDeserialize(const json& jsonData) override;
};