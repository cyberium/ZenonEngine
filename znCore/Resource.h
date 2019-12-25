#pragma once

namespace Resources
{
    ZN_API std::string Utf8_to_cp1251(const std::string& AString);
	ZN_API std::string cp1251_to_utf8(const std::string& AString);

	ZN_API std::wstring ConvertString(const std::string& AString);
	ZN_API std::string ConvertString(const std::wstring& WString);

	ZN_API std::wstring GetStringResource(HMODULE HModule, int ID, const std::wstring& type);
}