#include "stdafx.h"

// General
#include "ComponentBase.h"

// Additional
#include "Properties.h"

CComponentBase::CComponentBase(std::shared_ptr<ISceneNode> OwnerNode)
    : m_OwnerNode(OwnerNode)
{
	m_PropertyGroup = std::make_shared<CPropertiesGroup>();
	OwnerNode->GetProperties()->AddProperty(m_PropertyGroup);
}

CComponentBase::~CComponentBase()
{
}

std::shared_ptr<ISceneNode> CComponentBase::GetOwnerNode() const
{
    return m_OwnerNode.lock();
}



bool CComponentBase::Load(std::shared_ptr<IXMLReader> Reader)
{
	_ASSERT_EXPR(false, L"Not implemented.");
	return false;
}

bool CComponentBase::Save(std::shared_ptr<IXMLWriter> Writer)
{
	//_ASSERT_EXPR(false, L"Not implemented.");
	return false;
}

void CComponentBase::DoUpdate(UpdateEventArgs & e)
{
	// Do nothing...
}

bool CComponentBase::Accept(IVisitor* visitor)
{
    return false;
}


//
// ISceneNodeComponent
//
void CComponentBase::OnParentChanged()
{
    // do nothing
}

void CComponentBase::OnMessage(std::shared_ptr<ISceneNodeComponent> Component, ComponentMessageType Message)
{
    // do nothing
}

std::shared_ptr<IPropertiesGroup> CComponentBase::GetPropertiesGroup() const
{
	return m_PropertyGroup;
}



//
// Protected
//
void CComponentBase::RaiseComponentMessage(ComponentMessageType Message)
{
    std::shared_ptr<ISceneNode> ownerNode = m_OwnerNode.lock();
    _ASSERT(ownerNode != nullptr);

    ownerNode->RaiseComponentMessage(shared_from_this(), Message);
}
