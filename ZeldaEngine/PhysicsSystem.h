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
		void Reset(); // 새로운 씬을 로드할때 사용
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
			CollisionPossible,		// 충돌 가능함
			CollisionImpossible,	// 충돌 불가능함
			CollisionOccurred		// 충돌했음
		};

		struct CollisionInfo
		{
			Collider* colliderA = nullptr;
			Collider* colliderB = nullptr;
			ZeldaMath::Vector3D position;	// 충돌 위치
			ZeldaMath::Vector3D normalA;	// A의 충돌 방향 + 충돌 크기
		};

		struct Face
		{
			size_t v[3];
		};

		// GJK에서 사용하기 위한 클래스
		class Simplex
		{
		public:
			Simplex();

			ZeldaMath::Vector3D& operator[](int n);
			std::vector<ZeldaMath::Vector3D>& operator=(const std::vector<ZeldaMath::Vector3D>& right);
			void Push(const ZeldaMath::Vector3D& value);

			// 사면체 모양을 만듦, 사면체 모양 만들기에 실패시(네 점이 한 평면에 있는 경우 flase를 리턴)
			bool CreateTetrahedron();
			// 면을 확장함
			bool ExpandFace(Collider* colliderA, Collider* colliderB);
			// 가장 가까운 면의 거리
			float GetNearestFaceDistance() const;
			// 가장 가까운 면의 방향
			ZeldaMath::Vector3D GetNearestFaceDirection() const;

			size_t Size();

		//private:
			std::vector<ZeldaMath::Vector3D> vertices;
			std::multimap<float, Face> indices;

			ZeldaMath::Vector3D innerPoint;
		};

		bool isUpdatedHierarchy;

		// 충돌정보
		std::set<std::pair<Collider*, Collider*>>* lastCollisionResult;
		std::set<std::pair<Collider*, Collider*>>* collisionResult;
		// 충돌한 Simplex 정보
		std::map<std::pair<Collider*, Collider*>, Simplex> collisionSimplexResult;
		std::map<std::pair<Collider*, Collider*>, CollisionInfo> collisionInfoResult;

		Octree<Collider>* colliderTree;

		std::vector<Collider*> colliderList;
		std::vector<Rigidbody*> rigidbodyList;
		std::vector<Rigidbody*> allRigidbodyList;

		const static size_t GJK_MAXLOOP;
		const static size_t EPA_MAXLOOP;
		const static float EPA_EPSILON;				// EPA에서 다면체를 확장해 갈때, 새로운 면이 기존의 가장 가까운면과 거리차이가 EPA_EPSILON보다 작을 때 EPA를 종료한다.
		const static float EPA_VERTEX_EPSILON;		// 충돌지점을 판단할 때, 충돌 방향의 가장 끝에 있는 점으로부터 EPA_VERTEX_EPSILON보다 적게 차이나는 모든 점들로 부터 충돌지점을 계산한다.
		const static float COLLISION_EPSILON;		// 추가로 밀어내는 크기

		// 1. 겹친 깊이의 일정 비율로 속도를 주는 방법
		const static float INV_VELOCITY_DEPTH_MIN;	// 겹쳤을때 역방향 속도를 가하는 최소 깊이
		const static float INV_VELOCITY_SCALE;		// 겹쳤을때 역방향 속도의 크기

		// 2. 겹친 깊이에 따라 현재 속도의 일정 비율로 역방향 속도를 주는 방법
		const static float INV_VELOCITY_RATE_DEPTH_MIN;	// 겹쳤을 때 역방향 속도를 현재속도의 비율로 가하는 최소깊이
		const static float INV_VELOCITY_RATE_DEPTH_MAX; // 속도를 완전히 0으로 만드는 깊이
		const static float INV_VELOCITY_MIN_RATE;		// 최소깊이에서 가하는 비율

		void UpdateHierarchy();

		// 두 물체 사이의 마찰계수를 계산한다.
		float ComputeFriction(float frictionA, float frictionB);

		// point가 convexhull 내부에 있는지 판단한다.
		bool IsInnerPointInsideConvexHull(const ZeldaMath::Vector3D& point, const std::vector<ZeldaMath::Vector3D>& convexHull);

		// Graham's Scan알고리즘을 이용해서 ConvexHull로 바꾼다. (한 평면위의 vertices를 인자로 받는다고 가정한 함수임)
		std::vector<ZeldaMath::Vector3D> Make2DConvexHull(const std::vector<ZeldaMath::Vector3D>& vertices2D);

		ZeldaMath::Vector3D FindCollisionPosition(const std::vector<ZeldaMath::Vector3D>& verticesA, const std::vector<ZeldaMath::Vector3D>& verticesB, ZeldaMath::Vector3D nor);

		bool CollisionDetectionByGJK(Collider* collierA, Collider* colliderB);

		// CollierA의 충돌방향이자 ColliderB의 탈출 방향으로 CollisionInfo를 반환한다.
		CollisionInfo CollisionDetectionByEPA(Collider* collierA, Collider* colliderB, const Simplex& baseSimplex);

		// GJK의 기본이 되는 Support Point를 구한다.
		ZeldaMath::Vector3D FindSupportPoint(Collider* colliderA, Collider* colliderB, ZeldaMath::Vector3D dirVec);

		// Simplex의 내부에 원점이 존재하는지 판단하여 반환해주며, direction에 원점이 있을 가능성이 있는 방향을 넣어준다.
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