#pragma once

#include "ZeldaEngine.h"
#include "FSM.h"
#include "IHPObject.h"
#include <list>

// 편하게 쓰려면 using namespace 2개는 고정으로 해주는게 좋을것임
using namespace ZeldaEngine;
using namespace ZeldaEngine::ZeldaMath;

// vector, map의 serialize가 필요하다.....

// 컴포넌트 만들 때는 Component 클래스를 상속받아야한다.
// 아래는 현재 사용 가능한 이벤트함수 목록
// void Awake();							게임오브젝트가 활성화될 때, 단 한번만 호출된다.
// void Start();							게임오브젝트가 활성화될 때, 단 한번만 Awake 이후에 호출된다.
// void OnEnable();							게임오브젝트가 활성화될 때, 매번 호출된다.
// void OnDisable();						게임오브젝트가 비활성화될 때, 매번 호출된다.
// void OnDestroy();						게임오브젝트가 제거될 때, 단 한번만 호출된다.
// 
// void OnTriggerEnter();					// rigidbody를 가진 오브젝트가 임의의 오브젝트와 충돌을 시작한 순간 호출된다.
// void OnTriggerStay();					// rigidbody를 가진 오브젝트가 임의의 오브젝트와 충돌한 상태를 유지할 때 호출된다.
// void OnTriggerExit();					// rigidbody를 가진 오브젝트가 임의의 오브젝트와의 충돌에서 벗어나는 순간 호출된다.
// void OnTriggerEnter(Collider* other);	// rigidbody를 가진 오브젝트가 other과 충돌하는 순간 호출된다.				(한 프레임에 여러번 호출될 수 있다.)
// void OnTriggerStay(Collider* other);		// rigidbody를 가진 오브젝트가 other과 충돌한 상태를 유지할 때 호출된다.		(한 프레임에 여러번 호출될 수 있다.)
// void OnTriggerExit(Collider* other);		// rigidbody를 가진 오브젝트가 other과 충돌에서 벗어나는 순간 호출된다		(한 프레임에 여러번 호출될 수 있다.)
// 
// void EarlyUpdate();						// 매 프레임 한번씩만 호출된다. Update 전에 처리할 것이 있는 경우 사용한다.
// void Update();							// 매 프레임 한번씩만 호출된다. 기본적인 Update를 위한 함수
// void LateUpdate();						// 매 프레임 한번씩만 호출된다. Update 후에 처리할 것이 있는 경우 사용한다. (EX.카메라의 플레이어 위치로의 이동)

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

	// 축이 근접할시 오차범위
	float errorRange;
	// 공격시간
	float attackDelay;
	float AttackMotionDelay;
	float SpecialAttackMotionDelay;

	// Hp, Ap, Special Ap
	
	float playerAp;
	float playerSpecialAp;

	// 대쉬기
	float dashDuration;
	float dashChargingTime;

	float dashElapsed;

	// 일반공격
	float normalAttackTime;
	float normalAttackCoolTime;

	// 특수 공격
	float specialAttackChargingTime;

	// 무적
	float invincibleTime;


	// 무적 시 이속증가
	float hasteTime;

	// 경직
	float stunTime;
	float stunPushTime;

	// 방향키를 모두 int 변수로 뽑아냈다.
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

	// 플레이어의 월드 축
	GameObject* playerAxis;
	// 플레이어 특수공격 오브젝트
	GameObject* playerSpecialAttack;

	GameObject* playerBasicAttack;



	/// 160도 잡기가 어려워서 큐브로 대체하고 후에 생각 할 것
	// 플레이어 기본공격을 위한 실린더 1개 + 큐브 2개
	//GameObject* playerAttackCylinder;
	//GameObject* playerAttackCube1;
	//GameObject* playerAttackCube2;
	// 플레이어 공격 방향 좌표 오브젝트
	GameObject* playerAttackAxis;

	// 플레이어 캡슐 콜라이더


	// 일반공격 체크
	CubeCollider* basicAttackCheck;
	//CylinderCollider* normalAttackCheck;
	//CubeCollider* exceptAttackCheck1;
	//CubeCollider* exceptAttackCheck2;

	// 특수공격 체크
	CubeCollider* specialAttackCheck;
	// 1. Dotween 컴포넌트 선언
	Dotween* playerDotween;
	PlayerCollisionManager* playerAttackManager;

	/// Serialize
	/// 변수---------------------------------------------------------------------------------------------
	// 움직이는 속도
	float moveSpeed;
	// 이속 증가량
	float hasteSpeed;
	// 회전 각 속도
	float rotationAngleSpeed;
	// 플레이어 HP
	float playerHp;
	// 플레이어 최대HP
	float playerMaxHp;
	// 대쉬 속도
	float dashSpeed;
	// 대쉬 쿨타임
	float dashCoolTime;	
	// 특수공격 쿨타임
	float spacialAttackCoolTime;
	// 무적 시간
	float invincibleCoolTime;
	// 이속증가 시간
	float hasteCoolTime;
	// 스턴시간
	float stunCoolTime;
	float stunPushCoolTime;
	// 충격량
	float force;

	/// 포인터-------------------------------------------------------------------------------------------
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
	// Idle 상태로 진입 했을 때
	void IdleEngage();
	// Move 상태로 진입 했을 때
	void MoveEngage();
	// Dash 상태로 진입 했을 때
	void DashEngage();
	// Attack 상태로 진입 했을 때
	void AttackEngage();
	// SpecialAttack 상태로 진입 했을 때
	void SpecialAttackEngage();
	// Stun 상태로 진입 했을 때
	void StunEngage();
	// Dead 상태로 진입 했을 때
	void DeadEngage();
	// Fall 상태로 진입 했을 때
	void FallEngage();
	// Idle 상태가 유지 중일 때
	void IdleState();
	// Move 상태가 유지 중일 때
	void MoveState();
	// Dash 상태가 유지 중일 때
	void DashState();
	// Attack 상태가 유지 중일 때
	void AttackState();
	// SpecialAttack 상태가 유지 중일 때
	void SpecialAttackState();
	// Stun 상태가 유지 중일 때
	void StunState();
	// Dead 상태로 진입 했을 때
	void DeadState();
	// Fall 상태로 진입 했을 때
	void FallState();

private:
	// 플레이어의 움직임에 관한 함수
	void MoveEvent();

	// 플레이어의 대쉬에 관한 함수
	void DashEngageEvent();
	void DashStateEvent();

	// 플레이어 공격에 관한 함수
	void AttackEvent();

	// 플레이어 특수 공격에 관한 함수
	void SpecialAttackEvent();

	// 경직에 관한 함수
	void StunEvent();

	// 낙사에 관한 함수
	void FallEvent();

	// 쿨타임에 관한 함수
	void TimeEvent(float deltaTime);

	// 회전할 최소각을 구한다.
	float GetRotationAngle(const Vector3D& local, const Vector3D& world, float angle);

	void AttackedDamage(float damage);

	float easeInOutSine(float t);

	void OnCollisionEnter(Collider* other);
	/// 외부에 데이터를 주고 받기 위함
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
	// Component을(를) 통해 상속됨
	virtual void PreSerialize(json& jsonData) const override;
	virtual void PreDeserialize(const json& jsonData) override;
	virtual void PostSerialize(json& jsonData) const override;
	virtual void PostDeserialize(const json& jsonData) override;
};