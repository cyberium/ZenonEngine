#pragma once

#include "../../SceneFunctional/Base/TransformComponentBase.h"

class 
    __declspec(UUID_TransformComponent)
    OW_ENGINE_API
    CTransformComponent3D : public CTransformComponentBase
{
    typedef CTransformComponentBase base;
public:
                    CTransformComponent3D(std::shared_ptr<ISceneNode> OwnerNode);
    virtual         ~CTransformComponent3D();

    void            SetTranslate(cvec3 _translate);
    cvec3           GetTranslation() const;

    void            SetRotation(cvec3 _rotate);
    cvec3           GetRotation() const;

    void            SetRotationQuaternion(cquat _rotate);
    cquat           GetRotationQuaternion() const;

    void            SetScale(cvec3 _scale);
    cvec3           GetScale() const;

protected:
    virtual void    UpdateLocalTransform() override;
    virtual void    UpdateWorldTransform() override;

private:
    vec3            m_Translate;
    vec3            m_Rotate;
    quat            m_RotateQuat;
    bool            m_IsRotateQuat;
    vec3            m_Scale;
};
