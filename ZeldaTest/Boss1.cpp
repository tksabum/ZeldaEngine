#include "Boss1.h"
#include "ObjectPool.h"
#include "Dotween.h"
#include "DotweenTimer.h"
#include "NDComponent.h"
#include "BossCollisionManager.h"
#include "EffectPlayer.h"
#include "FloatingObject.h"
#include "BossFlag.h"
#include "GameManager.h"
#include "FollowCamera.h"
#include "Guilty.h"
#include "EdgeDetect.h"

using namespace ZeldaEngine;
using namespace ZeldaEngine::ZeldaMath;

void Boss1::SetGameManager(GameManager* gameManager)
{
	this->gameManager = gameManager;
}

Boss1::Boss1()
{
	// 에디터에서 설정 가능한 변수들을 생성자에서 초기화하기
	maxHp = 32;
	phaseNumber = 1;		// 에디터에서 페이즈 번호를 설정하면 해당 페이즈부터 실행됐으면...
	phase1Speed = 1.0f;
	phase2Speed = 1.0f;
	phase1RageSpeed = 1.5f;
	phase2RageSpeed = 1.5f;
	nomalRushCount = 2;
	rageRushCount = 2;
	pattern2DNormalLineNumber = 3;
	pattern2DRageLineNumber = 6;
	pattern2DNormalCount = 2;
	pattern2DRageCount = 2;
	ultimateWarningTime = 1.8f;
	normalAttackDelay = 0.2f;
	lastTimePattern5 = 0.6f;
	normalAttackStartDelay = 0.6f;
	defaultMoveSpeed = 15.0f;
	arriveTimePattern5 = 1.0f;
	guideLineRemainTime = 0.6f;		// 부채꼴 가이드라인 지속 시간
	groggyTime = 10.0f;
	branchDamage = 10.0f;
	chargeDamage = 6.0f;
	normalAttackDamage = 7.0f;
	rangedAttackDamage = 7.0f;
	fanDamage = 6.0f;
}

void Boss1::Awake()
{
	// 멤버들을 초기화 해준다.
	hpPerScroll = maxHp / 8;

	currenthp = maxHp;				// Hp : 족자 하나 당 타수 * 족자 갯수
	//currenthp = 1;				// Hp : 족자 하나 당 타수 * 족자 갯수

	phaseNumber = 1;

	finalRushCount = nomalRushCount;

	pattern2DFinalLineNumber = pattern2DNormalLineNumber;

	pattern2DFinalCount = pattern2DNormalCount;

	tempCount = 0;

	phase2PatternNumber = 6;
	phase2RageOnPatternNumber = 10;

	phase2DefaultSpeed = phase2Speed;
	phase2Default2DLineNumber = pattern2DNormalLineNumber;
	phase2Default2DCount = pattern2DNormalCount;

	phase1RageOn = false;

	isPattern1End = false;
	isPattern2End = false;
	isPattern3End = false;
	isPattern5End = false;
	isPattern6End = false;

	isPattern2DNormalEnd = false;
	isDistruptingMovementEnd = false;
	isPattern2DUltimateEnd = false;

	isBranchPattern1Used = false;
	isBranchPattern3Used = false;

	isJustDamaged = false;

	boss1FSM = new FSM<Boss1State>(Boss1State::Idle);

	m_gameObject = GetGameObject();

	// Pattern 관련 오브젝트 생성
	boss1TransformObject = SceneManager::GetInstance().CreateGameObject(L"boss1TransformObject");
	branch12ColliderObject = SceneManager::GetInstance().CreateGameObject(L"branch1Object");
	branch34ColliderObject = SceneManager::GetInstance().CreateGameObject(L"branch2Object");

	for (int i = 0; i < 4; i++)
	{
		branchEffectObjects[i] = CreateGameObjectFromFBX(L"VFX_Tree");
	}
	treeParent = SceneManager::GetInstance().CreateGameObject(L"treeParent");

	circleEffectObject = CreateGameObjectFromFBX(L"VFX_Circle");

	teleportDisappearEffectObject = CreateGameObjectFromFBX(L"VFX_Teleport");
	teleportAppearEffectObject = CreateGameObjectFromFBX(L"VFX_Teleport");

	for (int i = 0; i < 3; i++)
	{
		disruptingAppearEffectObject[i] = CreateGameObjectFromFBX(L"VFX_Circle");
		disruptingObjects[i] = CreateGameObjectFromFBX(L"BossObstacle");
		disruptingColliderObjects[i] = SceneManager::GetInstance().CreateGameObject(L"disruptingColliderObjects");
	}

	horizontalAttackEffectObject = CreateGameObjectFromFBX(L"VFX_BossNormalAttack_L");
	verticalAttackEffectObject = CreateGameObjectFromFBX(L"VFX_BossNormalAttack_R");
	protectEffectObject = CreateGameObjectFromFBX(L"VFX_Protect");
	protectEffectObject2 = CreateGameObjectFromFBX(L"VFX_Protect");

	rangedAttackEffectObject = CreateGameObjectFromFBX(L"VFX_RangedAttack");
	rangedExposionEffectObject = CreateGameObjectFromFBX(L"VFX_Explosion");
	projectileObject = CreateGameObjectFromFBX(L"BossProjectile");
	childProjectileObject = CreateGameObjectFromFBX(L"BossProjectile_Outer");

	knockBackAttackEffectObject = CreateGameObjectFromFBX(L"VFX_Knockback");
	knockBackGuideLineObject = CreateGameObjectFromFBX(L"VFX_KnockbackGuide");

	skill2DEffectObject = CreateGameObjectFromFBX(L"VFX_Skill2D");
	chargeAttackEffectObject = CreateGameObjectFromFBX(L"VFX_ChargeAttack");
	chargeSonicEffectObject = CreateGameObjectFromFBX(L"VFX_ChargeSonic");
	chargeGuideEffectObject = CreateGameObjectFromFBX(L"VFX_ChargeAttackGuide");
	normalAttackGuideObject = CreateGameObjectFromFBX(L"VFX_BossNormalAttackGuide");

	obstaclePatternEffectObject = CreateGameObjectFromFBX(L"VFX_Blocking");

	horizontalObject = SceneManager::GetInstance().CreateGameObject(L"horizontalObject");
	verticalObject = SceneManager::GetInstance().CreateGameObject(L"verticalObject");

	pattern2ColliderObject = SceneManager::GetInstance().CreateGameObject(L"pattern2ColliderObject");

	pattern5ColliderObject1 = SceneManager::GetInstance().CreateGameObject(L"pattern5ColliderObject1");

	for (int i = 0; i < 10; i++)
	{
		normal2DCollisionObject[i] = SceneManager::GetInstance().CreateGameObject(L"normal2DCollisionObject");
		ultimateCollisionObject[i] = SceneManager::GetInstance().CreateGameObject(L"ultimateCollisionObject");
		normal2DWarningObject[i] = SceneManager::GetInstance().CreateGameObject(L"normal2DWarningObject");
	}

	rageTextureObject = SceneManager::GetInstance().CreateGameObject(L"rageTextureObject");

	ultimateBottomObject = SceneManager::GetInstance().CreateGameObject(L"ultimateBottomObject");
	ultimateTopObject = SceneManager::GetInstance().CreateGameObject(L"ultimateTopObject");
	ultimateLeftObject = SceneManager::GetInstance().CreateGameObject(L"ultimateLeftObject");
	ultimateRightObject = SceneManager::GetInstance().CreateGameObject(L"ultimateRightObject");

	ultimateCollisionBottomObject = SceneManager::GetInstance().CreateGameObject(L"ultimateCollisionBottomObject");
	ultimateCollisionTopObject = SceneManager::GetInstance().CreateGameObject(L"ultimateCollisionTopObject");
	ultimateCollisionLeftObject = SceneManager::GetInstance().CreateGameObject(L"ultimateCollisionLeftObject");
	ultimateCollisionRightObject = SceneManager::GetInstance().CreateGameObject(L"ultimateCollisionRightObject");

	ultimateWarningTopObject = SceneManager::GetInstance().CreateGameObject(L"ultimateWarningTopObject");
	ultimateWarningLeftObject = SceneManager::GetInstance().CreateGameObject(L"ultimateWarningLeftObject");

	barrierCollisionObject = SceneManager::GetInstance().CreateGameObject(L"barrierCollisionObject");
	barrierTriggerObject = SceneManager::GetInstance().CreateGameObject(L"barrierTriggerObject");
	bossFrontObject = SceneManager::GetInstance().CreateGameObject(L"bossFrontObject");

	bossParticleObject = SceneManager::GetInstance().CreateGameObject(L"bossParticleObject");

	fanObject = SceneManager::GetInstance().CreateGameObject(L"fanObject");

	if (GetGameObject()->GetTransform()->GetChild(L"boss2DHitOjbect") != nullptr)
	{
		bossEdge = GetGameObject()->GetTransform()->GetChild(L"boss2DHitOjbect")->GetGameObject()->GetComponent<EdgeDetect>();
	}
}

