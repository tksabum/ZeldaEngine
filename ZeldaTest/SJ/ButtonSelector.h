#pragma once

#include "ZeldaEngine.h"

// ���ϰ� ������ using namespace 2���� �������� ���ִ°� ��������
using namespace ZeldaEngine;
using namespace ZeldaEngine::ZeldaMath;

class ButtonSelector : public Component
{
public:
	virtual void Awake() override;
	virtual void Start() override;
	virtual void LateUpdate() override;

	void AddButton(const GameObject& obj);
	bool SetFirstButton(const GameObject& obj);

private:
	enum class ButtonType
	{
		NONE,
		Start,
		Exit,
		Continue,
		Title,
		Restart
	};

	void SelectButton(GameObject& obj);
	void DeselectButton(GameObject& obj);
	void InvokeButton(GameObject& obj);
	GameObject* GetBeforeButton(GameObject& obj);
	GameObject* GetNextButton(GameObject& obj);
	ButtonType GetButtonType(GameObject& obj);
	void MouseEnterUpdate();

private:
	// ��� ������ �ƴ϶� GameObject �� �����ϰ�, �̸� ���� �����ϴ� ������� ���
	std::vector<GameObject*> buttonList;
	GameObject* firstButton = nullptr;
	GameObject* currentButton = nullptr;

	// Component��(��) ���� ��ӵ�
	virtual void PreSerialize(json& jsonData) const override;
	virtual void PreDeserialize(const json& jsonData) override;
	virtual void PostSerialize(json& jsonData) const override;
	virtual void PostDeserialize(const json& jsonData) override;
};