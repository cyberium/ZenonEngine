#include "stdafx.h"

// General
#include "ModelBase.h"

ModelBase::ModelBase(IRenderDevice& RenderDevice)
	: m_RenderDevice(RenderDevice)
	, m_FixMatrix(1.0f)
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
		auto otherIt = std::find_if(otherBones.begin(), otherBones.end(), [b](const std::shared_ptr<ISkeletonBone>& b2) {
			return b2->GetName() == b->GetName() /*&& b2->GetParentIndex() == b->GetParentIndex()*/;
		});
		if (otherIt != otherBones.end())
		{
			b->MergeWithOther(*otherIt);
			//AddBone(other);
		}
		else
		{

		}
			//throw CException("Unable find bone '%s'", b->GetName().c_str());
			//continue;

		
	}
}

void ModelBase::AddBone(std::shared_ptr<ISkeletonBone> Bone)
{
	if (Bone->GetParentIndex() == -1)
	{
		if (m_RootBone != nullptr)
			throw CException("Model: Unable to set '%s' as root bone, because '%s' already root.", Bone->GetName().c_str(), m_RootBone->GetName().c_str());
		m_RootBone = Bone;
		m_FixMatrix = Bone->GetLocalMatrix();
		Log::Green("Model: '%s' is root bone.", m_RootBone->GetName().c_str());
	}

	m_Bones.push_back(Bone);
}

void ModelBase::SetFixSkeleton(const glm::mat4 & Matrix)
{
	m_FixMatrix = Matrix;
}

glm::mat4 ModelBase::GetFixSkeleton() const
{
	return m_FixMatrix;
}

std::shared_ptr<ISkeletonBone> ModelBase::GetRootBone() const
{
	return m_RootBone;
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
void ModelBase::AddAnimation(const std::string& AnimationName, const std::shared_ptr<IAnimation>& Animation)
{
	const auto& it = m_Animations.find(AnimationName);
	if (it != m_Animations.end())
		throw CException("Animation '%s' already exists in model '%s'", AnimationName.c_str(), GetFileName().c_str());
	std::dynamic_pointer_cast<IAnimationInternal>(Animation)->SetName(AnimationName);
	m_Animations.insert(std::make_pair(AnimationName, Animation));
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

