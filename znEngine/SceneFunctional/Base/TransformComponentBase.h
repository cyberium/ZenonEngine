#pragma once

#include "ComponentBase.h"

#define UUID_TransformComponent uuid("D0E12217-352E-4B51-AE46-EC287CA9A19C")

// Component messages 0x00000010 - 0x00000019
#define UUID_TransformComponent_OnTranslateChanged      0x00000010
#define UUID_TransformComponent_OnLocalTransformChanged 0x00000011
#define UUID_TransformComponent_OnWorldTransformChanged 0x00000012

class 
    __declspec(UUID_TransformComponent)
    OW_ENGINE_API
    CTransformComponentBase 
	: public CComponentBase
{
    typedef CComponentBase base;
public:
                    CTransformComponentBase(std::shared_ptr<ISceneNode> OwnerNode);
    virtual         ~CTransformComponentBase();

    mat4            GetLocalTransform() const;
    mat4            GetInverseLocalTransform() const;
    void            SetLocalTransform(cmat4 localTransform);

    mat4            GetWorldTransfom() const;
    mat4            GetInverseWorldTransform() const;
    virtual mat4    GetParentWorldTransform() const;
    void            SetWorldTransform(cmat4 worldTransform);

    virtual void    ForceRecalculateLocalTransform();

    // ISceneNodeComponent
    virtual void    OnParentChanged() override; // update world transform

protected:
    virtual void    UpdateLocalTransform() = 0;
    virtual void    UpdateWorldTransform() = 0;

protected:
    mat4            m_LocalTransform;
    mat4            m_InverseLocalTransform;

    mat4            m_WorldTransform;
    mat4            m_InverseWorldTransform;
};
