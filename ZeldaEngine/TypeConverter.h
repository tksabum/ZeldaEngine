#pragma once

#include "ZeldaEngineAPI.h"

#include <string>
#include "nlohmann/json.hpp"

namespace ZeldaEngine
{
	ZELDAENGINE_API std::wstring json_to_wstring(const nlohmann::json& jsonValue);
	ZELDAENGINE_API nlohmann::json wstring_to_json(const std::wstring& wstr);
	ZELDAENGINE_API std::string wstring_to_string(const std::wstring& wstr);
	ZELDAENGINE_API std::wstring string_to_wstring(const std::string& str);
}