void Boss1::Start()
{
	// Boss1 컴포넌트를 AddComponent 할 GameObject에
	// Boss1이 들고 있는 Dotween을 AddComponent해준다.
	boss1Dotween = m_gameObject->AddComponent<Dotween>();

	/*------------------------------------------------------------- FSM Setting-------------------------------------------------------------*/
	/// FSM - transition, engageAction, updateAction 등을 세팅해준다.
	// 상태 변경 조건에 대한 정보는 transitions에 담는다.
	//Idle -> Pattern
	boss1FSM->transitions[Boss1State::Idle].push_back({ Boss1State::Pattern1,
	[this]() {return patternNum == int(Boss1State::Pattern1); } });

	boss1FSM->transitions[Boss1State::Idle].push_back({ Boss1State::Pattern2,
	[this]() {return patternNum == int(Boss1State::Pattern2); } });

	boss1FSM->transitions[Boss1State::Idle].push_back({ Boss1State::Pattern3,
	[this]() {return patternNum == int(Boss1State::Pattern3); } });

	boss1FSM->transitions[Boss1State::Idle].push_back({ Boss1State::Pattern5,
	[this]() {return patternNum == int(Boss1State::Pattern5); } });

	boss1FSM->transitions[Boss1State::Idle].push_back({ Boss1State::Pattern6,
	[this]() {return patternNum == int(Boss1State::Pattern6); } });

	boss1FSM->transitions[Boss1State::Idle].push_back({ Boss1State::PatternMove,
	[this]() {return patternNum == int(Boss1State::PatternMove); } });

	boss1FSM->transitions[Boss1State::Idle].push_back({ Boss1State::Pattern2DNormal,
	[this]() {return patternNum == int(Boss1State::Pattern2DNormal); } });

	boss1FSM->transitions[Boss1State::Idle].push_back({ Boss1State::DisruptingMovement,
	[this]() {return patternNum == int(Boss1State::DisruptingMovement); } });

	boss1FSM->transitions[Boss1State::Idle].push_back({ Boss1State::Pattern2DUltimate,
	[this]() {return patternNum == int(Boss1State::Pattern2DUltimate); } });

	// Death는 pattern2 ~ idle까지의 모든 상태에서 들어갈 수 있는 상태이다.
	//for (int i = 0; i < int(Boss1State::Death); i++)
	//{
	//	Boss1State tempState = static_cast<Boss1State>(i);
	//	boss1FSM->transitions[tempState].push_back({ Boss1State::Death,
	//	[this]() {return m_hp <= 0; } });
	//}

	// 제 2안. 하던 패턴을 종료하고 사망한다 -> idle에서 death만 가능하게
	boss1FSM->transitions[Boss1State::Idle].push_back({ Boss1State::Death,
	[this]() {	return patternNum == int(Boss1State::Death); } });


	// Pattern -> Idle
	boss1FSM->transitions[Boss1State::Pattern1].push_back({ Boss1State::Idle,
	[this]() {return (isPattern1End && !groggyPatternOn) || jumpToPhase2; } });

	boss1FSM->transitions[Boss1State::Pattern2].push_back({ Boss1State::Idle,
	[this]() {return isPattern2End || jumpToPhase2; } });

	boss1FSM->transitions[Boss1State::Pattern3].push_back({ Boss1State::Idle,
	[this]() {return isPattern3End || jumpToPhase2; } });

	boss1FSM->transitions[Boss1State::Pattern5].push_back({ Boss1State::Idle,
	[this]() {return isPattern5End || jumpToPhase2; } });

	boss1FSM->transitions[Boss1State::Pattern6].push_back({ Boss1State::Idle,
	[this]() {return isPattern6End || jumpToPhase2; } });

	boss1FSM->transitions[Boss1State::PatternMove].push_back({ Boss1State::Idle,
	[this]() {return isMoveEnd || jumpToPhase2; } });

	boss1FSM->transitions[Boss1State::Groggy].push_back({ Boss1State::Idle,
	[this]() {return isGroggyEnd || jumpToPhase2; } });

	boss1FSM->transitions[Boss1State::Pattern2DNormal].push_back({ Boss1State::Idle,
	[this]() {return isPattern2DNormalEnd || jumpToPhase2; } });

	boss1FSM->transitions[Boss1State::DisruptingMovement].push_back({ Boss1State::Idle,
	[this]() {return isDistruptingMovementEnd || jumpToPhase2; } });

	boss1FSM->transitions[Boss1State::Pattern2DUltimate].push_back({ Boss1State::Idle,
	[this]() {return isPattern2DUltimateEnd || jumpToPhase2; } });

	boss1FSM->transitions[Boss1State::Death].push_back({ Boss1State::Idle,
	[this]() {return jumpToNextPhase || jumpToPhase2; } });

	// 그래도 Groggy에서 Death로 가는 길은 뚫어 놓자
	boss1FSM->transitions[Boss1State::Groggy].push_back({ Boss1State::Death,
	[this]() {	return patternNum == int(Boss1State::Death); } });

	// 1페이즈 마지막 패턴1 -> 그로기 상태 전환
	boss1FSM->transitions[Boss1State::Pattern1].push_back({ Boss1State::Groggy,
	[this]() {return isPattern1End && groggyPatternOn; } });

	// 원거리 패턴은 거리가 멀 때만
	// 대신 원거리 패턴 때 거리가 가까우면, 바로 Pattern3 (근접 평타)로 전환해 주는 건 어떠한가
	boss1FSM->transitions[Boss1State::Pattern5].push_back({ Boss1State::Pattern3,
	[this]() {return isPlayerNear; } });

	boss1FSM->transitions[Boss1State::Pattern2].push_back({ Boss1State::DisruptingMovement,
	[this]() {return SceneManager::GetInstance().GetName() == L"BossPhase2Scene" && isStarted; } });

	//boss1FSM->transitions[Boss1State::Pattern2].push_back({ Boss1State::DisruptingMovement,
	//[this]() {return SceneManager::GetInstance().GetName() == L"BossPhase2Scene"; }});

	// 특정 상태의 한번만 호출되는 로직은 engageAction에 담는다.
	boss1FSM->engageAction[Boss1State::Pattern1] = [this]() { Pattern1Engage(); };
	boss1FSM->engageAction[Boss1State::Pattern2] = [this]() { Pattern2Engage(); };
	boss1FSM->engageAction[Boss1State::Pattern3] = [this]() { Pattern3Engage(); };
	boss1FSM->engageAction[Boss1State::Pattern5] = [this]() { Pattern5Engage(); };
	boss1FSM->engageAction[Boss1State::Pattern6] = [this]() { Pattern6Engage(); };
	boss1FSM->engageAction[Boss1State::PatternMove] = [this]() { PatternMoveEngage(); };
	boss1FSM->engageAction[Boss1State::Groggy] = [this]() { GroggyEngage(); };
	boss1FSM->engageAction[Boss1State::Idle] = [this]() { IdleEngage(); };
	boss1FSM->engageAction[Boss1State::Death] = [this]() { DeathEngage(); };

	boss1FSM->engageAction[Boss1State::Pattern2DNormal] = [this]() { Pattern2DNormalEngage(); };
	boss1FSM->engageAction[Boss1State::DisruptingMovement] = [this]() { DisruptingMovementEngage(); };
	boss1FSM->engageAction[Boss1State::Pattern2DUltimate] = [this]() { Pattern2DUltimateEngage(); };

	/*------------------------------------------------------------- Object Component Setting-------------------------------------------------------------*/
	branch12ColliderObject->SetActive(false);
	branch34ColliderObject->SetActive(false);
	horizontalObject->SetActive(false);
	verticalObject->SetActive(false);
	projectileObject->SetActive(false);
	pattern5ColliderObject1->SetActive(false);
	pattern2ColliderObject->SetActive(false);

	//m_playerCollider = m_playerTransform->GetGameObject()->GetComponent<CapsuleCollider>();

	// 충돌 판정 예시
	boss1SkinnedMeshRenderer = GetGameObject()->GetTransform()->GetChild(0)->GetGameObject()->GetComponent<SkinnedMeshRenderer>();
	projectileMeshRenderer = projectileObject->GetTransform()->GetChild(0)->GetGameObject()->GetComponent<MeshRenderer>();


	// 일반
	pattern1_1Collider = branch12ColliderObject->AddComponent<CubeCollider>();
	pattern1_1Collider->SetTrigger(true);
	branch12ColliderObject->SetLayerName(L"NormalAttackObject");

	pattern1_2Collider = branch34ColliderObject->AddComponent<CubeCollider>();
	pattern1_2Collider->SetTrigger(true);
	branch34ColliderObject->SetLayerName(L"NormalAttackObject");

	pattern2Collider = pattern2ColliderObject->AddComponent<CubeCollider>();
	pattern2Collider->SetTrigger(true);
	pattern2ColliderObject->SetLayerName(L"NormalAttackObject");

	pattern3Collider1 = horizontalObject->AddComponent<CubeCollider>();
	pattern3Collider1->SetTrigger(true);
	horizontalObject->SetLayerName(L"NormalAttackObject");

	pattern3Collider2 = verticalObject->AddComponent<CubeCollider>();
	pattern3Collider2->SetTrigger(true);
	verticalObject->SetLayerName(L"NormalAttackObject");

	pattern5Collider1 = pattern5ColliderObject1->AddComponent<CubeCollider>();
	pattern5Collider1->SetTrigger(true);
	pattern5ColliderObject1->SetLayerName(L"NormalAttackObject");

	pattern5Collider2 = projectileObject->AddComponent<SphereCollider>();
	pattern5Collider2->SetTrigger(true);
	projectileObject->SetLayerName(L"NormalAttackObject");

	bossParticleObject->AddComponent<ParticleRenderer>();
	bossParticleObject->SetActive(false);
	bossParticleObject->GetTransform()->SetParent(m_gameObject->GetTransform());
	bossParticleObject->GetTransform()->SetLocalPosition(Vector3D(0, 0, 0));

	std::vector<std::wstring> normal2DAnimationVector;

	normal2DAnimationVector.insert(normal2DAnimationVector.end(), { L"Normal2DPatternTexture_0",
		L"Normal2DPatternTexture_1",
		L"Normal2DPatternTexture_2",
		L"Normal2DPatternTexture_3",
		L"Normal2DPatternTexture_4",
		L"Normal2DPatternTexture_5" });

	for (int i = 0; i < 10; i++)
	{
		// GetComponent 대신 미리 생성 후 AddComponent 해주기
		normal2DCollider[i] = normal2DCollisionObject[i]->AddComponent<Collider2D>();
		normal2DCollider[i]->SetShape(Collider2D::Box);
		normal2DImage[i] = normal2DCollisionObject[i]->AddComponent<Image>();
		normal2DImage[i]->SetLayer(300);
		normal2DSpriteAnimator[i] = normal2DCollisionObject[i]->AddComponent<SpriteAnimator>();
		normal2DCollisionObject[i]->AddComponent<ZeldaUI>();
		normal2DCollisionObject[i]->SetLayerName(L"boss2DAttackCollider");
		normal2DCollisionObject[i]->SetActive(false);

		normal2DSpriteAnimator[i]->SetTexture(normal2DAnimationVector);
		normal2DSpriteAnimator[i]->SetLoop(false);
		normal2DSpriteAnimator[i]->SetPlaySpeed(normal2DAnimationSpeed);
		normal2DSpriteAnimator[i]->SetPlayWithAwake(false);

		normal2DWarningImage[i] = normal2DWarningObject[i]->AddComponent<Image>();
		normal2DWarningImage[i]->SetTexture(L"Normal2DPatternTexture_Warning");
		normal2DWarningImage[i]->SetLayer(300);
		normal2DWarningObject[i]->AddComponent<ZeldaUI>();
		normal2DWarningObject[i]->SetActive(false);

		normal2DCollisionObject[i]->GetTransform()->SetLocalScale(Vector3D(2400, 100.0f, 0));
		normal2DWarningObject[i]->GetTransform()->SetLocalScale(Vector3D(2400, 100.0f, 0));

		ultimate2DCollider[i] = ultimateCollisionObject[i]->AddComponent<Collider2D>();
		ultimate2DCollider[i]->SetShape(Collider2D::Box);
		ultimate2DImage[i] = ultimateCollisionObject[i]->AddComponent<Image>();
		ultimate2DImage[i]->SetLayer(300);
		ultimateSpriteAnimator[i] = ultimateCollisionObject[i]->AddComponent<SpriteAnimator>();
		ultimateCollisionObject[i]->AddComponent<ZeldaUI>();
		ultimateCollisionObject[i]->SetLayerName(L"boss2DAttackCollider");
		ultimateCollisionObject[i]->SetActive(false);
		ultimateSpriteAnimator[i]->SetTexture(normal2DAnimationVector);
		ultimateSpriteAnimator[i]->SetLoop(false);
		ultimateSpriteAnimator[i]->SetPlaySpeed(normal2DAnimationSpeed);
		ultimateSpriteAnimator[i]->SetPlayWithAwake(false);
		ultimateCollisionObject[i]->GetTransform()->SetLocalScale(Vector3D(2400, 100.0f, 0));

	}

	ultimateBottomObject->AddComponent<ZeldaUI>();
	ultimateTopObject->AddComponent<ZeldaUI>();
	ultimateLeftObject->AddComponent<ZeldaUI>();
	ultimateRightObject->AddComponent<ZeldaUI>();

	rageTextureImage = rageTextureObject->AddComponent<Image>();
	rageTextureImage->SetTexture(L"boss_rageline");
	rageTextureObject->AddComponent<ZeldaUI>();
	rageTextureObject->GetTransform()->SetLocalScale(Vector3D(1920, 1080, 0));
	rageTextureObject->SetActive(false);

	ultimateCollisionBottomObject->SetLayerName(L"boss2DAttackCollider");
	ultimateCollisionTopObject->SetLayerName(L"boss2DAttackCollider");
	ultimateCollisionLeftObject->SetLayerName(L"boss2DAttackCollider");
	ultimateCollisionRightObject->SetLayerName(L"boss2DAttackCollider");

	ultimateCollisionBottomObject->AddComponent<ZeldaUI>();
	ultimateCollisionTopObject->AddComponent<ZeldaUI>();
	ultimateCollisionLeftObject->AddComponent<ZeldaUI>();
	ultimateCollisionRightObject->AddComponent<ZeldaUI>();

	ultimateWarningTopImage = ultimateWarningTopObject->AddComponent<Image>();
	ultimateWarningTopImage->SetLayer(300);
	ultimateWarningTopImage->SetTexture(L"widebrush_horizontal_warning");
	ultimateWarningTopObject->AddComponent<ZeldaUI>();

	ultimateWarningLeftImage = ultimateWarningLeftObject->AddComponent<Image>();
	ultimateWarningLeftImage->SetLayer(300);
	ultimateWarningLeftImage->SetTexture(L"widebrush_vertical_warning");
	ultimateWarningLeftObject->AddComponent<ZeldaUI>();

	// 스케일 조정
	ultimateBottomObject->GetTransform()->SetLocalScale(Vector3D(1920, 1080, 0));
	ultimateTopObject->GetTransform()->SetLocalScale(Vector3D(1920, 1080, 0));
	ultimateLeftObject->GetTransform()->SetLocalScale(Vector3D(1920, 1080, 0));
	ultimateRightObject->GetTransform()->SetLocalScale(Vector3D(1920, 1080, 0));

	ultimateCollisionBottomObject->GetTransform()->SetLocalScale(Vector3D(1920, 1080, 0));
	ultimateCollisionTopObject->GetTransform()->SetLocalScale(Vector3D(1920, 1080, 0));
	ultimateCollisionLeftObject->GetTransform()->SetLocalScale(Vector3D(1920, 1080, 0));
	ultimateCollisionRightObject->GetTransform()->SetLocalScale(Vector3D(1920, 1080, 0));

	ultimateWarningTopObject->GetTransform()->SetLocalScale(Vector3D(1920, 1080, 0));
	ultimateWarningLeftObject->GetTransform()->SetLocalScale(Vector3D(1920, 1080, 0));

	// 시작 위치 설정
	ultimateBottomObject->GetTransform()->SetLocalPosition(Vector3D(0, -540, 0));
	ultimateTopObject->GetTransform()->SetLocalPosition(Vector3D(0, 540, 0));
	ultimateLeftObject->GetTransform()->SetLocalPosition(Vector3D(-960, 0, 0));
	ultimateRightObject->GetTransform()->SetLocalPosition(Vector3D(960, 0, 0));

	ultimateCollisionBottomObject->GetTransform()->SetLocalPosition(Vector3D(0, -540, 0));
	ultimateCollisionTopObject->GetTransform()->SetLocalPosition(Vector3D(0, 540, 0));
	ultimateCollisionLeftObject->GetTransform()->SetLocalPosition(Vector3D(-960, 0, 0));
	ultimateCollisionRightObject->GetTransform()->SetLocalPosition(Vector3D(960, 0, 0));

	ultimateCollisionBottomObject->GetTransform()->SetParent(ultimateBottomObject->GetTransform());
	ultimateCollisionTopObject->GetTransform()->SetParent(ultimateTopObject->GetTransform());
	ultimateCollisionLeftObject->GetTransform()->SetParent(ultimateLeftObject->GetTransform());
	ultimateCollisionRightObject->GetTransform()->SetParent(ultimateRightObject->GetTransform());

	ultimateWarningTopObject->SetActive(false);
	ultimateWarningLeftObject->SetActive(false);

	ultimateBottomObject->SetActive(false);
	ultimateTopObject->SetActive(false);
	ultimateLeftObject->SetActive(false);
	ultimateRightObject->SetActive(false);

	ultimateCollisionBottomObject->SetActive(false);
	ultimateCollisionTopObject->SetActive(false);
	ultimateCollisionLeftObject->SetActive(false);
	ultimateCollisionRightObject->SetActive(false);

	colliderobjectsPairVector.push_back({ branch12ColliderObject, branchDamage });
	colliderobjectsPairVector.push_back({ branch34ColliderObject, branchDamage });
	colliderobjectsPairVector.push_back({ pattern2ColliderObject, chargeDamage });
	colliderobjectsPairVector.push_back({ horizontalObject, normalAttackDamage });
	colliderobjectsPairVector.push_back({ verticalObject, normalAttackDamage });
	colliderobjectsPairVector.push_back({ pattern5ColliderObject1, rangedAttackDamage });
	colliderobjectsPairVector.push_back({ projectileObject, rangedAttackDamage });


	for (auto itr = colliderobjectsPairVector.begin(); itr != colliderobjectsPairVector.end(); itr++)
	{
		m_collisionManager[collisionIndex] = itr->first->AddComponent<BossCollisionManager>();

		m_collisionManager[collisionIndex]->SetPlayerComponent(m_playerComponent);
		m_collisionManager[collisionIndex]->SetBossComponent(this);
		m_collisionManager[collisionIndex]->SetPatternDamage(itr->second);
		itr->first->SetActive(false);
		collisionIndex++;
	}

	m_collisionManager[7] = fanObject->AddComponent<BossCollisionManager>();
	m_collisionManager[7]->SetPlayerComponent(m_playerComponent);
	m_collisionManager[7]->SetBossComponent(this);
	m_collisionManager[7]->SetPatternDamage(fanDamage);

	m_collisionManager[8] = barrierTriggerObject->AddComponent<BossCollisionManager>();
	m_collisionManager[8]->SetPlayerComponent(m_playerComponent);
	m_collisionManager[8]->SetBossComponent(this);
	m_collisionManager[8]->SetPatternDamage(0);


	branch1Dotween = branch12ColliderObject->AddComponent<Dotween>();
	branch2Dotween = branch34ColliderObject->AddComponent<Dotween>();
	branchAxisDotween = boss1TransformObject->AddComponent<Dotween>();
	treeParentDotween = treeParent->AddComponent<Dotween>();
	pattern2ColliderDotween = pattern2ColliderObject->AddComponent<Dotween>();
	horizontalDotween = horizontalObject->AddComponent<Dotween>();
	verticalDotween = verticalObject->AddComponent<Dotween>();
	pattern5ObjectDotween = projectileObject->AddComponent<Dotween>();
	protectEffectDotween = protectEffectObject->AddComponent<Dotween>();
	protectEffectDotween2 = protectEffectObject2->AddComponent<Dotween>();

	childProjectileObject->GetTransform()->SetParent(projectileObject->GetTransform());

	for (int i = 0; i < 3; i++)
	{
		obstacleDotween[i] = disruptingObjects[i]->AddComponent<Dotween>();

		disruptingColliderObjects[i]->GetTransform()->SetLocalScale(Vector3D(3.5, 7.5, 3.5));

		obstacleCollider[i] = disruptingColliderObjects[i]->AddComponent<CylinderCollider>();
		obstacleCollider[i]->SetTrigger(false);
		disruptingColliderObjects[i]->SetLayerName(L"obstacleCollider");

		obstacleRigidBody[i] = disruptingColliderObjects[i]->AddComponent<Rigidbody>();
		obstacleRigidBody[i]->SetKinematic(false);
		obstacleRigidBody[i]->SetUseGravity(false);

		obstacleRigidBody[i]->SetFreezePositionX(true);
		obstacleRigidBody[i]->SetFreezePositionY(true);
		obstacleRigidBody[i]->SetFreezePositionZ(true);
		obstacleRigidBody[i]->SetFreezeRotationX(true);
		obstacleRigidBody[i]->SetFreezeRotationY(true);
		obstacleRigidBody[i]->SetFreezeRotationZ(true);

		disruptingObjects[i]->SetActive(false);
		disruptingColliderObjects[i]->SetActive(false);
	}

	barrierCollisionObject->GetTransform()->SetLocalScale(Vector3D(0, 3, 0));
	barrierCollisionObject->GetTransform()->SetLocalPosition(Vector3D(0, 0.6, 0));			// 2, 1, 2?
	barrierCollisionObject->SetLayerName(L"BarrierCollider");

	barrierTriggerObject->GetTransform()->SetLocalScale(Vector3D(18.0f, 6.0f, 18.0f));
	barrierTriggerObject->GetTransform()->SetLocalPosition(Vector3D(0, 0.6, 0));			// 2, 1, 2?
	barrierTriggerObject->SetLayerName(L"BarrierCollider");

	barrierCollider = barrierCollisionObject->AddComponent<CylinderCollider>();
	barrierCollider->SetTrigger(false);
	barrierRigidbody = barrierCollisionObject->AddComponent<Rigidbody>();
	barrierRigidbody->SetKinematic(false);
	barrierRigidbody->SetUseGravity(false);
	barrierRigidbody->SetFreezePositionX(true);
	barrierRigidbody->SetFreezePositionY(true);
	barrierRigidbody->SetFreezePositionZ(true);
	barrierRigidbody->SetFreezeRotationX(true);
	barrierRigidbody->SetFreezeRotationY(true);
	barrierRigidbody->SetFreezeRotationZ(true);
	barrierCollisionObject->SetActive(false);

	barrierColliderDotween = barrierCollisionObject->AddComponent<Dotween>();

	barrierTrigger = barrierTriggerObject->AddComponent<CylinderCollider>();
	barrierTrigger->SetTrigger(true);
	barrierTriggerObject->SetActive(false);

	// EffectPlayer

	for (int i = 0; i < 4; i++)
	{
		branchEffectPlayers[i] = branchEffectObjects[i]->AddComponent<EffectPlayer>();
		// 일단 기본 세팅 속도로 해보장
		branchEffectPlayers[i]->SetMeshRenderer(branchEffectObjects[i]->GetTransform()->GetChild(0)->GetGameObject()->GetComponent<MeshRenderer>());
		branchEffectPlayers[i]->SetPlaySpeed(branchPatternEffectSpeed);
		branchEffectPlayers[i]->SetMaxValue(0.0f);
		branchEffectPlayers[i]->SetRemainTime(branchPatternEffectRemainedTime);				// 한바퀴 도는 시간으로 설정하기
		branchEffectPlayers[i]->SetDirection(Vector2D(1, 0));
		branchEffectObjects[i]->GetTransform()->SetParent(treeParent->GetTransform());
		branchEffectObjects[i]->GetTransform()->SetLocalRotation(Vector3D(0, 90 * i, 0));
		branchEffectObjects[i]->SetActive(false);
	}

	protectEffectPlayer = protectEffectObject->AddComponent<EffectPlayer>();
	protectEffectPlayer->SetMeshRenderer(protectEffectObject->GetTransform()->GetChild(0)->GetGameObject()->GetComponent<MeshRenderer>());
	protectEffectPlayer->SetMaxValue(0.0f);
	protectEffectPlayer->SetPlaySpeed(protectEffectSpeed);
	protectEffectPlayer->SetRemainTime(protectEffectRemainedTime);
	protectEffectPlayer->SetDirection(Vector2D(1, 0));
	protectEffectObject->GetTransform()->SetParent(m_gameObject->GetTransform());
	protectEffectObject->GetTransform()->SetLocalPosition(Vector3D(0, -4, 0));
	protectEffectObject->GetTransform()->SetLocalScale(Vector3D::zero);
	protectEffectObject->SetActive(false);

	protectEffectPlayer2 = protectEffectObject2->AddComponent<EffectPlayer>();
	protectEffectPlayer2->SetMeshRenderer(protectEffectObject2->GetTransform()->GetChild(0)->GetGameObject()->GetComponent<MeshRenderer>());
	protectEffectPlayer2->SetMaxValue(0.0f);
	protectEffectPlayer2->SetPlaySpeed(protectEffectSpeed);
	protectEffectPlayer2->SetRemainTime(protectEffectRemainedTime);
	protectEffectPlayer2->SetDirection(Vector2D(1, 0));
	protectEffectObject2->GetTransform()->SetParent(m_gameObject->GetTransform());
	protectEffectObject2->GetTransform()->SetLocalPosition(Vector3D(0, -4, 0));
	protectEffectObject2->GetTransform()->SetLocalScale(Vector3D::zero);
	protectEffectObject2->SetActive(false);


	circleEffectPlayer = circleEffectObject->AddComponent<EffectPlayer>();
	circleEffectPlayer->SetMeshRenderer(circleEffectObject->GetTransform()->GetChild(0)->GetGameObject()->GetComponent<MeshRenderer>());
	circleEffectPlayer->SetMaxValue(0.0f);
	circleEffectPlayer->SetPlaySpeed(circlePatternEffectSpeed);
	circleEffectPlayer->SetRemainTime(circleEffectRemainedTime);
	circleEffectPlayer->SetDirection(Vector2D(1, 0));
	circleEffectObject->SetActive(false);

	teleportDisappearEffectPlayer = teleportDisappearEffectObject->AddComponent<EffectPlayer>();
	teleportDisappearEffectPlayer->SetMeshRenderer(teleportDisappearEffectObject->GetTransform()->GetChild(0)->GetGameObject()->GetComponent<MeshRenderer>());
	teleportDisappearEffectPlayer->SetMaxValue(0.0f);
	teleportDisappearEffectPlayer->SetPlaySpeed(teleportEffectSpeed);
	teleportDisappearEffectPlayer->SetRemainTime(teleportEffectRemainedTime);
	teleportDisappearEffectPlayer->SetDirection(Vector2D(1, 0));
	teleportDisappearEffectObject->SetActive(false);

	teleportAppearEffectPlayer = teleportAppearEffectObject->AddComponent<EffectPlayer>();
	teleportAppearEffectPlayer->SetMeshRenderer(teleportAppearEffectObject->GetTransform()->GetChild(0)->GetGameObject()->GetComponent<MeshRenderer>());
	teleportAppearEffectPlayer->SetMaxValue(0.0f);
	teleportAppearEffectPlayer->SetPlaySpeed(teleportEffectSpeed);
	teleportAppearEffectPlayer->SetRemainTime(teleportEffectRemainedTime);
	teleportAppearEffectPlayer->SetDirection(Vector2D(1, 0));
	teleportAppearEffectObject->SetActive(false);

	for (int i = 0; i < 3; i++)
	{
		disruptingAppearEffectPlayer[i] = disruptingAppearEffectObject[i]->AddComponent<EffectPlayer>();
		disruptingAppearEffectPlayer[i]->SetMeshRenderer(disruptingAppearEffectObject[i]->GetTransform()->GetChild(0)->GetGameObject()->GetComponent<MeshRenderer>());
		disruptingAppearEffectPlayer[i]->SetMaxValue(0.0f);
		disruptingAppearEffectPlayer[i]->SetPlaySpeed(teleportEffectSpeed);
		disruptingAppearEffectPlayer[i]->SetRemainTime(teleportEffectRemainedTime);
		disruptingAppearEffectPlayer[i]->SetDirection(Vector2D(1, 0));
		disruptingAppearEffectObject[i]->SetActive(false);
	}


	horizontalAttackEffectPlayer = horizontalAttackEffectObject->AddComponent<EffectPlayer>();
	horizontalAttackEffectPlayer->SetMeshRenderer(horizontalAttackEffectObject->GetTransform()->GetChild(0)->GetGameObject()->GetComponent<MeshRenderer>());
	horizontalAttackEffectPlayer->SetMaxValue(0.0f);
	horizontalAttackEffectPlayer->SetPlaySpeed(horizontalAttackEffectSpeed);
	horizontalAttackEffectPlayer->SetRemainTime(horizontalAttackEffectRemainedTime);
	horizontalAttackEffectPlayer->SetDirection(Vector2D(1, 0));
	horizontalAttackEffectObject->SetActive(false);

	verticalAttackEffectPlayer = verticalAttackEffectObject->AddComponent<EffectPlayer>();
	verticalAttackEffectPlayer->SetMeshRenderer(verticalAttackEffectObject->GetTransform()->GetChild(0)->GetGameObject()->GetComponent<MeshRenderer>());
	verticalAttackEffectPlayer->SetMaxValue(0.0f);
	verticalAttackEffectPlayer->SetPlaySpeed(verticalAttackEffectSpeed);
	verticalAttackEffectPlayer->SetRemainTime(verticalAttackEffectRemainedTime);
	verticalAttackEffectPlayer->SetDirection(Vector2D(1, 0));
	verticalAttackEffectObject->SetActive(false);

	rangedAttackEffectPlayer = rangedAttackEffectObject->AddComponent<EffectPlayer>();
	rangedAttackEffectPlayer->SetMeshRenderer(rangedAttackEffectObject->GetTransform()->GetChild(0)->GetGameObject()->GetComponent<MeshRenderer>());
	rangedAttackEffectPlayer->SetMaxValue(0.0f);
	rangedAttackEffectPlayer->SetPlaySpeed(rangedAttackEffectSpeed);
	rangedAttackEffectPlayer->SetRemainTime(rangedAttackEffectRemainedTime);
	rangedAttackEffectPlayer->SetDirection(Vector2D(1, 0));
	rangedAttackEffectObject->GetTransform()->SetParent(m_gameObject->GetTransform());
	rangedAttackEffectObject->GetTransform()->SetLocalPosition(Vector3D(0, -4, 0));
	rangedAttackEffectObject->SetActive(false);

	explosionEffectPlayer = rangedExposionEffectObject->AddComponent<EffectPlayer>();
	explosionEffectPlayer->SetMeshRenderer(rangedExposionEffectObject->GetTransform()->GetChild(0)->GetGameObject()->GetComponent<MeshRenderer>());
	explosionEffectPlayer->SetMaxValue(0.0f);
	explosionEffectPlayer->SetPlaySpeed(explosionEffectSpeed);
	explosionEffectPlayer->SetRemainTime(explosionEffectRemainedTime);
	explosionEffectPlayer->SetDirection(Vector2D(1, 0));
	rangedExposionEffectObject->GetTransform()->SetParent(pattern5ColliderObject1->GetTransform());
	rangedExposionEffectObject->SetActive(false);


	knockBackAttackEffectPlayer = knockBackAttackEffectObject->AddComponent<EffectPlayer>();
	knockBackAttackEffectPlayer->SetMeshRenderer(knockBackAttackEffectObject->GetTransform()->GetChild(0)->GetGameObject()->GetComponent<MeshRenderer>());
	knockBackAttackEffectPlayer->SetMaxValue(0.0f);
	knockBackAttackEffectPlayer->SetPlaySpeed(knockBackAttackEffectSpeed);
	knockBackAttackEffectPlayer->SetRemainTime(knockBackAttackEffectRemainedTime);
	knockBackAttackEffectPlayer->SetDirection(Vector2D(1, 0));
	knockBackAttackEffectObject->SetActive(false);

	knockBackGuideEffectPlayer = knockBackGuideLineObject->AddComponent<EffectPlayer>();
	knockBackGuideEffectPlayer->SetMeshRenderer(knockBackGuideLineObject->GetTransform()->GetChild(0)->GetGameObject()->GetComponent<MeshRenderer>());
	knockBackGuideEffectPlayer->SetMaxValue(1.0f);
	knockBackGuideEffectPlayer->SetPlaySpeed(knockBackGuideEffectSpeed);
	knockBackGuideEffectPlayer->SetRemainTime(knockBackGuideEffectRemainedTime);
	knockBackGuideEffectPlayer->SetDirection(Vector2D(1, 0));
	knockBackGuideLineObject->SetActive(false);

	skill2DEffectPlayer = skill2DEffectObject->AddComponent<EffectPlayer>();
	skill2DEffectPlayer->SetMeshRenderer(skill2DEffectObject->GetTransform()->GetChild(0)->GetGameObject()->GetComponent<MeshRenderer>());
	skill2DEffectPlayer->SetMaxValue(0.0f);
	skill2DEffectPlayer->SetPlaySpeed(skill2DEffectSpeed);
	skill2DEffectPlayer->SetRemainTime(skill2DEffectRemainedTime);
	skill2DEffectPlayer->SetDirection(Vector2D(1, 0));
	skill2DEffectObject->SetActive(false);

	chargeAttackEffectPlayer = chargeAttackEffectObject->AddComponent<EffectPlayer>();
	chargeAttackEffectPlayer->SetMeshRenderer(chargeAttackEffectObject->GetTransform()->GetChild(0)->GetGameObject()->GetComponent<MeshRenderer>());
	chargeAttackEffectPlayer->SetMaxValue(0.0f);
	chargeAttackEffectPlayer->SetPlaySpeed(chargeAttackEffectSpeed);
	chargeAttackEffectPlayer->SetRemainTime(chargeAttackRemainedTime);
	chargeAttackEffectPlayer->SetDirection(Vector2D(1, 0));
	chargeAttackEffectObject->GetTransform()->SetParent(m_gameObject->GetTransform());
	chargeAttackEffectObject->GetTransform()->SetLocalPosition(Vector3D(0, -4, 0));
	chargeAttackEffectObject->SetActive(false);

	chargeSonicEffectPlayer = chargeSonicEffectObject->AddComponent<EffectPlayer>();
	chargeSonicEffectPlayer->SetMeshRenderer(chargeSonicEffectObject->GetTransform()->GetChild(0)->GetGameObject()->GetComponent<MeshRenderer>());
	chargeSonicEffectPlayer->SetMaxValue(0.0f);
	chargeSonicEffectPlayer->SetPlaySpeed(chargeSonicEffectSpeed);
	chargeSonicEffectPlayer->SetRemainTime(chargeSonicRemainedTime);
	chargeSonicEffectPlayer->SetDirection(Vector2D(1, 0));
	chargeSonicEffectObject->GetTransform()->SetParent(m_gameObject->GetTransform());
	chargeSonicEffectObject->GetTransform()->SetLocalPosition(Vector3D(0, -4, 0));
	chargeSonicEffectObject->SetActive(false);

	chargeGuideEffectPlayer = chargeGuideEffectObject->AddComponent<EffectPlayer>();
	chargeGuideEffectPlayer->SetMeshRenderer(chargeGuideEffectObject->GetTransform()->GetChild(0)->GetGameObject()->GetComponent<MeshRenderer>());
	chargeGuideEffectPlayer->SetMaxValue(0.0f);
	chargeGuideEffectPlayer->SetPlaySpeed(chargeGuideEffectSpeed);
	chargeGuideEffectPlayer->SetRemainTime(chargeGuideRemainedTime);
	chargeGuideEffectPlayer->SetDirection(Vector2D(1, 0));
	chargeGuideEffectObject->GetTransform()->SetParent(m_gameObject->GetTransform());
	chargeGuideEffectObject->GetTransform()->SetLocalPosition(Vector3D(0, -4, 0));
	chargeGuideEffectObject->SetActive(false);


	normalGuideEffectPlayer = normalAttackGuideObject->AddComponent<EffectPlayer>();
	normalGuideEffectPlayer->SetMeshRenderer(normalAttackGuideObject->GetTransform()->GetChild(0)->GetGameObject()->GetComponent<MeshRenderer>());
	normalGuideEffectPlayer->SetMaxValue(0.0f);
	normalGuideEffectPlayer->SetPlaySpeed(normalGuideEffectSpeed);
	normalGuideEffectPlayer->SetRemainTime(normalGuideRemainedTime);
	normalGuideEffectPlayer->SetDirection(Vector2D(1, 0));
	normalAttackGuideObject->GetTransform()->SetParent(m_gameObject->GetTransform());
	normalAttackGuideObject->GetTransform()->SetLocalPosition(Vector3D(0, -4, 0));
	normalAttackGuideObject->SetActive(false);


	obstaclePatternEffectPlayer = obstaclePatternEffectObject->AddComponent<EffectPlayer>();
	obstaclePatternEffectPlayer->SetMeshRenderer(obstaclePatternEffectObject->GetTransform()->GetChild(0)->GetGameObject()->GetComponent<MeshRenderer>());
	obstaclePatternEffectPlayer->SetMaxValue(0.0f);
	obstaclePatternEffectPlayer->SetPlaySpeed(blockingEffectSpeed);
	obstaclePatternEffectPlayer->SetRemainTime(blockingEffectRemainedTime);
	obstaclePatternEffectPlayer->SetDirection(Vector2D(1, 0));
	obstaclePatternEffectObject->GetTransform()->SetParent(m_gameObject->GetTransform());
	obstaclePatternEffectObject->GetTransform()->SetLocalPosition(Vector3D(0, -4, 0));
	obstaclePatternEffectObject->SetActive(false);


	boss1TransformObject->GetTransform()->SetParent(GetGameObject()->GetTransform());
	boss1TransformObject->GetTransform()->SetLocalPosition(Vector3D(0, 0, 0));

	// 보스에서도 원할 때 카메라를 써주고 싶다
	cameraDotween = SceneManager::GetInstance().GetMainCamera()->GetGameObject()->AddComponent<Dotween>();

	// 애니메이션 컴포넌트 가져오기
	m_animator = GetGameObject()->GetComponent<Animator>();

	m_bossFlagComponentIter = bossFlagComponentVector.begin();

	dotweenVector.push_back(branch1Dotween);
	dotweenVector.push_back(branch2Dotween);
	dotweenVector.push_back(branchAxisDotween);
	dotweenVector.push_back(treeParentDotween);
	dotweenVector.push_back(pattern2ColliderDotween);
	dotweenVector.push_back(horizontalDotween);
	dotweenVector.push_back(verticalDotween);
	dotweenVector.push_back(pattern5ObjectDotween);
	dotweenVector.push_back(ultimateObjectDotween);
	dotweenVector.push_back(ultimateColliderDotween);
	dotweenVector.push_back(timeDotween);
	dotweenVector.push_back(boss1Dotween);
	dotweenVector.push_back(cameraDotween);

	for (int i = 0; i < 3; i++)
	{
		dotweenVector.push_back(obstacleDotween[i]);
	}

	// 족자 iterator 초기화. 씬 로드 안해줄 경우의 초기화.
	m_floatingObjectIter = floatingObjectComponentVector.begin();

	if (SceneManager::GetInstance().GetName() == L"BossPhase2Scene")
	{
		gameManager->ShowPhase2CutScene1();
		m_gameObject->GetTransform()->SetLocalPosition(Vector3D(0, 4, 0));
		phaseNumber = 2;
		isPhaseJustChanged = true;
		phase1Speed = phase1RageSpeed;
		finalRushCount = rageRushCount;
		phase1RageOn = false;
		phase2RageOn = false;


		for (auto& e : floatingObjectComponentVector)
		{
			e->RevertObject();
		}

		for (auto& e : bossFlagComponentVector)
		{
			e->Revert();
		}

		// 족자 iterator 초기화. 씬 로드 해줄 경우의 초기화.
		m_floatingObjectIter = floatingObjectComponentVector.begin();

		previousStateNum = static_cast<int>(Boss1State::DisruptingMovement);

		firstDelay = 0;
	}

}

