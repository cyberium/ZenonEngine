#include "stdafx.h"

// General
#include "ReactPhysicsComponent.h"

CReactPhysicsComponent::CReactPhysicsComponent(const ISceneNode3D& OwnerNode, rp3d::RigidBody * RPRigidBody)
	: CComponentBase(OwnerNode)
	, m_RPRigidBody(RPRigidBody)
{
}

CReactPhysicsComponent::~CReactPhysicsComponent()
{
}

void CReactPhysicsComponent::Update(const UpdateEventArgs & e)
{
	ISceneNode3D& sceneNode3D = const_cast<ISceneNode3D&>(GetOwnerNode());
	sceneNode3D.SetTranslate(GetPhysicsPosition());


	const auto& rot = m_RPRigidBody->getTransform().getOrientation();
	//sceneNode3D.SetRotation(glm::vec3(rot.x, rot.y, rot.z));
	sceneNode3D.SetRotationQuaternion(glm::quat(rot.w, rot.x, rot.y, rot.z));
}

glm::vec3 CReactPhysicsComponent::GetPhysicsPosition() const
{
	_ASSERT(m_RPRigidBody != NULL);
	const auto& pos = m_RPRigidBody->getTransform().getPosition();
	return glm::vec3(pos.x, pos.y, pos.z);
}
