#pragma once

#include "ZeldaEngineAPI.h"

#include "Serializable.h"

namespace ZeldaEngine
{
	class ComponentFactory;
	class GameObject;
	class ComponentSystem;
	class Collider;
	class Collider2D;

	class ZELDAENGINE_API Component : public Serializable
	{
	public:
		virtual ~Component();

		virtual void Awake();
		virtual void Start();
		virtual void OnEnable();
		virtual void OnDisable();

		virtual void OnDestroy();

		virtual void OnTriggerEnter();
		virtual void OnTriggerStay();
		virtual void OnTriggerExit();
		virtual void OnTriggerEnter(Collider* other);
		virtual void OnTriggerStay(Collider* other);
		virtual void OnTriggerExit(Collider* other);

		virtual void OnCollisionEnter();
		virtual void OnCollisionStay();
		virtual void OnCollisionExit();
		virtual void OnCollisionEnter(Collider* other);
		virtual void OnCollisionStay(Collider* other);
		virtual void OnCollisionExit(Collider* other);

		virtual void OnCollisionEnter2D();
		virtual void OnCollisionStay2D();
		virtual void OnCollisionExit2D();
		virtual void OnCollisionEnter2D(Collider2D* other);
		virtual void OnCollisionStay2D(Collider2D* other);
		virtual void OnCollisionExit2D(Collider2D* other);

		// ������Ʈ ������ ���콺�� ������ ȣ��
		virtual void OnMouseLeftDown();
		virtual void OnMouseRightDown();
		// ������Ʈ ������ ���콺�� ���� ȣ��
		virtual void OnMouseLeftUp();
		virtual void OnMouseRightUp();

		// ���콺�� ������Ʈ ���� �ö��� �� ȣ��
		virtual void OnMouseEnter();
		// ���콺�� ������Ʈ���� ����� �� ȣ��
		virtual void OnMouseExit();
		// ���콺�� ������Ʈ ���� ������ ��� ȣ��
		virtual void OnMouseOver();

		//virtual void FixedUpdate();
		virtual void EarlyUpdate();
		virtual void Update();
		virtual void LateUpdate();

		GameObject* GetGameObject();

		virtual void PreSerialize(json& jsonData) const abstract;
		virtual void PreDeserialize(const json& jsonData) abstract;
		virtual void PostSerialize(json& jsonData) const abstract;
		virtual void PostDeserialize(const json& jsonData) abstract;
		
	protected:
		GameObject* gameObject;

		Component();

		// Component ��ü�� ������ �� �� �ѹ��� ȣ��Ǵ� �Լ�
		virtual void PreInitialize_();
		virtual void PostInitialize_();

	private:
#ifdef _DEBUG
		static bool addComponentFlag;
		static void SetAddComponentFlag(bool value);
		static bool GetAddComponentFlag();
#endif // _DEBUG


		friend ComponentFactory;
		friend GameObject;
		friend ComponentSystem;
	};
}