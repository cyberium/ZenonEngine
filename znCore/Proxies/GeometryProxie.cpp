#include "stdafx.h"

// General
#include "GeometryProxie.h"

GeometryProxie::GeometryProxie(const std::shared_ptr<IGeometry>& Geometry)
	: m_Geometry(Geometry)
{
	_ASSERT(m_Geometry != nullptr);
}

GeometryProxie::~GeometryProxie()
{}



//
// IModel
//
void GeometryProxie::SetBounds(const BoundingBox & Bounds)
{
	m_Geometry->SetBounds(Bounds);
}

BoundingBox GeometryProxie::GetBounds() const
{
	return m_Geometry->GetBounds();
}

void GeometryProxie::AddVertexBuffer(const BufferBinding & Binding, const std::shared_ptr<IBuffer>& VertexBuffer)
{
	m_Geometry->AddVertexBuffer(Binding, VertexBuffer);
}

void GeometryProxie::SetVertexBuffer(const std::shared_ptr<IBuffer>& GlobalVertexBuffer)
{
	m_Geometry->SetVertexBuffer(GlobalVertexBuffer);
}

void GeometryProxie::SetIndexBuffer(const std::shared_ptr<IBuffer>& IndexBuffer)
{
	m_Geometry->SetIndexBuffer(IndexBuffer);
}

void GeometryProxie::SetPrimitiveTopology(PrimitiveTopology Topology)
{
	m_Geometry->SetPrimitiveTopology(Topology);
}

bool GeometryProxie::Render(const RenderEventArgs & RenderEventArgs, const IShader * VertexShader, const SGeometryDrawArgs GeometryDrawArgs) const
{
	return m_Geometry->Render(RenderEventArgs, VertexShader, GeometryDrawArgs);
}

bool GeometryProxie::RenderInstanced(const RenderEventArgs & RenderEventArgs, const IShader * VertexShader, const SGeometryDrawInstancedArgs GeometryDrawInstancedArgs) const
{
	return m_Geometry->RenderInstanced(RenderEventArgs, VertexShader, GeometryDrawInstancedArgs);
}

void GeometryProxie::Accept(IVisitor * visitor, const IMaterial * Material, SGeometryDrawArgs GeometryDrawArgs)
{
	visitor->Visit(this, Material, GeometryDrawArgs);
}
