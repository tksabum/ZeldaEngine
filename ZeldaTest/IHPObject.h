#pragma once

// �÷��̾�� ���޵� �������� enum
enum class BossAttackType
{
	NormalAttack,
	PushAttack,
	BarrierAttack
};

enum class PlayerAttackType
{
	BasicAttack,
	RangedAttack,
	DubugAttack
};

class IHPObject abstract
{
public:
	virtual void OnDamaged(float damage, int type) abstract;
};

