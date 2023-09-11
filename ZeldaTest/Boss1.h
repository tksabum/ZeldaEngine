#pragma once
#include "ZeldaEngine.h"
#include "FSM.h"
#include "Easing.h"
#include "IHPObject.h"
#include <stack>
#include <unordered_map>

/// <summary>
/// ���������� ���� fsm�� Dotween�� �� �Ἥ Boss1�� �����ϰ� ������.
/// ���¿��� ũ�� Idle�� Pattern �� ������ �ִ�.
/// ��� Pattern�� ���� �� Idle�� ��ȯ, Idle ���� �� ���� �����ؼ� Pattern�� ������ ����.
/// </summary>

/// <summary>
/// ���ο� ������ ���鶩 �� Idle ���� ������ֱ�!
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

	// �÷��̾��� ������ ��������
	ZeldaEngine::Transform* m_playerTransform;
	ZeldaEngine::GameObject* m_playerGameObject;
	ZeldaEngine::CapsuleCollider* m_playerCollider;
	ZeldaEngine::Rigidbody* m_playerRigidBody;
	Player* m_playerComponent;
	FollowCamera* m_followCamera;

	ZeldaEngine::SpriteAnimator* bossHitSpriteAnimator;

	// ���� component�� �����´�
	FloatingObject* tempFloatingComponent;
	std::vector<FloatingObject*> floatingObjectComponentVector;
	std::vector<FloatingObject*>::iterator m_floatingObjectIter;

	// bossFlag component�� �����´�
	BossFlag* tempbossComponent;
	std::vector<BossFlag*> bossFlagComponentVector;
	std::vector<BossFlag*>::iterator m_bossFlagComponentIter;

	// Boss�� ���� ���� �� ����� ������Ʈ.
	ZeldaEngine::GameObject* timeDotweenObject;

	ZeldaEngine::GameObject* bossParticleObject;

	ZeldaEngine::GameObject* fanObject;

	ZeldaEngine::GameObject* branch12ColliderObject;
	ZeldaEngine::GameObject* branch34ColliderObject;					// ��ȭ ������ ������ �� ��
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
	
	ZeldaEngine::GameObject* rangedAttackEffectObject;		// ���� �׿��� ������ ����Ʈ
	ZeldaEngine::GameObject* rangedExposionEffectObject;	// ����ü ���� �� ���� ����Ʈ

	ZeldaEngine::GameObject* obstaclePatternEffectObject;

	ZeldaEngine::GameObject* horizontalObject;
	ZeldaEngine::GameObject* verticalObject;

	ZeldaEngine::GameObject* pattern2ColliderObject;

	ZeldaEngine::GameObject* projectileObject;
	ZeldaEngine::GameObject* childProjectileObject;
	ZeldaEngine::GameObject* pattern5ColliderObject1;				// ���Ÿ� ���� ��������

	ZeldaEngine::GameObject* normal2DCollisionObject[10];			// �̸� 10������ ����
	ZeldaEngine::GameObject* normal2DWarningObject[10];			
	
	ZeldaEngine::GameObject* ultimateCollisionObject[10];			// �̸� 10������ ����
	
	ZeldaEngine::GameObject* bossFrontObject;	

	ZeldaEngine::GameObject* rageTextureObject;			
	ZeldaEngine::Image* rageTextureImage;

	// �̹����� ���� ������Ʈ
	ZeldaEngine::GameObject* ultimateBottomObject;
	ZeldaEngine::GameObject* ultimateTopObject;
	ZeldaEngine::GameObject* ultimateLeftObject;
	ZeldaEngine::GameObject* ultimateRightObject;

	// ���� �ݶ��̴��� ���� ������Ʈ
	ZeldaEngine::GameObject* ultimateCollisionBottomObject;
	ZeldaEngine::GameObject* ultimateCollisionTopObject;
	ZeldaEngine::GameObject* ultimateCollisionLeftObject;
	ZeldaEngine::GameObject* ultimateCollisionRightObject;

	ZeldaEngine::GameObject* ultimateWarningTopObject;
	ZeldaEngine::GameObject* ultimateWarningLeftObject;

	ZeldaEngine::MeshRenderer* projectileMeshRenderer;

	// �ݶ��̴� ������Ʈ�� ��Ƴ��� ����
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



	// collider�� ���� ������Ʈ�鿡�� ���� �ٿ��� ������Ʈ.
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

// ��� ���� �ִ��� ������ ������
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

	int phase2PatternNumber;			// phase2�� ���� ����

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
	// ������ ������ �� True�� �ٲ��� bool ��.
	bool isPattern1End;
	bool isPattern2End;
	bool isPattern3End;
	bool isPattern5End;
	bool isPattern6End;
	bool isPattern2DNormalEnd;
	bool isDistruptingMovementEnd;
	bool isPattern2DUltimateEnd;
	bool jumpToNextPhase;

	double topWarningDegree;		// �Ʒ��� �������� ���
	double leftWarningDegree;		// ���� ���

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
	bool isHitPattern5;				// ���Ÿ� ��Ÿ ���� ����ü�� �¾Ҵ°�?

	bool phase1RageOn;					// ��� ������ 1.5�� ������ �ϴ� ����ȭ ���
	bool phase2RageOn;					// 2DNormalPattern�� 1 -> 3ȸ�� �ٲٴ� �� 2������ ����ȭ ���

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
	/// boss ���� ��� �Լ�. Engage���� ȣ���� ����...
	/// </summary>
	void Pattern1Function(ZeldaEngine::ZeldaMath::Vector3D branchScale, double endRotation, double rotateSpeed, easing_functions easeFunc = easing_functions::EaseInSine);			// ���� ���� ���� ����
	void Pattern2Function(int rushCount, double delay, double speed, easing_functions easeFunc = easing_functions::EaseInSine);											// ���� ����
	void Pattern3Function(double speed, double walkSpeed, int attackCount);														// ��Ÿ1 ����
	void Pattern5Function(double speed);
	void Pattern6Function(double speed, double walkSpeed);
	// Pattern7Function() -> Ⱦ ����� 2D ���� ���� ����

	void PatternMoveFunction(ZeldaEngine::ZeldaMath::Vector3D position, bool isPattern, double duration = 1.0f, double speed = 0.0f);													// ��Ÿ2 ���� - ���� ����� Collider

	void Pattern2DNormalFunction(int count, int lineNumber);
	void DisruptingMovementFunction();
	void Pattern2DUltimateFunction();

	void ChoosePattern();

	// ������ �� ������ ���� ����
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



	///-----------------------------------------�����Ϳ��� ���� ������ ���������-----------------------------------------
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
	// Component��(��) ���� ��ӵ�
	virtual void PreSerialize(json& jsonData) const override;
	virtual void PreDeserialize(const json& jsonData) override;
	virtual void PostSerialize(json& jsonData) const override;
	virtual void PostDeserialize(const json& jsonData) override;
};

