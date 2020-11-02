#include "stdafx.h"

// General
#include "ComponentBase.h"

// Additional
#include "Settings/Properties.h"
#include "Settings/Actions.h"

CComponentBase::CComponentBase(const ISceneNode3D& OwnerNode)
    : Object(OwnerNode.GetScene()->GetBaseManager())
	, m_OwnerNode(OwnerNode)
{
	m_Properties = MakeShared(CPropertiesGroup);
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
// IObjectLoadSave
//
void CComponentBase::Copy(std::shared_ptr<ISceneNodeComponent> Destination) const
{
}

void CComponentBase::Load(const std::shared_ptr<IXMLReader>& Reader)
{
	Object::Load(Reader);
}

void CComponentBase::Save(const std::shared_ptr<IXMLWriter>& Writer) const
{
	Object::Save(Writer);
}


//
// Protected
//
const ISceneNode3D& CComponentBase::GetOwnerNode() const
{
	return m_OwnerNode;
}

IBaseManager& CComponentBase::GetBaseManager() const
{
	return GetOwnerNode().GetScene()->GetBaseManager();
}

void CComponentBase::RaiseComponentMessage(ComponentMessageType Message)
{
	m_OwnerNode.RaiseComponentMessage(this, Message);
}
