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

void ModelBase::Load(const std::shared_ptr<IByteBuffer>& ByteBuffer)
{
	size_t connectionsCount = m_Connections.size();
	ByteBuffer->read(&connectionsCount);

	for (size_t i = 0; i < connectionsCount; i++)
	{
		auto material = m_RenderDevice.GetBaseManager().GetManager<IMaterialsFactory>()->CreateMaterial("MaterialModel");
		if (auto materialAsLoadableFromFile = std::dynamic_pointer_cast<ILoadableFromFile>(material))
		{
			materialAsLoadableFromFile->Load(ByteBuffer);
		}
		else
			_ASSERT(false);

		auto geometry = m_RenderDevice.GetObjectsFactory().CreateGeometry();
		if (auto geometryAsLoadableFromFile = std::dynamic_pointer_cast<ILoadableFromFile>(geometry))
		{
			geometryAsLoadableFromFile->Load(ByteBuffer);
		}
		else
			_ASSERT(false);

		AddConnection(material, geometry);
	}
}

void ModelBase::Save(const std::shared_ptr<IByteBuffer>& ByteBuffer)
{
	//std::vector<std::shared_ptr<IMaterial>> modelMaterials;
	size_t connectionsCount = m_Connections.size();
	ByteBuffer->write(&connectionsCount);

	for (const auto& it : m_Connections)
	{
		//if (std::find(modelMaterials.begin(), modelMaterials.end(), it.Material) != modelMaterials.end()

		if (auto materialAsLoadableFromFile = std::dynamic_pointer_cast<ILoadableFromFile>(it.Material))
		{
			materialAsLoadableFromFile->Save(ByteBuffer);
		}
		else
			throw CException("ModelBase: Material '%s' is not loadable from file.", it.Material->GetName().c_str());

		if (auto geometryAsLoadableFromFile = std::dynamic_pointer_cast<ILoadableFromFile>(it.Geometry))
		{
			geometryAsLoadableFromFile->Save(ByteBuffer);
		}
		else
			throw CException("ModelBase: Geometry is not loadable from file.");
	}
}
