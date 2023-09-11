#pragma once

#include "ZeldaMath.h"

#include <vector>
#include <set>

namespace ZeldaEngine
{
	class Collider2D;

	/// <summary>
	/// PhysicSystem ��� �̸��� ��������
	/// 4q�̴����� ���ʱ����� 2D���� ������ ���� �������.
	/// �׷��� ������ Rigidbody2D�� 2D������ ���� ��ȹ�� ����.
	/// </summary>
	class PhysicsSystem2D
	{
	public:
		void Initialize();
		void Reset(); // ���ο� ���� �ε��Ҷ� ���
		void PrePhysicsEvent();
		void CollisionDetection();
		void PhysicsEvent();
		void CollisionEvent();

		void AddCollider(Collider2D* collider);
		void RemoveCollider(Collider2D* collider);

	private:
		bool CollisionDetectionByOBB(Collider2D* colliderA, Collider2D* colliderB);

		std::vector<Collider2D*> colliderList;

		std::set<std::pair<Collider2D*, Collider2D*>> lastCollisionResult;
		std::set<std::pair<Collider2D*, Collider2D*>> collisionResult;

		//singleton
	public:
		static PhysicsSystem2D& GetInstance();

	private:
		PhysicsSystem2D();
		~PhysicsSystem2D();
		PhysicsSystem2D(const PhysicsSystem2D& ref) = delete;
		PhysicsSystem2D& operator=(const PhysicsSystem2D& ref) = delete;
	};
}