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
	m_BoundingBox = Bounds;
}

BoundingBox ModelBase::GetBounds() const
{
	return m_BoundingBox;
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

void ModelBase::SetFileName(const std::string & FileName)
{
	m_FileName = FileName;
}

std::string ModelBase::GetFileName() const
{
	return m_FileName;
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
// IObjectLoadSave
//
void ModelBase::Load(const std::shared_ptr<IByteBuffer>& ByteBuffer)
{
	if (auto file = std::dynamic_pointer_cast<IFile>(ByteBuffer))
		SetFileName(file->Path_Name());

	BoundingBox bounds;
	bounds.Load(ByteBuffer);
	m_BoundingBox = bounds;

	size_t connectionsCount = m_Connections.size();
	ByteBuffer->read(&connectionsCount);

	for (size_t i = 0; i < connectionsCount; i++)
	{
		auto material = m_RenderDevice.GetBaseManager().GetManager<IMaterialsFactory>()->CreateMaterial("MaterialModel");
		if (auto materialAsLoadableFromFile = std::dynamic_pointer_cast<IObjectLoadSave>(material))
		{
			materialAsLoadableFromFile->Load(ByteBuffer);
		}
		else
			_ASSERT(false);

		auto geometry = m_RenderDevice.GetObjectsFactory().CreateGeometry();
		if (auto geometryAsLoadableFromFile = std::dynamic_pointer_cast<IObjectLoadSave>(geometry))
		{
			geometryAsLoadableFromFile->Load(ByteBuffer);
		}
		else
			_ASSERT(false);

		SGeometryDrawArgs geometryDrawArgs;
		ByteBuffer->read(&geometryDrawArgs);

		AddConnection(material, geometry, geometryDrawArgs);
	}
}

void ModelBase::Save(const std::shared_ptr<IByteBuffer>& ByteBuffer) const
{
	m_BoundingBox.Save(ByteBuffer);

	//std::vector<std::shared_ptr<IMaterial>> modelMaterials;
	size_t connectionsCount = m_Connections.size();
	ByteBuffer->write(&connectionsCount);

	for (const auto& it : m_Connections)
	{
		//if (std::find(modelMaterials.begin(), modelMaterials.end(), it.Material) != modelMaterials.end()

		if (auto materialAsLoadableFromFile = std::dynamic_pointer_cast<IObjectLoadSave>(it.Material))
		{
			materialAsLoadableFromFile->Save(ByteBuffer);
		}
		else
			throw CException("ModelBase: Material '%s' is not loadable from file.", it.Material->GetName().c_str());

		if (auto geometryAsLoadableFromFile = std::dynamic_pointer_cast<IObjectLoadSave>(it.Geometry))
		{
			geometryAsLoadableFromFile->Save(ByteBuffer);
		}
		else
			throw CException("ModelBase: Geometry is not loadable from file.");

		ByteBuffer->write(&it.GeometryDrawArgs);
	}
}

void ModelBase::Load(const std::shared_ptr<IXMLReader>& Reader)
{
	if (auto fileNameReader = Reader->GetChild("FileName"))
		m_FileName = fileNameReader->GetValue();

	// TODO: Replace with models maanger
	auto modelFile = m_RenderDevice.GetBaseManager().GetManager<IFilesManager>()->Open(m_FileName);
	if (modelFile)
		Load(modelFile);
}

void ModelBase::Save(const std::shared_ptr<IXMLWriter>& Writer) const
{
	// TODO: Replace with models maanger
	auto file = m_RenderDevice.GetBaseManager().GetManager<IFilesManager>()->Open(m_FileName);
	if (false == m_FileName.empty() && file == nullptr)
	{
		file = std::make_shared<CFile>(m_FileName);
		Save(file);
		m_RenderDevice.GetBaseManager().GetManager<IFilesManager>()->GetFilesStorage("PCEveryFileAccess")->SaveFile(file);
	}

	CXMLManager xml;
	auto modelWriter = xml.CreateWriter("Model");

	auto fileNameWriter = xml.CreateWriter("FileName");
	fileNameWriter->SetValue(m_FileName);
	modelWriter->AddChild(fileNameWriter);

	Writer->AddChild(modelWriter);
}
