#pragma once

#include "QueryResult.h"
#include "Database.h"
#include "zthread/FastMutex.h"

#include <mysql.h>

class DatabaseMysql : public Database
{
public:
    DatabaseMysql();
    ~DatabaseMysql();

    //! Initializes Mysql and connects to a server.
    /*! infoString should be formated like hostname;username;password;database. */
    bool Initialize(const char *infoString, bool initDelayThread = true);
    void InitDelayThread(const char* infoString);
    void HaltDelayThread();
    QueryResult* Query(const char *sql);
    bool Execute(const char *sql);
    bool DirectExecute(const char* sql);
    bool BeginTransaction();
    bool CommitTransaction();
    bool RollbackTransaction();

    operator bool() const { return mMysql != NULL; }

    unsigned long escape_string(char *to, const char *from, unsigned long length);
    using Database::escape_string;

    // must be call before first query in thread
    void ThreadStart();
    // must be call before finish thread run
    void ThreadEnd();

private:
    ZThread::FastMutex mMutex;

    ZThread::ThreadImpl* tranThread;

    MYSQL *mMysql;

    static size_t db_count;

    bool _TransactionCmd(const char *sql);
};
