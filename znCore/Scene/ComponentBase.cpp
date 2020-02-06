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

const ISceneNode3D& CComponentBase::GetOwnerNode() const
{
    return m_OwnerNode;
}



//
// ISceneNodeComponent
//
void CComponentBase::OnParentChanged()
{
	// do nothing
}

void CComponentBase::OnMessage(ISceneNodeComponent* Component, ComponentMessageType Message)
{
	// do nothing
}

IPropertiesGroup* CComponentBase::GetPropertiesGroup() const
{
	return m_PropertyGroup.get();
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

void CComponentBase::Accept(IVisitor* visitor)
{
}



//
// Protected
//
void CComponentBase::RaiseComponentMessage(ComponentMessageType Message)
{
	m_OwnerNode.RaiseComponentMessage(this, Message);
}
