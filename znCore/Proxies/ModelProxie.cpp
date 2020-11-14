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
void ModelProxie::SetBounds(const BoundingBox & Bounds)
{
	m_Model->SetBounds(Bounds);
}

BoundingBox ModelProxie::GetBounds() const
{
	return m_Model->GetBounds();
}

void ModelProxie::SetFileName(const std::string & FileName)
{
	m_Model->SetFileName(FileName);
}

std::string ModelProxie::GetFileName() const
{
	return m_Model->GetFileName();
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
void ModelProxie::ApplyOtherSkeleton(std::shared_ptr<IModel> other)
{
	m_Model->ApplyOtherSkeleton(other);
}

void ModelProxie::AddBone(std::shared_ptr<ISkeletonBone> Bone)
{
	m_Model->AddBone(Bone);
}

std::shared_ptr<ISkeletonBone> ModelProxie::GetBone(size_t Index) const
{
	return m_Model->GetBone(Index);
}

size_t ModelProxie::GetBoneIndexByName(const std::string& BoneName) const
{
	return m_Model->GetBoneIndexByName(BoneName);
}

std::shared_ptr<ISkeletonBone> ModelProxie::GetBoneByName(const std::string& BoneName) const
{
	return m_Model->GetBoneByName(BoneName);
}

const std::vector<std::shared_ptr<ISkeletonBone>>& ModelProxie::GetBones() const
{
	return m_Model->GetBones();
}



//
// Animation
//
void ModelProxie::AddAnimation(uint16 AnimationId, const std::shared_ptr<IAnimation>& Animation)
{
	m_Model->AddAnimation(AnimationId, Animation);
}

const Animations_t& ModelProxie::GetAnimations() const
{
	return m_Model->GetAnimations();
}



bool ModelProxie::Render() const
{
	return m_Model->Render();
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
