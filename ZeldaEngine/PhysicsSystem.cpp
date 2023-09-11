#include "pch.h"
#include "PhysicsSystem.h"

#include "Rigidbody.h"
#include "Collider.h"
#include "GameObject.h"
#include "Transform.h"
#include "Component.h"

#include "IGraphics.h"
#include "ResourceManager.h"
#include "TimeController.h"

#include "EngineSetting.h"

#include "Octree.h"



// 디버깅용 임시..
#include "SceneManager.h"
#include "Input.h"

namespace ZeldaEngine
{
	using namespace ZeldaMath;

	const size_t PhysicsSystem::GJK_MAXLOOP = 50;
	const size_t PhysicsSystem::EPA_MAXLOOP = 50;
	const float PhysicsSystem::EPA_EPSILON = 0.1f;
	const float PhysicsSystem::EPA_VERTEX_EPSILON = 0.1f;
	const float PhysicsSystem::COLLISION_EPSILON = 0.01f;
	const float PhysicsSystem::INV_VELOCITY_DEPTH_MIN = 0.1f;
	const float PhysicsSystem::INV_VELOCITY_SCALE = 3.0f;
	const float PhysicsSystem::INV_VELOCITY_RATE_DEPTH_MIN = 0.2f;
	const float PhysicsSystem::INV_VELOCITY_RATE_DEPTH_MAX = 0.5f;
	const float PhysicsSystem::INV_VELOCITY_MIN_RATE = 0.3f;
	std::vector<PhysicsSystem::Simplex> PhysicsSystem::EPASimplex;

	void PhysicsSystem::Initialize()
	{
		colliderTree = new Octree<Collider>(Vector3D(-1000000, -1000000, -1000000), Vector3D(1000000, 1000000, 1000000), Vector3D(100, 100, 100));
		colliderTree->SetAutoRadiusUpdate(false);

		lastCollisionResult = new std::set<std::pair<Collider*, Collider*>>();
		collisionResult = new std::set<std::pair<Collider*, Collider*>>();
	}

	void PhysicsSystem::Reset()
	{
		lastCollisionResult->clear();
		collisionResult->clear();
	}

	void PhysicsSystem::PrePhysicsEvent()
	{
		for (size_t i = 0; i < allRigidbodyList.size(); i++)
		{
			if (allRigidbodyList[i]->GetGameObject()->GetTransform()->IsChangedParent())
			{
				isUpdatedHierarchy = false;
				allRigidbodyList[i]->GetGameObject()->GetTransform()->ResetChangedParent();
			}
		}

		for (size_t i = 0; i < colliderList.size(); i++)
		{
			if (colliderList[i]->GetGameObject()->GetTransform()->IsChangedParent())
			{
				isUpdatedHierarchy = false;
				colliderList[i]->GetGameObject()->GetTransform()->ResetChangedParent();
			}
		}

		// Rigidbody와 collider관계를 정리한다.
		if (isUpdatedHierarchy == false)
		{
			isUpdatedHierarchy = true;

			UpdateHierarchy();
		}
	}

	void PhysicsSystem::CollisionDetection()
	{
		// CollisionResult와 LastCollisionResult를 스왑한 후
		// CollisionResult 초기화
		auto* temp = collisionResult;
		collisionResult = lastCollisionResult;
		lastCollisionResult = temp;
		collisionResult->clear();

		// 충돌 결과의 Simplex를 저장하는 collisionSimplexResult 초기화
		collisionSimplexResult.clear();

		// EPA 결과 초기화
		collisionInfoResult.clear();

		// Octree 업데이트
		for (size_t i = 0; i < colliderList.size(); i++)
		{
			// 이동한 경우에만 업데이트
			if (colliderList[i]->GetGameObject()->GetTransform()->IsMoved())
			{
				colliderTree->Move(colliderList[i]->GetCenter(), colliderList[i]->GetMaxRadius(), colliderList[i]);

				// 이동상태 리셋
				colliderList[i]->GetGameObject()->GetTransform()->ResetMoved();
			}
		}

		// colliderTree는 SetAutoRadiusUpdate(false)으로 설정되어 있기 때문에 사용하기전에 반드시 UpdateAllRadius를 호출해 줘야 한다.
		colliderTree->UpdateAllRadius();

		// 충돌 체크
		for (size_t i = 0; i < colliderList.size(); i++)
		{
			Collider* currentCollider = colliderList[i];

			Vector3D center = currentCollider->GetCenter();
			float maxRadius = currentCollider->GetMaxRadius();

			// 충돌 가능한 콜라이더를 Octree를 이용해서 한차례 거른다.
			std::vector<Collider*> others = colliderTree->Query(center - Vector3D(1, 1, 1) * maxRadius, center + Vector3D(1, 1, 1) * maxRadius);

			for (size_t j = 0; j < others.size(); j++)
			{
				// Octree에는 자기자신도 들어있기 때문에 자기 자신과의 충돌처리는 막아야한다.
				if (currentCollider == others[j])
				{
					continue;
				}

				// 중복된 검사를 피하기 위해 주소값이 작은 콜라이더가 currentCollider일 때만 충돌체크를 한다.
				if (currentCollider > others[j])
				{
					continue;
				}

				// 게임오브젝트가 Active상태가 아니라면 충돌체크를 하지 않는다.
				if (!currentCollider->GetGameObject()->GetRootActive() || !others[j]->GetGameObject()->GetRootActive())
				{
					continue;
				}
				
				if (EngineSetting::GetInstance().GetCollisionSetting(currentCollider->GetGameObject()->GetLayerID(), others[j]->GetGameObject()->GetLayerID()) == false)
				{
					continue;
				}

				// trigger는 trigger끼리만, trigger가 아닌 것은 trigger가 아닌것 끼리만 충돌 가능
				if (currentCollider->IsTrigger() != others[j]->IsTrigger())
				{
					continue;
				}

				// 같은 rigidbody 내의 콜라이더 끼리는 충돌하지 않는다.
				if (currentCollider->GetMaster() != nullptr && others[j]->GetMaster() != nullptr && currentCollider->GetMaster()->GetRoot() == others[j]->GetMaster()->GetRoot())
				{
					continue;
				}
				
				// 실제 콜라이더간의 충돌체크
				bool result = CollisionDetectionByGJK(currentCollider, others[j]);
				
				if (result)
				{
					collisionResult->insert({ currentCollider, others[j] });
				}
			}
		}

		for (auto iter = collisionSimplexResult.begin(); iter != collisionSimplexResult.end(); iter++)
		{
			Collider* colliderA = (*iter).first.first;
			Collider* colliderB = (*iter).first.second;

			// Trigger는 충돌위치와 충돌깊이를 계산할 필요가 없다.
			if (colliderA->IsTrigger() == true && colliderB->IsTrigger() == true)
			{
				continue;
			}

			// Rigidbody가 없으면 물리연산을 하지 않음 (Trigger와 마찬가지)
			if (colliderA->GetMaster() == nullptr || colliderB->GetMaster() == nullptr)
			{
				continue;
			}

			CollisionInfo collisionInfo = CollisionDetectionByEPA(colliderA, colliderB, (*iter).second);

			// 노말이 0인 경우는 물리연산을 하지 않는다.
			if (collisionInfo.normalA != Vector3D::zero)
			{
				collisionInfoResult[{ min(colliderA, colliderB), max(colliderA, colliderB) }] = collisionInfo;
			}
		}

		// Octree를 사용하지 않는 충돌 체크
		//for (size_t i = 0; i < colliderList.size(); i++)
		//{
		//	Collider* currentCollider = colliderList[i];

		//	for (size_t j = 0; j < colliderList.size(); j++)
		//	{
		//		// Octree에는 자기자신도 들어있기 때문에 자기 자신과의 충돌처리는 막아야한다.
		//		if (currentCollider == colliderList[j])
		//		{
		//			continue;
		//		}

		//		// 중복된 검사를 피하기 위해 주소값이 작은 콜라이더가 currentCollider일 때만 충돌체크를 한다.
		//		if (currentCollider > colliderList[j])
		//		{
		//			continue;
		//		}

		//		// 게임오브젝트가 Active상태가 아니라면 충돌체크를 하지 않는다.
		//		if (!currentCollider->GetGameObject()->GetRootActive() || !colliderList[j]->GetGameObject()->GetRootActive())
		//		{
		//			continue;
		//		}

		//		// 실제 콜라이더간의 충돌체크
		//		bool result = CollisionDetectionByGJK(currentCollider, colliderList[j]);

		//		if (result)
		//		{
		//			collisionResult->insert({ currentCollider, colliderList[j] });
		//		}
		//	}
		//}


	}

