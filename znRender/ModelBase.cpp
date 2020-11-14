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



//
// Skeleton
//
void ModelBase::ApplyOtherSkeleton(std::shared_ptr<IModel> other)
{
	for (const auto& b : m_Bones)
	{
		auto otherBones = other->GetBones();
		auto otherIt = std::find(otherBones.begin(), otherBones.end(), b);
		if (otherIt == otherBones.end())
			//throw CException("Unable find bone '%s'", b.Name.c_str());
			continue;

		b->MergeWithOther(*otherIt);
	}
}

void ModelBase::AddBone(std::shared_ptr<ISkeletonBone> Bone)
{
	m_Bones.push_back(Bone);
}

std::shared_ptr<ISkeletonBone> ModelBase::GetBone(size_t Index) const
{
	_ASSERT(Index < m_Bones.size());
	return m_Bones.at(Index);
}

size_t ModelBase::GetBoneIndexByName(const std::string& BoneName) const
{
	for (size_t i = 0; i < m_Bones.size(); ++i)
		if (::_stricmp(m_Bones[i]->GetName().c_str(), BoneName.c_str()) == 0)
			return i;
	throw CException("Bone '%s' not found.", BoneName.c_str());
}

std::shared_ptr<ISkeletonBone> ModelBase::GetBoneByName(const std::string& BoneName) const
{
	for (size_t i = 0; i < m_Bones.size(); ++i)
		if (::_stricmp(m_Bones[i]->GetName().c_str(), BoneName.c_str()) == 0)
			return m_Bones[i];
	throw CException("Bone '%s' not found.", BoneName.c_str());
}

const std::vector<std::shared_ptr<ISkeletonBone>>& ModelBase::GetBones() const
{
	return m_Bones;
}



//
// Animation
//
void ModelBase::AddAnimation(uint16 AnimationId, const std::shared_ptr<IAnimation>& Animation)
{
	m_Animations.insert(std::make_pair(AnimationId, Animation));
}

const Animations_t& ModelBase::GetAnimations() const
{
	return m_Animations;
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

