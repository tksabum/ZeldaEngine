#pragma once
#include "ZeldaEngine.h"
#include "FSM.h"
#include "Easing.h"
#include "IHPObject.h"
#include <stack>
#include <unordered_map>

/// <summary>
/// 동훈이형이 만든 fsm과 Dotween을 잘 써서 Boss1을 간단하게 만들어보자.
/// 상태에는 크게 Idle과 Pattern 두 종류가 있다.
/// 모든 Pattern은 종료 시 Idle로 변환, Idle 진입 시 난수 생성해서 Pattern을 선택해 들어간다.
/// </summary>

/// <summary>
/// 새로운 패턴을 만들땐 꼭 Idle 전에 만들어주기!
/// </summary>
enum class Boss1State
{
	Pattern2,
	Pattern3,
	Pattern5,
	Pattern6,
	PatternMove,
	Idle,
	Pattern1,
	Groggy,
	Death,
	Pattern2DNormal,
	DisruptingMovement,
	Pattern2DUltimate,
	StateEnd
};

class Dotween;
class BossCollisionManager;
class EffectPlayer;
class Player;
class FloatingObject;
class BossFlag;
class GameManager;
class FollowCamera;
class Player2DCollisionManager;
class EdgeDetect;

class Boss1 : public ZeldaEngine::Component, public IHPObject
{
private:
	GameManager* gameManager;

	BossCollisionManager* m_collisionManager[12];
	int collisionIndex = 0;
	bool jumpToPhase2 = false;
	FSM<Boss1State>* boss1FSM;

	Dotween* boss1Dotween;
	Dotween* cameraDotween;

	ZeldaEngine::ZeldaMath::Vector3D circleDistanceVec;

	ZeldaEngine::GameObject* m_gameObject;

	// 플레이어의 정보를 가져오자
	ZeldaEngine::Transform* m_playerTransform;
	ZeldaEngine::GameObject* m_playerGameObject;
	ZeldaEngine::CapsuleCollider* m_playerCollider;
	ZeldaEngine::Rigidbody* m_playerRigidBody;
	Player* m_playerComponent;
	FollowCamera* m_followCamera;

	ZeldaEngine::SpriteAnimator* bossHitSpriteAnimator;

	// 족자 component를 가져온다
	FloatingObject* tempFloatingComponent;
	std::vector<FloatingObject*> floatingObjectComponentVector;
	std::vector<FloatingObject*>::iterator m_floatingObjectIter;

	// bossFlag component를 가져온다
	BossFlag* tempbossComponent;
	std::vector<BossFlag*> bossFlagComponentVector;
	std::vector<BossFlag*>::iterator m_bossFlagComponentIter;

	// Boss가 패턴 실행 시 사용할 오브젝트.
	ZeldaEngine::GameObject* timeDotweenObject;

	ZeldaEngine::GameObject* bossParticleObject;

	ZeldaEngine::GameObject* fanObject;

	ZeldaEngine::GameObject* branch12ColliderObject;
	ZeldaEngine::GameObject* branch34ColliderObject;					// 강화 패턴은 가지가 두 개
	ZeldaEngine::GameObject* boss1TransformObject;
	ZeldaEngine::GameObject* barrierCollisionObject;
	ZeldaEngine::GameObject* barrierTriggerObject;

	ZeldaEngine::GameObject* branchEffectObjects[4];
	ZeldaEngine::GameObject* treeParent;
	ZeldaEngine::GameObject* circleEffectObject;

	ZeldaEngine::GameObject* teleportDisappearEffectObject;
	ZeldaEngine::GameObject* teleportAppearEffectObject;

	ZeldaEngine::GameObject* disruptingAppearEffectObject[3];
	ZeldaEngine::GameObject* disruptingObjects[3];
	ZeldaEngine::GameObject* disruptingColliderObjects[3];

