#include "stdafx.h"

// General
#include "SqlDelayThread.h"

// Additional
#include "SqlOperations.h"
#include "DatabaseEnv.h"

SqlDelayThread::SqlDelayThread(Database* db, const char* infoString) :m_running(true)
{

    m_dbEngine = new DatabaseType;
    ((DatabaseType*)m_dbEngine)->Initialize(infoString, false);

}

void SqlDelayThread::run()
{
    SqlOperation* s;
#ifndef DO_POSTGRESQL
    mysql_thread_init();
#endif

    while (m_running)
    {
        try
        {
            s = m_sqlQueue.next();
        }
        catch (...)
        {
            continue;
        }
        if (!s)
            continue;
        s->Execute(m_dbEngine);
        delete s;

    }

#ifndef DO_POSTGRESQL
    mysql_thread_end();
#endif
}

void SqlDelayThread::Stop()
{
    m_running = false;
    m_sqlQueue.cancel();
}

