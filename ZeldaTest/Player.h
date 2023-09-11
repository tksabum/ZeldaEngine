#pragma once

#include "ZeldaEngine.h"
#include "FSM.h"
#include "IHPObject.h"
#include <list>

// ���ϰ� ������ using namespace 2���� �������� ���ִ°� ��������
using namespace ZeldaEngine;
using namespace ZeldaEngine::ZeldaMath;

// vector, map�� serialize�� �ʿ��ϴ�.....

// ������Ʈ ���� ���� Component Ŭ������ ��ӹ޾ƾ��Ѵ�.
// �Ʒ��� ���� ��� ������ �̺�Ʈ�Լ� ���
// void Awake();							���ӿ�����Ʈ�� Ȱ��ȭ�� ��, �� �ѹ��� ȣ��ȴ�.
// void Start();							���ӿ�����Ʈ�� Ȱ��ȭ�� ��, �� �ѹ��� Awake ���Ŀ� ȣ��ȴ�.
// void OnEnable();							���ӿ�����Ʈ�� Ȱ��ȭ�� ��, �Ź� ȣ��ȴ�.
// void OnDisable();						���ӿ�����Ʈ�� ��Ȱ��ȭ�� ��, �Ź� ȣ��ȴ�.
// void OnDestroy();						���ӿ�����Ʈ�� ���ŵ� ��, �� �ѹ��� ȣ��ȴ�.
// 
// void OnTriggerEnter();					// rigidbody�� ���� ������Ʈ�� ������ ������Ʈ�� �浹�� ������ ���� ȣ��ȴ�.
// void OnTriggerStay();					// rigidbody�� ���� ������Ʈ�� ������ ������Ʈ�� �浹�� ���¸� ������ �� ȣ��ȴ�.
// void OnTriggerExit();					// rigidbody�� ���� ������Ʈ�� ������ ������Ʈ���� �浹���� ����� ���� ȣ��ȴ�.
// void OnTriggerEnter(Collider* other);	// rigidbody�� ���� ������Ʈ�� other�� �浹�ϴ� ���� ȣ��ȴ�.				(�� �����ӿ� ������ ȣ��� �� �ִ�.)
// void OnTriggerStay(Collider* other);		// rigidbody�� ���� ������Ʈ�� other�� �浹�� ���¸� ������ �� ȣ��ȴ�.		(�� �����ӿ� ������ ȣ��� �� �ִ�.)
// void OnTriggerExit(Collider* other);		// rigidbody�� ���� ������Ʈ�� other�� �浹���� ����� ���� ȣ��ȴ�		(�� �����ӿ� ������ ȣ��� �� �ִ�.)
// 
// void EarlyUpdate();						// �� ������ �ѹ����� ȣ��ȴ�. Update ���� ó���� ���� �ִ� ��� ����Ѵ�.
// void Update();							// �� ������ �ѹ����� ȣ��ȴ�. �⺻���� Update�� ���� �Լ�
// void LateUpdate();						// �� ������ �ѹ����� ȣ��ȴ�. Update �Ŀ� ó���� ���� �ִ� ��� ����Ѵ�. (EX.ī�޶��� �÷��̾� ��ġ���� �̵�)

class Dotween;
class PlayerCollisionManager;
class Boss1;
class GameManager;
class EdgeDetect;

enum class PlayerState
{
	Idle,
	Move,
	Dash,
	Attack,
	SpecialAttack,
	Stun,
	Invincible,
	Interaction,
	Dead,
	Fall,
	StateEnd
};

class Player : public ZeldaEngine::Component, public IHPObject
{
public:
	Player();
	//~Player();

	void Awake();
	void Start();
	void Update();

private:
	FSM<PlayerState> playerFSM = FSM<PlayerState>(PlayerState::Idle);
	GameManager* gameManager;
	GameObject* boss;
	
	EdgeDetect* playerEdge;

	// ���� �����ҽ� ��������
	float errorRange;
	// ���ݽð�
	float attackDelay;
	float AttackMotionDelay;
	float SpecialAttackMotionDelay;

	// Hp, Ap, Special Ap
	
	float playerAp;
	float playerSpecialAp;

	// �뽬��
	float dashDuration;
	float dashChargingTime;

	float dashElapsed;

	// �Ϲݰ���
	float normalAttackTime;
	float normalAttackCoolTime;

	// Ư�� ����
	float specialAttackChargingTime;

	// ����
	float invincibleTime;


	// ���� �� �̼�����
	float hasteTime;

	// ����
	float stunTime;
	float stunPushTime;

	// ����Ű�� ��� int ������ �̾Ƴ´�.
	int moveUp;
	int moveDown;
	int moveLeft;
	int moveRight;
	int dashKey;
	int attackKey;
	int specialAttackKey;

	Vector3D inputMove;



	bool isInvincible;
	bool isDashInvincible;
	bool isHaste;
	bool isDamaged;
	bool isPush;
	bool isStop;

	// �÷��̾��� ���� ��
	GameObject* playerAxis;
	// �÷��̾� Ư������ ������Ʈ
	GameObject* playerSpecialAttack;

	GameObject* playerBasicAttack;



	/// 160�� ��Ⱑ ������� ť��� ��ü�ϰ� �Ŀ� ���� �� ��
	// �÷��̾� �⺻������ ���� �Ǹ��� 1�� + ť�� 2��
	//GameObject* playerAttackCylinder;
	//GameObject* playerAttackCube1;
	//GameObject* playerAttackCube2;
	// �÷��̾� ���� ���� ��ǥ ������Ʈ
	GameObject* playerAttackAxis;