	void PhysicsSystem::PhysicsEvent()
	{
		SceneManager* scenetest = &SceneManager::GetInstance();

		const float deltaTime = TimeController::GetInstance().GetPhysicsDeltaTime();

		// rigidbody간의 점촉점 갯수를 계산해야한다.
		std::map<std::pair<Rigidbody*, Rigidbody*>, int> rigidbodyCollsionCounter;

		for (auto iter = collisionInfoResult.begin(); iter != collisionInfoResult.end(); iter++)
		{
			Rigidbody* rigidbodyA = (*iter).second.colliderA->GetMaster()->GetRoot();
			Rigidbody* rigidbodyB = (*iter).second.colliderB->GetMaster()->GetRoot();

			Vector3D nor = (*iter).second.normalA;
			Vector3D pos = (*iter).second.position;

			if (rigidbodyCollsionCounter.count({ min(rigidbodyA, rigidbodyB), max(rigidbodyA, rigidbodyB) }) == 0)
			{
				rigidbodyCollsionCounter[{ min(rigidbodyA, rigidbodyB), max(rigidbodyA, rigidbodyB) }] = 1;
			}
			else
			{
				rigidbodyCollsionCounter[{ min(rigidbodyA, rigidbodyB), max(rigidbodyA, rigidbodyB) }] += 1;
			}
		}

		// 접촉점 갯수 만큼 분산된 힘이 적용된다.
		for (auto iter = collisionInfoResult.begin(); iter != collisionInfoResult.end(); iter++)
		{
			Rigidbody* rigidbodyA = (*iter).second.colliderA->GetMaster()->GetRoot();
			Rigidbody* rigidbodyB = (*iter).second.colliderB->GetMaster()->GetRoot();

			// Rigidbody가 없다면 연산할 필요가 없다.
			if (rigidbodyA == nullptr || rigidbodyB == nullptr) continue;

			// force, tanforce에 영향을 준다.
			float collisionCounter = static_cast<float>(rigidbodyCollsionCounter[{ min(rigidbodyA, rigidbodyB), max(rigidbodyA, rigidbodyB) }]);

			assert(collisionCounter != 0.0f);
			if (collisionCounter == 0.0f)
			{
				collisionCounter = 1.0f;
			}

			Vector3D nor = (*iter).second.normalA.Normalize();
			float depth = (*iter).second.normalA.Size();
			Vector3D pos = (*iter).second.position;

#pragma region 겹쳐진 만큼 밀어내기
			// 켭쳐진 만큼 밀어낸다.
			if (rigidbodyA->GetKinematic() && rigidbodyB->GetKinematic())
			{
				// 둘 다 kinematic인 경우 그냥 둔다.
			}
			else if (rigidbodyA->GetKinematic())
			{
				rigidbodyB->Move((nor * (depth + COLLISION_EPSILON)));
			}
			else if (rigidbodyB->GetKinematic())
			{
				rigidbodyA->Move(-(nor * (depth + COLLISION_EPSILON)));
			}
			else
			{
				rigidbodyA->Move(-(nor * (depth + (COLLISION_EPSILON) / 2.0f)));
				rigidbodyB->Move((nor * (depth + (COLLISION_EPSILON) / 2.0f)));
			}

			if (EngineSetting::GetInstance().GetPhysicsOverlapMode() == EngineSetting::PhysicsOverlapMode::Depth)
			{
				// depth가 INV_VELOCITY_DEPTH_MIN보다 깊은 경우 속도도 적용한다.
				if (depth > INV_VELOCITY_DEPTH_MIN)
				{
					float aSpeed = DotProduct(nor, rigidbodyA->GetVelocity());
					float bSpeed = DotProduct(-nor, rigidbodyB->GetVelocity());

					if (aSpeed > 0.0f)
					{
						rigidbodyA->ApplyAcceledration(-nor * min(INV_VELOCITY_SCALE, aSpeed));
					}
					
					if (bSpeed > 0.0f)
					{
						rigidbodyB->ApplyAcceledration(nor * min(INV_VELOCITY_SCALE, bSpeed));
					}
				}
			}
			else if (EngineSetting::GetInstance().GetPhysicsOverlapMode() == EngineSetting::PhysicsOverlapMode::Velocity)
			{
				if (depth > INV_VELOCITY_RATE_DEPTH_MIN)
				{
					float rate = max(min(1.0f, INV_VELOCITY_MIN_RATE), 0.0f);

					float vRate = 1.0f;
					if (INV_VELOCITY_RATE_DEPTH_MAX - INV_VELOCITY_RATE_DEPTH_MIN > 0.0f)
					{
						 vRate = INV_VELOCITY_MIN_RATE + (1.0f - INV_VELOCITY_MIN_RATE) * (min(depth, INV_VELOCITY_RATE_DEPTH_MAX) - INV_VELOCITY_RATE_DEPTH_MIN) / (INV_VELOCITY_RATE_DEPTH_MAX - INV_VELOCITY_RATE_DEPTH_MIN);
					}
					
					rigidbodyA->ApplyAcceledration(-rigidbodyA->GetVelocity() * vRate);
					rigidbodyB->ApplyAcceledration(-rigidbodyB->GetVelocity() * vRate);
				}
			}
			else
			{
				assert(0);
			}

#pragma endregion


#pragma region 충돌방향의 운동량 교환
			Vector3D AP = pos - rigidbodyA->GetCenterOfMass();
			Vector3D BP = pos - rigidbodyB->GetCenterOfMass();

			// A, B의 속도
			Vector3D v1 = rigidbodyA->GetVelocity() + CrossProduct(rigidbodyA->GetAngularVelocity(), AP);
			Vector3D v2 = rigidbodyB->GetVelocity() + CrossProduct(rigidbodyB->GetAngularVelocity(), BP);
			// 상대속도
			Vector3D relativeVelocity = v2 - v1;

			// 노말방향 속도
			float norV = DotProduct(relativeVelocity, nor);

			if (norV < 0.0f)
			{
				float massA = rigidbodyA->GetTotalMass();
				float massB = rigidbodyB->GetTotalMass();

				float invMassA = massA != 0.0f ? (1.0f / massA) : 0.0f;
				float invMassB = massB != 0.0f ? (1.0f / massB) : 0.0f;

				Vector3D inertiaA = CrossProduct(rigidbodyA->GetInverseInertiaTensor() * CrossProduct(AP, nor), AP);
				Vector3D inertiaB = CrossProduct(rigidbodyB->GetInverseInertiaTensor() * CrossProduct(BP, nor), BP);
				float numerator = invMassA + invMassB + DotProduct(inertiaA + inertiaB, nor);

				float force = -(1.0f + min(rigidbodyA->GetRestitution(), rigidbodyB->GetRestitution())) * norV;
				force /= numerator;
				force /= collisionCounter;

				Vector3D impulse = nor * force;
				rigidbodyA->ApplyForce(pos, -impulse);
				rigidbodyB->ApplyForce(pos, impulse);


#pragma region 마찰력 적용
				// 노말과 수직 방향의 상대속도
				Vector3D tangent = relativeVelocity - (nor * norV);

				// 상대 속도가 0이 아닌경우에만 적용한다.
				if (tangent != Vector3D::zero)
				{
					Vector3D tanNor = tangent.Normalize();

					float tanForce = -DotProduct(relativeVelocity, tanNor);
					tanForce /= numerator;
					tanForce /= collisionCounter;

					Vector3D frictionImpulse;

					// 정지 마찰
					if (Abs(tanForce) < force * ComputeFriction(rigidbodyA->GetSfriction(), rigidbodyB->GetSfriction()))
					{
						frictionImpulse = tanNor * tanForce;
					}
					// 운동 마찰
					else
					{
						frictionImpulse = tanNor * (-force) * ComputeFriction(rigidbodyA->GetKfriction(), rigidbodyB->GetKfriction());
					}

					rigidbodyA->ApplyForce(pos, -frictionImpulse);
					rigidbodyB->ApplyForce(pos, frictionImpulse);
				}
#pragma endregion
			}
#pragma endregion
		}

		// 중력 적용
		for (auto iter = rigidbodyList.begin(); iter != rigidbodyList.end(); iter++)
		{
			if ((*iter)->GetUseGravity())
			{
				(*iter)->ApplyAcceledration((*iter)->GetGravity() * deltaTime);
			}
		}


		// 가속도를 속도로 변환, 각가속도를 각속도로 변환
		for (auto iter = rigidbodyList.begin(); iter != rigidbodyList.end(); iter++)
		{
			(*iter)->LinearAccelerate();
			(*iter)->AngularAccelerate();
		}

		// 속도에 따른 이동
		for (auto iter = rigidbodyList.begin(); iter != rigidbodyList.end(); iter++)
		{
			(*iter)->Move((*iter)->GetVelocity()* deltaTime);
		}

		// 각속도에 따른 회전
		// 현재는 오브젝트의 중심을 지나는 축을 기준으로 회전한다...
		// 울트라핸드가 가능하려면 중심을 지나지 않는 축으로 회전이 가능해야 한다.
		for (auto iter = rigidbodyList.begin(); iter != rigidbodyList.end(); iter++)
		{
			Vector3D axis = (*iter)->angularVelocity.Normalize();
			float angle = (*iter)->angularVelocity.Size();
			
			(*iter)->GetGameObject()->GetTransform()->Rotate((*iter)->GetCenterOfMass(), axis, angle * deltaTime);
		}

		// 저항 적용
		for (auto iter = rigidbodyList.begin(); iter != rigidbodyList.end(); iter++)
		{
			(*iter)->SetVelocity((*iter)->GetVelocity() * (powf(1.0f - (*iter)->GetLinearDrag(), deltaTime)));
			(*iter)->SetAngularVelocity((*iter)->GetAngularVelocity() * (powf(1.0f - (*iter)->GetAngularDrag(), deltaTime)));
		}
	}

