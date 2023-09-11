#pragma once

#include <string>
#include <codecvt>
#include "nlohmann/json.hpp"

#include "TypeConverter.h"
#include "IOManager.h"

using json = nlohmann::json;

#pragma region Macro function for Component

	#pragma region Serialize
		// Pre, Post
		#define PRESERIALIZE_BASE(componentTypeName) \
		{ \
			jsonData["Component"]["ID"] = IOManager::GetInstance().GetNewID(static_cast<void*>(const_cast<componentTypeName*>(this))); \
			jsonData["Component"]["type"] = #componentTypeName; \
		}

		// Pre, Post
		#define POSTSERIALIZE_BASE() \
		{ \
			jsonData["Component"]["ID_gameObject"] = IOManager::GetInstance().GetID(static_cast<void*>(const_cast<GameObject*>(gameObject))); \
		}

		// Pre
		#define PRESERIALIZE_VALUE(value) \
		{ \
			jsonData["Component"][#value] = value; \
		}

		// Pre
		#define PRESERIALIZE_WSTRVALUE(value) \
		{ \
			jsonData["Component"][#value] = wstring_to_string(value); \
		}

		// Pre, Post
		#define PRESERIALIZE_SERVALUE(value) \
		{ \
			value.PreSerialize(jsonData["Component"][#value]); \
		}

		// Pre, Post
		#define POSTSERIALIZE_SERVALUE(value) \
		{ \
			value.PostSerialize(jsonData["Component"][#value]); \
		}

		// Pre, Post [주의: SERPTR 함수들은 객체를 생성해주지 않음]
		#define PRESERIALIZE_SERPTR(value) \
		{ \
			value->PreSerialize(jsonData["Component"][#value]); \
		}

		// Pre, Post [주의: SERPTR 함수들은 객체를 생성해주지 않음]
		#define POSTSERIALIZE_SERPTR(value) \
		{ \
			value->PostSerialize(jsonData["Component"][#value]); \
		}

		// Post
		#define POSTSERIALIZE_PTR(value) \
		{ \
			jsonData["Component"][std::string("ID_") + #value] = IOManager::GetInstance().GetID(static_cast<void*>(const_cast<decltype(value)>(value))); \
		}

		// Pre
		#define PRESERIALIZE_VECTOR_VALUE(value) \
		{ \
			for (size_t i = 0; i < value.size(); i++) \
			{ \
				jsonData["Component"][#value][std::to_string(i)] = value[i]; \
			} \
			jsonData["Component"][#value]["size"] = value.size(); \
		}

		// Pre, Post
		#define PRESERIALIZE_VECTOR_SERVALUE(value) \
		{ \
			for (size_t i = 0; i < value.size(); i++) \
			{ \
				value[i].PreSerialize(jsonData["Component"][#value][std::to_string(i)]); \
			} \
			jsonData["Component"][#value]["size"] = value.size(); \
		}

		// Pre, Post
		#define POSTSERIALIZE_VECTOR_SERVALUE(value) \
		{ \
			for (size_t i = 0; i < value.size(); i++) \
			{ \
				value[i].PostSerialize(jsonData["Component"][#value][std::to_string(i)]); \
			} \
		}

		// Pre, Post
		#define PRESERIALIZE_VECTOR_PTR(value) \
		{ \
			jsonData["Component"][#value]["size"] = value.size(); \
		}

		// Pre, Post
		#define POSTSERIALIZE_VECTOR_PTR(value) \
		{ \
			for (size_t i = 0; i < value.size(); i++) \
			{ \
				jsonData["Component"][#value][std::to_string(i)]["ID"] = IOManager::GetInstance().GetID(static_cast<void*>(const_cast<decltype(value[i])>(value[i]))); \
			} \
		}

	#pragma endregion

	#pragma region Deserialize
		// Pre, Post
		#define PREDESERIALIZE_BASE(componentTypeName) \
		{ \
			IOManager::GetInstance().SetNewAddress(jsonData["Component"]["ID"], static_cast<void*>(const_cast<componentTypeName*>(this))); \
		}

		// Pre, Post
		#define POSTDESERIALIZE_BASE() \
		{ \
			gameObject = static_cast<GameObject*>(IOManager::GetInstance().GetAddress(jsonData["Component"]["ID_gameObject"])); \
		}

		// Pre
		#define PREDESERIALIZE_VALUE(value) \
		{ \
			value = jsonData["Component"][#value]; \
		}

		// Pre
		#define PREDESERIALIZE_WSTRVALUE(value) \
		{ \
			value = string_to_wstring(jsonData["Component"][#value]); \
		}

		// Pre, Post
		#define PREDESERIALIZE_SERVALUE(value) \
		{ \
			value.PreDeserialize(jsonData["Component"][#value]); \
		}

		// Pre, Post
		#define POSTDESERIALIZE_SERVALUE(value) \
		{ \
			value.PostDeserialize(jsonData["Component"][#value]); \
		}

		// Pre, Post [주의: SERPTR 함수들은 객체를 생성해주지 않음]
		#define PREDESERIALIZE_SERPTR(value) \
		{ \
			value->PreDeserialize(jsonData["Component"][#value]); \
		}

		// Pre, Post [주의: SERPTR 함수들은 객체를 생성해주지 않음]
		#define POSTDESERIALIZE_SERPTR(value) \
		{ \
			value->PostDeserialize(jsonData["Component"][#value]); \
		}

		// Post
		#define POSTDESERIALIZE_PTR(value) \
		{ \
			value = static_cast<decltype(value)>(IOManager::GetInstance().GetAddress(jsonData["Component"][std::string("ID_") + #value])); \
		}

		// Pre
		#define PREDESERIALIZE_VECTOR_VALUE(value) \
		{ \
			value.clear(); \
			size_t valuesize = jsonData["Component"][#value]["size"]; \
			for (size_t i = 0; i < valuesize; i++) \
			{ \
				value.push_back(jsonData["Component"][#value][std::to_string(i)]); \
			} \
		}

		// Pre, Post
		#define PREDESERIALIZE_VECTOR_SERVALUE(value) \
		{ \
			size_t valuesize = jsonData["Component"][#value]["size"]; \
			value.resize(valuesize); \
			for (size_t i = 0; i < valuesize; i++) \
			{ \
				value[i].PreDeserialize(jsonData["Component"][#value][std::to_string(i)]); \
			} \
		}

		// Pre, Post
		#define POSTDESERIALIZE_VECTOR_SERVALUE(value) \
		{ \
			for (size_t i = 0; i < value.size(); i++) \
			{ \
				value[i].PostDeserialize(jsonData["Component"][#value][std::to_string(i)]); \
			} \
		}

		// Pre, Post
		#define PREDESERIALIZE_VECTOR_PTR(value) \
		{ \
			size_t valuesize = jsonData["Component"][#value]["size"]; \
			value.resize(valuesize, nullptr); \
		}

		// Pre, Post
		#define POSTDESERIALIZE_VECTOR_PTR(value) \
		{ \
			for (size_t i = 0; i < value.size(); i++) \
			{ \
				value[i] = static_cast<decltype(&*value[i])>(IOManager::GetInstance().GetAddress(jsonData["Component"][#value][std::to_string(i)]["ID"])); \
			} \
		}

	#pragma endregion

#pragma endregion

namespace ZeldaEngine
{
	class ISerializable
	{
	public:
		virtual json Serialize() const abstract;
		virtual void Deserialize(const json& jsonData) abstract;

	protected:
		virtual void PreSerialize(json& jsonData) const abstract;
		virtual void PreDeserialize(const json& jsonData) abstract;

		virtual void PostSerialize(json& jsonData) const abstract;
		virtual void PostDeserialize(const json& jsonData) abstract;
	};
}