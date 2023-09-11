#include "StandoffCoolTimeUI.h"

#include "../Player.h"
#include "BaseUI.h"
#include "MovableUI.h"
#include "CoverUI.h"
#include "NumUI.h"

void StandoffCoolTimeUI::Awake()
{
	if (moveCover)
	{
		moveCoverTransform = moveCover->GetGameObject()->GetTransform();
	}
}

void StandoffCoolTimeUI::Start()
{
	if (target)
	{
		auto player = target->GetComponent<Player>();
		if (player)
		{
			maxCoolTime = player->GetSpecialAttackCoolTime();
		}
	}

	if (moveCoverTransform)
	{
		moveCoverTransform->GetGameObject()->SetActive(true);
		moveCoverTransform->SetLocalScale(Vector3D::zero);
	}
}

void StandoffCoolTimeUI::Update()
{
	if (target) 
	{
		auto player = target->GetComponent<Player>();
		if (player) 
		{
			float time = player->GetSpecialAttackChargingTime();

			switch (countType)
			{
				case CoolTimeUI::CountType::NUM:
				{
					if (maxCoolTime - time == 0)
					{
						numUI->SetTexture(L"None");
						moveCover->SetTexture(L"None");
					}
					else
					{
						moveCover->SetTexture(L"CoolTime_DisableCover");

						if ((int)((maxCoolTime - time) * 10) > 0 && (int)((maxCoolTime - time) * 10) <= 10)
						{
							numUI->SetTexture(L"Num_1");
						}
						else if ((int)((maxCoolTime - time) * 10) > 10 && (int)((maxCoolTime - time) * 10) <= 20)
						{
							numUI->SetTexture(L"Num_2");
						}
						else if ((int)((maxCoolTime - time) * 10) > 20 && (int)((maxCoolTime - time) * 10) <= 30)
						{
							numUI->SetTexture(L"Num_3");
						}
						else if ((int)((maxCoolTime - time) * 10) > 30 && (int)((maxCoolTime - time) * 10) <= 40)
						{
							numUI->SetTexture(L"Num_4");
						}
						else if ((int)((maxCoolTime - time) * 10) > 40 && (int)((maxCoolTime - time) * 10) <= 50)
						{
							numUI->SetTexture(L"Num_5");
						}
						else if ((int)((maxCoolTime - time) * 10) > 50 && (int)((maxCoolTime - time) * 10) <= 60)
						{
							numUI->SetTexture(L"Num_6");
						}
						else if ((int)((maxCoolTime - time) * 10) > 60 && (int)((maxCoolTime - time) * 10) <= 70)
						{
							numUI->SetTexture(L"Num_7");
						}
						else if ((int)((maxCoolTime - time) * 10) > 70 && (int)((maxCoolTime - time) * 10) <= 80)
						{
							numUI->SetTexture(L"Num_8");
						}
						else if ((int)((maxCoolTime - time) * 10) > 80 && (int)((maxCoolTime - time) * 10) <= 90)
						{
							numUI->SetTexture(L"Num_9");
						}
					}
				}
				break;
				case CoolTimeUI::CountType::SLIDE:
				{
					if (moveCoverTransform)
					{
						moveCoverTransform->SetLocalScale(Vector3D(UISize.x, UISize.y - time / maxCoolTime * UISize.y, 0));
						moveCoverTransform->SetLocalPosition(Vector3D(0, -moveCoverTransform->GetLocalScale().y / 2 + UISize.y / 2, 0));
					}
				}
				break;
				default:
					break;
			}
		}
	}
}
	
void StandoffCoolTimeUI::PreSerialize(json& jsonData) const
{
	PRESERIALIZE_BASE(StandoffCoolTimeUI);

	PRESERIALIZE_SERVALUE(UISize);
	PRESERIALIZE_VALUE(countType);
}

void StandoffCoolTimeUI::PreDeserialize(const json& jsonData)
{
	PREDESERIALIZE_BASE(StandoffCoolTimeUI);

	PREDESERIALIZE_SERVALUE(UISize);
	PREDESERIALIZE_VALUE(countType);
}

void StandoffCoolTimeUI::PostSerialize(json& jsonData) const
{
	POSTSERIALIZE_BASE();

	POSTSERIALIZE_PTR(target);
	POSTSERIALIZE_PTR(base);
	POSTSERIALIZE_PTR(moveCover);
	POSTSERIALIZE_PTR(numUI);
	POSTSERIALIZE_PTR(cover);
	POSTSERIALIZE_PTR(moveCoverTransform);
}

void StandoffCoolTimeUI::PostDeserialize(const json& jsonData)
{
	POSTDESERIALIZE_BASE();

	POSTDESERIALIZE_PTR(target);
	POSTDESERIALIZE_PTR(base);
	POSTDESERIALIZE_PTR(moveCover);
	POSTDESERIALIZE_PTR(numUI);
	POSTDESERIALIZE_PTR(cover);
	POSTDESERIALIZE_PTR(moveCoverTransform);
}