	ZeldaEngine::GameObject* knockBackAttackEffectObject;
	ZeldaEngine::GameObject* knockBackGuideLineObject;

	ZeldaEngine::GameObject* chargeAttackEffectObject;
	ZeldaEngine::GameObject* chargeSonicEffectObject;
	ZeldaEngine::GameObject* chargeGuideEffectObject;
	ZeldaEngine::GameObject* normalAttackGuideObject;
	ZeldaEngine::GameObject* skill2DEffectObject;

	ZeldaEngine::GameObject* horizontalAttackEffectObject;
	ZeldaEngine::GameObject* verticalAttackEffectObject;
	ZeldaEngine::GameObject* protectEffectObject;
	ZeldaEngine::GameObject* protectEffectObject2;
	
	ZeldaEngine::GameObject* rangedAttackEffectObject;		// 보스 붓에서 나오는 이펙트
	ZeldaEngine::GameObject* rangedExposionEffectObject;	// 투사체 도달 시 폭발 이펙트

	ZeldaEngine::GameObject* obstaclePatternEffectObject;

	ZeldaEngine::GameObject* horizontalObject;
	ZeldaEngine::GameObject* verticalObject;

	ZeldaEngine::GameObject* pattern2ColliderObject;

	ZeldaEngine::GameObject* projectileObject;
	ZeldaEngine::GameObject* childProjectileObject;
	ZeldaEngine::GameObject* pattern5ColliderObject1;				// 원거리 공격 폭발지역

	ZeldaEngine::GameObject* normal2DCollisionObject[10];			// 미리 10개정도 생성
	ZeldaEngine::GameObject* normal2DWarningObject[10];			
	
	ZeldaEngine::GameObject* ultimateCollisionObject[10];			// 미리 10개정도 생성
	
	ZeldaEngine::GameObject* bossFrontObject;	

	ZeldaEngine::GameObject* rageTextureObject;			
	ZeldaEngine::Image* rageTextureImage;

	// 이미지가 붙을 오브젝트
	ZeldaEngine::GameObject* ultimateBottomObject;
	ZeldaEngine::GameObject* ultimateTopObject;
	ZeldaEngine::GameObject* ultimateLeftObject;
	ZeldaEngine::GameObject* ultimateRightObject;

	// 실제 콜라이더가 붙을 오브젝트
	ZeldaEngine::GameObject* ultimateCollisionBottomObject;
	ZeldaEngine::GameObject* ultimateCollisionTopObject;
	ZeldaEngine::GameObject* ultimateCollisionLeftObject;
	ZeldaEngine::GameObject* ultimateCollisionRightObject;

	ZeldaEngine::GameObject* ultimateWarningTopObject;
	ZeldaEngine::GameObject* ultimateWarningLeftObject;

	ZeldaEngine::MeshRenderer* projectileMeshRenderer;

	// 콜라이더 오브젝트를 담아놓는 백터
	std::vector<std::pair<ZeldaEngine::GameObject*, double>> colliderobjectsPairVector;

	// collider?
	ZeldaEngine::CapsuleCollider* m_bossHitCollider;

	ZeldaEngine::CubeCollider* pattern1_1Collider;
	ZeldaEngine::CubeCollider* pattern1_2Collider;
	ZeldaEngine::CubeCollider* pattern2Collider;
	ZeldaEngine::CubeCollider* pattern3Collider1;
	ZeldaEngine::CubeCollider* pattern3Collider2;
	ZeldaEngine::CubeCollider* pattern5Collider1;
	ZeldaEngine::CylinderCollider* obstacleCollider[3];
	ZeldaEngine::SphereCollider* pattern5Collider2;
	ZeldaEngine::CylinderCollider* barrierCollider;
	ZeldaEngine::CylinderCollider* barrierTrigger;

	ZeldaEngine::Rigidbody* obstacleRigidBody[3];
	ZeldaEngine::Rigidbody* barrierRigidbody;

