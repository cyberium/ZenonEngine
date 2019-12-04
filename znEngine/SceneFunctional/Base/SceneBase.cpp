#include "stdafx.h"

// General
#include "SceneBase.h"

SceneBase::SceneBase(std::shared_ptr<IBaseManager> BaseManager)
	: m_BaseManager(BaseManager)
{}

SceneBase::~SceneBase()
{}



//
// IScene
//
std::shared_ptr<ISceneNode> SceneBase::GetRootNode() const
{
	return m_RootNode;
}

void SceneBase::Accept(IVisitor * visitor)
{
	if (m_RootNode)
		m_RootNode->Accept(visitor);
}

void SceneBase::OnUpdate(UpdateEventArgs& e)
{
	if (m_RootNode)
		m_RootNode->OnUpdate(e);
}



//
// IBaseManagerHolder
//
std::shared_ptr<IBaseManager> SceneBase::GetBaseManager() const
{
	return m_BaseManager;
}
