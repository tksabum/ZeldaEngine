#pragma once

#include "ZeldaEngine.h"

// 편하게 쓰려면 using namespace 2개는 고정으로 해주는게 좋을것임
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
	// 상속 구조가 아니라서 GameObject 를 장착하고, 이를 통해 접근하는 방식으로 사용
	std::vector<GameObject*> buttonList;
	GameObject* firstButton = nullptr;
	GameObject* currentButton = nullptr;

	// Component을(를) 통해 상속됨
	virtual void PreSerialize(json& jsonData) const override;
	virtual void PreDeserialize(const json& jsonData) override;
	virtual void PostSerialize(json& jsonData) const override;
	virtual void PostDeserialize(const json& jsonData) override;
};