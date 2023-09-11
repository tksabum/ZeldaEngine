#include "RailTriggerPoint.h"

#include "../Boss1.h"
#include "../Player.h"

#include "../EffectPlayer.h"

#include <cmath>

void RailTriggerPoint::Awake()
{
	sceneName = SceneManager::GetInstance().GetName();

	if (!cam)
	{
		cam = SceneManager::GetInstance().GetMainCamera()->GetGameObject()->GetTransform();
	}

	if (sceneName == L"Directing Scene 0")
	{
		effectObj = CreateGameObjectFromFBX(L"VFX_Teleport");
		teleportEffectPlayer = effectObj->AddComponent<EffectPlayer>();
		teleportEffectPlayer->SetMeshRenderer(effectObj->GetTransform()->GetChild(0)->GetGameObject()->GetComponent<MeshRenderer>());
		teleportEffectPlayer->SetMaxValue(0.0f);
		teleportEffectPlayer->SetPlaySpeed(0.2f);
		teleportEffectPlayer->SetRemainTime(10.0f);
		teleportEffectPlayer->SetDirection(Vector2D(1, 0));
		effectObj->SetActive(false);
	}
}

void RailTriggerPoint::OnTriggerEnter()
{
	if (sceneName == L"Directing Scene 0")
	{
		switch (index)
		{
			case 0:
			{
				if (boss)
				{
					// 텔레포트 시작
					boss->SetActive(true);
					effectObj->GetTransform()->SetLocalPosition(Vector3D(0, 0, 0));

					// 1. 포탈이 나온다
					effectObj->SetActive(true);
					teleportEffectPlayer->SetMaxValue(0.5f);
					effectPlay = true;
				}
			}
			break;
			case 1:
			{
				if (boss)
				{
					boss->GetComponent<Animator>()->Play(L"root|PhaseTransition");
				}
			}
			break;
			case 2:
			{
				if (player)
				{
					SoundManager::GetInstance().PlaySFX(SoundManager::Player::BasicAttack);
					player->GetComponent<Animator>()->Play(L"Armature|attack");
				}
			}
			break;
			case 3:
			{
				if (boss)
				{
					SoundManager::GetInstance().PlaySFX(SoundManager::Boss::Telepo);
				}
			}
			break;
			case 4:
			{
				if (boss)
				{
					SoundManager::GetInstance().PlaySFX(SoundManager::Boss::Roar);
					GetGameObject()->SetActive(false);
				}
			}
			break;
			case 7:
			{
				LoadScene(L"GameScene");
			}
			break;
		}
	}
	else if (sceneName == L"Directing Scene 1")
	{
		switch (index)
		{
			case 0:
			{
				dynamic_cam = true;
			}
			break;
			case 1:
			{
				count++;
			}
			break;
			case 7:
			{
				LoadScene(L"GameSceneBossPhase3");
			}
			break;
			default:
				break;
		}
	}
	else if (sceneName == L"Directing Scene 2")
	{
		switch (index)
		{
			case 7:
			{
				LoadScene(L"GameSceneBossPhase3");
			}
			break;
			default:
				break;
		}
	}
}

void RailTriggerPoint::Update()
{
	if (sceneName == L"Directing Scene 0")
	{
		switch (index)
		{
			case 0:
			{
				if (boss->GetRootActive())
				{
					float speed = TimeController::GetInstance().GetDeltaTime() * 3;
					auto beforePos = boss->GetTransform()->GetLocalPosition();
					beforePos.y += speed;

					if (effectPlay && beforePos.y >= 2.4f)
					{
						teleportEffectPlayer->SetMaxValue(0.0f);
						teleportEffectPlayer->InverseDirection();
						effectPlay = false;
					}

					if (beforePos.y <= 4.2f)
					{
						boss->GetTransform()->SetLocalPosition(beforePos);
					}
				}
			}
		}
	}
	else if (sceneName == L"Directing Scene 2")
	{
		float speed = TimeController::GetInstance().GetDeltaTime();

		auto bossPos = boss->GetTransform()->GetLocalPosition();
		bossPos += boss->GetTransform()->GetFront() * speed * 3;
		boss->GetTransform()->SetLocalPosition(bossPos);

		auto playerPos = player->GetTransform()->GetLocalPosition();
		playerPos += player->GetTransform()->GetFront() * speed * 3;
		player->GetTransform()->SetLocalPosition(playerPos);
	}
}

void RailTriggerPoint::SetIndex(int index)
{
	this->index = index;
}

int RailTriggerPoint::GetIndex() const
{
	return index;
}

void RailTriggerPoint::PreSerialize(json& jsonData) const
{
	PRESERIALIZE_BASE(RailTriggerPoint);

	PRESERIALIZE_VALUE(index);
}

void RailTriggerPoint::PreDeserialize(const json& jsonData)
{
	PREDESERIALIZE_BASE(RailTriggerPoint);

	PREDESERIALIZE_VALUE(index);
}

void RailTriggerPoint::PostSerialize(json& jsonData) const
{
	POSTSERIALIZE_BASE();

	POSTSERIALIZE_PTR(boss);
	POSTSERIALIZE_PTR(player);
	POSTSERIALIZE_PTR(collider);
}

void RailTriggerPoint::PostDeserialize(const json& jsonData)
{
	POSTDESERIALIZE_BASE();

	POSTDESERIALIZE_PTR(boss);
	POSTDESERIALIZE_PTR(player);
	POSTDESERIALIZE_PTR(collider);
}
