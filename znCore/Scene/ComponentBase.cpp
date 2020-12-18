#include "stdafx.h"

// General
#include "ComponentBase.h"

CComponentBase::CComponentBase(const ISceneNode& OwnerNode)
    : Object(OwnerNode.GetScene().GetBaseManager())
	, m_OwnerNode(OwnerNode)
{
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

void CComponentBase::RaiseComponentMessage(ComponentMessageType Message)
{
	GetOwnerNode().RaiseComponentMessage(this, Message);
}
