#pragma once

#include "Query.h"

class QueryOGL : public Query
{
public:

	QueryOGL(QueryType queryType, uint8_t numBuffers);
	virtual ~QueryOGL();

	virtual void Begin(int64_t frame = 0L);
	virtual void End(int64_t frame = 0L);
	virtual bool QueryResultAvailable(int64_t frame = 0L);
	virtual QueryResult GetQueryResult(int64_t frame = 0L);
	virtual uint8_t GetBufferCount() const;

private:
	typedef std::vector<uint32> QueryBuffer;
	QueryBuffer m_Queries; // For timer queries, we need 2 sets of buffered queries.

	QueryType m_QueryType;
	GLenum m_QueryTypeGL;

	uint8_t m_NumBuffers; // How many queries will be used to prevent stalling the GPU.
};