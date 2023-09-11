#include "Player2DCollisionManager.h"
#include "Player.h"
#include "IHPObject.h"

Player2DCollisionManager::Player2DCollisionManager()
{
	normal2DDamage = 10.0f;
	ultimate2DDamage = 10.0f;
}

void Player2DCollisionManager::Awake()
{

}

void Player2DCollisionManager::OnCollisionEnter2D(Collider2D* other)
{
	if(other->GetGameObject()->GetName() == L"normal2DCollisionObject" 
		|| other->GetGameObject()->GetName() == L"ultimateCollisionObject")
		m_playerComponent->OnDamaged(normal2DDamage, int(BossAttackType::NormalAttack));

	else if (other->GetGameObject()->GetName() == L"ultimateCollisionBottomObject")
	{
		m_playerComponent->OnDamaged(ultimate2DDamage, int(BossAttackType::NormalAttack));
	}
	else if (other->GetGameObject()->GetName() == L"ultimateCollisionTopObject")
	{
		m_playerComponent->OnDamaged(ultimate2DDamage, int(BossAttackType::NormalAttack));
	}
	else if (other->GetGameObject()->GetName() == L"ultimateCollisionLeftObject")
	{
		m_playerComponent->OnDamaged(ultimate2DDamage, int(BossAttackType::NormalAttack));
	}
	else if (other->GetGameObject()->GetName() == L"ultimateCollisionRightObject")
	{
		m_playerComponent->OnDamaged(ultimate2DDamage, int(BossAttackType::NormalAttack));
	}
}


void Player2DCollisionManager::SetPlayerComponent(Player* playerComponent)
{
	m_playerComponent = playerComponent;
}


void Player2DCollisionManager::PreSerialize(json& jsonData) const
{
	PRESERIALIZE_BASE(Player2DCollisionManager);
	PRESERIALIZE_VALUE(normal2DDamage);
	PRESERIALIZE_VALUE(ultimate2DDamage);
}

void Player2DCollisionManager::PreDeserialize(const json& jsonData)
{
	PREDESERIALIZE_BASE(Player2DCollisionManager);
	PREDESERIALIZE_VALUE(normal2DDamage)
	PREDESERIALIZE_VALUE(ultimate2DDamage)
}

void Player2DCollisionManager::PostSerialize(json& jsonData) const
{
	POSTSERIALIZE_BASE();
	POSTSERIALIZE_PTR(m_playerComponent);
}

void Player2DCollisionManager::PostDeserialize(const json& jsonData)
{
	POSTDESERIALIZE_BASE();
	POSTDESERIALIZE_PTR(m_playerComponent);
}
