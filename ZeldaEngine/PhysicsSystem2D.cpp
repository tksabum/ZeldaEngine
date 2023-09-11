#include "pch.h"
#include "PhysicsSystem2D.h"

#include "SceneManager.h"
#include "GameObject.h"
#include "Transform.h"
#include "Camera.h"

#include "Collider2D.h"
#include "ZeldaUI.h"

#include "EngineSetting.h"

namespace ZeldaEngine
{
	using namespace ZeldaMath;

	void PhysicsSystem2D::Initialize()
	{

	}

	void PhysicsSystem2D::Reset()
	{
		lastCollisionResult.clear();
		collisionResult.clear();
	}

	void PhysicsSystem2D::PrePhysicsEvent()
	{

	}

	void PhysicsSystem2D::CollisionDetection()
	{
		lastCollisionResult = collisionResult;
		collisionResult.clear();

		// 트리같은거 없이 2중 for문으로 처리한다.
		for (int i = 0; i < static_cast<int>(colliderList.size()) - 1; i++)
		{
			for (int j = i + 1; j < colliderList.size(); j++)
			{
				// 게임오브젝트가 Active상태가 아니라면 충돌체크를 하지 않는다.
				if (!colliderList[i]->GetGameObject()->GetRootActive() || !colliderList[j]->GetGameObject()->GetRootActive())
				{
					continue;
				}

				// 서로 충돌하지 않는 Layer인 경우
				if (EngineSetting::GetInstance().GetCollisionSetting(colliderList[i]->GetGameObject()->GetLayerID(), colliderList[j]->GetGameObject()->GetLayerID()) == false)
				{
					continue;
				}

				bool result = CollisionDetectionByOBB(colliderList[i], colliderList[j]);

				if (result)
				{
					collisionResult.insert({ colliderList[i], colliderList[j] });
				}
			}
		}
	}

	void PhysicsSystem2D::PhysicsEvent()
	{

	}

	void PhysicsSystem2D::CollisionEvent()
	{
		std::set<std::pair<Collider2D*, Collider2D*>> enterList;
		std::set<std::pair<Collider2D*, Collider2D*>> stayList;
		std::set<std::pair<Collider2D*, Collider2D*>> exitList;

		std::set<Collider2D*> singleEnterList;
		std::set<Collider2D*> singleStayList;
		std::set<Collider2D*> singleExitList;

		for (auto iter = collisionResult.begin(); iter != collisionResult.end(); iter++)
		{
			if (lastCollisionResult.count(*iter) == 0)
			{
				enterList.insert(*iter);
				singleEnterList.insert(iter->first);
				singleEnterList.insert(iter->second);
			}
			else
			{
				stayList.insert(*iter);
				singleStayList.insert(iter->first);
				singleStayList.insert(iter->second);
			}
		}

		for (auto iter = lastCollisionResult.begin(); iter != lastCollisionResult.end(); iter++)
		{
			if (collisionResult.count(*iter) == 0)
			{
				exitList.insert(*iter);
				singleExitList.insert(iter->first);
				singleExitList.insert(iter->second);
			}
		}


		for (auto iter = singleEnterList.begin(); iter != singleEnterList.end(); iter++)
		{
			(*iter)->GetGameObject()->OnCollisionEnter2D();
		}

		for (auto iter = singleStayList.begin(); iter != singleStayList.end(); iter++)
		{
			(*iter)->GetGameObject()->OnCollisionStay2D();
		}

		for (auto iter = singleExitList.begin(); iter != singleExitList.end(); iter++)
		{
			(*iter)->GetGameObject()->OnCollisionExit2D();
		}


		for (auto iter = enterList.begin(); iter != enterList.end(); iter++)
		{
			(*iter).first->GetGameObject()->OnCollisionEnter2D(iter->second);
			(*iter).second->GetGameObject()->OnCollisionEnter2D(iter->first);
		}

		for (auto iter = stayList.begin(); iter != stayList.end(); iter++)
		{
			(*iter).first->GetGameObject()->OnCollisionStay2D(iter->second);
			(*iter).second->GetGameObject()->OnCollisionStay2D(iter->first);
		}

		for (auto iter = exitList.begin(); iter != exitList.end(); iter++)
		{
			(*iter).first->GetGameObject()->OnCollisionExit2D(iter->second);
			(*iter).second->GetGameObject()->OnCollisionExit2D(iter->first);
		}

	}

	void PhysicsSystem2D::AddCollider(Collider2D* collider)
	{
		colliderList.push_back(collider);
	}

	void PhysicsSystem2D::RemoveCollider(Collider2D* collider)
	{
		colliderList.erase(remove(colliderList.begin(), colliderList.end(), collider), colliderList.end());
	}

