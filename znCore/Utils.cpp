#include "stdafx.h"

// General
#include "Utils.h"

// Additional
#include <filesystem>
namespace fs = std::experimental::filesystem;

namespace Utils
{
	std::string Trim(std::string& s, const std::string& delimiters)
	{
		return TrimLeft(TrimRight(s, delimiters), delimiters);
	}

	std::string TrimLeft(std::string& s, const std::string& delimiters)
	{
		return s.erase(0, s.find_first_not_of(delimiters));
	}

	std::string TrimRight(std::string& s, const std::string& delimiters)
	{
		return s.erase(s.find_last_not_of(delimiters) + 1);
	}

	std::string ToLower(const std::string& _string)
	{
		std::string str = _string;
		str = Trim(str);
		transform(str.begin(), str.end(), str.begin(), ::tolower);
		return str;
	}

	std::string ToUpper(const std::string& _string)
	{
		std::string str = _string;
		str = Trim(str);
		transform(str.begin(), str.end(), str.begin(), ::toupper);
		return str;
	}

	glm::vec3 ToVec3(const std::string & _string)
	{
		float x = Math::MaxFloat, y = Math::MaxFloat, z = Math::MaxFloat;
		auto result = sscanf_s(_string.c_str(), "%f, %f, %f", &x, &y, &z);
		if (result <= 0)
			throw CException("Utils::ToVec3(%s) error.", _string.c_str());
		return glm::vec3(x, y, z);
	}

	glm::vec4 ToVec4(const std::string & _string)
	{
		float x = Math::MaxFloat, y = Math::MaxFloat, z = Math::MaxFloat, w = Math::MaxFloat;
		auto result = sscanf_s(_string.c_str(), "%f, %f, %f, %f", &x, &y, &z, &w);
		if (result <= 0)
			throw CException("Utils::ToVec4(%s) error.", _string.c_str());
		return glm::vec4(x, y, z, w);
	}

	std::vector<std::string> ArgumentsToVector(int argumentsCount, char * arguments[])
	{
		std::vector<std::string> argumnets;
		for (int i = 0; i < argumentsCount; i++)
		{
			argumnets.push_back(std::string(arguments[i]));
		}
		return argumnets;
	}

	std::vector<std::string> GetAllFilesInDirectory(const std::string& Directory, const std::string& FileExtention, const std::vector<std::string> DirSkipList)
	{
		std::vector<std::string> listOfFiles;

		try
		{
			bool isExists = fs::exists(Directory);
			bool isDir = fs::is_directory(Directory);

			// Check if given path exists and points to a directory
			if (isExists && isDir)
			{
				// Create a Recursive Directory Iterator object and points to the starting of directory
				fs::recursive_directory_iterator iter(Directory);

				// Create a Recursive Directory Iterator object pointing to end.
				fs::recursive_directory_iterator end;

				// Iterate till end
				while (iter != end)
				{
					// Check if current entry is a directory and if exists in skip list
					if (fs::is_directory(iter->path()) && (std::find(DirSkipList.begin(), DirSkipList.end(), iter->path().filename()) != DirSkipList.end()))
					{
						iter.disable_recursion_pending();
					}
					else if (!fs::is_directory(iter->path()) && (!FileExtention.empty()) && (iter->path().has_extension()) && (Utils::ToLower(iter->path().extension().string()) == Utils::ToLower(FileExtention)))
					{
						listOfFiles.push_back(iter->path().string());
					}
					else
					{
						iter.disable_recursion_pending();
					}

					std::error_code ec;
					// Increment the iterator to point to next entry in recursive iteration
					iter.increment(ec);
					if (ec)
						throw CException("GetAllFilesInDirectory: Error while accessing '%s'. Error: '%s'.", iter->path().string().c_str(), ec.message().c_str());
				}
			}
		}
		catch (const std::system_error& e)
		{
			throw CException("GetAllFilesInDirectory: Exception '%s'", e.what());
		}

		return listOfFiles;
	}
}

namespace Resources
{
	// Convert a multi-byte character std::string (UTF-8) to a multi-byte character std::string (CP1251)
	std::string utf8_to_cp1251(const std::string& AString)
	{
		std::string res;

		int result_u = MultiByteToWideChar(CP_UTF8, 0, AString.c_str(), -1, 0, 0);
		if (!result_u)
		{
			return 0;
		}

		wchar_t* ures = new wchar_t[result_u];
		if (!MultiByteToWideChar(CP_UTF8, 0, AString.c_str(), -1, ures, result_u))
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
	std::string cp1251_to_utf8(const std::string& AString)
	{
		std::string res;

		int result_u = MultiByteToWideChar(1251, 0, AString.c_str(), -1, 0, 0);
		if (!result_u)
		{
			return 0;
		}

		wchar_t *ures = new wchar_t[result_u];
		if (!MultiByteToWideChar(1251, 0, AString.c_str(), -1, ures, result_u))
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
	std::wstring utf8_to_utf16(const std::string& AString)
	{
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		return converter.from_bytes(AString);
	}

	// Converts a wide (UTF-16) encoded std::string into a multi-byte (UTF-8) character std::string.
	std::string utf16_to_utf8(const std::wstring& WString)
	{
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		return converter.to_bytes(WString);
	}

	// Gets a std::string resource from the module's resources.
	std::wstring GetStringResource(HMODULE HModule, int ID, const std::wstring& type)
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
}
