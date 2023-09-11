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
	// 플레이어의 월드축 좌표를 보존하기위한 텅빈 부모 오브젝트 생성
	if (playerAxis == nullptr)
	{
		playerAxis->GetTransform()->SetLocalPosition(GetGameObject()->GetTransform()->GetLocalPosition());
		playerAxis->GetTransform()->SetLocalRotation(GetGameObject()->GetTransform()->GetLocalRotation());
	}
	GetGameObject()->GetTransform()->SetParent(playerAxis->GetTransform());
	GetGameObject()->GetTransform()->AddChild(playerfrontObject->GetTransform());
	playerfrontObject->GetTransform()->SetLocalScale(Vector3D(0.1, 0.1, 0.1));
	playerfrontObject->GetTransform()->SetLocalPosition(Vector3D(0, 0, 1));

	// 플레이어의 앞쪽 표시
	MeshRenderer* mesh = playerfrontObject->AddComponent<MeshRenderer>();
	mesh->SetMesh(L"Cube");
	mesh->SetMaterial(L"BoundaryDebug");

	// 닷트윈 추가
	playerDotween = GetGameObject()->AddComponent<Dotween>();

	// Idle--------------------------------------------------------------------------------
	// Idle 상태에서 Move 상태로 가기위한 조건
	playerFSM.transitions[PlayerState::Idle].push_back({ PlayerState::Move,
		[]() {return Input::GetInstance().PressedKey(VK_UP) ||
		Input::GetInstance().PressedKey(VK_DOWN) ||
		Input::GetInstance().PressedKey(VK_LEFT) ||
		Input::GetInstance().PressedKey(VK_RIGHT) ||
		Input::GetInstance().PressedKey(VK_SHIFT) ||
		Input::GetInstance().PressedKey(VK_ESCAPE);
		} });

	// Idle 상태에서 Attack 상태로 가기위한 조건
	playerFSM.transitions[PlayerState::Idle].push_back({ PlayerState::Attack,
		[]() {return Input::GetInstance().PressedKey('Z'); } });

	// Idle 상태에서 SpecialAttack 상태로 가기위한 조건
	playerFSM.transitions[PlayerState::Idle].push_back({ PlayerState::SpecialAttack,
		[]() {return Input::GetInstance().PressedKey('X'); } });

	// Idle 상태에서 Interaction 상태로 가기위한 조건
	playerFSM.transitions[PlayerState::Idle].push_back({ PlayerState::Interaction,
		[]() {return Input::GetInstance().PressedKey('C'); } });


	// Move--------------------------------------------------------------------------------
	// Move 상태에서 Idle 상태로 가기위한 조건
	playerFSM.transitions[PlayerState::Move].push_back({ PlayerState::Idle,
		[]() {return !Input::GetInstance().PressedKey(VK_UP) &&
		!Input::GetInstance().PressedKey(VK_DOWN) &&
		!Input::GetInstance().PressedKey(VK_LEFT) &&
		!Input::GetInstance().PressedKey(VK_RIGHT) &&
		!Input::GetInstance().PressedKey(VK_SHIFT) &&
		!Input::GetInstance().PressedKey(VK_ESCAPE);
		} });

	// Move 상태에서 Attack 상태로 가기위한 조건
	playerFSM.transitions[PlayerState::Move].push_back({ PlayerState::Attack,
		[]() {return Input::GetInstance().PressedKey('Z'); } });

	// Move 상태에서 SpecialAttack 상태로 가기위한 조건
	playerFSM.transitions[PlayerState::Move].push_back({ PlayerState::SpecialAttack,
		[]() {return Input::GetInstance().PressedKey('X'); } });

	// Move 상태에서 Interaction 상태로 가기위한 조건
	playerFSM.transitions[PlayerState::Move].push_back({ PlayerState::Interaction,
		[]() {return Input::GetInstance().PressedKey('C'); } });


	// Attack------------------------------------------------------------------------------
	// Attack 상태에서 Idle 상태로 가기위한 조건
	playerFSM.transitions[PlayerState::Attack].push_back({ PlayerState::Idle,
		[]() {return !Input::GetInstance().PressedKey('Z'); } });


	// SpecialAttack-----------------------------------------------------------------------
	// SpecialAttack 상태에서 Idle 상태로 가기위한 조건
	playerFSM.transitions[PlayerState::SpecialAttack].push_back({ PlayerState::Idle,
		[]() {return !Input::GetInstance().PressedKey('X'); } });


	// Interaction-------------------------------------------------------------------------
	// Interaction 상태에서 Idle 상태로 가기위한 조건
	playerFSM.transitions[PlayerState::Interaction].push_back({ PlayerState::Idle,
		[]() {return !Input::GetInstance().PressedKey('C'); } });

	// 상태 진입-----------------------------------------------------------
	// Idle 상태로 진입 했을 때
	playerFSM.engageAction[PlayerState::Idle] = [this]() {IdleEngage(); };

	// Move 상태로 진입 했을 때
	playerFSM.engageAction[PlayerState::Move] = [this]() {MoveEngage(); };

	// Attack 상태로 진입 했을 때
	playerFSM.engageAction[PlayerState::Attack] = [this]() {AttackEngage(); };

	// SpecialAttack 상태로 진입 했을 때
	playerFSM.engageAction[PlayerState::SpecialAttack] = [this]() {SpecialAttackEngage(); };

	// Interaction 상태로 진입 했을 때
	playerFSM.engageAction[PlayerState::Interaction] = [this]() {InteractionEngage(); };

	// 상태 유지-----------------------------------------------------------
	// Idle 상태가 유지 중일 때
	playerFSM.updateAction[PlayerState::Idle] = [this]() {IdleState(); };

	// Move 상태가 유지 중일 때
	playerFSM.updateAction[PlayerState::Move] = [this]() {MoveState(); };

	// Attack 상태가 유지 중일 때
	playerFSM.updateAction[PlayerState::Attack] = [this]() {AttackState(); };

	// SpecialAttack 상태가 유지 중일 때
	playerFSM.updateAction[PlayerState::SpecialAttack] = [this]() {SpecialAttackState(); };

	// Interaction 상태가 유지 중일 때
	playerFSM.updateAction[PlayerState::Interaction] = [this]() {InteractionState(); };
}

