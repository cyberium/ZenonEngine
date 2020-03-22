#include "stdafx.h"

// General
#include "ModelProxie.h"

ModelProxie::ModelProxie(const std::shared_ptr<IModel>& Model)
	: m_Model(Model)
{
	_ASSERT(m_Model != nullptr);
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

const std::vector<ModelProxie::SConnection>& ModelProxie::GetConnections() const
{
	return m_Model->GetConnections();
}

bool ModelProxie::Render(const RenderEventArgs& renderEventArgs) const
{
	return m_Model->Render(renderEventArgs);
}

void ModelProxie::Accept(IVisitor* visitor)
{
	EVisitResult visitResult = visitor->Visit(this);

	if (visitResult & EVisitResult::AllowVisitContent)
	{

	}

	if (visitResult & EVisitResult::AllowVisitChilds)
	{
		for (const auto& connection : GetConnections())
		{
			connection.Geometry->Accept(visitor, connection.Material.get(), connection.GeometryDrawArgs);
		}
	}
}
