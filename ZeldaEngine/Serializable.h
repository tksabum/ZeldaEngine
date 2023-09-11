#pragma once

#include "ZeldaEngineAPI.h"

#include "ISerializable.h"

namespace ZeldaEngine
{
	class ZELDAENGINE_API Serializable : public ISerializable
	{
	public:
		virtual json Serialize() const override final;
		virtual void Deserialize(const json& jsonData) override final;

		virtual void PreSerialize(json& jsonData) const abstract;
		virtual void PreDeserialize(const json& jsonData) abstract;
		virtual void PostSerialize(json& jsonData) const abstract;
		virtual void PostDeserialize(const json& jsonData) abstract;
	};
}