#include "stdafx.h"

// General
#include "ModelBase.h"

ModelBase::ModelBase()
    : m_Name("ModelBase")
{}

ModelBase::~ModelBase()
{}

void ModelBase::SetName(const std::string& Name)
{
	m_Name = Name;
}

std::string ModelBase::GetName() const
{
	return m_Name;
}

void ModelBase::SetBounds(const BoundingBox& Bounds)
{
	// TODO:
	//m_Geometry->SetBounds(Bounds);
}

BoundingBox ModelBase::GetBounds() const
{
	// TODO:
	return BoundingBox();
}

void ModelBase::AddConnection(const std::shared_ptr<IMaterial>& Material, const std::shared_ptr<IGeometry>& Geometry, SGeometryDrawArgs GeometryDrawArgs)
{
	SConnection connection;
	connection.Material = Material;
	connection.Geometry = Geometry;
	connection.GeometryDrawArgs = GeometryDrawArgs;
	m_Connections.push_back(connection);
}

const std::vector<IModel::SConnection>& ModelBase::GetConnections() const
{
	return m_Connections;
}

void ModelBase::Accept(IVisitor* visitor)
{
	visitor->Visit(this);

	for (const auto& connection : m_Connections)
		connection.Geometry->Accept(visitor, connection.Material.get(), connection.GeometryDrawArgs);
}