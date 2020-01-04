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

void MeshProxie::SetName(const std::string & Name)
{
	m_Mesh->SetName(Name);
}

std::string MeshProxie::GetName() const
{
	return m_Mesh->GetName();
}

void MeshProxie::SetBounds(const BoundingBox & Bounds)
{
	m_Mesh->SetBounds(Bounds);
}

const BoundingBox & MeshProxie::GetBounds() const
{
	return m_Mesh->GetBounds();
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

void MeshProxie::SetMaterial(std::shared_ptr<const IMaterial> Material)
{
	m_Mesh->SetMaterial(Material);
}

void MeshProxie::AddMaterial(std::shared_ptr<const IMaterial> Material, SGeometryPartParams GeometryPartParams)
{
	m_Mesh->AddMaterial(Material, GeometryPartParams);
}

bool MeshProxie::Render(const RenderEventArgs* renderEventArgs, const IConstantBuffer* perObject, SGeometryPartParams GeometryPartParams)
{
	return m_Mesh->Render(renderEventArgs, perObject, GeometryPartParams);
}

bool MeshProxie::Accept(IVisitor* visitor, SGeometryPartParams GeometryPartParams)
{
	return visitor->Visit(this, GeometryPartParams);
}