	void PhysicsSystem::CollisionEvent()
	{
		// collisionResult와 lastCollisionResult로 enter, stay, exit를 구분
		std::set<std::pair<Collider*, Collider*>> enterList;
		std::set<std::pair<Collider*, Collider*>> stayList;
		std::set<std::pair<Collider*, Collider*>> exitList;

		std::set<Collider*> singleEnterList;
		std::set<Collider*> singleStayList;
		std::set<Collider*> singleExitList;

		for (auto iter = collisionResult->begin(); iter != collisionResult->end(); iter++)
		{
			if (lastCollisionResult->count(*iter) == 0)
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

		for (auto iter = lastCollisionResult->begin(); iter != lastCollisionResult->end(); iter++)
		{
			if (collisionResult->count(*iter) == 0)
			{
				exitList.insert(*iter);
				singleExitList.insert(iter->first);
				singleExitList.insert(iter->second);
			}
		}

		// 상대 콜라이더 정보를 받지 않는 함수
		// OnTriggerEnter
		for (auto iter = singleEnterList.begin(); iter != singleEnterList.end(); iter++)
		{
			std::vector<Component*> components = (*iter)->GetGameObject()->GetComponents();
			
			if ((*iter)->IsTrigger())
			{
				for (size_t i = 0; i < components.size(); i++)
				{
					components[i]->OnTriggerEnter();
				}
			}
			else
			{
				for (size_t i = 0; i < components.size(); i++)
				{
					components[i]->OnCollisionEnter();
				}
			}
		}

		// OnTriggerStay
		for (auto iter = singleStayList.begin(); iter != singleStayList.end(); iter++)
		{
			std::vector<Component*> components = (*iter)->GetGameObject()->GetComponents();

			if ((*iter)->IsTrigger())
			{
				for (size_t i = 0; i < components.size(); i++)
				{
					components[i]->OnTriggerStay();
				}
			}
			else
			{
				for (size_t i = 0; i < components.size(); i++)
				{
					components[i]->OnCollisionStay();
				}
			}
		}

		// OnTriggerExit
		for (auto iter = singleExitList.begin(); iter != singleExitList.end(); iter++)
		{
			std::vector<Component*> components = (*iter)->GetGameObject()->GetComponents();

			if ((*iter)->IsTrigger())
			{
				for (size_t i = 0; i < components.size(); i++)
				{
					components[i]->OnTriggerExit();
				}
			}
			else
			{
				for (size_t i = 0; i < components.size(); i++)
				{
					components[i]->OnCollisionExit();
				}
			}
		}


		// 상대 콜라이더 정보를 받는 함수
		// OnTriggerEnter
		for (auto iter = enterList.begin(); iter != enterList.end(); iter++)
		{
			assert((*iter).first->IsTrigger() == (*iter).second->IsTrigger());

			std::vector<Component*> firstComponents = (*iter).first->GetGameObject()->GetComponents();
			std::vector<Component*> secondComponents = (*iter).second->GetGameObject()->GetComponents();

			if ((*iter).first->IsTrigger() == true)
			{
				for (size_t i = 0; i < firstComponents.size(); i++)
				{
					firstComponents[i]->OnTriggerEnter((*iter).second);
				}

				for (size_t i = 0; i < secondComponents.size(); i++)
				{
					secondComponents[i]->OnTriggerEnter((*iter).first);
				}
			}
			else
			{
				for (size_t i = 0; i < firstComponents.size(); i++)
				{
					firstComponents[i]->OnCollisionEnter((*iter).second);
				}

				for (size_t i = 0; i < secondComponents.size(); i++)
				{
					secondComponents[i]->OnCollisionEnter((*iter).first);
				}
			}
		}

		// OnTriggerStay
		for (auto iter = stayList.begin(); iter != stayList.end(); iter++)
		{
			assert((*iter).first->IsTrigger() == (*iter).second->IsTrigger());

			std::vector<Component*> firstComponents = (*iter).first->GetGameObject()->GetComponents();
			std::vector<Component*> secondComponents = (*iter).second->GetGameObject()->GetComponents();

			if ((*iter).first->IsTrigger() == true)
			{
				for (size_t i = 0; i < firstComponents.size(); i++)
				{
					firstComponents[i]->OnTriggerStay((*iter).second);
				}

				for (size_t i = 0; i < secondComponents.size(); i++)
				{
					secondComponents[i]->OnTriggerStay((*iter).first);
				}
			}
			else
			{
				for (size_t i = 0; i < firstComponents.size(); i++)
				{
					firstComponents[i]->OnCollisionStay((*iter).second);
				}

				for (size_t i = 0; i < secondComponents.size(); i++)
				{
					secondComponents[i]->OnCollisionStay((*iter).first);
				}
			}
		}

		// OnTriggerExit
		for (auto iter = exitList.begin(); iter != exitList.end(); iter++)
		{
			assert((*iter).first->IsTrigger() == (*iter).second->IsTrigger());

			std::vector<Component*> firstComponents = (*iter).first->GetGameObject()->GetComponents();
			std::vector<Component*> secondComponents = (*iter).second->GetGameObject()->GetComponents();

			if ((*iter).first->IsTrigger() == true)
			{
				for (size_t i = 0; i < firstComponents.size(); i++)
				{
					firstComponents[i]->OnTriggerExit((*iter).second);
				}

				for (size_t i = 0; i < secondComponents.size(); i++)
				{
					secondComponents[i]->OnTriggerExit((*iter).first);
				}
			}
			else
			{
				for (size_t i = 0; i < firstComponents.size(); i++)
				{
					firstComponents[i]->OnCollisionExit((*iter).second);
				}

				for (size_t i = 0; i < secondComponents.size(); i++)
				{
					secondComponents[i]->OnCollisionExit((*iter).first);
				}
			}
		}
	}

	void PhysicsSystem::AddCollider(Collider* collider)
	{
		isUpdatedHierarchy = false;

		colliderList.push_back(collider);
		colliderTree->Insert(collider->GetCenter(), collider->GetMaxRadius(), collider);
	}

	void PhysicsSystem::RemoveCollider(Collider* collider)
	{
		isUpdatedHierarchy = false;

		colliderList.erase(remove(colliderList.begin(), colliderList.end(), collider), colliderList.end());

		// 콜라이더를 제거할때는 이미 shape, transform등이 이미 해제되었을 수 있기 때문에 위치정보없이 제거한다.
		colliderTree->Remove(collider);
	}

	void PhysicsSystem::AddRigidbody(Rigidbody* rigidbody)
	{
		isUpdatedHierarchy = false;

		allRigidbodyList.push_back(rigidbody);
	}

	void PhysicsSystem::RemoveRigidbody(Rigidbody* rigidbody)
	{
		isUpdatedHierarchy = false;

		allRigidbodyList.erase(remove(allRigidbodyList.begin(), allRigidbodyList.end(), rigidbody), allRigidbodyList.end());
	}

	void PhysicsSystem::RenderDebuggingInfo(IGraphics* graphics)
	{
		static std::wstring lineName;
		static std::wstring sphereName;
		lineName = ResourceManager::GetInstance().GetMaterial(L"BoundaryDebug_Line")->materialName;
		sphereName = ResourceManager::GetInstance().GetMaterial(L"BoundaryDebug")->materialName;
		
		for (size_t i = 0; i < colliderList.size(); i++)
		{
			if (colliderList[i]->GetGameObject()->GetRootActive() == true)
			{
				colliderList[i]->Render(graphics);
			}
		}

		graphics->DrawMesh(Matrix4x4::Scale(Vector3D(3, 3, 3)), L"Sphere", *ResourceManager::GetInstance().GetMaterial(L"BoundaryDebug"), sphereName,0);

		for (auto iter = collisionInfoResult.begin(); iter != collisionInfoResult.end(); iter++)
		{
			Vector3D normal = (*iter).second.normalA;
			Vector3D position = (*iter).second.position;

			//OutputDebugString((L"Pos: " + std::to_wstring(position.x) + L", " + std::to_wstring(position.y) + L", " + std::to_wstring(position.z) + L"   ").c_str());
			//OutputDebugString((L"Nor: " + std::to_wstring(normal.x) + L", " + std::to_wstring(normal.y) + L", " + std::to_wstring(normal.z) + L"\n").c_str());

			Vector3D originLine = Vector3D(1, 0, 0);

			Vector3D axis = CrossProduct(originLine, normal).Normalize();
			float angle = std::acos(DotProduct(originLine, normal.Normalize()));

			Matrix4x4 matrix = Matrix4x4::Translation(position) * Matrix4x4::Rotation(axis, angle) * Matrix4x4::Scale(normal.Size());

			graphics->DrawMesh(matrix, L"Line", *ResourceManager::GetInstance().GetMaterial(L"BoundaryDebug_Line"), lineName,0);
			graphics->DrawMesh(Matrix4x4::Translation(position), L"Sphere", *ResourceManager::GetInstance().GetMaterial(L"BoundaryDebug"), sphereName,0);
		}


		if (EPASimplex.size() == 0) return;

		static int drawTarget = 0;
		if (Input::GetInstance().PressedKey(VK_CONTROL) && Input::GetInstance().PressedKeyNow('Z') && EngineSetting::GetInstance().GetShowDebugInfo() && drawTarget > 0) drawTarget--;
		if (Input::GetInstance().PressedKey(VK_CONTROL) && Input::GetInstance().PressedKeyNow('X') && EngineSetting::GetInstance().GetShowDebugInfo()) drawTarget++;
		if (Input::GetInstance().PressedKey(VK_CONTROL) && Input::GetInstance().PressedKeyNow('C') && EngineSetting::GetInstance().GetShowDebugInfo()) drawTarget = 0;

		static int drawCount;
		drawCount = 0;

		// 그래픽스 메모리 최대치로 인해... 디버깅 정보 제한
		const int debugMaxCount = 8;

		for (int i = 0; i < EPASimplex.size(); i++)
		{
			if (drawCount == min(drawTarget, min(EPASimplex.size() - 1, debugMaxCount)))
			{
				while (!EPASimplex[drawCount].indices.empty())
				{
					Face f = (*EPASimplex[drawCount].indices.begin()).second;
					EPASimplex[drawCount].indices.erase(EPASimplex[drawCount].indices.begin());
					Vector3D a = EPASimplex[drawCount].vertices[f.v[0]];
					Vector3D b = EPASimplex[drawCount].vertices[f.v[1]];
					Vector3D c = EPASimplex[drawCount].vertices[f.v[2]];

					Vector3D ab = (b - a);
					Vector3D bc = (c - b);
					Vector3D ca = (a - c);

					Matrix4x4 abmatrix = Matrix4x4::Translation(a) * Matrix4x4::Rotation(CrossProduct(Vector3D(1, 0, 0), ab.Normalize()).Normalize(), std::acos(DotProduct(Vector3D(1, 0, 0), ab.Normalize()))) * Matrix4x4::Scale(ab.Size());
					Matrix4x4 bcmatrix = Matrix4x4::Translation(b) * Matrix4x4::Rotation(CrossProduct(Vector3D(1, 0, 0), bc.Normalize()).Normalize(), std::acos(DotProduct(Vector3D(1, 0, 0), bc.Normalize()))) * Matrix4x4::Scale(bc.Size());
					Matrix4x4 camatrix = Matrix4x4::Translation(c) * Matrix4x4::Rotation(CrossProduct(Vector3D(1, 0, 0), ca.Normalize()).Normalize(), std::acos(DotProduct(Vector3D(1, 0, 0), ca.Normalize()))) * Matrix4x4::Scale(ca.Size());

					lineName = ResourceManager::GetInstance().GetMaterial(L"BoundaryDebug_Line")->materialName;
					sphereName = ResourceManager::GetInstance().GetMaterial(L"BoundaryDebug")->materialName;

					graphics->DrawMesh(abmatrix, L"Line", *ResourceManager::GetInstance().GetMaterial(L"BoundaryDebug_Line"), lineName,0);
					graphics->DrawMesh(bcmatrix, L"Line", *ResourceManager::GetInstance().GetMaterial(L"BoundaryDebug_Line"), lineName, 0);
					graphics->DrawMesh(camatrix, L"Line", *ResourceManager::GetInstance().GetMaterial(L"BoundaryDebug_Line"), lineName, 0);
					graphics->DrawMesh(Matrix4x4::Translation(a), L"Sphere", *ResourceManager::GetInstance().GetMaterial(L"BoundaryDebug"), sphereName, 0);
					graphics->DrawMesh(Matrix4x4::Translation(b), L"Sphere", *ResourceManager::GetInstance().GetMaterial(L"BoundaryDebug"), sphereName, 0);
					graphics->DrawMesh(Matrix4x4::Translation(c), L"Sphere", *ResourceManager::GetInstance().GetMaterial(L"BoundaryDebug"), sphereName, 0);
				}
			}
			drawCount++;
		}
	}

	void PhysicsSystem::UpdateHierarchy()
	{
		isUpdatedHierarchy = true;

		// 기존의 정보를 전부 초기화한다.
		rigidbodyList.clear();

		for (size_t i = 0; i < allRigidbodyList.size(); i++)
		{
			allRigidbodyList[i]->SetParent(nullptr);
			allRigidbodyList[i]->RemoveAllChild();
			allRigidbodyList[i]->RemoveAllCollider();
		}

		for (size_t i = 0; i < colliderList.size(); i++)
		{
			colliderList[i]->SetMaster(nullptr);
		}

		// rigidbodyList를 만든다.
		for (size_t i = 0; i < allRigidbodyList.size(); i++)
		{
			Rigidbody* parent = allRigidbodyList[i]->FindParentRigidbody();
			allRigidbodyList[i]->SetParent(parent);
			if (parent == nullptr)
			{
				rigidbodyList.push_back(allRigidbodyList[i]);
			}
			else
			{
				parent->AddChild(allRigidbodyList[i]);
			}
		}

		// collider 추가
		for (size_t i = 0; i < colliderList.size(); i++)
		{
			Rigidbody* parent = colliderList[i]->FindMasterRigidbody();
			colliderList[i]->SetMaster(parent);
			if (parent != nullptr)
			{
				parent->AddCollider(colliderList[i]);
			}
		}
	}

	bool PhysicsSystem::IsInnerPointInsideConvexHull(const ZeldaMath::Vector3D& point, const std::vector<ZeldaMath::Vector3D>& convexHull)
	{
		Vector3D nor = Vector3D::zero;

		for (size_t i = 0; i < convexHull.size(); i++)
		{
			size_t j = (i + 1) % convexHull.size();

			Vector3D cross = CrossProduct(convexHull[j] - convexHull[i], point - convexHull[j]).Normalize();

			if (nor == Vector3D::zero)
			{
				nor = cross;
			}
			else
			{
				if (DotProduct(nor, cross) < 0.0f)
				{
					return false;
				}
			}
		}

		return true;
	}

	std::vector<ZeldaMath::Vector3D> PhysicsSystem::Make2DConvexHull(const std::vector<ZeldaMath::Vector3D>& vertices2D)
	{

		std::vector<Vector3D> result;

		assert(vertices2D.size() > 2);
		if (vertices2D.size() <= 2) return result;

		// 평면위의 임의의 벡터
		Vector3D dirVec = (vertices2D[0] - vertices2D[1]).Normalize();
		// 평면에 수직인 임의의 벡터
		Vector3D nor = CrossProduct((vertices2D[0] - vertices2D[1]).Normalize(), (vertices2D[0] - vertices2D[2]).Normalize()).Normalize();

		if (nor == Vector3D::zero)
		{
			for (size_t i = 0; i < vertices2D.size(); i++)
			{
				for (size_t j = 0; j < vertices2D.size(); j++)
				{
					for (size_t k = 0; k < vertices2D.size(); k++)
					{
						if (nor == Vector3D::zero)
						{
							nor = CrossProduct((vertices2D[i] - vertices2D[j]).Normalize(), (vertices2D[i] - vertices2D[k]).Normalize()).Normalize();
						}
						else
						{
							i = vertices2D.size();
							j = vertices2D.size();
							k = vertices2D.size();
						}
					}
				}
			}
		}

		if (nor == Vector3D::zero && vertices2D.size() != 0)
		{
			Vector3D everagePos = Vector3D::zero;
			for (size_t i = 0; i < vertices2D.size(); i++)
			{
				everagePos += vertices2D[i];
			}

			everagePos /= vertices2D.size();
			return { everagePos };
		}

		assert(nor != Vector3D::zero);

		float maxLength = DotProduct(dirVec, vertices2D[0]);
		size_t maxIdx = 0;

		// dirVec 방향으로 가장 먼 점을 찾는다.
		for (size_t i = 1; i < vertices2D.size(); i++)
		{
			float length = DotProduct(dirVec, vertices2D[i]);
			if (maxLength < length)
			{
				maxLength = length;
				maxIdx = i;
			}
		}

		// 가장 먼 점을 제외한 점
		std::vector<Vector3D> v;
		// 가장 먼 점
		Vector3D startP = vertices2D[maxIdx];

		for (size_t i = 0; i < vertices2D.size(); i++)
		{
			if (i != maxIdx)
			{
				v.push_back(vertices2D[i]);
			}
		}

		// dirVec, nor와 수직인 벡터 2D그림상에서 dirVec를 오른쪽 nor를 앞쪽(보는사람 쪽으로 나오는 방향)이라고 하면 basicVec는 윗방향이다.
		Vector3D basicVec = CrossProduct(dirVec, nor).Normalize();

		std::vector<std::pair<float, size_t>> sortedData;
		for (size_t i = 0; i < v.size(); i++)
		{
			// first로 사이각의 cos값을 넣음, 즉 정렬시 basicVec와 사잇각이 큰 순서대로 정렬됨
			sortedData.push_back({ DotProduct((v[i] - startP).Normalize(), basicVec), i });
		}

		sort(sortedData.begin(), sortedData.end());

		// 스택의 윗쪽을 기준으로 보면 아랫방향의 벡터를 기준으로 시계방향으로 정렬되어있음
		std::stack<Vector3D> st;

		for (size_t i = 0; i < sortedData.size(); i++)
		{
			st.push(v[sortedData[i].second]);
		}

		result.push_back(startP);

		while (!st.empty())
		{
			Vector3D now = st.top();
			st.pop();

			while (true)
			{
				if (result.size() < 2)
				{
					result.push_back(now);
					break;
				}
				else
				{
					Vector3D v0 = result[result.size() - 2];
					Vector3D v1 = result[result.size() - 1];

					// 시계방향임
					if (DotProduct(CrossProduct(v1 - v0, now - v1).Normalize(), nor) <= 0.0f)
					{
						result.push_back(now);
						break;
					}
					// 반시계방향임
					else
					{
						result.pop_back();
						continue;
					}
				}
			}
		}

		return result;
	}

	ZeldaMath::Vector3D PhysicsSystem::FindCollisionPosition(const std::vector<ZeldaMath::Vector3D>& verticesA, const std::vector<ZeldaMath::Vector3D>& verticesB, ZeldaMath::Vector3D nor)
	{
		// FindCollisionPosition에 들어온 verticesA와 verticesB는 매우 적은 점의 갯수를 가지고 있다고 생각해도 될 듯
		// 일단 vertices의 수가 많을 때의 효율은 신경쓰지 않고 구현하겠음

		// 방향벡터는 노말라이즈 해둔다.
		nor = nor.Normalize();

		// nor에 수직이며 원점을 지나는 평면에 정사영 (임의의 점을 지나는 평면에 하면 되는데 원점을 선택함)
		
		// 점 충돌은 이미 거르고 이 함수를 호출했다고 가정함
		assert(verticesA.size() != 1 && verticesB.size() != 1);

		// 선분과 선분의 충돌 
		if (verticesA.size() < 3 && verticesB.size() < 3)
		{
			if (verticesA[0] == verticesA[1])
			{
				return verticesA[0];
			}
			else if (verticesB[0] == verticesB[1])
			{
				return verticesB[0];
			}
			else
			{
				Vector3D intersectPoint = Vector3D::zero;
				bool isIntersect = IntersectionOfProjetedLineSegment(verticesA[0], verticesA[1], verticesB[0], verticesB[1], nor, &intersectPoint);

				if (!isIntersect)
				{
					intersectPoint = (verticesA[0] + verticesA[1] + verticesB[0] + verticesB[1]) / 4.0f;
				}

				return intersectPoint;
			}
		}
		// 선분과 면의 충돌
		else if (verticesA.size() < 3 || verticesB.size() < 3)
		{
			const std::vector<Vector3D>& line = (verticesA.size() < 3) ? (verticesA) : (verticesB);
			const std::vector<Vector3D>& plane = (verticesA.size() < 3) ? (verticesB) : (verticesA);

			std::vector<Vector3D> convexhullplane = Make2DConvexHull(plane);
			if (convexhullplane.size() == 1)
			{
				return convexhullplane[0];
			}

			bool inner0 = IsInnerPointInsideConvexHull(line[0], convexhullplane);
			bool inner1 = IsInnerPointInsideConvexHull(line[1], convexhullplane);

			if (inner0 && inner1)
			{
				return (line[0] + line[1]) / 2.0f;
			}
			else if (inner0)
			{
				for (size_t i = 0; i < convexhullplane.size(); i++)
				{
					size_t j = (i + 1) % convexhullplane.size();

					// 선분 교차함
					Vector3D intersectPoint = Vector3D::zero;
					bool isIntersect = IntersectionOfProjetedLineSegment(line[0], line[1], convexhullplane[i], convexhullplane[j], nor, &intersectPoint);

					if (isIntersect)
					{
						return (intersectPoint + line[0]) / 2.0f;
					}
				}

				return (line[0] + line[1]) / 2.0f;
			}
			else if (inner1)
			{
				for (size_t i = 0; i < convexhullplane.size(); i++)
				{
					size_t j = (i + 1) % convexhullplane.size();

					// 선분 교차함
					Vector3D intersectPoint = Vector3D::zero;
					bool isIntersect = IntersectionOfProjetedLineSegment(line[0], line[1], convexhullplane[i], convexhullplane[j], nor, &intersectPoint);

					if (isIntersect)
					{
						return (intersectPoint + line[1]) / 2.0f;
						
					}
				}

				return (line[0] + line[1]) / 2.0f;
			}
			else
			{
				Vector3D averagePos = Vector3D::zero;
				int cnt = 0;

				// 정상적인 경우라면 교점 2개가 나올것임 (면의 한 모서리와 일치한다면 안나올 수도 있긴함, 극히 드문경우)
				// 일단은 교차하면 전부 평균낸다.
				for (size_t i = 0; i < convexhullplane.size(); i++)
				{
					size_t j = (i + 1) % convexhullplane.size();

					Vector3D intersectPoint = Vector3D::zero;
					bool isIntersect = IntersectionOfProjetedLineSegment(line[0], line[1], convexhullplane[i], convexhullplane[j], nor, &intersectPoint);

					if (isIntersect)
					{
						averagePos += intersectPoint;
						cnt++;
					}
				}

				if (cnt != 0)
				{
					averagePos /= cnt;
				}
				else
				{
					averagePos = (line[0] + line[1]) / 2.0f;
				}

				return averagePos;
			}

			// 이게 나오면 안되는데... 생각보다 자주 나온다
			// 선분이 하나는 안에 하나는 밖에 있는 경우인데 선분과 면의 교차점을 찾지 못할 때
			// assert(0); 
			return (line[0] + line[1]) / 2.0f;
		}
		// A의 면 B의 면 충돌
		else
		{
			// 서로의 내부에 있는 모든 점 + 교차점의 평균
			Vector3D averagePos = Vector3D::zero;
			int cnt = 0;

			std::vector<Vector3D> planeA = Make2DConvexHull(verticesA);
			std::vector<Vector3D> planeB = Make2DConvexHull(verticesB);

			if (planeA.size() == 1) return planeA[0];
			if (planeB.size() == 1) return planeB[0];
			
			// 교차점 넣기
			for (size_t i = 0; i < planeA.size(); i++)
			{
				Vector3D A0 = planeA[i];
				Vector3D A1 = planeA[(i + 1) % planeA.size()];

				for (size_t j = 0; j < planeB.size(); j++)
				{
					Vector3D B0 = planeB[j];
					Vector3D B1 = planeB[(j + 1) % planeB.size()];

					Vector3D intersectPoint = Vector3D::zero;
					bool isIntersect = IntersectionOfProjetedLineSegment(A0, A1, B0, B1, nor, &intersectPoint);

					if (isIntersect)
					{
						averagePos += intersectPoint;
						cnt++;
					}
					// 평행한 경우 안쪽의 두점을 넣음
					else if (1.0f - Abs(DotProduct((A0 - A1).Normalize(), (B0 - B1).Normalize())) < 1e-6f)
					{
						// 일단 모든 점을 더하고
						averagePos += (A0 + A1 + B0 + B1);
						// 양방향으로 가장 먼 점을 뺀다.
						float dista0 = DotProduct((A1 - A0).Normalize(), A0 - A0);
						float dista1 = DotProduct((A1 - A0).Normalize(), A1 - A0);
						float distb0 = DotProduct((A1 - A0).Normalize(), B0 - A0);
						float distb1 = DotProduct((A1 - A0).Normalize(), B1 - A0);

						averagePos -= (A0 + (A0 - A1).Normalize() * min(dista0, min(dista1, min(distb0, distb1))));
						averagePos -= (A0 + (A0 - A1).Normalize() * max(dista0, max(dista1, max(distb0, distb1))));

						cnt += 2;
					}
				}
			}

			// 내부점 넣기
			for (size_t i = 0; i < planeA.size(); i++)
			{
				if (IsInnerPointInsideConvexHull(planeA[i], planeB))
				{
					averagePos += planeA[i];
					cnt++;
				}
			}

			for (size_t i = 0; i < planeB.size(); i++)
			{
				if (IsInnerPointInsideConvexHull(planeB[i], planeA))
				{
					averagePos += planeB[i];
					cnt++;
				}
			}

			//assert(cnt != 0);
			if (cnt == 0)
			{
				averagePos = Vector3D::zero;
			}
			else
			{
				averagePos /= cnt;
			}
			
			return averagePos;
		}
	}

	bool PhysicsSystem::CollisionDetectionByGJK(Collider* colliderA, Collider* colliderB)
	{
		size_t loopCount = 0;

		Simplex simplex;

		Vector3D direction = Vector3D::right;
		Vector3D supportPoint = FindSupportPoint(colliderA, colliderB, direction);

		simplex.Push(supportPoint);

		direction = -supportPoint.Normalize();

		// 디버깅을 위해 임시로 무한루프를 돌게 한다...
		while (loopCount < GJK_MAXLOOP)
		{
			direction = direction.Normalize();

			supportPoint = FindSupportPoint(colliderA, colliderB, direction);
			Vector3D OP = supportPoint.Normalize();

			// supportPoint와 direction의 방향이 90도 이상 차이가 난다면 충돌이 불가능 하다.
			if (DotProduct(OP, direction) < 0.0f)
			{
				return false;
			}

			// Simplex에 SupportPoint를 추가하고 Simplex와 원점의 위치 판정을 진행한다.
			simplex.Push(supportPoint);

			switch (DoSimplex(simplex, direction))
			{
			case CollisionOccurred:
				// 여기서 EPA에서 필요한 Simplex의 정보를 저장해둔다.
				collisionSimplexResult.insert({ { colliderA, colliderB }, simplex });
				return true;
				break;

			case CollisionPossible:
				// DoSimplex에서 다음에 조사할 방향까지 정해주기 때문에 아무것도 할 필요가 없다.
				break;

			case CollisionImpossible:
				return false;
				break;

			default:
				assert(0);
				return false;
				break;
			}

			loopCount++;
		}

		return false;
	}

	PhysicsSystem::CollisionInfo PhysicsSystem::CollisionDetectionByEPA(Collider* colliderA, Collider* colliderB, const Simplex& baseSimplex)
	{
		size_t loopCount = 0;

		Simplex simplex = baseSimplex;

		// 사면체가 되기위해 부족한 정점을 추가한다.
		if (simplex.Size() == 2)
		{
			Vector3D A = simplex[0];
			Vector3D B = simplex[1];
			Vector3D BANormal = (A - B).Normalize();

			Vector3D dirvec[4];
			dirvec[0] = Vector3D(BANormal.y - BANormal.z, BANormal.z - BANormal.x, BANormal.x - BANormal.y).Normalize();
			dirvec[1] = -dirvec[0];
			dirvec[2] = CrossProduct(BANormal, dirvec[0]);
			dirvec[3] = -dirvec[2];

			float maxLength = -1.0f;
			int maxIdx = -1;

			Vector3D support[4];

			for (int i = 0; i < 4; i++)
			{
				support[i] = FindSupportPoint(colliderA, colliderB, dirvec[i]);
				float length = DotProduct(dirvec[i], support[i] - B);

				if (length > maxLength)
				{
					maxLength = length;
					maxIdx = i;
				}
			}

			assert(maxIdx != -1);

			simplex.Push(support[maxIdx]);
		}

		// 사면체가 되기위해 부족한 정점을 추가한다.
		if (simplex.Size() == 3)
		{
			Vector3D A = simplex[0];
			Vector3D B = simplex[1];
			Vector3D C = simplex[2];

			Vector3D CANormal = (A - C).Normalize();
			Vector3D CBNormal = (B - C).Normalize();

			Vector3D CABNormal = CrossProduct(CANormal, CBNormal).Normalize();

			Vector3D support0 = FindSupportPoint(colliderA, colliderB, CABNormal);
			Vector3D support1 = FindSupportPoint(colliderA, colliderB, -CABNormal);

			float length0 = DotProduct(CABNormal, support0 - C);
			float length1 = DotProduct(-CABNormal, support1 - C);
			
			if (length0 > length1)
			{
				simplex.Push(support0);
			}
			else
			{
				simplex.Push(support1);
			}
		}

		// 사면체 모양을 잡는다. 내부적으로 반지름 길이를 구하고 정렬을 해둔다.
		bool resultTetrahedron = simplex.CreateTetrahedron();
		if (resultTetrahedron == false)
		{
			CollisionInfo result;
			result.colliderA = colliderA;
			result.colliderB = colliderB;
			result.position = Vector3D::zero;
			result.normalA = Vector3D::zero;

			return result;
		}

		EPASimplex.clear();

		while (true)
		{
			EPASimplex.push_back(simplex);

			// simplex의 확장
			bool expandResult = simplex.ExpandFace(colliderA, colliderB);

			// 확장에 성공하고 EPA_MAXLOOP를 초과하지 않은 경우
			if (expandResult && loopCount < EPA_MAXLOOP)
			{
				// 확장된 simplex에 추가된 면의 정보를 queue에 추가한다.
				// ExpandFace() 함수에서 알아서 추가됨, 각 면을 가까운 순서대로 정렬한다. 이미 정렬되어 있는 데이터가 있으므로 추가된 면만 이진탐색하여 빠르게 삽입한다.

			}
			// 확장에 실패했다면
			else
			{
				// 확장에 실패한 그 면과 원점 사이의 벡터가 충돌깊이와 충돌방향을 나타낸다.
				float collisionDepth = 0.0f;
				Vector3D collisionPosition = Vector3D::zero;
				Vector3D collisionDirection = simplex.GetNearestFaceDirection();

				// colliderA의 탈출 방향은 -collisionDirection
				// colliderB의 탈출 방향은 +collisionDirection


				if (collisionDirection != Vector3D::zero)
				{
					// 충돌깊이 계산
					collisionDepth = simplex.GetNearestFaceDistance();

					// 충돌위치 계산

					// A의 충돌방향으로 가장 멀리있는 점과 그 점과 Epsilon 크기만큼 차이나는 vertex를 모두 얻어온다.
					std::vector<Vector3D> collisionVerticesA = colliderA->GetFarthestPoint(collisionDirection, EPA_VERTEX_EPSILON);

					// B의 충돌방향으로 가장 멀리있는 점과 그 점과 Epsilon 크기만큼 차이나는 vertex를 모두 얻어온다.
					std::vector<Vector3D> collisionVerticesB = colliderB->GetFarthestPoint(-collisionDirection, EPA_VERTEX_EPSILON);


					// 그 충돌 방향벡터를 이용해서 colliderA와 colliderB가 점 충돌인지, 면 충돌인지 판단한다.
					// 점 충돌한 콜라이더가 존재한다면
					if (collisionVerticesA.size() == 1)
					{
						// 그 지점을 충돌위치로 한다.
						collisionPosition = collisionVerticesA[0];
					}
					else if (collisionVerticesB.size() == 1)
					{
						// 그 지점을 충돌위치로 한다.
						collisionPosition = collisionVerticesB[0];
					}
					// 두 콜라이더 모두 면으로 충돌한 경우
					else
					{
						// 그 면이 겹쳐진 부분을 얻어 내고 얻어낸 2d 도형의 각 꼭지점의 원본 위치(3d)를 평균내서 충돌 위치로 한다.

						collisionPosition = FindCollisionPosition(collisionVerticesA, collisionVerticesB, collisionDirection.Normalize());
					}
				}

				CollisionInfo result;
				result.colliderA = colliderA;
				result.colliderB = colliderB;
				result.position = collisionPosition;
				result.normalA = collisionDirection.Normalize() * collisionDepth;

				return result;
			}

			loopCount++;
		}
	}

	ZeldaMath::Vector3D PhysicsSystem::FindSupportPoint(Collider* colliderA, Collider* colliderB, ZeldaMath::Vector3D dirVec)
	{
		return colliderA->GetFarthestPoint(dirVec) - colliderB->GetFarthestPoint(-dirVec);
	}

	PhysicsSystem::SimplexResult PhysicsSystem::DoSimplex(Simplex& simplex, ZeldaMath::Vector3D& direction)
	{
		switch (simplex.Size())
		{
		case 2:
			return DoSimplex2(simplex, direction);
			break;

		case 3:
			return DoSimplex3(simplex, direction);
			break;

		case 4:
			return DoSimplex4(simplex, direction);
			break;

		default:
			assert(0);
			return CollisionImpossible;
			break;
		}
	}

	PhysicsSystem::SimplexResult PhysicsSystem::DoSimplex2(Simplex& simplex, ZeldaMath::Vector3D& direction)
	{
		Vector3D A = simplex[0];
		Vector3D B = simplex[1];
		
		Vector3D BANormal = (A - B).Normalize();
		Vector3D BONormal = -B.Normalize();

		// 입체 도형이 맞다면 A==B인 경우는 없다.
		assert(A != B);

		// 정확하게 점A 또는 점B만 충돌한 경우 or 점A, B, O가 한직선 위에 있는 경우
		if (A == Vector3D::zero || B == Vector3D::zero || BANormal == BONormal)
		{
			return CollisionOccurred;
		}

		direction = CrossProduct(CrossProduct(BANormal, BONormal), BANormal);
		return CollisionPossible;
	}

	PhysicsSystem::SimplexResult PhysicsSystem::DoSimplex3(Simplex& simplex, ZeldaMath::Vector3D& direction)
	{
		Vector3D A = simplex[0];
		Vector3D B = simplex[1];
		Vector3D C = simplex[2];
		Vector3D CANormal = (A - C).Normalize();
		Vector3D CBNormal = (B - C).Normalize();
		Vector3D CONormal = -C.Normalize();
		Vector3D CBA = CrossProduct(CBNormal, CANormal);

		assert(A != C);
		assert(B != C);

		// C가 AB위에 있는 경우, 이미 AB위에 O가 있는 경우는 걸러졌기 때문에 나와선 안된다.
		if (A == C || B == C || CANormal == CBNormal)
		{
			return CollisionImpossible;
		}

		// 삼각형 ABC가 이루는 평면과 수직이며 AC를 포함하는 평면의 바깥 부분에 원점이 존재하는 경우
		if (DotProduct(CrossProduct(CBA, CANormal), CONormal) > 0.0f)
		{
			simplex = { A, C };
			direction = CrossProduct(CrossProduct(CANormal, CONormal), CANormal);

			return CollisionPossible;
		}
		// 삼각형 ABC가 이루는 평면과 수직이며 BC를 포함하는 평면의 바깥 부분에 원점이 존재하는 경우
		else if (DotProduct(CrossProduct(-CBA, CBNormal), CONormal) > 0.0f)
		{
			simplex = { B, C };
			direction = CrossProduct(CrossProduct(CBNormal, CONormal), CBNormal);

			return CollisionPossible;
		}
		// 삼각형ABC 내부의 어떤 점에서 평면 ABC와 수직인 방향으로 이동한 위치에 원점이 존재하는 경우
		else
		{
			float dotCBA_CO = DotProduct(CBA, CONormal);

			// 삼각형ABC를 기준으로 CBA방향에 원점이 있는 경우
			if (dotCBA_CO > 0.0f)
			{
				direction = CBA;
				return CollisionPossible;
			}
			else if (dotCBA_CO < 0.0f)
			{
				simplex = { A, C, B };
				direction = -CBA;
				return CollisionPossible;
			}
			else
			{
				return CollisionOccurred;
			}
		}
	}

	PhysicsSystem::SimplexResult PhysicsSystem::DoSimplex4(Simplex& simplex, ZeldaMath::Vector3D& direction)
	{
		Vector3D A = simplex[0];
		Vector3D B = simplex[1];
		Vector3D C = simplex[2];
		Vector3D D = simplex[3];
		Vector3D DANormal = (A - D).Normalize();
		Vector3D DBNormal = (B - D).Normalize();
		Vector3D DCNormal = (C - D).Normalize();
		Vector3D DBA = CrossProduct(DBNormal, DANormal);
		Vector3D DAC = CrossProduct(DANormal, DCNormal);
		Vector3D DCB = CrossProduct(DCNormal, DBNormal);
		Vector3D DONormal = -D.Normalize();

		if (DotProduct(DBA, DONormal) > 0.0f)
		{
			simplex = { A, B, D };
			direction = DBA;

			return CollisionPossible;
		}

		if (DotProduct(DAC, DONormal) > 0.0f)
		{
			simplex = { A, D, C };
			direction = DAC;

			return CollisionPossible;
		}

		if (DotProduct(DCB, DONormal) > 0.0f)
		{
			simplex = { D, B, C };
			direction = DCB;

			return CollisionPossible;
		}

		return CollisionOccurred;
	}

	float PhysicsSystem::ComputeFriction(float frictionA, float frictionB)
	{
		return std::sqrt(frictionA * frictionB);
	}

	PhysicsSystem& PhysicsSystem::GetInstance()
	{
		static PhysicsSystem instance;
		return instance;
	}

	PhysicsSystem::PhysicsSystem() : colliderTree(nullptr), collisionResult(nullptr), lastCollisionResult(nullptr), isUpdatedHierarchy(false)
	{

	}

	PhysicsSystem::~PhysicsSystem()
	{
		delete colliderTree;
		delete collisionResult;
		delete lastCollisionResult;
	}


	// Simplex
	PhysicsSystem::Simplex::Simplex() : vertices(), indices()
	{

	}

	ZeldaMath::Vector3D& PhysicsSystem::Simplex::operator[](int n)
	{
		return vertices[n];
	}

	std::vector<ZeldaMath::Vector3D>& PhysicsSystem::Simplex::operator=(const std::vector<ZeldaMath::Vector3D>& right)
	{
		this->vertices = right;
		return this->vertices;
	}

	void PhysicsSystem::Simplex::Push(const ZeldaMath::Vector3D& value)
	{
		vertices.push_back(value);
	}

	bool PhysicsSystem::Simplex::CreateTetrahedron()
	{
		// [0, 1, 2], [0, 1, 3], [0, 2, 3], [1, 2, 3]
		// 위의 4개의 정점집합으로 4개의 면을 만든다.
		// 삼각형의 정점 순서 Winding Order는 바깥에서 바라볼때 시계방향으로 한다.
		// 즉 [a, b, c]에서 CrossProduct(ab, ac)를 했을 때, 다른 정점 d가 있는 방향 벡터가 나온다면 맞는 순서이다.

		assert(indices.size() == 0);

		size_t facetable[4][4] = { {0, 1, 2, 3}, {0, 1, 3, 2}, {0, 2, 3, 1}, {1, 2, 3, 0} };

		for (size_t i = 0; i < 4; i++)
		{
			Vector3D ab = vertices[facetable[i][1]] - vertices[facetable[i][0]];
			Vector3D ac = vertices[facetable[i][2]] - vertices[facetable[i][0]];
			Vector3D ad = vertices[facetable[i][3]] - vertices[facetable[i][0]];
			Vector3D ao = -vertices[facetable[i][0]];

			Vector3D insideNormal = -CrossProduct(ab, ac).Normalize();

			float result = DotProduct(ad, insideNormal);
			float depth = Abs(DotProduct(ao, insideNormal));

			// abcd가 한 평면에 있는 경우는 이미 걸러져 있어야 한다. (아주 작은 오차가 있을 때, 외적값이 0이 되면서 이 경우가 나오기도 했다... 리턴값을 이용해서 해결해야겠다.)
			if (result == 0.0f)
			{
				return false;
			}
			
			// 맞는 방향
			if (result > 0.0f)
			{
				Face face;
				face.v[0] = facetable[i][0];
				face.v[1] = facetable[i][1];
				face.v[2] = facetable[i][2];
				indices.insert({ depth, face });
			}
			// 반대 방향
			else
			{
				Face face;
				face.v[0] = facetable[i][0];
				face.v[1] = facetable[i][2];
				face.v[2] = facetable[i][1];
				indices.insert({ depth, face });
			}
		}

		innerPoint = Vector3D::zero;
		for (size_t i = 0; i < vertices.size(); i++)
		{
			innerPoint += vertices[i];
		}

		if (vertices.size() != 0)
		{
			innerPoint /= static_cast<float>(vertices.size());
		}

		return true;
	}

	bool PhysicsSystem::Simplex::ExpandFace(Collider* colliderA, Collider* colliderB)
	{
		assert(!indices.empty());

		Face face = (*indices.begin()).second;
		float depth = (*indices.begin()).first;

		Vector3D A = vertices[face.v[0]];
		Vector3D B = vertices[face.v[1]];
		Vector3D C = vertices[face.v[2]];

		Vector3D outsideNormal = CrossProduct(B - A, C - A).Normalize();
		
		Vector3D support = PhysicsSystem::GetInstance().FindSupportPoint(colliderA, colliderB, outsideNormal);

		float distDif = DotProduct(outsideNormal, support - A);

		// 새로운 면과의 거리차이가 너무 작은 경우
		if (distDif < EPA_EPSILON)
		{
			// 확장실패
			return false;
		}
		else
		{
			Vector3D D = support;
			size_t Aidx = face.v[0];
			size_t Bidx = face.v[1];
			size_t Cidx = face.v[2];
			size_t Didx = vertices.size();
			// 기존의 면을 제거한다.
			indices.erase(indices.begin());

			// 새로운 정점을 추가한다.
			vertices.push_back(D);

			// edgeList를 만든다.
			std::set<std::pair<size_t, size_t>> edges;
			edges.insert({ min(Aidx, Bidx), max(Aidx, Bidx) });
			edges.insert({ min(Bidx, Cidx), max(Bidx, Cidx) });
			edges.insert({ min(Cidx, Aidx), max(Cidx, Aidx) });

			// 새로운 정점 D가 추가되는 것으로 인해 사라져야 하는 면들을 찾아서 그 면의 모든 edge를 추가한다. 그 때 이미 중복된 edge가 set에 존재한다면 제거한다. (중복되는 edge로 인해 생성되는 면은 최종적으로 다면체 내부의 단면이라 추가하면 안된다.)
			for (auto iter = indices.begin(); iter != indices.end(); iter++)
			{
				size_t v0idx = (*iter).second.v[0];
				size_t v1idx = (*iter).second.v[1];
				size_t v2idx = (*iter).second.v[2];
				Vector3D v0 = vertices[v0idx];
				Vector3D v1 = vertices[v1idx];
				Vector3D v2 = vertices[v2idx];

				Vector3D faceNormal = CrossProduct(v1 - v0, v2 - v0).Normalize();
				Vector3D v0DNormal = (D - v0).Normalize();

				// 사라져야하는 면
				if (DotProduct(faceNormal, v0DNormal) > 0.0f)
				{
					// 현재 면을 삭제한다.
					iter = indices.erase(iter);

					// edges를 추가 또는 제거한다.
					if (edges.count({ min(v0idx, v1idx), max(v0idx, v1idx) }) != 0)
					{
						edges.erase({ min(v0idx, v1idx), max(v0idx, v1idx) });
					}
					else
					{
						edges.insert({ min(v0idx, v1idx), max(v0idx, v1idx) });
					}

					if (edges.count({ min(v1idx, v2idx), max(v1idx, v2idx) }) != 0)
					{
						edges.erase({ min(v1idx, v2idx), max(v1idx, v2idx) });
					}
					else
					{
						edges.insert({ min(v1idx, v2idx), max(v1idx, v2idx) });
					}

					if (edges.count({ min(v2idx, v0idx), max(v2idx, v0idx) }) != 0)
					{
						edges.erase({ min(v2idx, v0idx), max(v2idx, v0idx) });
					}
					else
					{
						edges.insert({ min(v2idx, v0idx), max(v2idx, v0idx) });
					}
				}

				// erase로 인해서 end가 됐다면 for문을 탈출해줘야한다.
				if (iter == indices.end())
				{
					break;
				}
			}

			for (auto iter = edges.begin(); iter != edges.end(); iter++)
			{
				size_t v0idx = (*iter).first;
				size_t v1idx = (*iter).second;
				Vector3D v0 = vertices[v0idx];
				Vector3D v1 = vertices[v1idx];

				//// 왠진 모르겠지만 충돌깊이와 방향이 0이 나오는 경우가 있음, 그것을 막기위한 코드
				//if (v0 == v1 || D == v0 || D == v1)
				//{
				//	continue;
				//}

				// 맞는 방향 (D, v0, v1 순서로 시계방향이라고 가정)
				if (DotProduct((D - innerPoint).Normalize(), CrossProduct(v0 - D, v1 - D).Normalize()) >= 0.0f)
				{
					Face tmpface;
					tmpface.v[0] = Didx;
					tmpface.v[1] = v0idx;
					tmpface.v[2] = v1idx;

					indices.insert({ DotProduct(D, CrossProduct(v0 - D, v1 - D).Normalize()), tmpface });
				}
				// 반대 방향
				else
				{
					Face tmpface;
					tmpface.v[0] = Didx;
					tmpface.v[1] = v1idx;
					tmpface.v[2] = v0idx;

					indices.insert({ DotProduct(D, CrossProduct(v1 - D, v0 - D).Normalize()), tmpface });
				}
			}

			return true;
		}
	}

	float PhysicsSystem::Simplex::GetNearestFaceDistance() const
	{
		return (*indices.begin()).first;
	}

	ZeldaMath::Vector3D PhysicsSystem::Simplex::GetNearestFaceDirection() const
	{
		Vector3D A = vertices[(*indices.begin()).second.v[0]];
		Vector3D B = vertices[(*indices.begin()).second.v[1]];
		Vector3D C = vertices[(*indices.begin()).second.v[2]];

		return CrossProduct(B - A, C - A).Normalize();
	}

	size_t PhysicsSystem::Simplex::Size()
	{
		return vertices.size();
	}
}