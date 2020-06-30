#include "stdafx.h"

// General
#include "SceneNode3DFactory.h"

// Additional
#include "Scene/3D/SceneNode3D.h"
#include "Scene/UI/SceneNodeUI.h"

CSceneNode3DFactory::CSceneNode3DFactory(IBaseManager& BaseManager, ObjectFactoryType Type)
	: CObjectClassFactory(BaseManager, Type)
{}

CSceneNode3DFactory::~CSceneNode3DFactory()
{}

//
// ISceneNode3DFactory
//
std::shared_ptr<ISceneNode3D> CSceneNode3DFactory::CreateSceneNode3D(IScene* Scene, ObjectClassType ObjectClassKey)
{
	class CSceneNode3DCreationArgs
		: public ISceneNode3DCreationArgs
	{
	public:
		CSceneNode3DCreationArgs(IScene* Scene)
			: m_Scene(Scene)
		{}

		IScene* GetScene() override
		{
			return m_Scene;
		}
	private:
		IScene* m_Scene;
	} const creationArgs(Scene);

	return std::dynamic_pointer_cast<ISceneNode3D>(CreateObject(ObjectClassKey, &creationArgs));
}

std::shared_ptr<ISceneNode3D> CSceneNode3DFactory::LoadSceneNode3D(IScene * Scene, std::shared_ptr<IByteBuffer> Bytes)
{
	return std::shared_ptr<ISceneNode3D>();
}



CSceneNodeUIFactory::CSceneNodeUIFactory(IBaseManager& BaseManager, ObjectFactoryType Type)
	: CObjectClassFactory(BaseManager, Type)
{
}

CSceneNodeUIFactory::~CSceneNodeUIFactory()
{}

//
// ISceneNodeUIFactory
//
std::shared_ptr<ISceneNodeUI> CSceneNodeUIFactory::CreateSceneNodeUI(IScene* Scene, ObjectClassType ObjectClassKey)
{
	class CSceneNodeUICreationArgs
		: public ISceneNodeUICreationArgs
	{
	public:
		CSceneNodeUICreationArgs(IScene* Scene)
			: m_Scene(Scene)
		{}

		IScene* GetScene() const override
		{
			return m_Scene;
		}

	private:
		IScene* m_Scene;

	} const creationArgs(Scene);

	return std::dynamic_pointer_cast<ISceneNodeUI>(CreateObject(ObjectClassKey, &creationArgs));
}