void NDComponent::Update()
{
	// 상태를 업데이트 시킨다.
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
	// 받은 키값을 벡터에 저장한다.
	Vector3D inputMove = Vector3D::zero;

	// 플레이어의 월드축 포지션을 가져온다.
	Vector3D moveDelta = playerAxis->GetTransform()->GetLocalPosition();

	// 델타타임을 가져온다.
	float deltaTime = TimeController::GetInstance().GetDeltaTime();

	// 플레이어의 정면에 대한 벡터를 가져온다.
	Vector3D playerFront = GetGameObject()->GetTransform()->GetFront();

	// 충전시간에 델타타임을 계속 더해준다.
	dashChargingTime += deltaTime;

	// 키의 입력을 받아서 inpputMove 벡터에 저장한다.
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

	// 대쉬기 함수
	if (Input::GetInstance().PressedKey(VK_SHIFT))
	{
		// 충전시간 조건
		if (dashChargingTime > 2.0f)
		{
			// 움직이고 있을때 대쉬 조건
			if (inputMove != Vector3D::zero)
			{
				playerDotween->DOMove(GetGameObject()->GetTransform()->GetLocalPosition() + inputMove * moveSpeed * 2, 0.5).SetEase(EaseOutSine);
				dashChargingTime = 0.0f;
			}
			// 아무런 입력이 없을때 대쉬 조건
			else
			{
				playerDotween->DOMove(GetGameObject()->GetTransform()->GetLocalPosition() + playerFront * moveSpeed * 2, 0.5).SetEase(EaseOutSine);
				dashChargingTime = 0.0f;
			}
		}
	}


	// 두 벡터사이의 각을 라디안으로 구한다.
	float dotAngleFront = RadianToDegree(acos(DotProduct(playerFront, inputMove.Normalize())));

	// 회전에 대한 계산
	if (dotAngleFront != 0 && inputMove != Vector3D::zero)
	{
		// 축에 가까워졌을때, 오차범위는 그냥 무시하고 그 축으로 정면을 고정시킨다.
		if (dotAngleFront < errorRange && -errorRange < dotAngleFront)
		{
			playerFront = inputMove.Normalize();
		}
		else
		{
			// 회전 각속도가 남은 회전각보다 클 경우, 입력받은 벡터로 축을 보정한다.
			if (dotAngleFront < (rotationAngleSpeed * deltaTime))
			{
				playerFront = inputMove.Normalize();
			}
			else
			{
				// 회전각의 크기를 계산하여 회전할 방향을 정한다.
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


	// 바뀐 표지션 값을 넣어준다.
	GetGameObject()->GetTransform()->SetLocalPosition(GetGameObject()->GetTransform()->GetLocalPosition() + moveDelta);
}

// 회전할 최소각을 구한다.
float NDComponent::GetRotationAngle(const Vector3D& local, const Vector3D& world, float angle)
{
	// 외적
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
