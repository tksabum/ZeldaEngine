#include "pch.h"
#include "TestComponent2.h"

#include "GameObject.h"
#include "Transform.h"
#include "Rigidbody.h"
#include "Camera.h"

#include "Input.h"
#include "TimeController.h"
#include "SceneManager.h"

namespace ZeldaEngine
{
	using namespace ZeldaMath;

	TestComponent2::TestComponent2() : limitLine(0.0f), rigid(nullptr), originPos(Vector3D::zero), originRot(Quaternion::identity)
	{

	}

	TestComponent2::~TestComponent2()
	{

	}

	void TestComponent2::Awake()
	{
		rigid = GetGameObject()->GetComponent<Rigidbody>();

		originPos = GetGameObject()->GetTransform()->GetLocalPosition();
		originRot = GetGameObject()->GetTransform()->GetLocalRotation();
	}

	void TestComponent2::Update()
	{
		float deltaTime = TimeController::GetInstance().GetDeltaTime();

		Vector3D updownForce = Vector3D::zero;
		Vector3D leftrightForce = Vector3D::zero;
		if (Input::GetInstance().PressedKey('I')) updownForce += Vector3D::up * deltaTime;
		if (Input::GetInstance().PressedKey('K')) updownForce -= Vector3D::up * deltaTime;
		if (Input::GetInstance().PressedKey('J')) leftrightForce += SceneManager::GetInstance().GetMainCamera()->GetGameObject()->GetTransform()->GetRight() * deltaTime;
		if (Input::GetInstance().PressedKey('L')) leftrightForce -= SceneManager::GetInstance().GetMainCamera()->GetGameObject()->GetTransform()->GetRight() * deltaTime;
		if (Input::GetInstance().PressedKey('O'))
		{
			GetGameObject()->GetTransform()->SetLocalPosition(originPos);
			GetGameObject()->GetTransform()->SetLocalRotation(originRot);

			if (rigid != nullptr)
			{
				rigid->SetVelocity(Vector3D::zero);
				rigid->SetAngularVelocity(Vector3D::zero);
			}
		}

		if (rigid != nullptr)
		{
			rigid->ApplyForce(rigid->GetCenterOfMass(), (updownForce + leftrightForce) * 30.0f);
		}

		if (GetGameObject()->GetTransform()->GetLocalPosition().y < limitLine)
		{
			GetGameObject()->GetTransform()->SetLocalPosition(originPos);
			GetGameObject()->GetTransform()->SetLocalRotation(originRot);

			if (rigid != nullptr)
			{
				rigid->SetVelocity(Vector3D::zero);
				rigid->SetAngularVelocity(Vector3D::zero);
			}
		}
	}

	void TestComponent2::SetLimitLine(float limitY)
	{
		limitLine = limitY;
	}

	void TestComponent2::PreSerialize(json& jsonData) const
	{
		PRESERIALIZE_BASE(TestComponent2);
		PRESERIALIZE_VALUE(limitLine);
	}

	void TestComponent2::PreDeserialize(const json& jsonData)
	{
		PREDESERIALIZE_BASE(TestComponent2);
		PREDESERIALIZE_VALUE(limitLine);
	}

	void TestComponent2::PostSerialize(json& jsonData) const
	{
		POSTSERIALIZE_BASE();
	}

	void TestComponent2::PostDeserialize(const json& jsonData)
	{
		POSTDESERIALIZE_BASE();
	}
}