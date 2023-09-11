#pragma once

#include "ZeldaEngine.h"

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

class SampleComponent : public Component
{
public:
	void Awake();
	void Update();

	Camera* cameraptr;

private:
	float moveSpeed;

	Vector3D vector;



private:
	// Component��(��) ���� ��ӵ�
	virtual void PreSerialize(json& jsonData) const override;
	virtual void PreDeserialize(const json& jsonData) override;
	virtual void PostSerialize(json& jsonData) const override;
	virtual void PostDeserialize(const json& jsonData) override;
};

