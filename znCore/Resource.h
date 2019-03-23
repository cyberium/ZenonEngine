#pragma once

namespace Resources
{
    std::string Utf8_to_cp1251(const char *str);
    std::string cp1251_to_utf8(const char *str);
    std::wstring ConvertString(const std::string& _string);
    std::string ConvertString(const std::wstring& wstring);
    std::wstring GetStringResource(HMODULE HModule, int ID, const std::wstring& type);

}