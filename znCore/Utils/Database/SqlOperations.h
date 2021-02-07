#pragma once

#include "zthread/LockedQueue.h"
#include "zthread/FastMutex.h"
#include "zthread/Thread.h"

/// ---- BASE ---

class Database;
class SqlDelayThread;

class SqlOperation
{
public:
    virtual void OnRemove() { delete this; }
    virtual void Execute(Database *db) = 0;
    virtual ~SqlOperation() {}
};

/// ---- ASYNC STATEMENTS / TRANSACTIONS ----

class SqlStatement : public SqlOperation
{
private:
    const char *m_sql;
public:
    SqlStatement(const char *sql) : m_sql(_strdup(sql)) {}
    ~SqlStatement() { void* tofree = const_cast<char*>(m_sql); free(tofree); }
    void Execute(Database *db);
};

class SqlTransaction : public SqlOperation
{
private:
    std::queue<const char *> m_queue;
public:
    SqlTransaction() {}
    void DelayExecute(const char *sql) { m_queue.push(_strdup(sql)); }
    void Execute(Database *db);
};

/// ---- ASYNC QUERIES ----

class SqlQuery;                                             /// contains a single async query
class QueryResult;                                          /// the result of one
class SqlResultQueue;                                       /// queue for thread sync
class SqlQueryHolder;                                       /// groups several async quries
class SqlQueryHolderEx;                                     /// points to a holder, added to the delay thread

class SqlResultQueue : public ZThread::LockedQueue<IQueryCallback*, ZThread::FastMutex>
{
public:
    SqlResultQueue() {}
    void Update();
};

class SqlQuery : public SqlOperation
{
private:
    const char *m_sql;
    IQueryCallback * m_callback;
    SqlResultQueue * m_queue;
public:
    SqlQuery(const char *sql, IQueryCallback * callback, SqlResultQueue * queue)
        : m_sql(_strdup(sql)), m_callback(callback), m_queue(queue) {}
    ~SqlQuery() { void* tofree = const_cast<char*>(m_sql); free(tofree); }
    void Execute(Database *db);
};

class SqlQueryHolder
{
    friend class SqlQueryHolderEx;
private:
    typedef std::pair<const char*, QueryResult*> SqlResultPair;
    std::vector<SqlResultPair> m_queries;
public:
    SqlQueryHolder() {}
    ~SqlQueryHolder();
    bool SetQuery(size_t index, const char *sql);
    bool SetPQuery(size_t index, const char *format, ...);
    void SetSize(size_t size);
    QueryResult* GetResult(size_t index);
    void SetResult(size_t index, QueryResult *result);
    bool Execute(IQueryCallback * callback, SqlDelayThread *thread, SqlResultQueue *queue);
};

class SqlQueryHolderEx : public SqlOperation
{
private:
    SqlQueryHolder * m_holder;
    IQueryCallback * m_callback;
    SqlResultQueue * m_queue;
public:
    SqlQueryHolderEx(SqlQueryHolder *holder, IQueryCallback * callback, SqlResultQueue * queue)
        : m_holder(holder), m_callback(callback), m_queue(queue) {}
    void Execute(Database *db);
};
