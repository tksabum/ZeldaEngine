#include "pch.h"
#include "TypeConverter.h"

#include <locale>
#include <codecvt>

namespace ZeldaEngine
{
	std::wstring json_to_wstring(const nlohmann::json& jsonValue)
	{
		std::string utf8Str = jsonValue.dump();
		std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
		return converter.from_bytes(utf8Str);
	}

	nlohmann::json wstring_to_json(const std::wstring& wstr)
	{
		std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
		std::string utf8Str = converter.to_bytes(wstr);
		return nlohmann::json::parse(utf8Str);
	}

	std::string wstring_to_string(const std::wstring& wstr)
	{
		std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
		return converter.to_bytes(wstr);
	}

	std::wstring string_to_wstring(const std::string& str)
	{
		std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
		return converter.from_bytes(str);
	}
}