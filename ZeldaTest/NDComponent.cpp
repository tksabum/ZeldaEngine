#include "NDComponent.h"
#include "Dotween.h"

NDComponent::NDComponent()
{

}

void NDComponent::Awake()
{
	moveSpeed = 10.0f;
	rotationAngleSpeed = 5.0f;
	errorRange = 2.0f;
	dashChargingTime = 2.0f;

	playerAxis = SceneManager::GetInstance().CreateGameObject(L"playerAxis");
	playerfrontObject = SceneManager::GetInstance().CreateGameObject(L"playerfront");
}

void NDComponent::Start()
{
	// �÷��̾��� ������ ��ǥ�� �����ϱ����� �ֺ� �θ� ������Ʈ ����
	if (playerAxis == nullptr)
	{
		playerAxis->GetTransform()->SetLocalPosition(GetGameObject()->GetTransform()->GetLocalPosition());
		playerAxis->GetTransform()->SetLocalRotation(GetGameObject()->GetTransform()->GetLocalRotation());
	}
	GetGameObject()->GetTransform()->SetParent(playerAxis->GetTransform());
	GetGameObject()->GetTransform()->AddChild(playerfrontObject->GetTransform());
	playerfrontObject->GetTransform()->SetLocalScale(Vector3D(0.1, 0.1, 0.1));
	playerfrontObject->GetTransform()->SetLocalPosition(Vector3D(0, 0, 1));

	// �÷��̾��� ���� ǥ��
	MeshRenderer* mesh = playerfrontObject->AddComponent<MeshRenderer>();
	mesh->SetMesh(L"Cube");
	mesh->SetMaterial(L"BoundaryDebug");

	// ��Ʈ�� �߰�
	playerDotween = GetGameObject()->AddComponent<Dotween>();

	// Idle--------------------------------------------------------------------------------
	// Idle ���¿��� Move ���·� �������� ����
	playerFSM.transitions[PlayerState::Idle].push_back({ PlayerState::Move,
		[]() {return Input::GetInstance().PressedKey(VK_UP) ||
		Input::GetInstance().PressedKey(VK_DOWN) ||
		Input::GetInstance().PressedKey(VK_LEFT) ||
		Input::GetInstance().PressedKey(VK_RIGHT) ||
		Input::GetInstance().PressedKey(VK_SHIFT) ||
		Input::GetInstance().PressedKey(VK_ESCAPE);
		} });

	// Idle ���¿��� Attack ���·� �������� ����
	playerFSM.transitions[PlayerState::Idle].push_back({ PlayerState::Attack,
		[]() {return Input::GetInstance().PressedKey('Z'); } });

	// Idle ���¿��� SpecialAttack ���·� �������� ����
	playerFSM.transitions[PlayerState::Idle].push_back({ PlayerState::SpecialAttack,
		[]() {return Input::GetInstance().PressedKey('X'); } });

	// Idle ���¿��� Interaction ���·� �������� ����
	playerFSM.transitions[PlayerState::Idle].push_back({ PlayerState::Interaction,
		[]() {return Input::GetInstance().PressedKey('C'); } });


	// Move--------------------------------------------------------------------------------
	// Move ���¿��� Idle ���·� �������� ����
	playerFSM.transitions[PlayerState::Move].push_back({ PlayerState::Idle,
		[]() {return !Input::GetInstance().PressedKey(VK_UP) &&
		!Input::GetInstance().PressedKey(VK_DOWN) &&
		!Input::GetInstance().PressedKey(VK_LEFT) &&
		!Input::GetInstance().PressedKey(VK_RIGHT) &&
		!Input::GetInstance().PressedKey(VK_SHIFT) &&
		!Input::GetInstance().PressedKey(VK_ESCAPE);
		} });

	// Move ���¿��� Attack ���·� �������� ����
	playerFSM.transitions[PlayerState::Move].push_back({ PlayerState::Attack,
		[]() {return Input::GetInstance().PressedKey('Z'); } });

	// Move ���¿��� SpecialAttack ���·� �������� ����
	playerFSM.transitions[PlayerState::Move].push_back({ PlayerState::SpecialAttack,
		[]() {return Input::GetInstance().PressedKey('X'); } });

	// Move ���¿��� Interaction ���·� �������� ����
	playerFSM.transitions[PlayerState::Move].push_back({ PlayerState::Interaction,
		[]() {return Input::GetInstance().PressedKey('C'); } });


	// Attack------------------------------------------------------------------------------
	// Attack ���¿��� Idle ���·� �������� ����
	playerFSM.transitions[PlayerState::Attack].push_back({ PlayerState::Idle,
		[]() {return !Input::GetInstance().PressedKey('Z'); } });


	// SpecialAttack-----------------------------------------------------------------------
	// SpecialAttack ���¿��� Idle ���·� �������� ����
	playerFSM.transitions[PlayerState::SpecialAttack].push_back({ PlayerState::Idle,
		[]() {return !Input::GetInstance().PressedKey('X'); } });


	// Interaction-------------------------------------------------------------------------
	// Interaction ���¿��� Idle ���·� �������� ����
	playerFSM.transitions[PlayerState::Interaction].push_back({ PlayerState::Idle,
		[]() {return !Input::GetInstance().PressedKey('C'); } });

	// ���� ����-----------------------------------------------------------
	// Idle ���·� ���� ���� ��
	playerFSM.engageAction[PlayerState::Idle] = [this]() {IdleEngage(); };

	// Move ���·� ���� ���� ��
	playerFSM.engageAction[PlayerState::Move] = [this]() {MoveEngage(); };

	// Attack ���·� ���� ���� ��
	playerFSM.engageAction[PlayerState::Attack] = [this]() {AttackEngage(); };

	// SpecialAttack ���·� ���� ���� ��
	playerFSM.engageAction[PlayerState::SpecialAttack] = [this]() {SpecialAttackEngage(); };

	// Interaction ���·� ���� ���� ��
	playerFSM.engageAction[PlayerState::Interaction] = [this]() {InteractionEngage(); };

	// ���� ����-----------------------------------------------------------
	// Idle ���°� ���� ���� ��
	playerFSM.updateAction[PlayerState::Idle] = [this]() {IdleState(); };

	// Move ���°� ���� ���� ��
	playerFSM.updateAction[PlayerState::Move] = [this]() {MoveState(); };

	// Attack ���°� ���� ���� ��
	playerFSM.updateAction[PlayerState::Attack] = [this]() {AttackState(); };

	// SpecialAttack ���°� ���� ���� ��
	playerFSM.updateAction[PlayerState::SpecialAttack] = [this]() {SpecialAttackState(); };

	// Interaction ���°� ���� ���� ��
	playerFSM.updateAction[PlayerState::Interaction] = [this]() {InteractionState(); };
}

