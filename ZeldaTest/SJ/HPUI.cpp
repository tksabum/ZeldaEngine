#include "HPUI.h"

#include "../Player.h"
#include "BaseUI.h"
#include "MovableUI.h"
#include "CoverUI.h"

void HPUI::Awake()
{
	if (moveBar)
	{
		moveBarTransform = moveBar->GetGameObject()->GetTransform();
	}
}

void HPUI::Start()
{
	if (player)
	{
		maxHP = player->GetPlayerMaxHp();
	}

	if (moveBarTransform)
	{
		moveBarTransform->GetGameObject()->SetActive(true);
		moveBarTransform->SetLocalScale(Vector3D::zero);
	}
}

void HPUI::Update()
{
	if (moveBarTransform)
	{
		moveBarTransform->SetLocalScale(Vector3D(player->GetPlayerHp() / maxHP * UISize.x, UISize.y, 0));
		moveBarTransform->SetLocalPosition(Vector3D(padding + moveBarTransform->GetLocalScale().x / 2 - UISize.x / 2, 0, 0));
	}
}

void HPUI::SetPlayer(const Player& player)
{
	this->player = const_cast<Player*>(&player);
}

void HPUI::SetBaseUI(const BaseUI& base)
{
	this->base = const_cast<BaseUI*>(&base);
}

void HPUI::SetMovableUI(const MovableUI& moveBar)
{
	this->moveBar = const_cast<MovableUI*>(&moveBar);
}

void HPUI::SetCoverUI(const CoverUI& cover)
{
	this->cover = const_cast<CoverUI*>(&cover);
}

void HPUI::SetUISize(const Vector2D& size)
{
	UISize = size;
}

void HPUI::PreSerialize(json& jsonData) const
{
	PRESERIALIZE_BASE(HPUI);

	PRESERIALIZE_SERVALUE(UISize);
}

void HPUI::PreDeserialize(const json& jsonData)
{
	PREDESERIALIZE_BASE(HPUI);

	PREDESERIALIZE_SERVALUE(UISize);
}

void HPUI::PostSerialize(json& jsonData) const
{
	POSTSERIALIZE_BASE();

	POSTSERIALIZE_PTR(player);
	POSTSERIALIZE_PTR(base);
	POSTSERIALIZE_PTR(moveBar);
	POSTSERIALIZE_PTR(cover);
	POSTSERIALIZE_PTR(moveBarTransform);
}

void HPUI::PostDeserialize(const json& jsonData)
{
	POSTDESERIALIZE_BASE();

	POSTDESERIALIZE_PTR(player);
	POSTDESERIALIZE_PTR(base);
	POSTDESERIALIZE_PTR(moveBar);
	POSTDESERIALIZE_PTR(cover);
	POSTDESERIALIZE_PTR(moveBarTransform);
}
