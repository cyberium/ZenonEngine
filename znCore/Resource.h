#pragma once

namespace Resources
{
    std::string Utf8_to_cp1251(const std::string& AString);
    std::string cp1251_to_utf8(const std::string& AString);

    std::wstring ConvertString(const std::string& AString);
    std::string ConvertString(const std::wstring& WString);

    std::wstring GetStringResource(HMODULE HModule, int ID, const std::wstring& type);

}