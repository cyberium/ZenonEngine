#include "stdafx.h"

// General
#include "SceneNode3DCreator.h"

// Additional
#include "Scene/3D/SceneNode3D.h"
#include "Scene/UI/SceneNodeUI.h"

CSceneNode3DCreator::CSceneNode3DCreator(IBaseManager& BaseManager)
	: CObjectClassCreator(BaseManager)
	, m_Parent(nullptr)
{}

CSceneNode3DCreator::~CSceneNode3DCreator()
{}

//
// ISceneNode3DCreator
//
std::shared_ptr<ISceneNode3D> CSceneNode3DCreator::CreateSceneNode3D(ISceneNode3D * Parent, ObjectClassType ObjectClassKey)
{
	m_Parent = Parent;
	return std::dynamic_pointer_cast<ISceneNode3D>(CreateObject(ObjectClassKey));
}



CSceneNodeUICreator::CSceneNodeUICreator(IBaseManager& BaseManager)
	: CObjectClassCreator(BaseManager)
	, m_Parent(nullptr)
{
}

CSceneNodeUICreator::~CSceneNodeUICreator()
{}

//
// ISceneNodeUICreator
//
std::shared_ptr<ISceneNodeUI> CSceneNodeUICreator::CreateSceneNodeUI(ISceneNodeUI * Parent, ObjectClassType ObjectClassKey)
{
	m_Parent = Parent;
	return std::dynamic_pointer_cast<ISceneNodeUI>(CreateObject(ObjectClassKey));
}
