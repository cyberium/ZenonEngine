#pragma once

class Utils
{
public:
    // Convert
    static   std::string ToString(const type_info& type, void* value);

    // Common types
    template <typename T>
    static   T ToType(cstring _string);

    static   bool ToBool(cstring _string);

    // My types
    static  vec2 ToPoint(std::string& _string);
    static  vec3 ToVector3(std::string& _string);

    // Parse
    static   std::string ParseSectionName(std::string& _string);
    static   std::string ParseSectionAndIncludeName(std::string& _string, std::string& _includeSectionName);
    static   void ParseKeyPair(std::string& _string, std::string& key, std::string& val);

    static   bool TryParse(const type_info& type, cstring _string, void* output);

    // String
    static   std::string Trim(std::string& _string, cstring delimiters = " \f\n\r\t\v");
    static   std::string TrimLeft(std::string& _string, cstring delimiters = " \f\n\r\t\v");
    static   std::string TrimRight(std::string& _string, cstring delimiters = " \f\n\r\t\v");

    static   std::string ToLower(cstring _string);
	static   std::string ToUpper(cstring _string);

    static   int popFirstInt(std::string& _string, char separator = 0);
    static   double popFirstDouble(std::string& _string, char separator = 0);
    static   float popFirstFloat(std::string& _string, char separator = 0);

    static   std::string GetFirstSubString(std::string& _string, char separator = 0);
    static   std::string getNextToken(cstring _string, uint32_t& cursor, char separator);
    static   std::string stripCarriageReturn(cstring line);
    static   std::string getLine(std::ifstream& infile);

    static   bool PointInRectangle(const vec2 _point, const vec2 _rectStart, const vec2 _rectEnd);
};

// Common types
template <typename T>
inline T Utils::ToType(cstring _string)
{
    T result;
    if (!(std::stringstream(_string) >> result))
    {
        result = 0;
    }
    return result;
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

inline vec4 fromARGB(uint32 color)
{
    const float a = ((color & 0xFF000000) >> 24) / 255.0f;
    const float r = ((color & 0x00FF0000) >> 16) / 255.0f;
    const float g = ((color & 0x0000FF00) >> 8) / 255.0f;
    const float b = ((color & 0x000000FF)) / 255.0f;
    return vec4(r, g, b, a);
}

inline vec3 fromRGB(uint32 color)
{
    const float r = ((color & 0xFF0000) >> 16) / 255.0f;
    const float g = ((color & 0x00FF00) >> 8) / 255.0f;
    const float b = ((color & 0x0000FF)) / 255.0f;
    return vec3(r, g, b);
}

inline vec4 fromBGRA(uint32 color)
{
    const float b = ((color & 0xFF000000) >> 24) / 255.0f;
    const float g = ((color & 0x00FF0000) >> 16) / 255.0f;
    const float r = ((color & 0x0000FF00) >> 8) / 255.0f;
    const float a = ((color & 0x000000FF)) / 255.0f;
    return vec4(r, g, b, a);
}

inline vec4 fromABGR(uint32 color)
{
    const float a = ((color & 0xFF000000) >> 24) / 255.0f;
    const float b = ((color & 0x00FF0000) >> 16) / 255.0f;
    const float g = ((color & 0x0000FF00) >> 8) / 255.0f;
    const float r = ((color & 0x000000FF)) / 255.0f;
    return vec4(r, g, b, a);
}

static inline void SwitchBool(bool& _value)
{
	_value = !_value;
}


static vec3 rotateV(cvec3 v, float theta)
{
	float cs = cosf(theta);
	float sn = sinf(theta);

	float X = v.x * cs - v.z * sn;
	float Z = v.x * sn + v.z * cs;

	vec3 neW = vec3(X, v.y, Z);

	return neW;
}


inline int iFloor(float x)
{
	return (int)floor(x);
}


inline std::string Utf8_to_cp1251(const char *str)
{
	std::string res;

	int result_u = MultiByteToWideChar(CP_UTF8, 0, str, -1, 0, 0);
	if (!result_u)
	{
		return 0;
	}

	wchar_t* ures = new wchar_t[result_u];
	if (!MultiByteToWideChar(CP_UTF8, 0, str, -1, ures, result_u))
	{
		delete[] ures;
		return 0;
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


inline std::string cp1251_to_utf8(const char *str)
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

inline std::string convertToString(uint32 _bytes)
{
	uint32 p = 0;
	uint32 first = 0;
	uint32 second = 0;
	while (true)
	{
		if ((_bytes / 1024) == 0)
			break;

		first = _bytes / 1024;
		second = _bytes % 1024;

		_bytes = _bytes / 1024;
		p++;
	}

	std::string buf;
	buf += std::to_string(first) + "," + std::to_string(second); // bytes
	switch (p)
	{
	case 0:
		buf += "B";
		break;
	case 1:
		buf += "kB";
		break;
	case 2:
		buf += "MB";
		break;
	case 3:
		buf += "GB";
		break;
	}

	return buf;
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


// Convert a multi-byte character std::string (UTF-8) to a wide (UTF-16) encoded std::string.
inline std::wstring ConvertString(cstring _string)
{
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	return converter.from_bytes(_string);
}

// Converts a wide (UTF-16) encoded std::string into a multi-byte (UTF-8) character std::string.
inline std::string ConvertString(const std::wstring& wstring)
{
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	return converter.to_bytes(wstring);
}

// Gets a std::string resource from the module's resources.
inline std::wstring GetStringResource(int ID, const std::wstring& type)
{
	HMODULE hModule = GetModuleHandle(nullptr);
	HRSRC hResource = FindResource(hModule, MAKEINTRESOURCE(ID), type.c_str());
	if (hResource)
	{
		HGLOBAL hResourceData = LoadResource(hModule, hResource);
		DWORD resourceSize = SizeofResource(hModule, hResource);
		if (hResourceData && resourceSize > 0)
		{
			const wchar_t* resourceData = static_cast<const wchar_t*>(LockResource(hResourceData));
			std::wstring strData(resourceData, resourceSize);
			return strData;
		}
	}
	// Just return an empty std::string.
	return std::wstring();
}