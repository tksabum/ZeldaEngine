#include "Player.h"
#include "Dotween.h"
#include "EdgeDetect.h"
#include "PlayerCollisionManager.h"
#include "GameManager.h"
#include "Boss1.h"

Player::Player()
	:
	moveSpeed(12.0f),
	hasteSpeed(1.5f),
	rotationAngleSpeed(30.0f),
	playerHp(100.0f), playerMaxHp(100.0f),
	playerAp(1.0f), playerSpecialAp(1.0f),
	dashSpeed(1.0f * moveSpeed), dashChargingTime(1.0f), dashCoolTime(1.0f), dashElapsed(0.0f), dashDuration(0.5),
	normalAttackTime(1.0f), normalAttackCoolTime(1.0f),
	specialAttackChargingTime(3.0f), spacialAttackCoolTime(3.0f),
	invincibleTime(2.0f), invincibleCoolTime(2.0f),
	hasteTime(3.0f), hasteCoolTime(3.0f),
	stunTime(0.3f), stunPushTime(0.5f), stunCoolTime(0.3f), stunPushCoolTime(0.5f),
	force(13.0f),
	isInvincible(false), isDashInvincible(false), isHaste(false), isDamaged(false), isPush(false), isStop(true),
	inputMove(Vector3D::zero),
	moveUp(VK_UP), moveDown(VK_DOWN), moveLeft(VK_LEFT), moveRight(VK_RIGHT), dashKey(VK_LSHIFT), attackKey((int)'Z'), specialAttackKey((int)'X'),
	bossTransform(nullptr), m_animator(nullptr), playerSkinnedMeshRenderer(nullptr), playerRigidbody(nullptr), playerCollider(nullptr), playerAnimTransform(nullptr), bossAnimTransform(nullptr), playerDamagedSpriteAnimator(nullptr), playerAttackEffect(nullptr), playerSpecialAttackEffect(nullptr)
{
	
}

void Player::Awake()
{
	playerAxis = SceneManager::GetInstance().CreateGameObject(L"playerAxis");
	playerAttackAxis = SceneManager::GetInstance().CreateGameObject(L"playerAttackAxis");
	playerBasicAttack = SceneManager::GetInstance().CreateGameObject(L"playerBasicAttack");
	playerSpecialAttack = SceneManager::GetInstance().CreateGameObject(L"playerSpecialAttackRange");

	GetGameObject()->GetTransform()->SetLocalRotation(Vector3D(0, 180, 0));
	// 변수
	attackDelay = 0.1f;
	AttackMotionDelay = 0.4f;
	SpecialAttackMotionDelay = 0.5f;
	errorRange = 0.75f;

	playerHp = playerMaxHp;

	hasteTime = hasteCoolTime;
	dashChargingTime = dashCoolTime;
	

	GetGameObject()->GetTransform()->SetLocalPosition(Vector3D(GetGameObject()->GetTransform()->GetLocalPosition().x, 3, GetGameObject()->GetTransform()->GetLocalPosition().z));
	if (GetGameObject()->GetTransform()->GetChild(L"player2DHitOjbect") != nullptr)
	{
		playerEdge = GetGameObject()->GetTransform()->GetChild(L"player2DHitOjbect")->GetGameObject()->GetComponent<EdgeDetect>();
	}
}

