#pragma once

#include <inttypes.h>

const size_t cBufferSize = 64;

namespace
{
	template <typename T>
	inline std::string ValueToStringInternal(const char* Pattern, T Value)
	{
		char buff[cBufferSize];
		if (sprintf_s(buff, cBufferSize, (std::string("%") + Pattern).c_str(), Value) <= 0)
			throw CException("ValueToStringInternal: Unable to write '%s' value.", typeid(T).name());
		return buff;
	}
}

template <typename T>
inline std::string ValueToString(const T& Value)
{
	throw CException("ValueToString for type '%s' is not specialized.", typeid(T).name());
}

template<> 
inline std::string ValueToString(const std::string& Value) 
{ 
	return Value; 
}

template<> inline std::string ValueToString(const int8& Value)   { return ValueToStringInternal<int16> (PRId8,  Value); }
template<> inline std::string ValueToString(const uint8& Value)  { return ValueToStringInternal<uint16>(PRIu8,  Value); }
template<> inline std::string ValueToString(const int16& Value)  { return ValueToStringInternal<int16> (PRId16, Value); }
template<> inline std::string ValueToString(const uint16& Value) { return ValueToStringInternal<uint16>(PRIu16, Value); }
template<> inline std::string ValueToString(const int32& Value)  { return ValueToStringInternal<int32> (PRId32, Value); }
template<> inline std::string ValueToString(const uint32& Value) { return ValueToStringInternal<uint32>(PRIu32, Value); }
template<> inline std::string ValueToString(const int64& Value)  { return ValueToStringInternal<int64> (PRId64, Value); }
template<> inline std::string ValueToString(const uint64& Value) { return ValueToStringInternal<uint64>(PRIu64, Value); }
template<> inline std::string ValueToString(const float& Value)  { return ValueToStringInternal<float> ("f",   Value); }
template<> inline std::string ValueToString(const double& Value) { return ValueToStringInternal<double>("lf",  Value); }

template<> inline 
std::string ValueToString(const bool& Value) 
{ 
	return Value ? "true" : "false";
}

template<>
inline std::string ValueToString(const glm::vec2& Value)
{
	char buff[cBufferSize];
	if (sprintf_s(buff, cBufferSize, "%f, %f", Value.x, Value.y) <= 0)
		throw CException("Unable to write glm::vec2 value.");
	return buff;
}

template<>
inline std::string ValueToString(const glm::vec3& Value)
{
	char buff[cBufferSize];
	if (sprintf_s(buff, cBufferSize, "%f, %f, %f", Value.x, Value.y, Value.z) <= 0)
		throw CException("Unable to write glm::vec3 value.");
	return buff;
}

template<>
inline std::string ValueToString(const glm::vec4& Value)
{
	char buff[cBufferSize];
	if (sprintf_s(buff, cBufferSize, "%f, %f, %f, %f", Value.x, Value.y, Value.z, Value.w) <= 0)
		throw CException("Unable to write glm::vec4 value.");
	return buff;
}


//
// StringToValue
//

namespace
{
	template <typename T>
	inline T StringToValueInternal(const std::string& FullString, const char* Pattern, T DefaultValue)
	{
		T value(DefaultValue);
		if (sscanf_s(FullString.c_str(), (std::string("%") + Pattern).c_str(), &value) != 1)
			throw CException("StringToValue: Unable to read value '%s' from string '%s'.", typeid(T).name(), FullString.c_str());
		return value;
	}
}

template <typename T>
inline T StringToValue(const std::string& String)
{
	throw CException("StringToValue: Unable read string '%s', because type '%s' is not specialized.", String.c_str(), typeid(T).name());
}

template<> 
inline std::string StringToValue(const std::string& String)
{ 
	return String;
}

template<> inline int8   StringToValue(const std::string& String) { return StringToValueInternal<int8>(String,   PRId8,  0); }
template<> inline uint8  StringToValue(const std::string& String) { return StringToValueInternal<uint8>(String,  PRIu8,  0); }
template<> inline int16  StringToValue(const std::string& String) { return StringToValueInternal<int16>(String,  PRId16, 0); }
template<> inline uint16 StringToValue(const std::string& String) { return StringToValueInternal<uint16>(String, PRIu16, 0); }
template<> inline int32  StringToValue(const std::string& String) { return StringToValueInternal<int32>(String,  PRId32, 0); }
template<> inline uint32 StringToValue(const std::string& String) { return StringToValueInternal<uint32>(String, PRIu32, 0); }
template<> inline int64  StringToValue(const std::string& String) { return StringToValueInternal<int64>(String,  PRId64, 0); }
template<> inline uint64 StringToValue(const std::string& String) { return StringToValueInternal<uint64>(String, PRIu64, 0); }
template<> inline float  StringToValue(const std::string& String) { return StringToValueInternal<float>(String,  "f",   0); }
template<> inline double StringToValue(const std::string& String) { return StringToValueInternal<double>(String, "lf",  0); }

template<> inline 
bool StringToValue(const std::string& String)
{
	if (String == "True" || String == "true" || String == "1")
		return true;
	return false;
}

template<>
inline glm::vec2 StringToValue(const std::string& String)
{
	glm::vec2 value(0.0f);
	if (sscanf_s(String.c_str(), "%f, %f", &value.x, &value.y) != 2)
		throw CException("StringToValue: Unable to read value '%s' from string '%s'.", typeid(glm::vec2).name(), String.c_str());
	return value;
}

template<>
inline glm::vec3 StringToValue(const std::string& String)
{
	glm::vec3 value(0.0f);
	if (sscanf_s(String.c_str(), "%f, %f, %f", &value.x, &value.y, &value.z) != 3)
		throw CException("StringToValue: Unable to read value '%s' from string '%s'.", typeid(glm::vec3).name(), String.c_str());
	return value;
}

template<>
inline glm::vec4 StringToValue(const std::string& String)
{
	glm::vec4 value(0.0f);
	if (sscanf_s(String.c_str(), "%f, %f, %f, %f", &value.x, &value.y, &value.z, &value.w) != 4)
		throw CException("StringToValue: Unable to read value '%s' from string '%s'.", typeid(glm::vec4).name(), String.c_str());
	return value;
}