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



//
// IMesh
//
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

const BoundingBox& MeshProxie::GetBounds() const
{
	return m_Mesh->GetBounds();
}

void MeshProxie::AddVertexBuffer(const BufferBinding & binding, const std::shared_ptr<IBuffer> buffer)
{
	m_Mesh->AddVertexBuffer(binding, buffer);
}

void MeshProxie::SetVertexBuffer(const std::shared_ptr<IBuffer> buffer)
{
	m_Mesh->SetVertexBuffer(buffer);
}

void MeshProxie::SetIndexBuffer(const std::shared_ptr<IBuffer> buffer)
{
	m_Mesh->SetIndexBuffer(buffer);
}

void MeshProxie::SetPrimitiveTopology(PrimitiveTopology _topology)
{
	m_Mesh->SetPrimitiveTopology(_topology);
}

void MeshProxie::SetMaterial(const std::shared_ptr<IMaterial> Material)
{
	m_Mesh->SetMaterial(Material);
}

void MeshProxie::AddMaterial(const std::shared_ptr<IMaterial> Material, SGeometryDrawArgs GeometryDrawArgs)
{
	m_Mesh->AddMaterial(Material, GeometryDrawArgs);
}

IGeometry& MeshProxie::GetGeometry() const
{
	return m_Mesh->GetGeometry();
}

bool MeshProxie::Render(const RenderEventArgs& renderEventArgs, const SGeometryDrawArgs& GeometryDrawArgs) const
{
	return m_Mesh->Render(renderEventArgs, GeometryDrawArgs);
}

bool MeshProxie::Accept(IVisitor* visitor, SGeometryDrawArgs GeometryDrawArgs)
{
	return m_Mesh->Accept(visitor, GeometryDrawArgs);
}
