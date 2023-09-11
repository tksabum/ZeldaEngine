#include "TriggerEventSetter.h"

TriggerEventSetter::TriggerEventSetter()
{
	collisionEnterScene = L"";
	triggerEnterScene = L"";
}

void TriggerEventSetter::OnCollisionEnter()
{
	if (collisionEnterScene != L"")
	{
		LoadScene(collisionEnterScene);
	}
}

void TriggerEventSetter::OnTriggerEnter()
{
	if (triggerEnterScene != L"")
	{
		LoadScene(triggerEnterScene);
	}
}

void TriggerEventSetter::SetTriggerEnterEventLoadScene(const std::wstring& sceneName)
{
	triggerEnterScene = sceneName;
}

void TriggerEventSetter::SetCollisionEnterEventLoadScene(const std::wstring& sceneName)
{
	collisionEnterScene = sceneName;
}

void TriggerEventSetter::PreSerialize(json& jsonData) const
{
	PRESERIALIZE_BASE(TriggerEventSetter);
	PRESERIALIZE_WSTRVALUE(triggerEnterScene);
	PRESERIALIZE_WSTRVALUE(collisionEnterScene);
}

void TriggerEventSetter::PreDeserialize(const json& jsonData)
{
	PREDESERIALIZE_BASE(TriggerEventSetter);
	PREDESERIALIZE_WSTRVALUE(triggerEnterScene);
	PREDESERIALIZE_WSTRVALUE(collisionEnterScene);
}

void TriggerEventSetter::PostSerialize(json& jsonData) const
{
	POSTSERIALIZE_BASE();
}

void TriggerEventSetter::PostDeserialize(const json& jsonData)
{
	POSTDESERIALIZE_BASE();
}