void Boss1::Update()
{
	if (Input::GetInstance().PressedKeyNow(VK_F9) && EngineSetting::GetInstance().GetShowDebugInfo()) OnDamaged(1.0f, static_cast<int>(PlayerAttackType::DubugAttack));
	if (Input::GetInstance().PressedKeyNow(VK_F10) && EngineSetting::GetInstance().GetShowDebugInfo()) OnDamaged(5.0f, static_cast<int>(PlayerAttackType::DubugAttack));
	if (Input::GetInstance().PressedKeyNow(VK_F11) && EngineSetting::GetInstance().GetShowDebugInfo()) OnDamaged(10.0f, static_cast<int>(PlayerAttackType::DubugAttack));

	boss1FSM->UpdateState();

	if (m_gameObject->GetTransform()->GetLocalPosition().x > 25)
		m_gameObject->GetTransform()->SetLocalPosition(Vector3D(25, m_gameObject->GetTransform()->GetLocalPosition().y, m_gameObject->GetTransform()->GetLocalPosition().z));

	if (m_gameObject->GetTransform()->GetLocalPosition().z > 25)
		m_gameObject->GetTransform()->SetLocalPosition(Vector3D(m_gameObject->GetTransform()->GetLocalPosition().x, m_gameObject->GetTransform()->GetLocalPosition().y, 25));

	if (m_gameObject->GetTransform()->GetLocalPosition().x < -25)
		m_gameObject->GetTransform()->SetLocalPosition(Vector3D(-25, m_gameObject->GetTransform()->GetLocalPosition().y, m_gameObject->GetTransform()->GetLocalPosition().z));

	if (m_gameObject->GetTransform()->GetLocalPosition().z < -25)
		m_gameObject->GetTransform()->SetLocalPosition(Vector3D(m_gameObject->GetTransform()->GetLocalPosition().x, m_gameObject->GetTransform()->GetLocalPosition().y, -25));
}

