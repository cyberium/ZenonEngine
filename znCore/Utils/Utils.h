#pragma once

namespace Utils
{
    ZN_API std::string Trim(std::string& _string, const std::string& delimiters = " \f\n\r\t\v");
	ZN_API std::string TrimLeft(std::string& _string, const std::string& delimiters = " \f\n\r\t\v");
	ZN_API std::string TrimRight(std::string& _string, const std::string& delimiters = " \f\n\r\t\v");

	ZN_API std::string ToLower(const std::string& _string);
	ZN_API std::string ToUpper(const std::string& _string);

	ZN_API std::vector<std::string> ArgumentsToVector(int argumentsCount, char* arguments[]);

	// Filename
	ZN_API std::string FixFilePath(const std::string& FilePath);
	struct ZN_API SFileName
	{
		std::string Path;
		std::string Name;
		std::string NameWithoutExtension;
		std::string Extension;

		std::string ToString() const
		{
			return Path + "/" + NameWithoutExtension + "." + Extension;
		}
	};
	ZN_API SFileName SplitFilename(const std::string& FilePath);

	// Files
	ZN_API std::string GetWorkingDirectory();
	ZN_API std::string LoadLocalFile(std::string FileName);
	ZN_API void        SaveToLocalFile(std::string FileName, const std::string& Content);

	// Base64
	ZN_API std::string        Base64_Encode(const uint8* buf, size_t bufLen);
	ZN_API std::vector<uint8> Base64_Decode(const std::string& String);

	ZN_API std::string FloatToString(const float Value, const int N = 3);
	ZN_API std::string MatrixToString(const glm::mat4& Matrix);
	ZN_API glm::mat4   StringToMatrix(const std::string& String);

	ZN_API std::string MatrixToBase64(const glm::mat4& Matrix);
	ZN_API glm::mat4   Base64ToMatrix(const std::string& Base64);
};


namespace Resources
{
#if 0
	ZN_API std::string utf8_to_cp1251(const std::string& AString);
	ZN_API std::string cp1251_to_utf8(const std::string& AString);
#endif

	ZN_API std::wstring utf8_to_utf16(const std::string& AString);
	ZN_API std::string utf16_to_utf8(const std::wstring& WString);
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
