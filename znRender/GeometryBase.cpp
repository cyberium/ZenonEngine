#include "stdafx.h"

// General
#include "GeometryBase.h"

GeometryBase::GeometryBase()
    : m_VertexBuffer(nullptr)
    , m_pIndexBuffer(nullptr)
{
}

GeometryBase::~GeometryBase()
{
}


void GeometryBase::SetBounds(const BoundingBox& Bounds)
{
	m_Bounds = Bounds;
}

BoundingBox GeometryBase::GetBounds() const
{
	return m_Bounds;
}

void GeometryBase::AddVertexBuffer(const BufferBinding& binding, const std::shared_ptr<IBuffer>& VertexBuffer)
{
    m_VertexBuffers[binding] = VertexBuffer;
}

void GeometryBase::SetVertexBuffer(const std::shared_ptr<IBuffer>& GlobalVertexBuffer)
{
    m_VertexBuffer = GlobalVertexBuffer;
}

void GeometryBase::SetIndexBuffer(const std::shared_ptr<IBuffer>& IndeBuffer)
{
    m_pIndexBuffer = IndeBuffer;
}

void GeometryBase::Accept(IVisitor * visitor, const IMaterial* Material, SGeometryDrawArgs GeometryDrawArgs)
{
	visitor->Visit(this, Material, GeometryDrawArgs);
}



//
// Protected
//

SGeometryDrawArgs GeometryBase::FixGeometryDrawArgs(const SGeometryDrawArgs & GeometryDrawArgs) const
{
	SGeometryDrawArgs fixedArgs;

	fixedArgs.IndexStartLocation = GeometryDrawArgs.IndexStartLocation;
	fixedArgs.IndexCnt = GeometryDrawArgs.IndexCnt;
	if (fixedArgs.IndexCnt == UINT_MAX && m_pIndexBuffer != nullptr)
		fixedArgs.IndexCnt = m_pIndexBuffer->GetElementCount();

	fixedArgs.VertexStartLocation = GeometryDrawArgs.VertexStartLocation;
	fixedArgs.VertexCnt = GeometryDrawArgs.VertexCnt;
	if (fixedArgs.VertexCnt == UINT_MAX)
	{
		if (m_VertexBuffer != nullptr)
		{
			fixedArgs.VertexCnt = m_VertexBuffer->GetElementCount();
		}
		else if (!m_VertexBuffers.empty())
		{
			fixedArgs.VertexCnt = (*m_VertexBuffers.begin()).second->GetElementCount();
		}
		else
		{
			_ASSERT(false);
		}
	}

	fixedArgs.InstanceStartIndex = GeometryDrawArgs.InstanceStartIndex;
	fixedArgs.InstanceCnt = GeometryDrawArgs.InstanceCnt;

	return fixedArgs;
}
