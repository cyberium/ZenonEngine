#include "stdafx.h"

// General
#include "GeometryProxie.h"

GeometryProxie::GeometryProxie(const std::shared_ptr<IGeometry>& Geometry)
	: m_Geometry(Geometry)
{
	_ASSERT(m_Geometry != nullptr);

	m_GeometryInternal = std::dynamic_pointer_cast<IGeometryInternal>(m_Geometry);
	_ASSERT(m_GeometryInternal != nullptr);
}

GeometryProxie::~GeometryProxie()
{}


//
// IGeometry
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

PrimitiveTopology GeometryProxie::GetPrimitiveTopology() const
{
	return m_Geometry->GetPrimitiveTopology();
}

void GeometryProxie::Render(const IShader * VertexShader, const SGeometryDrawArgs GeometryDrawArgs) const
{
	m_Geometry->Render(VertexShader, GeometryDrawArgs);
}

void GeometryProxie::Accept(IVisitor * visitor, const IMaterial * Material, SGeometryDrawArgs GeometryDrawArgs)
{
	visitor->Visit(this, Material, GeometryDrawArgs);
}


//
// IGeometryInternal
//
void GeometryProxie::Render_BindAllBuffers(const IShader * VertexShader) const
{
	m_GeometryInternal->Render_BindAllBuffers(VertexShader);
}

void GeometryProxie::Render_Draw(const SGeometryDrawArgs GeometryDrawArgs) const
{
	m_GeometryInternal->Render_Draw(GeometryDrawArgs);
}

void GeometryProxie::Render_UnbindAllBuffers(const IShader * VertexShader) const
{
	m_GeometryInternal->Render_UnbindAllBuffers(VertexShader);
}


//
// IObjectLoadSave
//
void GeometryProxie::Load(const std::shared_ptr<IByteBuffer>& ByteBuffer)
{
	if (const auto& loadableFromFile = std::dynamic_pointer_cast<IObjectLoadSave>(m_Geometry))
	{
		loadableFromFile->Load(ByteBuffer);
	}
	else
		_ASSERT(false);
}

void GeometryProxie::Save(const std::shared_ptr<IByteBuffer>& ByteBuffer) const
{
	if (const auto& loadableFromFile = std::dynamic_pointer_cast<IObjectLoadSave>(m_Geometry))
	{
		loadableFromFile->Save(ByteBuffer);
	}
	else
		_ASSERT(false);
}

void GeometryProxie::Load(const std::shared_ptr<IXMLReader>& Reader)
{
	_ASSERT(false);
}

void GeometryProxie::Save(const std::shared_ptr<IXMLWriter>& Writer) const
{
	_ASSERT(false);
}