	ZeldaEngine::Collider2D* normal2DCollider[10];
	ZeldaEngine::Collider2D* ultimate2DCollider[10];

	ZeldaEngine::Image* normal2DImage[10];
	ZeldaEngine::Image* ultimate2DImage[10];
	ZeldaEngine::SpriteAnimator* normal2DSpriteAnimator[10];
	ZeldaEngine::SpriteAnimator* ultimateSpriteAnimator[10];

	ZeldaEngine::Image* normal2DWarningImage[10];

	ZeldaEngine::Image* ultimateWarningTopImage;
	ZeldaEngine::Image* ultimateWarningLeftImage;



	// collider가 붙을 오브젝트들에게 전부 붙여줄 컴포넌트.
	BossCollisionManager* pattern5CollisionManager;

	ZeldaEngine::SkinnedMeshRenderer* boss1SkinnedMeshRenderer;

	ZeldaEngine::Animator* m_animator;

	// Dotween Component
	Dotween* branch1Dotween;
	Dotween* branch2Dotween;
	Dotween* branchAxisDotween;
	Dotween* treeParentDotween;
	Dotween* protectEffectDotween;
	Dotween* protectEffectDotween2;

	Dotween* pattern2ColliderDotween;
	Dotween* horizontalDotween;
	Dotween* verticalDotween;
	Dotween* pattern5ObjectDotween;
	
	Dotween* ultimateObjectDotween;
	Dotween* ultimateColliderDotween;

	Dotween* timeDotween;

	Dotween* obstacleDotween[3];

	Dotween* barrierColliderDotween;

	std::vector<Dotween*> dotweenVector;

	// EffectPlayer
	EffectPlayer* branchEffectPlayers[4];
	EffectPlayer* circleEffectPlayer;
	EffectPlayer* teleportDisappearEffectPlayer;
	EffectPlayer* teleportAppearEffectPlayer;
	EffectPlayer* horizontalAttackEffectPlayer;
	EffectPlayer* verticalAttackEffectPlayer;
	EffectPlayer* knockBackAttackEffectPlayer;
	EffectPlayer* knockBackGuideEffectPlayer;
	EffectPlayer* skill2DEffectPlayer;
	EffectPlayer* chargeAttackEffectPlayer;
	EffectPlayer* chargeSonicEffectPlayer;
	EffectPlayer* chargeGuideEffectPlayer;
	EffectPlayer* normalGuideEffectPlayer;
	EffectPlayer* obstaclePatternEffectPlayer;
	EffectPlayer* rangedAttackEffectPlayer;
	EffectPlayer* explosionEffectPlayer;
	EffectPlayer* protectEffectPlayer;
	EffectPlayer* protectEffectPlayer2;


	EffectPlayer* disruptingAppearEffectPlayer[3];

// 상수 없이 최대한 변수로 빼주자
private:
	bool isStarted = true; 

	double firstDelay = 2.0f;

	int hpPerScroll;

	bool isReverted = false;

	bool isFloatObjectReverted = false;

	int finalRushCount;

	int currenthp;

	bool isPhaseJustChanged;

	int tempCount;								
	int temp2DPatternCount;

	int pattern2DFinalCount;

	int pattern2DFinalLineNumber;				
	
	int patternNum;	

	int previousStateNum;

	int phase2PatternNumber;			// phase2의 패턴 갯수

	int phase2RageOnPatternNumber;
	int phaseNumber;
	
	double bossRageLookDuration = 1.0f;

	int phase2DefaultSpeed;
	int phase2Default2DLineNumber;
	int phase2Default2DCount;

	int isFirstCharge = true;
private:
	float pattern6Radius = 16;
	float pattern6Angle = 90;

	float normal2DEffectSync = 0.24f;

	double ci = 0;

	bool reserve2DPattern = false;

