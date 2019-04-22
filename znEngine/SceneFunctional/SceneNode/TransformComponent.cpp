#include "stdafx.h"

// Include
#include "SceneFunctional\\SceneNode3D.h"

// General
#include "TransformComponent.h"

CTransformComponent::CTransformComponent(std::shared_ptr<SceneNode3D> OwnerNode)
    : CComponentBase(OwnerNode)
    , m_Translate(vec3())
    , m_Rotate(vec3())
    , m_RotateQuat(quat())
    , m_IsRotateQuat(false)
    , m_Scale(1.0f, 1.0f, 1.0f)

    , m_LocalTransform(1.0f)
    , m_InverseLocalTransform(1.0f)	// This is the inverse of the local -> world transform.
    , m_WorldTransform(1.0f)
    , m_InverseWorldTransform(1.0f)
{
    m_InverseLocalTransform = glm::inverse(m_LocalTransform);
}

CTransformComponent::~CTransformComponent()
{
}


void CTransformComponent::SetTranslate(cvec3 _translate)
{
    m_Translate = _translate;

    UpdateLocalTransform();
}
cvec3 CTransformComponent::GetTranslation() const
{
    return m_Translate;
}


void CTransformComponent::SetRotation(cvec3 _rotate)
{
    m_Rotate = _rotate;

    UpdateLocalTransform();
}
cvec3 CTransformComponent::GetRotation() const
{
    return m_Rotate;
}


void CTransformComponent::SetRotationQuaternion(cquat _rotate)
{
    m_RotateQuat = _rotate;
    m_IsRotateQuat = true;

    UpdateLocalTransform();
}
cquat CTransformComponent::GetRotationQuaternion() const
{
    return m_RotateQuat;
}


void CTransformComponent::SetScale(cvec3 _scale)
{
    m_Scale = _scale;

    UpdateLocalTransform();
}
cvec3 CTransformComponent::GetScale() const
{
    return m_Scale;
}



// Local transform
mat4 CTransformComponent::GetLocalTransform() const
{
    return m_LocalTransform;
}

mat4 CTransformComponent::GetInverseLocalTransform() const
{
    return m_InverseLocalTransform;
}

void CTransformComponent::SetLocalTransform(cmat4 localTransform)
{
    m_LocalTransform = localTransform;
    m_InverseLocalTransform = glm::inverse(localTransform);

    UpdateWorldTransform();
}



// World transform
mat4 CTransformComponent::GetWorldTransfom() const
{
    return m_WorldTransform;
}

mat4 CTransformComponent::GetInverseWorldTransform() const
{
    return m_InverseWorldTransform;
}

mat4 CTransformComponent::GetParentWorldTransform() const
{
    mat4 parentTransform(1.0f);
    if (std::shared_ptr<SceneNode3D> parent = GetOwnerNode())
    {
        if (std::shared_ptr<CTransformComponent> transformComponent = parent->GetComponent<CTransformComponent>())
        {
            parentTransform = transformComponent->GetWorldTransfom();
        }
    }

    return parentTransform;
}

void CTransformComponent::SetWorldTransform(cmat4 worldTransform)
{
    mat4 inverseParentTransform = glm::inverse(GetParentWorldTransform());
    SetLocalTransform(inverseParentTransform * worldTransform);
}

void CTransformComponent::ForceRecalculateLocalTransform()
{
    UpdateLocalTransform();
}



//
// ISceneNodeComponent
//
void CTransformComponent::OnParentChanged()
{
    base::OnParentChanged();

    UpdateWorldTransform();
}



//
// Protected
//

void CTransformComponent::UpdateLocalTransform()
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

    // Don't forget to update world transform
    UpdateWorldTransform();
}

void CTransformComponent::UpdateWorldTransform()
{
    m_WorldTransform = GetParentWorldTransform() * m_LocalTransform;
    m_InverseWorldTransform = glm::inverse(m_WorldTransform);
}
