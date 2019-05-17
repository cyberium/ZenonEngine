#include "stdafx.h"

// Include
#include "SceneNode.h"

// General
#include "TransformComponentBase.h"

CTransformComponentBase::CTransformComponentBase(std::shared_ptr<SceneNode> OwnerNode)
    : CComponentBase(OwnerNode)

    , m_LocalTransform(1.0f)
    , m_InverseLocalTransform(1.0f)	// This is the inverse of the local -> world transform.
    , m_WorldTransform(1.0f)
    , m_InverseWorldTransform(1.0f)
{
    m_InverseLocalTransform = glm::inverse(m_LocalTransform);
}

CTransformComponentBase::~CTransformComponentBase()
{
}


// Local transform
mat4 CTransformComponentBase::GetLocalTransform() const
{
    return m_LocalTransform;
}

mat4 CTransformComponentBase::GetInverseLocalTransform() const
{
    return m_InverseLocalTransform;
}

void CTransformComponentBase::SetLocalTransform(cmat4 localTransform)
{
    m_LocalTransform = localTransform;
    m_InverseLocalTransform = glm::inverse(localTransform);

    UpdateWorldTransform();
}



// World transform
mat4 CTransformComponentBase::GetWorldTransfom() const
{
    return m_WorldTransform;
}

mat4 CTransformComponentBase::GetInverseWorldTransform() const
{
    return m_InverseWorldTransform;
}

mat4 CTransformComponentBase::GetParentWorldTransform() const
{
    mat4 parentTransform(1.0f);
    if (std::shared_ptr<SceneNode> parent = GetOwnerNode()->GetParent())
    {
        if (std::shared_ptr<CTransformComponentBase> transformComponent = parent->GetComponent<CTransformComponentBase>())
        {
            parentTransform = transformComponent->GetWorldTransfom();
        }
    }

    return parentTransform;
}

void CTransformComponentBase::SetWorldTransform(cmat4 worldTransform)
{
    mat4 inverseParentTransform = glm::inverse(GetParentWorldTransform());
    SetLocalTransform(inverseParentTransform * worldTransform);
}

void CTransformComponentBase::ForceRecalculateLocalTransform()
{
    UpdateLocalTransform();
}



//
// ISceneNodeComponent
//
void CTransformComponentBase::OnParentChanged()
{
    base::OnParentChanged();

    UpdateWorldTransform();
}
