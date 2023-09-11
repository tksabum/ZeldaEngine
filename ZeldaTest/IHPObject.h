#pragma once

// 플레이어에게 전달될 공격종류 enum
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