void Player::Start()
{
	// 플레이어의 월드축 좌표를 보존하기위한 텅빈 부모 오브젝트 생성
	if (playerAxis == nullptr)
	{
		playerAxis->GetTransform()->SetLocalPosition(GetGameObject()->GetTransform()->GetLocalPosition());
		playerAxis->GetTransform()->SetLocalRotation(GetGameObject()->GetTransform()->GetLocalRotation());
	}

	GetGameObject()->GetTransform()->SetParent(playerAxis->GetTransform());

	// 플레이어의 상속을 받지않고, 정면을 가져오는 오브젝트
	playerAttackAxis->GetTransform()->SetLocalPosition(GetGameObject()->GetTransform()->GetFront());
	playerAttackAxis->GetTransform()->SetLocalRotation(GetGameObject()->GetTransform()->GetLocalRotation());
	playerAttackAxis->GetTransform()->AddChild(playerSpecialAttack->GetTransform());
	playerAttackAxis->GetTransform()->AddChild(playerBasicAttack->GetTransform());

	// 일반 공격 범위 생성

	playerBasicAttack->SetActive(false);
	basicAttackCheck = playerBasicAttack->AddComponent<CubeCollider>();
	basicAttackCheck->SetTrigger(true);
	playerBasicAttack->SetLayerName(L"BasicAttack");
	playerBasicAttack->GetTransform()->SetLocalScale(Vector3D(8, 10, 7));
	playerBasicAttack->GetTransform()->SetLocalPosition(playerAttackAxis->GetTransform()->GetLocalPosition() + Vector3D(0, 0, 3.5f));

	// 특수 공격 범위 생성
	playerSpecialAttack->SetActive(false);
	specialAttackCheck = playerSpecialAttack->AddComponent<CubeCollider>();
	specialAttackCheck->SetTrigger(true);
	playerSpecialAttack->SetLayerName(L"RangedAttack");
	playerSpecialAttack->GetTransform()->SetLocalScale(Vector3D(8, 10, 12));
	playerSpecialAttack->GetTransform()->SetLocalPosition(playerAttackAxis->GetTransform()->GetLocalPosition() + Vector3D(0, 0, 6.0f));



	// 닷트윈 추가
	playerDotween = GetGameObject()->AddComponent<Dotween>();


	// Collider 충돌처리 컴포넌트 넣기
	playerBasicAttack->AddComponent<PlayerCollisionManager>();
	playerBasicAttack->GetComponent<PlayerCollisionManager>()->SetBossComponent(bossComponent);
	playerBasicAttack->GetComponent<PlayerCollisionManager>()->SetDamage(playerAp);
	playerSpecialAttack->AddComponent<PlayerCollisionManager>();
	playerSpecialAttack->GetComponent<PlayerCollisionManager>()->SetBossComponent(bossComponent);
	playerSpecialAttack->GetComponent<PlayerCollisionManager>()->SetDamage(playerSpecialAp);


	/// Idle-------------------------------------------------------------------------------------------------------------------------
	// Idle 상태에서 Move 상태로 가기위한 조건
	playerFSM.transitions[PlayerState::Idle].push_back({ PlayerState::Move,
		[this]() {return Input::GetInstance().PressedKey(moveUp) ||
		Input::GetInstance().PressedKey(moveDown) ||
		Input::GetInstance().PressedKey(moveLeft) ||
		Input::GetInstance().PressedKey(moveRight);
		} });
	// Idle 상태에서 Dash 상태로 가기위한 조건
	playerFSM.transitions[PlayerState::Idle].push_back({ PlayerState::Dash,
		[this]() {return Input::GetInstance().PressedKey(dashKey) && dashChargingTime >= dashCoolTime; } });
	// Idle 상태에서 Attack 상태로 가기위한 조건
	playerFSM.transitions[PlayerState::Idle].push_back({ PlayerState::Attack,
		[this]() {return Input::GetInstance().PressedKey(attackKey) && normalAttackTime >= normalAttackCoolTime; } });
	// Idle 상태에서 SpecialAttack 상태로 가기위한 조건
	playerFSM.transitions[PlayerState::Idle].push_back({ PlayerState::SpecialAttack,
		[this]() {return Input::GetInstance().PressedKey(specialAttackKey) &&
		specialAttackChargingTime >= spacialAttackCoolTime; } });
	// Idle 상태에서 Dead 상태로 가기위한 조건
	playerFSM.transitions[PlayerState::Idle].push_back({ PlayerState::Dead,
		[this]() {return playerHp <= 0; } });
	// Idle 상태에서 Stun 상태로 가기위한 조건
	playerFSM.transitions[PlayerState::Idle].push_back({ PlayerState::Stun,
		[this]() {return isDamaged == true; } });
	// Idle 상태에서 Fall 상태로 가기위한 조건
	playerFSM.transitions[PlayerState::Idle].push_back({ PlayerState::Fall,
		[this]() {return (GetGameObject()->GetTransform()->GetLocalPosition().y < -1) && (SceneManager::GetInstance().GetName() != L"IntroScene"); } });

	/// Move-------------------------------------------------------------------------------------------------------------------------
	// Move 상태에서 Idle 상태로 가기위한 조건
	playerFSM.transitions[PlayerState::Move].push_back({ PlayerState::Idle,
		[this]() {return !Input::GetInstance().PressedKey(moveUp) &&
		!Input::GetInstance().PressedKey(moveDown) &&
		!Input::GetInstance().PressedKey(moveLeft) &&
		!Input::GetInstance().PressedKey(moveRight);
		} });
	// Move 상태에서 Dash 상태로 가기위한 조건
	playerFSM.transitions[PlayerState::Move].push_back({ PlayerState::Dash,
		[this]() {return Input::GetInstance().PressedKey(dashKey) && dashChargingTime >= dashCoolTime; } });
	// Move 상태에서 Attack 상태로 가기위한 조건
	playerFSM.transitions[PlayerState::Move].push_back({ PlayerState::Attack,
		[this]() {return Input::GetInstance().PressedKey(attackKey) && normalAttackTime >= normalAttackCoolTime; } });
	// Move 상태에서 SpecialAttack 상태로 가기위한 조건
	playerFSM.transitions[PlayerState::Move].push_back({ PlayerState::SpecialAttack,
		[this]() {return Input::GetInstance().PressedKey(specialAttackKey) &&
		specialAttackChargingTime >= spacialAttackCoolTime; } });
	// Move 상태에서 Dead 상태로 가기위한 조건
	playerFSM.transitions[PlayerState::Move].push_back({ PlayerState::Dead,
		[this]() {return playerHp <= 0; } });
	// Move 상태에서 Stun 상태로 가기위한 조건
	playerFSM.transitions[PlayerState::Move].push_back({ PlayerState::Stun,
		[this]() {return isDamaged == true || isPush == true; } });
	// Move 상태에서 Fall 상태로 가기위한 조건
	playerFSM.transitions[PlayerState::Move].push_back({ PlayerState::Fall,
		[this]() {return (GetGameObject()->GetTransform()->GetLocalPosition().y < -1) && (SceneManager::GetInstance().GetName() != L"IntroScene"); } });

	/// Attack-----------------------------------------------------------------------------------------------------------------------
	// Attack 상태에서 Idle 상태로 가기위한 조건
	playerFSM.transitions[PlayerState::Attack].push_back({ PlayerState::Idle,
		[this]() {return normalAttackTime >= AttackMotionDelay; } });
	// Attack 상태에서 Dead 상태로 가기위한 조건z
	playerFSM.transitions[PlayerState::Attack].push_back({ PlayerState::Dead,
		[this]() {return playerHp <= 0; } });
	// Attack 상태에서 Stun 상태로 가기위한 조건
	playerFSM.transitions[PlayerState::Attack].push_back({ PlayerState::Stun,
		[this]() {return isDamaged == true || isPush == true; } });
	// Attack 상태에서 Fall 상태로 가기위한 조건
	playerFSM.transitions[PlayerState::Attack].push_back({ PlayerState::Fall,
		[this]() {return (GetGameObject()->GetTransform()->GetLocalPosition().y < -1) && (SceneManager::GetInstance().GetName() != L"IntroScene"); } });

	/// SpecialAttack----------------------------------------------------------------------------------------------------------------
	// SpecialAttack 상태에서 Idle 상태로 가기위한 조건
	playerFSM.transitions[PlayerState::SpecialAttack].push_back({ PlayerState::Idle,
		[this]() {return specialAttackChargingTime >= SpecialAttackMotionDelay; } });
	// SpecialAttack 상태에서 Dead 상태로 가기위한 조건
	playerFSM.transitions[PlayerState::SpecialAttack].push_back({ PlayerState::Dead,
		[this]() {return playerHp <= 0; } });
	// SpecialAttack 상태에서 Stun 상태로 가기위한 조건
	playerFSM.transitions[PlayerState::SpecialAttack].push_back({ PlayerState::Stun,
		[this]() {return isDamaged == true || isPush == true; } });
	// SpecialAttack 상태에서 Fall 상태로 가기위한 조건
	playerFSM.transitions[PlayerState::SpecialAttack].push_back({ PlayerState::Fall,
		[this]() {return (GetGameObject()->GetTransform()->GetLocalPosition().y < -1) && (SceneManager::GetInstance().GetName() != L"IntroScene"); } });

	/// Interaction------------------------------------------------------------------------------------------------------------------


	/// Stun-------------------------------------------------------------------------------------------------------------------------
	// Stun 상태에서 Invincible 상태로 가기위한 조건
	playerFSM.transitions[PlayerState::Stun].push_back({ PlayerState::Idle,
		[this]() {return stunTime >= stunCoolTime && stunPushTime >= stunPushCoolTime && isStop == true; } });
	// Stun 상태에서 Fall 상태로 가기위한 조건
	playerFSM.transitions[PlayerState::Stun].push_back({ PlayerState::Fall,
		[this]() {return (GetGameObject()->GetTransform()->GetLocalPosition().y < -1) && (SceneManager::GetInstance().GetName() != L"IntroScene"); } });

	/// Invincible-------------------------------------------------------------------------------------------------------------------


	/// Dash-------------------------------------------------------------------------------------------------------------------------
	// Dash 상태에서 Idle 상태로 가기위한 조건
	playerFSM.transitions[PlayerState::Dash].push_back({ PlayerState::Idle,
		[this]() {return dashElapsed >= dashDuration; } });
	// Stun 상태에서 Fall 상태로 가기위한 조건
	playerFSM.transitions[PlayerState::Dash].push_back({ PlayerState::Fall,
		[this]() {return (GetGameObject()->GetTransform()->GetLocalPosition().y < -1) && (SceneManager::GetInstance().GetName() != L"IntroScene"); } });

	/// 상태 진입---------------------------------------------------------------------------------------------------------------------
	// Idle 상태로 진입 했을 때
	playerFSM.engageAction[PlayerState::Idle] = [this]() {IdleEngage(); };
	// Move 상태로 진입 했을 때
	playerFSM.engageAction[PlayerState::Move] = [this]() {MoveEngage(); };
	// Dash 상태로 진입 했을 때
	playerFSM.engageAction[PlayerState::Dash] = [this]() {DashEngage(); };
	// Attack 상태로 진입 했을 때
	playerFSM.engageAction[PlayerState::Attack] = [this]() {AttackEngage(); };
	// SpecialAttack 상태로 진입 했을 때
	playerFSM.engageAction[PlayerState::SpecialAttack] = [this]() {SpecialAttackEngage(); };
	// Stun 상태로 진입 했을 때
	playerFSM.engageAction[PlayerState::Stun] = [this]() {StunEngage(); };
	// Dead 상태로 진입 했을 때
	playerFSM.engageAction[PlayerState::Dead] = [this]() {DeadEngage(); };
	// Fall 상태로 진입 했을 때
	playerFSM.engageAction[PlayerState::Fall] = [this]() {FallEngage(); };

	/// 상태 유지---------------------------------------------------------------------------------------------------------------------
	// Idle 상태가 유지 중일 때
	playerFSM.updateAction[PlayerState::Idle] = [this]() {IdleState(); };
	// Move 상태가 유지 중일 때
	playerFSM.updateAction[PlayerState::Move] = [this]() {MoveState(); };
	// Dash 상태가 유지 중일 때
	playerFSM.updateAction[PlayerState::Dash] = [this]() {DashState(); };
	// Attack 상태가 유지 중일 때
	playerFSM.updateAction[PlayerState::Attack] = [this]() {/*AttackState();*/ };
	// SpecialAttack 상태가 유지 중일 때
	playerFSM.updateAction[PlayerState::SpecialAttack] = [this]() {SpecialAttackState(); };
	// Stun 상태로 진입 했을 때
	playerFSM.updateAction[PlayerState::Stun] = [this]() {StunState(); };
	// Dead 상태로 진입 했을 때
	playerFSM.updateAction[PlayerState::Dead] = [this]() {DeadState(); };
	// Fall 상태로 진입 했을 때
	playerFSM.updateAction[PlayerState::Fall] = [this]() {};
}

