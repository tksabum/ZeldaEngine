#include "CoolTimeUI.h"

#include "../Player.h"
#include "BaseUI.h"
#include "MovableUI.h"
#include "CoverUI.h"
#include "NumUI.h"

void CoolTimeUI::SetTarget(const GameObject& target)
{
	this->target = const_cast<GameObject*>(&target);
}

void CoolTimeUI::SetBaseUI(const BaseUI& base)
{
	this->base = const_cast<BaseUI*>(&base);
}

void CoolTimeUI::SetMovableUI(const MovableUI& moveCover)
{
	this->moveCover = const_cast<MovableUI*>(&moveCover);
}

void CoolTimeUI::SetCoverUI(const CoverUI& cover)
{
	this->cover = const_cast<CoverUI*>(&cover);
}

void CoolTimeUI::SetNumUI(const NumUI& numUI)
{
	this->numUI = const_cast<NumUI*>(&numUI);
}

void CoolTimeUI::SetUISize(const Vector2D& size)
{
	UISize = size;
}

void CoolTimeUI::SetType(const CountType& type)
{
	this->countType = type;
}

