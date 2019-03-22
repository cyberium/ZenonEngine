#include "stdafx.h"

// General
#include "RefManager1DimAssync.h"

HANDLE m_Thread_Loader = nullptr;
HANDLE m_Event_Add = nullptr;

#ifndef DISABLE_ASSYNC
DWORD WINAPI ThreadProc(LPVOID lpParam)
{
	CRefManager1DimAssync<CRefItem>* manager = (CRefManager1DimAssync<CRefItem>*) lpParam;
	
	while (true)
	{
		manager->MakeContext();

		WaitForSingleObject(m_Event_Add, INFINITE);

		std::string name;
		CRefItem* refItem;
		bool result = manager->m_ObjectsToLoad.pop_front(&name, &refItem);
		if (!result)
		{
			ResetEvent(m_Event_Add);
			continue;
		}

		manager->LoadAction(name, refItem);

		Sleep(20); // DELETE ME
	}

	ExitThread(0);
}
#endif