void Player::Update()
{
	if (SceneManager::GetInstance().GetName() == L"IntroScene")
	{
		playerSkinnedMeshRenderer->GetMaterialInfo(0).vec4Params[0].x = 1;
	}
	else if (SceneManager::GetInstance().GetName() == L"GameScene")
	{
		playerSkinnedMeshRenderer->GetMaterialInfo(0).vec4Params[0].x = 0;
	}
	\
		// 상태를 업데이트 시킨다.
		playerFSM.UpdateState();

	// 델타타임을 가져온다.
	float deltaTime = TimeController::GetInstance().GetDeltaTime();

	// 쿨타임 시간을 업데이트 시킨다.
	TimeEvent(deltaTime);
}


void Player::IdleEngage()
{
	isDashInvincible = false;
	m_animator->Play(L"Armature|idle");
}

void Player::MoveEngage()
{
	SoundManager::GetInstance().PlaySFX(SoundManager::Player::Walk);
	m_animator->Play(L"Armature|running");
	MoveEvent();
}

void Player::DashEngage()
{
	SoundManager::GetInstance().PlaySFX(SoundManager::Player::Dash);
	isDashInvincible = true;
	m_animator->Play(L"Armature|dash");
	DashEngageEvent();
}

void Player::AttackEngage()
{
	if (normalAttackTime >= normalAttackCoolTime)
	{
		SoundManager::GetInstance().PlaySFX(SoundManager::Player::BasicAttack);
		playerAttackEffect->SetActive(true);
		m_animator->Play(L"Armature|attack");
	}
	AttackEvent();
}