//--s------------------------------------------------State Engage Function--------------------------------------------------
void Boss1::IdleEngage()
{
	patternNum = int(Boss1State::StateEnd);

	isPattern1End = false;
	isPattern2End = false;
	isPattern3End = false;
	isPattern5End = false;
	isHitPattern5 = false;
	isPattern6End = false;
	isMoveEnd = false;
	isGroggyEnd = false;
	groggyPatternOn = false;
	isPattern2DNormalEnd = false;
	isDistruptingMovementEnd = false;
	isPattern2DUltimateEnd = false;
	jumpToNextPhase = false;
	jumpToPhase2 = false;


	if (phase1RageOn)
	{
		phase1Speed = phase1RageSpeed;
		finalRushCount = rageRushCount;
		ApplyDamage(1, true);
	}

	if (phase2RageOn)
	{
		phase2Speed = phase2RageSpeed;
		pattern2DFinalLineNumber = pattern2DRageLineNumber;
		pattern2DFinalCount = pattern2DRageCount;
		ApplyDamage(2, true);
	}

	if (!phase2RageOn && phaseNumber == 2)							// Revert 됐을 때는 다시 초기값으로 돌려줘야 한다
	{
		phase2Speed = phase2DefaultSpeed;
		pattern2DFinalLineNumber = pattern2DNormalLineNumber;
		pattern2DFinalCount = pattern2DNormalCount;
		ApplyDamage(2, false);
	}



	if (previousStateNum == int(Boss1State::DisruptingMovement))
	{
		int a = (maxHp - currenthp) % hpPerScroll;

		for (int i = 0; i < a; i++)
		{
			if (m_floatingObjectIter != floatingObjectComponentVector.end())
			{
				(*m_floatingObjectIter)->DownObject();
				m_floatingObjectIter++;
			}
		}

		boss1Dotween->DONothing(0.1f).OnComplete([=]()
			{
				ChoosePattern();
			});
	}
	else if (phaseNumber == 2 && previousStateNum == int(Boss1State::Death))
	{
		boss1Dotween->DONothing(0.1f).OnComplete([=]()
			{
				ChoosePattern();
			});
	}
	else
	{
		bossFrontObject->GetTransform()->SetLocalPosition(m_gameObject->GetTransform()->GetLocalPosition() + Vector3D(0, 0, 1));
		if (!rage1AnimationPlayed && phase1RageOn)
		{
			boss1Dotween->DONothing(0.7f + bossRageLookDuration).OnComplete([=]()
				{
					boss1Dotween->DONothing(0.2f).OnComplete([=]()
						{
							SoundManager::GetInstance().PlaySFX(SoundManager::Boss::Roar);
							rageTextureObject->SetActive(true);
						});
					cameraDotween->DOShakePosition(1.0f, 1.0f).SetDelay(0.2f).OnComplete([=]()
						{
							rageTextureObject->SetActive(false);

							boss1Dotween->DOLookAt(m_playerTransform, 0.4f, false).SetDelay(1.0f).OnComplete([this]()
								{
									ChoosePattern();
									//patternNum = int(Boss1State::Pattern6);
								});
						});
				});
			boss1Dotween->DOLookAt(bossFrontObject->GetTransform(), bossRageLookDuration, false).OnComplete([=]()
				{
					m_animator->Play(L"root|PhaseTransition");
					bossParticleObject->SetActive(true);
				});
			rage1AnimationPlayed = true;
		}
		else if (!rage2AnimationPlayed && phase2RageOn)
		{
			boss1Dotween->DONothing(0.7f + bossRageLookDuration).OnComplete([=]()
				{
					boss1Dotween->DONothing(0.2f).OnComplete([=]()
						{
							SoundManager::GetInstance().PlaySFX(SoundManager::Boss::Roar);
							rageTextureObject->SetActive(true);
						});
					cameraDotween->DOShakePosition(1.0f, 1.0f).SetDelay(0.2f).OnComplete([=]()
						{
							rageTextureObject->SetActive(false);

							boss1Dotween->DOLookAt(m_playerTransform, 0.4f, false).SetDelay(1.0f).OnComplete([this]()
								{
									ChoosePattern();
									//patternNum = int(Boss1State::Pattern6);
								});
						});
				});
			boss1Dotween->DOLookAt(bossFrontObject->GetTransform(), bossRageLookDuration, false).OnComplete([=]()
				{
					m_animator->Play(L"root|PhaseTransition");
					bossParticleObject->SetActive(true);
				});
			rage2AnimationPlayed = true;
		}
		else
		{
			m_animator->Play(L"root|000.Idle");
			boss1Dotween->DOLookAt(m_playerTransform, 0.4f, false).OnComplete([this]()
				{
					boss1Dotween->DONothing(firstDelay).OnComplete([=]()
						{
							ChoosePattern();
							//patternNum = int(Boss1State::Pattern6);
							firstDelay = 0;
						});
				});
		}

	}


}

void Boss1::DeathEngage()
{
	// 보스가 죽을 때...
	// 패턴 종료 후 콜라이더 잔류하는 패턴의 오브젝트들 꺼주기
	horizontalObject->SetActive(false);
	verticalObject->SetActive(false);
	projectileObject->SetActive(false);
	pattern5ColliderObject1->SetActive(false);
	bossParticleObject->SetActive(false);

	previousStateNum = int(Boss1State::Death);

	// 페이즈 전환
	if (phaseNumber == 1)
	{
		Guilty::GetInstance().bossPhase = 2;
		gameManager->ShowPhase2CutScene1();
		m_gameObject->GetTransform()->SetLocalPosition(Vector3D(0, 4, 0));
		phaseNumber = 2;
		isPhaseJustChanged = true;
		jumpToNextPhase = true;
	}

	else if (phaseNumber == 2 && !isPhaseJustChanged)
	{
		for (int i = 0; i < 3; i++)
		{
			disruptingObjects[i]->SetActive(false);
			disruptingColliderObjects[i]->SetActive(false);
		}
		gameManager->ShowPhase2EndEvent();
		phaseNumber = 3;
	}

	// hp 초기화
	currenthp = maxHp;
	m_floatingObjectIter = floatingObjectComponentVector.begin();
	isPhaseJustChanged = false;
}

void Boss1::Pattern1Engage()
{
	previousStateNum = int(Boss1State::Pattern1);
	Pattern1Function(Vector3D(50, 10, 8), 180.0f, 1.0f);
	//isPattern1End = truze;
}

void Boss1::Pattern2Engage()
{
	if (!isStarted)
	{
		previousStateNum = int(Boss1State::Pattern2);
		Pattern2Function(finalRushCount, 2.0f, 45.0f * phase1Speed, EaseBasic);
		//isPattern2End = true;
	}

	else if (isStarted)
	{
		isPattern2End = true;
	}
	isStarted = false;
}

void Boss1::Pattern3Engage()
{
	isPlayerNear = false;

	previousStateNum = int(Boss1State::Pattern3);
	Pattern3Function(phase1Speed, defaultMoveSpeed * phase1Speed, 2);
	//isPattern3End = true;
}


void Boss1::Pattern5Engage()
{
	Vector3D distanceVector = m_playerTransform->GetLocalPosition() - m_gameObject->GetTransform()->GetLocalPosition();
	double distance = distanceVector.Size();
	if (distance < 10)
		isPlayerNear = true;			// 근거리 공격으로 전환

	else
	{
		previousStateNum = int(Boss1State::Pattern5);
		Pattern5Function(phase1Speed);
		//isPattern5End = true;
	}
}

void Boss1::Pattern6Engage()
{
	previousStateNum = int(Boss1State::Pattern6);
	Pattern6Function(phase1Speed, defaultMoveSpeed * phase1Speed);
	//isPattern6End = true;
}

void Boss1::PatternMoveEngage()
{
	previousStateNum = int(Boss1State::PatternMove);
	int minusOrPlusX = rand() % 2;			// 음수 : 0, 양수 : 1
	int minusOrPlusZ = rand() % 2;			// 음수 : 0, 양수 : 1
	int randXposition = rand() % 18;
	int randZposition = rand() % 18;

	if (minusOrPlusX == 0)
	{
		randXposition = -randXposition;
	}

	if (minusOrPlusZ == 0)
	{
		randZposition = -randZposition;
	}

	PatternMoveFunction(Vector3D(randXposition, 4.0, randZposition), true, 3.0f, defaultMoveSpeed * phase1Speed);
}

void Boss1::GroggyEngage()
{
	m_animator->Play(L"root|011-1.Groggy");

	boss1Dotween->DOCustom(groggyTime).OnUpdate([this]()
		{
			// groggyTime동안 hp가 0인지 확인해주기 
			if (currenthp <= 0)
			{
				previousStateNum = int(Boss1State::Groggy);
				patternNum = int(Boss1State::Death);
			}
		}).OnComplete([=]()
			{
				if (currenthp > 8)				// Death 상태 진입 시 hp가 초기화 되는 걸 활용해보자
				{
					m_animator->Play(L"root|011-2.Groogy->Idle");
				}
			});


	boss1Dotween->DONothing(groggyTime + 2.0f).OnComplete([this]()
		{
				if (currenthp <= 8 && currenthp > 0)
				{
					isGroggyEnd = true;
				}
				else if (currenthp <= 0)
				{
					patternNum = int(Boss1State::Death);
				}
		});
}

void Boss1::Pattern2DNormalEngage()
{
	previousStateNum = int(Boss1State::Pattern2DNormal);
	Pattern2DNormalFunction(pattern2DFinalCount, pattern2DFinalLineNumber);
	//OutputDebugString(L"Pattern2DNormalEngage");
}

void Boss1::DisruptingMovementEngage()
{
	previousStateNum = int(Boss1State::DisruptingMovement);
	DisruptingMovementFunction();
}

void Boss1::Pattern2DUltimateEngage()
{
	previousStateNum = int(Boss1State::Pattern2DUltimate);
	Pattern2DUltimateFunction();
}


void Boss1::Pattern1Function(Vector3D branchScale, double endRotation, double rotateSpeed, easing_functions easeFunc)
{
	m_bossHitCollider->GetGameObject()->SetActive(false);
	m_animator->Play(L"root|005.Teleport");
	treeParent->GetTransform()->SetLocalPosition(Vector3D(0, 0, 0));

	for (int i = 0; i < 4; i++)
	{
		branchEffectObjects[i]->SetActive(false);
	}

	if (currenthp <= maxHp - hpPerScroll * 2 && !isBranchPattern1Used)
		isBranchPattern1Used = true;

	//m_gameObject->GetTransform()->SetLocalRotation(Vector3D::zero);
	m_gameObject->GetTransform()->SetLocalRotation(Vector3D(0, 0, 0));

	// 원래는 DOShakePosition 함수 내부에서 해야할일지만 임시로...
	Vector3D camStartPosition = SceneManager::GetInstance().GetMainCamera()->GetGameObject()->GetTransform()->GetLocalPosition();

	branch12ColliderObject->GetTransform()->SetParent(boss1TransformObject->GetTransform());
	branch34ColliderObject->GetTransform()->SetParent(boss1TransformObject->GetTransform());

	boss1TransformObject->SetActive(true);

	// 콜라이더 크기를 조금 늘려 놓는다
	branch12ColliderObject->GetTransform()->SetLocalScale(Vector3D::zero);
	branch34ColliderObject->GetTransform()->SetLocalScale(Vector3D::zero);

	branch12ColliderObject->SetActive(true);
	branch34ColliderObject->SetActive(true);

	boss1Dotween->DONothing(1.0f).OnComplete([=]()
		{
			PatternMoveFunction(Vector3D(0, 4.0f, 0), false);
		});

	boss1Dotween->DONothing(7.5f).OnComplete([=]()
		{
			circleEffectObject->SetActive(true);
			circleEffectPlayer->SetMaxValue(0.5f);

			branchEffectObjects[0]->SetActive(true);
			branchEffectObjects[2]->SetActive(true);
			branchEffectPlayers[0]->SetMaxValue(0.5f);
			branchEffectPlayers[2]->SetMaxValue(0.5f);
			protectEffectPlayer->SetMaxValue(1000.0f);

			m_animator->Play(L"root|006-1.Skill_Tree");
		});

	boss1Dotween->DONothing(8.0f).OnComplete([this]()
		{
			// collision Manager에게 이제 범위 판정을 하지 말라고 전달해야 한다
			for (int i = 0; i < 7; i++)
			{
				m_collisionManager[i]->IsBranchMoveStart(true);
			}
		});

	barrierRigidbody->SetKinematic(true);
	barrierCollisionObject->SetActive(true);

	protectEffectObject->SetActive(true);
	barrierColliderDotween->DOScale(Vector3D(18.0f, 6.0f, 18.0f), 2).SetDelay(8.0f).SetEase(EaseInSine).OnComplete([=]()
		{
			barrierTriggerObject->SetActive(true);
			barrierRigidbody->SetKinematic(false);
		});
	protectEffectDotween->DOScale(Vector3D(1, 1, 1), 2).SetDelay(8.0f).SetEase(EaseInSine);

	branch1Dotween->DOScale(branchScale, 2).SetDelay(8.0f).SetEase(EaseInSine).OnComplete([=]()
		{
			SoundManager::GetInstance().PlaySFX(SoundManager::Boss::Pattern1);
			cameraDotween->DOShakePosition(4.0f, 0.1f, 3.0f).SetDelay(1.0f);
			branchAxisDotween->DOQRotate(boss1TransformObject->GetTransform()->GetUp(), endRotation, 4 / rotateSpeed).SetDelay(1.0f).SetEase(easeFunc).OnComplete([=]()
				{
					SoundManager::GetInstance().StopAllSFX();
					circleEffectPlayer->SetMaxValue(0.0f);
					circleEffectPlayer->InverseDirection();
					branchEffectPlayers[0]->SetMaxValue(0.0f);
					branchEffectPlayers[2]->SetMaxValue(0.0f);
					branchEffectPlayers[0]->InverseDirection();
					branchEffectPlayers[2]->InverseDirection();

					branch1Dotween->DOScale(Vector3D(0, 0, 0), 1).SetEase(EaseInSine).OnComplete([=]()
						{
							m_animator->Play(L"root|006-2.Skill_Tree->Idle");

							boss1Dotween->DONothing(1.0f).OnComplete([=]()
								{
									circleEffectPlayer->InverseDirection();
									branchEffectPlayers[0]->InverseDirection();
									branchEffectPlayers[2]->InverseDirection();
									boss1TransformObject->SetActive(false);
									branch12ColliderObject->SetActive(false);
									m_bossHitCollider->GetGameObject()->SetActive(true);
									isPattern1End = true;
									for (int i = 0; i < 7; i++)
									{
										m_collisionManager[i]->IsBranchMoveStart(false);
									}
								});
						});
					protectEffectDotween->DOScale(Vector3D(0, 0.01, 0), 1).SetEase(EaseInSine).OnComplete([=]()
						{
							protectEffectObject->SetActive(false);
						});

					barrierColliderDotween->DOScale(Vector3D(0, 0.01, 0), 1).SetEase(EaseInSine).OnComplete([=]()
						{
							barrierCollisionObject->SetActive(false);
							barrierTriggerObject->SetActive(false);
						});
				});

			treeParentDotween->DORotate(Vector3D(0, endRotation, 0), 4 / rotateSpeed).SetDelay(1.0f).SetEase(easeFunc);
		});


	// 강화 나뭇가지 패턴
	if ((currenthp <= maxHp - hpPerScroll * 6 && !isBranchPattern3Used) || phaseNumber == 2)
	{
		boss1Dotween->DONothing(7.5f).OnComplete([=]()
			{
				branchEffectObjects[1]->SetActive(true);
				branchEffectObjects[3]->SetActive(true);
				branchEffectPlayers[1]->SetMaxValue(0.5f);
				branchEffectPlayers[3]->SetMaxValue(0.5f);
			});

		protectEffectObject2->SetActive(true);
		protectEffectPlayer2->SetMaxValue(1000.0f);

		protectEffectDotween2->DOScale(Vector3D(10, 1, 10), 2).SetDelay(8.0f).SetEase(EaseInSine);

		branch2Dotween->DOScale(Vector3D(branchScale.z, branchScale.y, branchScale.x), 2).SetDelay(8.0f).SetEase(EaseInSine).OnComplete([=]()
			{
				branchAxisDotween->DORotate(Vector3D(0, endRotation, 0), 4 / rotateSpeed).SetDelay(1.0f).SetEase(easeFunc).OnComplete([=]()
					{
						branchEffectPlayers[1]->SetMaxValue(0.0f);
						branchEffectPlayers[3]->SetMaxValue(0.0f);
						branchEffectPlayers[1]->InverseDirection();
						branchEffectPlayers[3]->InverseDirection();

						protectEffectDotween2->DOScale(Vector3D(0, 0, 0), 1).SetEase(EaseInSine);

						branch2Dotween->DOScale(Vector3D(0, 0, 0), 1).SetEase(EaseInSine).OnComplete([=]()
							{
								boss1Dotween->DONothing(1.0f).OnComplete([=]()
									{
										if (phaseNumber == 2)
										{
											isPhase2BranchPatternUsed = true;
										}
										branchEffectPlayers[1]->InverseDirection();
										branchEffectPlayers[3]->InverseDirection();
										branch34ColliderObject->SetActive(false);
										protectEffectObject2->SetActive(false);
										groggyPatternOn = true;
									});
							});
					});
			});
		isBranchPattern3Used = true;
	}
}



