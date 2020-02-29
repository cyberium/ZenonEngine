#pragma once

#ifdef _DEBUG
	#define _CRTDBG_MAP_ALLOC
	#include <stdlib.h>
	#include <crtdbg.h>

	#define DEBUG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )

	//namespace std
	//{
		inline void* operator new(size_t sz)
		{
			if (sz == 8239)
				_errno();

			if (void *ptr = malloc(sz))
				return ptr;

			return nullptr;
		}

		inline void operator delete(void* ptr) noexcept
		{
			free(ptr);
		}
	//}

	#include <memory>
#endif

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

private:
	std::string m_Message;
};

#ifndef _DEBUG
	#define _ASSERT(expr)   \
	if ((!(expr)))          \
	{                       \
		throw CException("Assertion failed."); \
	}

	#define _ASSERT_EXPR(expr, msg)   \
	if ((!(expr)))                    \
	{                                 \
		throw CException(msg);        \
	} 
#endif