void Player::SpecialAttackEngage()
{
	SoundManager::GetInstance().PlaySFX(SoundManager::Player::SpecialAttack);
	playerSpecialAttackEffect->SetActive(true);
	m_animator->Play(L"Armature|attack2");
	SpecialAttackEvent();
}

void Player::StunEngage()
{
	SoundManager::GetInstance().PlaySFX(SoundManager::Player::Damaged);
	m_animator->Play(L"Armature|damage");
	StunEvent();
}

void Player::DeadEngage()
{
	SoundManager::GetInstance().PlaySFX(SoundManager::Player::Dead);
	m_animator->Play(L"Armature|down");
	EdgeOff();
	bossComponent->EdgeOff();
	gameManager->ShowGameOverScene();
}

void Player::FallEngage()
{

	FallEvent();
}

void Player::IdleState()
{

}

void Player::MoveState()
{
	MoveEvent();
}

void Player::DashState()
{
	DashStateEvent();
}

void Player::AttackState()
{
	AttackEvent();
}

void Player::SpecialAttackState()
{
	SpecialAttackEvent();
}

void Player::StunState()
{
	float stunPushVector =
		Vector3D(playerRigidbody->GetVelocity().x + playerRigidbody->GetAcceleration().x, 0, playerRigidbody->GetVelocity().z + playerRigidbody->GetAcceleration().z).Size();
	if (stunPushVector < 1.5f)
	{
		isStop = true;
	}
}

