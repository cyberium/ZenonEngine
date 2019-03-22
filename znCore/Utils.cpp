#include "stdafx.h"

// General
#include "Utils.h"

std::string Utils::ParseSectionName(std::string& s)
{
	uint32_t bracket = s.find_first_of(']');
	if (bracket == std::string::npos)
		return "";

	return s.substr(1, bracket - 1);
}

std::string Utils::ParseSectionAndIncludeName(std::string& s, std::string& _includeSectionName)
{
	uint32_t bracket = s.find_first_of(']');
	if (bracket == std::string::npos)
		return "";

	uint32_t includeSymbol = s.find_first_of(':');
	if (includeSymbol == std::string::npos)
		_includeSectionName = "";
	else
		_includeSectionName = s.substr(includeSymbol + 1);

	return s.substr(1, bracket - 1);
}

void Utils::ParseKeyPair(std::string& s, std::string& key, std::string& val)
{
	uint32_t separator = s.find_first_of('=');
	if (separator == std::string::npos)
	{
		key = "";
		val = "";
		return;
	}

	key = s.substr(0, separator);
	val = s.substr(separator + 1, s.length());

	key = Trim(key);
	val = Trim(val);
}

bool Utils::TryParse(const type_info& type, cstring value, void* output)
{
	std::stringstream stream(value);

	if (type == typeid(bool))
		stream >> (bool&)*((bool*)output);

	else if (type == typeid(int))
		stream >> (int&)*((int*)output);

	else if (type == typeid(unsigned int))
		stream >> (unsigned int&)*((unsigned int*)output);

	else if (type == typeid(short))
		stream >> (short&)*((short*)output);

	else if (type == typeid(unsigned short))
		stream >> (unsigned short&)*((unsigned short*)output);

	else if (type == typeid(char))
		stream >> (char&)*((char*)output);

	else if (type == typeid(unsigned char))
		stream >> (unsigned char&)*((unsigned char*)output);

	else if (type == typeid(float))
		stream >> (float&)*((float*)output);

	else if (type == typeid(double))
		stream >> (double&)*((double*)output);

	else if (type == typeid(std::string))
		*((std::string *)output) = value;

	else
	{
		//Log::Error("UtilsParsing: a required type is not defined! [%s]", value.c_str());
		return false;
	}

	return !stream.fail();
}

// String

std::string Utils::Trim(std::string& s, cstring delimiters)
{
	return TrimLeft(TrimRight(s, delimiters), delimiters);
}

std::string Utils::TrimLeft(std::string& s, cstring delimiters)
{
	return s.erase(0, s.find_first_not_of(delimiters));
}

std::string Utils::TrimRight(std::string& s, cstring delimiters)
{
	return s.erase(s.find_last_not_of(delimiters) + 1);
}

std::string Utils::ToLower(cstring _string)
{
	std::string str = _string;
	str = Trim(str);
	transform(str.begin(), str.end(), str.begin(), ::tolower);
	return str;
}

std::string Utils::ToUpper(cstring _string)
{
	std::string str = _string;
	str = Trim(str);
	transform(str.begin(), str.end(), str.begin(), ::toupper);
	return str;
}

int Utils::popFirstInt(std::string& s, char separator)
{
	return ToType<int>(GetFirstSubString(s, separator));
}

double Utils::popFirstDouble(std::string& s, char separator)
{
	return ToType<double>(GetFirstSubString(s, separator));
}

float Utils::popFirstFloat(std::string& s, char separator)
{
	return ToType<float>(GetFirstSubString(s, separator));
}

std::string Utils::GetFirstSubString(std::string& s, char separator)
{
	std::string outs = "";
	uint32_t seppos;

	if (separator == 0)
	{
		seppos = s.find_first_of(',');
		uint32_t alt_seppos = s.find_first_of(';');

		if (alt_seppos != std::string::npos && alt_seppos < seppos)
		{
			seppos = alt_seppos; // return the first ',' or ';'
		}
	}
	else
	{
		seppos = s.find_first_of(separator);
	}

	if (seppos == std::string::npos)
	{
		outs = s;
		s = "";
	}
	else
	{
		outs = s.substr(0, seppos);
		s = s.substr(seppos + 1, s.length());
	}

	return outs;
}

std::string Utils::getNextToken(cstring s, uint32_t& cursor, char separator)
{
	uint32_t seppos = s.find_first_of(separator, cursor);
	if (seppos == std::string::npos)
	{ // not found
		cursor = std::string::npos;
		return "";
	}
	std::string outs = s.substr(cursor, seppos - cursor);
	cursor = seppos + 1;
	return outs;
}

std::string Utils::stripCarriageReturn(cstring line)
{
	if (line.length() > 0)
	{
		if ('\r' == line.at(line.length() - 1))
		{
			return line.substr(0, line.length() - 1);
		}
	}
	return line;
}

std::string Utils::getLine(std::ifstream& infile)
{
	std::string line;
	// This is the standard way to check whether a read failed.
	if (!std::getline(infile, line))
		return "";
	line = stripCarriageReturn(line);
	return line;
}

bool Utils::PointInRectangle(const vec2 _point, const vec2 _rectStart, const vec2 _rectEnd)
{
	return
		_point.x > _rectStart.x && _point.y > _rectStart.y &&
		_point.x < _rectEnd.x && _point.y < _rectEnd.y;
}
