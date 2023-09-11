#include "PlayerCollisionManager.h"
#include "Player.h"
#include "Boss1.h"
#include "IHPObject.h"


void PlayerCollisionManager::Awake()
{

}

void PlayerCollisionManager::SetDamage(float damage)
{
	m_damage = damage;
}

void PlayerCollisionManager::SetBossComponent(Boss1* bossComponent)
{
	m_bossComponent = bossComponent;
}

void PlayerCollisionManager::OnTriggerEnter(Collider* other)
{
	if (other->GetGameObject()->GetLayerID() == 2)
	{
		if (this->GetGameObject()->GetLayerName() == L"BasicAttack")
		{
			m_bossComponent->OnDamaged(m_damage, static_cast<int>(PlayerAttackType::BasicAttack));
		}
		else if (this->GetGameObject()->GetLayerName() == L"RangedAttack")
		{
			m_bossComponent->OnDamaged(m_damage, static_cast<int>(PlayerAttackType::RangedAttack));
		}
	}
}

void PlayerCollisionManager::PreSerialize(json& jsonData) const
{
	PRESERIALIZE_BASE(PlayerCollisionManager);
}

void PlayerCollisionManager::PreDeserialize(const json& jsonData)
{
	PREDESERIALIZE_BASE(PlayerCollisionManager);
}

void PlayerCollisionManager::PostSerialize(json& jsonData) const
{
	POSTSERIALIZE_BASE();
	POSTSERIALIZE_PTR(m_bossComponent);
}

void PlayerCollisionManager::PostDeserialize(const json& jsonData)
{
	POSTDESERIALIZE_BASE();
	POSTDESERIALIZE_PTR(m_bossComponent);
}
