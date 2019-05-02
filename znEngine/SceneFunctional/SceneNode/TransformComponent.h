#pragma once

#include "ComponentBase.h"

#define UUID_TransformComponent uuid("D0E12217-352E-4B51-AE46-EC287CA9A19C")

// Component messages 0x00000010 - 0x00000019
#define UUID_TransformComponent_OnLocalTransformChanged 0x00000010
#define UUID_TransformComponent_OnWorldTransformChanged 0x00000011

class 
    __declspec(UUID_TransformComponent)
    CTransformComponent : public CComponentBase
{
    typedef CComponentBase base;
public:
                    CTransformComponent(std::shared_ptr<SceneNode3D> OwnerNode);
    virtual         ~CTransformComponent();

    void            SetTranslate(cvec3 _translate);
    cvec3           GetTranslation() const;

    void            SetRotation(cvec3 _rotate);
    cvec3           GetRotation() const;

    void            SetRotationQuaternion(cquat _rotate);
    cquat           GetRotationQuaternion() const;

    void            SetScale(cvec3 _scale);
    cvec3           GetScale() const;

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
    virtual void    UpdateLocalTransform();
    virtual void    UpdateWorldTransform();

private:
    mat4            m_LocalTransform;
    mat4            m_InverseLocalTransform;

    mat4            m_WorldTransform;
    mat4            m_InverseWorldTransform;

    vec3            m_Translate;
    vec3            m_Rotate;
    quat            m_RotateQuat;
    bool            m_IsRotateQuat;
    vec3            m_Scale;
};