void Boss1::Pattern2Function(int rushCount, double delay, double speed, easing_functions easeFunc)
{
	/// 보스 돌진 패턴, rushCount로 돌진 횟수를 정할 수 있다.
	/// 추후 속도 조절이 가능하도록 해보자.
	/// 맵 끝까지 이동.
	// 최초로 부드럽게 한번 봐준다. 
	boss1Dotween->DOLookAt(m_playerTransform, 0.15f, false).OnComplete([=]()
		{
			// 내적을 이용한 방향 뽑아내기
			// 실시간으로 각도 적용


			pattern2ColliderObject->SetActive(false);

			Vector3D frontVec = m_gameObject->GetTransform()->GetFront();
			Vector3D objectRight = m_gameObject->GetTransform()->GetRight();
			Vector3D objectLeft = Vector3D(-1 * objectRight.x, objectRight.y, -1 * objectRight.z);

			Vector3D distanceVec = Vector3D(m_playerTransform->GetLocalPosition().x - m_gameObject->GetTransform()->GetLocalPosition().x, 0,
				m_playerTransform->GetLocalPosition().z - m_gameObject->GetTransform()->GetLocalPosition().z);

			// 회전 방향 판정
			// object의 Right 벡터와 Left 벡터를 vec이랑 내적해 사잇값을 구한 후, 그 각이 더 낮은 쪽으로 회전하기.
			double angleRight = (objectRight.x * distanceVec.x + objectRight.z * distanceVec.z);
			double sqRight = (sqrt(pow(objectRight.x, 2) + pow(objectRight.z, 2)) *
				sqrt(pow(distanceVec.x, 2) + pow(distanceVec.z, 2)));
			double finalRightAngle = acos(angleRight / sqRight);

			double angleLeft = (objectLeft.x * distanceVec.x + objectLeft.z * distanceVec.z);
			double sqLeft = (sqrt(pow(objectLeft.x, 2) + pow(objectLeft.z, 2)) *
				sqrt(pow(distanceVec.x, 2) + pow(distanceVec.z, 2)));
			double finalLeftAngle = acos(angleLeft / sqLeft);

			double angle = (frontVec.x * distanceVec.x + frontVec.z * distanceVec.z);
			double vectorLength = (sqrt(pow(frontVec.x, 2) + pow(frontVec.z, 2)) *
				sqrt(pow(distanceVec.x, 2) + pow(distanceVec.z, 2)));

			pattern2FinalRadian = acos(angle / vectorLength);

			// 왼쪽으로 돌아야 할 때
			if ((finalRightAngle) > (finalLeftAngle))
				pattern2FinalRadian *= -1;

			if (!isnan(pattern2FinalRadian))
			{
				m_gameObject->GetTransform()->Rotate(m_gameObject->GetTransform()->GetUp(), pattern2FinalRadian);
			}

			Vector2D bossPosition = Vector2D(m_gameObject->GetTransform()->GetLocalPosition().x, m_gameObject->GetTransform()->GetLocalPosition().z);
			Vector2D playerPosition = Vector2D(m_playerTransform->GetLocalPosition().x, m_playerTransform->GetLocalPosition().z);

			// 기울기와 Y절편
			double tilt;
			double tempX;
			double tempY;

			if (bossPosition.x > playerPosition.x)
			{
				tempX = bossPosition.x - playerPosition.x;
				tempY = bossPosition.y - playerPosition.y;
			}

			else
			{
				tempX = playerPosition.x - bossPosition.x;
				tempY = playerPosition.y - bossPosition.y;
			}

			tilt = tempY / tempX;

			double yCluase = bossPosition.y - tilt * bossPosition.x;

			// 원의 방정식 대입
			double xValue;

			if (bossPosition.x > playerPosition.x)
			{
				xValue = ((-2 * tilt * yCluase) - sqrt(pow(2 * tilt * yCluase, 2) - 4 * (pow(tilt, 2) + 1) * (pow(yCluase, 2) - pow(23, 2)))) / (2 * (pow(tilt, 2) + 1));
			}
			else
			{
				xValue = ((-2 * tilt * yCluase) + sqrt(pow(2 * tilt * yCluase, 2) - 4 * (pow(tilt, 2) + 1) * (pow(yCluase, 2) - pow(23, 2)))) / (2 * (pow(tilt, 2) + 1));
			}
			double yValue = tilt * xValue + yCluase;

			// 경로와 원이 만나는 지점
			Vector3D pathCircleCrossDot;

			if (!isnan(xValue))
				pathCircleCrossDot = Vector3D(xValue, 0, yValue);

			circleDistanceVec = Vector3D(pathCircleCrossDot.x - m_gameObject->GetTransform()->GetLocalPosition().x, 0,
				pathCircleCrossDot.z - m_gameObject->GetTransform()->GetLocalPosition().z);

			chargeGuideEffectObject->SetActive(true);
			chargeGuideEffectPlayer->SetMaxValue(1.0f);

			boss1Dotween->DONothing(0.4f).OnComplete([=]()
				{
					chargeAttackEffectObject->SetActive(true);
					chargeAttackEffectPlayer->SetMaxValue(1.0f);
				});

			boss1Dotween->DONothing(0.6f).OnComplete([=]()					// 방향 판정 및 표시선 끝난 후
				{
					chargeGuideEffectObject->SetActive(false);
					double bossMoveDistance = circleDistanceVec.Size();

					// Pattern2 콜라이더 Transform설정은 여기서...
					pattern2ColliderObject->GetTransform()->SetLocalPosition(m_gameObject->GetTransform()->GetLocalPosition() + m_gameObject->GetTransform()->GetFront() * 3.0f - Vector3D(0, 2, 0));
					pattern2ColliderObject->GetTransform()->SetLocalRotation(m_gameObject->GetTransform()->GetLocalRotation());
					pattern2ColliderObject->GetTransform()->SetLocalScale(Vector3D(3, 5.5, 5));
					pattern2ColliderObject->SetActive(true);

					pattern2ColliderDotween->DOMove(m_gameObject->GetTransform()->GetLocalPosition() + m_gameObject->GetTransform()->GetFront() * bossMoveDistance + m_gameObject->GetTransform()->GetFront() * 3.0f - Vector3D(0, 2, 0), bossMoveDistance / speed).SetEase(easeFunc);

					m_animator->Play(L"root|007-1.Idle->Skill_ChargeAttack");

					chargeSonicEffectObject->SetActive(true);
					chargeSonicEffectPlayer->SetMaxValue(250.0f);

					boss1Dotween->DONothing(0.13f).OnComplete([=]()
						{
							m_animator->Play(L"root|007-2.Skill_ChargeAttack");		// 달리는 모션
						});

					boss1Dotween->DOMove(m_gameObject->GetTransform()->GetLocalPosition() + m_gameObject->GetTransform()->GetFront() * bossMoveDistance, bossMoveDistance / speed)
						.SetEase(easeFunc).OnComplete([=]()
							{
								SoundManager::GetInstance().PlaySFX(SoundManager::Boss::Pattern2);
								cameraDotween->DOShakePosition(0.2f, 1.0f);
								m_animator->Play(L"root|007-3.Skill_ChargeAttack->Idle");
								chargeAttackEffectPlayer->Reset();
								chargeAttackEffectObject->SetActive(false);
								chargeSonicEffectPlayer->Reset();
								chargeSonicEffectObject->SetActive(false);

								boss1Dotween->DONothing(0.2f).OnComplete([=]()
									{
										tempCount++;
										if (tempCount % rushCount == 0)
										{
											pattern2ColliderObject->SetActive(false);
											isPattern2End = true;
											tempCount = 0;
										}
										else
										{
											m_animator->Play(L"root|000.Idle");
											Pattern2Function(rushCount, delay, speed, easeFunc);
											pattern2ColliderObject->SetActive(false);
										}
									});
							});
				});
		});
}

void Boss1::Pattern3Function(double speed, double walkSpeed, int attackCount)
{
	/// 횡, 종 평타 공격을 한 번씩 하는 패턴.
	/// 회전은 필요하다면 쓰고 우선 같은 Rotation의 오브젝트 두 개 만들자.
	/// 일단 한번 공격하는 함수를 만들어 두번 호출해볼까

	horizontalAttackEffectObject->GetTransform()->SetParent(m_gameObject->GetTransform());
	verticalAttackEffectObject->GetTransform()->SetParent(m_gameObject->GetTransform());

	horizontalAttackEffectObject->GetTransform()->SetLocalPosition(Vector3D(0, -4, 0));
	verticalAttackEffectObject->GetTransform()->SetLocalPosition(Vector3D(0, -4, 0));

	double playerXpos;
	double playerZpos;

	if (m_playerTransform->GetLocalPosition().x > 24.0f)
	{
		playerXpos = 24.0f;
	}
	else
	{
		playerXpos = m_playerTransform->GetLocalPosition().x;
	}

	if (m_playerTransform->GetLocalPosition().z > 24.0f)
	{
		playerZpos = 24.0f;
	}
	else
	{
		playerZpos = m_playerTransform->GetLocalPosition().z;
	}


	Vector3D playerPosition = Vector3D(playerXpos, 4.0f, playerZpos);

	Vector3D distanceVector = playerPosition - m_gameObject->GetTransform()->GetLocalPosition();
	double length = distanceVector.Size();

	LookAt(m_gameObject, playerPosition);

	double distance = (playerPosition - m_gameObject->GetTransform()->GetLocalPosition()).Size();

	Vector3D destinationVector = m_gameObject->GetTransform()->GetLocalPosition() + (playerPosition - m_gameObject->GetTransform()->GetLocalPosition()) * 0.8f;

	if (distance <= notMoveDistance)
		distance = 0.0f;

	if (distance != 0)
	{
		m_animator->Play(L"root|001-1.Idle->Walk");

		boss1Dotween->DONothing(0.1f / speed).OnComplete([=]()
			{
				m_animator->Play(L"root|001-2.Walk");
				bossParticleObject->GetTransform()->SetLocalPosition(Vector3D(0, 0, 0.5f));
			});
	}

	boss1Dotween->DOMove(destinationVector, distance / walkSpeed).SetEase(EaseBasic).OnComplete([=]()
		{
			if (distance != 0)
			{
				bossParticleObject->GetTransform()->SetLocalPosition(Vector3D(0, 0, 0));
				m_animator->Play(L"root|001-3.Walk->Idle");
			}
				

			boss1Dotween->DONothing(0.2f / speed).OnComplete([=]()
				{
					Vector3D playerPosition = m_playerTransform->GetLocalPosition();
					boss1Dotween->DOLookAt(m_playerTransform, 0.08f / speed, false).OnComplete([=]()
						{
							normalAttackGuideObject->SetActive(true);
							normalGuideEffectPlayer->SetMaxValue(1.0f);
							boss1Dotween->DONothing(normalAttackStartDelay / speed).OnComplete([=]()
								{
									normalAttackGuideObject->SetActive(false);

									m_animator->Play(L"root|002-1.NormalAttack_R");
									boss1Dotween->DONothing(normalAttackDelay / speed).OnComplete([=]()
										{
											m_animator->Play(L"root|002-2.NormalAttack_R->Idle");
											normalAttackGuideObject->SetActive(true);
											normalGuideEffectPlayer->SetMaxValue(1.0f);
										});

									SoundManager::GetInstance().PlaySFX(SoundManager::Boss::Pattern3);

									horizontalObject->GetTransform()->SetLocalPosition(m_gameObject->GetTransform()->GetLocalPosition() + m_gameObject->GetTransform()->GetFront() * 4.0f);
									horizontalObject->GetTransform()->SetLocalScale(Vector3D(6.0f, 8.0f, 9.0f/*박스 크기*/));
									horizontalObject->GetTransform()->SetLocalRotation(m_gameObject->GetTransform()->GetLocalRotation());

									horizontalObject->SetActive(true);
									verticalAttackEffectObject->SetActive(true);
									verticalAttackEffectPlayer->SetPlaySpeed(verticalAttackEffectSpeed * 1.5f);
									verticalAttackEffectPlayer->SetMaxValue(1.0f);



									boss1Dotween->DONothing((1.0f /*사이 애니메이션 시간*/ + normalAttackDelay/*딜레이*/) / speed).OnComplete([=]()
										{
											m_animator->Play(L"root|003-1.NormalAttack_L");
											boss1Dotween->DONothing(normalAttackDelay / speed - 0.2f).OnComplete([=]()
												{
													normalAttackGuideObject->SetActive(false);
												});
											boss1Dotween->DONothing(normalAttackDelay / speed).OnComplete([=]()
												{
													m_animator->Play(L"root|003-2.NormalAttack_L->Idle");
												});

											SoundManager::GetInstance().PlaySFX(SoundManager::Boss::Pattern3);

											verticalObject->GetTransform()->SetLocalPosition(m_gameObject->GetTransform()->GetLocalPosition() + m_gameObject->GetTransform()->GetFront() * 4.0f);
											verticalObject->GetTransform()->SetLocalScale(Vector3D(6.0f, 8.0f, 9.0f/*박스 크기*/));
											verticalObject->GetTransform()->SetLocalRotation(m_gameObject->GetTransform()->GetLocalRotation());
											verticalObject->SetActive(true);

											horizontalAttackEffectObject->SetActive(true);
											horizontalAttackEffectPlayer->SetMaxValue(1.0f);
											horizontalAttackEffectPlayer->SetPlaySpeed(horizontalAttackEffectSpeed * 1.5f);

											boss1Dotween->DONothing((1.0f /*사이 애니메이션 시간*/ + normalAttackDelay/*딜레이*/) / speed).OnComplete([=]()
												{
													isPattern3End = true;
													horizontalAttackEffectObject->SetActive(false);
													verticalAttackEffectObject->SetActive(false);

												});
										});

									// 공격은 따로 남았다가 없어진다.
									boss1Dotween->DONothing(0.2f / speed).OnComplete([=]()
										{
											horizontalObject->SetActive(false);
										});
									boss1Dotween->DONothing((0.2f + 0.2f/*딜레이*/ + 1.0f) / speed).OnComplete([=]()
										{
											verticalObject->SetActive(false);
										});
								});

						});
				});
		});






}