	bool isPhase2BranchPatternUsed = false;

private:




private:
	// 패턴이 끝났을 때 True로 바꿔줄 bool 값.
	bool isPattern1End;
	bool isPattern2End;
	bool isPattern3End;
	bool isPattern5End;
	bool isPattern6End;
	bool isPattern2DNormalEnd;
	bool isDistruptingMovementEnd;
	bool isPattern2DUltimateEnd;
	bool jumpToNextPhase;

	double topWarningDegree;		// 아래로 내려오는 경고선
	double leftWarningDegree;		// 왼쪽 경고선

	bool isMoveEnd;
	bool isGroggyEnd;
	bool isPlayerNear;
	bool groggyPatternOn;

	bool BranchPattern1On;
	bool BranchPattern2On;
	bool BranchPattern3On;

	bool isBranchPattern1Used;
	bool isBranchPattern3Used;

	bool isJustDamaged;
	bool isHitPattern5;				// 원거리 평타 패턴 투사체에 맞았는가?

	bool phase1RageOn;					// 모든 패턴을 1.5배 빠르게 하는 광폭화 모드
	bool phase2RageOn;					// 2DNormalPattern을 1 -> 3회로 바꾸는 등 2페이즈 광폭화 모드

	bool rage1AnimationPlayed = false;
	bool rage2AnimationPlayed = false;

	bool isPattern6Damaged;
	bool isPhase2StartPatternUsed;

	double pattern2FinalRadian;
	double ultimateObjectDuration = 1.0f;

	double blockingAnimationEndTime = 0.4f;
	double potalOpenTime = 1.0f;
	double potalCloseTime = 3.0f;
	double obstacleArriveTime = 2.0f;

	double notMoveDistance = 6.0f;

	double walkSoundDelay = 0;

	EdgeDetect* bossEdge;

public:
	void SetGameManager(GameManager* gameManager);

	Boss1();
	void Awake() override;
	void Start() override;
	void Update() override;

	void IdleEngage();
	void DeathEngage();
	void Pattern1Engage();
	void Pattern2Engage();
	void Pattern3Engage();
	void Pattern5Engage();
	void Pattern6Engage();
	void PatternMoveEngage();
	void GroggyEngage();

	void Pattern2DNormalEngage();
	void DisruptingMovementEngage();
	void Pattern2DUltimateEngage();


	/// <summary>
	/// boss 패턴 기능 함수. Engage에서 호출할 예정...
	/// </summary>
	void Pattern1Function(ZeldaEngine::ZeldaMath::Vector3D branchScale, double endRotation, double rotateSpeed, easing_functions easeFunc = easing_functions::EaseInSine);			// 가지 범위 공격 패턴
	void Pattern2Function(int rushCount, double delay, double speed, easing_functions easeFunc = easing_functions::EaseInSine);											// 돌진 패턴
	void Pattern3Function(double speed, double walkSpeed, int attackCount);														// 평타1 패턴
	void Pattern5Function(double speed);
	void Pattern6Function(double speed, double walkSpeed);
	// Pattern7Function() -> 횡 만드는 2D 패턴 구현 예정

	void PatternMoveFunction(ZeldaEngine::ZeldaMath::Vector3D position, bool isPattern, double duration = 1.0f, double speed = 0.0f);													// 평타2 패턴 - 원뿔 모양의 Collider

	void Pattern2DNormalFunction(int count, int lineNumber);
	void DisruptingMovementFunction();
	void Pattern2DUltimateFunction();

	void ChoosePattern();

	// 페이즈 별 데미지 적용 로직
	void ApplyDamage(int phasenum, bool isRageOn);

	void RevertToPhaseTwo();

	void LookAt(ZeldaEngine::GameObject* obj, ZeldaEngine::ZeldaMath::Vector3D lookPosition);

