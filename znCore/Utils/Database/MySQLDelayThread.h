#pragma once

#include "SqlDelayThread.h"

class MySQLDelayThread : public SqlDelayThread
{
public:
    MySQLDelayThread(Database* db, const char* infoString) : SqlDelayThread(db, infoString) {}

    void Stop() { SqlDelayThread::Stop(); }
};
