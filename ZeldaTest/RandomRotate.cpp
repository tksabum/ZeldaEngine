#include "RandomRotate.h"

void RandomRotate::Awake()
{
	for (int i = 0; i < 20; i++)
	{
		{
			GameObject* object = SceneManager::GetInstance().CreateGameObject(L"Cube");
			object->SetLayerName(L"Weird");
			object->SetActive(false);
			object->AddComponent<CubeCollider>();
			Rigidbody* rigidbody = object->AddComponent<Rigidbody>();

			Transform* cubeTransform = object->GetTransform();
			cubeTransform->SetLocalScale(Vector3D(10, 10, 10));

			cubePool.push_back(cubeTransform);
		}

		{
			GameObject* object = SceneManager::GetInstance().CreateGameObject(L"Cylinder");
			object->SetLayerName(L"Weird");
			object->SetActive(false);
			object->AddComponent<CylinderCollider>();
			Rigidbody* rigidbody = object->AddComponent<Rigidbody>();

			Transform* cylinderTransform = object->GetTransform();
			cylinderTransform->SetLocalScale(Vector3D(10, 10, 10));

			cubePool.push_back(cylinderTransform);
		}

		{
			GameObject* object = SceneManager::GetInstance().CreateGameObject(L"Capsule");
			object->SetLayerName(L"Weird");
			object->SetActive(false);
			object->AddComponent<CapsuleCollider>();
			Rigidbody* rigidbody = object->AddComponent<Rigidbody>();

			Transform* capsuleTransform = object->GetTransform();
			capsuleTransform->SetLocalScale(Vector3D(10, 10, 10));

			cubePool.push_back(capsuleTransform);
		}
	}
}

void RandomRotate::Update()
{
	bool isExistCube = false;

	for (auto iter = activePool.begin(); iter != activePool.end(); iter++)
	{
		if ((*iter)->GetLocalPosition().y < -20)
		{
			(*iter)->GetGameObject()->SetActive(false);
			cubePool.push_back(*iter);
			iter = activePool.erase(iter);

			if (iter == activePool.end())
			{
				break;
			}
		}
	}

	for (auto iter = activePool.begin(); iter != activePool.end(); iter++)
	{
		if ((*iter)->GetLocalPosition().y > 30)
		{
			isExistCube = true;
		}
	}

	if (isExistCube == false && cubePool.empty() == false)
	{
		Transform* cubeTransform = cubePool.front();
		cubePool.pop_front();

		cubeTransform->GetGameObject()->SetActive(true);

		cubeTransform->SetLocalPosition(Vector3D((rand() % 50) - 25, 40, (rand() % 50) -25));
		cubeTransform->SetLocalRotation(Quaternion::identity);

		activePool.push_back(cubeTransform);
	}
}

void RandomRotate::PreSerialize(json& jsonData) const
{
	PRESERIALIZE_BASE(RandomRotate);
}

void RandomRotate::PreDeserialize(const json& jsonData)
{
	PREDESERIALIZE_BASE(RandomRotate);
}

void RandomRotate::PostSerialize(json& jsonData) const
{
	POSTSERIALIZE_BASE();
}

void RandomRotate::PostDeserialize(const json& jsonData)
{
	POSTDESERIALIZE_BASE();
}
