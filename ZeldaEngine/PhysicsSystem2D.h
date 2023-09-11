#pragma once

#include "ZeldaMath.h"

#include <vector>
#include <set>

namespace ZeldaEngine
{
	class Collider2D;

	/// <summary>
	/// PhysicSystem 라고 이름을 지었지만
	/// 4q미니플젝 사필귀정의 2D패턴 구현을 위해 만들었다.
	/// 그래서 당장은 Rigidbody2D와 2D물리를 만들 계획은 없다.
	/// </summary>
	class PhysicsSystem2D
	{
	public:
		void Initialize();
		void Reset(); // 새로운 씬을 로드할때 사용
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