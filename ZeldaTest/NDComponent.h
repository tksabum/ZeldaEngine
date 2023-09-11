#pragma once

#include "ZeldaEngine.h"
#include "FSM.h"
#include <list>

// ���ϰ� ������ using namespace 2���� �������� ���ִ°� ��������
using namespace ZeldaEngine;
using namespace ZeldaEngine::ZeldaMath;

// vector, map�� serialize�� �ʿ��ϴ�.....

// ������Ʈ ���� ���� Component Ŭ������ ��ӹ޾ƾ��Ѵ�.
// �Ʒ��� ���� ��� ������ �̺�Ʈ�Լ� ���
// void Awake();							���ӿ�����Ʈ�� Ȱ��ȭ�� ��, �� �ѹ��� ȣ��ȴ�.
// void Start();							���ӿ�����Ʈ�� Ȱ��ȭ�� ��, �� �ѹ��� Awake ���Ŀ� ȣ��ȴ�.
// void OnEnable();							���ӿ�����Ʈ�� Ȱ��ȭ�� ��, �Ź� ȣ��ȴ�.
// void OnDisable();						���ӿ�����Ʈ�� ��Ȱ��ȭ�� ��, �Ź� ȣ��ȴ�.
// void OnDestroy();						���ӿ�����Ʈ�� ���ŵ� ��, �� �ѹ��� ȣ��ȴ�.
// 
// void OnTriggerEnter();					// rigidbody�� ���� ������Ʈ�� ������ ������Ʈ�� �浹�� ������ ���� ȣ��ȴ�.
// void OnTriggerStay();					// rigidbody�� ���� ������Ʈ�� ������ ������Ʈ�� �浹�� ���¸� ������ �� ȣ��ȴ�.
// void OnTriggerExit();					// rigidbody�� ���� ������Ʈ�� ������ ������Ʈ���� �浹���� ����� ���� ȣ��ȴ�.
// void OnTriggerEnter(Collider* other);	// rigidbody�� ���� ������Ʈ�� other�� �浹�ϴ� ���� ȣ��ȴ�.				(�� �����ӿ� ������ ȣ��� �� �ִ�.)
// void OnTriggerStay(Collider* other);		// rigidbody�� ���� ������Ʈ�� other�� �浹�� ���¸� ������ �� ȣ��ȴ�.		(�� �����ӿ� ������ ȣ��� �� �ִ�.)
// void OnTriggerExit(Collider* other);		// rigidbody�� ���� ������Ʈ�� other�� �浹���� ����� ���� ȣ��ȴ�		(�� �����ӿ� ������ ȣ��� �� �ִ�.)
// 
// void EarlyUpdate();						// �� ������ �ѹ����� ȣ��ȴ�. Update ���� ó���� ���� �ִ� ��� ����Ѵ�.
// void Update();							// �� ������ �ѹ����� ȣ��ȴ�. �⺻���� Update�� ���� �Լ�
// void LateUpdate();						// �� ������ �ѹ����� ȣ��ȴ�. Update �Ŀ� ó���� ���� �ִ� ��� ����Ѵ�. (EX.ī�޶��� �÷��̾� ��ġ���� �̵�)

class Dotween;

enum class PlayerState
{
	Idle,
	Move,
	Attack,
	SpecialAttack,
	Interaction,
	StateEnd
};

class NDComponent : public ZeldaEngine::Component
{
public:
	NDComponent();
	//~NDComponent();

	void Awake();
	void Start();
	void Update();

private:
	FSM<PlayerState> playerFSM = FSM<PlayerState>(PlayerState::Idle);

	// �����̴� �ӵ�
	float moveSpeed;
	// ȸ�� �� �ӵ�
	float rotationAngleSpeed;

	// ���� �����ҽ� ��������
	float errorRange;

	// �뽬�� �����ð�
	float dashChargingTime;

	// �÷��̾��� ���� ��
	GameObject* playerAxis;

	//
	GameObject* playerfrontObject;

	// 1. Dotween ������Ʈ ����
	Dotween* playerDotween;

public:
	// Idle ���·� ���� ���� ��
	void IdleEngage();

	// Move ���·� ���� ���� ��
	void MoveEngage();

	// Attack ���·� ���� ���� ��
	void AttackEngage();

	// SpecialAttack ���·� ���� ���� ��
	void SpecialAttackEngage();

	// Interaction ���·� ���� ���� ��
	void InteractionEngage();


	// Idle ���°� ���� ���� ��
	void IdleState();

	// Move ���°� ���� ���� ��
	void MoveState();

	// Attack ���°� ���� ���� ��
	void AttackState();

	// SpecialAttack ���°� ���� ���� ��
	void SpecialAttackState();

	// Interaction ���°� ���� ���� ��
	void InteractionState();

private:
	// �÷��̾��� �����ӿ� ���� �Լ�
	void MoveEvent();

	// ȸ���� �ּҰ��� ���Ѵ�.
	float GetRotationAngle(const Vector3D& local, const Vector3D& world, float angle);

private:
	// Component��(��) ���� ��ӵ�
	virtual void PreSerialize(json& jsonData) const override;
	virtual void PreDeserialize(const json& jsonData) override;
	virtual void PostSerialize(json& jsonData) const override;
	virtual void PostDeserialize(const json& jsonData) override;
};