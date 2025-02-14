#ifndef DATABASE_H
#define DATABASE_H

#include "zthread/Thread.h"
#include "zthread/ThreadImpl.h"

#include "SqlDelayThread.h"

class SqlTransaction;
class SqlResultQueue;
class SqlQueryHolder;

typedef std::unordered_map<ZThread::ThreadImpl*, SqlTransaction*> TransactionQueues;
typedef std::unordered_map<ZThread::ThreadImpl*, SqlResultQueue*> QueryQueues;

#define MAX_QUERY_LEN   1024

class Database
{
protected:
    Database() : m_threadBody(NULL), m_delayThread(NULL) {};

    TransactionQueues m_tranQueues;                     ///< Transaction queues from diff. threads
    QueryQueues m_queryQueues;                          ///< Query queues from diff threads
    SqlDelayThread* m_threadBody;                       ///< Pointer to delay sql executer
    ZThread::Thread* m_delayThread;                     ///< Pointer to executer thread

public:

    virtual ~Database();

    virtual bool Initialize(const char *infoString);
    virtual void InitDelayThread(const char *infoString) = 0;
    virtual void HaltDelayThread() = 0;

    virtual QueryResult* Query(const char *sql) = 0;
    QueryResult* PQuery(const char *format, ...);

    /// Async queries and query holders, implemented in DatabaseImpl.h

    // Query / member
    template<class Class>
    bool AsyncQuery(Class *object, void (Class::*method)(QueryResult*), const char *sql);
    template<class Class, typename ParamType1>
    bool AsyncQuery(Class *object, void (Class::*method)(QueryResult*, ParamType1), ParamType1 param1, const char *sql);
    template<class Class, typename ParamType1, typename ParamType2>
    bool AsyncQuery(Class *object, void (Class::*method)(QueryResult*, ParamType1, ParamType2), ParamType1 param1, ParamType2 param2, const char *sql);
    template<class Class, typename ParamType1, typename ParamType2, typename ParamType3>
    bool AsyncQuery(Class *object, void (Class::*method)(QueryResult*, ParamType1, ParamType2, ParamType3), ParamType1 param1, ParamType2 param2, ParamType3 param3, const char *sql);
    // Query / static
    template<typename ParamType1>
    bool AsyncQuery(void(*method)(QueryResult*, ParamType1), ParamType1 param1, const char *sql);
    template<typename ParamType1, typename ParamType2>
    bool AsyncQuery(void(*method)(QueryResult*, ParamType1, ParamType2), ParamType1 param1, ParamType2 param2, const char *sql);
    template<typename ParamType1, typename ParamType2, typename ParamType3>
    bool AsyncQuery(void(*method)(QueryResult*, ParamType1, ParamType2, ParamType3), ParamType1 param1, ParamType2 param2, ParamType3 param3, const char *sql);
    // PQuery / member
    template<class Class>
    bool AsyncPQuery(Class *object, void (Class::*method)(QueryResult*), const char *format, ...);
    template<class Class, typename ParamType1>
    bool AsyncPQuery(Class *object, void (Class::*method)(QueryResult*, ParamType1), ParamType1 param1, const char *format, ...);
    template<class Class, typename ParamType1, typename ParamType2>
    bool AsyncPQuery(Class *object, void (Class::*method)(QueryResult*, ParamType1, ParamType2), ParamType1 param1, ParamType2 param2, const char *format, ...);
    template<class Class, typename ParamType1, typename ParamType2, typename ParamType3>
    bool AsyncPQuery(Class *object, void (Class::*method)(QueryResult*, ParamType1, ParamType2, ParamType3), ParamType1 param1, ParamType2 param2, ParamType3 param3, const char *format, ...);
    // PQuery / static
    template<typename ParamType1>
    bool AsyncPQuery(void(*method)(QueryResult*, ParamType1), ParamType1 param1, const char *format, ...);
    template<typename ParamType1, typename ParamType2>
    bool AsyncPQuery(void(*method)(QueryResult*, ParamType1, ParamType2), ParamType1 param1, ParamType2 param2, const char *format, ...);
    template<typename ParamType1, typename ParamType2, typename ParamType3>
    bool AsyncPQuery(void(*method)(QueryResult*, ParamType1, ParamType2, ParamType3), ParamType1 param1, ParamType2 param2, ParamType3 param3, const char *format, ...);
    template<class Class>
    // QueryHolder
    bool DelayQueryHolder(Class *object, void (Class::*method)(QueryResult*, SqlQueryHolder*), SqlQueryHolder *holder);
    template<class Class, typename ParamType1>
    bool DelayQueryHolder(Class *object, void (Class::*method)(QueryResult*, SqlQueryHolder*, ParamType1), SqlQueryHolder *holder, ParamType1 param1);

    virtual bool Execute(const char *sql) = 0;
    bool PExecute(const char *format, ...);
    virtual bool DirectExecute(const char* sql) = 0;
    bool DirectPExecute(const char *format, ...);

    // Writes SQL commands to a LOG file
    bool PExecuteLog(const char *format, ...);

    virtual bool BeginTransaction()                     // nothing do if DB not support transactions
    {
        return true;
    }
    virtual bool CommitTransaction()                    // nothing do if DB not support transactions
    {
        return true;
    }
    virtual bool RollbackTransaction()                  // can't rollback without transaction support
    {
        return false;
    }

    virtual operator bool() const = 0;

    virtual unsigned long escape_string(char *to, const char *from, unsigned long length) { strncpy_s(to, length, from, length); return length; }
    void escape_string(std::string& str);

    // must be called before first query in thread (one time for thread using one from existed Database objects)
    virtual void ThreadStart();
    // must be called before finish thread run (one time for thread using one from existed Database objects)
    virtual void ThreadEnd();

    // sets the result queue of the current thread, be careful what thread you call this from
    void SetResultQueue(SqlResultQueue * queue);

private:
    bool m_logSQL;
    std::string m_logsDir;
};
#endif

