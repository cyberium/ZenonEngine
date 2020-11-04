#pragma once

#include <reactphysics3d.h>

class CReactPhysicsComponent
	: public CComponentBase
	, public IPhysicsComponent
{
public:
	CReactPhysicsComponent(const ISceneNode& OwnerNode, rp3d::RigidBody * RPRigidBody);
	virtual ~CReactPhysicsComponent();

	// CComponentBase
	virtual void Update(const UpdateEventArgs& e) override;

	// IPhysicsComponent
	glm::vec3 GetPhysicsPosition() const override;

private:
	rp3d::RigidBody * m_RPRigidBody;
};