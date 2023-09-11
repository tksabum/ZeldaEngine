#pragma once

#include <vector>
#include <set>
#include <map>
#include <queue>
#include <stack>
#include <unordered_set>
#include <algorithm>

#include "ZeldaMath.h"

namespace ZeldaEngine
{
	class Rigidbody;
	class Collider;
	class IGraphics;
	template<typename T> class Octree;
	
	class PhysicsSystem
	{
	public:
		void Initialize();
		void Reset(); // ���ο� ���� �ε��Ҷ� ���
		void PrePhysicsEvent();
		void CollisionDetection();
		void PhysicsEvent();
		void CollisionEvent();

		void AddCollider(Collider* collider);
		void RemoveCollider(Collider* collider);

		void AddRigidbody(Rigidbody* rigidbody);
		void RemoveRigidbody(Rigidbody* rigidbody);

		void RenderDebuggingInfo(IGraphics* graphics);

	private:
		enum SimplexResult
		{
			CollisionPossible,		// �浹 ������
			CollisionImpossible,	// �浹 �Ұ�����
			CollisionOccurred		// �浹����
		};

		struct CollisionInfo
		{
			Collider* colliderA = nullptr;
			Collider* colliderB = nullptr;
			ZeldaMath::Vector3D position;	// �浹 ��ġ
			ZeldaMath::Vector3D normalA;	// A�� �浹 ���� + �浹 ũ��
		};

		struct Face
		{
			size_t v[3];
		};

		// GJK���� ����ϱ� ���� Ŭ����
		class Simplex
		{
		public:
			Simplex();

			ZeldaMath::Vector3D& operator[](int n);
			std::vector<ZeldaMath::Vector3D>& operator=(const std::vector<ZeldaMath::Vector3D>& right);
			void Push(const ZeldaMath::Vector3D& value);

			// ���ü ����� ����, ���ü ��� ����⿡ ���н�(�� ���� �� ��鿡 �ִ� ��� flase�� ����)
			bool CreateTetrahedron();
			// ���� Ȯ����
			bool ExpandFace(Collider* colliderA, Collider* colliderB);
			// ���� ����� ���� �Ÿ�
			float GetNearestFaceDistance() const;
			// ���� ����� ���� ����
			ZeldaMath::Vector3D GetNearestFaceDirection() const;

			size_t Size();

		//private:
			std::vector<ZeldaMath::Vector3D> vertices;
			std::multimap<float, Face> indices;

			ZeldaMath::Vector3D innerPoint;
		};

		bool isUpdatedHierarchy;

		// �浹����
		std::set<std::pair<Collider*, Collider*>>* lastCollisionResult;
		std::set<std::pair<Collider*, Collider*>>* collisionResult;
		// �浹�� Simplex ����
		std::map<std::pair<Collider*, Collider*>, Simplex> collisionSimplexResult;
		std::map<std::pair<Collider*, Collider*>, CollisionInfo> collisionInfoResult;

		Octree<Collider>* colliderTree;

		std::vector<Collider*> colliderList;
		std::vector<Rigidbody*> rigidbodyList;
		std::vector<Rigidbody*> allRigidbodyList;

		const static size_t GJK_MAXLOOP;
		const static size_t EPA_MAXLOOP;
		const static float EPA_EPSILON;				// EPA���� �ٸ�ü�� Ȯ���� ����, ���ο� ���� ������ ���� ������� �Ÿ����̰� EPA_EPSILON���� ���� �� EPA�� �����Ѵ�.
		const static float EPA_VERTEX_EPSILON;		// �浹������ �Ǵ��� ��, �浹 ������ ���� ���� �ִ� �����κ��� EPA_VERTEX_EPSILON���� ���� ���̳��� ��� ����� ���� �浹������ ����Ѵ�.
		const static float COLLISION_EPSILON;		// �߰��� �о�� ũ��

		// 1. ��ģ ������ ���� ������ �ӵ��� �ִ� ���
		const static float INV_VELOCITY_DEPTH_MIN;	// �������� ������ �ӵ��� ���ϴ� �ּ� ����
		const static float INV_VELOCITY_SCALE;		// �������� ������ �ӵ��� ũ��

		// 2. ��ģ ���̿� ���� ���� �ӵ��� ���� ������ ������ �ӵ��� �ִ� ���
		const static float INV_VELOCITY_RATE_DEPTH_MIN;	// ������ �� ������ �ӵ��� ����ӵ��� ������ ���ϴ� �ּұ���
		const static float INV_VELOCITY_RATE_DEPTH_MAX; // �ӵ��� ������ 0���� ����� ����
		const static float INV_VELOCITY_MIN_RATE;		// �ּұ��̿��� ���ϴ� ����

		void UpdateHierarchy();

		// �� ��ü ������ ��������� ����Ѵ�.
		float ComputeFriction(float frictionA, float frictionB);

		// point�� convexhull ���ο� �ִ��� �Ǵ��Ѵ�.
		bool IsInnerPointInsideConvexHull(const ZeldaMath::Vector3D& point, const std::vector<ZeldaMath::Vector3D>& convexHull);

		// Graham's Scan�˰����� �̿��ؼ� ConvexHull�� �ٲ۴�. (�� ������� vertices�� ���ڷ� �޴´ٰ� ������ �Լ���)
		std::vector<ZeldaMath::Vector3D> Make2DConvexHull(const std::vector<ZeldaMath::Vector3D>& vertices2D);

		ZeldaMath::Vector3D FindCollisionPosition(const std::vector<ZeldaMath::Vector3D>& verticesA, const std::vector<ZeldaMath::Vector3D>& verticesB, ZeldaMath::Vector3D nor);

		bool CollisionDetectionByGJK(Collider* collierA, Collider* colliderB);

		// CollierA�� �浹�������� ColliderB�� Ż�� �������� CollisionInfo�� ��ȯ�Ѵ�.
		CollisionInfo CollisionDetectionByEPA(Collider* collierA, Collider* colliderB, const Simplex& baseSimplex);

		// GJK�� �⺻�� �Ǵ� Support Point�� ���Ѵ�.
		ZeldaMath::Vector3D FindSupportPoint(Collider* colliderA, Collider* colliderB, ZeldaMath::Vector3D dirVec);

		// Simplex�� ���ο� ������ �����ϴ��� �Ǵ��Ͽ� ��ȯ���ָ�, direction�� ������ ���� ���ɼ��� �ִ� ������ �־��ش�.
		SimplexResult DoSimplex(Simplex& simplex, ZeldaMath::Vector3D& direction);
		SimplexResult DoSimplex2(Simplex& simplex, ZeldaMath::Vector3D& direction);
		SimplexResult DoSimplex3(Simplex& simplex, ZeldaMath::Vector3D& direction);
		SimplexResult DoSimplex4(Simplex& simplex, ZeldaMath::Vector3D& direction);

		static std::vector<Simplex> EPASimplex;

		//singleton
	public:
		static PhysicsSystem& GetInstance();

	private:
		PhysicsSystem();
		~PhysicsSystem();
		PhysicsSystem(const PhysicsSystem& ref) = delete;
		PhysicsSystem& operator=(const PhysicsSystem& ref) = delete;
	};
}