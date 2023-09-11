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



// ������ �ӽ�..
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

		// Rigidbody�� collider���踦 �����Ѵ�.
		if (isUpdatedHierarchy == false)
		{
			isUpdatedHierarchy = true;

			UpdateHierarchy();
		}
	}

	void PhysicsSystem::CollisionDetection()
	{
		// CollisionResult�� LastCollisionResult�� ������ ��
		// CollisionResult �ʱ�ȭ
		auto* temp = collisionResult;
		collisionResult = lastCollisionResult;
		lastCollisionResult = temp;
		collisionResult->clear();

		// �浹 ����� Simplex�� �����ϴ� collisionSimplexResult �ʱ�ȭ
		collisionSimplexResult.clear();

		// EPA ��� �ʱ�ȭ
		collisionInfoResult.clear();

		// Octree ������Ʈ
		for (size_t i = 0; i < colliderList.size(); i++)
		{
			// �̵��� ��쿡�� ������Ʈ
			if (colliderList[i]->GetGameObject()->GetTransform()->IsMoved())
			{
				colliderTree->Move(colliderList[i]->GetCenter(), colliderList[i]->GetMaxRadius(), colliderList[i]);

				// �̵����� ����
				colliderList[i]->GetGameObject()->GetTransform()->ResetMoved();
			}
		}

		// colliderTree�� SetAutoRadiusUpdate(false)���� �����Ǿ� �ֱ� ������ ����ϱ����� �ݵ�� UpdateAllRadius�� ȣ���� ��� �Ѵ�.
		colliderTree->UpdateAllRadius();

		// �浹 üũ
		for (size_t i = 0; i < colliderList.size(); i++)
		{
			Collider* currentCollider = colliderList[i];

			Vector3D center = currentCollider->GetCenter();
			float maxRadius = currentCollider->GetMaxRadius();

			// �浹 ������ �ݶ��̴��� Octree�� �̿��ؼ� ������ �Ÿ���.
			std::vector<Collider*> others = colliderTree->Query(center - Vector3D(1, 1, 1) * maxRadius, center + Vector3D(1, 1, 1) * maxRadius);

			for (size_t j = 0; j < others.size(); j++)
			{
				// Octree���� �ڱ��ڽŵ� ����ֱ� ������ �ڱ� �ڽŰ��� �浹ó���� ���ƾ��Ѵ�.
				if (currentCollider == others[j])
				{
					continue;
				}

				// �ߺ��� �˻縦 ���ϱ� ���� �ּҰ��� ���� �ݶ��̴��� currentCollider�� ���� �浹üũ�� �Ѵ�.
				if (currentCollider > others[j])
				{
					continue;
				}

				// ���ӿ�����Ʈ�� Active���°� �ƴ϶�� �浹üũ�� ���� �ʴ´�.
				if (!currentCollider->GetGameObject()->GetRootActive() || !others[j]->GetGameObject()->GetRootActive())
				{
					continue;
				}
				
				if (EngineSetting::GetInstance().GetCollisionSetting(currentCollider->GetGameObject()->GetLayerID(), others[j]->GetGameObject()->GetLayerID()) == false)
				{
					continue;
				}

				// trigger�� trigger������, trigger�� �ƴ� ���� trigger�� �ƴѰ� ������ �浹 ����
				if (currentCollider->IsTrigger() != others[j]->IsTrigger())
				{
					continue;
				}

				// ���� rigidbody ���� �ݶ��̴� ������ �浹���� �ʴ´�.
				if (currentCollider->GetMaster() != nullptr && others[j]->GetMaster() != nullptr && currentCollider->GetMaster()->GetRoot() == others[j]->GetMaster()->GetRoot())
				{
					continue;
				}
				
				// ���� �ݶ��̴����� �浹üũ
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

			// Trigger�� �浹��ġ�� �浹���̸� ����� �ʿ䰡 ����.
			if (colliderA->IsTrigger() == true && colliderB->IsTrigger() == true)
			{
				continue;
			}

			// Rigidbody�� ������ ���������� ���� ���� (Trigger�� ��������)
			if (colliderA->GetMaster() == nullptr || colliderB->GetMaster() == nullptr)
			{
				continue;
			}

			CollisionInfo collisionInfo = CollisionDetectionByEPA(colliderA, colliderB, (*iter).second);

			// �븻�� 0�� ���� ���������� ���� �ʴ´�.
			if (collisionInfo.normalA != Vector3D::zero)
			{
				collisionInfoResult[{ min(colliderA, colliderB), max(colliderA, colliderB) }] = collisionInfo;
			}
		}

		// Octree�� ������� �ʴ� �浹 üũ
		//for (size_t i = 0; i < colliderList.size(); i++)
		//{
		//	Collider* currentCollider = colliderList[i];

		//	for (size_t j = 0; j < colliderList.size(); j++)
		//	{
		//		// Octree���� �ڱ��ڽŵ� ����ֱ� ������ �ڱ� �ڽŰ��� �浹ó���� ���ƾ��Ѵ�.
		//		if (currentCollider == colliderList[j])
		//		{
		//			continue;
		//		}

		//		// �ߺ��� �˻縦 ���ϱ� ���� �ּҰ��� ���� �ݶ��̴��� currentCollider�� ���� �浹üũ�� �Ѵ�.
		//		if (currentCollider > colliderList[j])
		//		{
		//			continue;
		//		}

		//		// ���ӿ�����Ʈ�� Active���°� �ƴ϶�� �浹üũ�� ���� �ʴ´�.
		//		if (!currentCollider->GetGameObject()->GetRootActive() || !colliderList[j]->GetGameObject()->GetRootActive())
		//		{
		//			continue;
		//		}

		//		// ���� �ݶ��̴����� �浹üũ
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

		// rigidbody���� ������ ������ ����ؾ��Ѵ�.
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

		// ������ ���� ��ŭ �л�� ���� ����ȴ�.
		for (auto iter = collisionInfoResult.begin(); iter != collisionInfoResult.end(); iter++)
		{
			Rigidbody* rigidbodyA = (*iter).second.colliderA->GetMaster()->GetRoot();
			Rigidbody* rigidbodyB = (*iter).second.colliderB->GetMaster()->GetRoot();

			// Rigidbody�� ���ٸ� ������ �ʿ䰡 ����.
			if (rigidbodyA == nullptr || rigidbodyB == nullptr) continue;

			// force, tanforce�� ������ �ش�.
			float collisionCounter = static_cast<float>(rigidbodyCollsionCounter[{ min(rigidbodyA, rigidbodyB), max(rigidbodyA, rigidbodyB) }]);

			assert(collisionCounter != 0.0f);
			if (collisionCounter == 0.0f)
			{
				collisionCounter = 1.0f;
			}

			Vector3D nor = (*iter).second.normalA.Normalize();
			float depth = (*iter).second.normalA.Size();
			Vector3D pos = (*iter).second.position;

#pragma region ������ ��ŭ �о��
			// ������ ��ŭ �о��.
			if (rigidbodyA->GetKinematic() && rigidbodyB->GetKinematic())
			{
				// �� �� kinematic�� ��� �׳� �д�.
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
				// depth�� INV_VELOCITY_DEPTH_MIN���� ���� ��� �ӵ��� �����Ѵ�.
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


#pragma region �浹������ ��� ��ȯ
			Vector3D AP = pos - rigidbodyA->GetCenterOfMass();
			Vector3D BP = pos - rigidbodyB->GetCenterOfMass();

			// A, B�� �ӵ�
			Vector3D v1 = rigidbodyA->GetVelocity() + CrossProduct(rigidbodyA->GetAngularVelocity(), AP);
			Vector3D v2 = rigidbodyB->GetVelocity() + CrossProduct(rigidbodyB->GetAngularVelocity(), BP);
			// ���ӵ�
			Vector3D relativeVelocity = v2 - v1;

			// �븻���� �ӵ�
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


#pragma region ������ ����
				// �븻�� ���� ������ ���ӵ�
				Vector3D tangent = relativeVelocity - (nor * norV);

				// ��� �ӵ��� 0�� �ƴѰ�쿡�� �����Ѵ�.
				if (tangent != Vector3D::zero)
				{
					Vector3D tanNor = tangent.Normalize();

					float tanForce = -DotProduct(relativeVelocity, tanNor);
					tanForce /= numerator;
					tanForce /= collisionCounter;

					Vector3D frictionImpulse;

					// ���� ����
					if (Abs(tanForce) < force * ComputeFriction(rigidbodyA->GetSfriction(), rigidbodyB->GetSfriction()))
					{
						frictionImpulse = tanNor * tanForce;
					}
					// � ����
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

		// �߷� ����
		for (auto iter = rigidbodyList.begin(); iter != rigidbodyList.end(); iter++)
		{
			if ((*iter)->GetUseGravity())
			{
				(*iter)->ApplyAcceledration((*iter)->GetGravity() * deltaTime);
			}
		}


		// ���ӵ��� �ӵ��� ��ȯ, �����ӵ��� ���ӵ��� ��ȯ
		for (auto iter = rigidbodyList.begin(); iter != rigidbodyList.end(); iter++)
		{
			(*iter)->LinearAccelerate();
			(*iter)->AngularAccelerate();
		}

		// �ӵ��� ���� �̵�
		for (auto iter = rigidbodyList.begin(); iter != rigidbodyList.end(); iter++)
		{
			(*iter)->Move((*iter)->GetVelocity()* deltaTime);
		}

		// ���ӵ��� ���� ȸ��
		// ����� ������Ʈ�� �߽��� ������ ���� �������� ȸ���Ѵ�...
		// ��Ʈ���ڵ尡 �����Ϸ��� �߽��� ������ �ʴ� ������ ȸ���� �����ؾ� �Ѵ�.
		for (auto iter = rigidbodyList.begin(); iter != rigidbodyList.end(); iter++)
		{
			Vector3D axis = (*iter)->angularVelocity.Normalize();
			float angle = (*iter)->angularVelocity.Size();
			
			(*iter)->GetGameObject()->GetTransform()->Rotate((*iter)->GetCenterOfMass(), axis, angle * deltaTime);
		}

		// ���� ����
		for (auto iter = rigidbodyList.begin(); iter != rigidbodyList.end(); iter++)
		{
			(*iter)->SetVelocity((*iter)->GetVelocity() * (powf(1.0f - (*iter)->GetLinearDrag(), deltaTime)));
			(*iter)->SetAngularVelocity((*iter)->GetAngularVelocity() * (powf(1.0f - (*iter)->GetAngularDrag(), deltaTime)));
		}
	}

	void PhysicsSystem::CollisionEvent()
	{
		// collisionResult�� lastCollisionResult�� enter, stay, exit�� ����
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

		// ��� �ݶ��̴� ������ ���� �ʴ� �Լ�
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


		// ��� �ݶ��̴� ������ �޴� �Լ�
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

		// �ݶ��̴��� �����Ҷ��� �̹� shape, transform���� �̹� �����Ǿ��� �� �ֱ� ������ ��ġ�������� �����Ѵ�.
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

		// �׷��Ƚ� �޸� �ִ�ġ�� ����... ����� ���� ����
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

		// ������ ������ ���� �ʱ�ȭ�Ѵ�.
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

		// rigidbodyList�� �����.
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

		// collider �߰�
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

		// ������� ������ ����
		Vector3D dirVec = (vertices2D[0] - vertices2D[1]).Normalize();
		// ��鿡 ������ ������ ����
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

		// dirVec �������� ���� �� ���� ã�´�.
		for (size_t i = 1; i < vertices2D.size(); i++)
		{
			float length = DotProduct(dirVec, vertices2D[i]);
			if (maxLength < length)
			{
				maxLength = length;
				maxIdx = i;
			}
		}

		// ���� �� ���� ������ ��
		std::vector<Vector3D> v;
		// ���� �� ��
		Vector3D startP = vertices2D[maxIdx];

		for (size_t i = 0; i < vertices2D.size(); i++)
		{
			if (i != maxIdx)
			{
				v.push_back(vertices2D[i]);
			}
		}

		// dirVec, nor�� ������ ���� 2D�׸��󿡼� dirVec�� ������ nor�� ����(���»�� ������ ������ ����)�̶�� �ϸ� basicVec�� �������̴�.
		Vector3D basicVec = CrossProduct(dirVec, nor).Normalize();

		std::vector<std::pair<float, size_t>> sortedData;
		for (size_t i = 0; i < v.size(); i++)
		{
			// first�� ���̰��� cos���� ����, �� ���Ľ� basicVec�� ���հ��� ū ������� ���ĵ�
			sortedData.push_back({ DotProduct((v[i] - startP).Normalize(), basicVec), i });
		}

		sort(sortedData.begin(), sortedData.end());

		// ������ ������ �������� ���� �Ʒ������� ���͸� �������� �ð�������� ���ĵǾ�����
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

					// �ð������
					if (DotProduct(CrossProduct(v1 - v0, now - v1).Normalize(), nor) <= 0.0f)
					{
						result.push_back(now);
						break;
					}
					// �ݽð������
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
		// FindCollisionPosition�� ���� verticesA�� verticesB�� �ſ� ���� ���� ������ ������ �ִٰ� �����ص� �� ��
		// �ϴ� vertices�� ���� ���� ���� ȿ���� �Ű澲�� �ʰ� �����ϰ���

		// ���⺤�ʹ� �븻������ �صд�.
		nor = nor.Normalize();

		// nor�� �����̸� ������ ������ ��鿡 ���翵 (������ ���� ������ ��鿡 �ϸ� �Ǵµ� ������ ������)
		
		// �� �浹�� �̹� �Ÿ��� �� �Լ��� ȣ���ߴٰ� ������
		assert(verticesA.size() != 1 && verticesB.size() != 1);

		// ���а� ������ �浹 
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
		// ���а� ���� �浹
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

					// ���� ������
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

					// ���� ������
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

				// �������� ����� ���� 2���� ���ð��� (���� �� �𼭸��� ��ġ�Ѵٸ� �ȳ��� ���� �ֱ���, ���� �幮���)
				// �ϴ��� �����ϸ� ���� ��ճ���.
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

			// �̰� ������ �ȵǴµ�... �������� ���� ���´�
			// ������ �ϳ��� �ȿ� �ϳ��� �ۿ� �ִ� ����ε� ���а� ���� �������� ã�� ���� ��
			// assert(0); 
			return (line[0] + line[1]) / 2.0f;
		}
		// A�� �� B�� �� �浹
		else
		{
			// ������ ���ο� �ִ� ��� �� + �������� ���
			Vector3D averagePos = Vector3D::zero;
			int cnt = 0;

			std::vector<Vector3D> planeA = Make2DConvexHull(verticesA);
			std::vector<Vector3D> planeB = Make2DConvexHull(verticesB);

			if (planeA.size() == 1) return planeA[0];
			if (planeB.size() == 1) return planeB[0];
			
			// ������ �ֱ�
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
					// ������ ��� ������ ������ ����
					else if (1.0f - Abs(DotProduct((A0 - A1).Normalize(), (B0 - B1).Normalize())) < 1e-6f)
					{
						// �ϴ� ��� ���� ���ϰ�
						averagePos += (A0 + A1 + B0 + B1);
						// ��������� ���� �� ���� ����.
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

			// ������ �ֱ�
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

		// ������� ���� �ӽ÷� ���ѷ����� ���� �Ѵ�...
		while (loopCount < GJK_MAXLOOP)
		{
			direction = direction.Normalize();

			supportPoint = FindSupportPoint(colliderA, colliderB, direction);
			Vector3D OP = supportPoint.Normalize();

			// supportPoint�� direction�� ������ 90�� �̻� ���̰� ���ٸ� �浹�� �Ұ��� �ϴ�.
			if (DotProduct(OP, direction) < 0.0f)
			{
				return false;
			}

			// Simplex�� SupportPoint�� �߰��ϰ� Simplex�� ������ ��ġ ������ �����Ѵ�.
			simplex.Push(supportPoint);

			switch (DoSimplex(simplex, direction))
			{
			case CollisionOccurred:
				// ���⼭ EPA���� �ʿ��� Simplex�� ������ �����صд�.
				collisionSimplexResult.insert({ { colliderA, colliderB }, simplex });
				return true;
				break;

			case CollisionPossible:
				// DoSimplex���� ������ ������ ������� �����ֱ� ������ �ƹ��͵� �� �ʿ䰡 ����.
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

		// ���ü�� �Ǳ����� ������ ������ �߰��Ѵ�.
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

		// ���ü�� �Ǳ����� ������ ������ �߰��Ѵ�.
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

		// ���ü ����� ��´�. ���������� ������ ���̸� ���ϰ� ������ �صд�.
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

			// simplex�� Ȯ��
			bool expandResult = simplex.ExpandFace(colliderA, colliderB);

			// Ȯ�忡 �����ϰ� EPA_MAXLOOP�� �ʰ����� ���� ���
			if (expandResult && loopCount < EPA_MAXLOOP)
			{
				// Ȯ��� simplex�� �߰��� ���� ������ queue�� �߰��Ѵ�.
				// ExpandFace() �Լ����� �˾Ƽ� �߰���, �� ���� ����� ������� �����Ѵ�. �̹� ���ĵǾ� �ִ� �����Ͱ� �����Ƿ� �߰��� �鸸 ����Ž���Ͽ� ������ �����Ѵ�.

			}
			// Ȯ�忡 �����ߴٸ�
			else
			{
				// Ȯ�忡 ������ �� ��� ���� ������ ���Ͱ� �浹���̿� �浹������ ��Ÿ����.
				float collisionDepth = 0.0f;
				Vector3D collisionPosition = Vector3D::zero;
				Vector3D collisionDirection = simplex.GetNearestFaceDirection();

				// colliderA�� Ż�� ������ -collisionDirection
				// colliderB�� Ż�� ������ +collisionDirection


				if (collisionDirection != Vector3D::zero)
				{
					// �浹���� ���
					collisionDepth = simplex.GetNearestFaceDistance();

					// �浹��ġ ���

					// A�� �浹�������� ���� �ָ��ִ� ���� �� ���� Epsilon ũ�⸸ŭ ���̳��� vertex�� ��� ���´�.
					std::vector<Vector3D> collisionVerticesA = colliderA->GetFarthestPoint(collisionDirection, EPA_VERTEX_EPSILON);

					// B�� �浹�������� ���� �ָ��ִ� ���� �� ���� Epsilon ũ�⸸ŭ ���̳��� vertex�� ��� ���´�.
					std::vector<Vector3D> collisionVerticesB = colliderB->GetFarthestPoint(-collisionDirection, EPA_VERTEX_EPSILON);


					// �� �浹 ���⺤�͸� �̿��ؼ� colliderA�� colliderB�� �� �浹����, �� �浹���� �Ǵ��Ѵ�.
					// �� �浹�� �ݶ��̴��� �����Ѵٸ�
					if (collisionVerticesA.size() == 1)
					{
						// �� ������ �浹��ġ�� �Ѵ�.
						collisionPosition = collisionVerticesA[0];
					}
					else if (collisionVerticesB.size() == 1)
					{
						// �� ������ �浹��ġ�� �Ѵ�.
						collisionPosition = collisionVerticesB[0];
					}
					// �� �ݶ��̴� ��� ������ �浹�� ���
					else
					{
						// �� ���� ������ �κ��� ��� ���� �� 2d ������ �� �������� ���� ��ġ(3d)�� ��ճ��� �浹 ��ġ�� �Ѵ�.

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

		// ��ü ������ �´ٸ� A==B�� ���� ����.
		assert(A != B);

		// ��Ȯ�ϰ� ��A �Ǵ� ��B�� �浹�� ��� or ��A, B, O�� ������ ���� �ִ� ���
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

		// C�� AB���� �ִ� ���, �̹� AB���� O�� �ִ� ���� �ɷ����� ������ ���ͼ� �ȵȴ�.
		if (A == C || B == C || CANormal == CBNormal)
		{
			return CollisionImpossible;
		}

		// �ﰢ�� ABC�� �̷�� ���� �����̸� AC�� �����ϴ� ����� �ٱ� �κп� ������ �����ϴ� ���
		if (DotProduct(CrossProduct(CBA, CANormal), CONormal) > 0.0f)
		{
			simplex = { A, C };
			direction = CrossProduct(CrossProduct(CANormal, CONormal), CANormal);

			return CollisionPossible;
		}
		// �ﰢ�� ABC�� �̷�� ���� �����̸� BC�� �����ϴ� ����� �ٱ� �κп� ������ �����ϴ� ���
		else if (DotProduct(CrossProduct(-CBA, CBNormal), CONormal) > 0.0f)
		{
			simplex = { B, C };
			direction = CrossProduct(CrossProduct(CBNormal, CONormal), CBNormal);

			return CollisionPossible;
		}
		// �ﰢ��ABC ������ � ������ ��� ABC�� ������ �������� �̵��� ��ġ�� ������ �����ϴ� ���
		else
		{
			float dotCBA_CO = DotProduct(CBA, CONormal);

			// �ﰢ��ABC�� �������� CBA���⿡ ������ �ִ� ���
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
		// ���� 4���� ������������ 4���� ���� �����.
		// �ﰢ���� ���� ���� Winding Order�� �ٱ����� �ٶ󺼶� �ð�������� �Ѵ�.
		// �� [a, b, c]���� CrossProduct(ab, ac)�� ���� ��, �ٸ� ���� d�� �ִ� ���� ���Ͱ� ���´ٸ� �´� �����̴�.

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

			// abcd�� �� ��鿡 �ִ� ���� �̹� �ɷ��� �־�� �Ѵ�. (���� ���� ������ ���� ��, �������� 0�� �Ǹ鼭 �� ��찡 �����⵵ �ߴ�... ���ϰ��� �̿��ؼ� �ذ��ؾ߰ڴ�.)
			if (result == 0.0f)
			{
				return false;
			}
			
			// �´� ����
			if (result > 0.0f)
			{
				Face face;
				face.v[0] = facetable[i][0];
				face.v[1] = facetable[i][1];
				face.v[2] = facetable[i][2];
				indices.insert({ depth, face });
			}
			// �ݴ� ����
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

		// ���ο� ����� �Ÿ����̰� �ʹ� ���� ���
		if (distDif < EPA_EPSILON)
		{
			// Ȯ�����
			return false;
		}
		else
		{
			Vector3D D = support;
			size_t Aidx = face.v[0];
			size_t Bidx = face.v[1];
			size_t Cidx = face.v[2];
			size_t Didx = vertices.size();
			// ������ ���� �����Ѵ�.
			indices.erase(indices.begin());

			// ���ο� ������ �߰��Ѵ�.
			vertices.push_back(D);

			// edgeList�� �����.
			std::set<std::pair<size_t, size_t>> edges;
			edges.insert({ min(Aidx, Bidx), max(Aidx, Bidx) });
			edges.insert({ min(Bidx, Cidx), max(Bidx, Cidx) });
			edges.insert({ min(Cidx, Aidx), max(Cidx, Aidx) });

			// ���ο� ���� D�� �߰��Ǵ� ������ ���� ������� �ϴ� ����� ã�Ƽ� �� ���� ��� edge�� �߰��Ѵ�. �� �� �̹� �ߺ��� edge�� set�� �����Ѵٸ� �����Ѵ�. (�ߺ��Ǵ� edge�� ���� �����Ǵ� ���� ���������� �ٸ�ü ������ �ܸ��̶� �߰��ϸ� �ȵȴ�.)
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

				// ��������ϴ� ��
				if (DotProduct(faceNormal, v0DNormal) > 0.0f)
				{
					// ���� ���� �����Ѵ�.
					iter = indices.erase(iter);

					// edges�� �߰� �Ǵ� �����Ѵ�.
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

				// erase�� ���ؼ� end�� �ƴٸ� for���� Ż��������Ѵ�.
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

				//// ���� �𸣰����� �浹���̿� ������ 0�� ������ ��찡 ����, �װ��� �������� �ڵ�
				//if (v0 == v1 || D == v0 || D == v1)
				//{
				//	continue;
				//}

				// �´� ���� (D, v0, v1 ������ �ð�����̶�� ����)
				if (DotProduct((D - innerPoint).Normalize(), CrossProduct(v0 - D, v1 - D).Normalize()) >= 0.0f)
				{
					Face tmpface;
					tmpface.v[0] = Didx;
					tmpface.v[1] = v0idx;
					tmpface.v[2] = v1idx;

					indices.insert({ DotProduct(D, CrossProduct(v0 - D, v1 - D).Normalize()), tmpface });
				}
				// �ݴ� ����
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