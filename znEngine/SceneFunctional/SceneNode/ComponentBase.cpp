#include "stdafx.h"

// Include
#include "SceneFunctional\\SceneNode3D.h"

// General
#include "ComponentBase.h"

CComponentBase::CComponentBase(std::shared_ptr<SceneNode3D> OwnerNode)
    : m_OwnerNode(OwnerNode)
{
}

CComponentBase::~CComponentBase()
{
}

std::shared_ptr<SceneNode3D> CComponentBase::GetOwnerNode() const
{
    return m_OwnerNode.lock();
}



//
// ISceneNodeComponent
//
void CComponentBase::OnParentChanged()
{
}

bool CComponentBase::Accept(IVisitor & visitor)
{
    return false;
}
