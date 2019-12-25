#pragma once

#include "ComponentBase.h"

// Component messages 0x00000010 - 0x00000019
#define UUID_TransformComponent_OnTranslateChanged      0x00000010
#define UUID_TransformComponent_OnLocalTransformChanged 0x00000011
#define UUID_TransformComponent_OnWorldTransformChanged 0x00000012

class __declspec(UUID_TransformComponent) ZN_API CTransformComponentBase
	: public ITransformComponent
	, public CComponentBase
{
public:
	CTransformComponentBase(std::shared_ptr<ISceneNode> OwnerNode);
    virtual ~CTransformComponentBase();

	// ITransformComponent
	virtual mat4 GetLocalTransform() const;
	virtual mat4 GetInverseLocalTransform() const;
	virtual void SetLocalTransform(cmat4 localTransform);

	virtual mat4 GetWorldTransfom() const;
	virtual mat4 GetInverseWorldTransform() const;
    virtual mat4 GetParentWorldTransform() const;
	virtual void SetWorldTransform(cmat4 worldTransform);

    virtual void ForceRecalculateLocalTransform();

    // ISceneNodeComponent
    virtual void OnParentChanged() override; // update world transform

protected:
    virtual void UpdateLocalTransform() = 0;
    virtual void UpdateWorldTransform() = 0;

protected:
    glm::mat4 m_LocalTransform;
	glm::mat4 m_InverseLocalTransform;
	glm::mat4 m_WorldTransform;
	glm::mat4 m_InverseWorldTransform;
};
