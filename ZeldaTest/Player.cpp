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
	// ����
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
	// �÷��̾��� ������ ��ǥ�� �����ϱ����� �ֺ� �θ� ������Ʈ ����
	if (playerAxis == nullptr)
	{
		playerAxis->GetTransform()->SetLocalPosition(GetGameObject()->GetTransform()->GetLocalPosition());
		playerAxis->GetTransform()->SetLocalRotation(GetGameObject()->GetTransform()->GetLocalRotation());
	}

	GetGameObject()->GetTransform()->SetParent(playerAxis->GetTransform());

	// �÷��̾��� ����� �����ʰ�, ������ �������� ������Ʈ
	playerAttackAxis->GetTransform()->SetLocalPosition(GetGameObject()->GetTransform()->GetFront());
	playerAttackAxis->GetTransform()->SetLocalRotation(GetGameObject()->GetTransform()->GetLocalRotation());
	playerAttackAxis->GetTransform()->AddChild(playerSpecialAttack->GetTransform());
	playerAttackAxis->GetTransform()->AddChild(playerBasicAttack->GetTransform());

	// �Ϲ� ���� ���� ����

	playerBasicAttack->SetActive(false);
	basicAttackCheck = playerBasicAttack->AddComponent<CubeCollider>();
	basicAttackCheck->SetTrigger(true);
	playerBasicAttack->SetLayerName(L"BasicAttack");
	playerBasicAttack->GetTransform()->SetLocalScale(Vector3D(8, 10, 7));
	playerBasicAttack->GetTransform()->SetLocalPosition(playerAttackAxis->GetTransform()->GetLocalPosition() + Vector3D(0, 0, 3.5f));

	// Ư�� ���� ���� ����
	playerSpecialAttack->SetActive(false);
	specialAttackCheck = playerSpecialAttack->AddComponent<CubeCollider>();
	specialAttackCheck->SetTrigger(true);
	playerSpecialAttack->SetLayerName(L"RangedAttack");
	playerSpecialAttack->GetTransform()->SetLocalScale(Vector3D(8, 10, 12));
	playerSpecialAttack->GetTransform()->SetLocalPosition(playerAttackAxis->GetTransform()->GetLocalPosition() + Vector3D(0, 0, 6.0f));



	// ��Ʈ�� �߰�
	playerDotween = GetGameObject()->AddComponent<Dotween>();


	// Collider �浹ó�� ������Ʈ �ֱ�
	playerBasicAttack->AddComponent<PlayerCollisionManager>();
	playerBasicAttack->GetComponent<PlayerCollisionManager>()->SetBossComponent(bossComponent);
	playerBasicAttack->GetComponent<PlayerCollisionManager>()->SetDamage(playerAp);
	playerSpecialAttack->AddComponent<PlayerCollisionManager>();
	playerSpecialAttack->GetComponent<PlayerCollisionManager>()->SetBossComponent(bossComponent);
	playerSpecialAttack->GetComponent<PlayerCollisionManager>()->SetDamage(playerSpecialAp);


	/// Idle-------------------------------------------------------------------------------------------------------------------------
	// Idle ���¿��� Move ���·� �������� ����
	playerFSM.transitions[PlayerState::Idle].push_back({ PlayerState::Move,
		[this]() {return Input::GetInstance().PressedKey(moveUp) ||
		Input::GetInstance().PressedKey(moveDown) ||
		Input::GetInstance().PressedKey(moveLeft) ||
		Input::GetInstance().PressedKey(moveRight);
		} });
	// Idle ���¿��� Dash ���·� �������� ����
	playerFSM.transitions[PlayerState::Idle].push_back({ PlayerState::Dash,
		[this]() {return Input::GetInstance().PressedKey(dashKey) && dashChargingTime >= dashCoolTime; } });
	// Idle ���¿��� Attack ���·� �������� ����
	playerFSM.transitions[PlayerState::Idle].push_back({ PlayerState::Attack,
		[this]() {return Input::GetInstance().PressedKey(attackKey) && normalAttackTime >= normalAttackCoolTime; } });
	// Idle ���¿��� SpecialAttack ���·� �������� ����
	playerFSM.transitions[PlayerState::Idle].push_back({ PlayerState::SpecialAttack,
		[this]() {return Input::GetInstance().PressedKey(specialAttackKey) &&
		specialAttackChargingTime >= spacialAttackCoolTime; } });
	// Idle ���¿��� Dead ���·� �������� ����
	playerFSM.transitions[PlayerState::Idle].push_back({ PlayerState::Dead,
		[this]() {return playerHp <= 0; } });
	// Idle ���¿��� Stun ���·� �������� ����
	playerFSM.transitions[PlayerState::Idle].push_back({ PlayerState::Stun,
		[this]() {return isDamaged == true; } });
	// Idle ���¿��� Fall ���·� �������� ����
	playerFSM.transitions[PlayerState::Idle].push_back({ PlayerState::Fall,
		[this]() {return (GetGameObject()->GetTransform()->GetLocalPosition().y < -1) && (SceneManager::GetInstance().GetName() != L"IntroScene"); } });

	/// Move-------------------------------------------------------------------------------------------------------------------------
	// Move ���¿��� Idle ���·� �������� ����
	playerFSM.transitions[PlayerState::Move].push_back({ PlayerState::Idle,
		[this]() {return !Input::GetInstance().PressedKey(moveUp) &&
		!Input::GetInstance().PressedKey(moveDown) &&
		!Input::GetInstance().PressedKey(moveLeft) &&
		!Input::GetInstance().PressedKey(moveRight);
		} });
	// Move ���¿��� Dash ���·� �������� ����
	playerFSM.transitions[PlayerState::Move].push_back({ PlayerState::Dash,
		[this]() {return Input::GetInstance().PressedKey(dashKey) && dashChargingTime >= dashCoolTime; } });
	// Move ���¿��� Attack ���·� �������� ����
	playerFSM.transitions[PlayerState::Move].push_back({ PlayerState::Attack,
		[this]() {return Input::GetInstance().PressedKey(attackKey) && normalAttackTime >= normalAttackCoolTime; } });
	// Move ���¿��� SpecialAttack ���·� �������� ����
	playerFSM.transitions[PlayerState::Move].push_back({ PlayerState::SpecialAttack,
		[this]() {return Input::GetInstance().PressedKey(specialAttackKey) &&
		specialAttackChargingTime >= spacialAttackCoolTime; } });
	// Move ���¿��� Dead ���·� �������� ����
	playerFSM.transitions[PlayerState::Move].push_back({ PlayerState::Dead,
		[this]() {return playerHp <= 0; } });
	// Move ���¿��� Stun ���·� �������� ����
	playerFSM.transitions[PlayerState::Move].push_back({ PlayerState::Stun,
		[this]() {return isDamaged == true || isPush == true; } });
	// Move ���¿��� Fall ���·� �������� ����
	playerFSM.transitions[PlayerState::Move].push_back({ PlayerState::Fall,
		[this]() {return (GetGameObject()->GetTransform()->GetLocalPosition().y < -1) && (SceneManager::GetInstance().GetName() != L"IntroScene"); } });

	/// Attack-----------------------------------------------------------------------------------------------------------------------
	// Attack ���¿��� Idle ���·� �������� ����
	playerFSM.transitions[PlayerState::Attack].push_back({ PlayerState::Idle,
		[this]() {return normalAttackTime >= AttackMotionDelay; } });
	// Attack ���¿��� Dead ���·� �������� ����z
	playerFSM.transitions[PlayerState::Attack].push_back({ PlayerState::Dead,
		[this]() {return playerHp <= 0; } });
	// Attack ���¿��� Stun ���·� �������� ����
	playerFSM.transitions[PlayerState::Attack].push_back({ PlayerState::Stun,
		[this]() {return isDamaged == true || isPush == true; } });
	// Attack ���¿��� Fall ���·� �������� ����
	playerFSM.transitions[PlayerState::Attack].push_back({ PlayerState::Fall,
		[this]() {return (GetGameObject()->GetTransform()->GetLocalPosition().y < -1) && (SceneManager::GetInstance().GetName() != L"IntroScene"); } });

	/// SpecialAttack----------------------------------------------------------------------------------------------------------------
	// SpecialAttack ���¿��� Idle ���·� �������� ����
	playerFSM.transitions[PlayerState::SpecialAttack].push_back({ PlayerState::Idle,
		[this]() {return specialAttackChargingTime >= SpecialAttackMotionDelay; } });
	// SpecialAttack ���¿��� Dead ���·� �������� ����
	playerFSM.transitions[PlayerState::SpecialAttack].push_back({ PlayerState::Dead,
		[this]() {return playerHp <= 0; } });
	// SpecialAttack ���¿��� Stun ���·� �������� ����
	playerFSM.transitions[PlayerState::SpecialAttack].push_back({ PlayerState::Stun,
		[this]() {return isDamaged == true || isPush == true; } });
	// SpecialAttack ���¿��� Fall ���·� �������� ����
	playerFSM.transitions[PlayerState::SpecialAttack].push_back({ PlayerState::Fall,
		[this]() {return (GetGameObject()->GetTransform()->GetLocalPosition().y < -1) && (SceneManager::GetInstance().GetName() != L"IntroScene"); } });

	/// Interaction------------------------------------------------------------------------------------------------------------------


	/// Stun-------------------------------------------------------------------------------------------------------------------------
	// Stun ���¿��� Invincible ���·� �������� ����
	playerFSM.transitions[PlayerState::Stun].push_back({ PlayerState::Idle,
		[this]() {return stunTime >= stunCoolTime && stunPushTime >= stunPushCoolTime && isStop == true; } });
	// Stun ���¿��� Fall ���·� �������� ����
	playerFSM.transitions[PlayerState::Stun].push_back({ PlayerState::Fall,
		[this]() {return (GetGameObject()->GetTransform()->GetLocalPosition().y < -1) && (SceneManager::GetInstance().GetName() != L"IntroScene"); } });

	/// Invincible-------------------------------------------------------------------------------------------------------------------


	/// Dash-------------------------------------------------------------------------------------------------------------------------
	// Dash ���¿��� Idle ���·� �������� ����
	playerFSM.transitions[PlayerState::Dash].push_back({ PlayerState::Idle,
		[this]() {return dashElapsed >= dashDuration; } });
	// Stun ���¿��� Fall ���·� �������� ����
	playerFSM.transitions[PlayerState::Dash].push_back({ PlayerState::Fall,
		[this]() {return (GetGameObject()->GetTransform()->GetLocalPosition().y < -1) && (SceneManager::GetInstance().GetName() != L"IntroScene"); } });

	/// ���� ����---------------------------------------------------------------------------------------------------------------------
	// Idle ���·� ���� ���� ��
	playerFSM.engageAction[PlayerState::Idle] = [this]() {IdleEngage(); };
	// Move ���·� ���� ���� ��
	playerFSM.engageAction[PlayerState::Move] = [this]() {MoveEngage(); };
	// Dash ���·� ���� ���� ��
	playerFSM.engageAction[PlayerState::Dash] = [this]() {DashEngage(); };
	// Attack ���·� ���� ���� ��
	playerFSM.engageAction[PlayerState::Attack] = [this]() {AttackEngage(); };
	// SpecialAttack ���·� ���� ���� ��
	playerFSM.engageAction[PlayerState::SpecialAttack] = [this]() {SpecialAttackEngage(); };
	// Stun ���·� ���� ���� ��
	playerFSM.engageAction[PlayerState::Stun] = [this]() {StunEngage(); };
	// Dead ���·� ���� ���� ��
	playerFSM.engageAction[PlayerState::Dead] = [this]() {DeadEngage(); };
	// Fall ���·� ���� ���� ��
	playerFSM.engageAction[PlayerState::Fall] = [this]() {FallEngage(); };

	/// ���� ����---------------------------------------------------------------------------------------------------------------------
	// Idle ���°� ���� ���� ��
	playerFSM.updateAction[PlayerState::Idle] = [this]() {IdleState(); };
	// Move ���°� ���� ���� ��
	playerFSM.updateAction[PlayerState::Move] = [this]() {MoveState(); };
	// Dash ���°� ���� ���� ��
	playerFSM.updateAction[PlayerState::Dash] = [this]() {DashState(); };
	// Attack ���°� ���� ���� ��
	playerFSM.updateAction[PlayerState::Attack] = [this]() {/*AttackState();*/ };
	// SpecialAttack ���°� ���� ���� ��
	playerFSM.updateAction[PlayerState::SpecialAttack] = [this]() {SpecialAttackState(); };
	// Stun ���·� ���� ���� ��
	playerFSM.updateAction[PlayerState::Stun] = [this]() {StunState(); };
	// Dead ���·� ���� ���� ��
	playerFSM.updateAction[PlayerState::Dead] = [this]() {DeadState(); };
	// Fall ���·� ���� ���� ��
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
		// ���¸� ������Ʈ ��Ų��.
		playerFSM.UpdateState();

	// ��ŸŸ���� �����´�.
	float deltaTime = TimeController::GetInstance().GetDeltaTime();

	// ��Ÿ�� �ð��� ������Ʈ ��Ų��.
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

	// ���� Ű���� ���Ϳ� �����Ѵ�.
	inputMove = Vector3D::zero;


	// �÷��̾��� ������ �������� �����´�.
	Vector3D moveDelta = playerAxis->GetTransform()->GetLocalPosition();

	// ��ŸŸ���� �����´�.
	float deltaTime = TimeController::GetInstance().GetDeltaTime();

	// �÷��̾��� ���鿡 ���� ���͸� �����´�.
	Vector3D playerFront = GetGameObject()->GetTransform()->GetFront();

	// Ű�� �Է��� �޾Ƽ� inpputMove ���Ϳ� �����Ѵ�.
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


	// �� ���ͻ����� ���� �������� ���Ѵ�.
	float dotAngleFront = RadianToDegree(acos(DotProduct(playerFront, inputMove.Normalize())));

	// ȸ���� ���� ���
	if (dotAngleFront != 0 && inputMove != Vector3D::zero)
	{
		// �࿡ �����������, ���������� �׳� �����ϰ� �� ������ ������ ������Ų��.
		if (!(dotAngleFront < errorRange && -errorRange < dotAngleFront))
		{
			// ȸ�� ���ӵ��� ���� ȸ�������� Ŭ ���, �Է¹��� ���ͷ� ���� �����Ѵ�.
			if (dotAngleFront < RadianToDegree((rotationAngleSpeed * deltaTime)))
			{
				// ȸ������ ũ�⸦ ����Ͽ� ȸ���� ������ ���Ѵ�.
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
	// �ٲ� ǥ���� ���� �־��ش�.
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
	// ��ŸŸ���� �����´�.
	float deltaTime = TimeController::GetInstance().GetDeltaTime();
	dashElapsed += deltaTime;

	float sinBeforeElapsed = easeInOutSine((dashElapsed - deltaTime) / dashDuration);
	float sinElapsed = easeInOutSine(dashElapsed / dashDuration);

	Vector3D distance = inputMove.Normalize() * dashSpeed;

	// �����Ӵ� ������ ����
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
	// ���Ӱ��� ����
	// ���ݽ� �̵� �Ұ��� (���·� ���� �Ϸ�)
	// ��Ÿ�� �ʴ� 1ȸ
}

void Player::SpecialAttackEvent()
{
	// ��Ÿ�� 10��
	// �ٶ󺸴� �������� ����

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
	// �뽬 �ð�
	if (dashChargingTime < dashCoolTime)
	{
		dashChargingTime += deltaTime;
	}
	if (dashChargingTime >= dashCoolTime)
	{
		dashChargingTime = dashCoolTime;
	}

	// Ư������ �ð�
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

	// ���� �ð�
	if (stunTime < stunCoolTime)
	{
		stunTime += deltaTime;
	}
	if (stunTime >= stunCoolTime)
	{
		stunTime = stunCoolTime;
	}

	// �а� ���� �ð�
	if (stunPushTime < stunPushCoolTime)
	{
		stunPushTime += deltaTime;
	}
	if (stunPushTime >= stunPushCoolTime)
	{
		stunPushTime = stunPushCoolTime;
		isPush = false;
	}

	// ���� �ð�
	if (invincibleTime < invincibleCoolTime)
	{
		invincibleTime += deltaTime;
	}
	if (invincibleTime >= invincibleCoolTime)
	{
		invincibleTime = invincibleCoolTime;
		isInvincible = false;
	}
	// �̼����� �ð�
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

// ȸ���� �ּҰ��� ���Ѵ�.
float Player::GetRotationAngle(const Vector3D& local, const Vector3D& world, float angle)
{
	// ����
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
