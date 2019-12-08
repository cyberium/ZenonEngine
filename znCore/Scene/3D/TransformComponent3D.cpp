#include "stdafx.h"

// General
#include "TransformComponent3D.h"

CTransformComponent3D::CTransformComponent3D(std::shared_ptr<ISceneNode> OwnerNode)
    : base(OwnerNode)
    , m_Translate(vec3())
    , m_Rotate(vec3())
    , m_RotateQuat(quat())
    , m_IsRotateQuat(false)
    , m_Scale(1.0f, 1.0f, 1.0f)
{}

CTransformComponent3D::~CTransformComponent3D()
{}


void CTransformComponent3D::SetTranslate(cvec3 _translate)
{
    m_Translate = _translate;

    UpdateLocalTransform();

    RaiseComponentMessage(UUID_TransformComponent_OnTranslateChanged);
}
cvec3 CTransformComponent3D::GetTranslation() const
{
    return m_Translate;
}


void CTransformComponent3D::SetRotation(cvec3 _rotate)
{
    m_Rotate = _rotate;

    UpdateLocalTransform();
}
cvec3 CTransformComponent3D::GetRotation() const
{
    return m_Rotate;
}


void CTransformComponent3D::SetRotationQuaternion(cquat _rotate)
{
    m_RotateQuat = _rotate;
    m_IsRotateQuat = true;

    UpdateLocalTransform();
}
cquat CTransformComponent3D::GetRotationQuaternion() const
{
    return m_RotateQuat;
}


void CTransformComponent3D::SetScale(cvec3 _scale)
{
    m_Scale = _scale;

    UpdateLocalTransform();
}
cvec3 CTransformComponent3D::GetScale() const
{
    return m_Scale;
}

//
// Protected
//

void CTransformComponent3D::UpdateLocalTransform()
{
    m_LocalTransform = glm::mat4(1.0f);

    m_LocalTransform = glm::translate(m_LocalTransform, m_Translate);
    if (m_IsRotateQuat)
    {
        m_LocalTransform *= glm::toMat4(m_RotateQuat);
    }
    else
    {
        m_LocalTransform = glm::rotate(m_LocalTransform, m_Rotate.x, glm::vec3(1, 0, 0));
        m_LocalTransform = glm::rotate(m_LocalTransform, m_Rotate.y, glm::vec3(0, 1, 0));
        m_LocalTransform = glm::rotate(m_LocalTransform, m_Rotate.z, glm::vec3(0, 0, 1));
    }
    m_LocalTransform = glm::scale(m_LocalTransform, m_Scale);
    m_InverseLocalTransform = glm::inverse(m_LocalTransform);

    RaiseComponentMessage(UUID_TransformComponent_OnLocalTransformChanged);

    // Don't forget to update world transform
    UpdateWorldTransform();
}

void CTransformComponent3D::UpdateWorldTransform()
{
    m_WorldTransform = GetParentWorldTransform() * m_LocalTransform;
    m_InverseWorldTransform = glm::inverse(m_WorldTransform);

    RaiseComponentMessage(UUID_TransformComponent_OnWorldTransformChanged);
}
