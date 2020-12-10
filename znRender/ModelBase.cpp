#include "stdafx.h"

// General
#include "ModelBase.h"


ModelBase::ModelBase(IRenderDevice& RenderDevice)
	: Object(RenderDevice.GetBaseManager())
	, m_RenderDevice(RenderDevice)
{
	SetName("ModelBase");
}

ModelBase::~ModelBase()
{}



//
// IModel
//
std::string ModelBase::GetFileName() const
{
	return m_FileName;
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

		if (false == newBBox.IsInfinite())
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
const std::shared_ptr<ISkeleton>& ModelBase::GetSkeleton() const
{
	return m_Skeleton;
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
// IModelInternal
//
void ModelBase::SetFileName(const std::string & FileName)
{
	m_FileName = FileName;
}

void ModelBase::SetBounds(const BoundingBox& Bounds)
{
	m_BoundingBox = Bounds;
}

void ModelBase::SetSkeleton(std::shared_ptr<ISkeleton> Skeleton)
{
	m_Skeleton = Skeleton;
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