void Boss1::Pattern5Function(double speed)
{
	// 폭탄 발사 패턴
	// 플레이어 위치에다가 일단 던져보자
	// 던졌으면 던지기 시작했을 때 경고 표시, 도착하면 없애주기
	projectileObject->GetTransform()->SetLocalPosition(m_gameObject->GetTransform()->GetLocalPosition());

	LookAt(projectileObject, m_playerTransform->GetLocalPosition());

	projectileObject->GetTransform()->SetLocalPosition(projectileObject->GetTransform()->GetLocalPosition() + projectileObject->GetTransform()->GetFront().Normalize() * 3.0f);

	Vector3D playerPosition = m_playerTransform->GetLocalPosition();
	Vector3D bossPosition = projectileObject->GetTransform()->GetLocalPosition();


	// 가야 하는 거리
	double distanceX = playerPosition.x - bossPosition.x;
	double distanceY = playerPosition.y - bossPosition.y;
	double distanceZ = playerPosition.z - bossPosition.z;

	double pattern5Distance = (playerPosition - m_gameObject->GetTransform()->GetLocalPosition()).Size();
	explosionEffectPlayer->SetPlaySpeed(explosionEffectSpeed * speed);

	boss1Dotween->DOLookAt(m_playerTransform, 0.1f, false).OnComplete([=]()
		{
			m_animator->Play(L"root|004.RangedAttack");
		});

	boss1Dotween->DONothing(0.2f / speed).OnComplete([=]()
		{
			rangedAttackEffectObject->SetActive(true);
			rangedAttackEffectPlayer->SetMaxValue(1.0f);
		});

	boss1Dotween->DONothing(0.7f / speed).OnComplete([=]()
		{
			projectileObject->SetActive(true);
			childProjectileObject->SetActive(true);
		});

	pattern5ObjectDotween->DOCustom(arriveTimePattern5 / speed).OnUpdate([=]()
		{
			// 시작 값 저장
			auto lastElementIter = std::prev(pattern5ObjectDotween->m_dotweenTimerMap.end());

			Vector3D previousVector = Vector3D(lastElementIter->first->finalXvalue, lastElementIter->first->finalYvalue, lastElementIter->first->finalZvalue);

			lastElementIter->first->m_xValue = bossPosition.x;
			lastElementIter->first->m_yValue = bossPosition.y;
			lastElementIter->first->m_zValue = bossPosition.z;

			lastElementIter->first->m_ease = EaseOutExpo;

			if (!isHitPattern5)
			{
				// 시간 정규화 : 진행 시간 / 전체 기간
				lastElementIter->first->easeTime =
					(lastElementIter->first->elapsed - lastElementIter->first->delay) / (lastElementIter->first->duration - lastElementIter->first->delay);

				// 가야하는 거리의 정규화...
				// 진행 거리 / 전체 가야하는 거리
				// 진행 거리 = 시간 * 속력

				double easeWeight = getEasingFunction(lastElementIter->first->m_ease)(lastElementIter->first->easeTime);

				// 아래 세줄 안에 문제가 없다면 내 손에 장을 지짐 - 이윤우
				lastElementIter->first->movingDistanceX = easeWeight * (distanceX);
				lastElementIter->first->movingDistanceY = easeWeight * (distanceY);
				lastElementIter->first->movingDistanceZ = easeWeight * (distanceZ);

				lastElementIter->first->finalXvalue = lastElementIter->first->m_xValue + lastElementIter->first->movingDistanceX;
				lastElementIter->first->finalYvalue = lastElementIter->first->m_yValue + lastElementIter->first->movingDistanceY;
				lastElementIter->first->finalZvalue = lastElementIter->first->m_zValue + lastElementIter->first->movingDistanceZ;

				if (lastElementIter->first->easeTime >= 1)
				{
					lastElementIter->first->finalXvalue = playerPosition.x;
					lastElementIter->first->finalYvalue = playerPosition.y;
					lastElementIter->first->finalZvalue = playerPosition.z;
				}
				projectileObject->GetTransform()->
					SetLocalPosition(Vector3D(lastElementIter->first->finalXvalue, lastElementIter->first->finalYvalue, lastElementIter->first->finalZvalue));

				if ((projectileObject->GetTransform()->GetLocalPosition() - previousVector).Size() < 0.1f)
					childProjectileObject->SetActive(false);

			}

			else if (isHitPattern5)
			{
				// 해당 위치에서 폭발 시키는 로직
				Vector3D hitPosition =
					Vector3D(projectileObject->GetTransform()->GetLocalPosition().x, 0, projectileObject->GetTransform()->GetLocalPosition().z);
				pattern5ColliderObject1->GetTransform()->SetLocalPosition(hitPosition);
				pattern5ColliderObject1->GetTransform()->SetLocalScale(Vector3D(6, 6, 6));
				pattern5ColliderObject1->SetActive(true);
				projectileObject->SetActive(false);
				projectileObject->GetTransform()->SetLocalPosition(m_gameObject->GetTransform()->GetLocalPosition());

				lastElementIter->first->isDone = true;

				cameraDotween->DOShakePosition(0.2f / speed, 1.0f);

				rangedExposionEffectObject->SetActive(true);
				explosionEffectPlayer->SetMaxValue(1.0f);

				
				boss1Dotween->DONothing(0.2f / speed).OnComplete([=]()
					{
						SoundManager::GetInstance().PlaySFX(SoundManager::Boss::Pattern5);
					});

				boss1Dotween->DONothing(lastTimePattern5 / speed).OnComplete([=]()
					{
						pattern5ColliderObject1->SetActive(false);
						rangedAttackEffectObject->SetActive(false);
						rangedExposionEffectObject->SetActive(false);
						isPattern5End = true;
						isHitPattern5 = false;
					});
			}
		}).OnComplete([=]()
			{
				if (!isHitPattern5 && !isPattern5End)
				{
					/*projectileMeshRenderer*/
					//pattern5ObjectDotween->DOCustom(0.11f).OnUpdate([=]()
					//	{
					//		ci += TimeController::GetInstance().GetDeltaTime();
					//		//projectileMeshRenderer->SetColor(ZeldaColor(ci / 0.11f, 0, 0, 1));
					//		projectileMeshRenderer->SetColor(ZeldaColor(1, 0, 0, 1));
					//	});

					pattern5ObjectDotween->DONothing(0.3f - 0.2f / speed).OnComplete([=]()
						{
							SoundManager::GetInstance().PlaySFX(SoundManager::Boss::Pattern5);
						});

					pattern5ObjectDotween->DOShakePosition(0.3f / speed, 0.11f).OnComplete([=]()
						{
							projectileObject->SetActive(false);
							// Collider 생성
							pattern5ColliderObject1->GetTransform()->SetLocalPosition(playerPosition);
							pattern5ColliderObject1->GetTransform()->SetLocalScale(Vector3D(6, 6, 6));
							pattern5ColliderObject1->SetActive(true);

							cameraDotween->DOShakePosition(0.2f / speed, 1.0f);

							rangedExposionEffectObject->SetActive(true);
							explosionEffectPlayer->SetMaxValue(1.0f);

							projectileObject->GetTransform()->SetLocalPosition(m_gameObject->GetTransform()->GetLocalPosition());

							boss1Dotween->DONothing(lastTimePattern5 / speed).OnComplete([=]()
								{
									pattern5ColliderObject1->SetActive(false);
									rangedAttackEffectObject->SetActive(false);
									rangedExposionEffectObject->SetActive(false);
									isPattern5End = true;
									ci = 0;
								});
						});

				}
			});

}

/// <summary>
/// 한 곳을 찍고, 플레이어 방향으로 부채꼴 모양 공격.
/// </summary>
void Boss1::Pattern6Function(double speed, double walkSpeed)
{
	//pattern6GameObject1->SetActive(false);
	//pattern6GameObject2->SetActive(false);
	Vector3D bossPosition = Vector3D(m_gameObject->GetTransform()->GetLocalPosition().x, 4.0f, m_gameObject->GetTransform()->GetLocalPosition().z);

	double playerXpos;
	double playerYpos;
	double playerZpos;

	if (m_playerTransform->GetLocalPosition().x > 24.0f)
	{
		playerXpos = 24.0f;
	}
	else
	{
		playerXpos = m_playerTransform->GetLocalPosition().x;
	}

	if (m_playerTransform->GetLocalPosition().z > 24.0f)
	{
		playerZpos = 24.0f;
	}
	else
	{
		playerZpos = m_playerTransform->GetLocalPosition().z;
	}

	Vector3D playerPosition1 = Vector3D(playerXpos, 4.0f, playerZpos);

	Vector3D distanceVector = playerPosition1 - m_gameObject->GetTransform()->GetLocalPosition();
	double length = distanceVector.Size();

	LookAt(m_gameObject, playerPosition1);

	double distance = (playerPosition1 - m_gameObject->GetTransform()->GetLocalPosition()).Size();

	if (distance < notMoveDistance)
		distance = 0;

	if (distance != 0)
	{
		m_animator->Play(L"root|001-1.Idle->Walk");

		boss1Dotween->DONothing(0.1f).OnComplete([=]()
			{
				m_animator->Play(L"root|001-2.Walk");
			});

	}

	bossParticleObject->GetTransform()->SetLocalPosition(Vector3D(0, 0, 0.5f));

	Vector3D destinationVector = m_gameObject->GetTransform()->GetLocalPosition() + (playerPosition1 - m_gameObject->GetTransform()->GetLocalPosition()) * 0.8f;

	boss1Dotween->DOMove(destinationVector, distance / walkSpeed).SetEase(EaseBasic).OnComplete([=]()
		{
			if (distance != 0)
			{
				bossParticleObject->GetTransform()->SetLocalPosition(Vector3D(0, 0, 0));
				m_animator->Play(L"root|001-3.Walk->Idle");
			}

			boss1Dotween->DOLookAt(m_playerTransform, 0.1f, false).OnComplete([=]()
				{
					// 가이드 라인 이펙트 생성
					knockBackGuideLineObject->GetTransform()->SetParent(m_gameObject->GetTransform());
					knockBackGuideLineObject->GetTransform()->SetLocalPosition(Vector3D(0, -4, 0));
					knockBackGuideLineObject->SetActive(true);

					boss1Dotween->DONothing(guideLineRemainTime).OnComplete([=]()
						{
							knockBackGuideLineObject->SetActive(false);
						});

					boss1Dotween->DONothing(0.2f).OnComplete([=]()
						{
							isPattern6Damaged = false;

							boss1Dotween->DONothing(0.8f).OnComplete([=]()
								{
									Vector3D degreeVector3 = m_gameObject->GetTransform()->GetFront();
									Vector3D degreeVector4 = m_playerTransform->GetLocalPosition() - m_gameObject->GetTransform()->GetLocalPosition();


									Vector3D degreeVector1 = Vector3D(degreeVector3.x, 0, degreeVector3.z);
									Vector3D degreeVector2 = Vector3D(degreeVector4.x, 0, degreeVector4.z);

									Vector3D normalizedVector1 = degreeVector1.Normalize();
									Vector3D normalizedVector2 = degreeVector2.Normalize();

									double dot = DotProduct(normalizedVector1, normalizedVector2);

									double finalAngle = RadianToDegree(acos(std::clamp(dot, -1.0, 1.0)));

									if ((finalAngle < pattern6Angle / 2 && degreeVector2.Size() < pattern6Radius) && !isPattern6Damaged)
									{
										isPattern6Damaged = true;
										// GetComponent를 쓰기 싫은데...
										m_collisionManager[7]->DetectFanRange();
									}
								});



							boss1Dotween->DONothing(2.0f).OnComplete([=]()
								{
									isPattern6Damaged = false;
									isPattern6End = true;
									knockBackAttackEffectObject->SetActive(false);
								});


							boss1Dotween->DONothing(0.3f / speed).OnComplete([=]()
								{
									m_animator->Play(L"root|008.Skill_Knockback");

									boss1Dotween->DONothing(0.1f).OnComplete([=]()
										{
											// 이펙트 생성
											knockBackAttackEffectObject->GetTransform()->SetParent(m_gameObject->GetTransform());
											knockBackAttackEffectObject->GetTransform()->SetLocalPosition(Vector3D(0, -4, 0));
											knockBackAttackEffectObject->SetActive(true);
											boss1Dotween->DONothing(0.48f).OnComplete([=]()
												{
													cameraDotween->DOShakePosition(0.2f, 0.7f);
													SoundManager::GetInstance().PlaySFX(SoundManager::Boss::Pattern6);
												});

											knockBackAttackEffectPlayer->SetMaxValue(1.0f);
										});

								});
						});
				});
		});

}

void Boss1::PatternMoveFunction(Vector3D p_position, bool isPattern, double duration /*= 1.0f*/, double speed)
{
	/// isPattern 이 true일 경우에는 그냥 걸어서 이동
	/// false일 경우에는 아래로 슝 이동

	if (!isPattern)
	{
		// 보스 이동 시 
		teleportDisappearEffectObject->GetTransform()->SetLocalPosition(Vector3D(m_gameObject->GetTransform()->GetLocalPosition().x, 0.0f,
			m_gameObject->GetTransform()->GetLocalPosition().z));

		SoundManager::GetInstance().PlaySFX(SoundManager::Boss::Telepo);

		// 1. 포탈이 나온다
		teleportDisappearEffectObject->SetActive(true);
		teleportDisappearEffectPlayer->SetMaxValue(0.5f);

		boss1Dotween->DONothing(3.5f - 1.2f).OnComplete([this]()
			{
				// 2. 포탈이 사라진다
				teleportDisappearEffectPlayer->SetMaxValue(0.0f);
				teleportDisappearEffectPlayer->InverseDirection();
			});

		boss1Dotween->DONothing(2.7f).OnComplete([=]()
			{
				teleportAppearEffectObject->GetTransform()->SetLocalPosition(Vector3D(p_position.x, 0.0f, p_position.z));
				SoundManager::GetInstance().PlaySFX(SoundManager::Boss::Telepo);
				teleportAppearEffectObject->SetActive(true);
				teleportAppearEffectPlayer->SetMaxValue(0.5f);
			});

		boss1Dotween->DONothing(6.5 - 1.2f).OnComplete([this]()
			{
				teleportAppearEffectPlayer->SetMaxValue(0.0f);
				teleportAppearEffectPlayer->InverseDirection();
			});

		boss1Dotween->DOMove(m_gameObject->GetTransform()->GetLocalPosition() - Vector3D(0, 10, 0), 3.0f).SetDelay(0.5f).SetEase(EaseBasic).OnComplete([=]()
			{
				m_gameObject->GetTransform()->SetLocalPosition(Vector3D(p_position.x, -10, p_position.z));
				m_gameObject->GetTransform()->SetLocalRotation(Vector3D(0, 0, 0));
				boss1Dotween->DOMove(p_position, 3.0f).SetEase(EaseOutCubic);
				boss1Dotween->DONothing(8.0f).OnComplete([=]()
					{
						teleportDisappearEffectPlayer->InverseDirection();
						teleportAppearEffectPlayer->InverseDirection();
					});
			});

	}

	else if (isPattern)
	{
		bossParticleObject->GetTransform()->SetLocalPosition(Vector3D(0, 0, 0.5f));

		LookAt(m_gameObject, p_position);

		double distance = (p_position - m_gameObject->GetTransform()->GetLocalPosition()).Size();

		m_animator->Play(L"root|001-1.Idle->Walk");
		SoundManager::GetInstance().PlaySFX(SoundManager::Boss::Walk);

		boss1Dotween->DONothing(0.1f / speed).OnComplete([=]()
			{
				m_animator->Play(L"root|001-2.Walk");
			});

		boss1Dotween->DOMove(p_position, distance / speed).SetEase(EaseBasic).OnComplete([=]()
			{
				m_animator->Play(L"root|001-3.Walk->Idle");

				boss1Dotween->DONothing(0.2f / speed).OnComplete([=]()
					{
						isMoveEnd = true;
						bossParticleObject->GetTransform()->SetLocalPosition(Vector3D(0, 0, 0));
						walkSoundDelay = 0;
					});
			});

		boss1Dotween->DOCustom(distance / speed).OnUpdate([=]()
			{
				walkSoundDelay += TimeController::GetInstance().GetDeltaTime();
				if (walkSoundDelay > 0.5)
				{
					SoundManager::GetInstance().PlaySFX(SoundManager::Boss::Walk);
					walkSoundDelay = 0;
				}
			});

	}



}


