#include "stdafx.h"

#include "QueryDX11.h"

QueryDX11::QueryDX11(IRenderDeviceDX11* RenderDeviceD3D11, QueryType queryType, uint8_t numBuffers)
	: m_RenderDeviceD3D11(RenderDeviceD3D11)
	, m_QueryType(queryType)
	, m_NumBuffers(numBuffers)
{
	D3D11_QUERY_DESC queryDesc = {};

	switch (m_QueryType)
	{
	case QueryType::Timer:
		queryDesc.Query = D3D11_QUERY_TIMESTAMP;
		break;
	case QueryType::CountSamples:
		queryDesc.Query = D3D11_QUERY_OCCLUSION;
		break;
	case QueryType::CountSamplesPredicate:
		queryDesc.Query = D3D11_QUERY_OCCLUSION_PREDICATE;
		break;
	case QueryType::CountPrimitives:
	case QueryType::CountTransformFeedbackPrimitives:
		queryDesc.Query = D3D11_QUERY_SO_STATISTICS;
		break;
	default:
		break;
	}

	m_Queries[0].resize(m_NumBuffers);

	for (uint8_t i = 0; i < m_NumBuffers; ++i)
	{
		if (FAILED(m_RenderDeviceD3D11->GetDeviceD3D11()->CreateQuery(&queryDesc, &m_Queries[0][i])))
		{
			Log::Error("Failed to create Query object.");
		}
	}

	// For timer queries, we also need to create the disjoint timer queries.
	if (m_QueryType == IQuery::QueryType::Timer)
	{
		m_DisjointQueries.resize(m_NumBuffers);
		m_Queries[1].resize(m_NumBuffers);

		D3D11_QUERY_DESC disjointQueryDesc = {};
		disjointQueryDesc.Query = D3D11_QUERY_TIMESTAMP_DISJOINT;

		for (uint8_t i = 0; i < m_NumBuffers; ++i)
		{
			if (FAILED(m_RenderDeviceD3D11->GetDeviceD3D11()->CreateQuery(&queryDesc, &m_Queries[1][i])) ||
				FAILED(m_RenderDeviceD3D11->GetDeviceD3D11()->CreateQuery(&disjointQueryDesc, &m_DisjointQueries[i])))
			{
				Log::Error("Failed to create timer query object.");
			}
		}
	}
}

QueryDX11::~QueryDX11()
{}

void QueryDX11::Begin(int64_t frame)
{
	int buffer = (frame - 1L) % m_NumBuffers;
	if (buffer >= 0)
	{
		if (m_QueryType == QueryType::Timer)
		{
			m_RenderDeviceD3D11->GetDeviceContextD3D11()->Begin(m_DisjointQueries[buffer]);
			m_RenderDeviceD3D11->GetDeviceContextD3D11()->End(m_Queries[0][buffer]);
		}
		else
		{
			m_RenderDeviceD3D11->GetDeviceContextD3D11()->Begin(m_Queries[0][buffer]);
		}
	}
}

void QueryDX11::End(int64_t frame)
{
	int buffer = (frame - 1L) % m_NumBuffers;
	if (buffer >= 0)
	{
		if (m_QueryType == QueryType::Timer)
		{
			m_RenderDeviceD3D11->GetDeviceContextD3D11()->End(m_Queries[1][buffer]);
			m_RenderDeviceD3D11->GetDeviceContextD3D11()->End(m_DisjointQueries[buffer]);
		}
		else
		{
			m_RenderDeviceD3D11->GetDeviceContextD3D11()->End(m_Queries[0][buffer]);
		}
	}

}

bool QueryDX11::QueryResultAvailable(int64_t frame)
{
	bool result = false;
	int buffer = (frame - 1L) % m_NumBuffers;

	if (buffer >= 0)
	{
		if (m_QueryType == QueryType::Timer)
		{
			result = (m_RenderDeviceD3D11->GetDeviceContextD3D11()->GetData(m_DisjointQueries[buffer], nullptr, 0, 0) == S_OK);
		}
		else
		{
			result = (m_RenderDeviceD3D11->GetDeviceContextD3D11()->GetData(m_Queries[0][buffer], nullptr, 0, 0) == S_OK);
		}
	}

	return result;
}

IQuery::QueryResult QueryDX11::GetQueryResult(int64_t frame)
{
	QueryResult result = {};
	int buffer = (frame - 1L) % m_NumBuffers;

	if (buffer >= 0)
	{
		if (m_QueryType == QueryType::Timer)
		{
			while (m_RenderDeviceD3D11->GetDeviceContextD3D11()->GetData(m_DisjointQueries[buffer], nullptr, 0, 0) == S_FALSE)
			{
				Sleep(1L);
			}
			D3D11_QUERY_DATA_TIMESTAMP_DISJOINT timeStampDisjoint;
			m_RenderDeviceD3D11->GetDeviceContextD3D11()->GetData(m_DisjointQueries[buffer], &timeStampDisjoint, sizeof(D3D11_QUERY_DATA_TIMESTAMP_DISJOINT), 0);
			if (timeStampDisjoint.Disjoint == FALSE)
			{
				UINT64 beginTime, endTime;
				if (m_RenderDeviceD3D11->GetDeviceContextD3D11()->GetData(m_Queries[0][buffer], &beginTime, sizeof(UINT64), 0) == S_OK &&
					m_RenderDeviceD3D11->GetDeviceContextD3D11()->GetData(m_Queries[1][buffer], &endTime, sizeof(UINT64), 0) == S_OK)
				{
					result.ElapsedTime = (endTime - beginTime) / double(timeStampDisjoint.Frequency);
					result.IsValid = true;
				}
			}
		}
		else
		{
			// Wait for the results to become available.
			while (m_RenderDeviceD3D11->GetDeviceContextD3D11()->GetData(m_Queries[0][buffer], nullptr, 0, 0))
			{
				Sleep(1L);
			}

			switch (m_QueryType)
			{
			case QueryType::CountSamples:
			{
				UINT64 numSamples = 0;
				if (m_RenderDeviceD3D11->GetDeviceContextD3D11()->GetData(m_Queries[0][buffer], &numSamples, sizeof(UINT64), 0) == S_OK)
				{
					result.NumSamples = numSamples;
					result.IsValid = true;
				}
			}
			break;
			case IQuery::QueryType::CountSamplesPredicate:
			{
				BOOL anySamples = FALSE;
				if (m_RenderDeviceD3D11->GetDeviceContextD3D11()->GetData(m_Queries[0][buffer], &anySamples, sizeof(UINT64), 0) == S_OK)
				{
					result.AnySamples = anySamples == TRUE;
					result.IsValid = true;
				}
			}
			break;
			case IQuery::QueryType::CountPrimitives:
			case IQuery::QueryType::CountTransformFeedbackPrimitives:
			{
				D3D11_QUERY_DATA_SO_STATISTICS streamOutStats = {};
				if (m_RenderDeviceD3D11->GetDeviceContextD3D11()->GetData(m_Queries[0][buffer], &streamOutStats, sizeof(D3D11_QUERY_DATA_SO_STATISTICS), 0) == S_OK)
				{
					result.PrimitivesGenerated = result.TransformFeedbackPrimitives = streamOutStats.NumPrimitivesWritten;
					result.IsValid = true;
				}
			}
			break;
			default:
				break;
			}
		}
	}

	return result;
}

uint8_t QueryDX11::GetBufferCount() const
{
	return m_NumBuffers;
}
