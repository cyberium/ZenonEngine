#include "stdafx.h"

// General
#include "ComponentBase.h"

// Additional
#include "Properties.h"
#include "Actions.h"

CComponentBase::CComponentBase(const ISceneNode3D& OwnerNode)
    : m_OwnerNode(OwnerNode)
{
	m_Properties = std::make_shared<CPropertiesGroup>();
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

std::shared_ptr<IPropertiesGroup> CComponentBase::GetProperties() const
{
	return m_Properties;
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
