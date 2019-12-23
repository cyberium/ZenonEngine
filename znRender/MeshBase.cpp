#include "stdafx.h"

// General
#include "MeshBase.h"

MeshBase::MeshBase()
    : m_Name("MeshBase")
	, m_VertexBuffer(nullptr)
    , m_pIndexBuffer(nullptr)
    , m_pMaterial(nullptr)
{
}

MeshBase::~MeshBase()
{
}

void MeshBase::SetName(const std::string& Name)
{
	m_Name = Name;
}

std::string MeshBase::GetName() const
{
	return m_Name;
}

void MeshBase::SetBounds(const BoundingBox& Bounds)
{
	m_Bounds = Bounds;
}

const BoundingBox& MeshBase::GetBounds() const
{
	return m_Bounds;
}

void MeshBase::AddVertexBuffer(const BufferBinding& binding, std::shared_ptr<IBuffer> buffer)
{
    m_VertexBuffers[binding] = buffer;
}

void MeshBase::SetVertexBuffer(std::shared_ptr<IBuffer> buffer)
{
    m_VertexBuffer = buffer;
}

void MeshBase::SetIndexBuffer(std::shared_ptr<IBuffer> buffer)
{
    m_pIndexBuffer = buffer;
}

void MeshBase::SetMaterial(std::shared_ptr<const IMaterial> material)
{
    m_pMaterial = material;
}

std::shared_ptr<const IMaterial> MeshBase::GetMaterial() const
{
    return m_pMaterial;
}

bool MeshBase::Accept(IVisitor* visitor, UINT indexStartLocation, UINT indexCnt, UINT vertexStartLocation, UINT vertexCnt)
{
    return visitor->Visit(this, indexStartLocation, indexCnt, vertexStartLocation, vertexCnt);
}