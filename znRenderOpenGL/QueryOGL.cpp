#include <stdafx.h>

#include "QueryOGL.h"

GLenum GLTranslateQueryType(Query::QueryType _type)
{
	GLenum result = 0;

	switch (_type)
	{
	case Query::QueryType::Timer:
		return GL_TIME_ELAPSED;
	case Query::QueryType::CountSamples:
		return GL_SAMPLES_PASSED;
	case Query::QueryType::CountSamplesPredicate:
#ifdef _DEBUG
		return GL_ANY_SAMPLES_PASSED;
#else
		return GL_ANY_SAMPLES_PASSED_CONSERVATIVE;
#endif
	case Query::QueryType::CountPrimitives:
		return GL_PRIMITIVES_GENERATED;
	case Query::QueryType::CountTransformFeedbackPrimitives:
		return GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN;
	default:
		std::exception("Unknown QueryType");
	}

	return result;
}

// https://www.khronos.org/opengl/wiki/Query_Object

QueryOGL::QueryOGL(QueryType queryType, uint8_t numBuffers)
	: m_QueryType(queryType)
	, m_NumBuffers(numBuffers)
{
	m_QueryTypeGL = GLTranslateQueryType(m_QueryType);

	m_Queries.resize(m_NumBuffers);
	for (uint8_t i = 0; i < m_NumBuffers; ++i)
	{
		glGenQueries(1, &m_Queries[i]);
	}
}

QueryOGL::~QueryOGL()
{
	for (uint8_t i = 0; i < m_NumBuffers; ++i)
	{
		if (m_Queries[i] != 0)
		{
			glDeleteQueries(1, &m_Queries[i]);
			m_Queries[i] = 0;
		}
	}
}

void QueryOGL::Begin(int64_t frame)
{
	int buffer = (frame - 1LL) % m_NumBuffers;
	if (buffer >= 0)
	{
		glBeginQuery(m_QueryTypeGL, m_Queries[buffer]);
	}
}

void QueryOGL::End(int64_t frame)
{
	int buffer = (frame - 1LL) % m_NumBuffers;
	if (buffer >= 0)
	{
#ifdef _DEBUG
		GLint activeQueryObj;
		glGetQueryiv(m_QueryTypeGL, GL_CURRENT_QUERY, &activeQueryObj);
		assert1(activeQueryObj == m_Queries[buffer]);
#endif
		glEndQuery(m_QueryTypeGL);
	}
}

bool QueryOGL::QueryResultAvailable(int64_t frame)
{
	bool result = false;
	int buffer = (frame - 1LL) % m_NumBuffers;

	if (buffer >= 0)
	{
		GLint qResult;
		glGetQueryObjectiv(m_Queries[buffer], GL_QUERY_RESULT_AVAILABLE, &qResult);
		result = (qResult == GL_TRUE);
	}

	return result;
}

Query::QueryResult QueryOGL::GetQueryResult(int64_t frame)
{
	QueryResult result = {};
	int buffer = (frame - 1LL) % m_NumBuffers;

	if (buffer >= 0)
	{
		// Wait results
		GLint qResultAvaliable;
		do
		{
			glGetQueryObjectiv(m_Queries[buffer], GL_QUERY_RESULT_AVAILABLE, &qResultAvaliable);
		} while (qResultAvaliable == GL_FALSE);

		// Get result
		GLuint64 qResult = 0;
		glGetQueryObjectui64v(m_Queries[buffer], GL_QUERY_RESULT, &qResult);

		switch (m_QueryType)
		{
		case Query::QueryType::Timer:
			result.ElapsedTime = static_cast<double>(qResult);
			result.IsValid = true;
		break;

		case Query::QueryType::CountSamples:
			result.NumSamples = qResult;
			result.IsValid = true;
		break;

		case Query::QueryType::CountSamplesPredicate:
			result.AnySamples = (qResult > 0);
			result.IsValid = true;
		break;

		case Query::QueryType::CountPrimitives:
			result.PrimitivesGenerated = qResult;
			result.IsValid = true;
		break;

		case Query::QueryType::CountTransformFeedbackPrimitives:
			result.TransformFeedbackPrimitives = qResult;
			result.IsValid = true;
		break;

		default:
			std::exception("Unknown QueryType");
		}
	}

	return result;
}

uint8_t QueryOGL::GetBufferCount() const
{
	return m_NumBuffers;
}
