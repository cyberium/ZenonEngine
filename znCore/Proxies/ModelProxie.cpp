#include "stdafx.h"

// General
#include "ModelProxie.h"

ModelProxie::ModelProxie(const std::shared_ptr<IModel>& Model)
	: CObjectProxy(Model)
	, m_Model(Model)
{
	_ASSERT(m_Model != nullptr);
}

ModelProxie::~ModelProxie()
{}



//
// IModel
//
std::string ModelProxie::GetFileName() const
{
	return m_Model->GetFileName();
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



//
// Skeleton
//
const std::shared_ptr<ISkeleton>& ModelProxie::GetSkeleton() const
{
	return m_Model->GetSkeleton();
}



//
// Animation
//
void ModelProxie::AddAnimation(const std::string& AnimationName, const std::shared_ptr<IAnimation>& Animation)
{
	m_Model->AddAnimation(AnimationName, Animation);
}

const Animations_t& ModelProxie::GetAnimations() const
{
	return m_Model->GetAnimations();
}



bool ModelProxie::Render(const ShaderMap& Shaders) const
{
	return m_Model->Render(Shaders);
}

void ModelProxie::Accept(IVisitor* visitor)
{
	EVisitResult visitResult = visitor->Visit(shared_from_this());

	if (visitResult & EVisitResult::AllowVisitContent)
	{

	}

	if (visitResult & EVisitResult::AllowVisitChilds)
	{
		for (const auto& connection : GetConnections())
		{
			connection.Geometry->Accept(visitor, connection.Material, connection.GeometryDrawArgs);
		}
	}
}



//
// IObjectLoadSave
//
void ModelProxie::Load(const std::shared_ptr<IByteBuffer>& ByteBuffer)
{
	if (const auto& loadableFromFile = std::dynamic_pointer_cast<IObjectLoadSave>(m_Model))
	{
		loadableFromFile->Load(ByteBuffer);
	}
	else
		_ASSERT(false);
}

void ModelProxie::Save(const std::shared_ptr<IByteBuffer>& ByteBuffer) const
{
	if (const auto& loadableFromFile = std::dynamic_pointer_cast<IObjectLoadSave>(m_Model))
	{
		loadableFromFile->Save(ByteBuffer);
	}
	else
		_ASSERT(false);
}

void ModelProxie::Load(const std::shared_ptr<IXMLReader>& Reader)
{
	_ASSERT(false);
}

void ModelProxie::Save(const std::shared_ptr<IXMLWriter>& Writer) const
{
	_ASSERT(false);
}

const std::shared_ptr<IModel> ModelProxie::GetModel() const
{
	return m_Model;
}

const std::shared_ptr<IModelInternal> ModelProxie::GetModelInternal() const
{
	return std::dynamic_pointer_cast<IModelInternal>(m_Model);
}
