#include "stdafx.h"

// General
#include "Utils.h"

// Additional
#include "Files/ByteBuffer.h"

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

	std::vector<std::string> SplitTextToLines(const std::string& Text)
	{
		std::vector<std::string> lines;

		std::string currentLine = "";
		for (auto i = 0; i < Text.length(); i++)
		{
			const auto currentChar = Text[i];
			if (currentChar == '\n')
			{
				lines.push_back(currentLine);
				currentLine.clear();
				continue;
			}

			currentLine += currentChar;
		}

		return lines;
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

	ZN_API SFileName SplitFilename(const std::string & FilePath)
	{
		std::string fixedFilename = FixFilePath(FilePath);

		SFileName fileNameStruct;

		auto lastSlashPos = fixedFilename.find_last_of('/');
		if (lastSlashPos != std::string::npos)
		{
			fileNameStruct.Path = fixedFilename.substr(0, lastSlashPos + 1);
			fileNameStruct.Name = fixedFilename.substr(lastSlashPos + 1);
		}
		else
		{
			fileNameStruct.Path = "";
			fileNameStruct.Name = fixedFilename;
		}

		auto lastPointPos = fileNameStruct.Name.find_last_of('.');
		if (lastPointPos != std::string::npos)
		{
			fileNameStruct.NameWithoutExtension = fileNameStruct.Name.substr(0, lastPointPos);
			fileNameStruct.Extension = Utils::ToLower(fileNameStruct.Name.substr(lastPointPos + 1));
		}
		else
		{
			fileNameStruct.NameWithoutExtension = fileNameStruct.Name;
			fileNameStruct.Extension = "";
		}

		return fileNameStruct;
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

	static const std::string base64_chars =
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"abcdefghijklmnopqrstuvwxyz"
		"0123456789+/";


	static inline bool is_base64(uint8 c)
	{
		return (isalnum(c) || (c == '+') || (c == '/'));
	}

	std::string Base64_Encode(const uint8* buf, size_t bufLen) 
	{
		std::string ret;
		int i = 0;
		int j = 0;
		uint8 char_array_3[3];
		uint8 char_array_4[4];

		while (bufLen--) 
		{
			char_array_3[i++] = *(buf++);
			if (i == 3) 
			{
				char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
				char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
				char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
				char_array_4[3] = char_array_3[2] & 0x3f;

				for (i = 0; (i < 4); i++)
					ret += base64_chars[char_array_4[i]];
				i = 0;
			}
		}

		if (i)
		{
			for (j = i; j < 3; j++)
				char_array_3[j] = '\0';

			char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
			char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
			char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
			char_array_4[3] = char_array_3[2] & 0x3f;

			for (j = 0; (j < i + 1); j++)
				ret += base64_chars[char_array_4[j]];

			while ((i++ < 3))
				ret += '=';
		}

		return ret;
	}

	std::vector<uint8> Base64_Decode(const std::string& String)
	{
		int in_len = String.size();
		int i = 0;
		int j = 0;
		int in_ = 0;
		uint8 char_array_4[4], char_array_3[3];
		std::vector<uint8> ret;

		while (in_len-- && (String[in_] != '=') && is_base64(String[in_]))
		{
			char_array_4[i++] = String[in_]; in_++;
			if (i == 4) 
			{
				for (i = 0; i < 4; i++)
					char_array_4[i] = base64_chars.find(char_array_4[i]);

				char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
				char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
				char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

				for (i = 0; (i < 3); i++)
					ret.push_back(char_array_3[i]);
				i = 0;
			}
		}

		if (i) {
			for (j = i; j < 4; j++)
				char_array_4[j] = 0;

			for (j = 0; j < 4; j++)
				char_array_4[j] = base64_chars.find(char_array_4[j]);

			char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
			char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
			char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

			for (j = 0; (j < i - 1); j++) ret.push_back(char_array_3[j]);
		}

		return ret;
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

	std::string MatrixToString(const glm::mat4& Matrix)
	{
		char buffer[255];
		if (std::sprintf(buffer, "[%.3f %.3f %.3f %.3f] [%.3f %.3f %.3f %.3f] [%.3f %.3f %.3f %.3f] [%.3f %.3f %.3f %.3f]",
			Matrix[0][0], Matrix[0][1], Matrix[0][2], Matrix[0][3],
			Matrix[1][0], Matrix[1][1], Matrix[1][2], Matrix[1][3],
			Matrix[2][0], Matrix[2][1], Matrix[2][2], Matrix[2][3],
			Matrix[3][0], Matrix[3][1], Matrix[3][2], Matrix[3][3]) <= 0)
			throw CException("Error");
		return buffer;
	}

	glm::mat4 StringToMatrix(const std::string& String)
	{
		glm::mat4 matrix;
		if (std::sscanf(String.c_str(), "[%f %f %f %f] [%f %f %f %f] [%f %f %f %f] [%f %f %f %f]",
			&matrix[0][0], &matrix[0][1], &matrix[0][2], &matrix[0][3],
			&matrix[1][0], &matrix[1][1], &matrix[1][2], &matrix[1][3],
			&matrix[2][0], &matrix[2][1], &matrix[2][2], &matrix[2][3],
			&matrix[3][0], &matrix[3][1], &matrix[3][2], &matrix[3][3]) != 16)
			throw CException("Error");
		return matrix;
	}

	std::string MatrixToBase64(const glm::mat4& Matrix)
	{
		CByteBuffer byteBuffer;
		byteBuffer.write(&Matrix);
		return Base64_Encode(byteBuffer.getData(), byteBuffer.getSize());
	}
	glm::mat4 Base64ToMatrix(const std::string& Base64)
	{
		CByteBuffer byteBuffer(Base64_Decode(Base64));
		glm::mat4 matrix;
		byteBuffer.read(&matrix);
		return matrix;
	}
}

namespace Resources
{
#if 0
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
#endif

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
}
