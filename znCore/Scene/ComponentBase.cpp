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



void CComponentBase::Copy(std::shared_ptr<ISceneNodeComponent> Destination) const
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
void CComponentBase::Load(const std::shared_ptr<IByteBuffer>& Buffer)
{
}

void CComponentBase::Save(const std::shared_ptr<IByteBuffer>& Buffer) const
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
const ISceneNode3D& CComponentBase::GetOwnerNode() const
{
	return m_OwnerNode;
}

void CComponentBase::RaiseComponentMessage(ComponentMessageType Message)
{
	m_OwnerNode.RaiseComponentMessage(this, Message);
}
