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

const BoundingBox& GeometryBase::GetBounds() const
{
	return m_Bounds;
}

void GeometryBase::AddVertexBuffer(const BufferBinding& binding, std::shared_ptr<IBuffer> buffer)
{
    m_VertexBuffers[binding] = buffer;
}

void GeometryBase::SetVertexBuffer(std::shared_ptr<IBuffer> buffer)
{
    m_VertexBuffer = buffer;
}

void GeometryBase::SetIndexBuffer(std::shared_ptr<IBuffer> buffer)
{
    m_pIndexBuffer = buffer;
}
