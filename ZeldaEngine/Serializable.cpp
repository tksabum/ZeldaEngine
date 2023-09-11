#include "pch.h"
#include "Serializable.h"

json ZeldaEngine::Serializable::Serialize() const
{
	json jsonData;

	IOManager::GetInstance().Start();

	PreSerialize(jsonData);
	PostSerialize(jsonData);

	IOManager::GetInstance().End();

	return jsonData;
}

void ZeldaEngine::Serializable::Deserialize(const json& jsonData)
{
	IOManager::GetInstance().Start();

	PreDeserialize(jsonData);
	PostDeserialize(jsonData);

	IOManager::GetInstance().End();
}