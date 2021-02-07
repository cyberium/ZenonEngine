#pragma once

#include "zthread/Thread.h"
#include "zthread/Runnable.h"
#include "zthread/FastMutex.h"
#include "zthread/MonitoredQueue.h"

class Database;
class SqlOperation;

class SqlDelayThread : public ZThread::Runnable
{
    typedef ZThread::MonitoredQueue<SqlOperation*, ZThread::FastMutex> SqlQueue;
private:
    SqlQueue m_sqlQueue;                                ///< Queue of SQL statements
    Database* m_dbEngine;                               ///< Pointer to used Database engine
    bool m_running;

    SqlDelayThread();
public:
    SqlDelayThread(Database* db, const char* infoString);

    ///< Put sql statement to delay queue
    inline bool Delay(SqlOperation* sql) { m_sqlQueue.add(sql); return true; }

    virtual void Stop();                                ///< Stop event
    virtual void run();                                 ///< Main Thread loop
};
