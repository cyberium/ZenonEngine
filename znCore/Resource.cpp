#include "stdafx.h"

// General
#include "Resource.h"

// Convert a multi-byte character std::string (UTF-8) to a multi-byte character std::string (CP1251)
std::string Resources::Utf8_to_cp1251(const char *str)
{
    std::string res;

    int result_u = MultiByteToWideChar(CP_UTF8, 0, str, -1, 0, 0);
    if (!result_u)
    {
        return 0;
    }

    wchar_t* ures = new wchar_t[result_u];
    if (! MultiByteToWideChar(CP_UTF8, 0, str, -1, ures, result_u))
    {
        delete[] ures;
        return nullptr;
    }

    int result_c = WideCharToMultiByte(1251, 0, ures, -1, 0, 0, 0, 0);
    if (!result_c)
    {
        delete[] ures;
        return 0;
    }

    char *cres = new char[result_c];
    if (!WideCharToMultiByte(1251, 0, ures, -1, cres, result_c, 0, 0))
    {
        delete[] cres;
        return 0;
    }

    delete[] ures;
    res.append(cres);
    delete[] cres;
    return res;
}

// Convert a multi-byte character std::string (CP1251) to a multi-byte character std::string (UTF-8)
std::string Resources::cp1251_to_utf8(const char *str)
{
    std::string res;

    int result_u = MultiByteToWideChar(1251, 0, str, -1, 0, 0);
    if (!result_u)
    {
        return 0;
    }

    wchar_t *ures = new wchar_t[result_u];
    if (!MultiByteToWideChar(1251, 0, str, -1, ures, result_u))
    {
        delete[] ures;
        return 0;
    }

    int result_c = WideCharToMultiByte(CP_UTF8, 0, ures, -1, 0, 0, 0, 0);
    if (!result_c)
    {
        delete[] ures;
        return 0;
    }

    char* cres = new char[result_c];
    if (!WideCharToMultiByte(CP_UTF8, 0, ures, -1, cres, result_c, 0, 0))
    {
        delete[] cres;
        return 0;
    }

    delete[] ures;
    res.append(cres);
    delete[] cres;

    return res;
}

// Convert a multi-byte character std::string (UTF-8) to a wide (UTF-16) encoded std::string.
std::wstring Resources::ConvertString(const std::string& _string)
{
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    return converter.from_bytes(_string);
}

// Converts a wide (UTF-16) encoded std::string into a multi-byte (UTF-8) character std::string.
std::string Resources::ConvertString(const std::wstring& wstring)
{
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    return converter.to_bytes(wstring);
}

// Gets a std::string resource from the module's resources.
std::wstring Resources::GetStringResource(HMODULE HModule, int ID, const std::wstring& type)
{
    HRSRC hResource = FindResource(HModule, MAKEINTRESOURCE(ID), type.c_str());
    if (hResource)
    {
        HGLOBAL hResourceData = LoadResource(HModule, hResource);
        DWORD resourceSize = SizeofResource(HModule, hResource);
        if (hResourceData && resourceSize > 0)
        {
            const wchar_t* resourceData = static_cast<const wchar_t*>(LockResource(hResourceData));
            std::wstring strData(resourceData, resourceSize);
            return strData;
        }
    }

    return std::wstring();
}