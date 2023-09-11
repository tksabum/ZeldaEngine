#include "pch.h"
#include "TestComponent.h"

#include "Input.h"
#include "TimeController.h"
#include "SoundManager.h"
#include "SceneManager.h"

#include "GameObject.h"
#include "Transform.h"
#include "MeshRenderer.h"
#include "Camera.h"

namespace ZeldaEngine
{
	using namespace ZeldaMath;

	std::list<TestComponent*> TestComponent::testComponentList;
	bool TestComponent::updateOnlyOneComponent = false;

	TestComponent::TestComponent() :
		moveSpeed(50.0f),
		rotateSpeed(0.785f),
		renderer(nullptr),
		lastRotation(Quaternion::identity)
	{
		testComponentList.push_back(this);
	}

	TestComponent::~TestComponent()
	{
		for (auto iter = testComponentList.begin(); iter != testComponentList.end(); iter++)
		{
			if (*iter == this)
			{
				testComponentList.erase(iter);
				break;
			}
		}
	}

	void TestComponent::Update()
	{
		if (*(testComponentList.begin()) != this)
		{
			return;
		}

		if (!updateOnlyOneComponent)
		{
			updateOnlyOneComponent = true;
		}
		else
		{
			return;
		}

		OutputDebugString((std::to_wstring(GetGameObject()->GetTransform()->GetLocalPosition().x ) + L" "
			 + (std::to_wstring(GetGameObject()->GetTransform()->GetLocalPosition().y) + L" ")
			 + (std::to_wstring(GetGameObject()->GetTransform()->GetLocalPosition().z) + L" \n")).c_str());

		if (Input::GetInstance().PressedKeyNow('1')) SoundManager::GetInstance().Play(L"BGM_Test001");
		if (Input::GetInstance().PressedKeyNow('2')) SoundManager::GetInstance().Play(L"BGM_Test002");
		if (Input::GetInstance().PressedKeyNow('3')) SoundManager::GetInstance().Play(L"BGM_Test003");
		if (Input::GetInstance().PressedKeyNow('4')) SoundManager::GetInstance().Play(L"EFFECT_Test001");
		if (Input::GetInstance().PressedKeyNow('5')) SoundManager::GetInstance().Play(L"EFFECT_Test002");
		if (Input::GetInstance().PressedKeyNow('6')) SoundManager::GetInstance().Play(L"EFFECT_Test003");

		if (Input::GetInstance().PressedKeyNow('0')) SoundManager::GetInstance().StopAll();


		if (Input::GetInstance().PressedKeyNow(VK_ESCAPE))
		{
			testComponentList.erase(testComponentList.begin());
			testComponentList.push_back(this);
		}

		Vector3D moveDelta = Vector3D::zero;
		float deltaTime = TimeController::GetInstance().GetDeltaTime();

		// 카메라인경우
		if (GetGameObject() == SceneManager::GetInstance().GetMainCamera()->GetGameObject())
		{
			if (Input::GetInstance().PressedKey('W')) moveDelta += GetGameObject()->GetTransform()->GetFront() * moveSpeed * deltaTime;
			if (Input::GetInstance().PressedKey('S')) moveDelta -= GetGameObject()->GetTransform()->GetFront() * moveSpeed * deltaTime;
			if (Input::GetInstance().PressedKey('A')) moveDelta -= GetGameObject()->GetTransform()->GetRight() * moveSpeed * deltaTime;
			if (Input::GetInstance().PressedKey('D')) moveDelta += GetGameObject()->GetTransform()->GetRight() * moveSpeed * deltaTime;
			if (Input::GetInstance().PressedKey('Q')) moveDelta -= GetGameObject()->GetTransform()->GetUp() * moveSpeed * deltaTime;
			if (Input::GetInstance().PressedKey('E')) moveDelta += GetGameObject()->GetTransform()->GetUp() * moveSpeed * deltaTime;
		}
		// 아닌경우
		else
		{
			if (Input::GetInstance().PressedKey('W')) moveDelta += Vector3D::front * moveSpeed * deltaTime;
			if (Input::GetInstance().PressedKey('S')) moveDelta -= Vector3D::front * moveSpeed * deltaTime;
			if (Input::GetInstance().PressedKey('A')) moveDelta -= Vector3D::right * moveSpeed * deltaTime;
			if (Input::GetInstance().PressedKey('D')) moveDelta += Vector3D::right * moveSpeed * deltaTime;
			if (Input::GetInstance().PressedKey('Q')) moveDelta -= Vector3D::up * moveSpeed * deltaTime;
			if (Input::GetInstance().PressedKey('E')) moveDelta += Vector3D::up * moveSpeed * deltaTime;
		}
		

		float rotateDeltaY = 0.0f;
		float rotateDeltaX = 0.0f;

		GetGameObject()->GetTransform()->SetLocalPosition(GetGameObject()->GetTransform()->GetLocalPosition() + moveDelta);
		
		if (Input::GetInstance().PressedKey(VK_RBUTTON))
		{
			if (Input::GetInstance().PressedKeyNow(VK_RBUTTON))
			{
				lastMousePosition = Input::GetInstance().GetMousePosition();
			}
			else
			{
				Vector2D mousePos = Input::GetInstance().GetMousePosition();
				Vector2D mouseDelta = mousePos - lastMousePosition;

				rotateDeltaY += (100.0f / 1920.0f) * mouseDelta.x * deltaTime;
				rotateDeltaX += (100.0f / 1080.0f) * mouseDelta.y * deltaTime;

				GetGameObject()->GetTransform()->Rotate(ZeldaMath::Vector3D(0.0f, 1.0f, 0.0f), rotateDeltaY);
				GetGameObject()->GetTransform()->Rotate(GetGameObject()->GetTransform()->GetRight(), rotateDeltaX);

				lastMousePosition = mousePos;
			}
		}

		//if (Input::GetInstance().PressedKey('A')) rotateDeltaY -= rotateSpeed * deltaTime;
		//if (Input::GetInstance().PressedKey('D')) rotateDeltaY += rotateSpeed * deltaTime;
		//if (Input::GetInstance().PressedKey('W')) rotateDeltaX -= rotateSpeed * deltaTime;
		//if (Input::GetInstance().PressedKey('S')) rotateDeltaX += rotateSpeed * deltaTime;


	}

