#pragma once

class Utils
{
public:
    // Convert
    static   std::string ToString(const type_info& type, void* value);

    // Common types
    template <typename T>
    static   T ToType(const std::string& _string);

    static   bool ToBool(const std::string& _string);

    // My types
    static  vec2 ToPoint(std::string& _string);
    static  vec3 ToVector3(std::string& _string);

    // Parse
    static   std::string ParseSectionName(std::string& _string);
    static   std::string ParseSectionAndIncludeName(std::string& _string, std::string& _includeSectionName);
    static   void ParseKeyPair(std::string& _string, std::string& key, std::string& val);

    static   bool TryParse(const type_info& type, const std::string& _string, void* output);

    // String
    static   std::string Trim(std::string& _string, const std::string& delimiters = " \f\n\r\t\v");
    static   std::string TrimLeft(std::string& _string, const std::string& delimiters = " \f\n\r\t\v");
    static   std::string TrimRight(std::string& _string, const std::string& delimiters = " \f\n\r\t\v");

    static   std::string ToLower(const std::string& _string);
	static   std::string ToUpper(const std::string& _string);

    static   int popFirstInt(std::string& _string, char separator = 0);
    static   double popFirstDouble(std::string& _string, char separator = 0);
    static   float popFirstFloat(std::string& _string, char separator = 0);

    static   std::string GetFirstSubString(std::string& _string, char separator = 0);
    static   std::string getNextToken(const std::string& _string, uint32_t& cursor, char separator);
    static   std::string stripCarriageReturn(const std::string& line);
    static   std::string getLine(std::ifstream& infile);

    static   bool PointInRectangle(const vec2 _point, const vec2 _rectStart, const vec2 _rectEnd);
};

// Common types
template <typename T>
inline T Utils::ToType(const std::string& _string)
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