void NDComponent::Update()
{
	// ���¸� ������Ʈ ��Ų��.
	playerFSM.UpdateState();

}

void NDComponent::IdleEngage()
{

}

void NDComponent::MoveEngage()
{
	MoveEvent();
}

void NDComponent::AttackEngage()
{

}

void NDComponent::SpecialAttackEngage()
{

}

void NDComponent::InteractionEngage()
{

}

void NDComponent::IdleState()
{

}

void NDComponent::MoveState()
{
	MoveEvent();
}

void NDComponent::AttackState()
{

}

void NDComponent::SpecialAttackState()
{

}

void NDComponent::InteractionState()
{

}

void NDComponent::MoveEvent()
{
	// ���� Ű���� ���Ϳ� �����Ѵ�.
	Vector3D inputMove = Vector3D::zero;

	// �÷��̾��� ������ �������� �����´�.
	Vector3D moveDelta = playerAxis->GetTransform()->GetLocalPosition();

	// ��ŸŸ���� �����´�.
	float deltaTime = TimeController::GetInstance().GetDeltaTime();

	// �÷��̾��� ���鿡 ���� ���͸� �����´�.
	Vector3D playerFront = GetGameObject()->GetTransform()->GetFront();

	// �����ð��� ��ŸŸ���� ��� �����ش�.
	dashChargingTime += deltaTime;

	// Ű�� �Է��� �޾Ƽ� inpputMove ���Ϳ� �����Ѵ�.
	if (Input::GetInstance().PressedKey(VK_UP))
	{
		inputMove += (Vector3D::front);
		moveDelta += (Vector3D::front)*moveSpeed * deltaTime;
	}
	if (Input::GetInstance().PressedKey(VK_DOWN))
	{
		inputMove -= (Vector3D::front);
		moveDelta -= (Vector3D::front)*moveSpeed * deltaTime;
	}
	if (Input::GetInstance().PressedKey(VK_RIGHT))
	{
		inputMove += (Vector3D::right);
		moveDelta += (Vector3D::right)*moveSpeed * deltaTime;
	}
	if (Input::GetInstance().PressedKey(VK_LEFT))
	{

		inputMove -= (Vector3D::right);
		moveDelta -= (Vector3D::right)*moveSpeed * deltaTime;
	}

	// �뽬�� �Լ�
	if (Input::GetInstance().PressedKey(VK_SHIFT))
	{
		// �����ð� ����
		if (dashChargingTime > 2.0f)
		{
			// �����̰� ������ �뽬 ����
			if (inputMove != Vector3D::zero)
			{
				playerDotween->DOMove(GetGameObject()->GetTransform()->GetLocalPosition() + inputMove * moveSpeed * 2, 0.5).SetEase(EaseOutSine);
				dashChargingTime = 0.0f;
			}
			// �ƹ��� �Է��� ������ �뽬 ����
			else
			{
				playerDotween->DOMove(GetGameObject()->GetTransform()->GetLocalPosition() + playerFront * moveSpeed * 2, 0.5).SetEase(EaseOutSine);
				dashChargingTime = 0.0f;
			}
		}
	}


	// �� ���ͻ����� ���� �������� ���Ѵ�.
	float dotAngleFront = RadianToDegree(acos(DotProduct(playerFront, inputMove.Normalize())));

	// ȸ���� ���� ���
	if (dotAngleFront != 0 && inputMove != Vector3D::zero)
	{
		// �࿡ �����������, ���������� �׳� �����ϰ� �� ������ ������ ������Ų��.
		if (dotAngleFront < errorRange && -errorRange < dotAngleFront)
		{
			playerFront = inputMove.Normalize();
		}
		else
		{
			// ȸ�� ���ӵ��� ���� ȸ�������� Ŭ ���, �Է¹��� ���ͷ� ���� �����Ѵ�.
			if (dotAngleFront < (rotationAngleSpeed * deltaTime))
			{
				playerFront = inputMove.Normalize();
			}
			else
			{
				// ȸ������ ũ�⸦ ����Ͽ� ȸ���� ������ ���Ѵ�.
				if (GetRotationAngle(playerFront, inputMove.Normalize(), dotAngleFront) > 0)
				{
					GetGameObject()->GetTransform()->Rotate(GetGameObject()->GetTransform()->GetUp(), (rotationAngleSpeed * deltaTime));
				}
				else
				{
					GetGameObject()->GetTransform()->Rotate(GetGameObject()->GetTransform()->GetUp(), -(rotationAngleSpeed * deltaTime));
				}
			}
		}
	}
	else
	{
		playerFront = inputMove.Normalize();
	}

	OutputDebugString((L"Front : " + std::to_wstring(dotAngleFront) + L"\n").c_str());
	//OutputDebugString((L"Front : " + std::to_wstring(GetGameObject()->GetTransform()->GetFront().x) + L", ").c_str());
	//OutputDebugString((std::to_wstring(GetGameObject()->GetTransform()->GetFront().y) + L", ").c_str());
	//OutputDebugString((std::to_wstring(GetGameObject()->GetTransform()->GetFront().z) + L"\n").c_str());
	OutputDebugString((std::to_wstring(GetRotationAngle(playerFront, inputMove.Normalize(), dotAngleFront)) + L"\n").c_str());


	// �ٲ� ǥ���� ���� �־��ش�.
	GetGameObject()->GetTransform()->SetLocalPosition(GetGameObject()->GetTransform()->GetLocalPosition() + moveDelta);
}

// ȸ���� �ּҰ��� ���Ѵ�.
float NDComponent::GetRotationAngle(const Vector3D& local, const Vector3D& world, float angle)
{
	// ����
	Vector3D crossAxis = CrossProduct(local, world);
	if (crossAxis.y < 0)
	{
		angle = -angle;
	}
	return angle;
}

void NDComponent::PreSerialize(json& jsonData) const
{
	PRESERIALIZE_BASE(NDComponent);
}

void NDComponent::PreDeserialize(const json& jsonData)
{
	PREDESERIALIZE_BASE(NDComponent);
}

void NDComponent::PostSerialize(json& jsonData) const
{
	POSTSERIALIZE_BASE();
}

void NDComponent::PostDeserialize(const json& jsonData)
{
	POSTDESERIALIZE_BASE();
}
