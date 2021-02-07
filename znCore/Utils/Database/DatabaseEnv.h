#pragma once

// Base
#include "Field.h"
#include "QueryResult.h"
#include "Database.h"

// MySQL
#include "QueryResultMysql.h"
#include "DatabaseMysql.h"
typedef DatabaseMysql DatabaseType;

#define _LIKE_           "LIKE"
#define _TABLE_SIM_      "`"
#define _CONCAT3_(A,B,C) "CONCAT( " A " , " B " , " C " )"
#define _OFFSET_         "LIMIT %d,1"

