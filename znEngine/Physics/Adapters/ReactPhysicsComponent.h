#pragma once

#include "reactphysics3d.h"

class __declspec(UUID_PhysicsComponent) CReactPhysicsComponent
	: public CComponentBase
	, public IPhysicsComponent
{
public:
	CReactPhysicsComponent(const ISceneNode3D& OwnerNode, rp3d::RigidBody * RPRigidBody);
	virtual ~CReactPhysicsComponent();

	// CComponentBase
	virtual void Update(const UpdateEventArgs& e) override;

	// IPhysicsComponent
	glm::vec3 GetPhysicsPosition() const override;

private:
	rp3d::RigidBody * m_RPRigidBody;
};