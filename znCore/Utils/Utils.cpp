#include "stdafx.h"

// General
#include "Utils.h"

// Additional
#include <sstream>
#include <iomanip>
#include <filesystem>

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

	std::vector<std::string> ArgumentsToVector(int argumentsCount, char * arguments[])
	{
		std::vector<std::string> argumnets;
		for (int i = 0; i < argumentsCount; i++)
		{
			argumnets.push_back(std::string(arguments[i]));
		}
		return argumnets;
	}

	std::string FixFilePath(const std::string& FilePath)
	{
		size_t index = 0;
		std::string newFilePath = FilePath;
		while (true)
		{
			index = newFilePath.find('\\', index);
			if (index == std::string::npos)
				break;

			newFilePath.replace(index, 1, "/");
			index += 3;
		}
		return newFilePath;
	}

	std::pair<std::string, std::string> GetFilenamePathAndNameExtension(const std::string & FilePath)
	{
		std::string fixedFilename = FixFilePath(FilePath);

		auto lastSlashPos = fixedFilename.find_last_of('/');
		if (lastSlashPos != std::string::npos)
		{
			std::string path = fixedFilename.substr(0, lastSlashPos + 1);
			std::string name = fixedFilename.substr(lastSlashPos + 1);
			return std::make_pair(path, name);
		}

		return std::make_pair("", fixedFilename);
	}

	std::pair<std::string, std::string> GetFilenamePathNameAndExtension(const std::string& FilePath)
	{
		std::string fixedFilename = FixFilePath(FilePath);

		auto lastPointPos = fixedFilename.find_last_of('.');
		if (lastPointPos != std::string::npos)
		{
			std::string nameWithoutExtension = fixedFilename.substr(0, lastPointPos);
			std::string extension = fixedFilename.substr(lastPointPos + 1);
			extension = Utils::ToLower(extension);

			return std::make_pair(nameWithoutExtension, extension);
		}

		return std::make_pair(fixedFilename, "");
	}

	std::string GetExecutablePath()
	{
		char buffer[MAX_PATH];
		if (FALSE == GetModuleFileNameA(NULL, buffer, MAX_PATH))
			throw CException("Error while 'GetModuleFileName'.");
		std::string f(buffer);
		return f.substr(0, f.find_last_of("\\/"));
	}

	std::string GetWorkingDirectory()
	{
		return std::experimental::filesystem::current_path().string();
	}

	std::string LoadLocalFile(std::string FileName)
	{
		std::string fileName = GetWorkingDirectory() + "//" + FileName;

		std::ifstream stream;
		stream.open(fileName, std::ios::binary);
		if (false == stream.is_open())
			throw CException("Unable to open file '%s'.", FileName.c_str());

		// FileSize
		stream.seekg(0, stream.end);
		uint32_t fileSize = uint32_t(stream.tellg());
		stream.seekg(0, stream.beg);

		std::string content;
		content.resize(fileSize, '\0');

		stream.read(reinterpret_cast<char*>(&content[0]), fileSize);

		std::streamsize readedBytes = stream.gcount();
		if (readedBytes < fileSize)
		{
			stream.close();
			throw CException("File '%s' stream reading error. Readed '%d' of '%d'.", fileName.c_str(), static_cast<uint64>(readedBytes), fileSize);
		}

		stream.close();
		return content;
	}

	void SaveToLocalFile(std::string FileName, const std::string& Content)
	{
		std::string fileName = GetWorkingDirectory() + "//" + FileName;

		if (std::experimental::filesystem::exists(fileName))
		{
			Log::Warn("SaveToLocalFile: File '%s' already exists and will be removed.", fileName.c_str());

			if (false == std::experimental::filesystem::remove(fileName))
				throw CException("Unable to remove '%s' file.", fileName.c_str());
		}

		std::ofstream stream;
		stream.open(fileName, std::ios::binary);
		if (false == stream.is_open())
			throw CException("Unable to open ofstream '%s'.", fileName.c_str());

		stream.write(Content.c_str(), Content.size());
		stream.flush();
		stream.close();
	}

	std::string FloatToString(const float Value, const int N)
	{
		std::string strVal(std::to_string(Value));
		const auto& it = strVal.find('.');
		if (it == std::string::npos)
			return strVal;

		size_t offset = it + 1;
		if (offset + N < strVal.length())
			offset += N;
		else
			offset += strVal.length() - offset;

		return strVal.substr(0, offset);
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
