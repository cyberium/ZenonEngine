#include "stdafx.h"

// General
#include "MeshProxie.h"

MeshProxie::MeshProxie(std::shared_ptr<IMesh> _mesh) 
	: m_Mesh(_mesh)
{
	_ASSERT(m_Mesh);
}

MeshProxie::~MeshProxie()
{
}

void MeshProxie::AddVertexBuffer(const BufferBinding & binding, std::shared_ptr<IBuffer> buffer)
{
	m_Mesh->AddVertexBuffer(binding, buffer);
}

void MeshProxie::SetVertexBuffer(std::shared_ptr<IBuffer> buffer)
{
	m_Mesh->SetVertexBuffer(buffer);
}

void MeshProxie::SetIndexBuffer(std::shared_ptr<IBuffer> buffer)
{
	m_Mesh->SetIndexBuffer(buffer);
}

void MeshProxie::SetPrimitiveTopology(PrimitiveTopology _topology)
{
	m_Mesh->SetPrimitiveTopology(_topology);
}

void MeshProxie::SetMaterial(std::shared_ptr<const IMaterial> material)
{
	m_Mesh->SetMaterial(material);
}

std::shared_ptr<const IMaterial> MeshProxie::GetMaterial() const
{
	return m_Mesh->GetMaterial();
}

bool MeshProxie::Render(const RenderEventArgs* renderEventArgs, const IConstantBuffer* perObject, UINT indexStartLocation, UINT indexCnt, UINT vertexStartLocation, UINT vertexCnt)
{
	return m_Mesh->Render(renderEventArgs, perObject, indexStartLocation, indexCnt, vertexStartLocation, vertexCnt);
}

bool MeshProxie::Accept(IVisitor* visitor, UINT indexStartLocation, UINT indexCnt, UINT vertexStartLocation, UINT vertexCnt)
{
	return visitor->Visit(this, indexStartLocation, indexCnt, vertexStartLocation, vertexCnt);
}
