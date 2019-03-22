#include "stdafx.h"
#include "Utils.h"

std::string Utils::ToString(const type_info& type, void* value)
{
	std::stringstream stream;

	if (type == typeid(bool))
		stream << *((bool*)value);

	else if (type == typeid(int))
		stream << *((int*)value);

	else if (type == typeid(unsigned int))
		stream << *((unsigned int*)value);

	else if (type == typeid(short))
		stream << *((short*)value);

	else if (type == typeid(unsigned short))
		stream << *((unsigned short*)value);

	else if (type == typeid(char))
		stream << *((char*)value);

	else if (type == typeid(unsigned char))
		stream << *((unsigned char*)value);

	else if (type == typeid(float))
		stream << *((float*)value);

	else if (type == typeid(double))
		stream << *((double*)value);

	else if (type == typeid(std::string))
		return (std::string&)*((std::string *)value);

	else
	{
		Log::Error("UtilsParsing: a required type is not defined!");
		return "";
	}

	return stream.str();
}




bool Utils::ToBool(cstring _string)
{
	std::string _str = _string;
	Trim(_str);
	transform(_str.begin(), _str.end(), _str.begin(), ::tolower);

	if (_str == "true") return true;
	if (_str == "1") return true;

	if (_str == "false") return false;
	if (_str == "0") return false;

	return false;
}

// My types

vec2 Utils::ToPoint(std::string& _string)
{
	float _x = popFirstFloat(_string);
	float _y = popFirstFloat(_string);
	return vec2(_x, _y);
}

vec3 Utils::ToVector3(std::string& _string)
{
	float _x = popFirstFloat(_string);
	float _y = popFirstFloat(_string);
	float _z = popFirstFloat(_string);
	return vec3(_x, _y, _z);
}