/// <summary>
/// 기본 
/// </summary>
void Boss1::Pattern2DNormalFunction(int count, int lineNumber)
{
	m_bossHitCollider->GetGameObject()->SetActive(true);

	m_followCamera->FixCamera();

	for (int i = 0; i < lineNumber; i++)
	{
		double xPos = rand() % 250;
		double yPos = rand() % 250;

		int plusOrminusX = rand() % 2;
		int plusOrminusY = rand() % 2;
		if (plusOrminusX == 0)
		{
			xPos *= -1;
		}
		if (plusOrminusY == 0)
		{
			yPos *= -1;
		}
		double angle = rand() % 360;
		Vector3D Pattern2DPosition1 = Vector3D(xPos, yPos, 0);

		normal2DCollisionObject[i]->GetTransform()->SetLocalPosition(Pattern2DPosition1);
		normal2DWarningObject[i]->GetTransform()->SetLocalPosition(Pattern2DPosition1);

		normal2DCollisionObject[i]->GetTransform()->SetLocalUIRotation(angle);
		normal2DWarningObject[i]->GetTransform()->SetLocalUIRotation(angle);

		skill2DEffectObject->GetTransform()->SetParent(m_gameObject->GetTransform());
		skill2DEffectObject->GetTransform()->SetLocalPosition(Vector3D(0, -4, 0));

		normal2DWarningObject[i]->SetActive(true);

		boss1Dotween->DONothing(0.5f / phase2Speed).OnComplete([=]()
			{
				skill2DEffectObject->SetActive(true);
				skill2DEffectPlayer->SetMaxValue(1.0f);
				m_animator->Play(L"root|010.Skill_2D");
			});

		boss1Dotween->DONothing(0.5f + 0.3f + 0.2f / phase2Speed).OnComplete([=]()
			{
				normal2DWarningObject[i]->SetActive(false);
				normal2DCollisionObject[i]->SetActive(true);
				normal2DSpriteAnimator[i]->Play();
			});
	}

	boss1Dotween->DONothing(0.5f + 0.3f + 0.2f / phase2Speed + 0.1f).OnComplete([=]()
		{
			SoundManager::GetInstance().PlaySFX(SoundManager::Boss::Pattern7);
		});


	for (int i = 0; i < lineNumber; i++)
	{
		boss1Dotween->DONothing((0.5f + 0.3f + 0.2f + 1.5f) / phase2Speed).OnComplete([=]()
			{
				normal2DCollisionObject[i]->SetActive(false);
			});
	}

	boss1Dotween->DONothing((0.5f + 0.3f + 0.2f + 1.5f) / phase2Speed).OnComplete([=]()
		{
			boss1Dotween->DONothing(0.2f).OnComplete([=]()
				{
					temp2DPatternCount++;
					if (temp2DPatternCount % count == 0)
					{
						isPattern2DNormalEnd = true;
						m_followCamera->UnFixCamera();
						skill2DEffectObject->SetActive(false);
						temp2DPatternCount = 0;
					}
					else
					{
						boss1Dotween->DOLookAt(m_playerTransform, 0.2f / phase2Speed, false).OnComplete([=]()
							{
								Pattern2DNormalFunction(count, lineNumber);
							});
					}
				});
		});
}

void Boss1::DisruptingMovementFunction()
{
	// 시작 때 한 번 사용할 예정
	// Death에서 position을 가운데로 이동 시킨 후 실행될 예정
	m_bossHitCollider->GetGameObject()->SetActive(false);

	// 장애물 시작 위치
	Vector3D disruptingObject1StartPosition = Vector3D(12.5f, -13, -12.5f);		// 우측 45도
	Vector3D disruptingObject2StartPosition = Vector3D(-12.5f, -13, -12.5f);	// 좌측 45도
	Vector3D disruptingObject3StartPosition = Vector3D(0.0f, -13, 12.5f);		// 가운데
	disruptingObjects[0]->GetTransform()->SetLocalPosition(disruptingObject1StartPosition);
	disruptingObjects[1]->GetTransform()->SetLocalPosition(disruptingObject2StartPosition);
	disruptingObjects[2]->GetTransform()->SetLocalPosition(disruptingObject3StartPosition);

	disruptingColliderObjects[0]->GetTransform()->SetLocalPosition(Vector3D(12.5f, 4, -12.5f));
	disruptingColliderObjects[1]->GetTransform()->SetLocalPosition(Vector3D(-12.5f, 4, -12.5f));
	disruptingColliderObjects[2]->GetTransform()->SetLocalPosition(Vector3D(0.0f, 4, 12.5f));

	disruptingObjects[0]->GetTransform()->SetLocalRotation(Vector3D(0, -45, 0));
	disruptingObjects[1]->GetTransform()->SetLocalRotation(Vector3D(0, 45, 0));
	disruptingObjects[2]->GetTransform()->SetLocalRotation(Vector3D(0, 180, 0));

	disruptingObjects[0]->GetTransform()->SetLocalScale(Vector3D(2, 2, 2));
	disruptingObjects[1]->GetTransform()->SetLocalScale(Vector3D(2, 2, 2));
	disruptingObjects[2]->GetTransform()->SetLocalScale(Vector3D(2, 2, 2));

	// 장애물 끝 위치
	Vector3D disruptingObject1EndPosition = Vector3D(12.5f, 0, -12.5f);
	Vector3D disruptingObject2EndPosition = Vector3D(-12.5f, 0, -12.5f);
	Vector3D disruptingObject3EndPosition = Vector3D(0.0f, 0, 12.5f);
	Vector3D disruptingEndPosition[3] = { disruptingObject1EndPosition ,disruptingObject2EndPosition ,disruptingObject3EndPosition };

	// 포탈 위치
	Vector3D potal1Position = Vector3D(12.5f, 0, -12.5f);
	Vector3D potal2Position = Vector3D(-12.5f, 0, -12.5f);
	Vector3D potal3Position = Vector3D(0.0f, 0, 12.5f);

	// 포탈 위치 설정
	disruptingAppearEffectObject[0]->GetTransform()->SetLocalPosition(potal1Position);
	disruptingAppearEffectObject[1]->GetTransform()->SetLocalPosition(potal2Position);
	disruptingAppearEffectObject[2]->GetTransform()->SetLocalPosition(potal3Position);

	m_gameObject->GetTransform()->SetLocalRotation(Vector3D::zero);

	// 1. 내려찍기 애니메이션 재생
	m_animator->Play(L"root|009.Skill_Blocking");

	// 1.5. 이펙트는 0.5초로 하자
	boss1Dotween->DONothing(0.5f).OnComplete([=]()
		{
			SoundManager::GetInstance().PlaySFX(SoundManager::Boss::Pattern4);
			obstaclePatternEffectObject->SetActive(true);
			obstaclePatternEffectPlayer->SetMaxValue(1.0f);
		});

	// 2. 포탈을 연다, blockingAnimationEndTime 뒤에
	boss1Dotween->DONothing(blockingAnimationEndTime).OnComplete([=]()
		{
			cameraDotween->DOShakePosition(0.2f, 1.0f).SetDelay(0.5f);

			// 장애물 패턴 사운드

			for (int i = 0; i < 3; i++)
			{
				disruptingAppearEffectObject[i]->SetActive(true);
				disruptingAppearEffectPlayer[i]->SetMaxValue(0.5f);
			}

			for (auto& e : bossFlagComponentVector)
			{
				e->Dissolve();
			}
		});

	// 3. obstacle이 올라온다. blockingAnimationEndTime + potalOpenTime 뒤에
	for (int i = 0; i < 3; i++)
	{
		disruptingObjects[i]->SetActive(true);
		disruptingColliderObjects[i]->SetActive(false);

		boss1Dotween->DONothing(obstacleArriveTime / 2 + 2.0f).OnComplete([=]()
			{
				disruptingColliderObjects[i]->SetActive(true);
			});

		/*obstacleDotween[i]->DOQRotate(disruptingObjects[i]->GetTransform()->GetUp(), 360, obstacleArriveTime - 0.5f).SetDelay(blockingAnimationEndTime + potalOpenTime + 0.5f)
			.SetEase(EaseOutCirc);*/

		obstacleDotween[i]->DOMove(disruptingEndPosition[i], obstacleArriveTime).SetDelay(blockingAnimationEndTime + potalOpenTime).OnComplete([=]()
			{
				for (auto& e : floatingObjectComponentVector)
				{
					e->RevertObject();
				}
				isFloatObjectReverted = true;

				// 4. 포탈이 닫힌다.
				disruptingAppearEffectPlayer[i]->SetMaxValue(0.0f);
				disruptingAppearEffectPlayer[i]->InverseDirection();

				// SetKinematic을 false로 바꿔주자. freeze도 해주자

				obstacleCollider[i]->SetTrigger(false);

				boss1Dotween->DONothing(potalCloseTime).OnComplete([=]()
					{
						obstaclePatternEffectObject->SetActive(false);
					});
			});
	}

	boss1Dotween->DONothing(potalCloseTime + obstacleArriveTime + blockingAnimationEndTime + potalOpenTime - 1.5f).OnComplete([=]()
		{
			isDistruptingMovementEnd = true;
			reserve2DPattern = true;
			obstaclePatternEffectObject->SetActive(false);
			m_floatingObjectIter = floatingObjectComponentVector.begin();

		});

}

void Boss1::Pattern2DUltimateFunction()
{
	m_followCamera->FixCamera();

	// 난수로 결정되는 패턴에 사용될 오브젝트
	GameObject* patternImageObject = nullptr;

	int chooseDirection = rand() % 4;

	switch (chooseDirection)
	{
		case 0:
			for (int i = 0; i < 10; i++)
			{
				if (i % 2 == 0)
				{
					ultimateCollisionObject[i]->GetTransform()->SetLocalUIRotation(90);
					ultimateCollisionObject[i]->GetTransform()->SetLocalPosition(Vector3D(0, (i * 100) + 50, 0));
				}
				else
				{
					ultimateCollisionObject[i]->GetTransform()->SetLocalUIRotation(270);
					ultimateCollisionObject[i]->GetTransform()->SetLocalPosition(Vector3D(0, -(i * 100) - 50, 0));
				}
			}
			patternImageObject = ultimateRightObject;						// 이미지
			leftWarningDegree = 180;
			break;
		case 1:
			for (int i = 0; i < 10; i++)
			{
				if (i % 2 == 0)
				{
					ultimateCollisionObject[i]->GetTransform()->SetLocalUIRotation(270);
					ultimateCollisionObject[i]->GetTransform()->SetLocalPosition(Vector3D(0, (i * 100) + 50, 0));
				}
				else
				{
					ultimateCollisionObject[i]->GetTransform()->SetLocalUIRotation(90);
					ultimateCollisionObject[i]->GetTransform()->SetLocalPosition(Vector3D(0, -(i * 100) - 50, 0));
				}
			}
			patternImageObject = ultimateLeftObject;						// 이미지
			leftWarningDegree = 0;
			break;
		case 2:
			for (int i = 0; i < 10; i++)
			{
				if (i % 2 == 0)
				{
					ultimateCollisionObject[i]->GetTransform()->SetLocalUIRotation(180);
					ultimateCollisionObject[i]->GetTransform()->SetLocalPosition(Vector3D(0, (i * 100) + 50, 0));
				}
				else
				{
					ultimateCollisionObject[i]->GetTransform()->SetLocalPosition(Vector3D(0, -(i * 100) - 50, 0));
				}
			}
			patternImageObject = ultimateBottomObject;						// 이미지
			topWarningDegree = 180;
			break;
		case 3:
			for (int i = 0; i < 10; i++)
			{
				if (i % 2 == 0)
				{
					ultimateCollisionObject[i]->GetTransform()->SetLocalPosition(Vector3D(0, (i * 100) + 50, 0));
				}
				else
				{
					ultimateCollisionObject[i]->GetTransform()->SetLocalUIRotation(180);
					ultimateCollisionObject[i]->GetTransform()->SetLocalPosition(Vector3D(0, -(i * 100) - 50, 0));
				}
			}
			patternImageObject = ultimateTopObject;						// 이미지
			topWarningDegree = 0;
			break;
	}

	ultimateObjectDotween = patternImageObject->AddComponent<Dotween>();

	ultimateWarningTopObject->GetTransform()->SetLocalUIRotation(topWarningDegree);
	ultimateWarningLeftObject->GetTransform()->SetLocalUIRotation(leftWarningDegree);

	m_animator->Play(L"root|010.Skill_2D");

	if (chooseDirection == 0 || chooseDirection == 1)
		ultimateWarningLeftObject->SetActive(true);
	else
		ultimateWarningTopObject->SetActive(true);

	boss1Dotween->DONothing(normal2DEffectSync / phase2Speed).OnComplete([=]()
		{
			skill2DEffectObject->GetTransform()->SetParent(m_gameObject->GetTransform());
			skill2DEffectObject->GetTransform()->SetLocalPosition(Vector3D(0, -4, 0));
			skill2DEffectObject->SetActive(true);

			skill2DEffectPlayer->SetMaxValue(1.0f);
		});

	boss1Dotween->DONothing(ultimateWarningTime / phase2Speed).OnComplete([=]()
		{
			ultimateWarningTopObject->GetTransform()->SetLocalUIRotation(0);
			ultimateWarningLeftObject->GetTransform()->SetLocalUIRotation(0);

			ultimateWarningTopObject->SetActive(false);
			ultimateWarningLeftObject->SetActive(false);

			for (int i = 0; i < 10; i++)
			{
				boss1Dotween->DONothing(i * 0.03f).OnComplete([=]()
					{
						ultimateCollisionObject[i]->SetActive(true);
						ultimateSpriteAnimator[i]->Play();
						if (i % 2 == 0)
						{
							boss1Dotween->DONothing(0.1f).OnComplete([=]()
								{
									SoundManager::GetInstance().PlaySFX(SoundManager::Boss::Pattern7);
								});
						}
					});
			}

			patternImageObject->SetActive(true);

			boss1Dotween->DONothing(ultimateObjectDuration).OnComplete([=]()
				{
					for (int i = 0; i < 10; i++)
					{
						ultimateCollisionObject[i]->GetTransform()->SetLocalUIRotation(0);
						ultimateCollisionObject[i]->SetActive(false);
					}
					boss1Dotween->DONothing(1.0f / phase2Speed).OnComplete([=]()
						{
							isPattern2DUltimateEnd = true;
							m_followCamera->UnFixCamera();
							ultimateRightObject->SetActive(false);
							ultimateTopObject->SetActive(false);
							ultimateLeftObject->SetActive(false);
							ultimateBottomObject->SetActive(false);

							ultimateCollisionBottomObject->SetActive(false);
							ultimateCollisionTopObject->SetActive(false);
							ultimateCollisionLeftObject->SetActive(false);
							ultimateCollisionRightObject->SetActive(false);

							skill2DEffectObject->SetActive(false);

							// 시작 위치 설정
							ultimateBottomObject->GetTransform()->SetLocalPosition(Vector3D(0, -540, 0));
							ultimateTopObject->GetTransform()->SetLocalPosition(Vector3D(0, 540, 0));
							ultimateLeftObject->GetTransform()->SetLocalPosition(Vector3D(-960, 0, 0));
							ultimateRightObject->GetTransform()->SetLocalPosition(Vector3D(960, 0, 0));
						});
				});
		});
}

