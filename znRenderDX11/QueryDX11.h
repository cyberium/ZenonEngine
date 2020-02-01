#pragma once

#include "Query.h"

class ZN_API QueryDX11 
	: public IQuery
{
public:
	QueryDX11(IRenderDeviceDX11& RenderDeviceDX11, QueryType queryType, uint8_t numBuffers);
	virtual ~QueryDX11();

	virtual void Begin(int64_t frame = 0L);
	virtual void End(int64_t frame = 0L);
	virtual bool QueryResultAvailable(int64_t frame = 0L);
	virtual QueryResult GetQueryResult(int64_t frame = 0L);
	virtual uint8_t GetBufferCount() const;

protected:

private:
	typedef std::vector< ATL::CComPtr<ID3D11Query> > QueryBuffer;
	QueryBuffer m_DisjointQueries;
	// For timer queries, we need 2 sets of buffered queries.
	QueryBuffer m_Queries[2];

	QueryType m_QueryType;
	
	uint8_t m_NumBuffers; // How many queries will be used to prevent stalling the GPU.

private: // Link to parent d3d11 device
	IRenderDeviceDX11& m_RenderDeviceDX11;
};