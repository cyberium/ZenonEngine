#include "stdafx.h"

// General
#include "MeshBase.h"

MeshBase::MeshBase()
    : m_Name("MeshBase")
{}

MeshBase::~MeshBase()
{}

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
	m_Geometry->SetBounds(Bounds);
}

const BoundingBox& MeshBase::GetBounds() const
{
	return m_Geometry->GetBounds();
}

void MeshBase::AddVertexBuffer(const BufferBinding& binding, std::shared_ptr<IBuffer> buffer)
{
	m_Geometry->AddVertexBuffer(binding, buffer);
}

void MeshBase::SetVertexBuffer(std::shared_ptr<IBuffer> buffer)
{
	m_Geometry->SetVertexBuffer(buffer);
}

void MeshBase::SetIndexBuffer(std::shared_ptr<IBuffer> buffer)
{
	m_Geometry->SetIndexBuffer(buffer);
}

void MeshBase::SetMaterial(const std::shared_ptr<IMaterial> Material)
{
	if (m_MaterialForGeometryParts.empty())
	{
		SRenderGeometryArgs renderGeometryArgs;
		renderGeometryArgs.Material = Material;
		m_MaterialForGeometryParts.push_back(renderGeometryArgs);
	}
	else
	{
		SRenderGeometryArgs renderGeometryArgs;
		renderGeometryArgs.Material = Material;
		m_MaterialForGeometryParts[0] = renderGeometryArgs;
	}
}

void MeshBase::AddMaterial(const std::shared_ptr<IMaterial> Material, SGeometryDrawArgs GeometryDrawArgs)
{
	SRenderGeometryArgs renderGeometryArgs;
	renderGeometryArgs.Material = Material;
	renderGeometryArgs.GeometryDrawArgs = GeometryDrawArgs;
	m_MaterialForGeometryParts.push_back(renderGeometryArgs);
}

IGeometry& MeshBase::GetGeometry() const
{
	return *m_Geometry;
}

bool MeshBase::Accept(IVisitor* visitor, SGeometryDrawArgs GeometryDrawArgs = SGeometryDrawArgs())
{
	bool visitResult = visitor->Visit(this, GeometryDrawArgs);

	for (const auto& materialForGeometryPart : m_MaterialForGeometryParts)
	{
		const IMaterial* material = materialForGeometryPart.Material.get();

		visitResult = m_Geometry->Accept(visitor, material, ((! GeometryDrawArgs.IsDefault()) ? GeometryDrawArgs : materialForGeometryPart.GeometryDrawArgs));
	}

    return visitResult;
}