#include "AnimatorTest.h"

void AnimatorTest::Update()
{
	if (ZeldaEngine::Input::GetInstance().PressedKeyNow('T'))
	{
		animator->Play(L"rig|Run");
	}

	if (ZeldaEngine::Input::GetInstance().PressedKeyNow('Y'))
	{
		animator->Play(L"rig|basic");
	}
}

void AnimatorTest::PreSerialize(json& jsonData) const
{

}

void AnimatorTest::PreDeserialize(const json& jsonData)
{

}

void AnimatorTest::PostSerialize(json& jsonData) const
{

}

void AnimatorTest::PostDeserialize(const json& jsonData)
{

}
