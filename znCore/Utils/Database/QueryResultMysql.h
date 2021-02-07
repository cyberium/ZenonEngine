#pragma once

#include "QueryResult.h"

#include <mysql.h>

class QueryResultMysql : public QueryResult
{
public:
    QueryResultMysql(MYSQL_RES *result, uint64 rowCount, uint32 fieldCount);

    ~QueryResultMysql();

    bool NextRow();

private:
    enum Field::DataTypes ConvertNativeType(enum_field_types mysqlType) const;
    void EndQuery();

    MYSQL_RES *mResult;
};