void Player::DeadState()
{

}

void Player::FallState()
{
	FallEvent();
}

void Player::MoveEvent()
{

	// 받은 키값을 벡터에 저장한다.
	inputMove = Vector3D::zero;


	// 플레이어의 월드축 포지션을 가져온다.
	Vector3D moveDelta = playerAxis->GetTransform()->GetLocalPosition();

	// 델타타임을 가져온다.
	float deltaTime = TimeController::GetInstance().GetDeltaTime();

	// 플레이어의 정면에 대한 벡터를 가져온다.
	Vector3D playerFront = GetGameObject()->GetTransform()->GetFront();

	// 키의 입력을 받아서 inpputMove 벡터에 저장한다.
	if (Input::GetInstance().PressedKey(moveUp))
	{
		inputMove -= (Vector3D::front);
		moveDelta -= (Vector3D::front)*moveSpeed * deltaTime;
	}
	if (Input::GetInstance().PressedKey(moveDown))
	{
		inputMove += (Vector3D::front);
		moveDelta += (Vector3D::front)*moveSpeed * deltaTime;
	}
	if (Input::GetInstance().PressedKey(moveLeft))
	{
		inputMove += (Vector3D::right);
		moveDelta += (Vector3D::right)*moveSpeed * deltaTime;
	}
	if (Input::GetInstance().PressedKey(moveRight))
	{
		inputMove -= (Vector3D::right);
		moveDelta -= (Vector3D::right)*moveSpeed * deltaTime;
	}


	// 두 벡터사이의 각을 라디안으로 구한다.
	float dotAngleFront = RadianToDegree(acos(DotProduct(playerFront, inputMove.Normalize())));

	// 회전에 대한 계산
	if (dotAngleFront != 0 && inputMove != Vector3D::zero)
	{
		// 축에 가까워졌을때, 오차범위는 그냥 무시하고 그 축으로 정면을 고정시킨다.
		if (!(dotAngleFront < errorRange && -errorRange < dotAngleFront))
		{
			// 회전 각속도가 남은 회전각보다 클 경우, 입력받은 벡터로 축을 보정한다.
			if (dotAngleFront < RadianToDegree((rotationAngleSpeed * deltaTime)))
			{
				// 회전각의 크기를 계산하여 회전할 방향을 정한다.
				if (GetRotationAngle(playerFront, inputMove.Normalize(), dotAngleFront) > 0)
				{
					GetGameObject()->GetTransform()->Rotate(GetGameObject()->GetTransform()->GetUp(), DegreeToRadian(dotAngleFront));
				}
				else
				{
					GetGameObject()->GetTransform()->Rotate(GetGameObject()->GetTransform()->GetUp(), -DegreeToRadian(dotAngleFront));
				}
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
	// 바뀐 표지션 값을 넣어준다.
	if (isHaste == false)
	{
		GetGameObject()->GetTransform()->SetLocalPosition(GetGameObject()->GetTransform()->GetLocalPosition() + moveDelta);
	}
	else if (isHaste == true)
	{
		GetGameObject()->GetTransform()->SetLocalPosition(GetGameObject()->GetTransform()->GetLocalPosition() + (moveDelta * hasteSpeed));
	}

}

void Player::DashEngageEvent()
{
	dashElapsed = 0.0f;
	dashChargingTime = 0.0f;
}

void Player::DashStateEvent()
{
	// 델타타임을 가져온다.
	float deltaTime = TimeController::GetInstance().GetDeltaTime();
	dashElapsed += deltaTime;

	float sinBeforeElapsed = easeInOutSine((dashElapsed - deltaTime) / dashDuration);
	float sinElapsed = easeInOutSine(dashElapsed / dashDuration);

	Vector3D distance = inputMove.Normalize() * dashSpeed;

	// 프레임당 움직일 벡터
	Vector3D moveDistance = distance * sinElapsed - distance * sinBeforeElapsed;

	if (dashElapsed <= dashDuration)
	{
		GetGameObject()->GetTransform()->SetLocalPosition(GetGameObject()->GetTransform()->GetLocalPosition() + moveDistance);
	}
}

void Player::AttackEvent()
{
	if (normalAttackTime >= normalAttackCoolTime)
	{
		normalAttackTime = 0;
		playerAttackAxis->GetTransform()->SetLocalPosition(GetGameObject()->GetTransform()->GetLocalPosition());
		playerAttackAxis->GetTransform()->SetLocalRotation(GetGameObject()->GetTransform()->GetLocalRotation());

		playerBasicAttack->SetActive(true);

		playerDotween->DONothing(attackDelay).OnComplete([=]()
			{
				playerBasicAttack->SetActive(false);
			});
	}
	// 연속공격 가능
	// 공격시 이동 불가능 (상태로 구현 완료)
	// 평타는 초당 1회
}

void Player::SpecialAttackEvent()
{
	// 쿨타임 10초
	// 바라보는 방향으로 공격

	if (specialAttackChargingTime >= spacialAttackCoolTime)
	{
		specialAttackChargingTime = 0.0f;
		playerAttackAxis->GetTransform()->SetLocalPosition(GetGameObject()->GetTransform()->GetLocalPosition());
		playerAttackAxis->GetTransform()->SetLocalRotation(GetGameObject()->GetTransform()->GetLocalRotation());
		playerSpecialAttack->SetActive(true);

		playerDotween->DONothing(attackDelay).OnComplete([=]()
			{
				playerSpecialAttack->SetActive(false);
				playerSpecialAttack->GetTransform()->SetLocalPosition(Vector3D(0, 0, 6.0f));
				playerSpecialAttack->GetTransform()->SetLocalRotation(Vector3D::zero);
			});
	}
}

void Player::StunEvent()
{
	if (isPush == true)
	{
		stunPushTime = 0.0f;
	}
	else
	{
		stunTime = 0.0f;
	}
	if (isDamaged == true)
	{
		isDamaged = false;
		hasteTime = 0.0f;
		isHaste = true;
	}
}

void Player::FallEvent()
{
	EdgeOff();
	bossComponent->EdgeOff();
	gameManager->ShowGameOverScene();
}

void Player::TimeEvent(float deltaTime)
{
	// 대쉬 시간
	if (dashChargingTime < dashCoolTime)
	{
		dashChargingTime += deltaTime;
	}
	if (dashChargingTime >= dashCoolTime)
	{
		dashChargingTime = dashCoolTime;
	}

	// 특수공격 시간
	if (specialAttackChargingTime < spacialAttackCoolTime)
	{
		specialAttackChargingTime += deltaTime;
	}
	if (specialAttackChargingTime >= spacialAttackCoolTime)
	{
		specialAttackChargingTime = spacialAttackCoolTime;
	}

	if (normalAttackTime < normalAttackCoolTime)
	{
		normalAttackTime += deltaTime;
	}
	if (normalAttackTime >= normalAttackCoolTime)
	{
		normalAttackTime = normalAttackCoolTime;
	}

	// 경직 시간
	if (stunTime < stunCoolTime)
	{
		stunTime += deltaTime;
	}
	if (stunTime >= stunCoolTime)
	{
		stunTime = stunCoolTime;
	}

	// 밀격 경직 시간
	if (stunPushTime < stunPushCoolTime)
	{
		stunPushTime += deltaTime;
	}
	if (stunPushTime >= stunPushCoolTime)
	{
		stunPushTime = stunPushCoolTime;
		isPush = false;
	}

	// 무적 시간
	if (invincibleTime < invincibleCoolTime)
	{
		invincibleTime += deltaTime;
	}
	if (invincibleTime >= invincibleCoolTime)
	{
		invincibleTime = invincibleCoolTime;
		isInvincible = false;
	}
	// 이속증가 시간
	if (hasteTime < hasteCoolTime)
	{
		hasteTime += deltaTime;
	}
	if (hasteTime >= hasteCoolTime)
	{
		hasteTime = hasteCoolTime;
		isHaste = false;
	}
}

// 회전할 최소각을 구한다.
float Player::GetRotationAngle(const Vector3D& local, const Vector3D& world, float angle)
{
	// 외적
	Vector3D crossAxis = CrossProduct(local, world);
	if (crossAxis.y < 0)
	{
		angle = -angle;
	}
	return angle;
}

void Player::AttackedDamage(float damage)
{
	if (playerHp >= damage)
	{
		playerHp -= damage;
	}
	else
	{
		playerHp = 0;
	}
}

float Player::easeInOutSine(float t)
{
	return -1 * (cos(PI * t) - 1) / 2;
}

void Player::OnCollisionEnter(Collider* other)
{
	if (other->GetGameObject()->GetLayerID() == 12)
	{
		//GetGameObject()->GetTransform()->SetLocalPosition(Vector3D(0, 1, 0));
		playerHp = 0;
	}
}

void Player::SetGameManager(GameManager* gameManager)
{
	this->gameManager = gameManager;
}

CapsuleCollider* Player::GetPlayerCollider()
{
	return playerCollider;
}

void Player::OnDamaged(float damage, int type)
{
	switch (static_cast<BossAttackType>(type))
	{
		case BossAttackType::NormalAttack:
		{
			if (isInvincible == false && isDashInvincible == false)
			{
				playerDamagedSpriteAnimator->GetGameObject()->GetTransform()->SetLocalPosition(GetGameObject()->GetTransform()->GetScreenPosition());
				playerDamagedSpriteAnimator->Play();
				AttackedDamage(damage);
				invincibleTime = 0.0f;
				isDamaged = true;
			}
			break;
		}

		case BossAttackType::PushAttack:
		{
			if (isInvincible == false && isDashInvincible == false)
			{
				playerDamagedSpriteAnimator->GetGameObject()->GetTransform()->SetLocalPosition(GetGameObject()->GetTransform()->GetScreenPosition());
				playerDamagedSpriteAnimator->Play();
				AttackedDamage(damage);
				invincibleTime = 0.0f;
				isDamaged = true;
				isPush = true;
				isStop = false;
				Vector3D forceVector = Vector3D(playerAnimTransform->GetWorldPosition().x - bossAnimTransform->GetWorldPosition().x, 0, playerAnimTransform->GetWorldPosition().z - bossAnimTransform->GetWorldPosition().z);

				playerRigidbody->ApplyForce(playerAnimTransform->GetWorldPosition(), forceVector.Normalize() * force);
			}
			break;
		}

		case BossAttackType::BarrierAttack:
		{
			isPush = true;
			isStop = false;
			AttackedDamage(damage);
			Vector3D forceVector = Vector3D(playerAnimTransform->GetWorldPosition().x - bossAnimTransform->GetWorldPosition().x, 0, playerAnimTransform->GetWorldPosition().z - bossAnimTransform->GetWorldPosition().z);
			playerRigidbody->ApplyForce(playerAnimTransform->GetWorldPosition(), forceVector.Normalize() * (force * 0.2));
		}
		break;
	}

	if (isDamaged == true)
	{
		isInvincible = true;
		playerSkinnedMeshRenderer->SetColor(ZeldaColor::Red);
		playerDotween->DONothing(1.0f).OnComplete([=]()
			{
				playerSkinnedMeshRenderer->SetColor(ZeldaColor::White);
			});
	}
}

float Player::GetPlayerHp()
{
	return playerHp;
}

float Player::GetPlayerMaxHp()
{
	return playerMaxHp;
}

float Player::GetDashChargingTime()
{
	return dashChargingTime;
}

float Player::GetDashCoolTime()
{
	return dashCoolTime;
}

float Player::GetSpecialAttackChargingTime()
{
	return specialAttackChargingTime;
}

float Player::GetSpecialAttackCoolTime()
{
	return spacialAttackCoolTime;
}

void Player::SetBossTransform(Transform* transform)
{
	bossTransform = transform;
}

void Player::SetBossCollider(CapsuleCollider* collider)
{
	bossCollider = collider;
}

void Player::SetAnimator(Animator* animator)
{
	m_animator = animator;
}

void Player::SetSkinnedMeshRenderer(SkinnedMeshRenderer* skinnedMeshRenderer)
{
	playerSkinnedMeshRenderer = skinnedMeshRenderer;
}

void Player::SetRigidBody(Rigidbody* rigidbody)
{
	playerRigidbody = rigidbody;
}

void Player::SetPlayerAnimTransform(Transform* transform)
{
	playerAnimTransform = transform;
}

void Player::SetBossAnimTransform(Transform* transform)
{
	bossAnimTransform = transform;
}

void Player::SetPlayerDamagedSpriteAnimator(SpriteAnimator* spriteanimator)
{
	playerDamagedSpriteAnimator = spriteanimator;
}

void Player::SetPlayerAttackEffect(GameObject* attackEffect)
{
	playerAttackEffect = attackEffect;
}

void Player::SetPlayerSpecialAttackEffect(GameObject* specialAttackEffect)
{
	playerSpecialAttackEffect = specialAttackEffect;
}

void Player::SetBossComponent(Boss1* boss)
{
	bossComponent = boss;
}

void Player::EdgeOff()
{
	playerEdge->SetUseCollision(false);
	playerEdge->OffEdge();
}

void Player::PreSerialize(json& jsonData) const
{
	PRESERIALIZE_BASE(Player);
	PRESERIALIZE_VALUE(moveSpeed);
	PRESERIALIZE_VALUE(hasteSpeed);
	PRESERIALIZE_VALUE(rotationAngleSpeed);
	PRESERIALIZE_VALUE(playerHp);
	PRESERIALIZE_VALUE(playerMaxHp);
	PRESERIALIZE_VALUE(dashSpeed);
	PRESERIALIZE_VALUE(dashCoolTime);
	PRESERIALIZE_VALUE(spacialAttackCoolTime);
	PRESERIALIZE_VALUE(invincibleCoolTime);
	PRESERIALIZE_VALUE(hasteCoolTime);
	PRESERIALIZE_VALUE(stunCoolTime);
	PRESERIALIZE_VALUE(stunPushCoolTime);
	PRESERIALIZE_VALUE(force);
}

void Player::PreDeserialize(const json& jsonData)
{
	PREDESERIALIZE_BASE(Player);
	PREDESERIALIZE_VALUE(moveSpeed);
	PREDESERIALIZE_VALUE(hasteSpeed);
	PREDESERIALIZE_VALUE(rotationAngleSpeed);
	PREDESERIALIZE_VALUE(playerHp);
	PREDESERIALIZE_VALUE(playerMaxHp);
	PREDESERIALIZE_VALUE(dashSpeed);
	PREDESERIALIZE_VALUE(dashCoolTime);
	PREDESERIALIZE_VALUE(spacialAttackCoolTime);
	PREDESERIALIZE_VALUE(invincibleCoolTime);
	PREDESERIALIZE_VALUE(hasteCoolTime);
	PREDESERIALIZE_VALUE(stunCoolTime);
	PREDESERIALIZE_VALUE(stunPushCoolTime);
	PREDESERIALIZE_VALUE(force);
}

void Player::PostSerialize(json& jsonData) const
{
	POSTSERIALIZE_BASE();
	POSTSERIALIZE_PTR(bossTransform);
	POSTSERIALIZE_PTR(m_animator);
	POSTSERIALIZE_PTR(playerSkinnedMeshRenderer);
	POSTSERIALIZE_PTR(playerRigidbody);
	POSTSERIALIZE_PTR(playerCollider);
	POSTSERIALIZE_PTR(playerAnimTransform);
	POSTSERIALIZE_PTR(bossAnimTransform);
	POSTSERIALIZE_PTR(bossCollider);
	POSTSERIALIZE_PTR(playerDamagedSpriteAnimator);
	POSTSERIALIZE_PTR(playerAttackEffect);
	POSTSERIALIZE_PTR(playerSpecialAttackEffect);
	POSTSERIALIZE_PTR(bossComponent);
	POSTSERIALIZE_PTR(gameManager);
}

void Player::PostDeserialize(const json& jsonData)
{
	POSTDESERIALIZE_BASE();
	POSTDESERIALIZE_PTR(bossTransform);
	POSTDESERIALIZE_PTR(m_animator);
	POSTDESERIALIZE_PTR(playerSkinnedMeshRenderer);
	POSTDESERIALIZE_PTR(playerRigidbody);
	POSTDESERIALIZE_PTR(playerCollider);
	POSTDESERIALIZE_PTR(playerAnimTransform);
	POSTDESERIALIZE_PTR(bossAnimTransform);
	POSTDESERIALIZE_PTR(bossCollider);
	POSTDESERIALIZE_PTR(playerDamagedSpriteAnimator);
	POSTDESERIALIZE_PTR(playerAttackEffect);
	POSTDESERIALIZE_PTR(playerSpecialAttackEffect);
	POSTDESERIALIZE_PTR(bossComponent);
	POSTDESERIALIZE_PTR(gameManager);
}