	bool PhysicsSystem2D::CollisionDetectionByOBB(Collider2D* colliderA, Collider2D* colliderB)
	{
		const std::vector<Vector2D>& orgVerticesA = colliderA->GetVertices();
		const std::vector<Vector2D>& orgVerticesB = colliderB->GetVertices();

		std::vector<Vector2D> verticesA = orgVerticesA;
		std::vector<Vector2D> verticesB = orgVerticesB;

		// 정점의 Scale, Rotation 변환
		float rotA = colliderA->GetGameObject()->GetTransform()->GetWorldUIRotation();
		float rotB = colliderB->GetGameObject()->GetTransform()->GetWorldUIRotation();

		Matrix4x4 rotMatA = Matrix4x4::Rotation(-Vector3D::front, DegreeToRadian(rotA));
		Matrix4x4 rotMatB = Matrix4x4::Rotation(-Vector3D::front, DegreeToRadian(rotB));

		Vector3D posA = colliderA->GetGameObject()->GetTransform()->GetScreenPosition();
		Vector3D posB = colliderB->GetGameObject()->GetTransform()->GetScreenPosition();

		float cameraFar = SceneManager::GetInstance().GetMainCamera()->GetCameraFar();
		float cameraNear = SceneManager::GetInstance().GetMainCamera()->GetCameraNear();

		Vector3D scaleA = colliderA->GetGameObject()->GetTransform()->GetWorldScale();
		Vector3D scaleB = colliderB->GetGameObject()->GetTransform()->GetWorldScale();

		if (posA.z != 0.0f) scaleA *= posA.z / ((cameraNear + cameraFar) * 0.5f);
		if (posB.z != 0.0f) scaleB *= posB.z / ((cameraNear + cameraFar) * 0.5f);

		for (size_t i = 0; i < verticesA.size(); i++)
		{
			Matrix4x1 m41 = Matrix4x1(verticesA[i].x, verticesA[i].y, 0.0f, 0.0f);
			Vector3D v = rotMatA * Matrix4x4::Scale(scaleA) * m41;
			verticesA[i] = Vector2D(v.x, v.y);
		}

		for (size_t i = 0; i < verticesB.size(); i++)
		{
			Matrix4x1 m41 = Matrix4x1(verticesB[i].x, verticesB[i].y, 0.0f, 0.0f);
			Vector3D v = rotMatB * Matrix4x4::Scale(scaleB) * m41;
			verticesB[i] = Vector2D(v.x, v.y);
		}

		// 정점위치 + 화면상의 중심위치 계산
		for (size_t i = 0; i < verticesA.size(); i++)
		{
			verticesA[i] += posA;
		}

		for (size_t i = 0; i < verticesB.size(); i++)
		{
			verticesB[i] += posB;
		}

		




		// 각 변에 대해 회전을 하면서 충돌판정
		for (size_t i = 0; i < verticesA.size(); i++)
		{
			Vector2D a = verticesA[i];
			Vector2D b = verticesA[(i + 1) % verticesA.size()];

			Vector2D ab = b - a;

			float mindotA = INFINITY;
			float maxdotA = -INFINITY;
			float mindotB = INFINITY;
			float maxdotB = -INFINITY;

			for (size_t j = 0; j < verticesA.size(); j++)
			{
				float dot = DotProduct(ab, verticesA[j]);

				if (dot < mindotA) mindotA = dot;
				if (dot > maxdotA) maxdotA = dot;
			}

			for (size_t j = 0; j < verticesB.size(); j++)
			{
				float dot = DotProduct(ab, verticesB[j]);

				if (dot < mindotB) mindotB = dot;
				if (dot > maxdotB) maxdotB = dot;
			}

			if (maxdotA < mindotB || maxdotB < mindotA) return false;
		}

		for (size_t i = 0; i < verticesB.size(); i++)
		{
			Vector2D a = verticesB[i];
			Vector2D b = verticesB[(i + 1) % verticesB.size()];

			Vector2D ab = b - a;

			float mindotA = INFINITY;
			float maxdotA = -INFINITY;
			float mindotB = INFINITY;
			float maxdotB = -INFINITY;

			for (size_t j = 0; j < verticesA.size(); j++)
			{
				float dot = DotProduct(ab, verticesA[j]);

				if (dot < mindotA) mindotA = dot;
				if (dot > maxdotA) maxdotA = dot;
			}

			for (size_t j = 0; j < verticesB.size(); j++)
			{
				float dot = DotProduct(ab, verticesB[j]);

				if (dot < mindotB) mindotB = dot;
				if (dot > maxdotB) maxdotB = dot;
			}

			if (maxdotA < mindotB || maxdotB < mindotA) return false;
		}

		return true;
	}

	PhysicsSystem2D& PhysicsSystem2D::GetInstance()
	{
		static PhysicsSystem2D instance;
		return instance;
	}

	PhysicsSystem2D::PhysicsSystem2D()
	{

	}

	PhysicsSystem2D::~PhysicsSystem2D()
	{

	}
}