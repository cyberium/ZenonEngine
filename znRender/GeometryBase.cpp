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

size_t GeometryBase::GetHash() const
{
	return 0;
}


void GeometryBase::SetBounds(const BoundingBox& Bounds)
{
	m_Bounds = Bounds;
}

const BoundingBox& GeometryBase::GetBounds() const
{
	return m_Bounds;
}

void GeometryBase::AddVertexBuffer(const BufferBinding& binding, IBuffer* buffer)
{
    m_VertexBuffers[binding] = buffer;
}

void GeometryBase::SetVertexBuffer(IBuffer* buffer)
{
    m_VertexBuffer = buffer;
}

void GeometryBase::SetIndexBuffer(IBuffer* buffer)
{
    m_pIndexBuffer = buffer;
}

bool GeometryBase::Accept(IVisitor * visitor, const IMaterial* Material, SGeometryPartParams GeometryPartParams)
{
	return visitor->Visit(this, Material, GeometryPartParams);
}

void GeometryBase::UpdateHash()
{
	std::string hash = "";
	if (m_pIndexBuffer)
		hash += std::to_string(m_pIndexBuffer->GetElementCount());

	if (m_VertexBuffer)
		hash += std::to_string(m_VertexBuffer->GetElementCount());

	for (const auto& it : m_VertexBuffers)
	{
		hash += it.first.Name;
		hash += std::to_string(it.first.Index);

		if (it.second)
			hash += std::to_string(it.second->GetElementCount());
	}

	std::hash<std::string> hashFunc;
	m_Hash = hashFunc(hash);
}
