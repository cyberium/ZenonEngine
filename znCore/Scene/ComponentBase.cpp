#include "stdafx.h"

// General
#include "ComponentBase.h"

CComponentBase::CComponentBase(const ISceneNode& OwnerNode)
    : Object(OwnerNode.GetScene().GetBaseManager())
	, m_OwnerNode(OwnerNode)
{
	m_Properties = MakeShared(CPropertiesGroup, "ComponentBaseProperties", "descr");
}

CComponentBase::~CComponentBase()
{
}



//
// ISceneNodeComponent
//
void CComponentBase::OnMessage(const ISceneNodeComponent* FromComponent, ComponentMessageType Message)
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
void CComponentBase::CopyTo(std::shared_ptr<IObject> Destination) const
{
}

void CComponentBase::Load(const std::shared_ptr<IXMLReader>& Reader)
{
}

void CComponentBase::Save(const std::shared_ptr<IXMLWriter>& Writer) const
{
}



//
// Protected
//
const ISceneNode& CComponentBase::GetOwnerNode() const
{
	return m_OwnerNode;
}

IBaseManager& CComponentBase::GetBaseManager() const
{
	return GetOwnerNode().GetScene().GetBaseManager();
}

void CComponentBase::RaiseComponentMessage(ComponentMessageType Message)
{
	GetOwnerNode().RaiseComponentMessage(this, Message);
}
