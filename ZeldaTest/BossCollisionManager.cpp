#include "BossCollisionManager.h"
#include "Player.h"
#include "Boss1.h"
#include "IHPObject.h"

using namespace ZeldaEngine;

BossCollisionManager::BossCollisionManager()
{
}

void BossCollisionManager::Awake()
{
}

void BossCollisionManager::OnTriggerEnter(ZeldaEngine::Collider* other)
{

	if (other->GetGameObject()->GetLayerID() == 1)
	{
		// 밀격 아닐 때
		if (this->GetGameObject()->GetLayerName() == L"NormalAttackObject")
		{
			if (this->GetGameObject()->GetName() == L"BossProjectile")
			{
				m_playerComponent->OnDamaged(m_damage, int(BossAttackType::NormalAttack));
				m_bossComponent->Pattern5Hit();
			}
		
	
			else if (this->GetGameObject()->GetName() == L"branch1Object" &&
				(m_playerComponent->GetGameObject()->GetTransform()->GetLocalPosition() - m_bossComponent->GetGameObject()->GetTransform()->GetLocalPosition()).Size() > 4)
			{
				m_playerComponent->OnDamaged(m_damage, int(BossAttackType::NormalAttack));
			}

			else if (this->GetGameObject()->GetName() == L"branch2Object" &&
				(m_playerComponent->GetGameObject()->GetTransform()->GetLocalPosition() - m_bossComponent->GetGameObject()->GetTransform()->GetLocalPosition()).Size() > 4)
			{
				m_playerComponent->OnDamaged(m_damage, int(BossAttackType::NormalAttack));
			}

			else if (this->GetGameObject()->GetName() == L"branch2Object" &&
				(m_playerComponent->GetGameObject()->GetTransform()->GetLocalPosition() - m_bossComponent->GetGameObject()->GetTransform()->GetLocalPosition()).Size() <= 4)
			{
			}

			else if (this->GetGameObject()->GetName() == L"branch1Object" &&
				(m_playerComponent->GetGameObject()->GetTransform()->GetLocalPosition() - m_bossComponent->GetGameObject()->GetTransform()->GetLocalPosition()).Size() <= 4)
			{
			}

			else
			{
				m_playerComponent->OnDamaged(m_damage, int(BossAttackType::NormalAttack));
			}

		}

		// 밀격일 때
		else if (this->GetGameObject()->GetLayerName() == L"PushAttackObject")
		{
			m_playerComponent->OnDamaged(m_damage, int(BossAttackType::PushAttack));
		}
	}

	else if (other->GetGameObject()->GetLayerID() == 5 || other->GetGameObject()->GetLayerID() == 6)
	{
		// 무적 패턴일 때
		if (this->GetGameObject()->GetName() == L"barrierTriggerObject")
		{
			m_playerComponent->OnDamaged(0, int(BossAttackType::BarrierAttack));
		}
	}
}

void BossCollisionManager::OnTriggerStay(ZeldaEngine::Collider* other)
{
	if (other->GetGameObject()->GetLayerID() == 1)
	{
		// 밀격 아닐 때
		if (this->GetGameObject()->GetLayerName() == L"NormalAttackObject")
		{
			if (this->GetGameObject()->GetName() == L"BossProjectile")
			{
				m_playerComponent->OnDamaged(m_damage, int(BossAttackType::NormalAttack));
				m_bossComponent->Pattern5Hit();
			}

			if (!isBranchMoveStart)
			{
				if (this->GetGameObject()->GetName() == L"branch1Object" &&
					(m_playerComponent->GetGameObject()->GetTransform()->GetLocalPosition() - m_bossComponent->GetGameObject()->GetTransform()->GetLocalPosition()).Size() > 4)
				{
					m_playerComponent->OnDamaged(m_damage, int(BossAttackType::NormalAttack));
				}

				else if (this->GetGameObject()->GetName() == L"branch2Object" &&
					(m_playerComponent->GetGameObject()->GetTransform()->GetLocalPosition() - m_bossComponent->GetGameObject()->GetTransform()->GetLocalPosition()).Size() > 4)
				{
					m_playerComponent->OnDamaged(m_damage, int(BossAttackType::NormalAttack));
				}

				else if (this->GetGameObject()->GetName() == L"branch2Object" &&
					(m_playerComponent->GetGameObject()->GetTransform()->GetLocalPosition() - m_bossComponent->GetGameObject()->GetTransform()->GetLocalPosition()).Size() <= 4)
				{
				}

				else if (this->GetGameObject()->GetName() == L"branch1Object" &&
					(m_playerComponent->GetGameObject()->GetTransform()->GetLocalPosition() - m_bossComponent->GetGameObject()->GetTransform()->GetLocalPosition()).Size() <= 4)
				{
				}
			}

			else
			{
				m_playerComponent->OnDamaged(m_damage, int(BossAttackType::NormalAttack));
			}

		}

		// 밀격일 때
		else if (this->GetGameObject()->GetLayerName() == L"PushAttackObject")
		{
			m_playerComponent->OnDamaged(m_damage, int(BossAttackType::PushAttack));
		}
	}
}

void BossCollisionManager::SetPlayerComponent(Player* playerComponent)
{
	m_playerComponent = playerComponent;
}

void BossCollisionManager::SetBossComponent(Boss1* bossComponent)
{
	m_bossComponent = bossComponent;
}

void BossCollisionManager::SetPatternDamage(double damage)
{
	m_damage = damage;
}

void BossCollisionManager::DetectFanRange()
{
	m_playerComponent->OnDamaged(m_damage, int(BossAttackType::PushAttack));
}

void BossCollisionManager::IsBranchMoveStart(bool isStart)
{
	isBranchMoveStart = isStart;
}  

void BossCollisionManager::PreSerialize(json& jsonData) const
{
	PRESERIALIZE_BASE(BossCollisionManager);
}

void BossCollisionManager::PreDeserialize(const json& jsonData)
{
	PREDESERIALIZE_BASE(BossCollisionManager);

}

void BossCollisionManager::PostSerialize(json& jsonData) const
{
	POSTSERIALIZE_BASE();
}

void BossCollisionManager::PostDeserialize(const json& jsonData)
{
	POSTDESERIALIZE_BASE();
}

