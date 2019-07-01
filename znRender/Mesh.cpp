#include "stdafx.h"

// General
#include "Mesh.h"

IMesh::IMesh()
    : m_VertexBuffer(nullptr)
    , m_pIndexBuffer(nullptr)
    , m_pMaterial(nullptr)
{
}

IMesh::~IMesh()
{
}

void IMesh::AddVertexBuffer(const BufferBinding& binding, std::shared_ptr<IBuffer> buffer)
{
    m_VertexBuffers[binding] = buffer;
}

void IMesh::SetVertexBuffer(std::shared_ptr<IBuffer> buffer)
{
    m_VertexBuffer = buffer;
}

void IMesh::SetIndexBuffer(std::shared_ptr<IBuffer> buffer)
{
    m_pIndexBuffer = buffer;
}

void IMesh::SetMaterial(std::shared_ptr<const Material> material)
{
    m_pMaterial = material;
}

std::shared_ptr<const Material> IMesh::GetMaterial() const
{
    return m_pMaterial;
}

bool IMesh::Accept(std::shared_ptr<IVisitor> visitor, UINT indexStartLocation, UINT indexCnt, UINT vertexStartLocation, UINT vertexCnt)
{
    return visitor->Visit(shared_from_this(), indexStartLocation, indexCnt, vertexStartLocation, vertexCnt);
}