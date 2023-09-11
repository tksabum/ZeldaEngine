#include <Windows.h>

#pragma comment(lib, "ZeldaEngine.lib")

#include "ZeldaEngine.h"
#include "SampleComponent.h"
#include "Boss1.h"
#include "ObjectPool.h"
//#include "NDComponent.h"
#include "Player.h"
#include "FollowCamera.h"
#include "Dotween.h"
#include "BossCollisionManager.h"
#include "PlayerCollisionManager.h"
#include "Player2DCollisionManager.h"
#include "FloatingObject.h"
#include "AnimatorTest.h"
#include "RandomRotate.h"
#include "StartButton.h"
#include "ExitButton.h"
#include "RestartButton.h"
#include "SJ/TitleButton.h"
#include "SJ/ContinueButton.h"
#include "SJ/CoolTimeUI.h"
#include "SJ/DashCoolTimeUI.h"
#include "SJ/StandoffCoolTimeUI.h"
#include "SJ/HPUI.h"
#include "SJ/AttachUI.h"
#include "SJ/NumUI.h"
#include "SJ/BaseUI.h"
#include "SJ/MovableUI.h"
#include "SJ/CoverUI.h"
#include "SJ/SettingButton.h"
#include "SJ/SoundBar.h"
#include "SJ/ScrollUI.h"
#include "SJ/SoundHandle.h"
#include "SJ/ButtonSelector.h"
#include "SJ/RailMoveCamera.h"
#include "SJ/CameraRailPoint.h"
#include "SJ/RailTriggerPoint.h"
#include "GameManager.h"
#include "Tracer.h"
#include "TriggerEventSetter.h"
#include "Phase3EventManager.h"

#include "EffectPlayer.h"
#include "BossFlag.h"
#include "EdgeDetect.h"
#include "Phase3Character.h"
#include "StaffRoll.h"

using namespace std;
using namespace ZeldaEngine;
using namespace ZeldaEngine::ZeldaMath;


void CreateResourceBJ();
void CreateResourceDH();
void CreateResourceGH();
void CreateResourceSC();
void CreateResourceGame();

///
/// 예제 1.
/// 가장 기본적인 형태의 Windows 프로그램 (유니코드 버전)
/// WinMain 디폴트
/// 2022.11.09 게임인재원 김범준

/// <summary>
/// Win32API를 이용한 데스크톱 어플리케이션
/// 프로그램의 엔트리 포인트
/// </summary>
/// <param name="hInstance">인스턴스의 핸들</param>
/// <param name="hPrevInstance">이전 인스턴스의 핸들(Win16의 잔재)</param>
/// <param name="lpCmdLine">실행시 인자</param>
/// <param name="nCmdShow">실행시 인자</param>
/// <returns></returns>
int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	EngineSetting* settingtest = &EngineSetting::GetInstance();
	SceneManager* scenetest = &SceneManager::GetInstance();
	ResourceManager* resourcetest = &ResourceManager::GetInstance();

	// 추가 컴포넌트 등록
	ComponentFactory::RegisterComponent<SampleComponent>(L"SampleComponent");
	ComponentFactory::RegisterComponent<Player>(L"Player");
	//ComponentFactory::RegisterComponent<NDComponent>(L"NDComponent");
	ComponentFactory::RegisterComponent<RandomRotate>(L"RandomRotate");
	ComponentFactory::RegisterComponent<FollowCamera>(L"FollowCamera");
	ComponentFactory::RegisterComponent<Boss1>(L"Boss1");
	ComponentFactory::RegisterComponent<Dotween>(L"Dotween");
	ComponentFactory::RegisterComponent<BossCollisionManager>(L"BossCollisionManager");
	ComponentFactory::RegisterComponent<PlayerCollisionManager>(L"PlayerCollisionManager");
	ComponentFactory::RegisterComponent<FloatingObject>(L"FloatingObject");
	ComponentFactory::RegisterComponent<RestartButton>(L"RestartButton");
	ComponentFactory::RegisterComponent<StartButton>(L"StartButton");
	ComponentFactory::RegisterComponent<ExitButton>(L"ExitButton");
	ComponentFactory::RegisterComponent<GameManager>(L"GameManager");
	ComponentFactory::RegisterComponent<Player2DCollisionManager>(L"Player2DCollisionManager");
	ComponentFactory::RegisterComponent<TriggerEventSetter>(L"TriggerEventSetter");

	/// 김성찬 
	ComponentFactory::RegisterComponent<EffectPlayer>(L"EffectPlayer");
	ComponentFactory::RegisterComponent<Animator>(L"Animator");
	ComponentFactory::RegisterComponent<EdgeDetect>(L"EdgeDetect");
	ComponentFactory::RegisterComponent<Phase3Character>(L"Phase3Character");
	ComponentFactory::RegisterComponent<Phase3EventManager>(L"Phase3EventManager");
	ComponentFactory::RegisterComponent<StaffRoll>(L"StaffRoll");

	/// 김상준
	ComponentFactory::RegisterComponent<BaseUI>(L"BaseUI");
	ComponentFactory::RegisterComponent<MovableUI>(L"MovableUI");
	ComponentFactory::RegisterComponent<CoverUI>(L"CoverUI");
	ComponentFactory::RegisterComponent<NumUI>(L"NumUI");
	ComponentFactory::RegisterComponent<AttachUI>(L"AttachUI");
	ComponentFactory::RegisterComponent<DashCoolTimeUI>(L"DashCoolTimeUI");
	ComponentFactory::RegisterComponent<StandoffCoolTimeUI>(L"StandoffCoolTimeUI");
	ComponentFactory::RegisterComponent<HPUI>(L"HPUI");
	ComponentFactory::RegisterComponent<SettingButton>(L"SettingButton");
	ComponentFactory::RegisterComponent<SoundBar>(L"SoundBar");
	ComponentFactory::RegisterComponent<SoundHandle>(L"SoundHandle");
	ComponentFactory::RegisterComponent<ScrollUI>(L"ScrollUI");
	ComponentFactory::RegisterComponent<TitleButton>(L"TitleButton");
	ComponentFactory::RegisterComponent<ContinueButton>(L"ContinueButton");

	ComponentFactory::RegisterComponent<ButtonSelector>(L"ButtonSelector");
	ComponentFactory::RegisterComponent<Tracer>(L"Tracer");
	ComponentFactory::RegisterComponent<BossFlag>(L"BossFlag");
	ComponentFactory::RegisterComponent<RailMoveCamera>(L"RailMoveCamera");
	ComponentFactory::RegisterComponent<CameraRailPoint>(L"CameraRailPoint");
	ComponentFactory::RegisterComponent<RailTriggerPoint>(L"RailTriggerPoint");

	Input::GetInstance().RegisterGameKey(VK_ESCAPE);	// ESC 키 추가, 김상준
	Input::GetInstance().RegisterGameKey(VK_LBUTTON);	// LBUTTON 키 추가, 김상준
	Input::GetInstance().RegisterGameKey(VK_UP);
	Input::GetInstance().RegisterGameKey(VK_DOWN);
	Input::GetInstance().RegisterGameKey(VK_LEFT);
	Input::GetInstance().RegisterGameKey(VK_RIGHT);
	Input::GetInstance().RegisterGameKey(VK_LSHIFT);
	Input::GetInstance().RegisterGameKey(VK_RETURN);
	Input::GetInstance().RegisterGameKey('Q');
	Input::GetInstance().RegisterGameKey('E');
	Input::GetInstance().RegisterGameKey('W');
	Input::GetInstance().RegisterGameKey('A');
	Input::GetInstance().RegisterGameKey('S');
	Input::GetInstance().RegisterGameKey('D');

	Input::GetInstance().RegisterGameKey('Z');
	Input::GetInstance().RegisterGameKey('X');
	Input::GetInstance().RegisterGameKey('C');

	Input::GetInstance().RegisterGameKey('1');
	Input::GetInstance().RegisterGameKey('2');
	Input::GetInstance().RegisterGameKey('3');
	Input::GetInstance().RegisterGameKey('4');
	Input::GetInstance().RegisterGameKey('5');
	Input::GetInstance().RegisterGameKey('6');
	Input::GetInstance().RegisterGameKey('7');
	Input::GetInstance().RegisterGameKey('8');
	Input::GetInstance().RegisterGameKey('9');

	Input::GetInstance().RegisterGameKey('0');
	Input::GetInstance().RegisterGameKey(VK_F1);
	Input::GetInstance().RegisterGameKey(VK_F2);
	Input::GetInstance().RegisterGameKey(VK_F3);
	Input::GetInstance().RegisterGameKey(VK_F4);
	Input::GetInstance().RegisterGameKey(VK_F5);
	Input::GetInstance().RegisterGameKey(VK_F6);
	Input::GetInstance().RegisterGameKey(VK_F7);
	Input::GetInstance().RegisterGameKey(VK_F8);
	Input::GetInstance().RegisterGameKey(VK_F9);
	Input::GetInstance().RegisterGameKey(VK_F10);
	Input::GetInstance().RegisterGameKey(VK_F11);

	const int BJ = 0;
	const int DH = 1;
	const int GH = 2;
	const int SC = 3;
	const int SAVE = 99;
	const int LOAD = 100;
	const int GAME = 1000;

	// CreateResource를 변경하면 SAVE모드로 한번 실행해야한다.

	int x = LOAD;
	///////////////////////////////////////////////////


	if (x == SAVE)
	{
		EngineSetting::GetInstance().SetGameName(L"BJTest");
		EngineSetting::GetInstance().SetShowDebugInfo(false);
		EngineSetting::GetInstance().SetStartScene(L"SampleScene");
		EngineSetting::GetInstance().SetResourceFile(L"BJTestResource");
		EngineSetting::GetInstance().SetCollisionSetting(true, 0, 0);
		ResourceManager::GetInstance().Clear();
		CreateResourceBJ();
		SaveResourceData(L"BJTestResource");
		SaveSetting();

		EngineSetting::GetInstance().SetGameName(L"DHTest");
		EngineSetting::GetInstance().SetShowDebugInfo(false);
		EngineSetting::GetInstance().SetStartScene(L"SampleScene");
		EngineSetting::GetInstance().SetResourceFile(L"DHTestResource");


		ResourceManager::GetInstance().Clear();
		CreateResourceDH();
		SaveResourceData(L"DHTestResource");
		SaveSetting();

		EngineSetting::GetInstance().SetGameName(L"GHTest");
		EngineSetting::GetInstance().SetShowDebugInfo(false);


		EngineSetting::GetInstance().SetStartScene(L"SampleScene");
		EngineSetting::GetInstance().SetResourceFile(L"GHTestResource");

		ResourceManager::GetInstance().Clear();
		CreateResourceGH();
		SaveResourceData(L"GHTestResource");
		SaveSetting();

		EngineSetting::GetInstance().SetGameName(L"SCTest");
		EngineSetting::GetInstance().SetShowDebugInfo(false);
		EngineSetting::GetInstance().SetStartScene(L"SampleScene");
		EngineSetting::GetInstance().SetResourceFile(L"SCTestResource");
		ResourceManager::GetInstance().Clear();
		CreateResourceSC();
		SaveResourceData(L"SCTestResource");
		SaveSetting();

		EngineSetting::GetInstance().SetGameName(L"Fate's Justice");
		EngineSetting::GetInstance().SetShowDebugInfo(false);
		EngineSetting::GetInstance().SetStartScene(L"MainScene");
		EngineSetting::GetInstance().SetResourceFile(L"Fate's Justice");

		EngineSetting::GetInstance().SetLayerName(1, L"Player");
		EngineSetting::GetInstance().SetLayerName(2, L"Boss1");
		EngineSetting::GetInstance().SetLayerName(3, L"NormalAttackObject");
		EngineSetting::GetInstance().SetLayerName(4, L"PushAttackObject");
		EngineSetting::GetInstance().SetLayerName(5, L"BasicAttack");
		EngineSetting::GetInstance().SetLayerName(6, L"RangedAttack");
		EngineSetting::GetInstance().SetLayerName(7, L"Floor");
		EngineSetting::GetInstance().SetLayerName(8, L"player2DHitCollider");
		EngineSetting::GetInstance().SetLayerName(9, L"boss2DAttackCollider");
		EngineSetting::GetInstance().SetLayerName(10, L"boss2DHitCollider");
		EngineSetting::GetInstance().SetLayerName(11, L"obstacleCollider");
		EngineSetting::GetInstance().SetLayerName(12, L"FallingFloor");
		EngineSetting::GetInstance().SetLayerName(13, L"BarrierCollider");
		EngineSetting::GetInstance().SetLayerName(14, L"DirectingSceneCollider");

		EngineSetting::GetInstance().SetLayerName(19, L"Weird");

		EngineSetting::GetInstance().SetCollisionSetting(true, 1, 3);
		EngineSetting::GetInstance().SetCollisionSetting(true, 1, 4);
		EngineSetting::GetInstance().SetCollisionSetting(true, 2, 5);
		EngineSetting::GetInstance().SetCollisionSetting(true, 2, 6);
		EngineSetting::GetInstance().SetCollisionSetting(true, 7, 1);
		EngineSetting::GetInstance().SetCollisionSetting(true, 7, 2);
		EngineSetting::GetInstance().SetCollisionSetting(true, 8, 9);
		EngineSetting::GetInstance().SetCollisionSetting(true, 9, 10);
		EngineSetting::GetInstance().SetCollisionSetting(true, 1, 11);
		EngineSetting::GetInstance().SetCollisionSetting(true, 1, 12);
		EngineSetting::GetInstance().SetCollisionSetting(true, 1, 13);
		EngineSetting::GetInstance().SetCollisionSetting(true, 5, 13);
		EngineSetting::GetInstance().SetCollisionSetting(true, 6, 13);
		EngineSetting::GetInstance().SetCollisionSetting(true, 1, 14);
		EngineSetting::GetInstance().SetCollisionSetting(true, 19, 19);

		ResourceManager::GetInstance().Clear();
		CreateResourceGame();
		SaveResourceData(L"Fate's Justice");
		SaveSetting();

		return 0;
	}

	std::wstring gamename = L"Fate's Justice";

	if (x == BJ) gamename = L"BJTest";
	if (x == DH) gamename = L"DHTest";
	if (x == GH) gamename = L"GHTest";
	if (x == SC) gamename = L"SCTest";
	if (x == GAME) gamename = L"Fate's Justice";

	LoadSetting(gamename);

	HRESULT hrCoInitializeEx = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
	assert(hrCoInitializeEx == S_OK);

	Initialize(hInstance, gamename.c_str(), nCmdShow, 1920, 1080, false, true, false, false);

	SoundManager::GetInstance().UpdateAllVolume();

	LoadResourceData(EngineSetting::GetInstance().GetResourceFile());

	if (x == BJ)
	{
		GameObject* lightObject = SceneManager::GetInstance().CreateGameObject(L"DirectionalLight");
		DirectionalLight* directionalLight = lightObject->AddComponent<DirectionalLight>();
		lightObject->GetTransform()->SetLocalRotation(Vector3D(70, 30, 0));

		// 게임 매니저
		GameObject* gameManagerObject = SceneManager::GetInstance().CreateGameObject(L"GameManager");
		GameManager* gameManager = gameManagerObject->AddComponent<GameManager>();
		{
			GameObject* coverObject = SceneManager::GetInstance().CreateGameObject(L"CoverObject");
			coverObject->AddComponent<ZeldaUI>();
			Image* coverImage = coverObject->AddComponent<Image>();
			coverImage->SetLayer(901);
			coverObject->GetTransform()->SetLocalScale(Vector3D(1920, 1080));
			SpriteAnimator* mainanimator = coverObject->AddComponent<SpriteAnimator>();
			gameManager->SetMainAnimator(mainanimator);
		}

		{
			GameObject* coverObject = SceneManager::GetInstance().CreateGameObject(L"CoverObject");
			coverObject->AddComponent<ZeldaUI>();
			Image* coverImage = coverObject->AddComponent<Image>();
			coverImage->SetLayer(900);
			coverObject->GetTransform()->SetLocalScale(Vector3D(1920, 1080));
			SpriteAnimator* subanimator = coverObject->AddComponent<SpriteAnimator>();
			gameManager->SetSubAnimator(subanimator);
		}

		{
			GameObject* coverObject = SceneManager::GetInstance().CreateGameObject(L"CoverObject");
			coverObject->AddComponent<ZeldaUI>();
			Image* coverImage = coverObject->AddComponent<Image>();
			coverImage->SetLayer(899);
			coverObject->GetTransform()->SetLocalScale(Vector3D(1920, 1080));
			SpriteAnimator* thirdanimator = coverObject->AddComponent<SpriteAnimator>();
			gameManager->SetThirdAnimator(thirdanimator);
		}

		GameObject* phase3EventManagerObject = SceneManager::GetInstance().CreateGameObject(L"Phase3EventManager");
		Phase3EventManager* phase3EventManager = phase3EventManagerObject->AddComponent<Phase3EventManager>();

		// 카메라에 이동을 위한 컴포넌트 붙이기
		TestComponent* testcomponent = SceneManager::GetInstance().GetMainCamera()->GetGameObject()->AddComponent<TestComponent>();

		// 바닥
		{
			GameObject* collsionTestObject002 = SceneManager::GetInstance().CreateGameObject(L"collsionTestObject002");
			CubeCollider* collider2 = collsionTestObject002->AddComponent<CubeCollider>();
			Rigidbody* rigid2 = collsionTestObject002->AddComponent<Rigidbody>();
			rigid2->SetKinematic(true);
			rigid2->SetMass(0.0f);
			MeshRenderer* mesh2 = collsionTestObject002->AddComponent<MeshRenderer>();
			mesh2->SetMesh(L"Cube");
			mesh2->SetMaterial(L"BoundaryDebug");

			collsionTestObject002->GetTransform()->SetLocalScale(Vector3D(500, 1, 500));
			collsionTestObject002->GetTransform()->SetLocalPosition(Vector3D(-25, 0, 150));
		}

		for (int i = 0; i < 1; i++)
		{
			GameObject* obj = SceneManager::GetInstance().CreateGameObject(L"obj");
			obj->AddComponent<TestComponent>();
			TestComponent2* testcom2 = obj->AddComponent<TestComponent2>();
			testcom2->SetLimitLine(-500.0f);

			Collider2D* collider2d2 = obj->AddComponent<Collider2D>();
			collider2d2->SetShape(Collider2D::Box);

			Rigidbody* rigid = obj->AddComponent<Rigidbody>();
			rigid->SetMass(1.0f);

			CylinderCollider* collider = obj->AddComponent<CylinderCollider>();

			MeshRenderer* mesh = obj->AddComponent<MeshRenderer>();
			mesh->SetMesh(L"Cylinder");
			mesh->SetMaterial(L"BoundaryDebug");

			obj->GetTransform()->SetLocalScale(Vector3D(5, 5, 5));
			obj->GetTransform()->SetLocalRotation(Vector3D(180, 0, 0));
			obj->GetTransform()->SetLocalPosition(Vector3D(0, 65 + i * 50, 150));
		}

		SaveScene();
	}
	else if (x == DH)
	{
#pragma region Player
		Vector3D diffHeight = Vector3D(0, -1.5, 0);
		GameObject* testPlayer = SceneManager::GetInstance().CreateGameObject(L"testPlayer");
		GameObject* playerHitOjbect = SceneManager::GetInstance().CreateGameObject(L"playerHitOjbect");
		GameObject* playerAnimator = CreateGameObjectFromFBX(L"Player");
		testPlayer->GetTransform()->AddChild(playerHitOjbect->GetTransform());
		testPlayer->GetTransform()->AddChild(playerAnimator->GetTransform());
		playerAnimator->GetTransform()->SetLocalPosition(diffHeight);

		Animator* playeranim = playerAnimator->AddComponent<Animator>();
		FF::FFAnimationClip& playerrunClip = GetAnimationClip(L"Armature|running");
		playerrunClip.isLoop = true;

		FF::FFAnimationClip& playeridleClip = GetAnimationClip(L"Armature|idle");
		playeridleClip.isLoop = true;

		playeranim->PlusAnimationClip(GetAnimationClip(L"Armature|attack"));
		playeranim->PlusAnimationClip(GetAnimationClip(L"Armature|attack2"));
		playeranim->PlusAnimationClip(GetAnimationClip(L"Armature|damage"));
		playeranim->PlusAnimationClip(GetAnimationClip(L"Armature|dash"));
		playeranim->PlusAnimationClip(GetAnimationClip(L"Armature|down"));
		playeranim->PlusAnimationClip(GetAnimationClip(L"Armature|idle"));
		playeranim->PlusAnimationClip(GetAnimationClip(L"Armature|running"));

		playeranim->Play(L"Armature|idle");

		CapsuleCollider* playerHitCollider = playerHitOjbect->AddComponent<CapsuleCollider>();
		playerHitCollider->SetTrigger(true);
		//playerHitOjbect->GetTransform()->SetLocalPosition(Vector3D(10, 10, 11));

		SkinnedMeshRenderer* playerSkinnedMeshRenderer = playerAnimator->GetTransform()->GetChild(0)->GetGameObject()->GetComponent<SkinnedMeshRenderer>();

		Player* playerComponent = testPlayer->AddComponent<Player>();
		playerComponent->SetAnimator(playeranim);
		playerComponent->SetSkinnedMeshRenderer(playerSkinnedMeshRenderer);
		CapsuleCollider* playerCollider = testPlayer->AddComponent<CapsuleCollider>();
		//playerCollider->SetTrigger(true);
		Rigidbody* playerRigidBody = testPlayer->AddComponent<Rigidbody>();
		playerRigidBody->SetFreezeRotationX(true);
		playerRigidBody->SetFreezeRotationY(true);
		playerRigidBody->SetFreezeRotationZ(true);
		//playerRigidBody->SetKinematic(true);
		playerComponent->SetRigidBody(playerRigidBody);
		testPlayer->SetLayerName(L"Player");
		playerHitOjbect->SetLayerName(L"Player");
#pragma endregion

		SceneManager::GetInstance().GetMainCamera()->GetGameObject()->AddComponent<TestComponent>();
		SceneManager::GetInstance().GetMainCamera()->GetGameObject()->GetTransform()->SetLocalPosition(Vector3D(0, 180, 207));
		SceneManager::GetInstance().GetMainCamera()->GetGameObject()->GetTransform()->SetLocalRotation(Vector3D(40, 180, 0));

		GameObject* lightObject = SceneManager::GetInstance().CreateGameObject(L"GameObject004");
		DirectionalLight* directionalLight = lightObject->AddComponent<DirectionalLight>();
		lightObject->GetTransform()->SetLocalPosition(Vector3D(1000, 30, 0));
		lightObject->GetTransform()->SetLocalRotation(Vector3D(110, 25, 0));

		GameObject* boss1 = CreateGameObjectFromFBX(L"Boss");
		GameObject* boss1HitObject = SceneManager::GetInstance().CreateGameObject(L"Boss1HitObject");
		boss1->GetTransform()->AddChild(boss1HitObject->GetTransform());
		/// 나동
		boss1HitObject->SetLayerName(L"Boss1");
		boss1->SetLayerName(L"Boss1");

		//----------------------------------------------------------------------Animation----------------------------------------------------------------------
		Animator* anim = boss1->AddComponent<Animator>();
		FF::FFAnimationClip& idleClip = GetAnimationClip(L"root|000.Idle");
		idleClip.isLoop = true;

		FF::FFAnimationClip& chargeAttackClip = GetAnimationClip(L"root|007-2.Skill_ChargeAttack");
		chargeAttackClip.isLoop = true;

		FF::FFAnimationClip& walkClip = GetAnimationClip(L"root|001-2.Walk");
		walkClip.isLoop = true;

		anim->PlusAnimationClip(GetAnimationClip(L"root|000.Idle"));
		anim->PlusAnimationClip(GetAnimationClip(L"root|001-1.Idle->Walk"));
		anim->PlusAnimationClip(GetAnimationClip(L"root|001-2.Walk"));

		anim->PlusAnimationClip(GetAnimationClip(L"root|001-3.Walk->Idle"));

		anim->PlusAnimationClip(GetAnimationClip(L"root|002-1.NormalAttack_R"));
		anim->PlusAnimationClip(GetAnimationClip(L"root|002-2.NormalAttack_R->Idle"));

		anim->PlusAnimationClip(GetAnimationClip(L"root|003-1.NormalAttack_L"));
		anim->PlusAnimationClip(GetAnimationClip(L"root|003-2.NormalAttack_L->Idle"));

		anim->PlusAnimationClip(GetAnimationClip(L"root|004.RangedAttack"));

		anim->PlusAnimationClip(GetAnimationClip(L"root|005.Teleport"));

		anim->PlusAnimationClip(GetAnimationClip(L"root|006-1.Skill_Tree"));
		anim->PlusAnimationClip(GetAnimationClip(L"root|006-2.Skill_Tree->Idle"));

		anim->PlusAnimationClip(GetAnimationClip(L"root|007-1.Idle->Skill_ChargeAttack"));
		anim->PlusAnimationClip(GetAnimationClip(L"root|007-2.Skill_ChargeAttack"));

		anim->PlusAnimationClip(GetAnimationClip(L"root|007-3.Skill_ChargeAttack->Idle"));

		anim->PlusAnimationClip(GetAnimationClip(L"root|008.Skill_Knockback"));

		anim->PlusAnimationClip(GetAnimationClip(L"root|009.Skill_Blocking"));

		anim->PlusAnimationClip(GetAnimationClip(L"root|010.Skill_2D"));

		anim->PlusAnimationClip(GetAnimationClip(L"root|011-1.Groggy"));
		anim->PlusAnimationClip(GetAnimationClip(L"root|011-2.Groogy->Idle"));

		anim->PlusAnimationClip(GetAnimationClip(L"root|012.Death"));

		anim->Play(L"root|000.Idle");

		CapsuleCollider* boss1HitCollider = boss1HitObject->AddComponent<CapsuleCollider>();
		boss1HitCollider->SetTrigger(true);

		CapsuleCollider* boss1Collider = boss1->AddComponent<CapsuleCollider>();
		//boss1Collider->SetTrigger(true);

		// Boss1 Test
		Boss1* boss1Component = boss1->AddComponent<Boss1>();
		boss1Component->SetPlayerTransform(testPlayer->GetTransform());
		boss1Component->SetPlayerCollider(playerHitCollider);
		boss1Component->SetPlayerGameObject(testPlayer);

		testPlayer->GetTransform()->SetLocalPosition(Vector3D(10, 10, 11));

		boss1->GetTransform()->SetLocalPosition(Vector3D(0, 4.0f, 1));
		boss1->GetTransform()->Rotate(boss1->GetTransform()->GetUp(), DegreeToRadian(180));

		/// 나동
		testPlayer->GetComponent<Player>()->SetBossCollider(boss1HitCollider);
		testPlayer->GetComponent<Player>()->SetBossTransform(boss1HitObject->GetTransform());
		testPlayer->GetComponent<Player>()->SetPlayerAnimTransform(playerAnimator->GetTransform());
		testPlayer->GetComponent<Player>()->SetBossAnimTransform(boss1->GetTransform());
		//cam1->GetTransform()->SetParent(boss1->GetTransform());

#pragma region floor
		/// BossFloor
		GameObject* BossFloor = CreateGameObjectFromFBX(L"BossFloor");
		GameObject* BossFloorMesh = BossFloor->GetTransform()->GetChild(0)->GetGameObject();
		MeshRenderer* renderer = BossFloorMesh->GetComponent<MeshRenderer>();
		renderer->GetMaterialInfo(1).shader = L"..\\Resources\\Shader\\UI.hlsli";

		GameObject* child = SceneManager::GetInstance().CreateGameObject(L"child");
		child->SetLayerName(L"Floor");
		BossFloor->GetTransform()->AddChild(child->GetTransform());
		child->GetTransform()->SetLocalScale(Vector3D(50, 1, 50));
		Rigidbody* rigid = child->AddComponent<Rigidbody>();
		CylinderCollider* collider = child->AddComponent<CylinderCollider>();
		rigid->SetKinematic(true);
		rigid->SetRestitution(0.0f);
		rigid->SetKfriction(0.9f);
		rigid->SetSfriction(0.95f);
#pragma endregion

#pragma region door
		GameObject* door = CreateGameObjectFromFBX(L"BossDoor");
		door->GetTransform()->SetLocalPosition(Vector3D(0, 0, -25));

		GameObject* hand = CreateGameObjectFromFBX(L"BossDoorHand");

		hand->GetTransform()->SetParent(door->GetTransform());
		Animator* animator = hand->AddComponent<Animator>();
		FF::FFAnimationClip& clip = GetAnimationClip(L"Hand_rig|Idle");
		clip.isLoop = true;
		animator->PlusAnimationClip(GetAnimationClip(L"Hand_rig|Idle"));
		animator->Play(L"Hand_rig|Idle");
#pragma endregion

		SaveScene();
	}
	else if (x == GH)
	{

		// 게임 매니저
		GameObject* gameManagerObject = SceneManager::GetInstance().CreateGameObject(L"GameManager");
		GameManager* gameManager = gameManagerObject->AddComponent<GameManager>();

		{
			GameObject* coverObject = SceneManager::GetInstance().CreateGameObject(L"CoverObject");
			coverObject->AddComponent<ZeldaUI>();
			Image* coverImage = coverObject->AddComponent<Image>();
			coverImage->SetLayer(901);
			coverObject->GetTransform()->SetLocalScale(Vector3D(1920, 1080));
			SpriteAnimator* mainanimator = coverObject->AddComponent<SpriteAnimator>();
			gameManager->SetMainAnimator(mainanimator);
		}

		{
			GameObject* coverObject = SceneManager::GetInstance().CreateGameObject(L"CoverObject");
			coverObject->AddComponent<ZeldaUI>();
			Image* coverImage = coverObject->AddComponent<Image>();
			coverImage->SetLayer(900);
			coverObject->GetTransform()->SetLocalScale(Vector3D(1920, 1080));
			SpriteAnimator* subanimator = coverObject->AddComponent<SpriteAnimator>();
			gameManager->SetSubAnimator(subanimator);
		}

		{
			GameObject* coverObject = SceneManager::GetInstance().CreateGameObject(L"CoverObject");
			coverObject->AddComponent<ZeldaUI>();
			Image* coverImage = coverObject->AddComponent<Image>();
			coverImage->SetLayer(899);
			coverObject->GetTransform()->SetLocalScale(Vector3D(1920, 1080));
			SpriteAnimator* thirdanimator = coverObject->AddComponent<SpriteAnimator>();
			gameManager->SetThirdAnimator(thirdanimator);
		}

#pragma region Player
		Vector3D diffHeight = Vector3D(0, -1.5, 0);
		GameObject* testPlayer = SceneManager::GetInstance().CreateGameObject(L"testPlayer");
		GameObject* playerHitOjbect = SceneManager::GetInstance().CreateGameObject(L"playerHitOjbect");
		GameObject* player2DHitOjbect = SceneManager::GetInstance().CreateGameObject(L"player2DHitOjbect");
		GameObject* playerAnimator = CreateGameObjectFromFBX(L"Player");
		testPlayer->GetTransform()->AddChild(playerHitOjbect->GetTransform());
		testPlayer->GetTransform()->AddChild(player2DHitOjbect->GetTransform());
		testPlayer->GetTransform()->AddChild(playerAnimator->GetTransform());
		playerAnimator->GetTransform()->SetLocalPosition(diffHeight);

		Animator* playeranim = playerAnimator->AddComponent<Animator>();
		FF::FFAnimationClip& playerrunClip = GetAnimationClip(L"Armature|running");
		playerrunClip.isLoop = true;

		FF::FFAnimationClip& playeridleClip = GetAnimationClip(L"Armature|idle");
		playeridleClip.isLoop = true;


		playeranim->PlusAnimationClip(GetAnimationClip(L"Armature|attack"));
		playeranim->PlusAnimationClip(GetAnimationClip(L"Armature|attack2"));
		playeranim->PlusAnimationClip(GetAnimationClip(L"Armature|damage"));
		playeranim->PlusAnimationClip(GetAnimationClip(L"Armature|dash"));
		playeranim->PlusAnimationClip(GetAnimationClip(L"Armature|down"));
		playeranim->PlusAnimationClip(GetAnimationClip(L"Armature|idle"));
		playeranim->PlusAnimationClip(GetAnimationClip(L"Armature|running"));


		playeranim->Play(L"Armature|idle");

		CapsuleCollider* playerHitCollider = playerHitOjbect->AddComponent<CapsuleCollider>();
		playerHitCollider->SetTrigger(true);

		//playerHitOjbect->GetTransform()->SetLocalPosition(Vector3D(10, 10, 11));

		SkinnedMeshRenderer* playerSkinnedMeshRenderer = playerAnimator->GetTransform()->GetChild(0)->GetGameObject()->GetComponent<SkinnedMeshRenderer>();

		Player* playerComponent = testPlayer->AddComponent<Player>();
		playerComponent->SetAnimator(playeranim);
		playerComponent->SetSkinnedMeshRenderer(playerSkinnedMeshRenderer);
		CapsuleCollider* playerCollider = testPlayer->AddComponent<CapsuleCollider>();

		Collider2D* player2DHitCollider = player2DHitOjbect->AddComponent<Collider2D>();
		player2DHitCollider->SetShape(Collider2D::Box);

		EdgeDetect* playerEdge = player2DHitOjbect->AddComponent<EdgeDetect>();
		playerEdge->SetMeshRenderer(playerAnimator->GetTransform()->GetChild(0)->GetGameObject()->GetComponent<SkinnedMeshRenderer>());

		Player2DCollisionManager* Collision2DManagerComponent = player2DHitOjbect->AddComponent<Player2DCollisionManager>();
		Collision2DManagerComponent->SetPlayerComponent(playerComponent);

		//playerCollider->SetTrigger(true);
		Rigidbody* playerRigidBody = testPlayer->AddComponent<Rigidbody>();
		playerRigidBody->SetFreezeRotationX(true);
		playerRigidBody->SetFreezeRotationY(true);
		playerRigidBody->SetFreezeRotationZ(true);
		//playerRigidBody->SetKinematic(true);
		playerComponent->SetRigidBody(playerRigidBody);
		testPlayer->SetLayerName(L"Player");
		playerHitOjbect->SetLayerName(L"Player");
		player2DHitOjbect->SetLayerName(L"player2DHitCollider");

		GameObject* PlayerDamaged = SceneManager::GetInstance().CreateGameObject(L"PlayerDamaged");
		PlayerDamaged->AddComponent<ZeldaUI>();
		PlayerDamaged->AddComponent<Image>();
		SpriteAnimator* PlayerSprite = PlayerDamaged->AddComponent<SpriteAnimator>();
		PlayerSprite->SetTexture(
			{
				L"PlayerDamaged_4",
				L"PlayerDamaged_3",
				L"PlayerDamaged_2",
				L"PlayerDamaged_1",
				L"PlayerDamaged_0",
			});
		PlayerSprite->SetPlaySpeed(30);
		PlayerSprite->SetClearAfterPlayEnds(true);
		playerComponent->SetPlayerDamagedSpriteAnimator(PlayerSprite);
		PlayerDamaged->GetTransform()->SetLocalScale(Vector3D(128, 128, 128));


		GameObject* VFX_PlayerAttack = CreateGameObjectFromFBX(L"VFX_PlayerAttack");
		VFX_PlayerAttack->GetTransform()->SetLocalPosition(Vector3D(0, 0, 0));
		VFX_PlayerAttack->GetTransform()->SetParent(testPlayer->GetTransform());
		playerComponent->SetPlayerAttackEffect(VFX_PlayerAttack);
		VFX_PlayerAttack->SetActive(false);
		EffectPlayer* playerAttackEffect = VFX_PlayerAttack->AddComponent<EffectPlayer>();
		playerAttackEffect->SetMeshRenderer(VFX_PlayerAttack->GetTransform()->GetChild(0)->GetGameObject()->GetComponent<MeshRenderer>());
		playerAttackEffect->SetMaxValue(1.f);
		playerAttackEffect->SetPlaySpeed(1.5f);
		playerAttackEffect->SetRemainTime(0.3f);
		playerAttackEffect->SetDirection(Vector2D(1, 0));

		// 특수공격 이펙트
		GameObject* VFX_PlayerSpecialAttack = CreateGameObjectFromFBX(L"VFX_PlayerSpecialAttack");
		VFX_PlayerSpecialAttack->GetTransform()->SetLocalPosition(Vector3D(0, 0, 0));
		VFX_PlayerSpecialAttack->GetTransform()->SetParent(testPlayer->GetTransform());
		playerComponent->SetPlayerSpecialAttackEffect(VFX_PlayerSpecialAttack);
		VFX_PlayerSpecialAttack->SetActive(false);
		EffectPlayer* playerSpecialAttackEffect = VFX_PlayerSpecialAttack->AddComponent<EffectPlayer>();
		playerSpecialAttackEffect->SetMeshRenderer(VFX_PlayerSpecialAttack->GetTransform()->GetChild(0)->GetGameObject()->GetComponent<MeshRenderer>());
		playerSpecialAttackEffect->SetMaxValue(1.f);
		playerSpecialAttackEffect->SetPlaySpeed(1.5f);
		playerSpecialAttackEffect->SetRemainTime(0.3f);
		playerSpecialAttackEffect->SetDirection(Vector2D(1, 0));

#pragma endregion

		GameObject* cameraobject = SceneManager::GetInstance().GetMainCamera()->GetGameObject();
		FollowCamera* followCamera = cameraobject->AddComponent<FollowCamera>();
		followCamera->SetPlayerTransform(testPlayer->GetTransform());
		
		GameObject* lightObject = SceneManager::GetInstance().CreateGameObject(L"GameObject004");
		DirectionalLight* directionalLight = lightObject->AddComponent<DirectionalLight>();
		lightObject->GetTransform()->SetLocalPosition(Vector3D(1000, 30, 0));
		lightObject->GetTransform()->SetLocalRotation(Vector3D(110, 25, 0));

		//GameObject* collsionTestObject001 = CreateGameObjectFromFBX(L"Mole");
		GameObject* boss1 = CreateGameObjectFromFBX(L"Boss");
		GameObject* boss1HitObject = SceneManager::GetInstance().CreateGameObject(L"Boss1HitObject");
		boss1->GetTransform()->AddChild(boss1HitObject->GetTransform());

		GameObject* boss2DHitOjbect = SceneManager::GetInstance().CreateGameObject(L"boss2DHitOjbect");
		boss2DHitOjbect->GetTransform()->SetParent(boss1->GetTransform());
		Collider2D* boss2DHitCollider = boss2DHitOjbect->AddComponent<Collider2D>();
		boss2DHitCollider->SetShape(Collider2D::Box);
		boss2DHitOjbect->SetLayerName(L"boss2DHitCollider");

		EdgeDetect* bossEdge = boss2DHitOjbect->AddComponent<EdgeDetect>();
		bossEdge->SetMeshRenderer(boss1->GetTransform()->GetChild(0)->GetGameObject()->GetComponent<SkinnedMeshRenderer>());

		CapsuleCollider* boss1HitCollider = boss1HitObject->AddComponent<CapsuleCollider>();
		boss1HitCollider->SetTrigger(true);

		Boss1* boss1Component = boss1->AddComponent<Boss1>();
		boss1Component->SetGameManager(gameManager);
		boss1Component->SetPlayerTransform(testPlayer->GetTransform());
		boss1Component->SetPlayerCollider(playerHitCollider);
		boss1Component->SetBossHitCollider(boss1HitCollider);
		boss1Component->SetPlayerGameObject(testPlayer);
		boss1Component->SetPlayerComponent(playerComponent);

		GameObject* boss1HitEffect = SceneManager::GetInstance().CreateGameObject(L"boss1HitEffect");
		boss1HitEffect->AddComponent<ZeldaUI>();
		boss1HitEffect->AddComponent<Image>();
		SpriteAnimator* bossHitSpriteAnimator = boss1HitEffect->AddComponent<SpriteAnimator>();
		bossHitSpriteAnimator->SetTexture({
			L"boss_damage_0",
			L"boss_damage_1",
			L"boss_damage_2",
			L"boss_damage_3",
			L"boss_damage_4",
			L"boss_damage_5",
			L"boss_damage_6"
			});
		bossHitSpriteAnimator->SetPlaySpeed(30.0f);
		bossHitSpriteAnimator->SetClearAfterPlayEnds(true);
		boss1Component->SetBossHitSpriteAnimator(bossHitSpriteAnimator);
		boss1HitEffect->GetTransform()->SetLocalScale(Vector3D(128, 128, 128));


		/// 나동
		boss1HitObject->SetLayerName(L"Boss1");
		boss1->SetLayerName(L"Boss1");

		//----------------------------------------------------------------------Animation----------------------------------------------------------------------
		Animator* anim = boss1->AddComponent<Animator>();
		FF::FFAnimationClip& idleClip = GetAnimationClip(L"root|000.Idle");
		idleClip.isLoop = true;

		FF::FFAnimationClip& chargeAttackClip = GetAnimationClip(L"root|007-2.Skill_ChargeAttack");
		chargeAttackClip.isLoop = true;

		FF::FFAnimationClip& walkClip = GetAnimationClip(L"root|001-2.Walk");
		walkClip.isLoop = true;

		anim->PlusAnimationClip(GetAnimationClip(L"root|000.Idle"));
		anim->PlusAnimationClip(GetAnimationClip(L"root|001-1.Idle->Walk"));
		anim->PlusAnimationClip(GetAnimationClip(L"root|001-2.Walk"));

		anim->PlusAnimationClip(GetAnimationClip(L"root|001-3.Walk->Idle"));

		anim->PlusAnimationClip(GetAnimationClip(L"root|002-1.NormalAttack_R"));
		anim->PlusAnimationClip(GetAnimationClip(L"root|002-2.NormalAttack_R->Idle"));

		anim->PlusAnimationClip(GetAnimationClip(L"root|003-1.NormalAttack_L"));
		anim->PlusAnimationClip(GetAnimationClip(L"root|003-2.NormalAttack_L->Idle"));

		anim->PlusAnimationClip(GetAnimationClip(L"root|004.RangedAttack"));

		anim->PlusAnimationClip(GetAnimationClip(L"root|005.Teleport"));

		anim->PlusAnimationClip(GetAnimationClip(L"root|006-1.Skill_Tree"));
		anim->PlusAnimationClip(GetAnimationClip(L"root|006-2.Skill_Tree->Idle"));

		anim->PlusAnimationClip(GetAnimationClip(L"root|007-1.Idle->Skill_ChargeAttack"));
		anim->PlusAnimationClip(GetAnimationClip(L"root|007-2.Skill_ChargeAttack"));

		anim->PlusAnimationClip(GetAnimationClip(L"root|007-3.Skill_ChargeAttack->Idle"));

		anim->PlusAnimationClip(GetAnimationClip(L"root|008.Skill_Knockback"));

		anim->PlusAnimationClip(GetAnimationClip(L"root|009.Skill_Blocking"));

		anim->PlusAnimationClip(GetAnimationClip(L"root|010.Skill_2D"));

		anim->PlusAnimationClip(GetAnimationClip(L"root|011-1.Groggy"));
		anim->PlusAnimationClip(GetAnimationClip(L"root|011-2.Groogy->Idle"));

		anim->PlusAnimationClip(GetAnimationClip(L"root|012.Death"));

		anim->Play(L"root|000.Idle");

		CapsuleCollider* boss1Collider = boss1->AddComponent<CapsuleCollider>();
		//boss1Collider->SetTrigger(true);


		//testPlayer->GetTransform()->SetLocalPosition(Vector3D(10, 10, 11));

		boss1->GetTransform()->SetLocalPosition(Vector3D(0, 4.0f, 1));
		boss1->GetTransform()->Rotate(boss1->GetTransform()->GetUp(), DegreeToRadian(180));

		/// 나동
		testPlayer->GetComponent<Player>()->SetBossCollider(boss1HitCollider);
		testPlayer->GetComponent<Player>()->SetBossTransform(boss1HitObject->GetTransform());
		testPlayer->GetComponent<Player>()->SetPlayerAnimTransform(playerAnimator->GetTransform());
		testPlayer->GetComponent<Player>()->SetBossAnimTransform(boss1->GetTransform());
		testPlayer->GetComponent<Player>()->SetBossComponent(boss1Component);
		//cam1->GetTransform()->SetParent(boss1->GetTransform());

		// 족자 생성
		for (int i = 0; i < 8; i++)
		{
			GameObject* scrollObject = CreateGameObjectFromFBX(L"Scroll");
			FloatingObject* floatingComponent = scrollObject->AddComponent<FloatingObject>();
			floatingComponent->SetObjectTransform(scrollObject->GetTransform());
			scrollObject->GetTransform()->SetLocalPosition(Vector3D(-40 + i * 10, 0, -25));
			boss1Component->SetScrollComponent(floatingComponent);
		}

		// 깃발 생성
		for (int i = 0; i < 6; i++)
		{
			GameObject* bossFlag = CreateGameObjectFromFBX(L"BossFlag");
			MeshRenderer* flagRenderer = bossFlag->GetTransform()->GetChild(0)->GetGameObject()->GetComponent<MeshRenderer>();
			flagRenderer->GetMaterialInfo(0).texture[1] = L"..\\Resources\\Texture\\BossFlag\\delete.jpg";
			flagRenderer->GetMaterialInfo(0).texture[2] = L"..\\Resources\\Texture\\BossFlag\\T_Flag_Diffuse_002.png";
			flagRenderer->GetMaterialInfo(0).color = ZeldaColor::White;

			bossFlag->GetTransform()->SetLocalPosition(Vector3D(i * 5, 0, 0));


			BossFlag* flagComponent = bossFlag->AddComponent<BossFlag>();
			flagComponent->SetMeshRenderer(flagRenderer);
			flagComponent->SetPlaySpeed(0.000002);

			boss1Component->SetBossFlagComponent(flagComponent);
		}

		// followCamera 컴포넌트 등록
		boss1Component->SetFollowCameraComponent(followCamera);


#pragma region floor
		/// BossFloor
		GameObject* BossFloor = CreateGameObjectFromFBX(L"BossFloor");
		GameObject* BossFloorMesh = BossFloor->GetTransform()->GetChild(0)->GetGameObject();
		MeshRenderer* renderer = BossFloorMesh->GetComponent<MeshRenderer>();
		renderer->GetMaterialInfo(1).shader = L"..\\Resources\\Shader\\BossFloor.hlsli";

		GameObject* child = SceneManager::GetInstance().CreateGameObject(L"child");
		child->SetLayerName(L"Floor");
		BossFloor->GetTransform()->AddChild(child->GetTransform());
		child->GetTransform()->SetLocalScale(Vector3D(50, 1, 50));
		Rigidbody* rigid = child->AddComponent<Rigidbody>();
		CylinderCollider* collider = child->AddComponent<CylinderCollider>();
		rigid->SetKinematic(true);
		rigid->SetRestitution(0.0f);
		rigid->SetKfriction(0.9f);
		rigid->SetSfriction(0.95f);
#pragma endregion

#pragma region door
		GameObject* door = CreateGameObjectFromFBX(L"BossDoor");
		door->GetTransform()->SetLocalPosition(Vector3D(0, 0, -25));

		GameObject* hand = CreateGameObjectFromFBX(L"BossDoorHand");

		hand->GetTransform()->SetParent(door->GetTransform());
		Animator* animator = hand->AddComponent<Animator>();
		FF::FFAnimationClip& clip = GetAnimationClip(L"Hand_rig|Idle");
		clip.isLoop = true;
		animator->PlusAnimationClip(GetAnimationClip(L"Hand_rig|Idle"));
		animator->Play(L"Hand_rig|Idle");
#pragma endregion

		//// Dotween Test
		//Dotween* dotween1 = boss1->AddComponent<Dotween>();
		//
		//dotween1->DOMove(Vector3D(50, 50, 50), 10.0f).SetDelay(5.0f).SetEase(EaseInOutElastic)
		//	.OnComplete([=]()
		//		{
		//			dotween1->DORotate(Vector3D(0,180,0), 10.0f).SetEase(EaseInSine);
		//		});
		//dotween->DOQRotate(collsionTestObject001, Vector3D::up, 10.0f, 180.0f).SetEase(EaseInOutElastic);
		//dotween->DOMove(collsionTestObject001, Vector3D(1, 0, 0), 3.0f,
		//	[=]() { dotween->DORotate(collsionTestObject001, Vector3D(100, 100, 100), 10.0f).SetDelay(5.0f).SetEase(EaseInSine);
		//	}).SetDelay(5.0f).SetEase(EaseInSine);
		//dotween1->DOLookAt(collsionTestObject002->GetTransform()->GetLocalPosition()).SetDelay(5.0f);
		//dotween->DORotate(collsionTestObject001, Vector3D(-90, 0, 0), 3.0f).SetDelay(3.0f).SetEase(EaseInSine)
		//	.OnComplete([=]()
		//	{
		//			dotween->DOLookAt(collsionTestObject001, Vector3D::zero);
		//	});

		SaveScene();
	}
	else if (x == SC)
	{
		// 카메라에 이동을 위한 컴포넌트 붙이기
		SceneManager::GetInstance().GetMainCamera()->GetGameObject()->AddComponent<TestComponent>();
		SceneManager::GetInstance().GetMainCamera()->GetGameObject()->GetTransform()->SetLocalPosition(Vector3D(0, 180, -207));
		SceneManager::GetInstance().GetMainCamera()->GetGameObject()->GetTransform()->SetLocalRotation(Vector3D(40, 0, 0));

		GameObject* lightObject = SceneManager::GetInstance().CreateGameObject(L"GameObject004");
		DirectionalLight* directionalLight = lightObject->AddComponent<DirectionalLight>();
		lightObject->GetTransform()->SetLocalPosition(Vector3D(1000, 30, 0));
		lightObject->GetTransform()->SetLocalRotation(Vector3D(0, 20, 0));

		for (int i = 0; i < 1; ++i)
		{
			//GameObject* Effect = CreateGameObjectFromFBX(L"VFX");
			//Effect->GetTransform()->SetLocalPosition(Vector3D(0, 0, 0));
			//Effect->GetTransform()->SetLocalScale(Vector3D(1, 1, 1));
			//
			//EffectPlayer* effectPlayer = Effect->AddComponent<EffectPlayer>();
			//effectPlayer->SetMeshRenderer(Effect->GetTransform()->GetChild(0)->GetGameObject()->GetComponent<MeshRenderer>());
			//effectPlayer->SetPlaySpeed(1.f);
			//effectPlayer->SetMaxValue(1.f);
			//effectPlayer->SetRemainTime(4.f);
			//effectPlayer->SetDirection(Vector2D(1, 0));
		}

		{
			//GameObject* bossFlag = CreateGameObjectFromFBX(L"BossFlag");
			//bossFlag->GetTransform()->SetLocalRotation(Vector3D(0, 180, 0));
			//MeshRenderer* renderer = bossFlag->GetTransform()->GetChild(0)->GetGameObject()->GetComponent<MeshRenderer>();
			//renderer->GetMaterialInfo(0).texture[1] = L"..\\Resources\\Texture\\BossFlag\\delete.jpg";
			//renderer->GetMaterialInfo(0).texture[2] = L"..\\Resources\\Texture\\BossFlag\\T_Flag_Diffuse_002.png";
			//renderer->GetMaterialInfo(0).color = ZeldaColor::White;
			//
			//BossFlag* flagComponent = bossFlag->AddComponent<BossFlag>();
			//flagComponent->SetMeshRenderer(renderer);
			//flagComponent->SetPlaySpeed(0.000002);
		}

		{
			GameObject* BossFloor = CreateGameObjectFromFBX(L"BossFloor");
			GameObject* BossFloorMesh = BossFloor->GetTransform()->GetChild(0)->GetGameObject();
			MeshRenderer* renderer = BossFloorMesh->GetComponent<MeshRenderer>();
			renderer->GetMaterialInfo(1).shader = L"..\\Resources\\Shader\\UI.hlsli";

			GameObject* child = SceneManager::GetInstance().CreateGameObject(L"child");
			BossFloor->GetTransform()->AddChild(child->GetTransform());
			child->GetTransform()->SetLocalScale(Vector3D(50, 1, 50));
			Rigidbody* rigid = child->AddComponent<Rigidbody>();
			CubeCollider* collider = child->AddComponent<CubeCollider>();
			rigid->SetKinematic(true);
			rigid->SetRestitution(0.0f);
		}


		//for (int i = 0; i < 1; ++i)
		//{
		//	GameObject* player = CreateGameObjectFromFBX(L"Player");
		//	player->GetTransform()->SetLocalPosition(Vector3D(0, 4, 0));
		//
		//	Animator* anim = player->AddComponent<Animator>();
		//	FF::FFAnimationClip& runClip = GetAnimationClip(L"Armature|running");
		//	runClip.isLoop = true;
		//
		//	FF::FFAnimationClip& idleClip = GetAnimationClip(L"Armature|idle");
		//	idleClip.isLoop = true;
		//
		//
		//	anim->PlusAnimationClip(GetAnimationClip(L"Armature|attack"));
		//	anim->PlusAnimationClip(GetAnimationClip(L"Armature|attack2"));
		//	anim->PlusAnimationClip(GetAnimationClip(L"Armature|damage"));
		//	anim->PlusAnimationClip(GetAnimationClip(L"Armature|dash"));
		//	anim->PlusAnimationClip(GetAnimationClip(L"Armature|down"));
		//	anim->PlusAnimationClip(GetAnimationClip(L"Armature|idle"));
		//	anim->PlusAnimationClip(GetAnimationClip(L"Armature|running"));
		//
		//
		//	anim->Play(L"Armature|idle");
		//}


		//for (int i = 0; i < 1; ++i)
		//{
		//	GameObject* door = CreateGameObjectFromFBX(L"BossDoor");
		//	door->GetTransform()->SetLocalPosition(Vector3D(0, 0, 25));
		//	door->GetTransform()->SetLocalScale(Vector3D(1, 1, 1));
		//
		//	GameObject* hand = CreateGameObjectFromFBX(L"BossDoorHand");
		//	hand->GetTransform()->SetLocalScale(Vector3D(1, 1, 1));
		//
		//	hand->GetTransform()->SetParent(door->GetTransform());
		//	Animator* animator = hand->AddComponent<Animator>();
		//	FF::FFAnimationClip& clip = GetAnimationClip(L"Hand_rig|Idle");
		//	clip.isLoop = true;
		//	animator->PlusAnimationClip(GetAnimationClip(L"Hand_rig|Idle"));
		//	animator->Play(L"Hand_rig|Idle");
		//}


		//for (int i = 0; i < 8; ++i)
		//{
		//	GameObject* scroll = CreateGameObjectFromFBX(L"Scroll");
		//	scroll->GetTransform()->SetLocalPosition(Vector3D(0 + i, 4, 5));
		//	scroll->GetTransform()->SetLocalScale(Vector3D(1, 1, 1));
		//}


		//for (int i = 0; i < 6; ++i)
		//{
		//	GameObject* scroll = CreateGameObjectFromFBX(L"BossFlag");
		//	scroll->GetTransform()->SetLocalPosition(Vector3D(0 - i, 4, 5));
		//	scroll->GetTransform()->SetLocalScale(Vector3D(1, 1, 1));
		//}

		{
			GameObject* particle = SceneManager::GetInstance().CreateGameObject(L"Particle");
			particle->GetTransform()->SetLocalPosition(Vector3D(0,2.0f,0));
			particle->AddComponent<ParticleRenderer>();
		}

		for (int i = 0; i < 1; ++i)
		{
			GameObject* Boss = CreateGameObjectFromFBX(L"Boss");
			Boss->GetTransform()->SetLocalRotation(Vector3D(0, 180, 0));
			Boss->GetTransform()->SetLocalPosition(Vector3D(0, 4, 0));

			Animator* anim = Boss->AddComponent<Animator>();

			FF::FFAnimationClip& clip = GetAnimationClip(L"root|000.Idle");
			clip.isLoop = true;

			anim->PlusAnimationClip(GetAnimationClip(L"root|000.Idle"));
			anim->PlusAnimationClip(GetAnimationClip(L"root|001-1.Idle->Walk"));
			anim->PlusAnimationClip(GetAnimationClip(L"root|001-2.Walk"));
			anim->PlusAnimationClip(GetAnimationClip(L"root|001-3.Walk->Idle"));

			anim->PlusAnimationClip(GetAnimationClip(L"root|002-1.NormalAttack_R"));
			anim->PlusAnimationClip(GetAnimationClip(L"root|002-2.NormalAttack_R->Idle"));

			anim->PlusAnimationClip(GetAnimationClip(L"root|003-1.NormalAttack_L"));
			anim->PlusAnimationClip(GetAnimationClip(L"root|003-2.NormalAttack_L->Idle"));

			anim->PlusAnimationClip(GetAnimationClip(L"root|004.RangedAttack"));

			anim->PlusAnimationClip(GetAnimationClip(L"root|005.Teleport"));

			anim->PlusAnimationClip(GetAnimationClip(L"root|006-1.Skill_Tree"));
			anim->PlusAnimationClip(GetAnimationClip(L"root|006-2.Skill_Tree->Idle"));

			anim->PlusAnimationClip(GetAnimationClip(L"root|007-1.Idle->Skill_ChargeAttack"));
			anim->PlusAnimationClip(GetAnimationClip(L"root|007-2.Skill_ChargeAttack"));
			anim->PlusAnimationClip(GetAnimationClip(L"root|007-3.Skill_ChargeAttack->Idle"));

			anim->PlusAnimationClip(GetAnimationClip(L"root|008.Skill_Knockback"));

			anim->PlusAnimationClip(GetAnimationClip(L"root|009.Skill_Blocking"));

			anim->PlusAnimationClip(GetAnimationClip(L"root|010.Skill_2D"));

			anim->PlusAnimationClip(GetAnimationClip(L"root|011-1.Groggy"));
			anim->PlusAnimationClip(GetAnimationClip(L"root|011-2.Groogy->Idle"));

			anim->PlusAnimationClip(GetAnimationClip(L"root|012.Death"));

			anim->Play(L"root|000.Idle");
		}

		// 플레이어
		GameObject* player = CreateGameObjectFromFBX(L"Player");
		player->GetTransform()->GetChild(0)->GetGameObject()->GetComponent<SkinnedMeshRenderer>()->GetMaterialInfo(0).vec4Params[0] = 1.0f;
		Animator* playerAnim = player->AddComponent<Animator>();
		FF::FFAnimationClip& phase3PlayerClip = GetAnimationClip(L"Armature|Phase3_Player");
		phase3PlayerClip.isLoop = true;
		playerAnim->PlusAnimationClip(phase3PlayerClip);
		playerAnim->Play(L"Armature|Phase3_Player");

		// 보스 나뭇가지
		{
			//GameObject* vfx_circle = CreateGameObjectFromFBX(L"VFX_Circle");
			//vfx_circle->GetTransform()->SetLocalPosition(Vector3D(0, 5, 0));
			//EffectPlayer* circle_effectPlayer = vfx_circle->AddComponent<EffectPlayer>();
			//circle_effectPlayer->SetMeshRenderer(vfx_circle->GetTransform()->GetChild(0)->GetGameObject()->GetComponent<MeshRenderer>());
			//circle_effectPlayer->SetPlaySpeed(0.2f);
			//circle_effectPlayer->SetMaxValue(0.5f);
			//circle_effectPlayer->SetRemainTime(1000.f);
			//circle_effectPlayer->SetDirection(Vector2D(1, 0));
			////vfx_circle->GetTransform()->SetParent(treeParent->GetTransform());
			//
			//GameObject* treeParent = SceneManager::GetInstance().CreateGameObject(L"TreeParent");



			//GameObject* vfx_tree0 = CreateGameObjectFromFBX(L"VFX_Tree");
			//EffectPlayer* tree0_effectPlayer = vfx_tree0->AddComponent<EffectPlayer>();
			//tree0_effectPlayer->SetMeshRenderer(vfx_tree0->GetTransform()->GetChild(0)->GetGameObject()->GetComponent<MeshRenderer>());
			//tree0_effectPlayer->SetPlaySpeed(0.2f);
			//tree0_effectPlayer->SetMaxValue(0.5f);
			//tree0_effectPlayer->SetRemainTime(1000.f);
			//tree0_effectPlayer->SetDirection(Vector2D(1, 0));
			//vfx_tree0->GetTransform()->SetParent(treeParent->GetTransform());
			//vfx_tree0->GetTransform()->SetLocalRotation(Vector3D(0, 0, 0));
			//
			//GameObject* vfx_tree1 = CreateGameObjectFromFBX(L"VFX_Tree");
			//EffectPlayer* tree1_effectPlayer = vfx_tree1->AddComponent<EffectPlayer>();
			//tree1_effectPlayer->SetMeshRenderer(vfx_tree1->GetTransform()->GetChild(0)->GetGameObject()->GetComponent<MeshRenderer>());
			//tree1_effectPlayer->SetPlaySpeed(0.2f);
			//tree1_effectPlayer->SetMaxValue(0.5f);
			//tree1_effectPlayer->SetRemainTime(1000.f);
			//tree1_effectPlayer->SetDirection(Vector2D(1, 0));
			//vfx_tree1->GetTransform()->SetParent(treeParent->GetTransform());
			//vfx_tree1->GetTransform()->SetLocalRotation(Vector3D(0, 180, 0));
			//
			//GameObject* vfx_tree2 = CreateGameObjectFromFBX(L"VFX_Tree");
			//EffectPlayer* tree2_effectPlayer = vfx_tree2->AddComponent<EffectPlayer>();
			//tree2_effectPlayer->SetMeshRenderer(vfx_tree2->GetTransform()->GetChild(0)->GetGameObject()->GetComponent<MeshRenderer>());
			//tree2_effectPlayer->SetPlaySpeed(0.2f);
			//tree2_effectPlayer->SetMaxValue(0.5f);
			//tree2_effectPlayer->SetRemainTime(1000.f);
			//tree2_effectPlayer->SetDirection(Vector2D(1, 0));
			//vfx_tree2->GetTransform()->SetParent(treeParent->GetTransform());
			//vfx_tree2->GetTransform()->SetLocalRotation(Vector3D(0, 90, 0));
			//
			//GameObject* vfx_tree3 = CreateGameObjectFromFBX(L"VFX_Tree");
			//EffectPlayer* tree3_effectPlayer = vfx_tree3->AddComponent<EffectPlayer>();
			//tree3_effectPlayer->SetMeshRenderer(vfx_tree3->GetTransform()->GetChild(0)->GetGameObject()->GetComponent<MeshRenderer>());
			//tree3_effectPlayer->SetPlaySpeed(0.2f);
			//tree3_effectPlayer->SetMaxValue(0.5f);
			//tree3_effectPlayer->SetRemainTime(1000.f);
			//tree3_effectPlayer->SetDirection(Vector2D(1, 0));
			//vfx_tree3->GetTransform()->SetParent(treeParent->GetTransform());
			//vfx_tree3->GetTransform()->SetLocalRotation(Vector3D(0, 270, 0));
		}

		//for(int i = 0; i<10; ++i)
		//{
		//	GameObject* Boss = SceneManager::GetInstance().CreateGameObject(L"Boss");
		//	Boss->GetTransform()->SetLocalPosition(Vector3D(0,5,0));
		//	CapsuleCollider* collider = Boss->AddComponent<CapsuleCollider>();
		//	collider->SetTrigger(true);
		//	MeshRenderer* renderer = Boss->AddComponent<MeshRenderer>();
		//	renderer->SetMaterial(L"BoundaryDebug");
		//	renderer->SetMesh(L"Capsule");
		//}


		SaveScene();
	}
	else if (x == GAME)
	{
		{
			SceneManager::GetInstance().SetName(L"MainScene");

			// 게임 매니저
			GameObject* gameManagerObject = SceneManager::GetInstance().CreateGameObject(L"GameManager");
			GameManager* gameManager = gameManagerObject->AddComponent<GameManager>();

			{
				GameObject* coverObject = SceneManager::GetInstance().CreateGameObject(L"CoverObject");
				coverObject->AddComponent<ZeldaUI>();
				Image* coverImage = coverObject->AddComponent<Image>();
				coverImage->SetLayer(901);
				coverObject->GetTransform()->SetLocalScale(Vector3D(1920, 1080));
				SpriteAnimator* mainanimator = coverObject->AddComponent<SpriteAnimator>();
				gameManager->SetMainAnimator(mainanimator);
			}

			{
				GameObject* coverObject = SceneManager::GetInstance().CreateGameObject(L"CoverObject");
				coverObject->AddComponent<ZeldaUI>();
				Image* coverImage = coverObject->AddComponent<Image>();
				coverImage->SetLayer(900);
				coverObject->GetTransform()->SetLocalScale(Vector3D(1920, 1080));
				SpriteAnimator* subanimator = coverObject->AddComponent<SpriteAnimator>();
				gameManager->SetSubAnimator(subanimator);
			}

			{
				GameObject* coverObject = SceneManager::GetInstance().CreateGameObject(L"CoverObject");
				coverObject->AddComponent<ZeldaUI>();
				Image* coverImage = coverObject->AddComponent<Image>();
				coverImage->SetLayer(899);
				coverObject->GetTransform()->SetLocalScale(Vector3D(1920, 1080));
				SpriteAnimator* thirdanimator = coverObject->AddComponent<SpriteAnimator>();
				gameManager->SetThirdAnimator(thirdanimator);
			}

			// BGM
			SoundManager::GetInstance().PlayBGM(SoundManager::BGM::Opening);

			// 타이틀 배경
			{
				Vector2D backgroundSize = { 1920, 1080 };

				GameObject* object = SceneManager::GetInstance().CreateGameObject(L"TitleBackground");
				object->GetTransform()->SetLocalScale(Vector3D(backgroundSize.x, backgroundSize.y, 0));
				ZeldaUI* ui = object->AddComponent<ZeldaUI>();
				Image* image = object->AddComponent<Image>();
				image->SetTexture(L"TitleBackground");
				image->SetLayer(-900);
			}

			// SoundBar
			{
				Vector2D pos = { -660, 346 };
				Vector2D size = { 285, 25 };

				GameObject* obj = SceneManager::GetInstance().CreateGameObject(L"Obj_SoundBar");
				obj->GetTransform()->SetLocalScale(Vector3D(size.x, size.y, 0));
				obj->GetTransform()->SetLocalPosition(Vector3D(pos.x, pos.y, 0));
				SoundBar* sb = obj->AddComponent<SoundBar>();
				obj->AddComponent<ScrollUI>();
				ZeldaUI* ui = obj->AddComponent<ZeldaUI>();
				Image* image = obj->AddComponent<Image>();
				image->SetTexture(L"ScrollUI");
				image->SetLayer(0);
				sb->SetUISize(size);
				sb->SetPadding(20);

				// Handle
				{
					Vector2D hsize = { 30, 30 };

					GameObject* hobj = SceneManager::GetInstance().CreateGameObject(L"SoundBar_Handle");
					hobj->GetTransform()->SetParent(obj->GetTransform());
					hobj->GetTransform()->SetLocalScale(Vector3D(hsize.x, hsize.y, 0));
					hobj->SetActive(false);
					SoundHandle* sh = hobj->AddComponent<SoundHandle>();
					ZeldaUI* ui = hobj->AddComponent<ZeldaUI>();
					Image* image = hobj->AddComponent<Image>();
					image->SetTexture(L"SoundHandle");
					image->SetLayer(10);

					sb->SetHandle(*sh);
				}

				// SoundBarText
				{
					Vector2D textSize = { 125.0f, 34.0f };

					GameObject* soundText = SceneManager::GetInstance().CreateGameObject(L"Obj_SoundText");
					soundText->GetTransform()->SetParent(obj->GetTransform());
					soundText->GetTransform()->SetLocalScale(Vector3D(textSize.x, textSize.y, 0));
					soundText->GetTransform()->SetLocalPosition(Vector3D(-79.5, 37.1, 0));
					ZeldaUI* soundText_ui = soundText->AddComponent<ZeldaUI>();
					Image* soundText_texture = soundText->AddComponent<Image>();
					soundText_texture->SetTexture(L"SoundText");
					soundText_texture->SetLayer(5);
				}
			}

			// 타이틀 버튼
			{
				// 우측 하단으로부터 offset
				// 모든 버튼은 원점에 대해서 정렬함
				Vector2D offset = { 330, 200 };
				float interval = 20;

				float beforeUIHeight = 0;
				GameObject* titleButtonGroup = SceneManager::GetInstance().CreateGameObject(L"TitleButtonGroup");
				ZeldaUI* ui = titleButtonGroup->AddComponent<ZeldaUI>();
				titleButtonGroup->GetTransform()->SetLocalPosition(Vector3D(960 - offset.x, -540 + offset.y));

				// 키보드 입력에 따른 버튼 조정

				GameObject* bs = SceneManager::GetInstance().CreateGameObject(L"TitleButtonSelector");
				bs->GetTransform()->SetParent(titleButtonGroup->GetTransform());
				ButtonSelector* bsp = bs->AddComponent<ButtonSelector>();

				// 게임종료 버튼
				{
					Vector2D buttonSize = { 310, 140 };

					GameObject* object = SceneManager::GetInstance().CreateGameObject(L"GameExitButton");
					object->GetTransform()->SetParent(titleButtonGroup->GetTransform());
					object->GetTransform()->SetLocalScale(Vector3D(buttonSize.x, buttonSize.y, 0));
					ExitButton* eb = object->AddComponent<ExitButton>();
					ZeldaUI* ui = object->AddComponent<ZeldaUI>();
					Image* image = object->AddComponent<Image>();
					image->SetTexture(L"Exit_Idle");
					Button* button = object->AddComponent<Button>();
					/// 억지로 마우스 이벤트로 변경되는 것을 막음
					//button->SetImage(L"Exit_Idle", L"Exit_Enter", L"Exit_Click");

					bsp->AddButton(*object);

					// Attach UI
					{
						Vector2D uiSize = { 70, 70 };

						GameObject* attachUI = SceneManager::GetInstance().CreateGameObject(L"Exit_AttachUI");
						attachUI->GetTransform()->SetParent(object->GetTransform());
						attachUI->GetTransform()->SetLocalScale(Vector3D(uiSize.x, uiSize.y));
						attachUI->GetTransform()->SetLocalPosition(Vector3D(-170, 0, 0));
						AttachUI* atp = attachUI->AddComponent<AttachUI>();
						ZeldaUI* ui = attachUI->AddComponent<ZeldaUI>();
						Image* image = attachUI->AddComponent<Image>();
						image->SetTexture(L"AttachUI");

						eb->SetAttachUI(*atp);
						eb->SetUISize(buttonSize);
						eb->SetAttachUISize(uiSize);

						attachUI->SetActive(false);
					}

					beforeUIHeight = buttonSize.y;
				}

				// 게임시작 버튼
				{
					Vector2D buttonSize = { 316, 140 };

					GameObject* object = SceneManager::GetInstance().CreateGameObject(L"GameStartButton");
					object->GetTransform()->SetParent(titleButtonGroup->GetTransform());
					object->GetTransform()->SetLocalScale(Vector3D(buttonSize.x, buttonSize.y, 0));
					object->GetTransform()->SetLocalPosition(Vector3D(0, 147.7f, 0));
					StartButton* sb = object->AddComponent<StartButton>();
					ZeldaUI* ui = object->AddComponent<ZeldaUI>();
					Image* image = object->AddComponent<Image>();
					image->SetTexture(L"Start_Idle");
					Button* button = object->AddComponent<Button>();
					/// 억지로 마우스 이벤트로 변경되는 것을 막음
					//button->SetImage(L"Start_Idle", L"Start_Enter", L"Start_Click");

					bsp->AddButton(*object);
					bsp->SetFirstButton(*object);

					// Attach UI
					{
						Vector2D uiSize = { 70, 70 };

						GameObject* attachUI = SceneManager::GetInstance().CreateGameObject(L"Start_AttachUI");
						attachUI->GetTransform()->SetParent(object->GetTransform());
						attachUI->GetTransform()->SetLocalScale(Vector3D(uiSize.x, uiSize.y));
						attachUI->GetTransform()->SetLocalPosition(Vector3D(-170, 0, 0));
						AttachUI* atp = attachUI->AddComponent<AttachUI>();
						ZeldaUI* ui = attachUI->AddComponent<ZeldaUI>();
						Image* image = attachUI->AddComponent<Image>();
						image->SetTexture(L"AttachUI");

						sb->SetAttachUI(*atp);
						sb->SetUISize(buttonSize);
						sb->SetAttachUISize(uiSize);

						attachUI->SetActive(false);
					}

					beforeUIHeight = buttonSize.y;
				}
			}

			SaveScene();
		}

		SceneManager::GetInstance().DeleteAllGameObject();
		SceneManager::GetInstance().DeleteAllDontDestGameObject();

		{
			SceneManager::GetInstance().SetName(L"IntroScene");

			// 카메라
			GameObject* cameraObject = SceneManager::GetInstance().CreateGameObject(L"MainCamera");
			Camera* camera = cameraObject->AddComponent<Camera>();
			SceneManager::GetInstance().SetMainCamera(camera);

			GameObject* cameraRootObject = SceneManager::GetInstance().CreateGameObject(L"CameraRoot");
			cameraRootObject->GetTransform()->AddChild(cameraObject->GetTransform());

			cameraObject->GetTransform()->Rotate(Vector3D::right, DegreeToRadian(90));
			cameraObject->GetTransform()->Rotate(Vector3D::up, DegreeToRadian(180));

			Tracer* tracer = cameraRootObject->AddComponent<Tracer>();

			// 라이트
			GameObject* lightObject = SceneManager::GetInstance().CreateGameObject(L"DirectionalLight");
			DirectionalLight* directionalLight = lightObject->AddComponent<DirectionalLight>();
			lightObject->GetTransform()->SetLocalRotation(Vector3D(70, 30, 0));

			// 게임 매니저
			GameObject* gameManagerObject = SceneManager::GetInstance().CreateGameObject(L"GameManager");
			GameManager* gameManager = gameManagerObject->AddComponent<GameManager>();

			{
				GameObject* coverObject = SceneManager::GetInstance().CreateGameObject(L"CoverObject");
				coverObject->AddComponent<ZeldaUI>();
				Image* coverImage = coverObject->AddComponent<Image>();
				coverImage->SetLayer(901);
				coverObject->GetTransform()->SetLocalScale(Vector3D(1920, 1080));
				SpriteAnimator* mainanimator = coverObject->AddComponent<SpriteAnimator>();
				gameManager->SetMainAnimator(mainanimator);
			}

			{
				GameObject* coverObject = SceneManager::GetInstance().CreateGameObject(L"CoverObject");
				coverObject->AddComponent<ZeldaUI>();
				Image* coverImage = coverObject->AddComponent<Image>();
				coverImage->SetLayer(900);
				coverObject->GetTransform()->SetLocalScale(Vector3D(1920, 1080));
				SpriteAnimator* subanimator = coverObject->AddComponent<SpriteAnimator>();
				gameManager->SetSubAnimator(subanimator);
			}

			{
				GameObject* coverObject = SceneManager::GetInstance().CreateGameObject(L"CoverObject");
				coverObject->AddComponent<ZeldaUI>();
				Image* coverImage = coverObject->AddComponent<Image>();
				coverImage->SetLayer(899);
				coverObject->GetTransform()->SetLocalScale(Vector3D(1920, 1080));
				SpriteAnimator* thirdanimator = coverObject->AddComponent<SpriteAnimator>();
				gameManager->SetThirdAnimator(thirdanimator);
			}

			// 플레이어 생성
			Vector3D diffHeight = Vector3D(0, -0.9f, 0);
			GameObject* testPlayer = SceneManager::GetInstance().CreateGameObject(L"testPlayer");
			GameObject* playerHitOjbect = SceneManager::GetInstance().CreateGameObject(L"playerHitOjbect");
			GameObject* playerAnimator = CreateGameObjectFromFBX(L"Player");
			testPlayer->GetTransform()->AddChild(playerHitOjbect->GetTransform());
			testPlayer->GetTransform()->AddChild(playerAnimator->GetTransform());
			playerAnimator->GetTransform()->SetLocalPosition(diffHeight);

			Animator* playeranim = playerAnimator->AddComponent<Animator>();
			FF::FFAnimationClip& playerrunClip = GetAnimationClip(L"Armature|running");
			playerrunClip.isLoop = true;

			FF::FFAnimationClip& playeridleClip = GetAnimationClip(L"Armature|idle");
			playeridleClip.isLoop = true;


			playeranim->PlusAnimationClip(GetAnimationClip(L"Armature|attack"));
			playeranim->PlusAnimationClip(GetAnimationClip(L"Armature|attack2"));
			playeranim->PlusAnimationClip(GetAnimationClip(L"Armature|damage"));
			playeranim->PlusAnimationClip(GetAnimationClip(L"Armature|dash"));
			playeranim->PlusAnimationClip(GetAnimationClip(L"Armature|down"));
			playeranim->PlusAnimationClip(GetAnimationClip(L"Armature|idle"));
			playeranim->PlusAnimationClip(GetAnimationClip(L"Armature|running"));


			playeranim->Play(L"Armature|idle");



			CapsuleCollider* playerHitCollider = playerHitOjbect->AddComponent<CapsuleCollider>();
			playerHitCollider->SetTrigger(true);
			//playerHitOjbect->GetTransform()->SetLocalPosition(Vector3D(10, 10, 11));

			SkinnedMeshRenderer* playerSkinnedMeshRenderer = playerAnimator->GetTransform()->GetChild(0)->GetGameObject()->GetComponent<SkinnedMeshRenderer>();

			Player* playerComponent = testPlayer->AddComponent<Player>();
			playerComponent->SetAnimator(playeranim);
			playerComponent->SetSkinnedMeshRenderer(playerSkinnedMeshRenderer);
			CapsuleCollider* playerCollider = testPlayer->AddComponent<CapsuleCollider>();
			//playerCollider->SetTrigger(true);
			Rigidbody* playerRigidBody = testPlayer->AddComponent<Rigidbody>();
			playerRigidBody->SetFreezeRotationX(true);
			playerRigidBody->SetFreezeRotationY(true);
			playerRigidBody->SetFreezeRotationZ(true);
			//playerRigidBody->SetKinematic(true);
			playerComponent->SetRigidBody(playerRigidBody);
			testPlayer->SetLayerName(L"Player");
			playerHitOjbect->SetLayerName(L"Player");

			GameObject* VFX_PlayerAttack = CreateGameObjectFromFBX(L"VFX_PlayerAttack");
			VFX_PlayerAttack->GetTransform()->SetLocalPosition(Vector3D(0, 0, 0));
			VFX_PlayerAttack->GetTransform()->SetParent(testPlayer->GetTransform());
			playerComponent->SetPlayerAttackEffect(VFX_PlayerAttack);
			VFX_PlayerAttack->SetActive(false);
			EffectPlayer* playerAttackEffect = VFX_PlayerAttack->AddComponent<EffectPlayer>();
			playerAttackEffect->SetMeshRenderer(VFX_PlayerAttack->GetTransform()->GetChild(0)->GetGameObject()->GetComponent<MeshRenderer>());
			playerAttackEffect->SetMaxValue(1.f);
			playerAttackEffect->SetPlaySpeed(1.5f);
			playerAttackEffect->SetRemainTime(0.3f);
			playerAttackEffect->SetDirection(Vector2D(1, 0));

			// 특수공격 이펙트
			GameObject* VFX_PlayerSpecialAttack = CreateGameObjectFromFBX(L"VFX_PlayerSpecialAttack");
			VFX_PlayerSpecialAttack->GetTransform()->SetLocalPosition(Vector3D(0, 0, 0));
			VFX_PlayerSpecialAttack->GetTransform()->SetParent(testPlayer->GetTransform());
			playerComponent->SetPlayerSpecialAttackEffect(VFX_PlayerSpecialAttack);
			VFX_PlayerSpecialAttack->SetActive(false);
			EffectPlayer* playerSpecialAttackEffect = VFX_PlayerSpecialAttack->AddComponent<EffectPlayer>();
			playerSpecialAttackEffect->SetMeshRenderer(VFX_PlayerSpecialAttack->GetTransform()->GetChild(0)->GetGameObject()->GetComponent<MeshRenderer>());
			playerSpecialAttackEffect->SetMaxValue(1.f);
			playerSpecialAttackEffect->SetPlaySpeed(1.5f);
			playerSpecialAttackEffect->SetRemainTime(0.3f);
			playerSpecialAttackEffect->SetDirection(Vector2D(1, 0));

			// 바닥 & 벽
			float floorPosY = -10.0f;

			float width = 37.0f;
			float height = width * 7000.0f / 1920.0f;
			float wallThickness = 20.0f;
			float wallheight = 30.0f;
			float walloffset = -2.0f;
			float cameraoffset = 10.0f;
			float playeroffset = 3.0f;

			testPlayer->GetTransform()->SetLocalPosition(Vector3D(0, 0, height * 0.5f - playeroffset));

			GameObject* platform = SceneManager::GetInstance().CreateGameObject(L"platform");

			platform->SetLayerName(L"Floor");
			CubeCollider* platformCollider = platform->AddComponent<CubeCollider>();
			Rigidbody* platformRigidbody = platform->AddComponent<Rigidbody>();
			platformRigidbody->SetKinematic(true);
			// 반발계수
			platformRigidbody->SetRestitution(0);
			platformRigidbody->SetKfriction(0.9f);
			platformRigidbody->SetSfriction(0.95f);


			platform->GetTransform()->SetLocalScale(Vector3D(width + 2 * wallThickness, 1, height + 2 * wallThickness));
			platform->GetTransform()->SetLocalPosition(Vector3D(0, floorPosY, 0));

			{
				GameObject* introSpriteObject = SceneManager::GetInstance().CreateGameObject(L"Intro");
				SpriteRenderer* sprite = introSpriteObject->AddComponent<SpriteRenderer>();
				sprite->SetTexture(L"Intro");
				introSpriteObject->GetTransform()->SetLocalScale(Vector3D(width, height, 1));
				introSpriteObject->GetTransform()->SetLocalPosition(Vector3D(0, floorPosY + 0.5f, 0));
			}

			{
				GameObject* introSpriteObject = SceneManager::GetInstance().CreateGameObject(L"Intro");
				SpriteRenderer* sprite = introSpriteObject->AddComponent<SpriteRenderer>();
				sprite->SetTexture(L"Intro_Gradation");
				introSpriteObject->GetTransform()->SetLocalScale(Vector3D(width, height, 1));
				introSpriteObject->GetTransform()->SetLocalPosition(Vector3D(0, floorPosY + 10.f, 0));
			}


			int dx[4] = { 0, 0, -1, 1 };
			int dz[4] = { -1, 1, 0, 0 };

			for (int i = 0; i < 4; i++)
			{
				GameObject* wall = SceneManager::GetInstance().CreateGameObject(L"wall");
				wall->SetLayerName(L"Floor");
				CubeCollider* platformCollider = wall->AddComponent<CubeCollider>();
				Rigidbody* platformRigidbody = wall->AddComponent<Rigidbody>();
				platformRigidbody->SetKinematic(true);
				platformRigidbody->SetRestitution(0);
				platformRigidbody->SetKfriction(0.9f);
				platformRigidbody->SetSfriction(0.95f);


				if (i == 0)
				{
					TriggerEventSetter* eventsetter = wall->AddComponent<TriggerEventSetter>();
					eventsetter->SetCollisionEnterEventLoadScene(L"Directing Scene 0");
				}

				wall->GetTransform()->SetLocalScale(Vector3D(dx[i] == 0 ? (width + 2 * wallThickness) : (wallThickness), wallheight, dz[i] == 0 ? (height + 2 * wallThickness) : (wallThickness)));
				wall->GetTransform()->SetLocalPosition(Vector3D(dx[i] == 0 ? (0) : (dx[i] * ((width + wallThickness) * 0.5f + walloffset)), floorPosY, dz[i] == 0 ? (0) : (dz[i] * ((height + wallThickness) * 0.5f + walloffset))));
			}

			tracer->SetTarget(testPlayer->GetTransform());

			tracer->SetOffSet(Vector3D(0, 75, 0));
			tracer->TracePositionX(false);
			tracer->TracePositionY(true);
			tracer->TracePositionZ(true);
			tracer->SetClampPositionZ(true, -height * 0.5f + cameraoffset, height * 0.5f - cameraoffset);

			SaveScene();
		}

		SceneManager::GetInstance().DeleteAllGameObject();
		SceneManager::GetInstance().DeleteAllDontDestGameObject();

#pragma region 연출씬 0, 인트로 - 게임
		{
			SceneManager::GetInstance().SetName(L"Directing Scene 0");

			// 카메라
			GameObject* cameraObject = SceneManager::GetInstance().CreateGameObject(L"MainCamera");
			Camera* camera = cameraObject->AddComponent<Camera>();
			auto trigger = cameraObject->AddComponent<CubeCollider>();
			trigger->SetTrigger(true);
			SceneManager::GetInstance().SetMainCamera(camera);
			RailMoveCamera* cam = cameraObject->AddComponent<RailMoveCamera>();
			cameraObject->AddComponent<Dotween>();

			GameObject* obj_r0 = SceneManager::GetInstance().CreateGameObject(L"RailPoint0");
			obj_r0->GetTransform()->SetLocalPosition(Vector3D(0, 0, -100));
			obj_r0->GetTransform()->SetLocalRotation(Vector3D(0, 180, 0));
			auto r0 = obj_r0->AddComponent<CameraRailPoint>();
			cam->AddRailPoint(*r0);

			GameObject* obj_r1 = SceneManager::GetInstance().CreateGameObject(L"RailPoint1");
			obj_r1->GetTransform()->SetLocalPosition(Vector3D(0, 0, 0));
			obj_r1->GetTransform()->SetLocalRotation(Vector3D(0, 180, 0));
			auto r1 = obj_r1->AddComponent<CameraRailPoint>();
			cam->AddRailPoint(*r1);

			GameObject* obj_r2 = SceneManager::GetInstance().CreateGameObject(L"RailPoint2");
			obj_r2->GetTransform()->SetLocalPosition(Vector3D(0, 0, 100));
			obj_r2->GetTransform()->SetLocalRotation(Vector3D(0, 180, 0));
			auto r2 = obj_r2->AddComponent<CameraRailPoint>();
			cam->AddRailPoint(*r2);

			GameObject* obj_r3 = SceneManager::GetInstance().CreateGameObject(L"RailPoint3");
			obj_r3->GetTransform()->SetLocalPosition(Vector3D(0, 50, 100));
			obj_r3->GetTransform()->SetLocalRotation(Vector3D(30, 180, 0));
			auto r3 = obj_r3->AddComponent<CameraRailPoint>();
			cam->AddRailPoint(*r3);

			GameObject* obj_r4 = SceneManager::GetInstance().CreateGameObject(L"RailPoint4");
			obj_r4->GetTransform()->SetLocalPosition(Vector3D(0, 50, 100));
			obj_r4->GetTransform()->SetLocalRotation(Vector3D(30, 180, 0));
			auto r4 = obj_r4->AddComponent<CameraRailPoint>();
			cam->AddRailPoint(*r4);

			GameObject* obj_r5 = SceneManager::GetInstance().CreateGameObject(L"RailPoint5");
			obj_r5->GetTransform()->SetLocalPosition(Vector3D(0, 50, 100));
			obj_r5->GetTransform()->SetLocalRotation(Vector3D(30, 180, 0));
			auto r5 = obj_r5->AddComponent<CameraRailPoint>();
			cam->AddRailPoint(*r5);

			GameObject* obj_r6 = SceneManager::GetInstance().CreateGameObject(L"RailPoint6");
			obj_r6->GetTransform()->SetLocalPosition(Vector3D(0, 50, 100));
			obj_r5->GetTransform()->SetLocalRotation(Vector3D(30, 180, 0));
			auto r6 = obj_r6->AddComponent<CameraRailPoint>();
			cam->AddRailPoint(*r6);

			GameObject* obj_r7 = SceneManager::GetInstance().CreateGameObject(L"RailPoint7");
			obj_r7->GetTransform()->SetLocalPosition(Vector3D(0, 50, 100));
			obj_r5->GetTransform()->SetLocalRotation(Vector3D(30, 180, 0));
			auto r7 = obj_r7->AddComponent<CameraRailPoint>();
			cam->AddRailPoint(*r7);

			GameObject* obj_t0 = SceneManager::GetInstance().CreateGameObject(L"TriggerPoint0");
			obj_t0->GetTransform()->SetLocalPosition(Vector3D(0, 4.2f, -4.8f));
			auto t0 = obj_t0->AddComponent<RailTriggerPoint>();
			cam->AddTriggerPoint(*t0);
			auto col0 = obj_t0->AddComponent<CubeCollider>();
			col0->SetTrigger(true);
			t0->SetCollider(col0);

			GameObject* obj_t1 = SceneManager::GetInstance().CreateGameObject(L"TriggerPoint1");
			obj_t1->GetTransform()->SetLocalPosition(Vector3D(0, 4.2f, 12.6f));
			auto t1 = obj_t1->AddComponent<RailTriggerPoint>();
			cam->AddTriggerPoint(*t1);
			auto col1 = obj_t1->AddComponent<CubeCollider>();
			col1->SetTrigger(true);
			t1->SetCollider(col1);

			GameObject* obj_t2 = SceneManager::GetInstance().CreateGameObject(L"TriggerPoint2");
			obj_t2->GetTransform()->SetLocalPosition(Vector3D(0, 4.2f, 24.0f));
			auto t2 = obj_t2->AddComponent<RailTriggerPoint>();
			cam->AddTriggerPoint(*t2);
			auto col2 = obj_t2->AddComponent<CubeCollider>();
			col2->SetTrigger(true);
			t2->SetCollider(col2);

			GameObject* obj_t3 = SceneManager::GetInstance().CreateGameObject(L"TriggerPoint3");
			obj_t3->GetTransform()->SetLocalPosition(Vector3D(0, 45.0f, 32.0f));
			obj_t3->GetTransform()->SetLocalRotation(EulerToQuaternion(Vector3D(-130.0f, 0, 180.0f)));
			auto t3 = obj_t3->AddComponent<RailTriggerPoint>();
			cam->AddTriggerPoint(*t3);
			auto col3 = obj_t3->AddComponent<CubeCollider>();
			col3->SetTrigger(true);
			t3->SetCollider(col3);

			GameObject* obj_t4 = SceneManager::GetInstance().CreateGameObject(L"TriggerPoint4");
			obj_t4->GetTransform()->SetLocalPosition(Vector3D(0, 45.0f, 32.0f));
			obj_t4->GetTransform()->SetLocalRotation(EulerToQuaternion(Vector3D(-130.0f, 0, 180.0f)));
			auto t4 = obj_t4->AddComponent<RailTriggerPoint>();
			cam->AddTriggerPoint(*t4);
			auto col4 = obj_t4->AddComponent<CubeCollider>();
			col4->SetTrigger(true);
			t4->SetCollider(col4);

			GameObject* obj_t5 = SceneManager::GetInstance().CreateGameObject(L"TriggerPoint5");
			obj_t5->GetTransform()->SetLocalPosition(Vector3D(0, 45.0f, 32.0f));
			obj_t5->GetTransform()->SetLocalRotation(EulerToQuaternion(Vector3D(-130.0f, 0, 180.0f)));
			auto t5 = obj_t5->AddComponent<RailTriggerPoint>();
			cam->AddTriggerPoint(*t5);
			auto col5 = obj_t5->AddComponent<CubeCollider>();
			col5->SetTrigger(true);
			t5->SetCollider(col5);

			GameObject* obj_t6 = SceneManager::GetInstance().CreateGameObject(L"TriggerPoint6");
			obj_t6->GetTransform()->SetLocalPosition(Vector3D(0, 45.0f, 32.0f));
			obj_t6->GetTransform()->SetLocalRotation(EulerToQuaternion(Vector3D(-130.0f, 0, 180.0f)));
			auto t6 = obj_t6->AddComponent<RailTriggerPoint>();
			cam->AddTriggerPoint(*t6);
			auto col6 = obj_t6->AddComponent<CubeCollider>();
			col6->SetTrigger(true);
			t6->SetCollider(col6);

			GameObject* obj_t7 = SceneManager::GetInstance().CreateGameObject(L"TriggerPoint7");
			obj_t7->GetTransform()->SetLocalPosition(Vector3D(0, 45.0f, 32.0f));
			obj_t7->GetTransform()->SetLocalRotation(EulerToQuaternion(Vector3D(-130.0f, 0, 180.0f)));
			obj_t7->AddComponent<TriggerEventSetter>()->SetTriggerEnterEventLoadScene(L"GameScene");
			auto t7 = obj_t7->AddComponent<RailTriggerPoint>();
			cam->AddTriggerPoint(*t7);
			auto col7 = obj_t7->AddComponent<CubeCollider>();
			col7->SetTrigger(true);
			t7->SetCollider(col7);

			cam->SetRangePlayTime(0, 6);
			cam->SetRangePlayTime(1, 4);
			cam->SetRangePlayTime(2, 5);
			cam->SetRangePlayTime(3, 0);
			cam->SetRangePlayTime(4, 0);
			cam->SetRangePlayTime(5, 0);
			cam->SetRangePlayTime(6, 0);

			cam->SetRangeDelayTime(0, 2);
			cam->SetRangeDelayTime(1, 2);
			cam->SetRangeDelayTime(2, 0);
			cam->SetRangeDelayTime(3, 0);
			cam->SetRangeDelayTime(4, 0);
			cam->SetRangeDelayTime(5, 0);
			cam->SetRangeDelayTime(6, 0);

			// 라이트
			GameObject* lightObject = SceneManager::GetInstance().CreateGameObject(L"DirectionalLight");
			DirectionalLight* directionalLight = lightObject->AddComponent<DirectionalLight>();
			lightObject->GetTransform()->SetLocalRotation(Vector3D(70, 30, 0));

			// 게임 매니저
			GameObject* gameManagerObject = SceneManager::GetInstance().CreateGameObject(L"GameManager");
			GameManager* gameManager = gameManagerObject->AddComponent<GameManager>();

			{
				GameObject* coverObject = SceneManager::GetInstance().CreateGameObject(L"CoverObject");
				coverObject->AddComponent<ZeldaUI>();
				Image* coverImage = coverObject->AddComponent<Image>();
				coverImage->SetLayer(901);
				coverObject->GetTransform()->SetLocalScale(Vector3D(1920, 1080));
				SpriteAnimator* mainanimator = coverObject->AddComponent<SpriteAnimator>();
				gameManager->SetMainAnimator(mainanimator);
			}

			{
				GameObject* coverObject = SceneManager::GetInstance().CreateGameObject(L"CoverObject");
				coverObject->AddComponent<ZeldaUI>();
				Image* coverImage = coverObject->AddComponent<Image>();
				coverImage->SetLayer(900);
				coverObject->GetTransform()->SetLocalScale(Vector3D(1920, 1080));
				SpriteAnimator* subanimator = coverObject->AddComponent<SpriteAnimator>();
				gameManager->SetSubAnimator(subanimator);
			}

			{
				GameObject* coverObject = SceneManager::GetInstance().CreateGameObject(L"CoverObject");
				coverObject->AddComponent<ZeldaUI>();
				Image* coverImage = coverObject->AddComponent<Image>();
				coverImage->SetLayer(899);
				coverObject->GetTransform()->SetLocalScale(Vector3D(1920, 1080));
				SpriteAnimator* subanimator = coverObject->AddComponent<SpriteAnimator>();
				gameManager->SetThirdAnimator(subanimator);
			}

#pragma region Player
			// 보스
			GameObject* Boss = CreateGameObjectFromFBX(L"Boss");
			Boss1* bossComponent = Boss->AddComponent<Boss1>();
			Boss->GetTransform()->SetLocalPosition(Vector3D(0, 0, 30));
			Animator* bossAnim = Boss->AddComponent<Animator>();
			auto clip = GetAnimationClip(L"root|000.Idle");
			clip.isLoop = true;
			bossAnim->PlusAnimationClip(GetAnimationClip(L"root|Phase3_Boss"));
			bossAnim->PlusAnimationClip(GetAnimationClip(L"root|000.Idle"));
			bossAnim->PlusAnimationClip(GetAnimationClip(L"root|001-1.Idle->Walk"));
			bossAnim->PlusAnimationClip(GetAnimationClip(L"root|001-2.Walk"));
			bossAnim->PlusAnimationClip(GetAnimationClip(L"root|001-3.Walk->Idle"));
			bossAnim->PlusAnimationClip(GetAnimationClip(L"root|002-1.NormalAttack_R"));
			bossAnim->PlusAnimationClip(GetAnimationClip(L"root|002-2.NormalAttack_R->Idle"));
			bossAnim->PlusAnimationClip(GetAnimationClip(L"root|003-1.NormalAttack_L"));
			bossAnim->PlusAnimationClip(GetAnimationClip(L"root|003-2.NormalAttack_L->Idle"));
			bossAnim->PlusAnimationClip(GetAnimationClip(L"root|004.RangedAttack"));
			bossAnim->PlusAnimationClip(GetAnimationClip(L"root|005.Teleport"));
			bossAnim->PlusAnimationClip(GetAnimationClip(L"root|006-1.Skill_Tree"));
			bossAnim->PlusAnimationClip(GetAnimationClip(L"root|006-2.Skill_Tree->Idle"));
			bossAnim->PlusAnimationClip(GetAnimationClip(L"root|007-1.Idle->Skill_ChargeAttack"));
			bossAnim->PlusAnimationClip(GetAnimationClip(L"root|007-2.Skill_ChargeAttack"));
			bossAnim->PlusAnimationClip(GetAnimationClip(L"root|007-3.Skill_ChargeAttack->Idle"));
			bossAnim->PlusAnimationClip(GetAnimationClip(L"root|008.Skill_Knockback"));
			bossAnim->PlusAnimationClip(GetAnimationClip(L"root|009.Skill_Blocking"));
			bossAnim->PlusAnimationClip(GetAnimationClip(L"root|010.Skill_2D"));
			bossAnim->PlusAnimationClip(GetAnimationClip(L"root|011-1.Groggy"));
			bossAnim->PlusAnimationClip(GetAnimationClip(L"root|011-2.Groogy->Idle"));
			bossAnim->PlusAnimationClip(GetAnimationClip(L"root|012.Death"));
			bossAnim->PlusAnimationClip(GetAnimationClip(L"root|PhaseTransition"));
			bossAnim->Play(L"root|000.Idle");

			// 플레이어
			GameObject* player = CreateGameObjectFromFBX(L"Player");
			player->GetTransform()->SetLocalPosition(Vector3D(0, 0, 70));
			Animator* playerAnim = player->AddComponent<Animator>();
			FF::FFAnimationClip& phase3PlayerClip = GetAnimationClip(L"Armature|idle");
			phase3PlayerClip.isLoop = true;
			playerAnim->PlusAnimationClip(GetAnimationClip(L"Armature|attack"));
			playerAnim->PlusAnimationClip(GetAnimationClip(L"Armature|attack2"));
			playerAnim->PlusAnimationClip(GetAnimationClip(L"Armature|damage"));
			playerAnim->PlusAnimationClip(GetAnimationClip(L"Armature|dash"));
			playerAnim->PlusAnimationClip(GetAnimationClip(L"Armature|down"));
			playerAnim->PlusAnimationClip(GetAnimationClip(L"Armature|idle"));
			playerAnim->PlusAnimationClip(GetAnimationClip(L"Armature|running"));
			playerAnim->Play(L"Armature|idle");

			t0->SetBoss1(Boss);
			t0->SetPlayer(player);
			t1->SetBoss1(Boss);
			t1->SetPlayer(player);
			t2->SetBoss1(Boss);
			t2->SetPlayer(player);
			t3->SetBoss1(Boss);
			t3->SetPlayer(player);
			t4->SetBoss1(Boss);
			t4->SetPlayer(player);
			t5->SetBoss1(Boss);
			t5->SetPlayer(player);
			t6->SetBoss1(Boss);
			t6->SetPlayer(player);
			t7->SetBoss1(Boss);
			t7->SetPlayer(player);


			// 족자 생성
			for (int i = 0; i < 8; i++)
			{
				GameObject* scrollObject = CreateGameObjectFromFBX(L"Scroll");
				FloatingObject* floatingComponent = scrollObject->AddComponent<FloatingObject>();
				floatingComponent->SetObjectTransform(scrollObject->GetTransform());
				scrollObject->GetTransform()->SetLocalPosition(Vector3D(-40 + i * 10, 0, -25));
				bossComponent->SetScrollComponent(floatingComponent);
			}

			// 깃발 생성
			for (int i = 0; i < 6; i++)
			{
				GameObject* bossFlag = CreateGameObjectFromFBX(L"BossFlag");
				MeshRenderer* flagRenderer = bossFlag->GetTransform()->GetChild(0)->GetGameObject()->GetComponent<MeshRenderer>();
				flagRenderer->GetMaterialInfo(0).texture[1] = L"..\\Resources\\Texture\\BossFlag\\delete.jpg";
				flagRenderer->GetMaterialInfo(0).texture[2] = L"..\\Resources\\Texture\\BossFlag\\T_Flag_Diffuse_002.png";
				flagRenderer->GetMaterialInfo(0).color = ZeldaColor::White;

				bossFlag->GetTransform()->SetLocalPosition(Vector3D(i * 15, 0, -30));


				BossFlag* flagComponent = bossFlag->AddComponent<BossFlag>();
				flagComponent->SetMeshRenderer(flagRenderer);
				flagComponent->SetPlaySpeed(0.000002);
			}
#pragma endregion

#pragma region floor
			/// BossFloor
			GameObject* BossFloor = CreateGameObjectFromFBX(L"BossFloor");
			GameObject* BossFloorMesh = BossFloor->GetTransform()->GetChild(0)->GetGameObject();
			MeshRenderer* renderer = BossFloorMesh->GetComponent<MeshRenderer>();
			renderer->GetMaterialInfo(1).shader = L"..\\Resources\\Shader\\BossFloor.hlsli";

			GameObject* child = SceneManager::GetInstance().CreateGameObject(L"child");
			child->SetLayerName(L"Floor");
			BossFloor->GetTransform()->AddChild(child->GetTransform());
			child->GetTransform()->SetLocalScale(Vector3D(50, 1, 50));
			Rigidbody* rigid = child->AddComponent<Rigidbody>();
			CylinderCollider* collider = child->AddComponent<CylinderCollider>();
			rigid->SetKinematic(true);
			rigid->SetRestitution(0.0f);
			rigid->SetKfriction(0.9f);
			rigid->SetSfriction(0.95f);
#pragma endregion

#pragma region door
			{
				GameObject* door = CreateGameObjectFromFBX(L"BossDoor");
				door->GetTransform()->SetLocalPosition(Vector3D(0, 0, -25));

				GameObject* hand = CreateGameObjectFromFBX(L"BossDoorHand");

				hand->GetTransform()->SetParent(door->GetTransform());
				Animator* animator = hand->AddComponent<Animator>();
				FF::FFAnimationClip& clip = GetAnimationClip(L"Hand_rig|Idle");
				clip.isLoop = true;
				animator->PlusAnimationClip(GetAnimationClip(L"Hand_rig|Idle"));
				animator->Play(L"Hand_rig|Idle");
			}
#pragma endregion

			SaveScene();
		}
#pragma endregion

		SceneManager::GetInstance().DeleteAllGameObject();
		SceneManager::GetInstance().DeleteAllDontDestGameObject();

#pragma region 연출씬 1, 페이즈 2-3 사이
		{
			SceneManager::GetInstance().SetName(L"Directing Scene 1");

			GameObject* backGround0 = SceneManager::GetInstance().CreateGameObject(L"Cube0");
			auto b0 = backGround0->AddComponent<MeshRenderer>();
			b0->SetMesh(L"Cube");
			b0->SetMaterial(L"CubeMaterial_F");

			GameObject* backGround1 = SceneManager::GetInstance().CreateGameObject(L"Cube1");
			auto b1 = backGround1->AddComponent<MeshRenderer>();
			b1->SetMesh(L"Cube");
			b1->SetMaterial(L"CubeMaterial_F");

			GameObject* backGround2 = SceneManager::GetInstance().CreateGameObject(L"Cube2");
			auto b2 = backGround2->AddComponent<MeshRenderer>();
			b2->SetMesh(L"Cube");
			b2->SetMaterial(L"CubeMaterial_F");

			GameObject* backGround3 = SceneManager::GetInstance().CreateGameObject(L"Cube3");
			auto b3 = backGround3->AddComponent<MeshRenderer>();
			b3->SetMesh(L"Cube");
			b3->SetMaterial(L"CubeMaterial_F");

			GameObject* backGround4 = SceneManager::GetInstance().CreateGameObject(L"Cube4");
			auto b4 = backGround4->AddComponent<MeshRenderer>();
			b4->SetMesh(L"Cube");
			b4->SetMaterial(L"CubeMaterial_F");

			GameObject* backGround5 = SceneManager::GetInstance().CreateGameObject(L"Cube5");
			auto b5 = backGround5->AddComponent<MeshRenderer>();
			b5->SetMesh(L"Cube");
			b5->SetMaterial(L"CubeMaterial_F");

			// 카메라
			GameObject* cameraObject = SceneManager::GetInstance().CreateGameObject(L"MainCamera");
			Camera* camera = cameraObject->AddComponent<Camera>();
			auto trigger = cameraObject->AddComponent<CubeCollider>();
			trigger->SetTrigger(true);
			SceneManager::GetInstance().SetMainCamera(camera);
			RailMoveCamera* cam = cameraObject->AddComponent<RailMoveCamera>();
			cameraObject->AddComponent<Dotween>();

			GameObject* obj_r0 = SceneManager::GetInstance().CreateGameObject(L"RailPoint0");
			auto r0 = obj_r0->AddComponent<CameraRailPoint>();
			cam->AddRailPoint(*r0);

			GameObject* obj_r1 = SceneManager::GetInstance().CreateGameObject(L"RailPoint1");
			auto r1 = obj_r1->AddComponent<CameraRailPoint>();
			cam->AddRailPoint(*r1);

			GameObject* obj_r2 = SceneManager::GetInstance().CreateGameObject(L"RailPoint2");
			auto r2 = obj_r2->AddComponent<CameraRailPoint>();
			cam->AddRailPoint(*r2);

			GameObject* obj_r3 = SceneManager::GetInstance().CreateGameObject(L"RailPoint3");
			auto r3 = obj_r3->AddComponent<CameraRailPoint>();
			cam->AddRailPoint(*r3);

			GameObject* obj_r4 = SceneManager::GetInstance().CreateGameObject(L"RailPoint4");
			auto r4 = obj_r4->AddComponent<CameraRailPoint>();
			cam->AddRailPoint(*r4);

			GameObject* obj_r5 = SceneManager::GetInstance().CreateGameObject(L"RailPoint5");
			auto r5 = obj_r5->AddComponent<CameraRailPoint>();
			cam->AddRailPoint(*r5);

			GameObject* obj_r6 = SceneManager::GetInstance().CreateGameObject(L"RailPoint6");
			auto r6 = obj_r6->AddComponent<CameraRailPoint>();
			cam->AddRailPoint(*r6);

			GameObject* obj_r7 = SceneManager::GetInstance().CreateGameObject(L"RailPoint7");
			auto r7 = obj_r7->AddComponent<CameraRailPoint>();
			cam->AddRailPoint(*r7);

			GameObject* obj_t0 = SceneManager::GetInstance().CreateGameObject(L"TriggerPoint0");
			auto t0 = obj_t0->AddComponent<RailTriggerPoint>();
			cam->AddTriggerPoint(*t0);
			auto col0 = obj_t0->AddComponent<CubeCollider>();
			col0->SetTrigger(true);
			t0->SetCollider(col0);

			GameObject* obj_t1 = SceneManager::GetInstance().CreateGameObject(L"TriggerPoint1");
			auto t1 = obj_t1->AddComponent<RailTriggerPoint>();
			cam->AddTriggerPoint(*t1);
			auto col1 = obj_t1->AddComponent<CubeCollider>();
			col1->SetTrigger(true);
			t1->SetCollider(col1);

			GameObject* obj_t2 = SceneManager::GetInstance().CreateGameObject(L"TriggerPoint2");
			auto t2 = obj_t2->AddComponent<RailTriggerPoint>();
			cam->AddTriggerPoint(*t2);
			auto col2 = obj_t2->AddComponent<CubeCollider>();
			col2->SetTrigger(true);
			t2->SetCollider(col2);

			GameObject* obj_t3 = SceneManager::GetInstance().CreateGameObject(L"TriggerPoint3");
			auto t3 = obj_t3->AddComponent<RailTriggerPoint>();
			cam->AddTriggerPoint(*t3);
			auto col3 = obj_t3->AddComponent<CubeCollider>();
			col3->SetTrigger(true);
			t3->SetCollider(col3);

			GameObject* obj_t4 = SceneManager::GetInstance().CreateGameObject(L"TriggerPoint4");
			auto t4 = obj_t4->AddComponent<RailTriggerPoint>();
			cam->AddTriggerPoint(*t4);
			auto col4 = obj_t4->AddComponent<CubeCollider>();
			col4->SetTrigger(true);
			t4->SetCollider(col4);

			GameObject* obj_t5 = SceneManager::GetInstance().CreateGameObject(L"TriggerPoint5");
			auto t5 = obj_t5->AddComponent<RailTriggerPoint>();
			cam->AddTriggerPoint(*t5);
			auto col5 = obj_t5->AddComponent<CubeCollider>();
			col5->SetTrigger(true);
			t5->SetCollider(col5);

			GameObject* obj_t6 = SceneManager::GetInstance().CreateGameObject(L"TriggerPoint6");
			auto t6 = obj_t6->AddComponent<RailTriggerPoint>();
			cam->AddTriggerPoint(*t6);
			auto col6 = obj_t6->AddComponent<CubeCollider>();
			col6->SetTrigger(true);
			t6->SetCollider(col6);

			GameObject* obj_t7 = SceneManager::GetInstance().CreateGameObject(L"TriggerPoint7");
			obj_t7->AddComponent<TriggerEventSetter>()->SetTriggerEnterEventLoadScene(L"GameSceneBossPhase3");
			auto t7 = obj_t7->AddComponent<RailTriggerPoint>();
			cam->AddTriggerPoint(*t7);
			auto col7 = obj_t7->AddComponent<CubeCollider>();
			col7->SetTrigger(true);
			t7->SetCollider(col7);

			cam->SetRangePlayTime(0, 0);
			cam->SetRangePlayTime(1, 0);
			cam->SetRangePlayTime(2, 0);
			cam->SetRangePlayTime(3, 0);
			cam->SetRangePlayTime(4, 0);
			cam->SetRangePlayTime(5, 0);
			cam->SetRangePlayTime(6, 0);

			cam->SetRangeDelayTime(0, 0);
			cam->SetRangeDelayTime(1, 0);
			cam->SetRangeDelayTime(2, 0);
			cam->SetRangeDelayTime(3, 0);
			cam->SetRangeDelayTime(4, 0);
			cam->SetRangeDelayTime(5, 0);
			cam->SetRangeDelayTime(6, 0);

			// 라이트
			GameObject* lightObject = SceneManager::GetInstance().CreateGameObject(L"DirectionalLight");
			DirectionalLight* directionalLight = lightObject->AddComponent<DirectionalLight>();
			lightObject->GetTransform()->SetLocalRotation(Vector3D(70, 30, 0));

			// 게임 매니저
			GameObject* gameManagerObject = SceneManager::GetInstance().CreateGameObject(L"GameManager");
			GameManager* gameManager = gameManagerObject->AddComponent<GameManager>();

			{
				GameObject* coverObject = SceneManager::GetInstance().CreateGameObject(L"CoverObject");
				coverObject->AddComponent<ZeldaUI>();
				Image* coverImage = coverObject->AddComponent<Image>();
				coverImage->SetLayer(901);
				coverObject->GetTransform()->SetLocalScale(Vector3D(1920, 1080));
				SpriteAnimator* mainanimator = coverObject->AddComponent<SpriteAnimator>();
				gameManager->SetMainAnimator(mainanimator);
			}

			{
				GameObject* coverObject = SceneManager::GetInstance().CreateGameObject(L"CoverObject");
				coverObject->AddComponent<ZeldaUI>();
				Image* coverImage = coverObject->AddComponent<Image>();
				coverImage->SetLayer(900);
				coverObject->GetTransform()->SetLocalScale(Vector3D(1920, 1080));
				SpriteAnimator* subanimator = coverObject->AddComponent<SpriteAnimator>();
				gameManager->SetSubAnimator(subanimator);
			}

			{
				GameObject* coverObject = SceneManager::GetInstance().CreateGameObject(L"CoverObject");
				coverObject->AddComponent<ZeldaUI>();
				Image* coverImage = coverObject->AddComponent<Image>();
				coverImage->SetLayer(899);
				coverObject->GetTransform()->SetLocalScale(Vector3D(1920, 1080));
				SpriteAnimator* subanimator = coverObject->AddComponent<SpriteAnimator>();
				gameManager->SetThirdAnimator(subanimator);
			}

			// 보스
			GameObject* Boss = CreateGameObjectFromFBX(L"Boss");
			Boss->GetTransform()->SetLocalPosition(Vector3D(0, 0, 30));
			Animator* bossAnim = Boss->AddComponent<Animator>();
			auto clip = GetAnimationClip(L"root|Phase3_Boss");
			clip.isLoop = true;
			bossAnim->PlusAnimationClip(GetAnimationClip(L"root|Phase3_Boss"));
			bossAnim->PlusAnimationClip(GetAnimationClip(L"root|000.Idle"));
			bossAnim->PlusAnimationClip(GetAnimationClip(L"root|001-1.Idle->Walk"));
			bossAnim->PlusAnimationClip(GetAnimationClip(L"root|001-2.Walk"));
			bossAnim->PlusAnimationClip(GetAnimationClip(L"root|001-3.Walk->Idle"));
			bossAnim->PlusAnimationClip(GetAnimationClip(L"root|002-1.NormalAttack_R"));
			bossAnim->PlusAnimationClip(GetAnimationClip(L"root|002-2.NormalAttack_R->Idle"));
			bossAnim->PlusAnimationClip(GetAnimationClip(L"root|003-1.NormalAttack_L"));
			bossAnim->PlusAnimationClip(GetAnimationClip(L"root|003-2.NormalAttack_L->Idle"));
			bossAnim->PlusAnimationClip(GetAnimationClip(L"root|004.RangedAttack"));
			bossAnim->PlusAnimationClip(GetAnimationClip(L"root|005.Teleport"));
			bossAnim->PlusAnimationClip(GetAnimationClip(L"root|006-1.Skill_Tree"));
			bossAnim->PlusAnimationClip(GetAnimationClip(L"root|006-2.Skill_Tree->Idle"));
			bossAnim->PlusAnimationClip(GetAnimationClip(L"root|007-1.Idle->Skill_ChargeAttack"));
			bossAnim->PlusAnimationClip(GetAnimationClip(L"root|007-2.Skill_ChargeAttack"));
			bossAnim->PlusAnimationClip(GetAnimationClip(L"root|007-3.Skill_ChargeAttack->Idle"));
			bossAnim->PlusAnimationClip(GetAnimationClip(L"root|008.Skill_Knockback"));
			bossAnim->PlusAnimationClip(GetAnimationClip(L"root|009.Skill_Blocking"));
			bossAnim->PlusAnimationClip(GetAnimationClip(L"root|010.Skill_2D"));
			bossAnim->PlusAnimationClip(GetAnimationClip(L"root|011-1.Groggy"));
			bossAnim->PlusAnimationClip(GetAnimationClip(L"root|011-2.Groogy->Idle"));
			bossAnim->PlusAnimationClip(GetAnimationClip(L"root|012.Death"));
			bossAnim->PlusAnimationClip(GetAnimationClip(L"root|PhaseTransition"));
			bossAnim->Play(L"root|Phase3_Boss");

			// 플레이어
			GameObject* player = CreateGameObjectFromFBX(L"Player");
			player->GetTransform()->SetLocalPosition(Vector3D(0, 0, 70));
			Animator* playerAnim = player->AddComponent<Animator>();
			FF::FFAnimationClip& phase3PlayerClip = GetAnimationClip(L"Armature|Phase3_Player");
			phase3PlayerClip.isLoop = true;
			playerAnim->PlusAnimationClip(GetAnimationClip(L"Armature|attack"));
			playerAnim->PlusAnimationClip(GetAnimationClip(L"Armature|attack2"));
			playerAnim->PlusAnimationClip(GetAnimationClip(L"Armature|damage"));
			playerAnim->PlusAnimationClip(GetAnimationClip(L"Armature|dash"));
			playerAnim->PlusAnimationClip(GetAnimationClip(L"Armature|down"));
			playerAnim->PlusAnimationClip(GetAnimationClip(L"Armature|idle"));
			playerAnim->PlusAnimationClip(GetAnimationClip(L"Armature|running"));
			playerAnim->PlusAnimationClip(GetAnimationClip(L"Armature|Phase3_Player"));
			playerAnim->Play(L"Armature|Phase3_Player");

			t0->SetBoss1(Boss);
			t0->SetPlayer(player);
			t1->SetBoss1(Boss);
			t1->SetPlayer(player);
			t2->SetBoss1(Boss);
			t2->SetPlayer(player);
			t3->SetBoss1(Boss);
			t3->SetPlayer(player);
			t4->SetBoss1(Boss);
			t4->SetPlayer(player);
			t5->SetBoss1(Boss);
			t5->SetPlayer(player);
			t6->SetBoss1(Boss);
			t6->SetPlayer(player);
			t7->SetBoss1(Boss);
			t7->SetPlayer(player);

			SaveScene();
		}

		SceneManager::GetInstance().DeleteAllGameObject();
		SceneManager::GetInstance().DeleteAllDontDestGameObject();

#pragma region 연출씬 2, 페이즈 2-3 사이 ver2
		{
			SceneManager::GetInstance().SetName(L"Directing Scene 2");

			GameObject* backGround0 = SceneManager::GetInstance().CreateGameObject(L"Cube0");
			auto b0 = backGround0->AddComponent<MeshRenderer>();
			b0->SetMesh(L"Cube");
			b0->SetMaterial(L"CubeMaterial_F");

			GameObject* backGround1 = SceneManager::GetInstance().CreateGameObject(L"Cube1");
			auto b1 = backGround1->AddComponent<MeshRenderer>();
			b1->SetMesh(L"Cube");
			b1->SetMaterial(L"CubeMaterial_F");

			GameObject* backGround2 = SceneManager::GetInstance().CreateGameObject(L"Cube2");
			auto b2 = backGround2->AddComponent<MeshRenderer>();
			b2->SetMesh(L"Cube");
			b2->SetMaterial(L"CubeMaterial_F");

			GameObject* backGround3 = SceneManager::GetInstance().CreateGameObject(L"Cube3");
			auto b3 = backGround3->AddComponent<MeshRenderer>();
			b3->SetMesh(L"Cube");
			b3->SetMaterial(L"CubeMaterial_F");

			GameObject* backGround4 = SceneManager::GetInstance().CreateGameObject(L"Cube4");
			auto b4 = backGround4->AddComponent<MeshRenderer>();
			b4->SetMesh(L"Cube");
			b4->SetMaterial(L"CubeMaterial_F");

			GameObject* backGround5 = SceneManager::GetInstance().CreateGameObject(L"Cube5");
			auto b5 = backGround5->AddComponent<MeshRenderer>();
			b5->SetMesh(L"Cube");
			b5->SetMaterial(L"CubeMaterial_F");

			// 카메라
			GameObject* cameraObject = SceneManager::GetInstance().CreateGameObject(L"MainCamera");
			cameraObject->GetTransform()->SetLocalPosition(Vector3D(-100, 0, 0));
			cameraObject->SetLayerID(1);
			Camera* camera = cameraObject->AddComponent<Camera>();
			auto trigger = cameraObject->AddComponent<CubeCollider>();
			trigger->SetTrigger(true);
			SceneManager::GetInstance().SetMainCamera(camera);
			RailMoveCamera* cam = cameraObject->AddComponent<RailMoveCamera>();
			cameraObject->AddComponent<Dotween>();

			GameObject* obj_r0 = SceneManager::GetInstance().CreateGameObject(L"RailPoint0");
			auto r0 = obj_r0->AddComponent<CameraRailPoint>();
			cam->AddRailPoint(*r0);

			GameObject* obj_r1 = SceneManager::GetInstance().CreateGameObject(L"RailPoint1");
			auto r1 = obj_r1->AddComponent<CameraRailPoint>();
			cam->AddRailPoint(*r1);

			GameObject* obj_r2 = SceneManager::GetInstance().CreateGameObject(L"RailPoint2");
			auto r2 = obj_r2->AddComponent<CameraRailPoint>();
			cam->AddRailPoint(*r2);

			GameObject* obj_r3 = SceneManager::GetInstance().CreateGameObject(L"RailPoint3");
			auto r3 = obj_r3->AddComponent<CameraRailPoint>();
			cam->AddRailPoint(*r3);

			GameObject* obj_r4 = SceneManager::GetInstance().CreateGameObject(L"RailPoint4");
			auto r4 = obj_r4->AddComponent<CameraRailPoint>();
			cam->AddRailPoint(*r4);

			GameObject* obj_r5 = SceneManager::GetInstance().CreateGameObject(L"RailPoint5");
			auto r5 = obj_r5->AddComponent<CameraRailPoint>();
			cam->AddRailPoint(*r5);

			GameObject* obj_r6 = SceneManager::GetInstance().CreateGameObject(L"RailPoint6");
			auto r6 = obj_r6->AddComponent<CameraRailPoint>();
			cam->AddRailPoint(*r6);

			GameObject* obj_r7 = SceneManager::GetInstance().CreateGameObject(L"RailPoint7");
			auto r7 = obj_r7->AddComponent<CameraRailPoint>();
			cam->AddRailPoint(*r7);

			GameObject* obj_t0 = SceneManager::GetInstance().CreateGameObject(L"TriggerPoint0");
			auto t0 = obj_t0->AddComponent<RailTriggerPoint>();
			cam->AddTriggerPoint(*t0);
			auto col0 = obj_t0->AddComponent<CubeCollider>();
			col0->SetTrigger(true);
			t0->SetCollider(col0);

			GameObject* obj_t1 = SceneManager::GetInstance().CreateGameObject(L"TriggerPoint1");
			auto t1 = obj_t1->AddComponent<RailTriggerPoint>();
			obj_t1->SetLayerID(14);
			cam->AddTriggerPoint(*t1);
			auto col1 = obj_t1->AddComponent<CubeCollider>();
			col1->SetTrigger(true);
			t1->SetCollider(col1);

			GameObject* obj_t2 = SceneManager::GetInstance().CreateGameObject(L"TriggerPoint2");
			auto t2 = obj_t2->AddComponent<RailTriggerPoint>();
			obj_t2->SetLayerID(14);
			cam->AddTriggerPoint(*t2);
			auto col2 = obj_t2->AddComponent<CubeCollider>();
			col2->SetTrigger(true);
			t2->SetCollider(col2);

			GameObject* obj_t3 = SceneManager::GetInstance().CreateGameObject(L"TriggerPoint3");
			auto t3 = obj_t3->AddComponent<RailTriggerPoint>();
			obj_t3->SetLayerID(14);
			cam->AddTriggerPoint(*t3);
			auto col3 = obj_t3->AddComponent<CubeCollider>();
			col3->SetTrigger(true);
			t3->SetCollider(col3);

			GameObject* obj_t4 = SceneManager::GetInstance().CreateGameObject(L"TriggerPoint4");
			auto t4 = obj_t4->AddComponent<RailTriggerPoint>();
			obj_t4->SetLayerID(14);
			cam->AddTriggerPoint(*t4);
			auto col4 = obj_t4->AddComponent<CubeCollider>();
			col4->SetTrigger(true);
			t4->SetCollider(col4);

			GameObject* obj_t5 = SceneManager::GetInstance().CreateGameObject(L"TriggerPoint5");
			auto t5 = obj_t5->AddComponent<RailTriggerPoint>();
			obj_t5->SetLayerID(14);
			cam->AddTriggerPoint(*t5);
			auto col5 = obj_t5->AddComponent<CubeCollider>();
			col5->SetTrigger(true);
			t5->SetCollider(col5);

			GameObject* obj_t6 = SceneManager::GetInstance().CreateGameObject(L"TriggerPoint6");
			auto t6 = obj_t6->AddComponent<RailTriggerPoint>();
			obj_t6->SetLayerID(14);
			cam->AddTriggerPoint(*t6);
			auto col6 = obj_t6->AddComponent<CubeCollider>();
			col6->SetTrigger(true);
			t6->SetCollider(col6);

			GameObject* obj_t7 = SceneManager::GetInstance().CreateGameObject(L"TriggerPoint7");
			auto t7 = obj_t7->AddComponent<RailTriggerPoint>();
			obj_t7->SetLayerID(14);
			cam->AddTriggerPoint(*t7);
			auto col7 = obj_t7->AddComponent<CubeCollider>();
			col7->SetTrigger(true);
			t7->SetCollider(col7);

			cam->SetRangePlayTime(0, 30);
			cam->SetRangePlayTime(1, 0);
			cam->SetRangePlayTime(2, 0);
			cam->SetRangePlayTime(3, 0);
			cam->SetRangePlayTime(4, 0);
			cam->SetRangePlayTime(5, 0);
			cam->SetRangePlayTime(6, 0);

			cam->SetRangeDelayTime(0, 0);
			cam->SetRangeDelayTime(1, 0);
			cam->SetRangeDelayTime(2, 0);
			cam->SetRangeDelayTime(3, 0);
			cam->SetRangeDelayTime(4, 0);
			cam->SetRangeDelayTime(5, 0);
			cam->SetRangeDelayTime(6, 0);

			// 라이트
			GameObject* lightObject = SceneManager::GetInstance().CreateGameObject(L"DirectionalLight");
			DirectionalLight* directionalLight = lightObject->AddComponent<DirectionalLight>();
			lightObject->GetTransform()->SetLocalRotation(Vector3D(70, 30, 0));

			// 게임 매니저
			GameObject* gameManagerObject = SceneManager::GetInstance().CreateGameObject(L"GameManager");
			GameManager* gameManager = gameManagerObject->AddComponent<GameManager>();

			{
				GameObject* coverObject = SceneManager::GetInstance().CreateGameObject(L"CoverObject");
				coverObject->AddComponent<ZeldaUI>();
				Image* coverImage = coverObject->AddComponent<Image>();
				coverImage->SetLayer(901);
				coverObject->GetTransform()->SetLocalScale(Vector3D(1920, 1080));
				SpriteAnimator* mainanimator = coverObject->AddComponent<SpriteAnimator>();
				gameManager->SetMainAnimator(mainanimator);
			}

			{
				GameObject* coverObject = SceneManager::GetInstance().CreateGameObject(L"CoverObject");
				coverObject->AddComponent<ZeldaUI>();
				Image* coverImage = coverObject->AddComponent<Image>();
				coverImage->SetLayer(900);
				coverObject->GetTransform()->SetLocalScale(Vector3D(1920, 1080));
				SpriteAnimator* subanimator = coverObject->AddComponent<SpriteAnimator>();
				gameManager->SetSubAnimator(subanimator);
			}

			{
				GameObject* coverObject = SceneManager::GetInstance().CreateGameObject(L"CoverObject");
				coverObject->AddComponent<ZeldaUI>();
				Image* coverImage = coverObject->AddComponent<Image>();
				coverImage->SetLayer(899);
				coverObject->GetTransform()->SetLocalScale(Vector3D(1920, 1080));
				SpriteAnimator* subanimator = coverObject->AddComponent<SpriteAnimator>();
				gameManager->SetThirdAnimator(subanimator);
			}

			// 보스
			GameObject* Boss = CreateGameObjectFromFBX(L"Boss");
			Boss->GetTransform()->SetLocalPosition(Vector3D(0, 0, 30));
			Boss->AddComponent<Phase3Character>()->SetSkinnedMeshRenderer(Boss->GetTransform()->GetChild(0)->GetGameObject()->GetComponent<SkinnedMeshRenderer>());
			Animator* bossAnim = Boss->AddComponent<Animator>();
			FF::FFAnimationClip& clip = GetAnimationClip(L"root|001-2.Walk");
			clip.isLoop = true;
			bossAnim->PlusAnimationClip(GetAnimationClip(L"root|Phase3_Boss"));
			bossAnim->PlusAnimationClip(GetAnimationClip(L"root|000.Idle"));
			bossAnim->PlusAnimationClip(GetAnimationClip(L"root|001-1.Idle->Walk"));
			bossAnim->PlusAnimationClip(GetAnimationClip(L"root|001-2.Walk"));
			bossAnim->PlusAnimationClip(GetAnimationClip(L"root|001-3.Walk->Idle"));
			bossAnim->PlusAnimationClip(GetAnimationClip(L"root|002-1.NormalAttack_R"));
			bossAnim->PlusAnimationClip(GetAnimationClip(L"root|002-2.NormalAttack_R->Idle"));
			bossAnim->PlusAnimationClip(GetAnimationClip(L"root|003-1.NormalAttack_L"));
			bossAnim->PlusAnimationClip(GetAnimationClip(L"root|003-2.NormalAttack_L->Idle"));
			bossAnim->PlusAnimationClip(GetAnimationClip(L"root|004.RangedAttack"));
			bossAnim->PlusAnimationClip(GetAnimationClip(L"root|005.Teleport"));
			bossAnim->PlusAnimationClip(GetAnimationClip(L"root|006-1.Skill_Tree"));
			bossAnim->PlusAnimationClip(GetAnimationClip(L"root|006-2.Skill_Tree->Idle"));
			bossAnim->PlusAnimationClip(GetAnimationClip(L"root|007-1.Idle->Skill_ChargeAttack"));
			bossAnim->PlusAnimationClip(GetAnimationClip(L"root|007-2.Skill_ChargeAttack"));
			bossAnim->PlusAnimationClip(GetAnimationClip(L"root|007-3.Skill_ChargeAttack->Idle"));
			bossAnim->PlusAnimationClip(GetAnimationClip(L"root|008.Skill_Knockback"));
			bossAnim->PlusAnimationClip(GetAnimationClip(L"root|009.Skill_Blocking"));
			bossAnim->PlusAnimationClip(GetAnimationClip(L"root|010.Skill_2D"));
			bossAnim->PlusAnimationClip(GetAnimationClip(L"root|011-1.Groggy"));
			bossAnim->PlusAnimationClip(GetAnimationClip(L"root|011-2.Groogy->Idle"));
			bossAnim->PlusAnimationClip(GetAnimationClip(L"root|012.Death"));
			bossAnim->PlusAnimationClip(GetAnimationClip(L"root|PhaseTransition"));
			bossAnim->Play(L"root|001-2.Walk");

			// 플레이어
			GameObject* player = CreateGameObjectFromFBX(L"Player");
			player->AddComponent<Phase3Character>()->SetSkinnedMeshRenderer(player->GetTransform()->GetChild(0)->GetGameObject()->GetComponent<SkinnedMeshRenderer>());
			player->GetTransform()->SetLocalPosition(Vector3D(0, 0, 70));
			Animator* playerAnim = player->AddComponent<Animator>();
			FF::FFAnimationClip& phase3PlayerClip = GetAnimationClip(L"Armature|running");
			phase3PlayerClip.isLoop = true;
			playerAnim->PlusAnimationClip(GetAnimationClip(L"Armature|attack"));
			playerAnim->PlusAnimationClip(GetAnimationClip(L"Armature|attack2"));
			playerAnim->PlusAnimationClip(GetAnimationClip(L"Armature|damage"));
			playerAnim->PlusAnimationClip(GetAnimationClip(L"Armature|dash"));
			playerAnim->PlusAnimationClip(GetAnimationClip(L"Armature|down"));
			playerAnim->PlusAnimationClip(GetAnimationClip(L"Armature|idle"));
			playerAnim->PlusAnimationClip(GetAnimationClip(L"Armature|running"));
			playerAnim->PlusAnimationClip(GetAnimationClip(L"Armature|Phase3_Player"));
			playerAnim->Play(L"Armature|running");

			t0->SetBoss1(Boss);
			t0->SetPlayer(player);
			t1->SetBoss1(Boss);
			t1->SetPlayer(player);
			t2->SetBoss1(Boss);
			t2->SetPlayer(player);
			t3->SetBoss1(Boss);
			t3->SetPlayer(player);
			t4->SetBoss1(Boss);
			t4->SetPlayer(player);
			t5->SetBoss1(Boss);
			t5->SetPlayer(player);
			t6->SetBoss1(Boss);
			t6->SetPlayer(player);
			t7->SetBoss1(Boss);
			t7->SetPlayer(player);

			SaveScene();
		}

#pragma endregion

		SceneManager::GetInstance().DeleteAllGameObject();
		SceneManager::GetInstance().DeleteAllDontDestGameObject();

		{
			SceneManager::GetInstance().SetName(L"GameScene");

			// 카메라
			GameObject* cameraObject = SceneManager::GetInstance().CreateGameObject(L"MainCamera");
			Camera* camera = cameraObject->AddComponent<Camera>();
			SceneManager::GetInstance().SetMainCamera(camera);

			// 게임 매니저
			GameObject* gameManagerObject = SceneManager::GetInstance().CreateGameObject(L"GameManager");
			GameManager* gameManager = gameManagerObject->AddComponent<GameManager>();

			{
				GameObject* coverObject = SceneManager::GetInstance().CreateGameObject(L"CoverObject");
				coverObject->AddComponent<ZeldaUI>();
				Image* coverImage = coverObject->AddComponent<Image>();
				coverImage->SetLayer(901);
				coverObject->GetTransform()->SetLocalScale(Vector3D(1920, 1080));
				SpriteAnimator* mainanimator = coverObject->AddComponent<SpriteAnimator>();
				gameManager->SetMainAnimator(mainanimator);
			}

			{
				GameObject* coverObject = SceneManager::GetInstance().CreateGameObject(L"CoverObject");
				coverObject->AddComponent<ZeldaUI>();
				Image* coverImage = coverObject->AddComponent<Image>();
				coverImage->SetLayer(900);
				coverObject->GetTransform()->SetLocalScale(Vector3D(1920, 1080));
				SpriteAnimator* subanimator = coverObject->AddComponent<SpriteAnimator>();
				gameManager->SetSubAnimator(subanimator);
			}

			{
				GameObject* coverObject = SceneManager::GetInstance().CreateGameObject(L"CoverObject");
				coverObject->AddComponent<ZeldaUI>();
				Image* coverImage = coverObject->AddComponent<Image>();
				coverImage->SetLayer(899);
				coverObject->GetTransform()->SetLocalScale(Vector3D(1920, 1080));
				SpriteAnimator* thirdanimator = coverObject->AddComponent<SpriteAnimator>();
				gameManager->SetThirdAnimator(thirdanimator);
			}

			{
				GameObject* backGround = SceneManager::GetInstance().CreateGameObject(L"BackGround");
				backGround->GetTransform()->SetLocalScale(Vector3D(312.5, 200));
				backGround->GetTransform()->SetLocalPosition(Vector3D(0, -240, -270));
				SpriteRenderer* backGroundRenderer = backGround->AddComponent<SpriteRenderer>();
				backGroundRenderer->SetTexture(L"BackGround");
			}

#pragma region Player
			Vector3D diffHeight = Vector3D(0, -1.5, 0);
			GameObject* testPlayer = SceneManager::GetInstance().CreateGameObject(L"testPlayer");
			GameObject* playerHitOjbect = SceneManager::GetInstance().CreateGameObject(L"playerHitOjbect");
			GameObject* player2DHitOjbect = SceneManager::GetInstance().CreateGameObject(L"player2DHitOjbect");
			GameObject* playerAnimator = CreateGameObjectFromFBX(L"Player");
			testPlayer->GetTransform()->AddChild(playerHitOjbect->GetTransform());
			testPlayer->GetTransform()->AddChild(player2DHitOjbect->GetTransform());
			testPlayer->GetTransform()->AddChild(playerAnimator->GetTransform());
			playerAnimator->GetTransform()->SetLocalPosition(diffHeight);


			Animator* playeranim = playerAnimator->AddComponent<Animator>();
			FF::FFAnimationClip& playerrunClip = GetAnimationClip(L"Armature|running");
			playerrunClip.isLoop = true;

			FF::FFAnimationClip& playeridleClip = GetAnimationClip(L"Armature|idle");
			playeridleClip.isLoop = true;


			playeranim->PlusAnimationClip(GetAnimationClip(L"Armature|attack"));
			playeranim->PlusAnimationClip(GetAnimationClip(L"Armature|attack2"));
			playeranim->PlusAnimationClip(GetAnimationClip(L"Armature|damage"));
			playeranim->PlusAnimationClip(GetAnimationClip(L"Armature|dash"));
			playeranim->PlusAnimationClip(GetAnimationClip(L"Armature|down"));
			playeranim->PlusAnimationClip(GetAnimationClip(L"Armature|idle"));
			playeranim->PlusAnimationClip(GetAnimationClip(L"Armature|running"));


			playeranim->Play(L"Armature|idle");

			CapsuleCollider* playerHitCollider = playerHitOjbect->AddComponent<CapsuleCollider>();
			playerHitCollider->SetTrigger(true);

			SkinnedMeshRenderer* playerSkinnedMeshRenderer = playerAnimator->GetTransform()->GetChild(0)->GetGameObject()->GetComponent<SkinnedMeshRenderer>();

			Player* playerComponent = testPlayer->AddComponent<Player>();
			playerComponent->SetGameManager(gameManager);
			playerComponent->SetAnimator(playeranim);
			playerComponent->SetSkinnedMeshRenderer(playerSkinnedMeshRenderer);
			CapsuleCollider* playerCollider = testPlayer->AddComponent<CapsuleCollider>();

			Collider2D* player2DHitCollider = player2DHitOjbect->AddComponent<Collider2D>();
			player2DHitCollider->SetShape(Collider2D::Box);

			EdgeDetect* playerEdge = player2DHitOjbect->AddComponent<EdgeDetect>();
			playerEdge->SetMeshRenderer(playerAnimator->GetTransform()->GetChild(0)->GetGameObject()->GetComponent<SkinnedMeshRenderer>());

			Player2DCollisionManager* Collision2DManagerComponent = player2DHitOjbect->AddComponent<Player2DCollisionManager>();
			Collision2DManagerComponent->SetPlayerComponent(playerComponent);

			Rigidbody* playerRigidBody = testPlayer->AddComponent<Rigidbody>();
			playerRigidBody->SetFreezeRotationX(true);
			playerRigidBody->SetFreezeRotationY(true);
			playerRigidBody->SetFreezeRotationZ(true);
			playerRigidBody->SetLinearDrag(0.7);
			playerComponent->SetRigidBody(playerRigidBody);
			testPlayer->SetLayerName(L"Player");
			playerHitOjbect->SetLayerName(L"Player");
			player2DHitOjbect->SetLayerName(L"player2DHitCollider");

			GameObject* PlayerDamaged = SceneManager::GetInstance().CreateGameObject(L"PlayerDamaged");
			PlayerDamaged->AddComponent<ZeldaUI>();
			PlayerDamaged->AddComponent<Image>();
			SpriteAnimator* PlayerSprite = PlayerDamaged->AddComponent<SpriteAnimator>();
			PlayerSprite->SetTexture(
				{
					L"PlayerDamaged_4",
					L"PlayerDamaged_3",
					L"PlayerDamaged_2",
					L"PlayerDamaged_1",
					L"PlayerDamaged_0",
				});
			PlayerSprite->SetPlaySpeed(30);
			PlayerSprite->SetClearAfterPlayEnds(true);
			playerComponent->SetPlayerDamagedSpriteAnimator(PlayerSprite);
			PlayerDamaged->GetTransform()->SetLocalScale(Vector3D(128, 128, 128));


			GameObject* VFX_PlayerAttack = CreateGameObjectFromFBX(L"VFX_PlayerAttack");
			VFX_PlayerAttack->GetTransform()->SetLocalPosition(Vector3D(0, 0, 0));
			VFX_PlayerAttack->GetTransform()->SetParent(testPlayer->GetTransform());
			playerComponent->SetPlayerAttackEffect(VFX_PlayerAttack);
			VFX_PlayerAttack->SetActive(false);
			EffectPlayer* playerAttackEffect = VFX_PlayerAttack->AddComponent<EffectPlayer>();
			playerAttackEffect->SetMeshRenderer(VFX_PlayerAttack->GetTransform()->GetChild(0)->GetGameObject()->GetComponent<MeshRenderer>());
			playerAttackEffect->SetMaxValue(1.f);
			playerAttackEffect->SetPlaySpeed(1.5f);
			playerAttackEffect->SetRemainTime(0.3f);
			playerAttackEffect->SetDirection(Vector2D(1, 0));

			// 특수공격 이펙트
			GameObject* VFX_PlayerSpecialAttack = CreateGameObjectFromFBX(L"VFX_PlayerSpecialAttack");
			VFX_PlayerSpecialAttack->GetTransform()->SetLocalPosition(Vector3D(0, 0, 0));
			VFX_PlayerSpecialAttack->GetTransform()->SetParent(testPlayer->GetTransform());
			playerComponent->SetPlayerSpecialAttackEffect(VFX_PlayerSpecialAttack);
			VFX_PlayerSpecialAttack->SetActive(false);
			EffectPlayer* playerSpecialAttackEffect = VFX_PlayerSpecialAttack->AddComponent<EffectPlayer>();
			playerSpecialAttackEffect->SetMeshRenderer(VFX_PlayerSpecialAttack->GetTransform()->GetChild(0)->GetGameObject()->GetComponent<MeshRenderer>());
			playerSpecialAttackEffect->SetMaxValue(1.f);
			playerSpecialAttackEffect->SetPlaySpeed(1.8f);
			playerSpecialAttackEffect->SetRemainTime(0.3f);
			playerSpecialAttackEffect->SetDirection(Vector2D(1, 0));

#pragma endregion

			GameObject* cameraobject = SceneManager::GetInstance().GetMainCamera()->GetGameObject();
			FollowCamera* followCamera = cameraobject->AddComponent<FollowCamera>();
			followCamera->SetPlayerTransform(testPlayer->GetTransform());

			GameObject* lightObject = SceneManager::GetInstance().CreateGameObject(L"GameObject004");
			DirectionalLight* directionalLight = lightObject->AddComponent<DirectionalLight>();
			lightObject->GetTransform()->SetLocalPosition(Vector3D(1000, 30, 0));
			lightObject->GetTransform()->SetLocalRotation(Vector3D(110, 25, 0));

			//GameObject* collsionTestObject001 = CreateGameObjectFromFBX(L"Mole");
			GameObject* boss1 = CreateGameObjectFromFBX(L"Boss");
			GameObject* boss1HitObject = SceneManager::GetInstance().CreateGameObject(L"Boss1HitObject");
			boss1->GetTransform()->AddChild(boss1HitObject->GetTransform());

			GameObject* boss2DHitOjbect = SceneManager::GetInstance().CreateGameObject(L"boss2DHitOjbect");
			boss2DHitOjbect->GetTransform()->SetParent(boss1->GetTransform());
			Collider2D* boss2DHitCollider = boss2DHitOjbect->AddComponent<Collider2D>();
			boss2DHitCollider->SetShape(Collider2D::Box);
			boss2DHitOjbect->SetLayerName(L"boss2DHitCollider");

			EdgeDetect* bossEdge = boss2DHitOjbect->AddComponent<EdgeDetect>();
			bossEdge->SetMeshRenderer(boss1->GetTransform()->GetChild(0)->GetGameObject()->GetComponent<SkinnedMeshRenderer>());

			CapsuleCollider* boss1HitCollider = boss1HitObject->AddComponent<CapsuleCollider>();
			boss1HitCollider->SetTrigger(true);

			Boss1* boss1Component = boss1->AddComponent<Boss1>();
			boss1Component->SetGameManager(gameManager);
			boss1Component->SetPlayerTransform(testPlayer->GetTransform());
			boss1Component->SetPlayerCollider(playerHitCollider);
			boss1Component->SetBossHitCollider(boss1HitCollider);
			boss1Component->SetPlayerGameObject(testPlayer);
			boss1Component->SetPlayerComponent(playerComponent);

			GameObject* boss1HitEffect = SceneManager::GetInstance().CreateGameObject(L"boss1HitEffect");
			boss1HitEffect->AddComponent<ZeldaUI>();
			boss1HitEffect->AddComponent<Image>();
			SpriteAnimator* bossHitSpriteAnimator = boss1HitEffect->AddComponent<SpriteAnimator>();
			bossHitSpriteAnimator->SetTexture({
				L"boss_damage_0",
				L"boss_damage_1",
				L"boss_damage_2",
				L"boss_damage_3",
				L"boss_damage_4",
				L"boss_damage_5",
				L"boss_damage_6"
				});
			bossHitSpriteAnimator->SetPlaySpeed(30.0f);
			bossHitSpriteAnimator->SetClearAfterPlayEnds(true);
			boss1Component->SetBossHitSpriteAnimator(bossHitSpriteAnimator);
			boss1HitEffect->GetTransform()->SetLocalScale(Vector3D(128, 128, 128));


			/// 나동
			boss1HitObject->SetLayerName(L"Boss1");
			boss1->SetLayerName(L"Boss1");

			//----------------------------------------------------------------------Animation----------------------------------------------------------------------
			Animator* anim = boss1->AddComponent<Animator>();
			FF::FFAnimationClip& idleClip = GetAnimationClip(L"root|000.Idle");
			idleClip.isLoop = true;

			FF::FFAnimationClip& chargeAttackClip = GetAnimationClip(L"root|007-2.Skill_ChargeAttack");
			chargeAttackClip.isLoop = true;

			FF::FFAnimationClip& walkClip = GetAnimationClip(L"root|001-2.Walk");
			walkClip.isLoop = true;

			anim->PlusAnimationClip(GetAnimationClip(L"root|000.Idle"));
			anim->PlusAnimationClip(GetAnimationClip(L"root|001-1.Idle->Walk"));
			anim->PlusAnimationClip(GetAnimationClip(L"root|001-2.Walk"));

			anim->PlusAnimationClip(GetAnimationClip(L"root|001-3.Walk->Idle"));

			anim->PlusAnimationClip(GetAnimationClip(L"root|002-1.NormalAttack_R"));
			anim->PlusAnimationClip(GetAnimationClip(L"root|002-2.NormalAttack_R->Idle"));

			anim->PlusAnimationClip(GetAnimationClip(L"root|003-1.NormalAttack_L"));
			anim->PlusAnimationClip(GetAnimationClip(L"root|003-2.NormalAttack_L->Idle"));

			anim->PlusAnimationClip(GetAnimationClip(L"root|004.RangedAttack"));

			anim->PlusAnimationClip(GetAnimationClip(L"root|005.Teleport"));

			anim->PlusAnimationClip(GetAnimationClip(L"root|006-1.Skill_Tree"));
			anim->PlusAnimationClip(GetAnimationClip(L"root|006-2.Skill_Tree->Idle"));

			anim->PlusAnimationClip(GetAnimationClip(L"root|007-1.Idle->Skill_ChargeAttack"));
			anim->PlusAnimationClip(GetAnimationClip(L"root|007-2.Skill_ChargeAttack"));

			anim->PlusAnimationClip(GetAnimationClip(L"root|007-3.Skill_ChargeAttack->Idle"));

			anim->PlusAnimationClip(GetAnimationClip(L"root|008.Skill_Knockback"));

			anim->PlusAnimationClip(GetAnimationClip(L"root|009.Skill_Blocking"));

			anim->PlusAnimationClip(GetAnimationClip(L"root|010.Skill_2D"));

			anim->PlusAnimationClip(GetAnimationClip(L"root|011-1.Groggy"));
			anim->PlusAnimationClip(GetAnimationClip(L"root|011-2.Groogy->Idle"));

			anim->PlusAnimationClip(GetAnimationClip(L"root|012.Death"));

			anim->PlusAnimationClip(GetAnimationClip(L"root|PhaseTransition"));

			anim->Play(L"root|000.Idle");

			CapsuleCollider* boss1Collider = boss1->AddComponent<CapsuleCollider>();
			//boss1Collider->SetTrigger(true);

			// y축 무조건 3으로 해야해요!!!!!!!!! 안하면 플레이어 그냥 떨어져 죽어요!!!!!!!!!
			// 제발!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			testPlayer->GetTransform()->SetLocalPosition(Vector3D(0, 3, 13));

			boss1->GetTransform()->SetLocalPosition(Vector3D(0, 4.0f, 1));
			boss1->GetTransform()->Rotate(boss1->GetTransform()->GetUp(), DegreeToRadian(180));

			/// 나동
			testPlayer->GetComponent<Player>()->SetBossCollider(boss1HitCollider);
			testPlayer->GetComponent<Player>()->SetBossTransform(boss1HitObject->GetTransform());
			testPlayer->GetComponent<Player>()->SetPlayerAnimTransform(playerAnimator->GetTransform());
			testPlayer->GetComponent<Player>()->SetBossAnimTransform(boss1->GetTransform());
			testPlayer->GetComponent<Player>()->SetBossComponent(boss1Component);

			// 족자 생성
			{
				GameObject* scrollObject = CreateGameObjectFromFBX(L"Scroll");
				FloatingObject* floatingComponent = scrollObject->AddComponent<FloatingObject>();
				floatingComponent->SetObjectTransform(scrollObject->GetTransform());
				scrollObject->GetTransform()->SetLocalPosition(Vector3D(25.0, 3.0, -11.0));
				scrollObject->GetTransform()->SetLocalRotation(Quaternion(0.8660253882408142, 0, -0.5, 0));
				boss1Component->SetScrollComponent(floatingComponent);
			}

			{
				GameObject* scrollObject = CreateGameObjectFromFBX(L"Scroll");
				FloatingObject* floatingComponent = scrollObject->AddComponent<FloatingObject>();
				floatingComponent->SetObjectTransform(scrollObject->GetTransform());
				scrollObject->GetTransform()->SetLocalPosition(Vector3D(-25.0, 3.0, -11.0));
				scrollObject->GetTransform()->SetLocalRotation(Quaternion(0.8660253882408142, 0, 0.5, 0));
				boss1Component->SetScrollComponent(floatingComponent);
			}

			{
				GameObject* scrollObject = CreateGameObjectFromFBX(L"Scroll");
				FloatingObject* floatingComponent = scrollObject->AddComponent<FloatingObject>();
				floatingComponent->SetObjectTransform(scrollObject->GetTransform());
				scrollObject->GetTransform()->SetLocalPosition(Vector3D(22.0, 3.0, -16.0));
				scrollObject->GetTransform()->SetLocalRotation(Quaternion(0.9396926164627075, 0, -0.3420201241970062, 0));
				boss1Component->SetScrollComponent(floatingComponent);
			}

			{
				GameObject* scrollObject = CreateGameObjectFromFBX(L"Scroll");
				FloatingObject* floatingComponent = scrollObject->AddComponent<FloatingObject>();
				floatingComponent->SetObjectTransform(scrollObject->GetTransform());
				scrollObject->GetTransform()->SetLocalPosition(Vector3D(-22.0, 3.0, -16.0));
				scrollObject->GetTransform()->SetLocalRotation(Quaternion(0.9396926164627075, 0, 0.3420201241970062, 0));
				boss1Component->SetScrollComponent(floatingComponent);
			}

			{
				GameObject* scrollObject = CreateGameObjectFromFBX(L"Scroll");
				FloatingObject* floatingComponent = scrollObject->AddComponent<FloatingObject>();
				floatingComponent->SetObjectTransform(scrollObject->GetTransform());
				scrollObject->GetTransform()->SetLocalPosition(Vector3D(18.0, 3.0, -20.0));
				scrollObject->GetTransform()->SetLocalRotation(Quaternion(0.9537169337272644, 0, -0.30070579051971436, 0));
				boss1Component->SetScrollComponent(floatingComponent);
			}

			{
				GameObject* scrollObject = CreateGameObjectFromFBX(L"Scroll");
				FloatingObject* floatingComponent = scrollObject->AddComponent<FloatingObject>();
				floatingComponent->SetObjectTransform(scrollObject->GetTransform());
				scrollObject->GetTransform()->SetLocalPosition(Vector3D(-18.0, 3.0, -20.0));
				scrollObject->GetTransform()->SetLocalRotation(Quaternion(0.9537169337272644, 0, 0.30070579051971436, 0));
				boss1Component->SetScrollComponent(floatingComponent);
			}

			{
				GameObject* scrollObject = CreateGameObjectFromFBX(L"Scroll");
				FloatingObject* floatingComponent = scrollObject->AddComponent<FloatingObject>();
				floatingComponent->SetObjectTransform(scrollObject->GetTransform());
				scrollObject->GetTransform()->SetLocalPosition(Vector3D(14.0, 3.0, -23.0));
				scrollObject->GetTransform()->SetLocalRotation(Quaternion(0.9790454506874084, 0, -0.20364175736904144, 0));
				boss1Component->SetScrollComponent(floatingComponent);
			}

			{
				GameObject* scrollObject = CreateGameObjectFromFBX(L"Scroll");
				FloatingObject* floatingComponent = scrollObject->AddComponent<FloatingObject>();
				floatingComponent->SetObjectTransform(scrollObject->GetTransform());
				scrollObject->GetTransform()->SetLocalPosition(Vector3D(-14.0, 3.0, -23.0));
				scrollObject->GetTransform()->SetLocalRotation(Quaternion(0.9790454506874084, 0, 0.20364175736904144, 0));
				boss1Component->SetScrollComponent(floatingComponent);
			}

			// 깃발 생성
			{
				GameObject* bossFlag = CreateGameObjectFromFBX(L"BossFlag");
				MeshRenderer* flagRenderer = bossFlag->GetTransform()->GetChild(0)->GetGameObject()->GetComponent<MeshRenderer>();
				flagRenderer->GetMaterialInfo(0).texture[1] = L"..\\Resources\\Texture\\BossFlag\\delete.jpg";
				flagRenderer->GetMaterialInfo(0).texture[2] = L"..\\Resources\\Texture\\BossFlag\\T_Flag_Diffuse_002.png";
				flagRenderer->GetMaterialInfo(0).color = ZeldaColor::White;
				bossFlag->GetTransform()->SetLocalPosition(Vector3D(-43.0, 10, 9));
				bossFlag->GetTransform()->SetLocalRotation(Quaternion(0.8526401519775391, 0, 0.5224985480308533, 0));

				BossFlag* flagComponent = bossFlag->AddComponent<BossFlag>();
				flagComponent->SetMeshRenderer(flagRenderer);
				flagComponent->SetPlaySpeed(0.000002);

				boss1Component->SetBossFlagComponent(flagComponent);
			}

			{
				GameObject* bossFlag = CreateGameObjectFromFBX(L"BossFlag");
				MeshRenderer* flagRenderer = bossFlag->GetTransform()->GetChild(0)->GetGameObject()->GetComponent<MeshRenderer>();
				flagRenderer->GetMaterialInfo(0).texture[1] = L"..\\Resources\\Texture\\BossFlag\\delete.jpg";
				flagRenderer->GetMaterialInfo(0).texture[2] = L"..\\Resources\\Texture\\BossFlag\\T_Flag_Diffuse_002.png";
				flagRenderer->GetMaterialInfo(0).color = ZeldaColor::White;
				bossFlag->GetTransform()->SetLocalPosition(Vector3D(-37.0, 12.0, -13.0));
				bossFlag->GetTransform()->SetLocalRotation(Quaternion(0.9455185532569885, 0, 0.32556813955307007, 0));

				BossFlag* flagComponent = bossFlag->AddComponent<BossFlag>();
				flagComponent->SetMeshRenderer(flagRenderer);
				flagComponent->SetPlaySpeed(0.000002);

				boss1Component->SetBossFlagComponent(flagComponent);
			}

			{
				GameObject* bossFlag = CreateGameObjectFromFBX(L"BossFlag");
				MeshRenderer* flagRenderer = bossFlag->GetTransform()->GetChild(0)->GetGameObject()->GetComponent<MeshRenderer>();
				flagRenderer->GetMaterialInfo(0).texture[1] = L"..\\Resources\\Texture\\BossFlag\\delete.jpg";
				flagRenderer->GetMaterialInfo(0).texture[2] = L"..\\Resources\\Texture\\BossFlag\\T_Flag_Diffuse_002.png";
				flagRenderer->GetMaterialInfo(0).color = ZeldaColor::White;
				bossFlag->GetTransform()->SetLocalPosition(Vector3D(-23.0, 16.0, -23.0));
				bossFlag->GetTransform()->SetLocalRotation(Quaternion(0.9876883625984192, 0, 0.15643447637557983, 0));

				BossFlag* flagComponent = bossFlag->AddComponent<BossFlag>();
				flagComponent->SetMeshRenderer(flagRenderer);
				flagComponent->SetPlaySpeed(0.000002);

				boss1Component->SetBossFlagComponent(flagComponent);
			}

			{
				GameObject* bossFlag = CreateGameObjectFromFBX(L"BossFlag");
				MeshRenderer* flagRenderer = bossFlag->GetTransform()->GetChild(0)->GetGameObject()->GetComponent<MeshRenderer>();
				flagRenderer->GetMaterialInfo(0).texture[1] = L"..\\Resources\\Texture\\BossFlag\\delete.jpg";
				flagRenderer->GetMaterialInfo(0).texture[2] = L"..\\Resources\\Texture\\BossFlag\\T_Flag_Diffuse_002.png";
				flagRenderer->GetMaterialInfo(0).color = ZeldaColor::White;
				bossFlag->GetTransform()->SetLocalPosition(Vector3D(23.0, 16.0, -23.0));
				bossFlag->GetTransform()->SetLocalRotation(Quaternion(0.9876883625984192, 0, -0.15643447637557983, 0));

				BossFlag* flagComponent = bossFlag->AddComponent<BossFlag>();
				flagComponent->SetMeshRenderer(flagRenderer);
				flagComponent->SetPlaySpeed(0.000002);

				boss1Component->SetBossFlagComponent(flagComponent);
			}

			{
				GameObject* bossFlag = CreateGameObjectFromFBX(L"BossFlag");
				MeshRenderer* flagRenderer = bossFlag->GetTransform()->GetChild(0)->GetGameObject()->GetComponent<MeshRenderer>();
				flagRenderer->GetMaterialInfo(0).texture[1] = L"..\\Resources\\Texture\\BossFlag\\delete.jpg";
				flagRenderer->GetMaterialInfo(0).texture[2] = L"..\\Resources\\Texture\\BossFlag\\T_Flag_Diffuse_002.png";
				flagRenderer->GetMaterialInfo(0).color = ZeldaColor::White;
				bossFlag->GetTransform()->SetLocalPosition(Vector3D(37.0, 12.0, -13.0));
				bossFlag->GetTransform()->SetLocalRotation(Quaternion(0.9455185532569885, 0, -0.32556813955307007, 0));

				BossFlag* flagComponent = bossFlag->AddComponent<BossFlag>();
				flagComponent->SetMeshRenderer(flagRenderer);
				flagComponent->SetPlaySpeed(0.000002);

				boss1Component->SetBossFlagComponent(flagComponent);
			}

			{
				GameObject* bossFlag = CreateGameObjectFromFBX(L"BossFlag");
				MeshRenderer* flagRenderer = bossFlag->GetTransform()->GetChild(0)->GetGameObject()->GetComponent<MeshRenderer>();
				flagRenderer->GetMaterialInfo(0).texture[1] = L"..\\Resources\\Texture\\BossFlag\\delete.jpg";
				flagRenderer->GetMaterialInfo(0).texture[2] = L"..\\Resources\\Texture\\BossFlag\\T_Flag_Diffuse_002.png";
				flagRenderer->GetMaterialInfo(0).color = ZeldaColor::White;
				bossFlag->GetTransform()->SetLocalPosition(Vector3D(43.0, 10, 9));
				bossFlag->GetTransform()->SetLocalRotation(Quaternion(0.8526401519775391, 0, -0.5224985480308533, 0));

				BossFlag* flagComponent = bossFlag->AddComponent<BossFlag>();
				flagComponent->SetMeshRenderer(flagRenderer);
				flagComponent->SetPlaySpeed(0.000002);

				boss1Component->SetBossFlagComponent(flagComponent);
			}

			// followCamera 컴포넌트 등록
			boss1Component->SetFollowCameraComponent(followCamera);


#pragma region floor
			/// BossFloor
			GameObject* BossFloor = CreateGameObjectFromFBX(L"BossFloor");
			GameObject* BossFloorMesh = BossFloor->GetTransform()->GetChild(0)->GetGameObject();
			MeshRenderer* renderer = BossFloorMesh->GetComponent<MeshRenderer>();
			renderer->GetMaterialInfo(1).shader = L"..\\Resources\\Shader\\BossFloor.hlsli";

			GameObject* child = SceneManager::GetInstance().CreateGameObject(L"child");
			child->SetLayerName(L"Floor");
			BossFloor->GetTransform()->AddChild(child->GetTransform());
			child->GetTransform()->SetLocalScale(Vector3D(50, 1, 50));
			Rigidbody* rigid = child->AddComponent<Rigidbody>();
			CylinderCollider* collider = child->AddComponent<CylinderCollider>();
			rigid->SetKinematic(true);
			rigid->SetRestitution(0.0f);
			rigid->SetKfriction(0.9f);
			rigid->SetSfriction(0.95f);
#pragma endregion

#pragma region door
			GameObject* door = CreateGameObjectFromFBX(L"BossDoor");
			door->GetTransform()->SetLocalPosition(Vector3D(0, -14, -37));

			GameObject* hand = CreateGameObjectFromFBX(L"BossDoorHand");

			hand->GetTransform()->SetParent(door->GetTransform());
			Animator* animator = hand->AddComponent<Animator>();
			FF::FFAnimationClip& clip = GetAnimationClip(L"Hand_rig|Idle");
			clip.isLoop = true;
			animator->PlusAnimationClip(GetAnimationClip(L"Hand_rig|Idle"));
			animator->Play(L"Hand_rig|Idle");
#pragma endregion

			/// 플레이어 없으면 UI 에서 터지게 되어 있어서 일단 플레이어 넣음
			GameObject* playerObj = testPlayer;
			Player* player = playerComponent;


			// 인게임 UI
			{
				// 체력바
				{
					// 좌측 하단 기준 offset
					Vector2D UISize = { 470, 80 };

					GameObject* object = SceneManager::GetInstance().CreateGameObject(L"HP_UI");
					ZeldaUI* ui = object->AddComponent<ZeldaUI>();
					object->GetTransform()->SetLocalPosition(Vector3D(-675.00, -458.20));
					HPUI* hpUI = object->AddComponent<HPUI>();
					hpUI->SetUISize(UISize);

					// Base
					GameObject* baseUIobj = SceneManager::GetInstance().CreateGameObject(L"HP_Base");
					baseUIobj->GetTransform()->SetParent(object->GetTransform());
					baseUIobj->GetTransform()->SetLocalScale(Vector3D(UISize.x, UISize.y));
					auto bu = baseUIobj->AddComponent<BaseUI>();
					ZeldaUI* b_ui = baseUIobj->AddComponent<ZeldaUI>();
					Image* b_image = baseUIobj->AddComponent<Image>();
					b_image->SetTexture(L"HPBarBase");
					b_image->SetLayer(0);

					// Move
					GameObject* moveUIobj = SceneManager::GetInstance().CreateGameObject(L"HP_Move");
					moveUIobj->GetTransform()->SetParent(object->GetTransform());
					auto mu = moveUIobj->AddComponent<MovableUI>();
					ZeldaUI* m_ui = moveUIobj->AddComponent<ZeldaUI>();
					Image* m_image = moveUIobj->AddComponent<Image>();
					m_image->SetTexture(L"HPBar");
					m_image->SetLayer(10);
					moveUIobj->SetActive(false);

					// Cover
					GameObject* coverUIobj = SceneManager::GetInstance().CreateGameObject(L"HP_Cover");
					coverUIobj->GetTransform()->SetParent(object->GetTransform());
					coverUIobj->GetTransform()->SetLocalScale(Vector3D(UISize.x, UISize.y));
					auto cu = coverUIobj->AddComponent<CoverUI>();
					ZeldaUI* c_ui = coverUIobj->AddComponent<ZeldaUI>();
					Image* c_image = coverUIobj->AddComponent<Image>();
					c_image->SetTexture(L"HPBarCover");
					c_image->SetLayer(20);

					// UI 장착
					hpUI->SetBaseUI(*bu);
					hpUI->SetMovableUI(*mu);
					hpUI->SetCoverUI(*cu);
					hpUI->SetPlayer(*player);
				}

				// 쿨타임 UI
				{
					GameObject* CoolTimeUIGroup = SceneManager::GetInstance().CreateGameObject(L"CoolTimeUIGroup");
					ZeldaUI* ui = CoolTimeUIGroup->AddComponent<ZeldaUI>();
					CoolTimeUIGroup->GetTransform()->SetLocalPosition(Vector3D(-675.00f, -400.40f));

					// 대시 스킬 UI
					{
						Vector2D UISize = { 470, 22 };

						GameObject* object = SceneManager::GetInstance().CreateGameObject(L"DashUI");
						object->GetTransform()->SetParent(CoolTimeUIGroup->GetTransform());
						object->GetTransform()->SetLocalScale(Vector3D(UISize.x, UISize.y));
						CoolTimeUI* ui = object->AddComponent<DashCoolTimeUI>();
						ui->SetUISize(UISize);

						// Base
						GameObject* baseUIobj = SceneManager::GetInstance().CreateGameObject(L"Dash_Base");
						baseUIobj->GetTransform()->SetParent(object->GetTransform());
						baseUIobj->GetTransform()->SetLocalScale(Vector3D(UISize.x, UISize.y));
						auto bu = baseUIobj->AddComponent<BaseUI>();
						ZeldaUI* b_ui = baseUIobj->AddComponent<ZeldaUI>();
						Image* b_image = baseUIobj->AddComponent<Image>();
						b_image->SetTexture(L"DashBase");
						b_image->SetLayer(0);

						// Move
						GameObject* moveUIobj = SceneManager::GetInstance().CreateGameObject(L"Dash_Move");
						moveUIobj->GetTransform()->SetParent(object->GetTransform());
						auto mu = moveUIobj->AddComponent<MovableUI>();
						ZeldaUI* m_ui = moveUIobj->AddComponent<ZeldaUI>();
						Image* m_image = moveUIobj->AddComponent<Image>();
						m_image->SetTexture(L"DashMoveCover");
						m_image->SetLayer(10);
						moveUIobj->SetActive(false);

						// Num
						GameObject* numUIobj = SceneManager::GetInstance().CreateGameObject(L"Dash_Num");
						numUIobj->GetTransform()->SetParent(object->GetTransform());
						numUIobj->GetTransform()->SetLocalScale(Vector3D(UISize.x, UISize.y));
						auto nu = numUIobj->AddComponent<NumUI>();
						ZeldaUI* n_ui = numUIobj->AddComponent<ZeldaUI>();
						Image* n_image = numUIobj->AddComponent<Image>();
						n_image->SetTexture(L"None");
						n_image->SetLayer(20);

						// Cover
						GameObject* coverUIobj = SceneManager::GetInstance().CreateGameObject(L"Dash_Cover");
						coverUIobj->GetTransform()->SetParent(object->GetTransform());
						coverUIobj->GetTransform()->SetLocalScale(Vector3D(UISize.x, UISize.y));
						auto cu = coverUIobj->AddComponent<CoverUI>();
						ZeldaUI* c_ui = coverUIobj->AddComponent<ZeldaUI>();
						Image* c_image = coverUIobj->AddComponent<Image>();
						c_image->SetTexture(L"DashCover");
						c_image->SetLayer(30);

						// UI 장착
						ui->SetBaseUI(*bu);
						ui->SetMovableUI(*mu);
						ui->SetNumUI(*nu);
						ui->SetCoverUI(*cu);
						ui->SetTarget(*playerObj);
					}

					// 원거리 스킬 UI
					{
						Vector2D UISize = { 115, 115 };

						GameObject* object = SceneManager::GetInstance().CreateGameObject(L"StandoffUI");
						object->GetTransform()->SetParent(CoolTimeUIGroup->GetTransform());
						object->GetTransform()->SetLocalScale(Vector3D(UISize.x, UISize.y));
						object->GetTransform()->SetLocalPosition(Vector3D(298.8, -40.6));
						CoolTimeUI* ui = object->AddComponent<StandoffCoolTimeUI>();
						ui->SetUISize(UISize);

						// Base
						GameObject* baseUIobj = SceneManager::GetInstance().CreateGameObject(L"Standoff_Base");
						baseUIobj->GetTransform()->SetParent(object->GetTransform());
						baseUIobj->GetTransform()->SetLocalScale(Vector3D(UISize.x, UISize.y));
						auto bu = baseUIobj->AddComponent<BaseUI>();
						ZeldaUI* b_ui = baseUIobj->AddComponent<ZeldaUI>();
						Image* b_image = baseUIobj->AddComponent<Image>();
						b_image->SetTexture(L"StandoffBase");
						b_image->SetLayer(0);

						// Move
						GameObject* moveUIobj = SceneManager::GetInstance().CreateGameObject(L"Standoff_Move");
						moveUIobj->GetTransform()->SetParent(object->GetTransform());
						auto mu = moveUIobj->AddComponent<MovableUI>();
						ZeldaUI* m_ui = moveUIobj->AddComponent<ZeldaUI>();
						Image* m_image = moveUIobj->AddComponent<Image>();
						m_image->SetTexture(L"StandoffMoveCover");
						m_image->SetLayer(10);
						moveUIobj->SetActive(false);

						// Num
						GameObject* numUIobj = SceneManager::GetInstance().CreateGameObject(L"Standoff_Num");
						numUIobj->GetTransform()->SetParent(object->GetTransform());
						numUIobj->GetTransform()->SetLocalScale(Vector3D(UISize.x, UISize.y));
						auto nu = numUIobj->AddComponent<NumUI>();
						ZeldaUI* n_ui = numUIobj->AddComponent<ZeldaUI>();
						Image* n_image = numUIobj->AddComponent<Image>();
						n_image->SetTexture(L"None");
						n_image->SetLayer(20);

						// Cover
						GameObject* coverUIobj = SceneManager::GetInstance().CreateGameObject(L"Standoff_Cover");
						coverUIobj->GetTransform()->SetParent(object->GetTransform());
						coverUIobj->GetTransform()->SetLocalScale(Vector3D(UISize.x, UISize.y));
						auto cu = coverUIobj->AddComponent<CoverUI>();
						ZeldaUI* c_ui = coverUIobj->AddComponent<ZeldaUI>();
						Image* c_image = coverUIobj->AddComponent<Image>();
						c_image->SetTexture(L"StandoffCover");
						c_image->SetLayer(30);

						// UI 장착
						ui->SetBaseUI(*bu);
						ui->SetMovableUI(*mu);
						ui->SetNumUI(*nu);
						ui->SetCoverUI(*cu);
						ui->SetTarget(*playerObj);
					}
				}

				// 설정 UI
				{
					/// Button
					// 우측 상단 기준 offset
					Vector2D buttonSize = { 87, 93 };

					GameObject* object = SceneManager::GetInstance().CreateGameObject(L"SettingUI");
					object->GetTransform()->SetLocalScale(Vector3D(buttonSize.x, buttonSize.y, 0));
					object->GetTransform()->SetLocalPosition(Vector3D(845.10f, 444.50f));
					SettingButton* setbt = object->AddComponent<SettingButton>();
					ZeldaUI* ui = object->AddComponent<ZeldaUI>();
					Image* image = object->AddComponent<Image>();
					image->SetTexture(L"SettingUI_Idle");
					Button* button = object->AddComponent<Button>();
					//button->SetImage(L"SettingUI_Idle", L"SettingUI_Enter", L"SettingUI_Click");

					// Panel
					Vector2D backgroundSize = { 1920, 1080 };

					GameObject* settingObj = SceneManager::GetInstance().CreateGameObject(L"SettingGroup");
					settingObj->GetTransform()->SetLocalScale(Vector3D(backgroundSize.x, backgroundSize.y, 0));
					ZeldaUI* setting_background = settingObj->AddComponent<ZeldaUI>();
					Image* background_image = settingObj->AddComponent<Image>();
					background_image->SetTexture(L"BlackCoverBackground");
					background_image->SetLayer(900);
					settingObj->SetActive(false);

					// 족자 이미지
					{
						Vector2D size = { 617, 846 };

						GameObject* obj = SceneManager::GetInstance().CreateGameObject(L"Panel");
						obj->GetTransform()->SetParent(settingObj->GetTransform());
						obj->GetTransform()->SetLocalScale(Vector3D(size.x, size.y, 0));
						obj->GetTransform()->SetLocalPosition(Vector3D(620.30, 73.50, 0));
						ZeldaUI* ui = obj->AddComponent<ZeldaUI>();
						Image* image = obj->AddComponent<Image>();
						image->SetTexture(L"SettingBackground");
						image->SetLayer(910);
					}

					// SoundBar
					{
						Vector2D size = { 285, 25 };

						GameObject* obj = SceneManager::GetInstance().CreateGameObject(L"Obj_SoundBar");
						obj->GetTransform()->SetParent(settingObj->GetTransform());
						obj->GetTransform()->SetLocalScale(Vector3D(size.x, size.y, 0));
						obj->GetTransform()->SetLocalPosition(Vector3D(626.7, 300, 0));
						SoundBar* sb = obj->AddComponent<SoundBar>();
						obj->AddComponent<ScrollUI>();
						ZeldaUI* ui = obj->AddComponent<ZeldaUI>();
						Image* image = obj->AddComponent<Image>();
						image->SetTexture(L"ScrollUI");
						image->SetLayer(920);
						sb->SetUISize(size);
						sb->SetPadding(20);

						// Handle
						{
							Vector2D hsize = { 30, 30 };

							GameObject* hobj = SceneManager::GetInstance().CreateGameObject(L"SoundBar_Handle");
							hobj->GetTransform()->SetParent(obj->GetTransform());
							hobj->GetTransform()->SetLocalScale(Vector3D(hsize.x, hsize.y, 0));
							hobj->SetActive(false);
							SoundHandle* sh = hobj->AddComponent<SoundHandle>();
							ZeldaUI* ui = hobj->AddComponent<ZeldaUI>();
							Image* image = hobj->AddComponent<Image>();
							image->SetTexture(L"SoundHandle");
							image->SetLayer(950);

							sb->SetHandle(*sh);
						}

						// SoundBarText
						{
							Vector2D textSize = { 112, 47.7f };

							GameObject* soundText = SceneManager::GetInstance().CreateGameObject(L"Obj_SoundText");
							soundText->GetTransform()->SetParent(obj->GetTransform());
							soundText->GetTransform()->SetLocalScale(Vector3D(textSize.x, textSize.y, 0));
							soundText->GetTransform()->SetLocalPosition(Vector3D(-88.6, 35.8, 0));
							ZeldaUI* soundText_ui = soundText->AddComponent<ZeldaUI>();
							Image* soundText_texture = soundText->AddComponent<Image>();
							soundText_texture->SetTexture(L"SoundText_Ingame_Idle");
							soundText_texture->SetLayer(920);
						}
					}

					// 키보드 입력에 따른 버튼 조정

					GameObject* bs = SceneManager::GetInstance().CreateGameObject(L"SettingButtonSelector");
					bs->GetTransform()->SetParent(settingObj->GetTransform());
					ButtonSelector* bsp = bs->AddComponent<ButtonSelector>();

					// Continue Button
					{
						Vector2D size = { 260, 80 };

						GameObject* obj = SceneManager::GetInstance().CreateGameObject(L"Setting_ContinueButton");
						obj->GetTransform()->SetParent(settingObj->GetTransform());
						obj->GetTransform()->SetLocalScale(Vector3D(size.x, size.y, 0));
						obj->GetTransform()->SetLocalPosition(Vector3D(624, 170, 0));
						ContinueButton* sb = obj->AddComponent<ContinueButton>();
						ZeldaUI* ui = obj->AddComponent<ZeldaUI>();
						Image* image = obj->AddComponent<Image>();
						image->SetTexture(L"Continue_Idle");
						image->SetLayer(920);
						Button* button = obj->AddComponent<Button>();
						//button->SetImage(L"Continue_Idle", L"Continue_Enter", L"Continue_Click");
						sb->SetSettingButton(*setbt);

						// Attach UI
						{
							Vector2D uiSize = { 70, 70 };

							GameObject* attachUI = SceneManager::GetInstance().CreateGameObject(L"Exit_AttachUI");
							attachUI->GetTransform()->SetParent(obj->GetTransform());
							attachUI->GetTransform()->SetLocalScale(Vector3D(uiSize.x, uiSize.y));
							attachUI->GetTransform()->SetLocalPosition(Vector3D(-157.30, 0, 0));
							AttachUI* atp = attachUI->AddComponent<AttachUI>();
							ZeldaUI* ui = attachUI->AddComponent<ZeldaUI>();
							Image* image = attachUI->AddComponent<Image>();
							image->SetTexture(L"AttachUI");
							image->SetLayer(950);

							sb->SetAttachUI(*atp);
							sb->SetUISize(size);
							sb->SetAttachUISize(uiSize);

							attachUI->SetActive(false);
						}

						bsp->AddButton(*obj);
						bsp->SetFirstButton(*obj);
					}

					// Exit Button
					{
						Vector2D size = { 252.7, 74.3 };

						GameObject* obj = SceneManager::GetInstance().CreateGameObject(L"Setting_ExitButton");
						obj->GetTransform()->SetParent(settingObj->GetTransform());
						obj->GetTransform()->SetLocalScale(Vector3D(size.x, size.y, 0));
						obj->GetTransform()->SetLocalPosition(Vector3D(624, 40, 0));
						ExitButton* eb = obj->AddComponent<ExitButton>();
						ZeldaUI* ui = obj->AddComponent<ZeldaUI>();
						Image* image = obj->AddComponent<Image>();
						image->SetTexture(L"Exit_Ingame_Idle");
						image->SetLayer(920);
						Button* button = obj->AddComponent<Button>();
						//button->SetImage(L"Exit_Idle", L"Exit_Enter", L"Exit_Click");

						// Attach UI
						{
							Vector2D uiSize = { 70, 70 };

							GameObject* attachUI = SceneManager::GetInstance().CreateGameObject(L"Exit_AttachUI");
							attachUI->GetTransform()->SetParent(obj->GetTransform());
							attachUI->GetTransform()->SetLocalScale(Vector3D(uiSize.x, uiSize.y));
							attachUI->GetTransform()->SetLocalPosition(Vector3D(-157.30, 0, 0));
							AttachUI* atp = attachUI->AddComponent<AttachUI>();
							ZeldaUI* ui = attachUI->AddComponent<ZeldaUI>();
							Image* image = attachUI->AddComponent<Image>();
							image->SetTexture(L"AttachUI");
							image->SetLayer(950);

							eb->SetAttachUI(*atp);
							eb->SetUISize(size);
							eb->SetAttachUISize(uiSize);

							attachUI->SetActive(false);
						}

						bsp->AddButton(*obj);
					}

					// Title Button
					{
						Vector2D size = { 250.2, 86 };

						GameObject* obj = SceneManager::GetInstance().CreateGameObject(L"Setting_TitleButton");
						obj->GetTransform()->SetParent(settingObj->GetTransform());
						obj->GetTransform()->SetLocalScale(Vector3D(size.x, size.y, 0));
						obj->GetTransform()->SetLocalPosition(Vector3D(624, -90, 0));
						TitleButton* tb = obj->AddComponent <TitleButton>();
						ZeldaUI* ui = obj->AddComponent<ZeldaUI>();
						Image* image = obj->AddComponent<Image>();
						image->SetTexture(L"Title_Idle");
						image->SetLayer(920);
						Button* button = obj->AddComponent<Button>();
						//button->SetImage(L"Title_Idle", L"Title_Enter", L"Title_Click");

						// Attach UI
						{
							Vector2D uiSize = { 70, 70 };

							GameObject* attachUI = SceneManager::GetInstance().CreateGameObject(L"Exit_AttachUI");
							attachUI->GetTransform()->SetParent(obj->GetTransform());
							attachUI->GetTransform()->SetLocalScale(Vector3D(uiSize.x, uiSize.y));
							attachUI->GetTransform()->SetLocalPosition(Vector3D(-157.30, 0, 0));
							AttachUI* atp = attachUI->AddComponent<AttachUI>();
							ZeldaUI* ui = attachUI->AddComponent<ZeldaUI>();
							Image* image = attachUI->AddComponent<Image>();
							image->SetTexture(L"AttachUI");
							image->SetLayer(950);

							tb->SetAttachUI(*atp);
							tb->SetUISize(size);
							tb->SetAttachUISize(uiSize);

							attachUI->SetActive(false);
						}

						bsp->AddButton(*obj);
					}

					setbt->SetPanel(*settingObj->GetTransform());
				}
			}

			SaveScene();


			SceneManager::GetInstance().DeleteAllGameObject();
			SceneManager::GetInstance().DeleteAllDontDestGameObject();

			{
				SceneManager::GetInstance().SetName(L"GameSceneBossPhase3");

				// 게임 매니저
				GameObject* gameManagerObject = SceneManager::GetInstance().CreateGameObject(L"GameManager");
				GameManager* gameManager = gameManagerObject->AddComponent<GameManager>();
				{
					GameObject* coverObject = SceneManager::GetInstance().CreateGameObject(L"CoverObject");
					coverObject->AddComponent<ZeldaUI>();
					Image* coverImage = coverObject->AddComponent<Image>();
					coverImage->SetLayer(901);
					coverObject->GetTransform()->SetLocalScale(Vector3D(1920, 1080));
					SpriteAnimator* mainanimator = coverObject->AddComponent<SpriteAnimator>();
					gameManager->SetMainAnimator(mainanimator);
				}
				{
					GameObject* coverObject = SceneManager::GetInstance().CreateGameObject(L"CoverObject");
					coverObject->AddComponent<ZeldaUI>();
					Image* coverImage = coverObject->AddComponent<Image>();
					coverImage->SetLayer(900);
					coverObject->GetTransform()->SetLocalScale(Vector3D(1920, 1080));
					SpriteAnimator* subanimator = coverObject->AddComponent<SpriteAnimator>();
					gameManager->SetSubAnimator(subanimator);
				}
				{
					GameObject* coverObject = SceneManager::GetInstance().CreateGameObject(L"CoverObject");
					coverObject->AddComponent<ZeldaUI>();
					Image* coverImage = coverObject->AddComponent<Image>();
					coverImage->SetLayer(899);
					coverObject->GetTransform()->SetLocalScale(Vector3D(1920, 1080));
					SpriteAnimator* thirdanimator = coverObject->AddComponent<SpriteAnimator>();
					gameManager->SetThirdAnimator(thirdanimator);
				}

				// 카메라
				GameObject* cameraObject = SceneManager::GetInstance().CreateGameObject(L"MainCamera");
				cameraObject->GetTransform()->SetLocalPosition(Vector3D(0, 0, 10));
				Camera* camera = cameraObject->AddComponent<Camera>();
				SceneManager::GetInstance().SetMainCamera(camera);

				TestComponent* testcomponent = SceneManager::GetInstance().GetMainCamera()->GetGameObject()->AddComponent<TestComponent>();

				// 라이트
				GameObject* lightObject = SceneManager::GetInstance().CreateGameObject(L"DirectionalLight");
				DirectionalLight* directionalLight = lightObject->AddComponent<DirectionalLight>();
				lightObject->GetTransform()->SetLocalRotation(Vector3D(70, 30, 0));

				GameObject* phase3EventManagerObject = SceneManager::GetInstance().CreateGameObject(L"Phase3EventManager");
				Phase3EventManager* phase3EventManager = phase3EventManagerObject->AddComponent<Phase3EventManager>();
				phase3EventManager->SetGameManager(gameManager);

				// 보스
				GameObject* Boss = CreateGameObjectFromFBX(L"Boss");
				Boss->AddComponent<Phase3Character>()->SetSkinnedMeshRenderer(Boss->GetTransform()->GetChild(0)->GetGameObject()->GetComponent<SkinnedMeshRenderer>());
				Animator* bossAnim = Boss->AddComponent<Animator>();
				FF::FFAnimationClip& phase3BossClip = GetAnimationClip(L"root|Phase3_Boss");
				phase3BossClip.isLoop = true;
				bossAnim->PlusAnimationClip(GetAnimationClip(L"root|Phase3_Boss"));
				bossAnim->Play(L"root|Phase3_Boss");

				// 플레이어
				GameObject* player = CreateGameObjectFromFBX(L"Player");
				player->AddComponent<Phase3Character>()->SetSkinnedMeshRenderer(player->GetTransform()->GetChild(0)->GetGameObject()->GetComponent<SkinnedMeshRenderer>());
				Animator* playerAnim = player->AddComponent<Animator>();
				FF::FFAnimationClip& phase3PlayerClip = GetAnimationClip(L"Armature|Phase3_Player");
				phase3PlayerClip.isLoop = true;
				playerAnim->PlusAnimationClip(phase3PlayerClip);
				playerAnim->Play(L"Armature|Phase3_Player");

				// 배경
				for (int i = 0; i < 6; ++i)
				{
					GameObject* backGroud = SceneManager::GetInstance().CreateGameObject(L"BackGroud");
					MeshRenderer* renderer = backGroud->AddComponent<MeshRenderer>();
					renderer->SetMesh(L"Cube");
					renderer->SetMaterial(L"CubeMaterial_F");
				}

			}

			SaveScene();

			SceneManager::GetInstance().DeleteAllGameObject();
			SceneManager::GetInstance().DeleteAllDontDestGameObject();
		}

		{
			SceneManager::GetInstance().SetName(L"BossPhase2Scene");

			// 카메라
			GameObject* cameraObject = SceneManager::GetInstance().CreateGameObject(L"MainCamera");
			Camera* camera = cameraObject->AddComponent<Camera>();
			SceneManager::GetInstance().SetMainCamera(camera);

			// 게임 매니저
			GameObject* gameManagerObject = SceneManager::GetInstance().CreateGameObject(L"GameManager");
			GameManager* gameManager = gameManagerObject->AddComponent<GameManager>();

			{
				GameObject* coverObject = SceneManager::GetInstance().CreateGameObject(L"CoverObject");
				coverObject->AddComponent<ZeldaUI>();
				Image* coverImage = coverObject->AddComponent<Image>();
				coverImage->SetLayer(901);
				coverObject->GetTransform()->SetLocalScale(Vector3D(1920, 1080));
				SpriteAnimator* mainanimator = coverObject->AddComponent<SpriteAnimator>();
				gameManager->SetMainAnimator(mainanimator);
			}

			{
				GameObject* coverObject = SceneManager::GetInstance().CreateGameObject(L"CoverObject");
				coverObject->AddComponent<ZeldaUI>();
				Image* coverImage = coverObject->AddComponent<Image>();
				coverImage->SetLayer(900);
				coverObject->GetTransform()->SetLocalScale(Vector3D(1920, 1080));
				SpriteAnimator* subanimator = coverObject->AddComponent<SpriteAnimator>();
				gameManager->SetSubAnimator(subanimator);
			}

			{
				GameObject* coverObject = SceneManager::GetInstance().CreateGameObject(L"CoverObject");
				coverObject->AddComponent<ZeldaUI>();
				Image* coverImage = coverObject->AddComponent<Image>();
				coverImage->SetLayer(899);
				coverObject->GetTransform()->SetLocalScale(Vector3D(1920, 1080));
				SpriteAnimator* thirdanimator = coverObject->AddComponent<SpriteAnimator>();
				gameManager->SetThirdAnimator(thirdanimator);
			}

			{
				GameObject* backGround = SceneManager::GetInstance().CreateGameObject(L"BackGround");
				backGround->GetTransform()->SetLocalScale(Vector3D(312.5, 200));
				backGround->GetTransform()->SetLocalPosition(Vector3D(0, -240, -270));
				SpriteRenderer* backGroundRenderer = backGround->AddComponent<SpriteRenderer>();
				backGroundRenderer->SetTexture(L"BackGround");
			}

#pragma region Player
			Vector3D diffHeight = Vector3D(0, -1.5, 0);
			GameObject* testPlayer = SceneManager::GetInstance().CreateGameObject(L"testPlayer");
			GameObject* playerHitOjbect = SceneManager::GetInstance().CreateGameObject(L"playerHitOjbect");
			GameObject* player2DHitOjbect = SceneManager::GetInstance().CreateGameObject(L"player2DHitOjbect");
			GameObject* playerAnimator = CreateGameObjectFromFBX(L"Player");
			testPlayer->GetTransform()->AddChild(playerHitOjbect->GetTransform());
			testPlayer->GetTransform()->AddChild(player2DHitOjbect->GetTransform());
			testPlayer->GetTransform()->AddChild(playerAnimator->GetTransform());
			playerAnimator->GetTransform()->SetLocalPosition(diffHeight);


			Animator* playeranim = playerAnimator->AddComponent<Animator>();
			FF::FFAnimationClip& playerrunClip = GetAnimationClip(L"Armature|running");
			playerrunClip.isLoop = true;

			FF::FFAnimationClip& playeridleClip = GetAnimationClip(L"Armature|idle");
			playeridleClip.isLoop = true;


			playeranim->PlusAnimationClip(GetAnimationClip(L"Armature|attack"));
			playeranim->PlusAnimationClip(GetAnimationClip(L"Armature|attack2"));
			playeranim->PlusAnimationClip(GetAnimationClip(L"Armature|damage"));
			playeranim->PlusAnimationClip(GetAnimationClip(L"Armature|dash"));
			playeranim->PlusAnimationClip(GetAnimationClip(L"Armature|down"));
			playeranim->PlusAnimationClip(GetAnimationClip(L"Armature|idle"));
			playeranim->PlusAnimationClip(GetAnimationClip(L"Armature|running"));


			playeranim->Play(L"Armature|idle");

			CapsuleCollider* playerHitCollider = playerHitOjbect->AddComponent<CapsuleCollider>();
			playerHitCollider->SetTrigger(true);

			//playerHitOjbect->GetTransform()->SetLocalPosition(Vector3D(10, 10, 11));

			SkinnedMeshRenderer* playerSkinnedMeshRenderer = playerAnimator->GetTransform()->GetChild(0)->GetGameObject()->GetComponent<SkinnedMeshRenderer>();

			Player* playerComponent = testPlayer->AddComponent<Player>();
			playerComponent->SetGameManager(gameManager);
			playerComponent->SetAnimator(playeranim);
			playerComponent->SetSkinnedMeshRenderer(playerSkinnedMeshRenderer);
			CapsuleCollider* playerCollider = testPlayer->AddComponent<CapsuleCollider>();

			Collider2D* player2DHitCollider = player2DHitOjbect->AddComponent<Collider2D>();
			player2DHitCollider->SetShape(Collider2D::Box);

			EdgeDetect* playerEdge = player2DHitOjbect->AddComponent<EdgeDetect>();
			playerEdge->SetMeshRenderer(playerAnimator->GetTransform()->GetChild(0)->GetGameObject()->GetComponent<SkinnedMeshRenderer>());

			Player2DCollisionManager* Collision2DManagerComponent = player2DHitOjbect->AddComponent<Player2DCollisionManager>();
			Collision2DManagerComponent->SetPlayerComponent(playerComponent);

			//playerCollider->SetTrigger(true);
			Rigidbody* playerRigidBody = testPlayer->AddComponent<Rigidbody>();
			playerRigidBody->SetFreezeRotationX(true);
			playerRigidBody->SetFreezeRotationY(true);
			playerRigidBody->SetFreezeRotationZ(true);
			playerRigidBody->SetLinearDrag(0.7);
			//playerRigidBody->SetKinematic(true);
			playerComponent->SetRigidBody(playerRigidBody);
			testPlayer->SetLayerName(L"Player");
			playerHitOjbect->SetLayerName(L"Player");
			player2DHitOjbect->SetLayerName(L"player2DHitCollider");

			GameObject* PlayerDamaged = SceneManager::GetInstance().CreateGameObject(L"PlayerDamaged");
			PlayerDamaged->AddComponent<ZeldaUI>();
			PlayerDamaged->AddComponent<Image>();
			SpriteAnimator* PlayerSprite = PlayerDamaged->AddComponent<SpriteAnimator>();
			PlayerSprite->SetTexture(
				{
					L"PlayerDamaged_4",
					L"PlayerDamaged_3",
					L"PlayerDamaged_2",
					L"PlayerDamaged_1",
					L"PlayerDamaged_0",
				});
			PlayerSprite->SetPlaySpeed(30);
			PlayerSprite->SetClearAfterPlayEnds(true);
			playerComponent->SetPlayerDamagedSpriteAnimator(PlayerSprite);
			PlayerDamaged->GetTransform()->SetLocalScale(Vector3D(128, 128, 128));


			GameObject* VFX_PlayerAttack = CreateGameObjectFromFBX(L"VFX_PlayerAttack");
			VFX_PlayerAttack->GetTransform()->SetLocalPosition(Vector3D(0, 0, 0));
			VFX_PlayerAttack->GetTransform()->SetParent(testPlayer->GetTransform());
			playerComponent->SetPlayerAttackEffect(VFX_PlayerAttack);
			VFX_PlayerAttack->SetActive(false);
			EffectPlayer* playerAttackEffect = VFX_PlayerAttack->AddComponent<EffectPlayer>();
			playerAttackEffect->SetMeshRenderer(VFX_PlayerAttack->GetTransform()->GetChild(0)->GetGameObject()->GetComponent<MeshRenderer>());
			playerAttackEffect->SetMaxValue(1.f);
			playerAttackEffect->SetPlaySpeed(1.5f);
			playerAttackEffect->SetRemainTime(0.3f);
			playerAttackEffect->SetDirection(Vector2D(1, 0));

			// 특수공격 이펙트
			GameObject* VFX_PlayerSpecialAttack = CreateGameObjectFromFBX(L"VFX_PlayerSpecialAttack");
			VFX_PlayerSpecialAttack->GetTransform()->SetLocalPosition(Vector3D(0, 0, 0));
			VFX_PlayerSpecialAttack->GetTransform()->SetParent(testPlayer->GetTransform());
			playerComponent->SetPlayerSpecialAttackEffect(VFX_PlayerSpecialAttack);
			VFX_PlayerSpecialAttack->SetActive(false);
			EffectPlayer* playerSpecialAttackEffect = VFX_PlayerSpecialAttack->AddComponent<EffectPlayer>();
			playerSpecialAttackEffect->SetMeshRenderer(VFX_PlayerSpecialAttack->GetTransform()->GetChild(0)->GetGameObject()->GetComponent<MeshRenderer>());
			playerSpecialAttackEffect->SetMaxValue(1.f);
			playerSpecialAttackEffect->SetPlaySpeed(1.5f);
			playerSpecialAttackEffect->SetRemainTime(0.3f);
			playerSpecialAttackEffect->SetDirection(Vector2D(1, 0));

#pragma endregion

			GameObject* cameraobject = SceneManager::GetInstance().GetMainCamera()->GetGameObject();
			FollowCamera* followCamera = cameraobject->AddComponent<FollowCamera>();
			followCamera->SetPlayerTransform(testPlayer->GetTransform());
			//SceneManager::GetInstance().GetMainCamera()->GetGameObject()->GetComponent<FollowCamera>()->SetPlayerTransform(playerAnimator->GetTransform());
			//SceneManager::GetInstance().GetMainCamera()->GetGameObject()->GetTransform()->SetLocalPosition(Vector3D(0, 180, 207));
			//SceneManager::GetInstance().GetMainCamera()->GetGameObject()->GetTransform()->SetLocalRotation(Vector3D(40, 180, 0));

			GameObject* lightObject = SceneManager::GetInstance().CreateGameObject(L"GameObject004");
			DirectionalLight* directionalLight = lightObject->AddComponent<DirectionalLight>();
			lightObject->GetTransform()->SetLocalPosition(Vector3D(1000, 30, 0));
			lightObject->GetTransform()->SetLocalRotation(Vector3D(110, 25, 0));

			//GameObject* collsionTestObject001 = CreateGameObjectFromFBX(L"Mole");
			GameObject* boss1 = CreateGameObjectFromFBX(L"Boss");
			GameObject* boss1HitObject = SceneManager::GetInstance().CreateGameObject(L"Boss1HitObject");
			boss1->GetTransform()->AddChild(boss1HitObject->GetTransform());

			GameObject* boss2DHitOjbect = SceneManager::GetInstance().CreateGameObject(L"boss2DHitOjbect");
			boss2DHitOjbect->GetTransform()->SetParent(boss1->GetTransform());
			Collider2D* boss2DHitCollider = boss2DHitOjbect->AddComponent<Collider2D>();
			boss2DHitCollider->SetShape(Collider2D::Box);
			boss2DHitOjbect->SetLayerName(L"boss2DHitCollider");

			EdgeDetect* bossEdge = boss2DHitOjbect->AddComponent<EdgeDetect>();
			bossEdge->SetMeshRenderer(boss1->GetTransform()->GetChild(0)->GetGameObject()->GetComponent<SkinnedMeshRenderer>());

			CapsuleCollider* boss1HitCollider = boss1HitObject->AddComponent<CapsuleCollider>();
			boss1HitCollider->SetTrigger(true);

			Boss1* boss1Component = boss1->AddComponent<Boss1>();
			boss1Component->SetGameManager(gameManager);
			boss1Component->SetPlayerTransform(testPlayer->GetTransform());
			boss1Component->SetPlayerCollider(playerHitCollider);
			boss1Component->SetBossHitCollider(boss1HitCollider);
			boss1Component->SetPlayerGameObject(testPlayer);
			boss1Component->SetPlayerComponent(playerComponent);

			GameObject* boss1HitEffect = SceneManager::GetInstance().CreateGameObject(L"boss1HitEffect");
			boss1HitEffect->AddComponent<ZeldaUI>();
			boss1HitEffect->AddComponent<Image>();
			SpriteAnimator* bossHitSpriteAnimator = boss1HitEffect->AddComponent<SpriteAnimator>();
			bossHitSpriteAnimator->SetTexture({
				L"boss_damage_0",
				L"boss_damage_1",
				L"boss_damage_2",
				L"boss_damage_3",
				L"boss_damage_4",
				L"boss_damage_5",
				L"boss_damage_6"
				});
			bossHitSpriteAnimator->SetPlaySpeed(30.0f);
			bossHitSpriteAnimator->SetClearAfterPlayEnds(true);
			boss1Component->SetBossHitSpriteAnimator(bossHitSpriteAnimator);
			boss1HitEffect->GetTransform()->SetLocalScale(Vector3D(128, 128, 128));


			/// 나동
			boss1HitObject->SetLayerName(L"Boss1");
			boss1->SetLayerName(L"Boss1");

			//----------------------------------------------------------------------Animation----------------------------------------------------------------------
			Animator* anim = boss1->AddComponent<Animator>();
			FF::FFAnimationClip& idleClip = GetAnimationClip(L"root|000.Idle");
			idleClip.isLoop = true;

			FF::FFAnimationClip& chargeAttackClip = GetAnimationClip(L"root|007-2.Skill_ChargeAttack");
			chargeAttackClip.isLoop = true;

			FF::FFAnimationClip& walkClip = GetAnimationClip(L"root|001-2.Walk");
			walkClip.isLoop = true;

			anim->PlusAnimationClip(GetAnimationClip(L"root|000.Idle"));
			anim->PlusAnimationClip(GetAnimationClip(L"root|001-1.Idle->Walk"));
			anim->PlusAnimationClip(GetAnimationClip(L"root|001-2.Walk"));

			anim->PlusAnimationClip(GetAnimationClip(L"root|001-3.Walk->Idle"));

			anim->PlusAnimationClip(GetAnimationClip(L"root|002-1.NormalAttack_R"));
			anim->PlusAnimationClip(GetAnimationClip(L"root|002-2.NormalAttack_R->Idle"));

			anim->PlusAnimationClip(GetAnimationClip(L"root|003-1.NormalAttack_L"));
			anim->PlusAnimationClip(GetAnimationClip(L"root|003-2.NormalAttack_L->Idle"));

			anim->PlusAnimationClip(GetAnimationClip(L"root|004.RangedAttack"));

			anim->PlusAnimationClip(GetAnimationClip(L"root|005.Teleport"));

			anim->PlusAnimationClip(GetAnimationClip(L"root|006-1.Skill_Tree"));
			anim->PlusAnimationClip(GetAnimationClip(L"root|006-2.Skill_Tree->Idle"));

			anim->PlusAnimationClip(GetAnimationClip(L"root|007-1.Idle->Skill_ChargeAttack"));
			anim->PlusAnimationClip(GetAnimationClip(L"root|007-2.Skill_ChargeAttack"));

			anim->PlusAnimationClip(GetAnimationClip(L"root|007-3.Skill_ChargeAttack->Idle"));

			anim->PlusAnimationClip(GetAnimationClip(L"root|008.Skill_Knockback"));

			anim->PlusAnimationClip(GetAnimationClip(L"root|009.Skill_Blocking"));

			anim->PlusAnimationClip(GetAnimationClip(L"root|010.Skill_2D"));

			anim->PlusAnimationClip(GetAnimationClip(L"root|011-1.Groggy"));
			anim->PlusAnimationClip(GetAnimationClip(L"root|011-2.Groogy->Idle"));

			anim->PlusAnimationClip(GetAnimationClip(L"root|012.Death"));

			anim->PlusAnimationClip(GetAnimationClip(L"root|PhaseTransition"));

			anim->Play(L"root|000.Idle");

			CapsuleCollider* boss1Collider = boss1->AddComponent<CapsuleCollider>();
			//boss1Collider->SetTrigger(true);


			testPlayer->GetTransform()->SetLocalPosition(Vector3D(0, 4, 13));

			boss1->GetTransform()->SetLocalPosition(Vector3D(0, 4.0f, 1));
			boss1->GetTransform()->Rotate(boss1->GetTransform()->GetUp(), DegreeToRadian(180));

			/// 나동
			testPlayer->GetComponent<Player>()->SetBossCollider(boss1HitCollider);
			testPlayer->GetComponent<Player>()->SetBossTransform(boss1HitObject->GetTransform());
			testPlayer->GetComponent<Player>()->SetPlayerAnimTransform(playerAnimator->GetTransform());
			testPlayer->GetComponent<Player>()->SetBossAnimTransform(boss1->GetTransform());
			testPlayer->GetComponent<Player>()->SetBossComponent(boss1Component);
			//cam1->GetTransform()->SetParent(boss1->GetTransform());

			// 족자 생성
			{
				GameObject* scrollObject = CreateGameObjectFromFBX(L"Scroll");
				FloatingObject* floatingComponent = scrollObject->AddComponent<FloatingObject>();
				floatingComponent->SetObjectTransform(scrollObject->GetTransform());
				scrollObject->GetTransform()->SetLocalPosition(Vector3D(25.0, 3.0, -11.0));
				scrollObject->GetTransform()->SetLocalRotation(Quaternion(0.8660253882408142, 0, -0.5, 0));
				boss1Component->SetScrollComponent(floatingComponent);
			}

			{
				GameObject* scrollObject = CreateGameObjectFromFBX(L"Scroll");
				FloatingObject* floatingComponent = scrollObject->AddComponent<FloatingObject>();
				floatingComponent->SetObjectTransform(scrollObject->GetTransform());
				scrollObject->GetTransform()->SetLocalPosition(Vector3D(-25.0, 3.0, -11.0));
				scrollObject->GetTransform()->SetLocalRotation(Quaternion(0.8660253882408142, 0, 0.5, 0));
				boss1Component->SetScrollComponent(floatingComponent);
			}

			{
				GameObject* scrollObject = CreateGameObjectFromFBX(L"Scroll");
				FloatingObject* floatingComponent = scrollObject->AddComponent<FloatingObject>();
				floatingComponent->SetObjectTransform(scrollObject->GetTransform());
				scrollObject->GetTransform()->SetLocalPosition(Vector3D(22.0, 3.0, -16.0));
				scrollObject->GetTransform()->SetLocalRotation(Quaternion(0.9396926164627075, 0, -0.3420201241970062, 0));
				boss1Component->SetScrollComponent(floatingComponent);
			}

			{
				GameObject* scrollObject = CreateGameObjectFromFBX(L"Scroll");
				FloatingObject* floatingComponent = scrollObject->AddComponent<FloatingObject>();
				floatingComponent->SetObjectTransform(scrollObject->GetTransform());
				scrollObject->GetTransform()->SetLocalPosition(Vector3D(-22.0, 3.0, -16.0));
				scrollObject->GetTransform()->SetLocalRotation(Quaternion(0.9396926164627075, 0, 0.3420201241970062, 0));
				boss1Component->SetScrollComponent(floatingComponent);
			}

			{
				GameObject* scrollObject = CreateGameObjectFromFBX(L"Scroll");
				FloatingObject* floatingComponent = scrollObject->AddComponent<FloatingObject>();
				floatingComponent->SetObjectTransform(scrollObject->GetTransform());
				scrollObject->GetTransform()->SetLocalPosition(Vector3D(18.0, 3.0, -20.0));
				scrollObject->GetTransform()->SetLocalRotation(Quaternion(0.9537169337272644, 0, -0.30070579051971436, 0));
				boss1Component->SetScrollComponent(floatingComponent);
			}

			{
				GameObject* scrollObject = CreateGameObjectFromFBX(L"Scroll");
				FloatingObject* floatingComponent = scrollObject->AddComponent<FloatingObject>();
				floatingComponent->SetObjectTransform(scrollObject->GetTransform());
				scrollObject->GetTransform()->SetLocalPosition(Vector3D(-18.0, 3.0, -20.0));
				scrollObject->GetTransform()->SetLocalRotation(Quaternion(0.9537169337272644, 0, 0.30070579051971436, 0));
				boss1Component->SetScrollComponent(floatingComponent);
			}

			{
				GameObject* scrollObject = CreateGameObjectFromFBX(L"Scroll");
				FloatingObject* floatingComponent = scrollObject->AddComponent<FloatingObject>();
				floatingComponent->SetObjectTransform(scrollObject->GetTransform());
				scrollObject->GetTransform()->SetLocalPosition(Vector3D(14.0, 3.0, -23.0));
				scrollObject->GetTransform()->SetLocalRotation(Quaternion(0.9790454506874084, 0, -0.20364175736904144, 0));
				boss1Component->SetScrollComponent(floatingComponent);
			}

			{
				GameObject* scrollObject = CreateGameObjectFromFBX(L"Scroll");
				FloatingObject* floatingComponent = scrollObject->AddComponent<FloatingObject>();
				floatingComponent->SetObjectTransform(scrollObject->GetTransform());
				scrollObject->GetTransform()->SetLocalPosition(Vector3D(-14.0, 3.0, -23.0));
				scrollObject->GetTransform()->SetLocalRotation(Quaternion(0.9790454506874084, 0, 0.20364175736904144, 0));
				boss1Component->SetScrollComponent(floatingComponent);
			}

			// 깃발 생성
			{
				GameObject* bossFlag = CreateGameObjectFromFBX(L"BossFlag");
				MeshRenderer* flagRenderer = bossFlag->GetTransform()->GetChild(0)->GetGameObject()->GetComponent<MeshRenderer>();
				flagRenderer->GetMaterialInfo(0).texture[1] = L"..\\Resources\\Texture\\BossFlag\\delete.jpg";
				flagRenderer->GetMaterialInfo(0).texture[2] = L"..\\Resources\\Texture\\BossFlag\\T_Flag_Diffuse_002.png";
				flagRenderer->GetMaterialInfo(0).color = ZeldaColor::White;
				bossFlag->GetTransform()->SetLocalPosition(Vector3D(-43.0, 10, 9));
				bossFlag->GetTransform()->SetLocalRotation(Quaternion(0.8526401519775391, 0, 0.5224985480308533, 0));

				BossFlag* flagComponent = bossFlag->AddComponent<BossFlag>();
				flagComponent->SetMeshRenderer(flagRenderer);
				flagComponent->SetPlaySpeed(0.000002);

				boss1Component->SetBossFlagComponent(flagComponent);
			}

			{
				GameObject* bossFlag = CreateGameObjectFromFBX(L"BossFlag");
				MeshRenderer* flagRenderer = bossFlag->GetTransform()->GetChild(0)->GetGameObject()->GetComponent<MeshRenderer>();
				flagRenderer->GetMaterialInfo(0).texture[1] = L"..\\Resources\\Texture\\BossFlag\\delete.jpg";
				flagRenderer->GetMaterialInfo(0).texture[2] = L"..\\Resources\\Texture\\BossFlag\\T_Flag_Diffuse_002.png";
				flagRenderer->GetMaterialInfo(0).color = ZeldaColor::White;
				bossFlag->GetTransform()->SetLocalPosition(Vector3D(-37.0, 12.0, -13.0));
				bossFlag->GetTransform()->SetLocalRotation(Quaternion(0.9455185532569885, 0, 0.32556813955307007, 0));

				BossFlag* flagComponent = bossFlag->AddComponent<BossFlag>();
				flagComponent->SetMeshRenderer(flagRenderer);
				flagComponent->SetPlaySpeed(0.000002);

				boss1Component->SetBossFlagComponent(flagComponent);
			}

			{
				GameObject* bossFlag = CreateGameObjectFromFBX(L"BossFlag");
				MeshRenderer* flagRenderer = bossFlag->GetTransform()->GetChild(0)->GetGameObject()->GetComponent<MeshRenderer>();
				flagRenderer->GetMaterialInfo(0).texture[1] = L"..\\Resources\\Texture\\BossFlag\\delete.jpg";
				flagRenderer->GetMaterialInfo(0).texture[2] = L"..\\Resources\\Texture\\BossFlag\\T_Flag_Diffuse_002.png";
				flagRenderer->GetMaterialInfo(0).color = ZeldaColor::White;
				bossFlag->GetTransform()->SetLocalPosition(Vector3D(-23.0, 16.0, -23.0));
				bossFlag->GetTransform()->SetLocalRotation(Quaternion(0.9876883625984192, 0, 0.15643447637557983, 0));

				BossFlag* flagComponent = bossFlag->AddComponent<BossFlag>();
				flagComponent->SetMeshRenderer(flagRenderer);
				flagComponent->SetPlaySpeed(0.000002);

				boss1Component->SetBossFlagComponent(flagComponent);
			}

			{
				GameObject* bossFlag = CreateGameObjectFromFBX(L"BossFlag");
				MeshRenderer* flagRenderer = bossFlag->GetTransform()->GetChild(0)->GetGameObject()->GetComponent<MeshRenderer>();
				flagRenderer->GetMaterialInfo(0).texture[1] = L"..\\Resources\\Texture\\BossFlag\\delete.jpg";
				flagRenderer->GetMaterialInfo(0).texture[2] = L"..\\Resources\\Texture\\BossFlag\\T_Flag_Diffuse_002.png";
				flagRenderer->GetMaterialInfo(0).color = ZeldaColor::White;
				bossFlag->GetTransform()->SetLocalPosition(Vector3D(23.0, 16.0, -23.0));
				bossFlag->GetTransform()->SetLocalRotation(Quaternion(0.9876883625984192, 0, -0.15643447637557983, 0));

				BossFlag* flagComponent = bossFlag->AddComponent<BossFlag>();
				flagComponent->SetMeshRenderer(flagRenderer);
				flagComponent->SetPlaySpeed(0.000002);

				boss1Component->SetBossFlagComponent(flagComponent);
			}

			{
				GameObject* bossFlag = CreateGameObjectFromFBX(L"BossFlag");
				MeshRenderer* flagRenderer = bossFlag->GetTransform()->GetChild(0)->GetGameObject()->GetComponent<MeshRenderer>();
				flagRenderer->GetMaterialInfo(0).texture[1] = L"..\\Resources\\Texture\\BossFlag\\delete.jpg";
				flagRenderer->GetMaterialInfo(0).texture[2] = L"..\\Resources\\Texture\\BossFlag\\T_Flag_Diffuse_002.png";
				flagRenderer->GetMaterialInfo(0).color = ZeldaColor::White;
				bossFlag->GetTransform()->SetLocalPosition(Vector3D(37.0, 12.0, -13.0));
				bossFlag->GetTransform()->SetLocalRotation(Quaternion(0.9455185532569885, 0, -0.32556813955307007, 0));

				BossFlag* flagComponent = bossFlag->AddComponent<BossFlag>();
				flagComponent->SetMeshRenderer(flagRenderer);
				flagComponent->SetPlaySpeed(0.000002);

				boss1Component->SetBossFlagComponent(flagComponent);
			}

			{
				GameObject* bossFlag = CreateGameObjectFromFBX(L"BossFlag");
				MeshRenderer* flagRenderer = bossFlag->GetTransform()->GetChild(0)->GetGameObject()->GetComponent<MeshRenderer>();
				flagRenderer->GetMaterialInfo(0).texture[1] = L"..\\Resources\\Texture\\BossFlag\\delete.jpg";
				flagRenderer->GetMaterialInfo(0).texture[2] = L"..\\Resources\\Texture\\BossFlag\\T_Flag_Diffuse_002.png";
				flagRenderer->GetMaterialInfo(0).color = ZeldaColor::White;
				bossFlag->GetTransform()->SetLocalPosition(Vector3D(43.0, 10, 9));
				bossFlag->GetTransform()->SetLocalRotation(Quaternion(0.8526401519775391, 0, -0.5224985480308533, 0));

				BossFlag* flagComponent = bossFlag->AddComponent<BossFlag>();
				flagComponent->SetMeshRenderer(flagRenderer);
				flagComponent->SetPlaySpeed(0.000002);

				boss1Component->SetBossFlagComponent(flagComponent);
			}

			// followCamera 컴포넌트 등록
			boss1Component->SetFollowCameraComponent(followCamera);


#pragma region floor
			/// BossFloor
			GameObject* BossFloor = CreateGameObjectFromFBX(L"BossFloor");
			GameObject* BossFloorMesh = BossFloor->GetTransform()->GetChild(0)->GetGameObject();
			MeshRenderer* renderer = BossFloorMesh->GetComponent<MeshRenderer>();
			renderer->GetMaterialInfo(1).shader = L"..\\Resources\\Shader\\BossFloor.hlsli";

			GameObject* child = SceneManager::GetInstance().CreateGameObject(L"child");
			child->SetLayerName(L"Floor");
			BossFloor->GetTransform()->AddChild(child->GetTransform());
			child->GetTransform()->SetLocalScale(Vector3D(50, 1, 50));
			Rigidbody* rigid = child->AddComponent<Rigidbody>();
			CylinderCollider* collider = child->AddComponent<CylinderCollider>();
			rigid->SetKinematic(true);
			rigid->SetRestitution(0.0f);
			rigid->SetKfriction(0.9f);
			rigid->SetSfriction(0.95f);
#pragma endregion

#pragma region door
			GameObject* door = CreateGameObjectFromFBX(L"BossDoor");
			door->GetTransform()->SetLocalPosition(Vector3D(0, -14, -37));

			GameObject* hand = CreateGameObjectFromFBX(L"BossDoorHand");

			hand->GetTransform()->SetParent(door->GetTransform());
			Animator* animator = hand->AddComponent<Animator>();
			FF::FFAnimationClip& clip = GetAnimationClip(L"Hand_rig|Idle");
			clip.isLoop = true;
			animator->PlusAnimationClip(GetAnimationClip(L"Hand_rig|Idle"));
			animator->Play(L"Hand_rig|Idle");
#pragma endregion

			/// 플레이어 없으면 UI 에서 터지게 되어 있어서 일단 플레이어 넣음
			GameObject* playerObj = testPlayer;
			Player* player = playerComponent;


			// 인게임 UI
			{
				// 체력바
				{
					// 좌측 하단 기준 offset
					Vector2D UISize = { 470, 80 };

					GameObject* object = SceneManager::GetInstance().CreateGameObject(L"HP_UI");
					ZeldaUI* ui = object->AddComponent<ZeldaUI>();
					object->GetTransform()->SetLocalPosition(Vector3D(-675.00, -458.20));
					HPUI* hpUI = object->AddComponent<HPUI>();
					hpUI->SetUISize(UISize);

					// Base
					GameObject* baseUIobj = SceneManager::GetInstance().CreateGameObject(L"HP_Base");
					baseUIobj->GetTransform()->SetParent(object->GetTransform());
					baseUIobj->GetTransform()->SetLocalScale(Vector3D(UISize.x, UISize.y));
					auto bu = baseUIobj->AddComponent<BaseUI>();
					ZeldaUI* b_ui = baseUIobj->AddComponent<ZeldaUI>();
					Image* b_image = baseUIobj->AddComponent<Image>();
					b_image->SetTexture(L"HPBarBase");
					b_image->SetLayer(0);

					// Move
					GameObject* moveUIobj = SceneManager::GetInstance().CreateGameObject(L"HP_Move");
					moveUIobj->GetTransform()->SetParent(object->GetTransform());
					auto mu = moveUIobj->AddComponent<MovableUI>();
					ZeldaUI* m_ui = moveUIobj->AddComponent<ZeldaUI>();
					Image* m_image = moveUIobj->AddComponent<Image>();
					m_image->SetTexture(L"HPBar");
					m_image->SetLayer(10);
					moveUIobj->SetActive(false);

					// Cover
					GameObject* coverUIobj = SceneManager::GetInstance().CreateGameObject(L"HP_Cover");
					coverUIobj->GetTransform()->SetParent(object->GetTransform());
					coverUIobj->GetTransform()->SetLocalScale(Vector3D(UISize.x, UISize.y));
					auto cu = coverUIobj->AddComponent<CoverUI>();
					ZeldaUI* c_ui = coverUIobj->AddComponent<ZeldaUI>();
					Image* c_image = coverUIobj->AddComponent<Image>();
					c_image->SetTexture(L"HPBarCover");
					c_image->SetLayer(20);

					// UI 장착
					hpUI->SetBaseUI(*bu);
					hpUI->SetMovableUI(*mu);
					hpUI->SetCoverUI(*cu);
					hpUI->SetPlayer(*player);
				}

				// 쿨타임 UI
				{
					GameObject* CoolTimeUIGroup = SceneManager::GetInstance().CreateGameObject(L"CoolTimeUIGroup");
					ZeldaUI* ui = CoolTimeUIGroup->AddComponent<ZeldaUI>();
					CoolTimeUIGroup->GetTransform()->SetLocalPosition(Vector3D(-675.00f, -400.40f));

					// 대시 스킬 UI
					{
						Vector2D UISize = { 470, 22 };

						GameObject* object = SceneManager::GetInstance().CreateGameObject(L"DashUI");
						object->GetTransform()->SetParent(CoolTimeUIGroup->GetTransform());
						object->GetTransform()->SetLocalScale(Vector3D(UISize.x, UISize.y));
						CoolTimeUI* ui = object->AddComponent<DashCoolTimeUI>();
						ui->SetUISize(UISize);

						// Base
						GameObject* baseUIobj = SceneManager::GetInstance().CreateGameObject(L"Dash_Base");
						baseUIobj->GetTransform()->SetParent(object->GetTransform());
						baseUIobj->GetTransform()->SetLocalScale(Vector3D(UISize.x, UISize.y));
						auto bu = baseUIobj->AddComponent<BaseUI>();
						ZeldaUI* b_ui = baseUIobj->AddComponent<ZeldaUI>();
						Image* b_image = baseUIobj->AddComponent<Image>();
						b_image->SetTexture(L"DashBase");
						b_image->SetLayer(0);

						// Move
						GameObject* moveUIobj = SceneManager::GetInstance().CreateGameObject(L"Dash_Move");
						moveUIobj->GetTransform()->SetParent(object->GetTransform());
						auto mu = moveUIobj->AddComponent<MovableUI>();
						ZeldaUI* m_ui = moveUIobj->AddComponent<ZeldaUI>();
						Image* m_image = moveUIobj->AddComponent<Image>();
						m_image->SetTexture(L"DashMoveCover");
						m_image->SetLayer(10);
						moveUIobj->SetActive(false);

						// Num
						GameObject* numUIobj = SceneManager::GetInstance().CreateGameObject(L"Dash_Num");
						numUIobj->GetTransform()->SetParent(object->GetTransform());
						numUIobj->GetTransform()->SetLocalScale(Vector3D(UISize.x, UISize.y));
						auto nu = numUIobj->AddComponent<NumUI>();
						ZeldaUI* n_ui = numUIobj->AddComponent<ZeldaUI>();
						Image* n_image = numUIobj->AddComponent<Image>();
						n_image->SetTexture(L"None");
						n_image->SetLayer(20);

						// Cover
						GameObject* coverUIobj = SceneManager::GetInstance().CreateGameObject(L"Dash_Cover");
						coverUIobj->GetTransform()->SetParent(object->GetTransform());
						coverUIobj->GetTransform()->SetLocalScale(Vector3D(UISize.x, UISize.y));
						auto cu = coverUIobj->AddComponent<CoverUI>();
						ZeldaUI* c_ui = coverUIobj->AddComponent<ZeldaUI>();
						Image* c_image = coverUIobj->AddComponent<Image>();
						c_image->SetTexture(L"DashCover");
						c_image->SetLayer(30);

						// UI 장착
						ui->SetBaseUI(*bu);
						ui->SetMovableUI(*mu);
						ui->SetNumUI(*nu);
						ui->SetCoverUI(*cu);
						ui->SetTarget(*playerObj);
					}

					// 원거리 스킬 UI
					{
						Vector2D UISize = { 115, 115 };

						GameObject* object = SceneManager::GetInstance().CreateGameObject(L"StandoffUI");
						object->GetTransform()->SetParent(CoolTimeUIGroup->GetTransform());
						object->GetTransform()->SetLocalScale(Vector3D(UISize.x, UISize.y));
						object->GetTransform()->SetLocalPosition(Vector3D(298.8, -40.6));
						CoolTimeUI* ui = object->AddComponent<StandoffCoolTimeUI>();
						ui->SetUISize(UISize);

						// Base
						GameObject* baseUIobj = SceneManager::GetInstance().CreateGameObject(L"Standoff_Base");
						baseUIobj->GetTransform()->SetParent(object->GetTransform());
						baseUIobj->GetTransform()->SetLocalScale(Vector3D(UISize.x, UISize.y));
						auto bu = baseUIobj->AddComponent<BaseUI>();
						ZeldaUI* b_ui = baseUIobj->AddComponent<ZeldaUI>();
						Image* b_image = baseUIobj->AddComponent<Image>();
						b_image->SetTexture(L"StandoffBase");
						b_image->SetLayer(0);

						// Move
						GameObject* moveUIobj = SceneManager::GetInstance().CreateGameObject(L"Standoff_Move");
						moveUIobj->GetTransform()->SetParent(object->GetTransform());
						auto mu = moveUIobj->AddComponent<MovableUI>();
						ZeldaUI* m_ui = moveUIobj->AddComponent<ZeldaUI>();
						Image* m_image = moveUIobj->AddComponent<Image>();
						m_image->SetTexture(L"StandoffMoveCover");
						m_image->SetLayer(10);
						moveUIobj->SetActive(false);

						// Num
						GameObject* numUIobj = SceneManager::GetInstance().CreateGameObject(L"Standoff_Num");
						numUIobj->GetTransform()->SetParent(object->GetTransform());
						numUIobj->GetTransform()->SetLocalScale(Vector3D(UISize.x, UISize.y));
						auto nu = numUIobj->AddComponent<NumUI>();
						ZeldaUI* n_ui = numUIobj->AddComponent<ZeldaUI>();
						Image* n_image = numUIobj->AddComponent<Image>();
						n_image->SetTexture(L"None");
						n_image->SetLayer(20);

						// Cover
						GameObject* coverUIobj = SceneManager::GetInstance().CreateGameObject(L"Standoff_Cover");
						coverUIobj->GetTransform()->SetParent(object->GetTransform());
						coverUIobj->GetTransform()->SetLocalScale(Vector3D(UISize.x, UISize.y));
						auto cu = coverUIobj->AddComponent<CoverUI>();
						ZeldaUI* c_ui = coverUIobj->AddComponent<ZeldaUI>();
						Image* c_image = coverUIobj->AddComponent<Image>();
						c_image->SetTexture(L"StandoffCover");
						c_image->SetLayer(30);

						// UI 장착
						ui->SetBaseUI(*bu);
						ui->SetMovableUI(*mu);
						ui->SetNumUI(*nu);
						ui->SetCoverUI(*cu);
						ui->SetTarget(*playerObj);
					}
				}

				// 설정 UI
				{
					/// Button
					// 우측 상단 기준 offset
					Vector2D buttonSize = { 87, 93 };

					GameObject* object = SceneManager::GetInstance().CreateGameObject(L"SettingUI");
					object->GetTransform()->SetLocalScale(Vector3D(buttonSize.x, buttonSize.y, 0));
					object->GetTransform()->SetLocalPosition(Vector3D(845.10f, 444.50f));
					SettingButton* setbt = object->AddComponent<SettingButton>();
					ZeldaUI* ui = object->AddComponent<ZeldaUI>();
					Image* image = object->AddComponent<Image>();
					image->SetTexture(L"SettingUI_Idle");
					Button* button = object->AddComponent<Button>();
					//button->SetImage(L"SettingUI_Idle", L"SettingUI_Enter", L"SettingUI_Click");

					// Panel
					Vector2D backgroundSize = { 1920, 1080 };

					GameObject* settingObj = SceneManager::GetInstance().CreateGameObject(L"SettingGroup");
					settingObj->GetTransform()->SetLocalScale(Vector3D(backgroundSize.x, backgroundSize.y, 0));
					ZeldaUI* setting_background = settingObj->AddComponent<ZeldaUI>();
					Image* background_image = settingObj->AddComponent<Image>();
					background_image->SetTexture(L"BlackCoverBackground");
					background_image->SetLayer(900);
					settingObj->SetActive(false);

					// 족자 이미지
					{
						Vector2D size = { 617, 846 };

						GameObject* obj = SceneManager::GetInstance().CreateGameObject(L"Panel");
						obj->GetTransform()->SetParent(settingObj->GetTransform());
						obj->GetTransform()->SetLocalScale(Vector3D(size.x, size.y, 0));
						obj->GetTransform()->SetLocalPosition(Vector3D(620.30, 73.50, 0));
						ZeldaUI* ui = obj->AddComponent<ZeldaUI>();
						Image* image = obj->AddComponent<Image>();
						image->SetTexture(L"SettingBackground");
						image->SetLayer(910);
					}

					// SoundBar
					{
						Vector2D size = { 285, 25 };

						GameObject* obj = SceneManager::GetInstance().CreateGameObject(L"Obj_SoundBar");
						obj->GetTransform()->SetParent(settingObj->GetTransform());
						obj->GetTransform()->SetLocalScale(Vector3D(size.x, size.y, 0));
						obj->GetTransform()->SetLocalPosition(Vector3D(626.7, 300, 0));
						SoundBar* sb = obj->AddComponent<SoundBar>();
						obj->AddComponent<ScrollUI>();
						ZeldaUI* ui = obj->AddComponent<ZeldaUI>();
						Image* image = obj->AddComponent<Image>();
						image->SetTexture(L"ScrollUI");
						image->SetLayer(920);
						sb->SetUISize(size);
						sb->SetPadding(20);

						// Handle
						{
							Vector2D hsize = { 30, 30 };

							GameObject* hobj = SceneManager::GetInstance().CreateGameObject(L"SoundBar_Handle");
							hobj->GetTransform()->SetParent(obj->GetTransform());
							hobj->GetTransform()->SetLocalScale(Vector3D(hsize.x, hsize.y, 0));
							hobj->SetActive(false);
							SoundHandle* sh = hobj->AddComponent<SoundHandle>();
							ZeldaUI* ui = hobj->AddComponent<ZeldaUI>();
							Image* image = hobj->AddComponent<Image>();
							image->SetTexture(L"SoundHandle");
							image->SetLayer(950);

							sb->SetHandle(*sh);
						}

						// SoundBarText
						{
							Vector2D textSize = { 112, 47.7f };

							GameObject* soundText = SceneManager::GetInstance().CreateGameObject(L"Obj_SoundText");
							soundText->GetTransform()->SetParent(obj->GetTransform());
							soundText->GetTransform()->SetLocalScale(Vector3D(textSize.x, textSize.y, 0));
							soundText->GetTransform()->SetLocalPosition(Vector3D(-88.6, 35.8, 0));
							ZeldaUI* soundText_ui = soundText->AddComponent<ZeldaUI>();
							Image* soundText_texture = soundText->AddComponent<Image>();
							soundText_texture->SetTexture(L"SoundText_Ingame_Idle");
							soundText_texture->SetLayer(920);
						}
					}

					// 키보드 입력에 따른 버튼 조정

					GameObject* bs = SceneManager::GetInstance().CreateGameObject(L"SettingButtonSelector");
					bs->GetTransform()->SetParent(settingObj->GetTransform());
					ButtonSelector* bsp = bs->AddComponent<ButtonSelector>();

					// Continue Button
					{
						Vector2D size = { 260, 80 };

						GameObject* obj = SceneManager::GetInstance().CreateGameObject(L"Setting_ContinueButton");
						obj->GetTransform()->SetParent(settingObj->GetTransform());
						obj->GetTransform()->SetLocalScale(Vector3D(size.x, size.y, 0));
						obj->GetTransform()->SetLocalPosition(Vector3D(624, 170, 0));
						ContinueButton* sb = obj->AddComponent<ContinueButton>();
						ZeldaUI* ui = obj->AddComponent<ZeldaUI>();
						Image* image = obj->AddComponent<Image>();
						image->SetTexture(L"Continue_Idle");
						image->SetLayer(920);
						Button* button = obj->AddComponent<Button>();
						//button->SetImage(L"Continue_Idle", L"Continue_Enter", L"Continue_Click");
						sb->SetSettingButton(*setbt);

						// Attach UI
						{
							Vector2D uiSize = { 70, 70 };

							GameObject* attachUI = SceneManager::GetInstance().CreateGameObject(L"Exit_AttachUI");
							attachUI->GetTransform()->SetParent(obj->GetTransform());
							attachUI->GetTransform()->SetLocalScale(Vector3D(uiSize.x, uiSize.y));
							attachUI->GetTransform()->SetLocalPosition(Vector3D(-157.30, 0, 0));
							AttachUI* atp = attachUI->AddComponent<AttachUI>();
							ZeldaUI* ui = attachUI->AddComponent<ZeldaUI>();
							Image* image = attachUI->AddComponent<Image>();
							image->SetTexture(L"AttachUI");
							image->SetLayer(950);

							sb->SetAttachUI(*atp);
							sb->SetUISize(size);
							sb->SetAttachUISize(uiSize);

							attachUI->SetActive(false);
						}

						bsp->AddButton(*obj);
						bsp->SetFirstButton(*obj);
					}

					// Exit Button
					{
						Vector2D size = { 252.7, 74.3 };

						GameObject* obj = SceneManager::GetInstance().CreateGameObject(L"Setting_ExitButton");
						obj->GetTransform()->SetParent(settingObj->GetTransform());
						obj->GetTransform()->SetLocalScale(Vector3D(size.x, size.y, 0));
						obj->GetTransform()->SetLocalPosition(Vector3D(624, 40, 0));
						ExitButton* eb = obj->AddComponent<ExitButton>();
						ZeldaUI* ui = obj->AddComponent<ZeldaUI>();
						Image* image = obj->AddComponent<Image>();
						image->SetTexture(L"Exit_Ingame_Idle");
						image->SetLayer(920);
						Button* button = obj->AddComponent<Button>();
						//button->SetImage(L"Exit_Idle", L"Exit_Enter", L"Exit_Click");

						// Attach UI
						{
							Vector2D uiSize = { 70, 70 };

							GameObject* attachUI = SceneManager::GetInstance().CreateGameObject(L"Exit_AttachUI");
							attachUI->GetTransform()->SetParent(obj->GetTransform());
							attachUI->GetTransform()->SetLocalScale(Vector3D(uiSize.x, uiSize.y));
							attachUI->GetTransform()->SetLocalPosition(Vector3D(-157.30, 0, 0));
							AttachUI* atp = attachUI->AddComponent<AttachUI>();
							ZeldaUI* ui = attachUI->AddComponent<ZeldaUI>();
							Image* image = attachUI->AddComponent<Image>();
							image->SetTexture(L"AttachUI");
							image->SetLayer(950);

							eb->SetAttachUI(*atp);
							eb->SetUISize(size);
							eb->SetAttachUISize(uiSize);

							attachUI->SetActive(false);
						}

						bsp->AddButton(*obj);
					}

					// Title Button
					{
						Vector2D size = { 250.2, 86 };

						GameObject* obj = SceneManager::GetInstance().CreateGameObject(L"Setting_TitleButton");
						obj->GetTransform()->SetParent(settingObj->GetTransform());
						obj->GetTransform()->SetLocalScale(Vector3D(size.x, size.y, 0));
						obj->GetTransform()->SetLocalPosition(Vector3D(624, -90, 0));
						TitleButton* tb = obj->AddComponent <TitleButton>();
						ZeldaUI* ui = obj->AddComponent<ZeldaUI>();
						Image* image = obj->AddComponent<Image>();
						image->SetTexture(L"Title_Idle");
						image->SetLayer(920);
						Button* button = obj->AddComponent<Button>();
						//button->SetImage(L"Title_Idle", L"Title_Enter", L"Title_Click");

						// Attach UI
						{
							Vector2D uiSize = { 70, 70 };

							GameObject* attachUI = SceneManager::GetInstance().CreateGameObject(L"Exit_AttachUI");
							attachUI->GetTransform()->SetParent(obj->GetTransform());
							attachUI->GetTransform()->SetLocalScale(Vector3D(uiSize.x, uiSize.y));
							attachUI->GetTransform()->SetLocalPosition(Vector3D(-157.30, 0, 0));
							AttachUI* atp = attachUI->AddComponent<AttachUI>();
							ZeldaUI* ui = attachUI->AddComponent<ZeldaUI>();
							Image* image = attachUI->AddComponent<Image>();
							image->SetTexture(L"AttachUI");
							image->SetLayer(950);

							tb->SetAttachUI(*atp);
							tb->SetUISize(size);
							tb->SetAttachUISize(uiSize);

							attachUI->SetActive(false);
						}

						bsp->AddButton(*obj);
					}

					setbt->SetPanel(*settingObj->GetTransform());
				}
			}

			SaveScene();


			SceneManager::GetInstance().DeleteAllGameObject();
			SceneManager::GetInstance().DeleteAllDontDestGameObject();

			{
				SceneManager::GetInstance().SetName(L"GameSceneBossPhase3");

				// 게임 매니저
				GameObject* gameManagerObject = SceneManager::GetInstance().CreateGameObject(L"GameManager");
				GameManager* gameManager = gameManagerObject->AddComponent<GameManager>();
				{
					GameObject* coverObject = SceneManager::GetInstance().CreateGameObject(L"CoverObject");
					coverObject->AddComponent<ZeldaUI>();
					Image* coverImage = coverObject->AddComponent<Image>();
					coverImage->SetLayer(901);
					coverObject->GetTransform()->SetLocalScale(Vector3D(1920, 1080));
					SpriteAnimator* mainanimator = coverObject->AddComponent<SpriteAnimator>();
					gameManager->SetMainAnimator(mainanimator);
				}
				{
					GameObject* coverObject = SceneManager::GetInstance().CreateGameObject(L"CoverObject");
					coverObject->AddComponent<ZeldaUI>();
					Image* coverImage = coverObject->AddComponent<Image>();
					coverImage->SetLayer(900);
					coverObject->GetTransform()->SetLocalScale(Vector3D(1920, 1080));
					SpriteAnimator* subanimator = coverObject->AddComponent<SpriteAnimator>();
					gameManager->SetSubAnimator(subanimator);
				}
				{
					GameObject* coverObject = SceneManager::GetInstance().CreateGameObject(L"CoverObject");
					coverObject->AddComponent<ZeldaUI>();
					Image* coverImage = coverObject->AddComponent<Image>();
					coverImage->SetLayer(899);
					coverObject->GetTransform()->SetLocalScale(Vector3D(1920, 1080));
					SpriteAnimator* thirdanimator = coverObject->AddComponent<SpriteAnimator>();
					gameManager->SetThirdAnimator(thirdanimator);
				}

				// 카메라
				GameObject* cameraObject = SceneManager::GetInstance().CreateGameObject(L"MainCamera");
				cameraObject->GetTransform()->SetLocalPosition(Vector3D(0, 0, 10));
				Camera* camera = cameraObject->AddComponent<Camera>();
				SceneManager::GetInstance().SetMainCamera(camera);

				TestComponent* testcomponent = SceneManager::GetInstance().GetMainCamera()->GetGameObject()->AddComponent<TestComponent>();

				// 라이트
				GameObject* lightObject = SceneManager::GetInstance().CreateGameObject(L"DirectionalLight");
				DirectionalLight* directionalLight = lightObject->AddComponent<DirectionalLight>();
				lightObject->GetTransform()->SetLocalRotation(Vector3D(70, 30, 0));

				GameObject* phase3EventManagerObject = SceneManager::GetInstance().CreateGameObject(L"Phase3EventManager");
				Phase3EventManager* phase3EventManager = phase3EventManagerObject->AddComponent<Phase3EventManager>();
				phase3EventManager->SetGameManager(gameManager);

				// 보스
				GameObject* Boss = CreateGameObjectFromFBX(L"Boss");
				Boss->AddComponent<Phase3Character>()->SetSkinnedMeshRenderer(Boss->GetTransform()->GetChild(0)->GetGameObject()->GetComponent<SkinnedMeshRenderer>());
				Animator* bossAnim = Boss->AddComponent<Animator>();
				FF::FFAnimationClip& phase3BossClip = GetAnimationClip(L"root|Phase3_Boss");
				phase3BossClip.isLoop = true;
				bossAnim->PlusAnimationClip(GetAnimationClip(L"root|Phase3_Boss"));
				bossAnim->Play(L"root|Phase3_Boss");

				// 플레이어
				GameObject* player = CreateGameObjectFromFBX(L"Player");
				player->AddComponent<Phase3Character>()->SetSkinnedMeshRenderer(player->GetTransform()->GetChild(0)->GetGameObject()->GetComponent<SkinnedMeshRenderer>());
				Animator* playerAnim = player->AddComponent<Animator>();
				FF::FFAnimationClip& phase3PlayerClip = GetAnimationClip(L"Armature|Phase3_Player");
				phase3PlayerClip.isLoop = true;
				playerAnim->PlusAnimationClip(phase3PlayerClip);
				playerAnim->Play(L"Armature|Phase3_Player");

				// 배경
				for (int i = 0; i < 6; ++i)
				{
					GameObject* backGroud = SceneManager::GetInstance().CreateGameObject(L"BackGroud");
					MeshRenderer* renderer = backGroud->AddComponent<MeshRenderer>();
					renderer->SetMesh(L"Cube");
					renderer->SetMaterial(L"CubeMaterial_F");
				}

			}

			SaveScene();

			SceneManager::GetInstance().DeleteAllGameObject();
			SceneManager::GetInstance().DeleteAllDontDestGameObject();
		}

		{
			SceneManager::GetInstance().SetName(L"GameOverScene");

			// 카메라
			GameObject* cameraObject = SceneManager::GetInstance().CreateGameObject(L"MainCamera");
			Camera* camera = cameraObject->AddComponent<Camera>();
			SceneManager::GetInstance().SetMainCamera(camera);

			// 게임 매니저
			GameObject* gameManagerObject = SceneManager::GetInstance().CreateGameObject(L"GameManager");
			GameManager* gameManager = gameManagerObject->AddComponent<GameManager>();

			{
				GameObject* coverObject = SceneManager::GetInstance().CreateGameObject(L"CoverObject");
				coverObject->AddComponent<ZeldaUI>();
				Image* coverImage = coverObject->AddComponent<Image>();
				coverImage->SetLayer(901);
				coverObject->GetTransform()->SetLocalScale(Vector3D(1920, 1080));
				SpriteAnimator* mainanimator = coverObject->AddComponent<SpriteAnimator>();
				gameManager->SetMainAnimator(mainanimator);
			}

			{
				GameObject* coverObject = SceneManager::GetInstance().CreateGameObject(L"CoverObject");
				coverObject->AddComponent<ZeldaUI>();
				Image* coverImage = coverObject->AddComponent<Image>();
				coverImage->SetLayer(900);
				coverObject->GetTransform()->SetLocalScale(Vector3D(1920, 1080));
				SpriteAnimator* subanimator = coverObject->AddComponent<SpriteAnimator>();
				gameManager->SetSubAnimator(subanimator);
			}

			{
				GameObject* coverObject = SceneManager::GetInstance().CreateGameObject(L"CoverObject");
				coverObject->AddComponent<ZeldaUI>();
				Image* coverImage = coverObject->AddComponent<Image>();
				coverImage->SetLayer(899);
				coverObject->GetTransform()->SetLocalScale(Vector3D(1920, 1080));
				SpriteAnimator* thirdanimator = coverObject->AddComponent<SpriteAnimator>();
				gameManager->SetThirdAnimator(thirdanimator);
			}

			// 게임오버 배경
			{
				Vector2D backgroundSize = { 1920, 1080 };

				GameObject* object = SceneManager::GetInstance().CreateGameObject(L"BackGround");
				object->GetTransform()->SetLocalScale(Vector3D(backgroundSize.x, backgroundSize.y, 0));
				ZeldaUI* ui = object->AddComponent<ZeldaUI>();
				Image* image = object->AddComponent<Image>();
				image->SetTexture(L"GameOverBackground");
				image->SetLayer(-900);
			}

			// 게임오버 버튼
			{
				GameObject* gameOverButtonGroup = SceneManager::GetInstance().CreateGameObject(L"GameOverButtonGroup");
				gameOverButtonGroup->GetTransform()->SetLocalPosition(Vector3D(0, -220, 0));

				// 키보드 입력에 따른 버튼 조정

				GameObject* bs = SceneManager::GetInstance().CreateGameObject(L"ButtonSelector");
				bs->GetTransform()->SetParent(gameOverButtonGroup->GetTransform());
				ButtonSelector* bsp = bs->AddComponent<ButtonSelector>();

				// 타이틀 버튼
				{
					Vector2D buttonSize = { 319, 157 };

					GameObject* object = SceneManager::GetInstance().CreateGameObject(L"TitleButton");
					object->GetTransform()->SetParent(gameOverButtonGroup->GetTransform());
					object->GetTransform()->SetLocalScale(Vector3D(buttonSize.x, buttonSize.y, 0));
					object->GetTransform()->SetLocalPosition(Vector3D(180,0,0));
					TitleButton* eb = object->AddComponent<TitleButton>();
					ZeldaUI* ui = object->AddComponent<ZeldaUI>();
					Image* image = object->AddComponent<Image>();
					image->SetTexture(L"Title_GameOver_Idle");
					Button* button = object->AddComponent<Button>();

					bsp->AddButton(*object);

					// Attach UI
					{
						Vector2D uiSize = { 70, 70 };

						GameObject* attachUI = SceneManager::GetInstance().CreateGameObject(L"Title_AttachUI");
						attachUI->GetTransform()->SetParent(object->GetTransform());
						attachUI->GetTransform()->SetLocalScale(Vector3D(uiSize.x, uiSize.y));
						attachUI->GetTransform()->SetLocalPosition(Vector3D(-164,0));
						AttachUI* atp = attachUI->AddComponent<AttachUI>();
						ZeldaUI* ui = attachUI->AddComponent<ZeldaUI>();
						Image* image = attachUI->AddComponent<Image>();
						image->SetTexture(L"AttachUI");

						eb->SetAttachUI(*atp);
						eb->SetUISize(buttonSize);
						eb->SetAttachUISize(uiSize);

						attachUI->SetActive(false);
					}
				}

				// 게임재시작 버튼
				{
					Vector2D buttonSize = { 319.5, 157 };

					GameObject* object = SceneManager::GetInstance().CreateGameObject(L"GameRestartButton");
					object->GetTransform()->SetParent(gameOverButtonGroup->GetTransform());
					object->GetTransform()->SetLocalScale(Vector3D(buttonSize.x, buttonSize.y, 0));
					object->GetTransform()->SetLocalPosition(Vector3D(-180, 4.8, 0));
					RestartButton* sb = object->AddComponent<RestartButton>();
					ZeldaUI* ui = object->AddComponent<ZeldaUI>();
					Image* image = object->AddComponent<Image>();
					image->SetTexture(L"Restart_Idle");
					Button* button = object->AddComponent<Button>();

					bsp->AddButton(*object);
					bsp->SetFirstButton(*object);

					// Attach UI
					{
						Vector2D uiSize = { 70, 70 };

						GameObject* attachUI = SceneManager::GetInstance().CreateGameObject(L"Restart_AttachUI");
						attachUI->GetTransform()->SetParent(object->GetTransform());
						attachUI->GetTransform()->SetLocalScale(Vector3D(uiSize.x, uiSize.y, 0));
						attachUI->GetTransform()->SetLocalPosition(Vector3D(-164, 0));
						AttachUI* atp = attachUI->AddComponent<AttachUI>();
						ZeldaUI* ui = attachUI->AddComponent<ZeldaUI>();
						Image* image = attachUI->AddComponent<Image>();
						image->SetTexture(L"AttachUI");

						sb->SetAttachUI(*atp);
						sb->SetUISize(buttonSize);
						sb->SetAttachUISize(uiSize);

						attachUI->SetActive(false);
					}
				}
			}
			SaveScene();


			SceneManager::GetInstance().DeleteAllGameObject();
			SceneManager::GetInstance().DeleteAllDontDestGameObject();

			{
				SceneManager::GetInstance().SetName(L"StaffRollScene");

				// 게임 매니저
				GameObject* gameManagerObject = SceneManager::GetInstance().CreateGameObject(L"GameManager");
				GameManager* gameManager = gameManagerObject->AddComponent<GameManager>();

				{
					GameObject* coverObject = SceneManager::GetInstance().CreateGameObject(L"CoverObject");
					coverObject->AddComponent<ZeldaUI>();
					Image* coverImage = coverObject->AddComponent<Image>();
					coverImage->SetLayer(901);
					coverObject->GetTransform()->SetLocalScale(Vector3D(1920, 1080));
					SpriteAnimator* mainanimator = coverObject->AddComponent<SpriteAnimator>();
					gameManager->SetMainAnimator(mainanimator);
				}

				{
					GameObject* coverObject = SceneManager::GetInstance().CreateGameObject(L"CoverObject");
					coverObject->AddComponent<ZeldaUI>();
					Image* coverImage = coverObject->AddComponent<Image>();
					coverImage->SetLayer(900);
					coverObject->GetTransform()->SetLocalScale(Vector3D(1920, 1080));
					SpriteAnimator* subanimator = coverObject->AddComponent<SpriteAnimator>();
					gameManager->SetSubAnimator(subanimator);
				}

				{
					GameObject* coverObject = SceneManager::GetInstance().CreateGameObject(L"CoverObject");
					coverObject->AddComponent<ZeldaUI>();
					Image* coverImage = coverObject->AddComponent<Image>();
					coverImage->SetLayer(899);
					coverObject->GetTransform()->SetLocalScale(Vector3D(1920, 1080));
					SpriteAnimator* thirdanimator = coverObject->AddComponent<SpriteAnimator>();
					gameManager->SetThirdAnimator(thirdanimator);
				}

				// 카메라
				GameObject* cameraObject = SceneManager::GetInstance().CreateGameObject(L"MainCamera");
				Camera* camera = cameraObject->AddComponent<Camera>();
				SceneManager::GetInstance().SetMainCamera(camera);

				GameObject* lightObject = SceneManager::GetInstance().CreateGameObject(L"DirectionalLight");
				DirectionalLight* directionalLight = lightObject->AddComponent<DirectionalLight>();
				lightObject->GetTransform()->SetLocalRotation(Vector3D(70, 30, 0));

				GameObject* staffRollObj = SceneManager::GetInstance().CreateGameObject(L"staffRollObj");
				staffRollObj->GetTransform()->SetLocalPosition(Vector3D(0, -960, 0));
				staffRollObj->AddComponent<StaffRoll>();
				staffRollObj->AddComponent<ZeldaUI>();
				Image* image = staffRollObj->AddComponent<Image>();
				image->SetTexture(L"StaffRoll");
				staffRollObj->GetTransform()->SetLocalScale(Vector3D(1920, 3000));
			}
			SaveScene();


			SceneManager::GetInstance().DeleteAllGameObject();
			SceneManager::GetInstance().DeleteAllDontDestGameObject();

			{
				SceneManager::GetInstance().SetName(L"UnnecessaryScene");

				// 게임 매니저
				GameObject* gameManagerObject = SceneManager::GetInstance().CreateGameObject(L"GameManager");
				GameManager* gameManager = gameManagerObject->AddComponent<GameManager>();

				{
					GameObject* coverObject = SceneManager::GetInstance().CreateGameObject(L"CoverObject");
					coverObject->AddComponent<ZeldaUI>();
					Image* coverImage = coverObject->AddComponent<Image>();
					coverImage->SetLayer(901);
					coverObject->GetTransform()->SetLocalScale(Vector3D(1920, 1080));
					SpriteAnimator* mainanimator = coverObject->AddComponent<SpriteAnimator>();
					gameManager->SetMainAnimator(mainanimator);
				}

				{
					GameObject* coverObject = SceneManager::GetInstance().CreateGameObject(L"CoverObject");
					coverObject->AddComponent<ZeldaUI>();
					Image* coverImage = coverObject->AddComponent<Image>();
					coverImage->SetLayer(900);
					coverObject->GetTransform()->SetLocalScale(Vector3D(1920, 1080));
					SpriteAnimator* subanimator = coverObject->AddComponent<SpriteAnimator>();
					gameManager->SetSubAnimator(subanimator);
				}

				{
					GameObject* coverObject = SceneManager::GetInstance().CreateGameObject(L"CoverObject");
					coverObject->AddComponent<ZeldaUI>();
					Image* coverImage = coverObject->AddComponent<Image>();
					coverImage->SetLayer(899);
					coverObject->GetTransform()->SetLocalScale(Vector3D(1920, 1080));
					SpriteAnimator* thirdanimator = coverObject->AddComponent<SpriteAnimator>();
					gameManager->SetThirdAnimator(thirdanimator);
				}

				GameObject* cameraObject = SceneManager::GetInstance().CreateGameObject(L"MainCamera");
				cameraObject->AddComponent<TestComponent>();
				Camera* camera = cameraObject->AddComponent<Camera>();
				SceneManager::GetInstance().SetMainCamera(camera);
				cameraObject->GetTransform()->SetLocalPosition(Vector3D(0, 0, -100));

				GameObject* lightObject = SceneManager::GetInstance().CreateGameObject(L"DirectionalLight");
				DirectionalLight* directionalLight = lightObject->AddComponent<DirectionalLight>();
				lightObject->GetTransform()->SetLocalRotation(Vector3D(70, 30, 0));

				GameObject* floorObject = SceneManager::GetInstance().CreateGameObject(L"Floor");
				floorObject->SetLayerName(L"Weird");
				floorObject->AddComponent<CylinderCollider>();
				floorObject->AddComponent<Rigidbody>()->SetKinematic(true);

				floorObject->GetTransform()->SetLocalPosition(Vector3D(10, 0, 50));
				floorObject->GetTransform()->SetLocalScale(Vector3D(20, 20, 20));

				GameObject* weirdObject = SceneManager::GetInstance().CreateGameObject(L"weirdObject");
				weirdObject->SetLayerName(L"Weird");
				weirdObject->AddComponent<TestComponent>();
				weirdObject->AddComponent<CapsuleCollider>();
				weirdObject->AddComponent<Rigidbody>()->SetKinematic(true);

				weirdObject->GetTransform()->SetLocalPosition(Vector3D(-10, 0, 50));
				weirdObject->GetTransform()->SetLocalScale(Vector3D(20, 20, 20));
			}
			SaveScene();

			SceneManager::GetInstance().DeleteAllGameObject();
			SceneManager::GetInstance().DeleteAllDontDestGameObject();

			{
				SceneManager::GetInstance().SetName(L"UnnecessaryScene2");

				// 게임 매니저
				GameObject* gameManagerObject = SceneManager::GetInstance().CreateGameObject(L"GameManager");
				GameManager* gameManager = gameManagerObject->AddComponent<GameManager>();

				{
					GameObject* coverObject = SceneManager::GetInstance().CreateGameObject(L"CoverObject");
					coverObject->AddComponent<ZeldaUI>();
					Image* coverImage = coverObject->AddComponent<Image>();
					coverImage->SetLayer(901);
					coverObject->GetTransform()->SetLocalScale(Vector3D(1920, 1080));
					SpriteAnimator* mainanimator = coverObject->AddComponent<SpriteAnimator>();
					gameManager->SetMainAnimator(mainanimator);
				}

				{
					GameObject* coverObject = SceneManager::GetInstance().CreateGameObject(L"CoverObject");
					coverObject->AddComponent<ZeldaUI>();
					Image* coverImage = coverObject->AddComponent<Image>();
					coverImage->SetLayer(900);
					coverObject->GetTransform()->SetLocalScale(Vector3D(1920, 1080));
					SpriteAnimator* subanimator = coverObject->AddComponent<SpriteAnimator>();
					gameManager->SetSubAnimator(subanimator);
				}

				{
					GameObject* coverObject = SceneManager::GetInstance().CreateGameObject(L"CoverObject");
					coverObject->AddComponent<ZeldaUI>();
					Image* coverImage = coverObject->AddComponent<Image>();
					coverImage->SetLayer(899);
					coverObject->GetTransform()->SetLocalScale(Vector3D(1920, 1080));
					SpriteAnimator* thirdanimator = coverObject->AddComponent<SpriteAnimator>();
					gameManager->SetThirdAnimator(thirdanimator);
				}

				GameObject* cameraObject = SceneManager::GetInstance().CreateGameObject(L"MainCamera");
				cameraObject->AddComponent<TestComponent>();
				Camera* camera = cameraObject->AddComponent<Camera>();
				SceneManager::GetInstance().SetMainCamera(camera);
				cameraObject->GetTransform()->SetLocalPosition(Vector3D(0, 0, -100));

				GameObject* lightObject = SceneManager::GetInstance().CreateGameObject(L"DirectionalLight");
				DirectionalLight* directionalLight = lightObject->AddComponent<DirectionalLight>();
				lightObject->GetTransform()->SetLocalRotation(Vector3D(70, 30, 0));

				GameObject* floorObject = SceneManager::GetInstance().CreateGameObject(L"Floor");
				floorObject->SetLayerName(L"Weird");
				floorObject->AddComponent<CubeCollider>();
				Rigidbody* floorRigidbody = floorObject->AddComponent<Rigidbody>();
				floorRigidbody->SetKinematic(true);

				floorObject->GetTransform()->SetLocalScale(Vector3D(50, 1, 50));

				GameObject* weirdObject = SceneManager::GetInstance().CreateGameObject(L"weirdObject");
				weirdObject->AddComponent<RandomRotate>();
			}
			SaveScene();

			SceneManager::GetInstance().DeleteAllGameObject();
			SceneManager::GetInstance().DeleteAllDontDestGameObject();

			{
				SceneManager::GetInstance().SetName(L"UnnecessaryScene3");

				// 게임 매니저
				GameObject* gameManagerObject = SceneManager::GetInstance().CreateGameObject(L"GameManager");
				GameManager* gameManager = gameManagerObject->AddComponent<GameManager>();

				{
					GameObject* coverObject = SceneManager::GetInstance().CreateGameObject(L"CoverObject");
					coverObject->AddComponent<ZeldaUI>();
					Image* coverImage = coverObject->AddComponent<Image>();
					coverImage->SetLayer(901);
					coverObject->GetTransform()->SetLocalScale(Vector3D(1920, 1080));
					SpriteAnimator* mainanimator = coverObject->AddComponent<SpriteAnimator>();
					gameManager->SetMainAnimator(mainanimator);
				}

				{
					GameObject* coverObject = SceneManager::GetInstance().CreateGameObject(L"CoverObject");
					coverObject->AddComponent<ZeldaUI>();
					Image* coverImage = coverObject->AddComponent<Image>();
					coverImage->SetLayer(900);
					coverObject->GetTransform()->SetLocalScale(Vector3D(1920, 1080));
					SpriteAnimator* subanimator = coverObject->AddComponent<SpriteAnimator>();
					gameManager->SetSubAnimator(subanimator);
				}

				{
					GameObject* coverObject = SceneManager::GetInstance().CreateGameObject(L"CoverObject");
					coverObject->AddComponent<ZeldaUI>();
					Image* coverImage = coverObject->AddComponent<Image>();
					coverImage->SetLayer(899);
					coverObject->GetTransform()->SetLocalScale(Vector3D(1920, 1080));
					SpriteAnimator* thirdanimator = coverObject->AddComponent<SpriteAnimator>();
					gameManager->SetThirdAnimator(thirdanimator);
				}

				GameObject* cameraObject = SceneManager::GetInstance().CreateGameObject(L"MainCamera");
				cameraObject->AddComponent<TestComponent>();
				Camera* camera = cameraObject->AddComponent<Camera>();
				SceneManager::GetInstance().SetMainCamera(camera);
				cameraObject->GetTransform()->SetLocalPosition(Vector3D(0, 0, -100));

				GameObject* lightObject = SceneManager::GetInstance().CreateGameObject(L"DirectionalLight");
				DirectionalLight* directionalLight = lightObject->AddComponent<DirectionalLight>();
				lightObject->GetTransform()->SetLocalRotation(Vector3D(70, 30, 0));

				GameObject* floorObject = SceneManager::GetInstance().CreateGameObject(L"Floor");
				floorObject->SetLayerName(L"Weird");
				floorObject->AddComponent<CubeCollider>();
				Rigidbody* floorRigidbody = floorObject->AddComponent<Rigidbody>();
				floorRigidbody->SetKinematic(true);

				floorObject->GetTransform()->SetLocalScale(Vector3D(50, 1, 50));

				GameObject* weirdObject = SceneManager::GetInstance().CreateGameObject(L"weirdObject");
				weirdObject->AddComponent<RandomRotate>();
			}
			SaveScene();

		}
	}

	SoundManager::GetInstance().StopAll();
	SoundManager::GetInstance().StopAllBGM();

	// 그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워
	// 그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워
	// 그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워
	/* 그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워 */SceneManager::GetInstance().DeleteAllGameObject();			// 그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워
	/* 그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워 */SceneManager::GetInstance().DeleteAllDontDestGameObject();	// 그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워
	/* 그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워 */LoadScene(EngineSetting::GetInstance().GetStartScene());	// 그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워
	// 그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워
	// 그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워
	// 그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워그만지워

#pragma region MyRegion1
	//SceneManager::GetInstance().DeleteAllGameObject();
	//SceneManager::GetInstance().DeleteAllDontDestGameObject();
	//LoadScene(EngineSetting::GetInstance().GetStartScene());
	//SceneManager::GetInstance().DeleteAllGameObject();
	//SceneManager::GetInstance().DeleteAllDontDestGameObject();
	//LoadScene(EngineSetting::GetInstance().GetStartScene());
	//SceneManager::GetInstance().DeleteAllGameObject();
	//SceneManager::GetInstance().DeleteAllDontDestGameObject();
	//LoadScene(EngineSetting::GetInstance().GetStartScene());
	//SceneManager::GetInstance().DeleteAllGameObject();
	//SceneManager::GetInstance().DeleteAllDontDestGameObject();
	//LoadScene(EngineSetting::GetInstance().GetStartScene());
	//SceneManager::GetInstance().DeleteAllGameObject();
	//SceneManager::GetInstance().DeleteAllDontDestGameObject();
	//LoadScene(EngineSetting::GetInstance().GetStartScene());
	//SceneManager::GetInstance().DeleteAllGameObject();
	//SceneManager::GetInstance().DeleteAllDontDestGameObject();
	//LoadScene(EngineSetting::GetInstance().GetStartScene());
	//SceneManager::GetInstance().DeleteAllGameObject();
	//SceneManager::GetInstance().DeleteAllDontDestGameObject();
	//LoadScene(EngineSetting::GetInstance().GetStartScene());
	//SceneManager::GetInstance().DeleteAllGameObject();
	//SceneManager::GetInstance().DeleteAllDontDestGameObject();
	//LoadScene(EngineSetting::GetInstance().GetStartScene());
	//SceneManager::GetInstance().DeleteAllGameObject();
	//SceneManager::GetInstance().DeleteAllDontDestGameObject();
	//LoadScene(EngineSetting::GetInstance().GetStartScene());
	//SceneManager::GetInstance().DeleteAllGameObject();
	//SceneManager::GetInstance().DeleteAllDontDestGameObject();
	//LoadScene(EngineSetting::GetInstance().GetStartScene());
	//SceneManager::GetInstance().DeleteAllGameObject();
	//SceneManager::GetInstance().DeleteAllDontDestGameObject();
	//LoadScene(EngineSetting::GetInstance().GetStartScene());
	//SceneManager::GetInstance().DeleteAllGameObject();
	//SceneManager::GetInstance().DeleteAllDontDestGameObject();
	//LoadScene(EngineSetting::GetInstance().GetStartScene());
	//SceneManager::GetInstance().DeleteAllGameObject();
	//SceneManager::GetInstance().DeleteAllDontDestGameObject();
	//LoadScene(EngineSetting::GetInstance().GetStartScene());
	//SceneManager::GetInstance().DeleteAllGameObject();
	//SceneManager::GetInstance().DeleteAllDontDestGameObject();
	//LoadScene(EngineSetting::GetInstance().GetStartScene());
	//SceneManager::GetInstance().DeleteAllGameObject();
	//SceneManager::GetInstance().DeleteAllDontDestGameObject();
	//SceneManager::GetInstance().DeleteAllGameObject();
	//SceneManager::GetInstance().DeleteAllDontDestGameObject();
	//LoadScene(EngineSetting::GetInstance().GetStartScene());
	//SceneManager::GetInstance().DeleteAllGameObject();
	//SceneManager::GetInstance().DeleteAllDontDestGameObject();
	//LoadScene(EngineSetting::GetInstance().GetStartScene());
	//SceneManager::GetInstance().DeleteAllGameObject();
	//SceneManager::GetInstance().DeleteAllDontDestGameObject();
	//LoadScene(EngineSetting::GetInstance().GetStartScene());
	//SceneManager::GetInstance().DeleteAllGameObject();
	//SceneManager::GetInstance().DeleteAllDontDestGameObject();
	//LoadScene(EngineSetting::GetInstance().GetStartScene());
	//SceneManager::GetInstance().DeleteAllGameObject();
	//SceneManager::GetInstance().DeleteAllDontDestGameObject();
	//LoadScene(EngineSetting::GetInstance().GetStartScene());
	//SceneManager::GetInstance().DeleteAllGameObject();
	//SceneManager::GetInstance().DeleteAllDontDestGameObject();
	//LoadScene(EngineSetting::GetInstance().GetStartScene());
	//SceneManager::GetInstance().DeleteAllGameObject();
	//SceneManager::GetInstance().DeleteAllDontDestGameObject();
	//LoadScene(EngineSetting::GetInstance().GetStartScene());
	//SceneManager::GetInstance().DeleteAllGameObject();
	//SceneManager::GetInstance().DeleteAllDontDestGameObject();
	//LoadScene(EngineSetting::GetInstance().GetStartScene());
	//SceneManager::GetInstance().DeleteAllGameObject();
	//SceneManager::GetInstance().DeleteAllDontDestGameObject();
	//LoadScene(EngineSetting::GetInstance().GetStartScene());
	//SceneManager::GetInstance().DeleteAllGameObject();
	//SceneManager::GetInstance().DeleteAllDontDestGameObject();
	//LoadScene(EngineSetting::GetInstance().GetStartScene());
	//SceneManager::GetInstance().DeleteAllGameObject();
	//SceneManager::GetInstance().DeleteAllDontDestGameObject();
	//LoadScene(EngineSetting::GetInstance().GetStartScene());
	//SceneManager::GetInstance().DeleteAllGameObject();
	//SceneManager::GetInstance().DeleteAllDontDestGameObject();
	//LoadScene(EngineSetting::GetInstance().GetStartScene());
	//SceneManager::GetInstance().DeleteAllGameObject();
	//SceneManager::GetInstance().DeleteAllDontDestGameObject();
	//LoadScene(EngineSetting::GetInstance().GetStartScene());
	//SceneManager::GetInstance().DeleteAllGameObject();
	//SceneManager::GetInstance().DeleteAllDontDestGameObject();
	//LoadScene(EngineSetting::GetInstance().GetStartScene());
	//SceneManager::GetInstance().DeleteAllGameObject();
	//SceneManager::GetInstance().DeleteAllDontDestGameObject();
	//LoadScene(EngineSetting::GetInstance().GetStartScene());
	//SceneManager::GetInstance().DeleteAllGameObject();
	//SceneManager::GetInstance().DeleteAllDontDestGameObject();
	//LoadScene(EngineSetting::GetInstance().GetStartScene());
	//SceneManager::GetInstance().DeleteAllGameObject();
	//SceneManager::GetInstance().DeleteAllDontDestGameObject();
	//LoadScene(EngineSetting::GetInstance().GetStartScene());
	//SceneManager::GetInstance().DeleteAllGameObject();
	//SceneManager::GetInstance().DeleteAllDontDestGameObject();
	//LoadScene(EngineSetting::GetInstance().GetStartScene());
	//SceneManager::GetInstance().DeleteAllGameObject();
	//SceneManager::GetInstance().DeleteAllDontDestGameObject();
	//LoadScene(EngineSetting::GetInstance().GetStartScene());
	//SceneManager::GetInstance().DeleteAllGameObject();
	//SceneManager::GetInstance().DeleteAllDontDestGameObject();
	//LoadScene(EngineSetting::GetInstance().GetStartScene());
	//SceneManager::GetInstance().DeleteAllGameObject();
	//SceneManager::GetInstance().DeleteAllDontDestGameObject();
	//LoadScene(EngineSetting::GetInstance().GetStartScene());
	//SceneManager::GetInstance().DeleteAllGameObject();
	//SceneManager::GetInstance().DeleteAllDontDestGameObject();
	//LoadScene(EngineSetting::GetInstance().GetStartScene());
	//SceneManager::GetInstance().DeleteAllGameObject();
	//SceneManager::GetInstance().DeleteAllDontDestGameObject();
	//LoadScene(EngineSetting::GetInstance().GetStartScene());
	//SceneManager::GetInstance().DeleteAllGameObject();
	//SceneManager::GetInstance().DeleteAllDontDestGameObject();
	//LoadScene(EngineSetting::GetInstance().GetStartScene());
	//SceneManager::GetInstance().DeleteAllGameObject();
	//SceneManager::GetInstance().DeleteAllDontDestGameObject();
	//LoadScene(EngineSetting::GetInstance().GetStartScene());
	//SceneManager::GetInstance().DeleteAllGameObject();
	//SceneManager::GetInstance().DeleteAllDontDestGameObject();
	//LoadScene(EngineSetting::GetInstance().GetStartScene());
	//SceneManager::GetInstance().DeleteAllGameObject();
	//SceneManager::GetInstance().DeleteAllDontDestGameObject();
	//LoadScene(EngineSetting::GetInstance().GetStartScene());
	//SceneManager::GetInstance().DeleteAllGameObject();
	//SceneManager::GetInstance().DeleteAllDontDestGameObject();
	//LoadScene(EngineSetting::GetInstance().GetStartScene());
	//SceneManager::GetInstance().DeleteAllGameObject();
	//SceneManager::GetInstance().DeleteAllDontDestGameObject();
	//LoadScene(EngineSetting::GetInstance().GetStartScene());
	//SceneManager::GetInstance().DeleteAllGameObject();
	//SceneManager::GetInstance().DeleteAllDontDestGameObject();
	//LoadScene(EngineSetting::GetInstance().GetStartScene());
#pragma endregion

#pragma region MyRegion2
	//SceneManager::GetInstance().DeleteAllGameObject();
	//SceneManager::GetInstance().DeleteAllDontDestGameObject();
	//LoadScene(EngineSetting::GetInstance().GetStartScene());
	//SceneManager::GetInstance().DeleteAllGameObject();
	//SceneManager::GetInstance().DeleteAllDontDestGameObject();
	//LoadScene(EngineSetting::GetInstance().GetStartScene());
	//SceneManager::GetInstance().DeleteAllGameObject();
	//SceneManager::GetInstance().DeleteAllDontDestGameObject();
	//LoadScene(EngineSetting::GetInstance().GetStartScene());
	//SceneManager::GetInstance().DeleteAllGameObject();
	//SceneManager::GetInstance().DeleteAllDontDestGameObject();
	//LoadScene(EngineSetting::GetInstance().GetStartScene());
	//SceneManager::GetInstance().DeleteAllGameObject();
	//SceneManager::GetInstance().DeleteAllDontDestGameObject();
	//LoadScene(EngineSetting::GetInstance().GetStartScene());
	//SceneManager::GetInstance().DeleteAllGameObject();
	//SceneManager::GetInstance().DeleteAllDontDestGameObject();
	//LoadScene(EngineSetting::GetInstance().GetStartScene());
	//SceneManager::GetInstance().DeleteAllGameObject();
	//SceneManager::GetInstance().DeleteAllDontDestGameObject();
	//LoadScene(EngineSetting::GetInstance().GetStartScene());
	//SceneManager::GetInstance().DeleteAllGameObject();
	//SceneManager::GetInstance().DeleteAllDontDestGameObject();
	//LoadScene(EngineSetting::GetInstance().GetStartScene());
	//SceneManager::GetInstance().DeleteAllGameObject();
	//SceneManager::GetInstance().DeleteAllDontDestGameObject();
	//LoadScene(EngineSetting::GetInstance().GetStartScene());
	//SceneManager::GetInstance().DeleteAllGameObject();
	//SceneManager::GetInstance().DeleteAllDontDestGameObject();
	//LoadScene(EngineSetting::GetInstance().GetStartScene());
	//SceneManager::GetInstance().DeleteAllGameObject();
	//SceneManager::GetInstance().DeleteAllDontDestGameObject();
	//LoadScene(EngineSetting::GetInstance().GetStartScene());
	//SceneManager::GetInstance().DeleteAllGameObject();
	//SceneManager::GetInstance().DeleteAllDontDestGameObject();
	//LoadScene(EngineSetting::GetInstance().GetStartScene());
	//SceneManager::GetInstance().DeleteAllGameObject();
	//SceneManager::GetInstance().DeleteAllDontDestGameObject();
	//LoadScene(EngineSetting::GetInstance().GetStartScene());
	//SceneManager::GetInstance().DeleteAllGameObject();
	//SceneManager::GetInstance().DeleteAllDontDestGameObject();
	//LoadScene(EngineSetting::GetInstance().GetStartScene());
	//SceneManager::GetInstance().DeleteAllGameObject();
	//SceneManager::GetInstance().DeleteAllDontDestGameObject();
	//SceneManager::GetInstance().DeleteAllGameObject();
	//SceneManager::GetInstance().DeleteAllDontDestGameObject();
	//LoadScene(EngineSetting::GetInstance().GetStartScene());
	//SceneManager::GetInstance().DeleteAllGameObject();
	//SceneManager::GetInstance().DeleteAllDontDestGameObject();
	//LoadScene(EngineSetting::GetInstance().GetStartScene());
	//SceneManager::GetInstance().DeleteAllGameObject();
	//SceneManager::GetInstance().DeleteAllDontDestGameObject();
	//LoadScene(EngineSetting::GetInstance().GetStartScene());
	//SceneManager::GetInstance().DeleteAllGameObject();
	//SceneManager::GetInstance().DeleteAllDontDestGameObject();
	//LoadScene(EngineSetting::GetInstance().GetStartScene());
	//SceneManager::GetInstance().DeleteAllGameObject();
	//SceneManager::GetInstance().DeleteAllDontDestGameObject();
	//LoadScene(EngineSetting::GetInstance().GetStartScene());
	//SceneManager::GetInstance().DeleteAllGameObject();
	//SceneManager::GetInstance().DeleteAllDontDestGameObject();
	//LoadScene(EngineSetting::GetInstance().GetStartScene());
	//SceneManager::GetInstance().DeleteAllGameObject();
	//SceneManager::GetInstance().DeleteAllDontDestGameObject();
	//LoadScene(EngineSetting::GetInstance().GetStartScene());
	//SceneManager::GetInstance().DeleteAllGameObject();
	//SceneManager::GetInstance().DeleteAllDontDestGameObject();
	//LoadScene(EngineSetting::GetInstance().GetStartScene());
	//SceneManager::GetInstance().DeleteAllGameObject();
	//SceneManager::GetInstance().DeleteAllDontDestGameObject();
	//LoadScene(EngineSetting::GetInstance().GetStartScene());
	//SceneManager::GetInstance().DeleteAllGameObject();
	//SceneManager::GetInstance().DeleteAllDontDestGameObject();
	//LoadScene(EngineSetting::GetInstance().GetStartScene());
	//SceneManager::GetInstance().DeleteAllGameObject();
	//SceneManager::GetInstance().DeleteAllDontDestGameObject();
	//LoadScene(EngineSetting::GetInstance().GetStartScene());
	//SceneManager::GetInstance().DeleteAllGameObject();
	//SceneManager::GetInstance().DeleteAllDontDestGameObject();
	//LoadScene(EngineSetting::GetInstance().GetStartScene());
	//SceneManager::GetInstance().DeleteAllGameObject();
	//SceneManager::GetInstance().DeleteAllDontDestGameObject();
	//LoadScene(EngineSetting::GetInstance().GetStartScene());
	//SceneManager::GetInstance().DeleteAllGameObject();
	//SceneManager::GetInstance().DeleteAllDontDestGameObject();
	//LoadScene(EngineSetting::GetInstance().GetStartScene());
	//SceneManager::GetInstance().DeleteAllGameObject();
	//SceneManager::GetInstance().DeleteAllDontDestGameObject();
	//LoadScene(EngineSetting::GetInstance().GetStartScene());
	//SceneManager::GetInstance().DeleteAllGameObject();
	//SceneManager::GetInstance().DeleteAllDontDestGameObject();
	//LoadScene(EngineSetting::GetInstance().GetStartScene());
	//SceneManager::GetInstance().DeleteAllGameObject();
	//SceneManager::GetInstance().DeleteAllDontDestGameObject();
	//LoadScene(EngineSetting::GetInstance().GetStartScene());
	//SceneManager::GetInstance().DeleteAllGameObject();
	/*SceneManager::GetInstance().DeleteAllDontDestGameObject();																																								*/ SceneManager::GetInstance().DeleteAllGameObject();
	/*LoadScene(EngineSetting::GetInstance().GetStartScene());																																									*/ SceneManager::GetInstance().DeleteAllDontDestGameObject();
	/*SceneManager::GetInstance().DeleteAllGameObject();																																										*/ LoadScene(EngineSetting::GetInstance().GetStartScene());
	//SceneManager::GetInstance().DeleteAllDontDestGameObject();
	//LoadScene(EngineSetting::GetInstance().GetStartScene());
	//SceneManager::GetInstance().DeleteAllGameObject();
	//SceneManager::GetInstance().DeleteAllDontDestGameObject();
	//LoadScene(EngineSetting::GetInstance().GetStartScene());
	//SceneManager::GetInstance().DeleteAllGameObject();
	//SceneManager::GetInstance().DeleteAllDontDestGameObject();
	//LoadScene(EngineSetting::GetInstance().GetStartScene());
	//SceneManager::GetInstance().DeleteAllGameObject();
	//SceneManager::GetInstance().DeleteAllDontDestGameObject();
	//LoadScene(EngineSetting::GetInstance().GetStartScene());
	//SceneManager::GetInstance().DeleteAllGameObject();
	//SceneManager::GetInstance().DeleteAllDontDestGameObject();
	//LoadScene(EngineSetting::GetInstance().GetStartScene());
	//SceneManager::GetInstance().DeleteAllGameObject();
	//SceneManager::GetInstance().DeleteAllDontDestGameObject();z
	//LoadScene(EngineSetting::GetInstance().GetStartScene());
	//SceneManager::GetInstance().DeleteAllGameObject();
	//SceneManager::GetInstance().DeleteAllDontDestGameObject();
	//LoadScene(EngineSetting::GetInstance().GetStartScene());
	//SceneManager::GetInstance().DeleteAllGameObject();
	//SceneManager::GetInstance().DeleteAllDontDestGameObject();
	//LoadScene(EngineSetting::GetInstance().GetStartScene());
	//SceneManager::GetInstance().DeleteAllGameObject();
	//SceneManager::GetInstance().DeleteAllDontDestGameObject();
	//LoadScene(EngineSetting::GetInstance().GetStartScene());
	//SceneManager::GetInstance().DeleteAllGameObject();
	//SceneManager::GetInstance().DeleteAllDontDestGameObject();
	//LoadScene(EngineSetting::GetInstance().GetStartScene());
	//SceneManager::GetInstance().DeleteAllGameObject();
	//SceneManager::GetInstance().DeleteAllDontDestGameObject();
	//LoadScene(EngineSetting::GetInstance().GetStartScene());
	//SceneManager::GetInstance().DeleteAllGameObject();
	//SceneManager::GetInstance().DeleteAllDontDestGameObject();
	//LoadScene(EngineSetting::GetInstance().GetStartScene());
#pragma 


	Run();

	//Finalize();

	return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 리소스

void CreateResourceBJ()
{
	// Mesh
	ResourceManager::GetInstance().CreateMesh(L"Cube", L"Cube");
	ResourceManager::GetInstance().CreateMesh(L"Sphere", L"Sphere");
	ResourceManager::GetInstance().CreateMesh(L"Capsule", L"Capsule");
	ResourceManager::GetInstance().CreateMesh(L"Cylinder", L"Cylinder");
	ResourceManager::GetInstance().CreateMesh(L"Rectangle", L"Rectangle");
	ResourceManager::GetInstance().CreateMesh(L"UI", L"Rectangle");
	ResourceManager::GetInstance().CreateMesh(L"Sprite", L"Rectangle");
	ResourceManager::GetInstance().CreateMesh(L"Line", L"Line");

	// Shader
	ShaderInfo spriteShaderInfo;
	spriteShaderInfo.shaderType = SHADER_TYPE::FORWARD;
	spriteShaderInfo.blendType = BLEND_TYPE::ALPHA_BLEND;
	ResourceManager::GetInstance().CreateShader(L"Sprite", L"..\\Resources\\Shader\\Sprite3D.hlsli", spriteShaderInfo);

	ShaderInfo uiShaderInfo;
	uiShaderInfo.rasterizerType = RASTERIZER_TYPE::CULL_NONE;
	uiShaderInfo.depthStencilType = DEPTH_STENCIL_TYPE::NO_DEPTH_TEST_NO_WRITE;
	uiShaderInfo.shaderType = SHADER_TYPE::UI;
	uiShaderInfo.blendType = BLEND_TYPE::ALPHA_BLEND;
	ResourceManager::GetInstance().CreateShader(L"UI", L"..\\Resources\\Shader\\UI.hlsli", uiShaderInfo);

	ShaderInfo deferredShaderInfo;
	deferredShaderInfo.shaderType = SHADER_TYPE::DEFERRED;
	ResourceManager::GetInstance().CreateShader(L"Deferred", L"..\\Resources\\Shader\\deferred.hlsli", deferredShaderInfo);

	ShaderInfo forwardShaderInfo;
	ResourceManager::GetInstance().CreateShader(L"Forward", L"..\\Resources\\Shader\\forward.hlsli", forwardShaderInfo);

	ShaderInfo skyBoxShaderInfo;
	skyBoxShaderInfo.rasterizerType = RASTERIZER_TYPE::CULL_FRONT;
	skyBoxShaderInfo.depthStencilType = DEPTH_STENCIL_TYPE::LESS_EQUAL;
	ResourceManager::GetInstance().CreateShader(L"SkyBox", L"..\\Resources\\Shader\\skybox.hlsli", skyBoxShaderInfo);

	ShaderInfo debugShaderInfo;
	debugShaderInfo.shaderType = SHADER_TYPE::FORWARD;
	debugShaderInfo.rasterizerType = RASTERIZER_TYPE::WIREFRAME;
	ResourceManager::GetInstance().CreateShader(L"Debug", L"..\\Resources\\Shader\\debug.hlsli", debugShaderInfo);

	ShaderInfo debugLineShaderInfo;
	debugLineShaderInfo.shaderType = SHADER_TYPE::FORWARD;
	debugLineShaderInfo.rasterizerType = RASTERIZER_TYPE::WIREFRAME;
	debugLineShaderInfo.topology = FFTopology::D3D_PRIMITIVE_TOPOLOGY_LINELIST;
	ResourceManager::GetInstance().CreateShader(L"Debug_Line", L"..\\Resources\\Shader\\debug_line.hlsli", debugLineShaderInfo);

	ShaderInfo lightingDirectionalShaderInfo;
	lightingDirectionalShaderInfo.shaderType = SHADER_TYPE::LIGHTING;
	lightingDirectionalShaderInfo.rasterizerType = RASTERIZER_TYPE::CULL_NONE;
	lightingDirectionalShaderInfo.depthStencilType = DEPTH_STENCIL_TYPE::NO_DEPTH_TEST_NO_WRITE;
	lightingDirectionalShaderInfo.blendType = BLEND_TYPE::ONE_TO_ONE_BLEND;
	lightingDirectionalShaderInfo.vs = "VS_DirLight";
	lightingDirectionalShaderInfo.ps = "PS_DirLight";
	ResourceManager::GetInstance().CreateShader(L"LightingDirectional", L"..\\Resources\\Shader\\lightingDirectional.hlsli", lightingDirectionalShaderInfo);

	ShaderInfo skinnedMeshShader;
	skinnedMeshShader.shaderType = SHADER_TYPE::DEFERRED_AND_SKIN;
	ResourceManager::GetInstance().CreateShader(L"SkinnedMesh", L"..\\Resources\\Shader\\deferred_skinned.hlsli", skinnedMeshShader);

	// Texture
	// Number
	ResourceManager::GetInstance().CreateTexture(L"None", L"..\\Resources\\Texture\\UI\\InGameUI\\None.png");
	ResourceManager::GetInstance().CreateTexture(L"Num_0", L"..\\Resources\\Texture\\UI\\InGameUI\\Num_0.png");
	ResourceManager::GetInstance().CreateTexture(L"Num_1", L"..\\Resources\\Texture\\UI\\InGameUI\\Num_1.png");
	ResourceManager::GetInstance().CreateTexture(L"Num_2", L"..\\Resources\\Texture\\UI\\InGameUI\\Num_2.png");
	ResourceManager::GetInstance().CreateTexture(L"Num_3", L"..\\Resources\\Texture\\UI\\InGameUI\\Num_3.png");
	ResourceManager::GetInstance().CreateTexture(L"Num_4", L"..\\Resources\\Texture\\UI\\InGameUI\\Num_4.png");
	ResourceManager::GetInstance().CreateTexture(L"Num_5", L"..\\Resources\\Texture\\UI\\InGameUI\\Num_5.png");
	ResourceManager::GetInstance().CreateTexture(L"Num_6", L"..\\Resources\\Texture\\UI\\InGameUI\\Num_6.png");
	ResourceManager::GetInstance().CreateTexture(L"Num_7", L"..\\Resources\\Texture\\UI\\InGameUI\\Num_7.png");
	ResourceManager::GetInstance().CreateTexture(L"Num_8", L"..\\Resources\\Texture\\UI\\InGameUI\\Num_8.png");
	ResourceManager::GetInstance().CreateTexture(L"Num_9", L"..\\Resources\\Texture\\UI\\InGameUI\\Num_9.png");

	ResourceManager::GetInstance().CreateTexture(L"Black", L"..\\Resources\\Texture\\UI\\etc\\Our Future.png");
	ResourceManager::GetInstance().CreateTexture(L"White", L"..\\Resources\\Texture\\UI\\etc\\A being out of this world.png");

	ResourceManager::GetInstance().CreateTexture(L"Test", L"..\\Resources\\Texture\\Zelda.jpg");

	ResourceManager::GetInstance().CreateTexture(L"SkyMap", L"..\\Resources\\Texture\\Zelda.jpg");

	ResourceManager::GetInstance().CreateTexture(L"SettingWindow", L"..\\Resources\\Texture\\SettingWindow.png");
	ResourceManager::GetInstance().CreateTexture(L"Test_Idle", L"..\\Resources\\Texture\\Test_Idle.png");
	ResourceManager::GetInstance().CreateTexture(L"Test_Enter", L"..\\Resources\\Texture\\Test_Enter.png");
	ResourceManager::GetInstance().CreateTexture(L"Test_Click", L"..\\Resources\\Texture\\Test_Click.png");

	ResourceManager::GetInstance().CreateTexture(L"AnimationTest000", L"..\\Resources\\Texture\\SpriteAnimation\\Enemy_BulletDestroy_000 (40ms).png");
	ResourceManager::GetInstance().CreateTexture(L"AnimationTest001", L"..\\Resources\\Texture\\SpriteAnimation\\Enemy_BulletDestroy_001 (40ms).png");
	ResourceManager::GetInstance().CreateTexture(L"AnimationTest002", L"..\\Resources\\Texture\\SpriteAnimation\\Enemy_BulletDestroy_002 (40ms).png");
	ResourceManager::GetInstance().CreateTexture(L"AnimationTest003", L"..\\Resources\\Texture\\SpriteAnimation\\Enemy_BulletDestroy_003 (40ms).png");
	ResourceManager::GetInstance().CreateTexture(L"AnimationTest004", L"..\\Resources\\Texture\\SpriteAnimation\\Enemy_BulletDestroy_004 (40ms).png");
	ResourceManager::GetInstance().CreateTexture(L"AnimationTest005", L"..\\Resources\\Texture\\SpriteAnimation\\Enemy_BulletDestroy_005 (40ms).png");

	ResourceManager::GetInstance().CreateTexture(L"2DTest000", L"..\\Resources\\Texture\\2DPattern\\brushstroke_2phase_0.png");
	ResourceManager::GetInstance().CreateTexture(L"2DTest001", L"..\\Resources\\Texture\\2DPattern\\brushstroke_2phase_1.png");
	ResourceManager::GetInstance().CreateTexture(L"2DTest002", L"..\\Resources\\Texture\\2DPattern\\brushstroke_2phase_2.png");
	ResourceManager::GetInstance().CreateTexture(L"2DTest003", L"..\\Resources\\Texture\\2DPattern\\brushstroke_2phase_3.png");
	ResourceManager::GetInstance().CreateTexture(L"2DTest004", L"..\\Resources\\Texture\\2DPattern\\brushstroke_2phase_4.png");
	ResourceManager::GetInstance().CreateTexture(L"2DTest005", L"..\\Resources\\Texture\\2DPattern\\brushstroke_2phase_5.png");

	// phase3
	ResourceManager::GetInstance().CreateTexture(L"BrushPahse3Boss0", L"..\\Resources\\Texture\\2DPattern\\3phase_brush\\boss\\brushstroke_3phase_boss_0.png");
	ResourceManager::GetInstance().CreateTexture(L"BrushPahse3Boss1", L"..\\Resources\\Texture\\2DPattern\\3phase_brush\\boss\\brushstroke_3phase_boss_1.png");
	ResourceManager::GetInstance().CreateTexture(L"BrushPahse3Boss2", L"..\\Resources\\Texture\\2DPattern\\3phase_brush\\boss\\brushstroke_3phase_boss_2.png");
	ResourceManager::GetInstance().CreateTexture(L"BrushPahse3Boss3", L"..\\Resources\\Texture\\2DPattern\\3phase_brush\\boss\\brushstroke_3phase_boss_3.png");
	ResourceManager::GetInstance().CreateTexture(L"BrushPahse3Boss4", L"..\\Resources\\Texture\\2DPattern\\3phase_brush\\boss\\brushstroke_3phase_boss_4.png");
	ResourceManager::GetInstance().CreateTexture(L"BrushPahse3Boss5", L"..\\Resources\\Texture\\2DPattern\\3phase_brush\\boss\\brushstroke_3phase_boss_5.png");
	ResourceManager::GetInstance().CreateTexture(L"BrushPahse3Boss6", L"..\\Resources\\Texture\\2DPattern\\3phase_brush\\boss\\brushstroke_3phase_boss_6.png");
	ResourceManager::GetInstance().CreateTexture(L"BrushPahse3Player0", L"..\\Resources\\Texture\\2DPattern\\3phase_brush\\mainchar\\brushstroke_3phase_mainchar_0.png");
	ResourceManager::GetInstance().CreateTexture(L"BrushPahse3Player1", L"..\\Resources\\Texture\\2DPattern\\3phase_brush\\mainchar\\brushstroke_3phase_mainchar_1.png");
	ResourceManager::GetInstance().CreateTexture(L"BrushPahse3Player2", L"..\\Resources\\Texture\\2DPattern\\3phase_brush\\mainchar\\brushstroke_3phase_mainchar_2.png");
	ResourceManager::GetInstance().CreateTexture(L"BrushPahse3Player3", L"..\\Resources\\Texture\\2DPattern\\3phase_brush\\mainchar\\brushstroke_3phase_mainchar_3.png");
	ResourceManager::GetInstance().CreateTexture(L"BrushPahse3Player4", L"..\\Resources\\Texture\\2DPattern\\3phase_brush\\mainchar\\brushstroke_3phase_mainchar_4.png");
	ResourceManager::GetInstance().CreateTexture(L"BrushPahse3Player5", L"..\\Resources\\Texture\\2DPattern\\3phase_brush\\mainchar\\brushstroke_3phase_mainchar_5.png");
	ResourceManager::GetInstance().CreateTexture(L"BrushPahse3Player6", L"..\\Resources\\Texture\\2DPattern\\3phase_brush\\mainchar\\brushstroke_3phase_mainchar_6.png");

	// Material
#pragma region Sprite Material
	{
		// TODO
		MaterialInfo material;
		material.shader = L"..\\Resources\\Shader\\Sprite3D.hlsli";
		ResourceManager::GetInstance().CreateMaterial(L"Sprite", material);
	}
#pragma endregion

#pragma region UI Material
	{
		// TODO
		MaterialInfo material;
		material.shader = L"..\\Resources\\Shader\\UI.hlsli";
		//material.texture[0] = L".png";
		ResourceManager::GetInstance().CreateMaterial(L"UI", material);
	}
#pragma endregion

#pragma region Deferred Render Material
	{
		MaterialInfo material;
		material.shader = L"..\\Resources\\Shader\\deferred.hlsli";
		ResourceManager::GetInstance().CreateMaterial(L"CubeMaterial", material);
	}
#pragma endregion

#pragma region SkyBox
	{
		MaterialInfo material;
		material.shader = L"..\\Resources\\Shader\\skybox.hlsli";
		material.texture[0] = L"..\\Resources\\Texture\\Zelda.jpg";
		ResourceManager::GetInstance().CreateMaterial(L"SkyBoxMaterial", material);
	}
#pragma endregion

#pragma region Forward Render Material
	{
		MaterialInfo material;
		material.shader = L"..\\Resources\\Shader\\forward.hlsli";
		ResourceManager::GetInstance().CreateMaterial(L"CubeMaterial_F", material);
	}
#pragma endregion

#pragma region DirectionalLight Material
	{
		MaterialInfo material;
		material.shader = L"..\\Resources\\Shader\\lightingDirectional.hlsli";
		material.texture[0] = L"D_Position";
		material.texture[1] = L"D_Normal";
		ResourceManager::GetInstance().CreateMaterial(L"DirLight", material);
	}
#pragma endregion

#pragma region Collider Boundary
	{
		MaterialInfo material;
		material.shader = L"..\\Resources\\Shader\\debug.hlsli";
		material.color = ZeldaColor::Blue;
		ResourceManager::GetInstance().CreateMaterial(L"BoundaryDebug", material);
	}
#pragma endregion

#pragma region Line Boundary
	{
		MaterialInfo material;
		material.shader = L"..\\Resources\\Shader\\debug_line.hlsli";
		material.color = ZeldaColor::Blue;
		ResourceManager::GetInstance().CreateMaterial(L"BoundaryDebug_Line", material);
	}
#pragma endregion
}

void CreateResourceDH()
{
	// Mesh
	ResourceManager::GetInstance().CreateMesh(L"Cube", L"Cube");
	ResourceManager::GetInstance().CreateMesh(L"Sphere", L"Sphere");
	ResourceManager::GetInstance().CreateMesh(L"Rectangle", L"Rectangle");
	ResourceManager::GetInstance().CreateMesh(L"Cylinder", L"Cylinder");
	ResourceManager::GetInstance().CreateMesh(L"UI", L"Rectangle");
	ResourceManager::GetInstance().CreateMesh(L"Line", L"Line");

	// Shader
	ShaderInfo uiShaderInfo;
	uiShaderInfo.rasterizerType = RASTERIZER_TYPE::CULL_NONE;
	uiShaderInfo.depthStencilType = DEPTH_STENCIL_TYPE::NO_DEPTH_TEST_NO_WRITE;
	uiShaderInfo.shaderType = SHADER_TYPE::UI;
	uiShaderInfo.blendType = BLEND_TYPE::ALPHA_BLEND;
	ResourceManager::GetInstance().CreateShader(L"UI", L"..\\Resources\\Shader\\UI.hlsli", uiShaderInfo);

	ShaderInfo deferredShaderInfo;
	deferredShaderInfo.shaderType = SHADER_TYPE::DEFERRED;
	ResourceManager::GetInstance().CreateShader(L"Deferred", L"..\\Resources\\Shader\\deferred.hlsli", deferredShaderInfo);

	// 디퍼드 + 스키닝 + 그림자
	ShaderInfo deferredSkinnedShadowShaderInfo;
	deferredSkinnedShadowShaderInfo.shaderType = SHADER_TYPE::DEFFERRED_SKIN_SHADOW;
	ResourceManager::GetInstance().CreateShader(L"deferred_skinned_shadow", L"..\\Resources\\Shader\\deferred_skinned_shadow.hlsli", deferredSkinnedShadowShaderInfo);

	ShaderInfo forwardShaderInfo;
	ResourceManager::GetInstance().CreateShader(L"Forward", L"..\\Resources\\Shader\\forward.hlsli", forwardShaderInfo);

	ShaderInfo skyBoxShaderInfo;
	skyBoxShaderInfo.rasterizerType = RASTERIZER_TYPE::CULL_FRONT;
	skyBoxShaderInfo.depthStencilType = DEPTH_STENCIL_TYPE::LESS_EQUAL;
	ResourceManager::GetInstance().CreateShader(L"SkyBox", L"..\\Resources\\Shader\\skybox.hlsli", skyBoxShaderInfo);

	ShaderInfo debugShaderInfo;
	debugShaderInfo.shaderType = SHADER_TYPE::FORWARD;
	debugShaderInfo.rasterizerType = RASTERIZER_TYPE::WIREFRAME;
	ResourceManager::GetInstance().CreateShader(L"Debug", L"..\\Resources\\Shader\\debug.hlsli", debugShaderInfo);

	ShaderInfo debugLineShaderInfo;
	debugLineShaderInfo.shaderType = SHADER_TYPE::FORWARD;
	debugLineShaderInfo.rasterizerType = RASTERIZER_TYPE::WIREFRAME;
	debugLineShaderInfo.topology = FFTopology::D3D_PRIMITIVE_TOPOLOGY_LINELIST;
	ResourceManager::GetInstance().CreateShader(L"Debug_Line", L"..\\Resources\\Shader\\debug_line.hlsli", debugLineShaderInfo);

	ShaderInfo lightingDirectionalShaderInfo;
	lightingDirectionalShaderInfo.shaderType = SHADER_TYPE::LIGHTING;
	lightingDirectionalShaderInfo.rasterizerType = RASTERIZER_TYPE::CULL_NONE;
	lightingDirectionalShaderInfo.depthStencilType = DEPTH_STENCIL_TYPE::NO_DEPTH_TEST_NO_WRITE;
	lightingDirectionalShaderInfo.blendType = BLEND_TYPE::ONE_TO_ONE_BLEND;
	lightingDirectionalShaderInfo.vs = "VS_DirLight";
	lightingDirectionalShaderInfo.ps = "PS_DirLight";
	ResourceManager::GetInstance().CreateShader(L"LightingDirectional", L"..\\Resources\\Shader\\lightingDirectional.hlsli", lightingDirectionalShaderInfo);

	ShaderInfo skinnedMeshShader;
	skinnedMeshShader.shaderType = SHADER_TYPE::DEFERRED_AND_SKIN;
	ResourceManager::GetInstance().CreateShader(L"SkinnedMesh", L"..\\Resources\\Shader\\deferred_skinned.hlsli", skinnedMeshShader);

	// Effec test
	ShaderInfo textureAnimShaderInfo;
	textureAnimShaderInfo.shaderType = SHADER_TYPE::EFFECT_ANIM;
	textureAnimShaderInfo.blendType = BLEND_TYPE::ALPHA_BLEND;
	ResourceManager::GetInstance().CreateShader(L"TextureAnim", L"..\\Resources\\Shader\\TextureAnim.hlsli", textureAnimShaderInfo);

	// BossFloor
	ShaderInfo bossFloorShaderInfo;
	//textureAnimShaderInfo.depthStencilType = DEPTH_STENCIL_TYPE::NO_DEPTH_TEST_NO_WRITE;
	bossFloorShaderInfo.shaderType = SHADER_TYPE::FORWARD;
	bossFloorShaderInfo.blendType = BLEND_TYPE::ALPHA_BLEND;
	bossFloorShaderInfo.rasterizerType = RASTERIZER_TYPE::CULL_NONE;
	ResourceManager::GetInstance().CreateShader(L"BossFloor", L"..\\Resources\\Shader\\UI.hlsli", bossFloorShaderInfo);


	// Boss
	ShaderInfo bossShaderInfo;
	bossShaderInfo.shaderType = SHADER_TYPE::DEFFERRED_SKIN_SHADOW;
	ResourceManager::GetInstance().CreateShader(L"Boss", L"..\\Resources\\Shader\\boss.hlsli", bossShaderInfo);

	// Texture
	ResourceManager::GetInstance().CreateTexture(L"Test", L"..\\Resources\\Texture\\Zelda.jpg");

	ResourceManager::GetInstance().CreateTexture(L"SkyMap", L"..\\Resources\\Texture\\Zelda.jpg");

	ResourceManager::GetInstance().CreateTexture(L"SettingWindow", L"..\\Resources\\Texture\\SettingWindow.png");
	ResourceManager::GetInstance().CreateTexture(L"Test_Idle", L"..\\Resources\\Texture\\Test_Idle.png");
	ResourceManager::GetInstance().CreateTexture(L"Test_Enter", L"..\\Resources\\Texture\\Test_Enter.png");
	ResourceManager::GetInstance().CreateTexture(L"Test_Click", L"..\\Resources\\Texture\\Test_Click.png");

	// Material
#pragma region UI Material
	{
		// TODO
		MaterialInfo material;
		material.shader = L"..\\Resources\\Shader\\UI.hlsli";
		//material.texture[0] = L".png";
		ResourceManager::GetInstance().CreateMaterial(L"UI", material);
	}
#pragma endregion

#pragma region Deferred Render Material
	{
		MaterialInfo material;
		material.shader = L"..\\Resources\\Shader\\deferred.hlsli";
		ResourceManager::GetInstance().CreateMaterial(L"CubeMaterial", material);
	}
#pragma endregion

#pragma region SkyBox
	{
		MaterialInfo material;
		material.shader = L"..\\Resources\\Shader\\skybox.hlsli";
		material.texture[0] = L"..\\Resources\\Texture\\Zelda.jpg";
		ResourceManager::GetInstance().CreateMaterial(L"SkyBoxMaterial", material);
	}
#pragma endregion

#pragma region Forward Render Material
	{
		MaterialInfo material;
		material.shader = L"..\\Resources\\Shader\\forward.hlsli";
		ResourceManager::GetInstance().CreateMaterial(L"CubeMaterial_F", material);
	}
#pragma endregion

#pragma region DirectionalLight Material
	{
		MaterialInfo material;
		material.shader = L"..\\Resources\\Shader\\lightingDirectional.hlsli";
		material.texture[0] = L"D_Position";
		material.texture[1] = L"D_Normal";
		ResourceManager::GetInstance().CreateMaterial(L"DirLight", material);
	}
#pragma endregion

#pragma region Collider Boundary
	{
		MaterialInfo material;
		material.shader = L"..\\Resources\\Shader\\debug.hlsli";
		material.color = ZeldaColor::Blue;
		ResourceManager::GetInstance().CreateMaterial(L"BoundaryDebug", material);
	}
#pragma endregion

#pragma region Line Boundary
	{
		MaterialInfo material;
		material.shader = L"..\\Resources\\Shader\\debug_line.hlsli";
		material.color = ZeldaColor::Blue;
		ResourceManager::GetInstance().CreateMaterial(L"BoundaryDebug_Line", material);
	}
#pragma endregion

#pragma region 보스 몬스터
	{
		MaterialInfo material;
		material.shader = L"..\\Resources\\Shader\\deferred.hlsli";
		material.color = ZeldaColor::Red;
		ResourceManager::GetInstance().CreateMaterial(L"Boss1", material);
	}
#pragma endregion

#pragma region 플레이어(임시)
	{
		MaterialInfo material;
		material.shader = L"..\\Resources\\Shader\\deferred.hlsli";
		material.color = ZeldaColor::Green;
		ResourceManager::GetInstance().CreateMaterial(L"PlayerTemp", material);
	}
#pragma endregion

#pragma region 플랫폼(임시)
	{
		MaterialInfo material;
		material.shader = L"..\\Resources\\Shader\\deferred.hlsli";
		material.color = ZeldaColor::White;
		ResourceManager::GetInstance().CreateMaterial(L"PlatformTemp", material);
	}
#pragma endregion

#pragma region 돌진 패턴 오브젝트
	{
		MaterialInfo material;
		material.shader = L"..\\Resources\\Shader\\deferred.hlsli";
		material.color = ZeldaColor::Red;
		ResourceManager::GetInstance().CreateMaterial(L"attackRange", material);
	}
#pragma endregion

#pragma region 2D 패턴 오브젝트
	{
		MaterialInfo material;
		material.shader = L"..\\Resources\\Shader\\deferred.hlsli";
		material.color = ZeldaColor::Red;
		ResourceManager::GetInstance().CreateMaterial(L"pattern4Material", material);
	}
#pragma endregion

#pragma region 이동 연출 오브젝트

	{
		MaterialInfo material;
		material.shader = L"..\\Resources\\Shader\\deferred.hlsli";
		material.color = ZeldaColor::Green;
		ResourceManager::GetInstance().CreateMaterial(L"patternMoveMaterial", material);
	}
#pragma endregion

#pragma region Player FBX
	{
		ResourceManager::GetInstance().FBXLoad(L"Player", L"..\\Resources\\FBX\\Player\\Player.fbx", L"..\\Resources\\Shader\\deferred_skinned_shadow.hlsli");
	}
#pragma endregion

#pragma region BossDoor FBX
	{
		ResourceManager::GetInstance().FBXLoad(L"BossDoor", L"..\\Resources\\FBX\\BossDoor\\BossDoor.fbx", L"..\\Resources\\Shader\\deferred.hlsli");
	}
#pragma endregion

#pragma region BossDoorHand FBX
	{
		ResourceManager::GetInstance().FBXLoad(L"BossDoorHand", L"..\\Resources\\FBX\\BossDoorHand\\BossDoorHand.fbx", L"..\\Resources\\Shader\\deferred_skinned_shadow.hlsli");
	}
#pragma endregion

#pragma region BossFloor FBX
	{
		ResourceManager::GetInstance().FBXLoad(L"BossFloor", L"..\\Resources\\FBX\\BossFloor\\BossFloor.fbx", L"..\\Resources\\Shader\\deferred.hlsli");
	}
#pragma endregion

#pragma region BossFlag FBX
	{
		ResourceManager::GetInstance().FBXLoad(L"BossFlag", L"..\\Resources\\FBX\\BossFlag\\BossFlag.fbx", L"..\\Resources\\Shader\\UI.hlsli");
	}
#pragma endregion

#pragma region Boss FBX
	{
		ResourceManager::GetInstance().FBXLoad(L"Boss", L"..\\Resources\\FBX\\Boss\\Boss.fbx", L"..\\Resources\\Shader\\boss.hlsli");
	}
#pragma endregion

#pragma region Scroll FBX
	{
		ResourceManager::GetInstance().FBXLoad(L"Scroll", L"..\\Resources\\FBX\\Scroll\\Scroll.fbx", L"..\\Resources\\Shader\\deferred.hlsli");
	}
#pragma endregion

#pragma region VFX_Circle FBX
	{
		ResourceManager::GetInstance().FBXLoad(L"VFX_Circle", L"..\\Resources\\FBX\\VFX_Circle\\VFX_Circle.fbx", L"..\\Resources\\Shader\\TextureAnim.hlsli");
	}
#pragma endregion

#pragma region VFX_Tree FBX
	{
		ResourceManager::GetInstance().FBXLoad(L"VFX_Tree", L"..\\Resources\\FBX\\VFX_Tree\\VFX_Tree.fbx", L"..\\Resources\\Shader\\TextureAnim.hlsli");
	}
#pragma endregion

#pragma region VFX_Tree FBX
	{
		ResourceManager::GetInstance().FBXLoad(L"VFX_Teleport", L"..\\Resources\\FBX\\VFX_Teleport\\VFX_Teleport.fbx", L"..\\Resources\\Shader\\TextureAnim.hlsli");
	}
#pragma endregion

}

void CreateResourceGH()
{
	// Mesh
	ResourceManager::GetInstance().CreateMesh(L"Cube", L"Cube");
	ResourceManager::GetInstance().CreateMesh(L"Sphere", L"Sphere");
	ResourceManager::GetInstance().CreateMesh(L"Rectangle", L"Rectangle");
	ResourceManager::GetInstance().CreateMesh(L"Cylinder", L"Cylinder");
	ResourceManager::GetInstance().CreateMesh(L"UI", L"Rectangle");
	ResourceManager::GetInstance().CreateMesh(L"Line", L"Line");

	// Shader
	ShaderInfo uiShaderInfo;
	uiShaderInfo.rasterizerType = RASTERIZER_TYPE::CULL_NONE;
	uiShaderInfo.depthStencilType = DEPTH_STENCIL_TYPE::NO_DEPTH_TEST_NO_WRITE;
	uiShaderInfo.shaderType = SHADER_TYPE::UI;
	uiShaderInfo.blendType = BLEND_TYPE::ALPHA_BLEND;
	ResourceManager::GetInstance().CreateShader(L"UI", L"..\\Resources\\Shader\\UI.hlsli", uiShaderInfo);
	///

	ShaderInfo deferredShaderInfo;
	deferredShaderInfo.shaderType = SHADER_TYPE::DEFERRED;
	ResourceManager::GetInstance().CreateShader(L"Deferred", L"..\\Resources\\Shader\\deferred.hlsli", deferredShaderInfo);
	///


	// 디퍼드이면서 그림자 출력
	ShaderInfo deferredShadowShaderInfo;
	deferredShadowShaderInfo.shaderType = SHADER_TYPE::DEFFERRED_SHADOW;
	ResourceManager::GetInstance().CreateShader(L"deferred_shadow", L"..\\Resources\\Shader\\deferred_shadow.hlsli", deferredShadowShaderInfo);
	///

	// 디퍼드 + 스키닝 + 그림자
	ShaderInfo deferredSkinnedShadowShaderInfo;
	deferredSkinnedShadowShaderInfo.shaderType = SHADER_TYPE::DEFFERRED_SKIN_SHADOW;
	ResourceManager::GetInstance().CreateShader(L"deferred_skinned_shadow", L"..\\Resources\\Shader\\deferred_skinned_shadow.hlsli", deferredSkinnedShadowShaderInfo);
	///

	ShaderInfo forwardShaderInfo;
	ResourceManager::GetInstance().CreateShader(L"Forward", L"..\\Resources\\Shader\\forward.hlsli", forwardShaderInfo);
	///

	ShaderInfo skyBoxShaderInfo;
	skyBoxShaderInfo.rasterizerType = RASTERIZER_TYPE::CULL_FRONT;
	skyBoxShaderInfo.depthStencilType = DEPTH_STENCIL_TYPE::LESS_EQUAL;
	ResourceManager::GetInstance().CreateShader(L"SkyBox", L"..\\Resources\\Shader\\skybox.hlsli", skyBoxShaderInfo);
	///

	ShaderInfo debugShaderInfo;
	debugShaderInfo.shaderType = SHADER_TYPE::FORWARD;
	debugShaderInfo.rasterizerType = RASTERIZER_TYPE::WIREFRAME;
	ResourceManager::GetInstance().CreateShader(L"Debug", L"..\\Resources\\Shader\\debug.hlsli", debugShaderInfo);
	///

	ShaderInfo debugLineShaderInfo;
	debugLineShaderInfo.shaderType = SHADER_TYPE::FORWARD;
	debugLineShaderInfo.rasterizerType = RASTERIZER_TYPE::WIREFRAME;
	debugLineShaderInfo.topology = FFTopology::D3D_PRIMITIVE_TOPOLOGY_LINELIST;
	ResourceManager::GetInstance().CreateShader(L"Debug_Line", L"..\\Resources\\Shader\\debug_line.hlsli", debugLineShaderInfo);
	///

	ShaderInfo lightingDirectionalShaderInfo;
	lightingDirectionalShaderInfo.shaderType = SHADER_TYPE::LIGHTING;
	lightingDirectionalShaderInfo.rasterizerType = RASTERIZER_TYPE::CULL_NONE;
	lightingDirectionalShaderInfo.depthStencilType = DEPTH_STENCIL_TYPE::NO_DEPTH_TEST_NO_WRITE;
	lightingDirectionalShaderInfo.blendType = BLEND_TYPE::ONE_TO_ONE_BLEND;
	lightingDirectionalShaderInfo.vs = "VS_DirLight";
	lightingDirectionalShaderInfo.ps = "PS_DirLight";
	ResourceManager::GetInstance().CreateShader(L"LightingDirectional", L"..\\Resources\\Shader\\lightingDirectional.hlsli", lightingDirectionalShaderInfo);
	///

	ShaderInfo skinnedMeshShader;
	skinnedMeshShader.shaderType = SHADER_TYPE::DEFERRED_AND_SKIN;
	ResourceManager::GetInstance().CreateShader(L"SkinnedMesh", L"..\\Resources\\Shader\\deferred_skinned.hlsli", skinnedMeshShader);
	///

	// Effec test
	ShaderInfo textureAnimShaderInfo;
	textureAnimShaderInfo.shaderType = SHADER_TYPE::EFFECT_ANIM;
	textureAnimShaderInfo.blendType = BLEND_TYPE::ALPHA_BLEND;
	textureAnimShaderInfo.rasterizerType = RASTERIZER_TYPE::CULL_NONE;
	ResourceManager::GetInstance().CreateShader(L"TextureAnim", L"..\\Resources\\Shader\\TextureAnim.hlsli", textureAnimShaderInfo);
	///

	// BossFloor
	ShaderInfo bossFloorShaderInfo;
	//textureAnimShaderInfo.depthStencilType = DEPTH_STENCIL_TYPE::NO_DEPTH_TEST_NO_WRITE;
	bossFloorShaderInfo.shaderType = SHADER_TYPE::FORWARD;
	bossFloorShaderInfo.blendType = BLEND_TYPE::ALPHA_BLEND;
	bossFloorShaderInfo.rasterizerType = RASTERIZER_TYPE::CULL_NONE;
	ResourceManager::GetInstance().CreateShader(L"BossFloor", L"..\\Resources\\Shader\\BossFloor.hlsli", bossFloorShaderInfo);
	///

	// EffectGuideLine
	ShaderInfo guideLineShaderInfo;
	guideLineShaderInfo.shaderType = SHADER_TYPE::FORWARD;
	guideLineShaderInfo.blendType = BLEND_TYPE::ALPHA_BLEND;
	guideLineShaderInfo.rasterizerType = RASTERIZER_TYPE::CULL_NONE;
	ResourceManager::GetInstance().CreateShader(L"GuideLine", L"..\\Resources\\Shader\\GuideLine.hlsli", guideLineShaderInfo);
	///

	// Boss
	ShaderInfo bossShaderInfo;
	bossShaderInfo.shaderType = SHADER_TYPE::DEFFERRED_SKIN_SHADOW;
	ResourceManager::GetInstance().CreateShader(L"Boss", L"..\\Resources\\Shader\\boss.hlsli", bossShaderInfo);
	///

	// Player
	ShaderInfo playerShaderInfo;
	playerShaderInfo.shaderType = SHADER_TYPE::DEFFERRED_SKIN_SHADOW;
	ResourceManager::GetInstance().CreateShader(L"Player", L"..\\Resources\\Shader\\player.hlsli", playerShaderInfo);
	///

	// 보스 깃발 용 셰이더
	ShaderInfo bossFlagShaderInfo;
	bossFlagShaderInfo.shaderType = SHADER_TYPE::FORWARD;
	bossFlagShaderInfo.blendType = BLEND_TYPE::ALPHA_BLEND;
	ResourceManager::GetInstance().CreateShader(L"BossFlag", L"..\\Resources\\Shader\\BossFlag.hlsli", bossFlagShaderInfo);
	///

	// Texture
	ResourceManager::GetInstance().CreateTexture(L"Test", L"..\\Resources\\Texture\\Zelda.jpg");

	ResourceManager::GetInstance().CreateTexture(L"SkyMap", L"..\\Resources\\Texture\\Zelda.jpg");

	ResourceManager::GetInstance().CreateTexture(L"SettingWindow", L"..\\Resources\\Texture\\SettingWindow.png");
	ResourceManager::GetInstance().CreateTexture(L"Test_Idle", L"..\\Resources\\Texture\\Test_Idle.png");
	ResourceManager::GetInstance().CreateTexture(L"Test_Enter", L"..\\Resources\\Texture\\Test_Enter.png");
	ResourceManager::GetInstance().CreateTexture(L"Test_Click", L"..\\Resources\\Texture\\Test_Click.png");
	///

	ResourceManager::GetInstance().CreateTexture(L"Normal2DPatternTexture_0", L"..\\Resources\\Texture\\2DPattern\\brushstroke_2phase_0.png");
	ResourceManager::GetInstance().CreateTexture(L"Normal2DPatternTexture_1", L"..\\Resources\\Texture\\2DPattern\\brushstroke_2phase_1.png");
	ResourceManager::GetInstance().CreateTexture(L"Normal2DPatternTexture_2", L"..\\Resources\\Texture\\2DPattern\\brushstroke_2phase_2.png");
	ResourceManager::GetInstance().CreateTexture(L"Normal2DPatternTexture_3", L"..\\Resources\\Texture\\2DPattern\\brushstroke_2phase_3.png");
	ResourceManager::GetInstance().CreateTexture(L"Normal2DPatternTexture_4", L"..\\Resources\\Texture\\2DPattern\\brushstroke_2phase_4.png");
	ResourceManager::GetInstance().CreateTexture(L"Normal2DPatternTexture_5", L"..\\Resources\\Texture\\2DPattern\\brushstroke_2phase_5.png");
	ResourceManager::GetInstance().CreateTexture(L"Normal2DPatternTexture_Warning", L"..\\Resources\\Texture\\2DPattern\\brushstroke_2phase_warning.png");

	ResourceManager::GetInstance().CreateTexture(L"widebrush_bottom", L"..\\Resources\\Texture\\2DPattern\\2phase_widebrush\\widebrush_bottom.png");
	ResourceManager::GetInstance().CreateTexture(L"widebrush_top", L"..\\Resources\\Texture\\2DPattern\\2phase_widebrush\\widebrush_top.png");
	ResourceManager::GetInstance().CreateTexture(L"widebrush_right", L"..\\Resources\\Texture\\2DPattern\\2phase_widebrush\\widebrush_right.png");
	ResourceManager::GetInstance().CreateTexture(L"widebrush_left", L"..\\Resources\\Texture\\2DPattern\\2phase_widebrush\\widebrush_left.png");

	ResourceManager::GetInstance().CreateTexture(L"widebrush_horizontal_warning", L"..\\Resources\\Texture\\2DPattern\\2phase_widebrush\\widebrush_horizontal_warning.png");
	ResourceManager::GetInstance().CreateTexture(L"widebrush_vertical_warning", L"..\\Resources\\Texture\\2DPattern\\2phase_widebrush\\widebrush_vertical_warning.png");
	ResourceManager::GetInstance().CreateTexture(L"PlayerDamaged_0", L"..\\Resources\\Texture\\playerdamage\\player_damage_0.png");
	ResourceManager::GetInstance().CreateTexture(L"PlayerDamaged_1", L"..\\Resources\\Texture\\playerdamage\\player_damage_1.png");
	ResourceManager::GetInstance().CreateTexture(L"PlayerDamaged_2", L"..\\Resources\\Texture\\playerdamage\\player_damage_2.png");
	ResourceManager::GetInstance().CreateTexture(L"PlayerDamaged_3", L"..\\Resources\\Texture\\playerdamage\\player_damage_3.png");
	ResourceManager::GetInstance().CreateTexture(L"PlayerDamaged_4", L"..\\Resources\\Texture\\playerdamage\\player_damage_4.png");
	///

	ResourceManager::GetInstance().CreateTexture(L"boss_damage_0", L"..\\Resources\\Texture\\bossdamage\\boss_damage_0.png");
	ResourceManager::GetInstance().CreateTexture(L"boss_damage_1", L"..\\Resources\\Texture\\bossdamage\\boss_damage_1.png");
	ResourceManager::GetInstance().CreateTexture(L"boss_damage_2", L"..\\Resources\\Texture\\bossdamage\\boss_damage_2.png");
	ResourceManager::GetInstance().CreateTexture(L"boss_damage_3", L"..\\Resources\\Texture\\bossdamage\\boss_damage_3.png");
	ResourceManager::GetInstance().CreateTexture(L"boss_damage_4", L"..\\Resources\\Texture\\bossdamage\\boss_damage_4.png");
	ResourceManager::GetInstance().CreateTexture(L"boss_damage_5", L"..\\Resources\\Texture\\bossdamage\\boss_damage_5.png");
	ResourceManager::GetInstance().CreateTexture(L"boss_damage_6", L"..\\Resources\\Texture\\bossdamage\\boss_damage_6.png");
	///

	ResourceManager::GetInstance().CreateTexture(L"Phase2CutScene1_0", L"..\\Resources\\Texture\\phase_cutscene\\2phase\\first\\2phasestart_first_0.png");
	ResourceManager::GetInstance().CreateTexture(L"Phase2CutScene1_1", L"..\\Resources\\Texture\\phase_cutscene\\2phase\\first\\2phasestart_first_1.png");
	ResourceManager::GetInstance().CreateTexture(L"Phase2CutScene1_2", L"..\\Resources\\Texture\\phase_cutscene\\2phase\\first\\2phasestart_first_2.png");
	ResourceManager::GetInstance().CreateTexture(L"Phase2CutScene1_3", L"..\\Resources\\Texture\\phase_cutscene\\2phase\\first\\2phasestart_first_3.png");
	ResourceManager::GetInstance().CreateTexture(L"Phase2CutScene1_4", L"..\\Resources\\Texture\\phase_cutscene\\2phase\\first\\2phasestart_first_4.png");
	ResourceManager::GetInstance().CreateTexture(L"Phase2CutScene1_5", L"..\\Resources\\Texture\\phase_cutscene\\2phase\\first\\2phasestart_first_5.png");
	ResourceManager::GetInstance().CreateTexture(L"Phase2CutScene1_6", L"..\\Resources\\Texture\\phase_cutscene\\2phase\\first\\2phasestart_first_6.png");

	ResourceManager::GetInstance().CreateTexture(L"Phase2CutScene2_0", L"..\\Resources\\Texture\\phase_cutscene\\2phase\\second\\2phasestart_second_0.png");
	ResourceManager::GetInstance().CreateTexture(L"Phase2CutScene2_1", L"..\\Resources\\Texture\\phase_cutscene\\2phase\\second\\2phasestart_second_1.png");
	ResourceManager::GetInstance().CreateTexture(L"Phase2CutScene2_2", L"..\\Resources\\Texture\\phase_cutscene\\2phase\\second\\2phasestart_second_2.png");
	ResourceManager::GetInstance().CreateTexture(L"Phase2CutScene2_3", L"..\\Resources\\Texture\\phase_cutscene\\2phase\\second\\2phasestart_second_3.png");
	ResourceManager::GetInstance().CreateTexture(L"Phase2CutScene2_4", L"..\\Resources\\Texture\\phase_cutscene\\2phase\\second\\2phasestart_second_4.png");
	ResourceManager::GetInstance().CreateTexture(L"Phase2CutScene2_5", L"..\\Resources\\Texture\\phase_cutscene\\2phase\\second\\2phasestart_second_5.png");
	ResourceManager::GetInstance().CreateTexture(L"Phase2CutScene2_6", L"..\\Resources\\Texture\\phase_cutscene\\2phase\\second\\2phasestart_second_6.png");
	ResourceManager::GetInstance().CreateTexture(L"Phase2CutScene2_7", L"..\\Resources\\Texture\\phase_cutscene\\2phase\\second\\2phasestart_second_7.png");
	ResourceManager::GetInstance().CreateTexture(L"Phase2CutScene2_8", L"..\\Resources\\Texture\\phase_cutscene\\2phase\\second\\2phasestart_second_8.png");
	ResourceManager::GetInstance().CreateTexture(L"Phase2CutScene2_9", L"..\\Resources\\Texture\\phase_cutscene\\2phase\\second\\2phasestart_second_9.png");

	ResourceManager::GetInstance().CreateTexture(L"Phase3CutScene0", L"..\\Resources\\Texture\\phase_cutscene\\3phase\\Phase3CutScene0.png");
	ResourceManager::GetInstance().CreateTexture(L"Phase3CutScene1", L"..\\Resources\\Texture\\phase_cutscene\\3phase\\Phase3CutScene1.png");
	///

#pragma region 디졸브 효과
	{
		ResourceManager::GetInstance().CreateTexture(L"Delete", L"..\\Resources\\Texture\\BossFlag\\delete.jpg");
		ResourceManager::GetInstance().CreateTexture(L"Flag2", L"..\\Resources\\Texture\\BossFlag\\T_Flag_Diffuse_002.png");
	}
#pragma endregion

	// Material
#pragma region UI Material
	{
		// TODO
		MaterialInfo material;
		material.shader = L"..\\Resources\\Shader\\UI.hlsli";
		//material.texture[0] = L".png";
		ResourceManager::GetInstance().CreateMaterial(L"UI", material);
	}
#pragma endregion
	///

#pragma region Deferred Render Material
	{
		MaterialInfo material;
		material.shader = L"..\\Resources\\Shader\\deferred.hlsli";
		ResourceManager::GetInstance().CreateMaterial(L"CubeMaterial", material);
	}
#pragma endregion
	///

#pragma region SkyBox
	{
		MaterialInfo material;
		material.shader = L"..\\Resources\\Shader\\skybox.hlsli";
		material.texture[0] = L"..\\Resources\\Texture\\Zelda.jpg";
		ResourceManager::GetInstance().CreateMaterial(L"SkyBoxMaterial", material);
	}
#pragma endregion
	///

#pragma region Forward Render Material
	{
		MaterialInfo material;
		material.shader = L"..\\Resources\\Shader\\forward.hlsli";
		ResourceManager::GetInstance().CreateMaterial(L"CubeMaterial_F", material);
	}
#pragma endregion
	///

#pragma region DirectionalLight Material
	{
		MaterialInfo material;
		material.shader = L"..\\Resources\\Shader\\lightingDirectional.hlsli";
		material.texture[0] = L"D_Position";
		material.texture[1] = L"D_Normal";
		ResourceManager::GetInstance().CreateMaterial(L"DirLight", material);
	}
#pragma endregion
	///

#pragma region Collider Boundary
	{
		MaterialInfo material;
		material.shader = L"..\\Resources\\Shader\\debug.hlsli";
		material.color = ZeldaColor::Blue;
		ResourceManager::GetInstance().CreateMaterial(L"BoundaryDebug", material);
	}
#pragma endregion
	///

#pragma region Line Boundary
	{
		MaterialInfo material;
		material.shader = L"..\\Resources\\Shader\\debug_line.hlsli";
		material.color = ZeldaColor::Blue;
		ResourceManager::GetInstance().CreateMaterial(L"BoundaryDebug_Line", material);
	}
#pragma endregion
	///

#pragma region 보스 몬스터
	{
		MaterialInfo material;
		material.shader = L"..\\Resources\\Shader\\deferred.hlsli";
		material.color = ZeldaColor::Red;
		ResourceManager::GetInstance().CreateMaterial(L"Boss1", material);
	}
#pragma endregion

#pragma region 플레이어(임시)
	{
		MaterialInfo material;
		material.shader = L"..\\Resources\\Shader\\deferred.hlsli";
		material.color = ZeldaColor::Green;
		ResourceManager::GetInstance().CreateMaterial(L"PlayerTemp", material);
	}
#pragma endregion

#pragma region 플랫폼(임시)
	{
		MaterialInfo material;
		material.shader = L"..\\Resources\\Shader\\deferred.hlsli";
		material.color = ZeldaColor::White;
		ResourceManager::GetInstance().CreateMaterial(L"PlatformTemp", material);
	}
#pragma endregion

#pragma region 돌진 패턴 오브젝트
	{
		MaterialInfo material;
		material.shader = L"..\\Resources\\Shader\\deferred.hlsli";
		material.color = ZeldaColor::Red;
		ResourceManager::GetInstance().CreateMaterial(L"attackRange", material);
	}
#pragma endregion

#pragma region 2D 패턴 오브젝트
	{
		MaterialInfo material;
		material.shader = L"..\\Resources\\Shader\\deferred.hlsli";
		material.color = ZeldaColor::Red;
		ResourceManager::GetInstance().CreateMaterial(L"pattern4Material", material);
	}
#pragma endregion

#pragma region 이동 연출 오브젝트
	{
		MaterialInfo material;
		material.shader = L"..\\Resources\\Shader\\deferred.hlsli";
		material.color = ZeldaColor::Green;
		ResourceManager::GetInstance().CreateMaterial(L"patternMoveMaterial", material);
	}
#pragma endregion
	///

#pragma region Effect FBX
	{
		ResourceManager::GetInstance().FBXLoad(L"VFX", L"..\\Resources\\FBX\\VFX\\VFX.fbx", L"..\\Resources\\Shader\\TextureAnim.hlsli");
	}
#pragma endregion

#pragma region Player FBX
	{
		ResourceManager::GetInstance().FBXLoad(L"Player", L"..\\Resources\\FBX\\Player\\Player.fbx", L"..\\Resources\\Shader\\player.hlsli");
	}
#pragma endregion
	///

#pragma region BossDoor FBX
	{
		ResourceManager::GetInstance().FBXLoad(L"BossDoor", L"..\\Resources\\FBX\\BossDoor\\BossDoor.fbx", L"..\\Resources\\Shader\\deferred.hlsli");
	}
#pragma endregion
	///

#pragma region BossDoorHand FBX
	{
		ResourceManager::GetInstance().FBXLoad(L"BossDoorHand", L"..\\Resources\\FBX\\BossDoorHand\\BossDoorHand.fbx", L"..\\Resources\\Shader\\deferred_skinned_shadow.hlsli");
	}
#pragma endregion
	///

#pragma region BossFloor FBX
	{
		ResourceManager::GetInstance().FBXLoad(L"BossFloor", L"..\\Resources\\FBX\\BossFloor\\BossFloor.fbx", L"..\\Resources\\Shader\\deferred_shadow.hlsli");
	}
#pragma endregion
	///

#pragma region BossFlag FBX
	{
		ResourceManager::GetInstance().FBXLoad(L"BossFlag", L"..\\Resources\\FBX\\BossFlag\\BossFlag.fbx", L"..\\Resources\\Shader\\BossFlag.hlsli");
	}
#pragma endregion
	///

#pragma region Boss FBX
	{
		ResourceManager::GetInstance().FBXLoad(L"Boss", L"..\\Resources\\FBX\\Boss\\Boss.fbx", L"..\\Resources\\Shader\\boss.hlsli");
	}
#pragma endregion
	///

#pragma region Scroll FBX
	{
		ResourceManager::GetInstance().FBXLoad(L"Scroll", L"..\\Resources\\FBX\\Scroll\\Scroll.fbx", L"..\\Resources\\Shader\\deferred.hlsli");
	}
#pragma endregion
	///

#pragma region VFX_Circle FBX
	{
		ResourceManager::GetInstance().FBXLoad(L"VFX_Circle", L"..\\Resources\\FBX\\VFX_Circle\\VFX_Circle.fbx", L"..\\Resources\\Shader\\TextureAnim.hlsli");
	}
#pragma endregion
	///

#pragma region VFX_Tree FBX
	{
		ResourceManager::GetInstance().FBXLoad(L"VFX_Tree", L"..\\Resources\\FBX\\VFX_Tree\\VFX_Tree.fbx", L"..\\Resources\\Shader\\TextureAnim.hlsli");
	}
#pragma endregion
	///

#pragma region VFX_Teleport FBX
	{
		ResourceManager::GetInstance().FBXLoad(L"VFX_Teleport", L"..\\Resources\\FBX\\VFX_Teleport\\VFX_Teleport.fbx", L"..\\Resources\\Shader\\TextureAnim.hlsli");
	}
#pragma endregion
	///

#pragma region VFX_BossNormalAttack_L FBX
	{
		ResourceManager::GetInstance().FBXLoad(L"VFX_BossNormalAttack_L", L"..\\Resources\\FBX\\VFX_BossNormalAttack_L\\VFX_BossNormalAttack_L.fbx", L"..\\Resources\\Shader\\TextureAnim.hlsli");
	}
#pragma endregion
	///

#pragma region VFX_Knockback FBX
	{
		ResourceManager::GetInstance().FBXLoad(L"VFX_Knockback", L"..\\Resources\\FBX\\VFX_Knockback\\VFX_Knockback.fbx", L"..\\Resources\\Shader\\TextureAnim.hlsli");
	}
#pragma endregion

#pragma region VFX_KnockbackGuide FBX
	{
		ResourceManager::GetInstance().FBXLoad(L"VFX_KnockbackGuide", L"..\\Resources\\FBX\\VFX_KnockbackGuide\\VFX_KnockbackGuide.fbx", L"..\\Resources\\Shader\\GuideLine.hlsli");
	}
#pragma endregion

#pragma region VFX_BossNormalAttack_R FBX
	{
		ResourceManager::GetInstance().FBXLoad(L"VFX_BossNormalAttack_R", L"..\\Resources\\FBX\\VFX_BossNormalAttack_R\\VFX_BossNormalAttack_R.fbx", L"..\\Resources\\Shader\\TextureAnim.hlsli");
	}
#pragma endregion

#pragma region VFX_Skill2D FBX
	{
		ResourceManager::GetInstance().FBXLoad(L"VFX_Skill2D", L"..\\Resources\\FBX\\VFX_Skill2D\\VFX_Skill2D.fbx", L"..\\Resources\\Shader\\TextureAnim.hlsli");
	}
#pragma endregion

#pragma region VFX_ChargeAttack FBX
	{
		ResourceManager::GetInstance().FBXLoad(L"VFX_ChargeAttack", L"..\\Resources\\FBX\\VFX_ChargeAttack\\VFX_ChargeAttack.fbx", L"..\\Resources\\Shader\\TextureAnim.hlsli");
	}
#pragma endregion

#pragma region VFX_ChargeAttackGuide FBX
	{
		ResourceManager::GetInstance().FBXLoad(L"VFX_ChargeAttackGuide", L"..\\Resources\\FBX\\VFX_ChargeAttackGuide\\VFX_ChargeAttackGuide.fbx", L"..\\Resources\\Shader\\TextureAnim.hlsli");
	}
#pragma endregion

	// 플레이어 VFX
#pragma region VFX_PlayerAttack FBX
	{
		ResourceManager::GetInstance().FBXLoad(L"VFX_PlayerAttack", L"..\\Resources\\FBX\\VFX_PlayerAttack\\VFX_PlayerAttack.fbx", L"..\\Resources\\Shader\\TextureAnim.hlsli");
	}
#pragma endregion

#pragma region VFX_PlayerSpecialAttack FBX
	{
		ResourceManager::GetInstance().FBXLoad(L"VFX_PlayerSpecialAttack", L"..\\Resources\\FBX\\VFX_PlayerSpecialAttack\\VFX_PlayerSpecialAttack.fbx", L"..\\Resources\\Shader\\TextureAnim.hlsli");
	}
#pragma endregion

}

void CreateResourceSC()
{
	// Mesh
	ResourceManager::GetInstance().CreateMesh(L"Cube", L"Cube");
	ResourceManager::GetInstance().CreateMesh(L"Sphere", L"Sphere");
	ResourceManager::GetInstance().CreateMesh(L"Capsule", L"Capsule");
	ResourceManager::GetInstance().CreateMesh(L"Cylinder", L"Cylinder");
	ResourceManager::GetInstance().CreateMesh(L"Rectangle", L"Rectangle");
	ResourceManager::GetInstance().CreateMesh(L"UI", L"Rectangle");
	ResourceManager::GetInstance().CreateMesh(L"Line", L"Line");
	ResourceManager::GetInstance().CreateMesh(L"Point", L"Point");

	// 보스 깃발 용 셰이더
	ShaderInfo bossFlagShaderInfo;
	bossFlagShaderInfo.shaderType = SHADER_TYPE::FORWARD;
	bossFlagShaderInfo.blendType = BLEND_TYPE::ALPHA_BLEND;
	ResourceManager::GetInstance().CreateShader(L"BossFlag", L"..\\Resources\\Shader\\BossFlag.hlsli", bossFlagShaderInfo);

	//Test
	ShaderInfo testShaderInfo;
	testShaderInfo.shaderType = SHADER_TYPE::DEFERRED;
	ResourceManager::GetInstance().CreateShader(L"Test", L"..\\Resources\\Shader\\Test.hlsli", testShaderInfo);

	// 디퍼드이면서 그림자 출력
	ShaderInfo deferredShadowShaderInfo;
	deferredShadowShaderInfo.shaderType = SHADER_TYPE::DEFFERRED_SHADOW;
	ResourceManager::GetInstance().CreateShader(L"deferred_shadow", L"..\\Resources\\Shader\\deferred_shadow.hlsli", deferredShadowShaderInfo);

	// 디퍼드 + 스키닝 + 그림자
	ShaderInfo deferredSkinnedShadowShaderInfo;
	deferredSkinnedShadowShaderInfo.shaderType = SHADER_TYPE::DEFFERRED_SKIN_SHADOW;
	ResourceManager::GetInstance().CreateShader(L"deferred_skinned_shadow", L"..\\Resources\\Shader\\deferred_skinned_shadow.hlsli", deferredSkinnedShadowShaderInfo);

	// BossFloor
	ShaderInfo bossFloorShaderInfo;
	//textureAnimShaderInfo.depthStencilType = DEPTH_STENCIL_TYPE::NO_DEPTH_TEST_NO_WRITE;
	bossFloorShaderInfo.shaderType = SHADER_TYPE::FORWARD;
	bossFloorShaderInfo.blendType = BLEND_TYPE::ALPHA_BLEND;
	bossFloorShaderInfo.rasterizerType = RASTERIZER_TYPE::CULL_NONE;
	ResourceManager::GetInstance().CreateShader(L"BossFloor", L"..\\Resources\\Shader\\UI.hlsli", bossFloorShaderInfo);

	// Boss
	ShaderInfo bossShaderInfo;
	bossShaderInfo.shaderType = SHADER_TYPE::DEFFERRED_SKIN_SHADOW;
	bossShaderInfo.rasterizerType = RASTERIZER_TYPE::CULL_NONE;
	ResourceManager::GetInstance().CreateShader(L"Boss", L"..\\Resources\\Shader\\boss.hlsli", bossShaderInfo);

	// Effec test
	ShaderInfo textureAnimShaderInfo;
	textureAnimShaderInfo.shaderType = SHADER_TYPE::EFFECT_ANIM;
	textureAnimShaderInfo.blendType = BLEND_TYPE::ALPHA_BLEND;
	ResourceManager::GetInstance().CreateShader(L"TextureAnim", L"..\\Resources\\Shader\\TextureAnim.hlsli", textureAnimShaderInfo);

	/////////////////////ShaderInfo effectSubShaderInfo;
	///////////////////////effectSubShaderInfo.depthStencilType = DEPTH_STENCIL_TYPE::;
	/////////////////////effectSubShaderInfo.shaderType = SHADER_TYPE::EFFECT;
	/////////////////////effectSubShaderInfo.blendType = BLEND_TYPE::ALPHA_BLEND;
	/////////////////////ResourceManager::GetInstance().CreateShader(L"EffectSub", L"..\\Resources\\Shader\\EffectSub.hlsli", effectSubShaderInfo);

	// Shader
	ShaderInfo uiShaderInfo;
	uiShaderInfo.rasterizerType = RASTERIZER_TYPE::CULL_NONE;
	uiShaderInfo.depthStencilType = DEPTH_STENCIL_TYPE::NO_DEPTH_TEST_NO_WRITE;
	uiShaderInfo.shaderType = SHADER_TYPE::UI;
	uiShaderInfo.blendType = BLEND_TYPE::ALPHA_BLEND;
	ResourceManager::GetInstance().CreateShader(L"UI", L"..\\Resources\\Shader\\UI.hlsli", uiShaderInfo);

	ShaderInfo deferredShaderInfo;
	deferredShaderInfo.shaderType = SHADER_TYPE::DEFERRED;
	ResourceManager::GetInstance().CreateShader(L"Deferred", L"..\\Resources\\Shader\\deferred.hlsli", deferredShaderInfo);

	ShaderInfo forwardShaderInfo;
	ResourceManager::GetInstance().CreateShader(L"Forward", L"..\\Resources\\Shader\\forward.hlsli", forwardShaderInfo);

	ShaderInfo skyBoxShaderInfo;
	skyBoxShaderInfo.rasterizerType = RASTERIZER_TYPE::CULL_FRONT;
	skyBoxShaderInfo.depthStencilType = DEPTH_STENCIL_TYPE::LESS_EQUAL;
	ResourceManager::GetInstance().CreateShader(L"SkyBox", L"..\\Resources\\Shader\\skybox.hlsli", skyBoxShaderInfo);

	ShaderInfo debugShaderInfo;
	debugShaderInfo.shaderType = SHADER_TYPE::FORWARD;
	debugShaderInfo.rasterizerType = RASTERIZER_TYPE::WIREFRAME;
	ResourceManager::GetInstance().CreateShader(L"Debug", L"..\\Resources\\Shader\\debug.hlsli", debugShaderInfo);

	ShaderInfo debugLineShaderInfo;
	debugLineShaderInfo.shaderType = SHADER_TYPE::FORWARD;
	debugLineShaderInfo.rasterizerType = RASTERIZER_TYPE::WIREFRAME;
	debugLineShaderInfo.topology = FFTopology::D3D_PRIMITIVE_TOPOLOGY_LINELIST;
	ResourceManager::GetInstance().CreateShader(L"Debug_Line", L"..\\Resources\\Shader\\debug_line.hlsli", debugLineShaderInfo);

	ShaderInfo lightingDirectionalShaderInfo;
	lightingDirectionalShaderInfo.shaderType = SHADER_TYPE::LIGHTING;
	lightingDirectionalShaderInfo.rasterizerType = RASTERIZER_TYPE::CULL_NONE;
	lightingDirectionalShaderInfo.depthStencilType = DEPTH_STENCIL_TYPE::NO_DEPTH_TEST_NO_WRITE;
	lightingDirectionalShaderInfo.blendType = BLEND_TYPE::ONE_TO_ONE_BLEND;
	lightingDirectionalShaderInfo.vs = "VS_DirLight";
	lightingDirectionalShaderInfo.ps = "PS_DirLight";
	ResourceManager::GetInstance().CreateShader(L"LightingDirectional", L"..\\Resources\\Shader\\lightingDirectional.hlsli", lightingDirectionalShaderInfo);

	ShaderInfo skinnedMeshShader;
	skinnedMeshShader.shaderType = SHADER_TYPE::DEFERRED_AND_SKIN;
	ResourceManager::GetInstance().CreateShader(L"SkinnedMesh", L"..\\Resources\\Shader\\deferred_skinned.hlsli", skinnedMeshShader);
	
#pragma region 디졸브 효과
	{
		ResourceManager::GetInstance().CreateTexture(L"Delete", L"..\\Resources\\Texture\\BossFlag\\delete.jpg");
		ResourceManager::GetInstance().CreateTexture(L"Flag2", L"..\\Resources\\Texture\\BossFlag\\T_Flag_Diffuse_002.png");
	}
#pragma endregion

	// Material
#pragma region UI Material
	{
		// TODO
		MaterialInfo material;
		material.shader = L"..\\Resources\\Shader\\UI.hlsli";
		//material.texture[0] = L".png";
		ResourceManager::GetInstance().CreateMaterial(L"UI", material);
	}
#pragma endregion

#pragma region Deferred Render Material
	{
		MaterialInfo material;
		material.shader = L"..\\Resources\\Shader\\deferred.hlsli";
		ResourceManager::GetInstance().CreateMaterial(L"CubeMaterial", material);
	}
#pragma endregion

#pragma region SkyBox
	{
		MaterialInfo material;
		material.shader = L"..\\Resources\\Shader\\skybox.hlsli";
		material.texture[0] = L"..\\Resources\\Texture\\Zelda.jpg";
		ResourceManager::GetInstance().CreateMaterial(L"SkyBoxMaterial", material);
	}
#pragma endregion

#pragma region Forward Render Material
	{
		MaterialInfo material;
		material.shader = L"..\\Resources\\Shader\\forward.hlsli";
		ResourceManager::GetInstance().CreateMaterial(L"CubeMaterial_F", material);
	}
#pragma endregion

#pragma region DirectionalLight Material
	{
		MaterialInfo material;
		material.shader = L"..\\Resources\\Shader\\lightingDirectional.hlsli";
		material.texture[0] = L"D_Position";
		material.texture[1] = L"D_Normal";
		ResourceManager::GetInstance().CreateMaterial(L"DirLight", material);
	}
#pragma endregion

#pragma region Collider Boundary
	{
		MaterialInfo material;
		material.shader = L"..\\Resources\\Shader\\debug.hlsli";
		material.color = ZeldaColor::Blue;
		ResourceManager::GetInstance().CreateMaterial(L"BoundaryDebug", material);
	}
#pragma endregion

#pragma region Line Boundary
	{
		MaterialInfo material;
		material.shader = L"..\\Resources\\Shader\\debug_line.hlsli";
		material.color = ZeldaColor::Blue;
		ResourceManager::GetInstance().CreateMaterial(L"BoundaryDebug_Line", material);
	}
#pragma endregion

#pragma region Effect FBX
	{
		ResourceManager::GetInstance().FBXLoad(L"VFX", L"..\\Resources\\FBX\\VFX\\VFX.fbx", L"..\\Resources\\Shader\\TextureAnim.hlsli");
	}
#pragma endregion

#pragma region Player FBX
	{
		ResourceManager::GetInstance().FBXLoad(L"Player", L"..\\Resources\\FBX\\Player\\Player.fbx", L"..\\Resources\\Shader\\deferred_skinned_shadow.hlsli");
	}
#pragma endregion

#pragma region BossDoor FBX
	{
		ResourceManager::GetInstance().FBXLoad(L"BossDoor", L"..\\Resources\\FBX\\BossDoor\\BossDoor.fbx", L"..\\Resources\\Shader\\deferred.hlsli");
	}
#pragma endregion

#pragma region BossDoorHand FBX
	{
		ResourceManager::GetInstance().FBXLoad(L"BossDoorHand", L"..\\Resources\\FBX\\BossDoorHand\\BossDoorHand.fbx", L"..\\Resources\\Shader\\deferred_skinned.hlsli");
	}
#pragma endregion

#pragma region BossFloor FBX
	{
		ResourceManager::GetInstance().FBXLoad(L"BossFloor", L"..\\Resources\\FBX\\BossFloor\\BossFloor.fbx", L"..\\Resources\\Shader\\deferred.hlsli");
	}
#pragma endregion

#pragma region BossFlag FBX
	{
		ResourceManager::GetInstance().FBXLoad(L"BossFlag", L"..\\Resources\\FBX\\BossFlag\\BossFlag.fbx", L"..\\Resources\\Shader\\BossFlag.hlsli");
	}
#pragma endregion

#pragma region Boss FBX
	{
		ResourceManager::GetInstance().FBXLoad(L"Boss", L"..\\Resources\\FBX\\Boss\\Boss.fbx", L"..\\Resources\\Shader\\boss.hlsli");
	}
#pragma endregion

#pragma region Scroll FBX
	{
		ResourceManager::GetInstance().FBXLoad(L"Scroll", L"..\\Resources\\FBX\\Scroll\\Scroll.fbx", L"..\\Resources\\Shader\\deferred.hlsli");
	}
#pragma endregion

#pragma region VFX_Circle FBX
	{
		ResourceManager::GetInstance().FBXLoad(L"VFX_Circle", L"..\\Resources\\FBX\\VFX_Circle\\VFX_Circle.fbx", L"..\\Resources\\Shader\\TextureAnim.hlsli");
	}
#pragma endregion

#pragma region VFX_Tree FBX
	{
		ResourceManager::GetInstance().FBXLoad(L"VFX_Tree", L"..\\Resources\\FBX\\VFX_Tree\\VFX_Tree.fbx", L"..\\Resources\\Shader\\TextureAnim.hlsli");
	}
#pragma endregion

#pragma region VFX_BossNormalAttack_L FBX
	{
		ResourceManager::GetInstance().FBXLoad(L"VFX_BossNormalAttack_L", L"..\\Resources\\FBX\\VFX_BossNormalAttack_L\\vfx_BossNormalAttack_L.fbx", L"..\\Resources\\Shader\\TextureAnim.hlsli");
	}
#pragma endregion

#pragma region VFX_BossNormalAttack_R FBX
	{
		ResourceManager::GetInstance().FBXLoad(L"VFX_BossNormalAttack_R", L"..\\Resources\\FBX\\VFX_BossNormalAttack_R\\vfx_BossNormalAttack_R.fbx", L"..\\Resources\\Shader\\TextureAnim.hlsli");
	}
#pragma endregion

#pragma region VFX_Knockback FBX
	{
		ResourceManager::GetInstance().FBXLoad(L"VFX_Knockback", L"..\\Resources\\FBX\\VFX_Knockback\\VFX_Knockback.fbx", L"..\\Resources\\Shader\\TextureAnim.hlsli");
	}
#pragma endregion

}

void CreateResourceGame()
{
	// Mesh
	ResourceManager::GetInstance().CreateMesh(L"Cube", L"Cube");
	ResourceManager::GetInstance().CreateMesh(L"Sphere", L"Sphere");
	ResourceManager::GetInstance().CreateMesh(L"Capsule", L"Capsule");
	ResourceManager::GetInstance().CreateMesh(L"Cylinder", L"Cylinder");
	ResourceManager::GetInstance().CreateMesh(L"Rectangle", L"Rectangle");
	ResourceManager::GetInstance().CreateMesh(L"UI", L"Rectangle");
	ResourceManager::GetInstance().CreateMesh(L"Sprite", L"Rectangle");
	ResourceManager::GetInstance().CreateMesh(L"Line", L"Line");
	ResourceManager::GetInstance().CreateMesh(L"Point", L"Point");


	// Shader
	ShaderInfo spriteShaderInfo;
	spriteShaderInfo.shaderType = SHADER_TYPE::FORWARD;
	spriteShaderInfo.blendType = BLEND_TYPE::ALPHA_BLEND;
	spriteShaderInfo.depthStencilType = DEPTH_STENCIL_TYPE::LESS_NO_WRITE;
	ResourceManager::GetInstance().CreateShader(L"Sprite", L"..\\Resources\\Shader\\Sprite3D.hlsli", spriteShaderInfo);

	ShaderInfo deferredShaderInfo;
	deferredShaderInfo.shaderType = SHADER_TYPE::DEFERRED;
	ResourceManager::GetInstance().CreateShader(L"Deferred", L"..\\Resources\\Shader\\deferred.hlsli", deferredShaderInfo);

	ShaderInfo uiShaderInfo;
	uiShaderInfo.rasterizerType = RASTERIZER_TYPE::CULL_NONE;
	uiShaderInfo.depthStencilType = DEPTH_STENCIL_TYPE::NO_DEPTH_TEST_NO_WRITE;
	uiShaderInfo.shaderType = SHADER_TYPE::UI;
	uiShaderInfo.blendType = BLEND_TYPE::ALPHA_BLEND;
	ResourceManager::GetInstance().CreateShader(L"UI", L"..\\Resources\\Shader\\UI.hlsli", uiShaderInfo);

	ShaderInfo lightingDirectionalShaderInfo;
	lightingDirectionalShaderInfo.shaderType = SHADER_TYPE::LIGHTING;
	lightingDirectionalShaderInfo.rasterizerType = RASTERIZER_TYPE::CULL_NONE;
	lightingDirectionalShaderInfo.depthStencilType = DEPTH_STENCIL_TYPE::NO_DEPTH_TEST_NO_WRITE;
	lightingDirectionalShaderInfo.blendType = BLEND_TYPE::ONE_TO_ONE_BLEND;
	lightingDirectionalShaderInfo.vs = "VS_DirLight";
	lightingDirectionalShaderInfo.ps = "PS_DirLight";
	ResourceManager::GetInstance().CreateShader(L"LightingDirectional", L"..\\Resources\\Shader\\lightingDirectional.hlsli", lightingDirectionalShaderInfo);

	ShaderInfo debugShaderInfo;
	debugShaderInfo.shaderType = SHADER_TYPE::FORWARD;
	debugShaderInfo.rasterizerType = RASTERIZER_TYPE::WIREFRAME;
	ResourceManager::GetInstance().CreateShader(L"Debug", L"..\\Resources\\Shader\\debug.hlsli", debugShaderInfo);

	ShaderInfo debugLineShaderInfo;
	debugLineShaderInfo.shaderType = SHADER_TYPE::FORWARD;
	debugLineShaderInfo.rasterizerType = RASTERIZER_TYPE::WIREFRAME;
	debugLineShaderInfo.topology = FFTopology::D3D_PRIMITIVE_TOPOLOGY_LINELIST;
	ResourceManager::GetInstance().CreateShader(L"Debug_Line", L"..\\Resources\\Shader\\debug_line.hlsli", debugLineShaderInfo);

	ShaderInfo skinnedMeshShader;
	skinnedMeshShader.shaderType = SHADER_TYPE::DEFERRED_AND_SKIN;
	ResourceManager::GetInstance().CreateShader(L"SkinnedMesh", L"..\\Resources\\Shader\\deferred_skinned.hlsli", skinnedMeshShader);

	// 디퍼드 + 스키닝 + 그림자
	ShaderInfo deferredSkinnedShadowShaderInfo;
	deferredSkinnedShadowShaderInfo.shaderType = SHADER_TYPE::DEFFERRED_SKIN_SHADOW;
	ResourceManager::GetInstance().CreateShader(L"deferred_skinned_shadow", L"..\\Resources\\Shader\\deferred_skinned_shadow.hlsli", deferredSkinnedShadowShaderInfo);

	// 디퍼드이면서 그림자 출력
	ShaderInfo deferredShadowShaderInfo;
	deferredShadowShaderInfo.shaderType = SHADER_TYPE::DEFFERRED_SHADOW;
	ResourceManager::GetInstance().CreateShader(L"deferred_shadow", L"..\\Resources\\Shader\\deferred_shadow.hlsli", deferredShadowShaderInfo);

	ShaderInfo forwardShaderInfo;
	ResourceManager::GetInstance().CreateShader(L"Forward", L"..\\Resources\\Shader\\forward.hlsli", forwardShaderInfo);

	ShaderInfo skyBoxShaderInfo;
	skyBoxShaderInfo.rasterizerType = RASTERIZER_TYPE::CULL_FRONT;
	skyBoxShaderInfo.depthStencilType = DEPTH_STENCIL_TYPE::LESS_EQUAL;
	ResourceManager::GetInstance().CreateShader(L"SkyBox", L"..\\Resources\\Shader\\skybox.hlsli", skyBoxShaderInfo);

	// Effec test
	ShaderInfo textureAnimShaderInfo;
	textureAnimShaderInfo.shaderType = SHADER_TYPE::EFFECT_ANIM;
	textureAnimShaderInfo.blendType = BLEND_TYPE::ALPHA_BLEND;
	textureAnimShaderInfo.rasterizerType = RASTERIZER_TYPE::CULL_NONE;
	ResourceManager::GetInstance().CreateShader(L"TextureAnim", L"..\\Resources\\Shader\\TextureAnim.hlsli", textureAnimShaderInfo);

	// BossFloor
	ShaderInfo bossFloorShaderInfo;
	//textureAnimShaderInfo.depthStencilType = DEPTH_STENCIL_TYPE::NO_DEPTH_TEST_NO_WRITE;
	bossFloorShaderInfo.shaderType = SHADER_TYPE::FORWARD;
	bossFloorShaderInfo.blendType = BLEND_TYPE::ALPHA_BLEND;
	bossFloorShaderInfo.rasterizerType = RASTERIZER_TYPE::CULL_NONE;
	ResourceManager::GetInstance().CreateShader(L"BossFloor", L"..\\Resources\\Shader\\BossFloor.hlsli", bossFloorShaderInfo);

	// EffectGuideLine
	ShaderInfo guideLineShaderInfo;
	guideLineShaderInfo.shaderType = SHADER_TYPE::FORWARD;
	guideLineShaderInfo.blendType = BLEND_TYPE::ALPHA_BLEND;
	guideLineShaderInfo.rasterizerType = RASTERIZER_TYPE::CULL_NONE;
	ResourceManager::GetInstance().CreateShader(L"GuideLine", L"..\\Resources\\Shader\\GuideLine.hlsli", guideLineShaderInfo);

	// Boss
	ShaderInfo bossShaderInfo;
	bossShaderInfo.shaderType = SHADER_TYPE::DEFFERRED_SKIN_SHADOW;
	bossShaderInfo.rasterizerType = RASTERIZER_TYPE::CULL_NONE;
	ResourceManager::GetInstance().CreateShader(L"Boss", L"..\\Resources\\Shader\\boss.hlsli", bossShaderInfo);

	// 디퍼드이면서 그림자 출력
	ShaderInfo bossProjectileShaderInfo;
	bossProjectileShaderInfo.shaderType = SHADER_TYPE::DEFFERRED_SHADOW;
	bossProjectileShaderInfo.blendType = BLEND_TYPE::ALPHA_BLEND;
	bossProjectileShaderInfo.depthStencilType = DEPTH_STENCIL_TYPE::LESS_NO_WRITE;
	ResourceManager::GetInstance().CreateShader(L"BossProjectile", L"..\\Resources\\Shader\\BossProjectile.hlsli", bossProjectileShaderInfo);

	// Player
	ShaderInfo playerShaderInfo;
	playerShaderInfo.shaderType = SHADER_TYPE::DEFFERRED_SKIN_SHADOW;
	ResourceManager::GetInstance().CreateShader(L"Player", L"..\\Resources\\Shader\\player.hlsli", playerShaderInfo);

	// 보스 깃발 용 셰이더
	ShaderInfo bossFlagShaderInfo;
	bossFlagShaderInfo.shaderType = SHADER_TYPE::FORWARD;
	bossFlagShaderInfo.blendType = BLEND_TYPE::ALPHA_BLEND;
	ResourceManager::GetInstance().CreateShader(L"BossFlag", L"..\\Resources\\Shader\\BossFlag.hlsli", bossFlagShaderInfo);

	// Texture
	/// TitleUI
	ResourceManager::GetInstance().CreateTexture(L"TitleBackground", L"..\\Resources\\Texture\\UI\\TitleUI\\TitleBackground.png");
	ResourceManager::GetInstance().CreateTexture(L"BlackCoverBackground", L"..\\Resources\\Texture\\UI\\InGameUI\\BlackCoverBackground.png");

	ResourceManager::GetInstance().CreateTexture(L"AttachUI", L"..\\Resources\\Texture\\UI\\etc\\pointer.png");

	ResourceManager::GetInstance().CreateTexture(L"Exit_Idle", L"..\\Resources\\Texture\\UI\\TitleUI\\exit.png");
	ResourceManager::GetInstance().CreateTexture(L"Exit_Enter", L"..\\Resources\\Texture\\UI\\TitleUI\\exit_enter.png");
	ResourceManager::GetInstance().CreateTexture(L"Exit_Click", L"..\\Resources\\Texture\\UI\\TitleUI\\exit_click.png");

	ResourceManager::GetInstance().CreateTexture(L"Start_Idle", L"..\\Resources\\Texture\\UI\\TitleUI\\start.png");
	ResourceManager::GetInstance().CreateTexture(L"Start_Enter", L"..\\Resources\\Texture\\UI\\TitleUI\\start_enter.png");
	ResourceManager::GetInstance().CreateTexture(L"Start_Click", L"..\\Resources\\Texture\\UI\\TitleUI\\start_click.png");

	/// InGameUI
	ResourceManager::GetInstance().CreateTexture(L"HPBarBase", L"..\\Resources\\Texture\\UI\\InGameUI\\HPBarBase.png");
	ResourceManager::GetInstance().CreateTexture(L"HPBar", L"..\\Resources\\Texture\\UI\\InGameUI\\HPBar.png");
	ResourceManager::GetInstance().CreateTexture(L"HPBarCover", L"..\\Resources\\Texture\\UI\\InGameUI\\HPBarCover.png");

	ResourceManager::GetInstance().CreateTexture(L"DashBase", L"..\\Resources\\Texture\\UI\\InGameUI\\DashBase.png");
	ResourceManager::GetInstance().CreateTexture(L"DashMoveCover", L"..\\Resources\\Texture\\UI\\InGameUI\\Dash_Middle.png");
	ResourceManager::GetInstance().CreateTexture(L"DashCover", L"..\\Resources\\Texture\\UI\\InGameUI\\Dash_Top.png");

	ResourceManager::GetInstance().CreateTexture(L"StandoffBase", L"..\\Resources\\Texture\\UI\\InGameUI\\StandoffBase.png");
	ResourceManager::GetInstance().CreateTexture(L"StandoffMoveCover", L"..\\Resources\\Texture\\UI\\InGameUI\\CoolTimeMoveCover.png");
	ResourceManager::GetInstance().CreateTexture(L"StandoffCover", L"..\\Resources\\Texture\\UI\\InGameUI\\CoolTimeCover.png");

	ResourceManager::GetInstance().CreateTexture(L"CoolTime_DisableCover", L"..\\Resources\\Texture\\UI\\InGameUI\\CoolTime_DisableCover.png");

	/// Player
	ResourceManager::GetInstance().CreateTexture(L"PlayerDamaged_0", L"..\\Resources\\Texture\\playerdamage\\player_damage_0.png");
	ResourceManager::GetInstance().CreateTexture(L"PlayerDamaged_1", L"..\\Resources\\Texture\\playerdamage\\player_damage_1.png");
	ResourceManager::GetInstance().CreateTexture(L"PlayerDamaged_2", L"..\\Resources\\Texture\\playerdamage\\player_damage_2.png");
	ResourceManager::GetInstance().CreateTexture(L"PlayerDamaged_3", L"..\\Resources\\Texture\\playerdamage\\player_damage_3.png");
	ResourceManager::GetInstance().CreateTexture(L"PlayerDamaged_4", L"..\\Resources\\Texture\\playerdamage\\player_damage_4.png");


	/// Setting UI
	ResourceManager::GetInstance().CreateTexture(L"SettingUI_Idle", L"..\\Resources\\Texture\\UI\\InGameUI\\SettingUI.png");
	ResourceManager::GetInstance().CreateTexture(L"SettingUI_Enter", L"..\\Resources\\Texture\\UI\\InGameUI\\SettingUI_Enter.png");
	ResourceManager::GetInstance().CreateTexture(L"SettingUI_Click", L"..\\Resources\\Texture\\UI\\InGameUI\\SettingUI.png");
	ResourceManager::GetInstance().CreateTexture(L"SettingBackground", L"..\\Resources\\Texture\\UI\\InGameUI\\SettingBackground.png");
	ResourceManager::GetInstance().CreateTexture(L"ScrollUI", L"..\\Resources\\Texture\\UI\\InGameUI\\ScrollUI.png");
	ResourceManager::GetInstance().CreateTexture(L"SoundHandle", L"..\\Resources\\Texture\\UI\\InGameUI\\SoundHandle.png");
	ResourceManager::GetInstance().CreateTexture(L"SoundText", L"..\\Resources\\Texture\\UI\\InGameUI\\SoundText.png");
	ResourceManager::GetInstance().CreateTexture(L"Title_Idle", L"..\\Resources\\Texture\\UI\\InGameUI\\Title_Idle.png");
	ResourceManager::GetInstance().CreateTexture(L"Title_Enter", L"..\\Resources\\Texture\\UI\\InGameUI\\Title_Enter.png");
	ResourceManager::GetInstance().CreateTexture(L"Title_Click", L"..\\Resources\\Texture\\UI\\InGameUI\\Title_Click.png");
	ResourceManager::GetInstance().CreateTexture(L"Continue_Idle", L"..\\Resources\\Texture\\UI\\InGameUI\\Continue_Idle.png");
	ResourceManager::GetInstance().CreateTexture(L"Continue_Enter", L"..\\Resources\\Texture\\UI\\InGameUI\\Continue_Enter.png");
	ResourceManager::GetInstance().CreateTexture(L"Continue_Click", L"..\\Resources\\Texture\\UI\\InGameUI\\Continue_Click.png");
	ResourceManager::GetInstance().CreateTexture(L"Exit_Ingame_Idle", L"..\\Resources\\Texture\\UI\\InGameUI\\ui_option_quit.png");
	ResourceManager::GetInstance().CreateTexture(L"Exit_Ingame_Enter", L"..\\Resources\\Texture\\UI\\InGameUI\\ui_option_quit_hover.png");
	ResourceManager::GetInstance().CreateTexture(L"Exit_Ingame_Click", L"..\\Resources\\Texture\\UI\\InGameUI\\ui_option_quit.png");
	ResourceManager::GetInstance().CreateTexture(L"SoundText_Ingame_Idle", L"..\\Resources\\Texture\\UI\\InGameUI\\ui_option_sound.png");
	ResourceManager::GetInstance().CreateTexture(L"SoundText_Ingame_Enter", L"..\\Resources\\Texture\\UI\\InGameUI\\ui_option_sound_hover.png");
	ResourceManager::GetInstance().CreateTexture(L"SoundText_Ingame_Click", L"..\\Resources\\Texture\\UI\\InGameUI\\ui_option_sound.png");

	// Number
	ResourceManager::GetInstance().CreateTexture(L"None", L"..\\Resources\\Texture\\UI\\InGameUI\\None.png");
	ResourceManager::GetInstance().CreateTexture(L"Num_0", L"..\\Resources\\Texture\\UI\\InGameUI\\Num_0.png");
	ResourceManager::GetInstance().CreateTexture(L"Num_1", L"..\\Resources\\Texture\\UI\\InGameUI\\Num_1.png");
	ResourceManager::GetInstance().CreateTexture(L"Num_2", L"..\\Resources\\Texture\\UI\\InGameUI\\Num_2.png");
	ResourceManager::GetInstance().CreateTexture(L"Num_3", L"..\\Resources\\Texture\\UI\\InGameUI\\Num_3.png");
	ResourceManager::GetInstance().CreateTexture(L"Num_4", L"..\\Resources\\Texture\\UI\\InGameUI\\Num_4.png");
	ResourceManager::GetInstance().CreateTexture(L"Num_5", L"..\\Resources\\Texture\\UI\\InGameUI\\Num_5.png");
	ResourceManager::GetInstance().CreateTexture(L"Num_6", L"..\\Resources\\Texture\\UI\\InGameUI\\Num_6.png");
	ResourceManager::GetInstance().CreateTexture(L"Num_7", L"..\\Resources\\Texture\\UI\\InGameUI\\Num_7.png");
	ResourceManager::GetInstance().CreateTexture(L"Num_8", L"..\\Resources\\Texture\\UI\\InGameUI\\Num_8.png");
	ResourceManager::GetInstance().CreateTexture(L"Num_9", L"..\\Resources\\Texture\\UI\\InGameUI\\Num_9.png");

	// 배경
	ResourceManager::GetInstance().CreateTexture(L"BackGround", L"..\\Resources\\Texture\\backgrounds.png");

	// GameOver
	ResourceManager::GetInstance().CreateTexture(L"GameOverBackground", L"..\\Resources\\Texture\\GameOver\\gameover.png");
	ResourceManager::GetInstance().CreateTexture(L"Title_GameOver_Idle", L"..\\Resources\\Texture\\UI\\InGameUI\\ui_gameover_gotitle.png");
	ResourceManager::GetInstance().CreateTexture(L"Title_GameOver_Enter", L"..\\Resources\\Texture\\UI\\InGameUI\\ui_gameover_gotitle_hover.png");
	ResourceManager::GetInstance().CreateTexture(L"Title_GameOver_Click", L"..\\Resources\\Texture\\UI\\InGameUI\\ui_gameover_gotitle.png");
	ResourceManager::GetInstance().CreateTexture(L"Restart_Idle", L"..\\Resources\\Texture\\UI\\InGameUI\\ui_gameover_restart.png");
	ResourceManager::GetInstance().CreateTexture(L"Restart_Enter", L"..\\Resources\\Texture\\UI\\InGameUI\\ui_gameover_restart_hover.png");
	ResourceManager::GetInstance().CreateTexture(L"Restart_Click", L"..\\Resources\\Texture\\UI\\InGameUI\\ui_gameover_restart.png");

	// intro
	ResourceManager::GetInstance().CreateTexture(L"Intro", L"..\\Resources\\Texture\\Intro\\intro.png");
	ResourceManager::GetInstance().CreateTexture(L"Intro_Gradation", L"..\\Resources\\Texture\\Intro\\intro_gradation.png");

	ResourceManager::GetInstance().CreateTexture(L"Phase2CutScene1_0", L"..\\Resources\\Texture\\phase_cutscene\\2phase\\first\\2phasestart_first_0.png");
	ResourceManager::GetInstance().CreateTexture(L"Phase2CutScene1_1", L"..\\Resources\\Texture\\phase_cutscene\\2phase\\first\\2phasestart_first_1.png");
	ResourceManager::GetInstance().CreateTexture(L"Phase2CutScene1_2", L"..\\Resources\\Texture\\phase_cutscene\\2phase\\first\\2phasestart_first_2.png");
	ResourceManager::GetInstance().CreateTexture(L"Phase2CutScene1_3", L"..\\Resources\\Texture\\phase_cutscene\\2phase\\first\\2phasestart_first_3.png");
	ResourceManager::GetInstance().CreateTexture(L"Phase2CutScene1_4", L"..\\Resources\\Texture\\phase_cutscene\\2phase\\first\\2phasestart_first_4.png");
	ResourceManager::GetInstance().CreateTexture(L"Phase2CutScene1_5", L"..\\Resources\\Texture\\phase_cutscene\\2phase\\first\\2phasestart_first_5.png");
	ResourceManager::GetInstance().CreateTexture(L"Phase2CutScene1_6", L"..\\Resources\\Texture\\phase_cutscene\\2phase\\first\\2phasestart_first_6.png");

	ResourceManager::GetInstance().CreateTexture(L"Phase2CutScene2_0", L"..\\Resources\\Texture\\phase_cutscene\\2phase\\second\\2phasestart_second_0.png");
	ResourceManager::GetInstance().CreateTexture(L"Phase2CutScene2_1", L"..\\Resources\\Texture\\phase_cutscene\\2phase\\second\\2phasestart_second_1.png");
	ResourceManager::GetInstance().CreateTexture(L"Phase2CutScene2_2", L"..\\Resources\\Texture\\phase_cutscene\\2phase\\second\\2phasestart_second_2.png");
	ResourceManager::GetInstance().CreateTexture(L"Phase2CutScene2_3", L"..\\Resources\\Texture\\phase_cutscene\\2phase\\second\\2phasestart_second_3.png");
	ResourceManager::GetInstance().CreateTexture(L"Phase2CutScene2_4", L"..\\Resources\\Texture\\phase_cutscene\\2phase\\second\\2phasestart_second_4.png");
	ResourceManager::GetInstance().CreateTexture(L"Phase2CutScene2_5", L"..\\Resources\\Texture\\phase_cutscene\\2phase\\second\\2phasestart_second_5.png");
	ResourceManager::GetInstance().CreateTexture(L"Phase2CutScene2_6", L"..\\Resources\\Texture\\phase_cutscene\\2phase\\second\\2phasestart_second_6.png");
	ResourceManager::GetInstance().CreateTexture(L"Phase2CutScene2_7", L"..\\Resources\\Texture\\phase_cutscene\\2phase\\second\\2phasestart_second_7.png");
	ResourceManager::GetInstance().CreateTexture(L"Phase2CutScene2_8", L"..\\Resources\\Texture\\phase_cutscene\\2phase\\second\\2phasestart_second_8.png");
	ResourceManager::GetInstance().CreateTexture(L"Phase2CutScene2_9", L"..\\Resources\\Texture\\phase_cutscene\\2phase\\second\\2phasestart_second_9.png");

	ResourceManager::GetInstance().CreateTexture(L"Phase3CutScene0", L"..\\Resources\\Texture\\phase_cutscene\\3phase\\3phasestart_0.png");
	ResourceManager::GetInstance().CreateTexture(L"Phase3CutScene1", L"..\\Resources\\Texture\\phase_cutscene\\3phase\\3phasestart_1.png");
	ResourceManager::GetInstance().CreateTexture(L"Phase3CutScene2", L"..\\Resources\\Texture\\phase_cutscene\\3phase\\3phasestart_2.png");
	ResourceManager::GetInstance().CreateTexture(L"Phase3CutScene3", L"..\\Resources\\Texture\\phase_cutscene\\3phase\\3phasestart_3.png");
	ResourceManager::GetInstance().CreateTexture(L"Phase3CutScene4", L"..\\Resources\\Texture\\phase_cutscene\\3phase\\3phasestart_4.png");
	ResourceManager::GetInstance().CreateTexture(L"Phase3CutScene5", L"..\\Resources\\Texture\\phase_cutscene\\3phase\\3phasestart_5.png");
	ResourceManager::GetInstance().CreateTexture(L"Phase3CutScene6", L"..\\Resources\\Texture\\phase_cutscene\\3phase\\3phasestart_6.png");
	ResourceManager::GetInstance().CreateTexture(L"Phase3CutScene7", L"..\\Resources\\Texture\\phase_cutscene\\3phase\\3phasestart_7.png");
	ResourceManager::GetInstance().CreateTexture(L"Phase3CutScene8", L"..\\Resources\\Texture\\phase_cutscene\\3phase\\3phasestart_8.png");
	ResourceManager::GetInstance().CreateTexture(L"Phase3CutScene9", L"..\\Resources\\Texture\\phase_cutscene\\3phase\\3phasestart_9.png");
	ResourceManager::GetInstance().CreateTexture(L"Phase3CutScene10", L"..\\Resources\\Texture\\phase_cutscene\\3phase\\3phasestart_10.png");
	ResourceManager::GetInstance().CreateTexture(L"Phase3CutScene11", L"..\\Resources\\Texture\\phase_cutscene\\3phase\\3phasestart_11.png");
	ResourceManager::GetInstance().CreateTexture(L"Phase3CutScene12", L"..\\Resources\\Texture\\phase_cutscene\\3phase\\3phasestart_12.png");

	ResourceManager::GetInstance().CreateTexture(L"EndingCutScene0", L"..\\Resources\\Texture\\phase_cutscene\\ending\\ending_0.png");
	ResourceManager::GetInstance().CreateTexture(L"EndingCutScene1", L"..\\Resources\\Texture\\phase_cutscene\\ending\\ending_1.png");
	ResourceManager::GetInstance().CreateTexture(L"EndingCutScene2", L"..\\Resources\\Texture\\phase_cutscene\\ending\\ending_2.png");

	// etc
	ResourceManager::GetInstance().CreateTexture(L"Black", L"..\\Resources\\Texture\\UI\\etc\\Our Future.png");
	ResourceManager::GetInstance().CreateTexture(L"White", L"..\\Resources\\Texture\\UI\\etc\\A being out of this world.png");
	ResourceManager::GetInstance().CreateTexture(L"Delete", L"..\\Resources\\Texture\\BossFlag\\delete.jpg");
	ResourceManager::GetInstance().CreateTexture(L"Flag2", L"..\\Resources\\Texture\\BossFlag\\T_Flag_Diffuse_002.png");

	/// Boss
	ResourceManager::GetInstance().CreateTexture(L"Normal2DPatternTexture_0", L"..\\Resources\\Texture\\2DPattern\\brushstroke_2phase_0.png");
	ResourceManager::GetInstance().CreateTexture(L"Normal2DPatternTexture_1", L"..\\Resources\\Texture\\2DPattern\\brushstroke_2phase_1.png");
	ResourceManager::GetInstance().CreateTexture(L"Normal2DPatternTexture_2", L"..\\Resources\\Texture\\2DPattern\\brushstroke_2phase_2.png");
	ResourceManager::GetInstance().CreateTexture(L"Normal2DPatternTexture_3", L"..\\Resources\\Texture\\2DPattern\\brushstroke_2phase_3.png");
	ResourceManager::GetInstance().CreateTexture(L"Normal2DPatternTexture_4", L"..\\Resources\\Texture\\2DPattern\\brushstroke_2phase_4.png");
	ResourceManager::GetInstance().CreateTexture(L"Normal2DPatternTexture_5", L"..\\Resources\\Texture\\2DPattern\\brushstroke_2phase_5.png");
	ResourceManager::GetInstance().CreateTexture(L"Normal2DPatternTexture_Warning", L"..\\Resources\\Texture\\2DPattern\\brushstroke_2phase_warning.png");

	ResourceManager::GetInstance().CreateTexture(L"widebrush_bottom", L"..\\Resources\\Texture\\2DPattern\\2phase_widebrush\\widebrush_bottom.png");
	ResourceManager::GetInstance().CreateTexture(L"widebrush_top", L"..\\Resources\\Texture\\2DPattern\\2phase_widebrush\\widebrush_top.png");
	ResourceManager::GetInstance().CreateTexture(L"widebrush_right", L"..\\Resources\\Texture\\2DPattern\\2phase_widebrush\\widebrush_right.png");
	ResourceManager::GetInstance().CreateTexture(L"widebrush_left", L"..\\Resources\\Texture\\2DPattern\\2phase_widebrush\\widebrush_left.png");
	ResourceManager::GetInstance().CreateTexture(L"widebrush_horizontal_warning", L"..\\Resources\\Texture\\2DPattern\\2phase_widebrush\\widebrush_horizontal_warning.png");
	ResourceManager::GetInstance().CreateTexture(L"widebrush_vertical_warning", L"..\\Resources\\Texture\\2DPattern\\2phase_widebrush\\widebrush_vertical_warning.png");

	ResourceManager::GetInstance().CreateTexture(L"boss_damage_0", L"..\\Resources\\Texture\\bossdamage\\boss_damage_0.png");
	ResourceManager::GetInstance().CreateTexture(L"boss_damage_1", L"..\\Resources\\Texture\\bossdamage\\boss_damage_1.png");
	ResourceManager::GetInstance().CreateTexture(L"boss_damage_2", L"..\\Resources\\Texture\\bossdamage\\boss_damage_2.png");
	ResourceManager::GetInstance().CreateTexture(L"boss_damage_3", L"..\\Resources\\Texture\\bossdamage\\boss_damage_3.png");
	ResourceManager::GetInstance().CreateTexture(L"boss_damage_4", L"..\\Resources\\Texture\\bossdamage\\boss_damage_4.png");
	ResourceManager::GetInstance().CreateTexture(L"boss_damage_5", L"..\\Resources\\Texture\\bossdamage\\boss_damage_5.png");
	ResourceManager::GetInstance().CreateTexture(L"boss_damage_6", L"..\\Resources\\Texture\\bossdamage\\boss_damage_6.png");

	ResourceManager::GetInstance().CreateTexture(L"boss_rageline", L"..\\Resources\\Texture\\BossRageTexture\\concentrationline.png");


	// phase3
	ResourceManager::GetInstance().CreateTexture(L"BrushPahse3Boss0", L"..\\Resources\\Texture\\2DPattern\\3phase_brush\\boss\\brushstroke_3phase_boss_0.png");
	ResourceManager::GetInstance().CreateTexture(L"BrushPahse3Boss1", L"..\\Resources\\Texture\\2DPattern\\3phase_brush\\boss\\brushstroke_3phase_boss_1.png");
	ResourceManager::GetInstance().CreateTexture(L"BrushPahse3Boss2", L"..\\Resources\\Texture\\2DPattern\\3phase_brush\\boss\\brushstroke_3phase_boss_2.png");
	ResourceManager::GetInstance().CreateTexture(L"BrushPahse3Boss3", L"..\\Resources\\Texture\\2DPattern\\3phase_brush\\boss\\brushstroke_3phase_boss_3.png");
	ResourceManager::GetInstance().CreateTexture(L"BrushPahse3Boss4", L"..\\Resources\\Texture\\2DPattern\\3phase_brush\\boss\\brushstroke_3phase_boss_4.png");
	ResourceManager::GetInstance().CreateTexture(L"BrushPahse3Boss5", L"..\\Resources\\Texture\\2DPattern\\3phase_brush\\boss\\brushstroke_3phase_boss_5.png");
	ResourceManager::GetInstance().CreateTexture(L"BrushPahse3Boss6", L"..\\Resources\\Texture\\2DPattern\\3phase_brush\\boss\\brushstroke_3phase_boss_6.png");
	ResourceManager::GetInstance().CreateTexture(L"BrushPahse3Player0", L"..\\Resources\\Texture\\2DPattern\\3phase_brush\\mainchar\\brushstroke_3phase_mainchar_0.png");
	ResourceManager::GetInstance().CreateTexture(L"BrushPahse3Player1", L"..\\Resources\\Texture\\2DPattern\\3phase_brush\\mainchar\\brushstroke_3phase_mainchar_1.png");
	ResourceManager::GetInstance().CreateTexture(L"BrushPahse3Player2", L"..\\Resources\\Texture\\2DPattern\\3phase_brush\\mainchar\\brushstroke_3phase_mainchar_2.png");
	ResourceManager::GetInstance().CreateTexture(L"BrushPahse3Player3", L"..\\Resources\\Texture\\2DPattern\\3phase_brush\\mainchar\\brushstroke_3phase_mainchar_3.png");
	ResourceManager::GetInstance().CreateTexture(L"BrushPahse3Player4", L"..\\Resources\\Texture\\2DPattern\\3phase_brush\\mainchar\\brushstroke_3phase_mainchar_4.png");
	ResourceManager::GetInstance().CreateTexture(L"BrushPahse3Player5", L"..\\Resources\\Texture\\2DPattern\\3phase_brush\\mainchar\\brushstroke_3phase_mainchar_5.png");
	ResourceManager::GetInstance().CreateTexture(L"BrushPahse3Player6", L"..\\Resources\\Texture\\2DPattern\\3phase_brush\\mainchar\\brushstroke_3phase_mainchar_6.png");

	ResourceManager::GetInstance().CreateTexture(L"StaffRoll", L"..\\Resources\\Texture\\phase_cutscene\\staffroll.png");
	
	ResourceManager::GetInstance().CreateTexture(L"UIHitHit", L"..\\Resources\\Texture\\UI\\InGameUI\\ui_3phase_hithit.png");

	// Material
#pragma region Sprite Material
	{
		// TODO
		MaterialInfo material;
		material.shader = L"..\\Resources\\Shader\\Sprite3D.hlsli";
		ResourceManager::GetInstance().CreateMaterial(L"Sprite", material);
	}
#pragma endregion

#pragma region UI Material
	{
		// TODO
		MaterialInfo material;
		material.shader = L"..\\Resources\\Shader\\UI.hlsli";
		//material.texture[0] = L".png";
		ResourceManager::GetInstance().CreateMaterial(L"UI", material);
	}
#pragma endregion


#pragma region DirectionalLight Material
	{
		MaterialInfo material;
		material.shader = L"..\\Resources\\Shader\\lightingDirectional.hlsli";
		material.texture[0] = L"D_Position";
		material.texture[1] = L"D_Normal";
		ResourceManager::GetInstance().CreateMaterial(L"DirLight", material);
	}
#pragma endregion

#pragma region Collider Boundary
	{
		MaterialInfo material;
		material.shader = L"..\\Resources\\Shader\\debug.hlsli";
		material.color = ZeldaColor::Blue;
		ResourceManager::GetInstance().CreateMaterial(L"BoundaryDebug", material);
	}
#pragma endregion

#pragma region Line Boundary
	{
		MaterialInfo material;
		material.shader = L"..\\Resources\\Shader\\debug_line.hlsli";
		material.color = ZeldaColor::Blue;
		ResourceManager::GetInstance().CreateMaterial(L"BoundaryDebug_Line", material);
	}
#pragma endregion

#pragma region Deferred Render Material
	{
		MaterialInfo material;
		material.shader = L"..\\Resources\\Shader\\deferred.hlsli";
		ResourceManager::GetInstance().CreateMaterial(L"CubeMaterial", material);
	}
#pragma endregion

#pragma region Forward Render Material
	{
		MaterialInfo material;
		material.shader = L"..\\Resources\\Shader\\forward.hlsli";
		ResourceManager::GetInstance().CreateMaterial(L"CubeMaterial_F", material);
	}
#pragma endregion

#pragma region Player FBX
	{
		ResourceManager::GetInstance().FBXLoad(L"Player", L"..\\Resources\\FBX\\Player\\Player.fbx", L"..\\Resources\\Shader\\player.hlsli");
	}
#pragma endregion

#pragma region BossDoor FBX
	{
		ResourceManager::GetInstance().FBXLoad(L"BossDoor", L"..\\Resources\\FBX\\BossDoor\\BossDoor.fbx", L"..\\Resources\\Shader\\deferred.hlsli");
	}
#pragma endregion

#pragma region BossDoorHand FBX
	{
		ResourceManager::GetInstance().FBXLoad(L"BossDoorHand", L"..\\Resources\\FBX\\BossDoorHand\\BossDoorHand.fbx", L"..\\Resources\\Shader\\deferred_skinned_shadow.hlsli");
	}
#pragma endregion

#pragma region BossFloor FBX
	{
		ResourceManager::GetInstance().FBXLoad(L"BossFloor", L"..\\Resources\\FBX\\BossFloor\\BossFloor.fbx", L"..\\Resources\\Shader\\deferred_shadow.hlsli");
	}
#pragma endregion

#pragma region BossFlag FBX
	{
		ResourceManager::GetInstance().FBXLoad(L"BossFlag", L"..\\Resources\\FBX\\BossFlag\\BossFlag.fbx", L"..\\Resources\\Shader\\BossFlag.hlsli");
	}
#pragma endregion

#pragma region Boss FBX
	{
		ResourceManager::GetInstance().FBXLoad(L"Boss", L"..\\Resources\\FBX\\Boss\\Boss.fbx", L"..\\Resources\\Shader\\boss.hlsli");
	}
#pragma endregion

#pragma region BossObstacle FBX
	{
		ResourceManager::GetInstance().FBXLoad(L"BossObstacle", L"..\\Resources\\FBX\\BossObstacle\\BossObstacle.fbx", L"..\\Resources\\Shader\\deferred_shadow.hlsli");
	}
#pragma endregion

#pragma region BossProjectile FBX
	{
		ResourceManager::GetInstance().FBXLoad(L"BossProjectile", L"..\\Resources\\FBX\\BossProjectile\\BossProjectile.fbx", L"..\\Resources\\Shader\\deferred_shadow.hlsli");
	}
#pragma endregion

#pragma region BossProjectile_Outer FBX
	{
		ResourceManager::GetInstance().FBXLoad(L"BossProjectile_Outer", L"..\\Resources\\FBX\\BossProjectile_Outer\\BossProjectile_Outer.fbx", L"..\\Resources\\Shader\\BossProjectile.hlsli");
	}
#pragma endregion

#pragma region Scroll FBX
	{
		ResourceManager::GetInstance().FBXLoad(L"Scroll", L"..\\Resources\\FBX\\Scroll\\Scroll.fbx", L"..\\Resources\\Shader\\deferred.hlsli");
	}
#pragma endregion

#pragma region VFX_Circle FBX
	{
		ResourceManager::GetInstance().FBXLoad(L"VFX_Circle", L"..\\Resources\\FBX\\VFX_Circle\\VFX_Circle.fbx", L"..\\Resources\\Shader\\TextureAnim.hlsli");
	}
#pragma endregion

#pragma region VFX_Tree FBX
	{
		ResourceManager::GetInstance().FBXLoad(L"VFX_Tree", L"..\\Resources\\FBX\\VFX_Tree\\VFX_Tree.fbx", L"..\\Resources\\Shader\\TextureAnim.hlsli");
	}
#pragma endregion

#pragma region VFX_Protect FBX
	{
		ResourceManager::GetInstance().FBXLoad(L"VFX_Protect", L"..\\Resources\\FBX\\VFX_Protect\\VFX_Protect.fbx", L"..\\Resources\\Shader\\TextureAnim.hlsli");
	}
#pragma endregion

#pragma region VFX_Teleport FBX
	{
		ResourceManager::GetInstance().FBXLoad(L"VFX_Teleport", L"..\\Resources\\FBX\\VFX_Teleport\\VFX_Teleport.fbx", L"..\\Resources\\Shader\\TextureAnim.hlsli");
	}
#pragma endregion

#pragma region VFX_BossNormalAttack_L FBX
	{
		ResourceManager::GetInstance().FBXLoad(L"VFX_BossNormalAttack_L", L"..\\Resources\\FBX\\VFX_BossNormalAttack_L\\VFX_BossNormalAttack_L.fbx", L"..\\Resources\\Shader\\TextureAnim.hlsli");
	}
#pragma endregion

#pragma region VFX_Knockback FBX
	{
		ResourceManager::GetInstance().FBXLoad(L"VFX_Knockback", L"..\\Resources\\FBX\\VFX_Knockback\\VFX_Knockback.fbx", L"..\\Resources\\Shader\\TextureAnim.hlsli");
	}
#pragma endregion

#pragma region VFX_KnockbackGuide FBX
	{
		ResourceManager::GetInstance().FBXLoad(L"VFX_KnockbackGuide", L"..\\Resources\\FBX\\VFX_KnockbackGuide\\VFX_KnockbackGuide.fbx", L"..\\Resources\\Shader\\GuideLine.hlsli");
	}
#pragma endregion

#pragma region VFX_BossNormalAttack_R FBX
	{
		ResourceManager::GetInstance().FBXLoad(L"VFX_BossNormalAttack_R", L"..\\Resources\\FBX\\VFX_BossNormalAttack_R\\VFX_BossNormalAttack_R.fbx", L"..\\Resources\\Shader\\TextureAnim.hlsli");
	}
#pragma endregion

#pragma region VFX_BossNormalAttackGuide FBX
	{
		ResourceManager::GetInstance().FBXLoad(L"VFX_BossNormalAttackGuide", L"..\\Resources\\FBX\\VFX_BossNormalAttackGuide\\VFX_BossNormalAttackGuide.fbx", L"..\\Resources\\Shader\\GuideLine.hlsli");
	}
#pragma endregion

#pragma region VFX_Skill2D FBX
	{
		ResourceManager::GetInstance().FBXLoad(L"VFX_Skill2D", L"..\\Resources\\FBX\\VFX_Skill2D\\VFX_Skill2D.fbx", L"..\\Resources\\Shader\\TextureAnim.hlsli");
	}
#pragma endregion

#pragma region VFX_ChargeAttack FBX
	{
		ResourceManager::GetInstance().FBXLoad(L"VFX_ChargeAttack", L"..\\Resources\\FBX\\VFX_ChargeAttack\\VFX_ChargeAttack.fbx", L"..\\Resources\\Shader\\TextureAnim.hlsli");
	}
#pragma endregion

#pragma region VFX_ChargeAttackGuide FBX
	{
		ResourceManager::GetInstance().FBXLoad(L"VFX_ChargeAttackGuide", L"..\\Resources\\FBX\\VFX_ChargeAttackGuide\\VFX_ChargeAttackGuide.fbx", L"..\\Resources\\Shader\\TextureAnim.hlsli");
	}
#pragma endregion

#pragma region VFX_Blocking FBX
	{
		ResourceManager::GetInstance().FBXLoad(L"VFX_Blocking", L"..\\Resources\\FBX\\VFX_Blocking\\VFX_Blocking.fbx", L"..\\Resources\\Shader\\TextureAnim.hlsli");
	}
#pragma endregion

#pragma region VFX_ChargeSonic FBX
	{
		ResourceManager::GetInstance().FBXLoad(L"VFX_ChargeSonic", L"..\\Resources\\FBX\\VFX_ChargeSonic\\VFX_ChargeSonic.fbx", L"..\\Resources\\Shader\\TextureAnim.hlsli");
	}
#pragma endregion

#pragma region VFX_Explosion FBX
	{
		ResourceManager::GetInstance().FBXLoad(L"VFX_Explosion", L"..\\Resources\\FBX\\VFX_Explosion\\VFX_Explosion.fbx", L"..\\Resources\\Shader\\TextureAnim.hlsli");
	}
#pragma endregion

#pragma region VFX_RangedAttack FBX
	{
		ResourceManager::GetInstance().FBXLoad(L"VFX_RangedAttack", L"..\\Resources\\FBX\\VFX_RangedAttack\\VFX_RangedAttack.fbx", L"..\\Resources\\Shader\\TextureAnim.hlsli");
	}
#pragma endregion

	// 플레이어 VFX
#pragma region VFX_PlayerAttack FBX
	{
		ResourceManager::GetInstance().FBXLoad(L"VFX_PlayerAttack", L"..\\Resources\\FBX\\VFX_PlayerAttack\\VFX_PlayerAttack.fbx", L"..\\Resources\\Shader\\TextureAnim.hlsli");
	}
#pragma endregion

#pragma region VFX_PlayerSpecialAttack FBX
	{
		ResourceManager::GetInstance().FBXLoad(L"VFX_PlayerSpecialAttack", L"..\\Resources\\FBX\\VFX_PlayerSpecialAttack\\VFX_PlayerSpecialAttack.fbx", L"..\\Resources\\Shader\\TextureAnim.hlsli");
	}
#pragma endregion
}