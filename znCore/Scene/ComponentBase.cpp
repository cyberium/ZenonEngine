#include "stdafx.h"

// General
#include "ComponentBase.h"

// Additional
#include "Properties.h"

CComponentBase::CComponentBase(const ISceneNode3D& OwnerNode)
    : m_OwnerNode(OwnerNode)
{
	m_PropertyGroup = std::make_shared<CPropertiesGroup>();
	OwnerNode.GetProperties()->AddProperty(m_PropertyGroup);
}

CComponentBase::~CComponentBase()
{
}



//
// ISceneNodeComponent
//
void CComponentBase::OnMessage(const ISceneNodeComponent* Component, ComponentMessageType Message)
{
	// do nothing
}

IPropertiesGroup* CComponentBase::GetPropertiesGroup() const
{
	return m_PropertyGroup.get();
}


void CComponentBase::Update(const UpdateEventArgs & e)
{
	// Do nothing...
}

void CComponentBase::Accept(IVisitor* visitor)
{
	// Do nothing...
}



//
// Protected
//
const ISceneNode3D& CComponentBase::GetOwnerNode() const
{
	return m_OwnerNode;
}

void CComponentBase::RaiseComponentMessage(ComponentMessageType Message)
{
	m_OwnerNode.RaiseComponentMessage(this, Message);
}