/// <summary>
/// 상태 선택 함수. 지금 상태 나오면 다시 굴려준다.
/// </summary>
void Boss1::ChoosePattern()
{
	// 페이즈 1 상태 전환 로직
	if (phaseNumber == 1)
	{
		if (currenthp <= maxHp - hpPerScroll * 4)
		{
			phase1RageOn = true;
		}

		// 보스 hp에 따른 가지 패턴
		if (currenthp <= maxHp - hpPerScroll * 2 && !isBranchPattern1Used)
			patternNum = int(Boss1State::Pattern1);

		else if (currenthp <= maxHp - hpPerScroll * 6 && !isBranchPattern3Used)
			patternNum = int(Boss1State::Pattern1);

		else if (currenthp <= 0)
			patternNum = int(Boss1State::Death);
		else
		{
			int temp = rand() % int(Boss1State::Idle);
			if (temp == previousStateNum)
				ChoosePattern();
			else
				patternNum = temp;
		}
	}

	// 페이즈 2 상태 전환 로직
	else if (phaseNumber == 2)
	{
		if (!isPhase2StartPatternUsed)
		{
			patternNum = int(Boss1State::DisruptingMovement);
			isPhase2StartPatternUsed = true;
		}

		else if (reserve2DPattern)
		{
			patternNum = int(Boss1State::Pattern2DNormal);
			gameManager->ShowPhase2CutScene2();
			reserve2DPattern = false;
		}

		else
		{
			// 사망
			if (currenthp <= 0)
				patternNum = int(Boss1State::Death);


			// 족자 2개일 때 강화 나뭇가지 패턴
			else if (currenthp <= hpPerScroll * 2 && !isPhase2BranchPatternUsed)
				patternNum = int(Boss1State::Pattern1);

			// 광폭화. 2D 패턴의 빈도 증가 2 : 1 정도?
			else if (currenthp <= hpPerScroll * 4)
			{
				// 페이즈 2 광폭화
				phase2RageOn = true;

				int temp1 = rand() % phase2RageOnPatternNumber;
				switch (temp1)
				{
					case 0:
						patternNum = int(Boss1State::Pattern2);
						break;
					case 1:
						patternNum = int(Boss1State::Pattern3);
						break;
					case 2:
						patternNum = int(Boss1State::Pattern6);
						break;
					case 3:
						patternNum = int(Boss1State::Pattern5);
						break;
					case 4:
						patternNum = int(Boss1State::Pattern2DNormal);		// 일자 패턴
						break;
					case 5:
						patternNum = int(Boss1State::Pattern2DUltimate);	// 샌즈 패턴
						break;
					case 6:
						patternNum = int(Boss1State::Pattern2DNormal);		// 일자 패턴
						break;
					case 7:
						patternNum = int(Boss1State::Pattern2DUltimate);	// 샌즈 패턴
						break;
					case 8:
						patternNum = int(Boss1State::Pattern2DNormal);		// 일자 패턴
						break;
					case 9:
						patternNum = int(Boss1State::Pattern2DUltimate);	// 샌즈 패턴
						break;
				}
			}

			else
			{
				int temp2 = rand() % phase2PatternNumber;
				switch (temp2)
				{
					case 0:
						patternNum = int(Boss1State::Pattern2);				// 돌진
						break;
					case 1:
						patternNum = int(Boss1State::Pattern3);				// 평타
						break;
					case 2:
						patternNum = int(Boss1State::Pattern5);				// 평타
						break;
					case 3:
						patternNum = int(Boss1State::Pattern6);				// 밀격
						break;
					case 4:
						patternNum = int(Boss1State::Pattern2DNormal);		// 일자 패턴
						break;
					case 5:
						patternNum = int(Boss1State::Pattern2DUltimate);	// 샌즈 패턴
						break;
				}

				if (patternNum == previousStateNum)
					ChoosePattern();
			}

			if (patternNum == previousStateNum)
				ChoosePattern();
		}
	}
}

// idleEngage 에 넣어주자
void Boss1::ApplyDamage(int phasenum, bool isRageOn)
{
	// 1페이즈 광폭화 후
	if (phasenum == 1 && isRageOn == true)
	{
		branchDamage = 13;
		chargeDamage = 8;
		normalAttackDamage = 9;
		rangedAttackDamage = 9;
		fanDamage = 9;
	}

	// 2페이즈 광폭화 후
	else if (phasenum == 2 && isRageOn == true)
	{
		branchDamage = 15;
		chargeDamage = 10;
		normalAttackDamage = 11;
		rangedAttackDamage = 11;
		fanDamage = 12;
	}


	m_collisionManager[0]->SetPatternDamage(branchDamage);
	m_collisionManager[1]->SetPatternDamage(branchDamage);
	m_collisionManager[2]->SetPatternDamage(chargeDamage);
	m_collisionManager[3]->SetPatternDamage(normalAttackDamage);
	m_collisionManager[4]->SetPatternDamage(normalAttackDamage);
	m_collisionManager[5]->SetPatternDamage(rangedAttackDamage);
	m_collisionManager[6]->SetPatternDamage(rangedAttackDamage);
	m_collisionManager[7]->SetPatternDamage(fanDamage);

}

void Boss1::RevertToPhaseTwo()
{
	jumpToPhase2 = true;
	phaseNumber = 2;
	currenthp = maxHp;
	phase1RageOn = true;
	phase2RageOn = false;
	temp2DPatternCount = 0;
	previousStateNum = int(Boss1State::Death);
	boss1FSM->currentState = Boss1State::Idle;
	isPhase2StartPatternUsed = false;
	reserve2DPattern = false;
	rage1AnimationPlayed = true;
	m_gameObject->GetTransform()->SetLocalPosition(Vector3D(0, 4, 0));

	phase1Speed = phase1RageSpeed;
	finalRushCount = rageRushCount;

	for (auto e : dotweenVector)
	{
		if (e == nullptr)
			continue;

		if (e->tempTimer == nullptr)
			continue;

		e->clearDotweenTimerMap();
	}


	// dotween 내부의 끄고 켜는 GameObject 비활성화
	branch12ColliderObject->SetActive(false);
	branch34ColliderObject->SetActive(false);
	horizontalObject->SetActive(false);
	verticalObject->SetActive(false);
	projectileObject->SetActive(false);
	pattern5ColliderObject1->SetActive(false);
	pattern2ColliderObject->SetActive(false);
	rageTextureObject->SetActive(false);

	for (int i = 0; i < 10; i++)
	{
		normal2DCollisionObject[i]->SetActive(false);
		normal2DWarningObject[i]->SetActive(false);
	}

	ultimateWarningTopObject->SetActive(false);
	ultimateWarningLeftObject->SetActive(false);
	ultimateBottomObject->SetActive(false);
	ultimateTopObject->SetActive(false);
	ultimateLeftObject->SetActive(false);
	ultimateRightObject->SetActive(false);
	ultimateCollisionBottomObject->SetActive(false);
	ultimateCollisionTopObject->SetActive(false);
	ultimateCollisionLeftObject->SetActive(false);
	ultimateCollisionRightObject->SetActive(false);
	barrierCollisionObject->SetActive(false);
	barrierTriggerObject->SetActive(false);
	protectEffectObject->SetActive(false);
	circleEffectObject->SetActive(false);
	teleportDisappearEffectObject->SetActive(false);
	teleportAppearEffectObject->SetActive(false);
	horizontalAttackEffectObject->SetActive(false);
	verticalAttackEffectObject->SetActive(false);
	rangedAttackEffectObject->SetActive(false);
	rangedExposionEffectObject->SetActive(false);
	knockBackAttackEffectObject->SetActive(false);
	knockBackGuideLineObject->SetActive(false);
	skill2DEffectObject->SetActive(false);
	chargeAttackEffectObject->SetActive(false);
	chargeSonicEffectObject->SetActive(false);
	chargeGuideEffectObject->SetActive(false);
	normalAttackGuideObject->SetActive(false);
	obstaclePatternEffectObject->SetActive(false);


	for (auto itr = colliderobjectsPairVector.begin(); itr != colliderobjectsPairVector.end(); itr++)
	{
		itr->first->SetActive(false);
	}

	for (int i = 0; i < 3; i++)
	{
		disruptingObjects[i]->SetActive(false);
		disruptingAppearEffectObject[i]->SetActive(false);
	}

	for (int i = 0; i < 4; i++)
	{
		branchEffectObjects[i]->SetActive(false);
	}

	m_animator->Play(L"root|000.Idle");
}

/// <summary>
/// obj가 lookPosition을 바라보도록 하는 함수
/// </summary>
/// <param name="obj"></param>
/// <param name="lookPosition"></param>
void Boss1::LookAt(ZeldaEngine::GameObject* obj, ZeldaEngine::ZeldaMath::Vector3D lookPosition)
{
	Vector3D frontVector = obj->GetTransform()->GetFront();
	Vector3D distanceVector = lookPosition - obj->GetTransform()->GetLocalPosition();

	Vector3D axis = CrossProduct(frontVector, distanceVector);

	double dot = DotProduct(frontVector, distanceVector);

	double length = frontVector.Size() * distanceVector.Size();

	double finalAngle = acos(std::clamp(dot / length, -1.0, 1.0));

	if (axis.y < 0)
		finalAngle = finalAngle * -1;

	obj->GetTransform()->Rotate(m_gameObject->GetTransform()->GetUp(), finalAngle);
}

void Boss1::SetPlayerTransform(Transform* playerTransform)
{
	m_playerTransform = playerTransform;
}

void Boss1::SetPlayerCollider(CapsuleCollider* playerCollider)
{
	m_playerCollider = playerCollider;
}

void Boss1::SetBossHitCollider(ZeldaEngine::CapsuleCollider* bossCollider)
{
	m_bossHitCollider = bossCollider;
}

void Boss1::SetPlayerRigidBody(ZeldaEngine::Rigidbody* playerRigidBody)
{
	m_playerRigidBody = playerRigidBody;
}

void Boss1::SetPlayerGameObject(ZeldaEngine::GameObject* playerGameObject)
{
	m_playerGameObject = playerGameObject;
}


void Boss1::SetPlayerComponent(Player* playerComponent)
{
	m_playerComponent = playerComponent;
}

void Boss1::SetBossHitSpriteAnimator(SpriteAnimator* hitanimator)
{
	bossHitSpriteAnimator = hitanimator;
}

void Boss1::SetScrollComponent(FloatingObject* floatingComponent)
{
	tempFloatingComponent = floatingComponent;
	floatingObjectComponentVector.push_back(tempFloatingComponent);
}

void Boss1::SetBossFlagComponent(BossFlag* flagComponent)
{
	tempbossComponent = flagComponent;
	bossFlagComponentVector.push_back(tempbossComponent);
}

void Boss1::SetFollowCameraComponent(FollowCamera* followComponent)
{
	m_followCamera = followComponent;
}


void Boss1::Pattern5Hit()
{
	isHitPattern5 = true;
}



void Boss1::EdgeOff()
{
	bossEdge->SetUseCollision(false);
	bossEdge->OffEdge();
}

CapsuleCollider* Boss1::GetBossCollider()
{
	return m_bossHitCollider;
}

void Boss1::OnDamaged(float damage, int type)
{
	// 플레이어가 호출할 함수. damage에 따라 hp가 까이고
	// 플레이어의 공격 타입에 따라 처리를 다르게 할 수 있다 ex) 이펙트, 밀려나는 효과 등등

	this->currenthp -= damage;

	if (currenthp > 0)
	{
		cameraDotween->DOShakePosition(0.1f, 0.5f);
	}

	if (phaseNumber == 2 && currenthp <= 0)
	{
		gameManager->ShowPhase2EndEvent();
	}

	SoundManager::GetInstance().PlaySFX(SoundManager::Boss::Damaged);
	bossHitSpriteAnimator->Play();
	bossHitSpriteAnimator->GetGameObject()->GetTransform()->SetLocalPosition(m_gameObject->GetTransform()->GetScreenPosition());
	if (currenthp >= 0)
	{
		if (!isJustDamaged)
		{
			isJustDamaged = true;
			boss1SkinnedMeshRenderer->SetColor(ZeldaColor::Red);
			boss1Dotween->DONothing(1.0f).OnComplete([=]()
				{
					boss1SkinnedMeshRenderer->SetColor(ZeldaColor::White);
					isJustDamaged = false;
				});
		}

		if (currenthp % hpPerScroll == 0 && currenthp != maxHp)
		{
			if (!(boss1FSM->currentState == Boss1State::DisruptingMovement && !isFloatObjectReverted))
			{
				if (m_floatingObjectIter != floatingObjectComponentVector.end())
				{
					(*m_floatingObjectIter)->DownObject();
					m_floatingObjectIter++;
				}
			}
		}
	}
}

void Boss1::PreSerialize(json& jsonData) const
{
	PRESERIALIZE_BASE(Boss1);

	PRESERIALIZE_VALUE(maxHp);
	PRESERIALIZE_VALUE(phase1RageSpeed);
	PRESERIALIZE_VALUE(phase2RageSpeed);
	PRESERIALIZE_VALUE(phase1Speed);
	PRESERIALIZE_VALUE(phase2Speed);
	PRESERIALIZE_VALUE(nomalRushCount);
	PRESERIALIZE_VALUE(rageRushCount);
	PRESERIALIZE_VALUE(pattern2DNormalLineNumber);
	PRESERIALIZE_VALUE(pattern2DRageLineNumber);
	PRESERIALIZE_VALUE(pattern2DNormalCount);
	PRESERIALIZE_VALUE(pattern2DRageCount);
	PRESERIALIZE_VALUE(ultimateWarningTime);
	PRESERIALIZE_VALUE(normalAttackDelay);
	PRESERIALIZE_VALUE(normalAttackStartDelay);
	PRESERIALIZE_VALUE(defaultMoveSpeed);
	PRESERIALIZE_VALUE(arriveTimePattern5);
	PRESERIALIZE_VALUE(lastTimePattern5);
	PRESERIALIZE_VALUE(guideLineRemainTime);
	PRESERIALIZE_VALUE(groggyTime);
	PRESERIALIZE_VALUE(branchDamage);
	PRESERIALIZE_VALUE(chargeDamage);
	PRESERIALIZE_VALUE(normalAttackDamage);
	PRESERIALIZE_VALUE(rangedAttackDamage);
	PRESERIALIZE_VALUE(fanDamage);

	PRESERIALIZE_VECTOR_PTR(floatingObjectComponentVector);
	PRESERIALIZE_VECTOR_PTR(bossFlagComponentVector);
}

void Boss1::PreDeserialize(const json& jsonData)
{
	PREDESERIALIZE_BASE(Boss1);

	PREDESERIALIZE_VALUE(maxHp);
	PREDESERIALIZE_VALUE(phase1RageSpeed);
	PREDESERIALIZE_VALUE(phase2RageSpeed);
	PREDESERIALIZE_VALUE(phase1Speed);
	PREDESERIALIZE_VALUE(phase2Speed);
	PREDESERIALIZE_VALUE(nomalRushCount);
	PREDESERIALIZE_VALUE(rageRushCount);
	PREDESERIALIZE_VALUE(pattern2DNormalLineNumber);
	PREDESERIALIZE_VALUE(pattern2DRageLineNumber);
	PREDESERIALIZE_VALUE(pattern2DNormalCount);
	PREDESERIALIZE_VALUE(pattern2DRageCount);
	PREDESERIALIZE_VALUE(ultimateWarningTime);
	PREDESERIALIZE_VALUE(normalAttackDelay);
	PREDESERIALIZE_VALUE(normalAttackStartDelay);
	PREDESERIALIZE_VALUE(defaultMoveSpeed);
	PREDESERIALIZE_VALUE(arriveTimePattern5);
	PREDESERIALIZE_VALUE(lastTimePattern5);
	PREDESERIALIZE_VALUE(guideLineRemainTime);
	PREDESERIALIZE_VALUE(groggyTime);
	PREDESERIALIZE_VALUE(branchDamage);
	PREDESERIALIZE_VALUE(chargeDamage);
	PREDESERIALIZE_VALUE(normalAttackDamage);
	PREDESERIALIZE_VALUE(rangedAttackDamage);
	PREDESERIALIZE_VALUE(fanDamage);


	PREDESERIALIZE_VECTOR_PTR(floatingObjectComponentVector);
	PREDESERIALIZE_VECTOR_PTR(bossFlagComponentVector);
}

void Boss1::PostSerialize(json& jsonData) const
{
	POSTSERIALIZE_BASE();
	POSTSERIALIZE_PTR(m_playerTransform);
	POSTSERIALIZE_PTR(m_playerGameObject);
	POSTSERIALIZE_PTR(m_playerCollider);
	POSTSERIALIZE_PTR(m_playerRigidBody);
	POSTSERIALIZE_PTR(m_playerComponent);
	POSTSERIALIZE_PTR(m_bossHitCollider);
	POSTSERIALIZE_PTR(bossHitSpriteAnimator);
	POSTSERIALIZE_PTR(tempFloatingComponent);
	POSTSERIALIZE_PTR(tempbossComponent);
	POSTSERIALIZE_PTR(gameManager);
	POSTSERIALIZE_PTR(m_followCamera);

	POSTSERIALIZE_VECTOR_PTR(floatingObjectComponentVector);
	POSTSERIALIZE_VECTOR_PTR(bossFlagComponentVector);
}

void Boss1::PostDeserialize(const json& jsonData)
{
	POSTDESERIALIZE_BASE();
	POSTDESERIALIZE_PTR(m_playerTransform);
	POSTDESERIALIZE_PTR(m_playerGameObject);
	POSTDESERIALIZE_PTR(m_playerCollider);
	POSTDESERIALIZE_PTR(m_playerRigidBody);
	POSTDESERIALIZE_PTR(m_playerComponent);
	POSTDESERIALIZE_PTR(m_bossHitCollider);
	POSTDESERIALIZE_PTR(bossHitSpriteAnimator);
	POSTDESERIALIZE_PTR(tempFloatingComponent);
	POSTDESERIALIZE_PTR(tempbossComponent);
	POSTDESERIALIZE_PTR(gameManager);
	POSTDESERIALIZE_PTR(m_followCamera);

	POSTDESERIALIZE_VECTOR_PTR(floatingObjectComponentVector);
	POSTDESERIALIZE_VECTOR_PTR(bossFlagComponentVector);
}
