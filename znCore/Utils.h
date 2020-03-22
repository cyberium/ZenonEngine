#pragma once

namespace Utils
{
    ZN_API std::string Trim(std::string& _string, const std::string& delimiters = " \f\n\r\t\v");
	ZN_API std::string TrimLeft(std::string& _string, const std::string& delimiters = " \f\n\r\t\v");
	ZN_API std::string TrimRight(std::string& _string, const std::string& delimiters = " \f\n\r\t\v");

	ZN_API std::string ToLower(const std::string& _string);
	ZN_API std::string ToUpper(const std::string& _string);

	ZN_API std::vector<std::string> ArgumentsToVector(int argumentsCount, char* arguments[]);
};


namespace Resources
{
	ZN_API std::string utf8_to_cp1251(const std::string& AString);
	ZN_API std::string cp1251_to_utf8(const std::string& AString);

	ZN_API std::wstring utf8_to_utf16(const std::string& AString);
	ZN_API std::string utf16_to_utf8(const std::wstring& WString);

	ZN_API std::wstring GetStringResource(HMODULE HModule, int ID, const std::wstring& type);
}

inline bool getBitH2L(uint8* data, uint32 bit)
{
    uint8 mask = 0x80;
    mask >>= (bit % 8);

    return ((data[bit / 8]) & mask) == mask;
}

inline bool getBitL2H(uint8* data, uint32 bit)
{
    uint8 mask = 0x1;
    mask <<= (bit % 8);

    return ((data[bit / 8]) & mask) == mask;
}

template<typename T>
inline void SafeDelete(T& ptr)
{
	if (ptr != NULL)
	{
		delete ptr;
		ptr = NULL;
	}
}

template<typename T>
inline void SafeDeleteArray(T& ptr)
{
	if (ptr != NULL)
	{
		delete[] ptr;
		ptr = NULL;
	}
}
