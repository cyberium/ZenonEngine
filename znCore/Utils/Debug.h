#pragma once

namespace StaticAssert
{
	template<bool>
	struct StaticAssertFailed;

	template<>
	struct StaticAssertFailed<true> {};
}
#define assert_static(exp) (StaticAssert::StaticAssertFailed<(exp) != 0>())

//

inline void FatalMessageBox(const char* _title, const char* _message, ...)
{
	va_list args;
	va_start(args, _message);

	int len = vsnprintf(NULL, 0, _message, args);
	if (len > 0)
	{
		std::string message;
		message.resize(len);
		vsnprintf(&message[0], len, _message, args);
		MessageBoxA(HWND_DESKTOP, message.c_str(), _title, MB_ICONERROR | MB_OK);
	}
		
	va_end(args);

	DebugBreak();
	exit(-1);
}

class ZN_API CException 
{
public:
	CException(const char* Message, ...);
	CException(std::string Message);

	CException(const wchar_t* WMessage, ...);
	CException(std::wstring WMessage);

	const std::string Message() const { return m_Message; }
	const char* MessageCStr() const { return m_Message.c_str(); }

protected:
	CException() 
		: m_Message("")
	{}

protected:
	std::string m_Message;
};

class ZN_API CznRenderException
	: public CException
{
public:
	CznRenderException(const char* Message, ...);
	CznRenderException(std::string Message) : CException(Message) {}
	CznRenderException(const wchar_t* WMessage, ...);
	CznRenderException(std::wstring WMessage) : CException(WMessage) {}
};