	void TestComponent::LateUpdate()
	{
		updateOnlyOneComponent = false;
	}
	
	void TestComponent::OnTriggerEnter()
	{
		if (renderer == nullptr)
		{
			renderer = GetGameObject()->GetComponent<MeshRenderer>();
		}

		if (renderer != nullptr)
		{
			renderer->SetColor(ZeldaColor::Red);
		}
	}
	
	void TestComponent::OnTriggerExit()
	{
		if (renderer == nullptr)
		{
			renderer = GetGameObject()->GetComponent<MeshRenderer>();
		}

		if (renderer != nullptr)
		{
			renderer->SetColor(ZeldaColor::Blue);
		}
	}

	void TestComponent::PreSerialize(json& jsonData) const
	{
		PRESERIALIZE_BASE(TestComponent);

		PRESERIALIZE_VALUE(moveSpeed);
		PRESERIALIZE_VALUE(rotateSpeed);

		PRESERIALIZE_VECTOR_VALUE(testVectorValue);
		PRESERIALIZE_VECTOR_SERVALUE(testVectorSerValue);
		PRESERIALIZE_VECTOR_PTR(testVectorPtr);

		PRESERIALIZE_VECTOR_VALUE(testVectorValue0);
		PRESERIALIZE_VECTOR_SERVALUE(testVectorSerValue0);
		PRESERIALIZE_VECTOR_PTR(testVectorPtr0);
	}

	void TestComponent::PreDeserialize(const json& jsonData)
	{
		PREDESERIALIZE_BASE(TestComponent);

		PREDESERIALIZE_VALUE(moveSpeed);
		PREDESERIALIZE_VALUE(rotateSpeed);

		PREDESERIALIZE_VECTOR_VALUE(testVectorValue);
		PREDESERIALIZE_VECTOR_SERVALUE(testVectorSerValue);
		PREDESERIALIZE_VECTOR_PTR(testVectorPtr);

		PREDESERIALIZE_VECTOR_VALUE(testVectorValue0);
		PREDESERIALIZE_VECTOR_SERVALUE(testVectorSerValue0);
		PREDESERIALIZE_VECTOR_PTR(testVectorPtr0);
	}

	void TestComponent::PostSerialize(json& jsonData) const
	{
		POSTSERIALIZE_BASE();

		POSTSERIALIZE_VECTOR_SERVALUE(testVectorSerValue);
		POSTSERIALIZE_VECTOR_PTR(testVectorPtr);

		POSTSERIALIZE_VECTOR_SERVALUE(testVectorSerValue0);
		POSTSERIALIZE_VECTOR_PTR(testVectorPtr0);
	}

	void TestComponent::PostDeserialize(const json& jsonData)
	{
		POSTDESERIALIZE_BASE();
		
		POSTDESERIALIZE_VECTOR_SERVALUE(testVectorSerValue);
		POSTDESERIALIZE_VECTOR_PTR(testVectorPtr);

		POSTDESERIALIZE_VECTOR_SERVALUE(testVectorSerValue0);
		POSTDESERIALIZE_VECTOR_PTR(testVectorPtr0);
	}
}