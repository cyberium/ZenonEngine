#pragma once

#pragma warning(push, 1)
#include <reactphysics3d/reactphysics3d.h>
#pragma warning(pop)

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