#include "stdafx.h"

// General
#include "ModelBase.h"

ModelBase::ModelBase(IRenderDevice& RenderDevice)
	: m_RenderDevice(RenderDevice)
{
	SetName("ModelBase");
}

ModelBase::~ModelBase()
{}

void ModelBase::SetFileName(const std::string & FileName)
{
	m_FileName = FileName;
}

std::string ModelBase::GetFileName() const
{
	return m_FileName;
}

void ModelBase::SetBounds(const BoundingBox& Bounds)
{
	m_BoundingBox = Bounds;
}

BoundingBox ModelBase::GetBounds() const
{
	if (m_BoundingBox.IsInfinite())
	{
		glm::vec3 floatMin(Math::MinFloat);
		glm::vec3 floatMax(Math::MaxFloat);
		BoundingBox newBBox(floatMax, floatMin);

		for (const auto& c : m_Connections)
			newBBox.makeUnion(c.Geometry->GetBounds());

		if (!newBBox.IsInfinite())
			const_cast<ModelBase*>(this)->SetBounds(newBBox);
	}
	return m_BoundingBox;
}

void ModelBase::AddConnection(const std::shared_ptr<IMaterial>& Material, const std::shared_ptr<IGeometry>& Geometry, SGeometryDrawArgs GeometryDrawArgs)
{
	SConnection connection;
	connection.Material = Material;
	connection.Geometry = Geometry;
	connection.GeometryDrawArgs = GeometryDrawArgs;

	UpdateBounds(Geometry);

	m_Connections.push_back(connection);
}

const std::vector<IModel::SConnection>& ModelBase::GetConnections() const
{
	return m_Connections;
}

void ModelBase::Accept(IVisitor* visitor)
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



//
// Private
//
void ModelBase::UpdateBounds(const std::shared_ptr<IGeometry>& Geometry)
{
	auto geomBounds = Geometry->GetBounds();
	if (geomBounds.IsInfinite())
		throw CException("ModelBase: UpdateBounds: Geometry bounds is empty.");

	m_BoundingBox.makeUnion(geomBounds);
}