	void SetPlayerTransform(ZeldaEngine::Transform* playerTransform);
	void SetPlayerCollider(ZeldaEngine::CapsuleCollider* playerCollider);
	void SetBossHitCollider(ZeldaEngine::CapsuleCollider* bossCollider);
	void SetPlayerRigidBody(ZeldaEngine::Rigidbody* playerRigidBody);
	void SetPlayerGameObject(ZeldaEngine::GameObject* playerGameObject);
	void SetPlayerComponent(Player* playerComponent);
	void SetBossHitSpriteAnimator(ZeldaEngine::SpriteAnimator* hitanimator);
	void SetScrollComponent(FloatingObject* floatingComponent);
	void SetBossFlagComponent(BossFlag* flagComponent);
	void SetFollowCameraComponent(FollowCamera* followComponent);
	void Pattern5Hit();
	void EdgeOff();


	ZeldaEngine::CapsuleCollider* GetBossCollider();

private:
	float branchPatternEffectSpeed = 0.2f;
	float branchPatternEffectRemainedTime = 10.0f;

	float circlePatternEffectSpeed = 0.2f;
	float circleEffectRemainedTime = 10.0f;

	// Teleport
	float teleportEffectSpeed = 0.2f;
	float teleportEffectRemainedTime = 10.0f;

	float protectEffectSpeed = 0.7f;
	float protectEffectRemainedTime = 30.0f;

	float horizontalAttackEffectSpeed = 1.3f;
	float horizontalAttackEffectRemainedTime = 3.5f;

	float verticalAttackEffectSpeed = 1.3f;
	float verticalAttackEffectRemainedTime = 3.5f;

	float rangedAttackEffectSpeed = 1.2f;
	float rangedAttackEffectRemainedTime = 3.5f;

	float explosionEffectSpeed = 0.8f;
	float explosionEffectRemainedTime = 3.5f;

	float knockBackAttackEffectSpeed = 1.3f;
	float knockBackAttackEffectRemainedTime = 3.5f;

	float skill2DEffectSpeed = 1.0f;
	float skill2DEffectRemainedTime = 3.5f;

	float chargeAttackEffectSpeed = 0.5f;
	float chargeAttackRemainedTime = 1.0f;

	float chargeSonicEffectSpeed = 1.2f;
	float chargeSonicRemainedTime = 5.0f;

	float chargeGuideEffectSpeed = 0.2f;
	float chargeGuideRemainedTime = 3.5f;

	float normalGuideEffectSpeed = 0.2f;
	float normalGuideRemainedTime = 3.5f;

	float blockingEffectSpeed = 1.3f;
	float blockingEffectRemainedTime = 3.5f;

	float knockBackGuideEffectSpeed = 0.65f;
	float knockBackGuideEffectRemainedTime = 3.5f;

	float normal2DAnimationSpeed = 25.0f;

public:
	virtual void OnDamaged(float damage, int type) override;



	///-----------------------------------------에디터에서 조절 가능한 멤버변수들-----------------------------------------
private:
	int maxHp;
	double phase1RageSpeed;
	double phase2RageSpeed;
	double phase1Speed;
	double phase2Speed;
	int nomalRushCount;
	int rageRushCount;
	int pattern2DNormalLineNumber;
	int pattern2DRageLineNumber;
	int pattern2DNormalCount;
	int pattern2DRageCount;
	float ultimateWarningTime;
	double normalAttackDelay;
	double normalAttackStartDelay;
	double defaultMoveSpeed;
	double arriveTimePattern5;
	double lastTimePattern5;
	double guideLineRemainTime;
	double groggyTime;
	double branchDamage;
	double chargeDamage;
	double normalAttackDamage;
	double rangedAttackDamage;
	double fanDamage;

private:
	// Component을(를) 통해 상속됨
	virtual void PreSerialize(json& jsonData) const override;
	virtual void PreDeserialize(const json& jsonData) override;
	virtual void PostSerialize(json& jsonData) const override;
	virtual void PostDeserialize(const json& jsonData) override;
};

