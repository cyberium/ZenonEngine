#include "stdafx.h"

// General
#include "ModelProxie.h"

ModelProxie::ModelProxie(std::shared_ptr<IModel> Model)
	: m_Model(Model)
{
	_ASSERT(m_Model);
}

ModelProxie::~ModelProxie()
{}



//
// IModel
//
void ModelProxie::SetName(const std::string & Name)
{
	m_Model->SetName(Name);
}

std::string ModelProxie::GetName() const
{
	return m_Model->GetName();
}

void ModelProxie::SetBounds(const BoundingBox & Bounds)
{
	m_Model->SetBounds(Bounds);
}

BoundingBox ModelProxie::GetBounds() const
{
	return m_Model->GetBounds();
}

void ModelProxie::AddConnection(const std::shared_ptr<IMaterial>& Material, const std::shared_ptr<IGeometry>& Geometry, SGeometryDrawArgs GeometryDrawArgs)
{
	m_Model->AddConnection(Material, Geometry, GeometryDrawArgs);
}

bool ModelProxie::Render(const RenderEventArgs& renderEventArgs) const
{
	return m_Model->Render(renderEventArgs);
}

bool ModelProxie::Accept(IVisitor* visitor)
{
	return m_Model->Accept(visitor);
}
