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