	// �÷��̾� ĸ�� �ݶ��̴�


	// �Ϲݰ��� üũ
	CubeCollider* basicAttackCheck;
	//CylinderCollider* normalAttackCheck;
	//CubeCollider* exceptAttackCheck1;
	//CubeCollider* exceptAttackCheck2;

	// Ư������ üũ
	CubeCollider* specialAttackCheck;
	// 1. Dotween ������Ʈ ����
	Dotween* playerDotween;
	PlayerCollisionManager* playerAttackManager;

	/// Serialize
	/// ����---------------------------------------------------------------------------------------------
	// �����̴� �ӵ�
	float moveSpeed;
	// �̼� ������
	float hasteSpeed;
	// ȸ�� �� �ӵ�
	float rotationAngleSpeed;
	// �÷��̾� HP
	float playerHp;
	// �÷��̾� �ִ�HP
	float playerMaxHp;
	// �뽬 �ӵ�
	float dashSpeed;
	// �뽬 ��Ÿ��
	float dashCoolTime;	
	// Ư������ ��Ÿ��
	float spacialAttackCoolTime;
	// ���� �ð�
	float invincibleCoolTime;
	// �̼����� �ð�
	float hasteCoolTime;
	// ���Ͻð�
	float stunCoolTime;
	float stunPushCoolTime;
	// ��ݷ�
	float force;

	/// ������-------------------------------------------------------------------------------------------
	Transform* bossTransform;
	Animator* m_animator;
	SkinnedMeshRenderer* playerSkinnedMeshRenderer;
	Rigidbody* playerRigidbody;
	CapsuleCollider* playerCollider;
	Transform* playerAnimTransform;
	Transform* bossAnimTransform;
	CapsuleCollider* bossCollider;
	SpriteAnimator* playerDamagedSpriteAnimator;
	GameObject* playerAttackEffect;
	GameObject* playerSpecialAttackEffect;
	Boss1* bossComponent;


public:
	/// FSM
	// Idle ���·� ���� ���� ��
	void IdleEngage();
	// Move ���·� ���� ���� ��
	void MoveEngage();
	// Dash ���·� ���� ���� ��
	void DashEngage();
	// Attack ���·� ���� ���� ��
	void AttackEngage();
	// SpecialAttack ���·� ���� ���� ��
	void SpecialAttackEngage();
	// Stun ���·� ���� ���� ��
	void StunEngage();
	// Dead ���·� ���� ���� ��
	void DeadEngage();
	// Fall ���·� ���� ���� ��
	void FallEngage();
	// Idle ���°� ���� ���� ��
	void IdleState();
	// Move ���°� ���� ���� ��
	void MoveState();
	// Dash ���°� ���� ���� ��
	void DashState();
	// Attack ���°� ���� ���� ��
	void AttackState();
	// SpecialAttack ���°� ���� ���� ��
	void SpecialAttackState();
	// Stun ���°� ���� ���� ��
	void StunState();
	// Dead ���·� ���� ���� ��
	void DeadState();
	// Fall ���·� ���� ���� ��
	void FallState();

private:
	// �÷��̾��� �����ӿ� ���� �Լ�
	void MoveEvent();

	// �÷��̾��� �뽬�� ���� �Լ�
	void DashEngageEvent();
	void DashStateEvent();

	// �÷��̾� ���ݿ� ���� �Լ�
	void AttackEvent();

	// �÷��̾� Ư�� ���ݿ� ���� �Լ�
	void SpecialAttackEvent();

	// ������ ���� �Լ�
	void StunEvent();

	// ���翡 ���� �Լ�
	void FallEvent();

	// ��Ÿ�ӿ� ���� �Լ�
	void TimeEvent(float deltaTime);

	// ȸ���� �ּҰ��� ���Ѵ�.
	float GetRotationAngle(const Vector3D& local, const Vector3D& world, float angle);

	void AttackedDamage(float damage);

	float easeInOutSine(float t);

	void OnCollisionEnter(Collider* other);
	/// �ܺο� �����͸� �ְ� �ޱ� ����
public:
	void SetGameManager(GameManager* gameManager);
	CapsuleCollider* GetPlayerCollider();
	virtual void OnDamaged(float damage, int type) override;
	float GetPlayerHp();
	float GetPlayerMaxHp();
	float GetDashChargingTime();
	float GetDashCoolTime();
	float GetSpecialAttackChargingTime();
	float GetSpecialAttackCoolTime();


	void SetBossTransform(Transform* tranform);
	void SetBossCollider(CapsuleCollider* collider);
	void SetAnimator(Animator* animator);
	void SetSkinnedMeshRenderer(SkinnedMeshRenderer* skinnedMeshRenderer);
	void SetRigidBody(Rigidbody* rigidbody);
	void SetPlayerAnimTransform(Transform* transform);
	void SetBossAnimTransform(Transform* transform);
	void SetPlayerDamagedSpriteAnimator(SpriteAnimator* spriteanimator);
	void SetPlayerAttackEffect(GameObject* attackEffect);
	void SetPlayerSpecialAttackEffect(GameObject* specialAttackEffect);
	void SetBossComponent(Boss1* boss);
	void EdgeOff();
private:
	// Component��(��) ���� ��ӵ�
	virtual void PreSerialize(json& jsonData) const override;
	virtual void PreDeserialize(const json& jsonData) override;
	virtual void PostSerialize(json& jsonData) const override;
	virtual void PostDeserialize(const json& jsonData) override;
};