#pragma once

#ifdef _DEBUG
/*#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#define MYDEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the allocations to be of _CLIENT_BLOCK type
// Compile options needed: /Zi /D_DEBUG /MLd

#define new MYDEBUG_NEW
*/
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
	char* buff = nullptr;
	if (len > 0)
	{
		buff = new char[len + 1];
		vsnprintf(&buff[0], len + 1, _message, args);
		MessageBoxA(HWND_DESKTOP, buff, _title, MB_ICONERROR | MB_OK);
		delete buff;
	}

	va_end(args);
}

#define fail1() \
{ \
	FatalMessageBox("Assertion failed!", "File: [%s]\nLine: [%d]\nFunction: [%s]\n", __FILE__, __LINE__, __FUNCTION__); \
    terminate();\
}

#define fail2(message) \
{ \
	FatalMessageBox("Assertion failed!", "File: [%s]\nLine: [%d]\nFunction: [%s]\nDescription: [%s]\n", __FILE__, __LINE__, __FUNCTION__, message); \
    terminate();\
}

//

#ifdef _DEBUG

#define assert1(expr) _ASSERT(expr)
#define assert2(expr, message) _ASSERT(expr) 
#define assert3(expr, message, arg0) _ASSERT(expr)
#define assert4(expr, message, arg0, arg1) _ASSERT(expr)
#define assert5(expr, message, arg0, arg1, arg2) _ASSERT(expr)
#define assert6(expr, message, arg0, arg1, arg2, arg3) _ASSERT(expr)

#else

#define assert1(expr) \
if(!(expr))\
{\
    FatalMessageBox("Assertion failed!", "File: [%s]\nLine: [%d]\nFunction: [%s]\nExpression: [%s]\n", __FILE__, __LINE__, __FUNCTION__, #expr);\
    terminate();\
}

#define assert2(expr, message) \
if(!(expr))\
{\
    FatalMessageBox("Assertion failed!", "File: [%s]\nLine: [%d]\nFunction: [%s]\nExpression: [%s]\nDescription: [%s]\n", __FILE__, __LINE__, __FUNCTION__, #expr, message);\
    terminate();\
}

#define assert3(expr, message, arg0) \
if(!(expr))\
{\
    FatalMessageBox("Assertion failed!", "File: [%s]\nLine: [%d]\nFunction: [%s]\nExpression: [%s]\nDescription: [%s]\nArgument0: [%s]\n", __FILE__, __LINE__, __FUNCTION__, #expr, message, arg0);\
    terminate();\
}

#define assert4(expr, message, arg0, arg1) \
if(!(expr))\
{\
    FatalMessageBox("Assertion failed!", "File: [%s]\nLine: [%d]\nFunction: [%s]\nExpression: [%s]\nDescription: [%s]\nArgument0: [%s]\nArgument1: [%s]\n", __FILE__, __LINE__, __FUNCTION__, #expr, message, arg0, arg1);\
    terminate();\
}

#define assert5(expr, message, arg0, arg1, arg2) \
if(!(expr))\
{\
    FatalMessageBox("Assertion failed!", "File: [%s]\nLine: [%d]\nFunction: [%s]\nExpression: [%s]\nDescription: [%s]\nArgument0: [%s]\nArgument1: [%s]\nArgument2: [%s]\n", __FILE__, __LINE__, __FUNCTION__, #expr, message, arg0, arg1, arg2);\
    terminate();\
}

#define assert6(expr, message, arg0, arg1, arg2, arg3) \
if(!(expr))\
{\
    FatalMessageBox("Assertion failed!", "File: [%s]\nLine: [%d]\nFunction: [%s]\nExpression: [%s]\nDescription: [%s]\nArgument0: [%s]\nArgument1: [%s]\nArgument2: [%s]\nArgument3: [%s]\n", __FILE__, __LINE__, __FUNCTION__, #expr, message, arg0, arg1, arg2, arg3);\
    terminate();\
}

#endif