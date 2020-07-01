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
std::shared_ptr<ISceneNode3D> CSceneNode3DFactory::CreateSceneNode3D(IScene* Scene, ObjectClassType ObjectClassKey, const std::shared_ptr<ISceneNode3D>& Parent)
{
	class CSceneNode3DCreationArgs
		: public ISceneNode3DCreationArgs
	{
	public:
		CSceneNode3DCreationArgs(IScene* Scene, const std::shared_ptr<ISceneNode3D>& Parent)
			: m_Scene(Scene)
			, m_Parent(Parent)
		{}

		IScene* GetScene() override
		{
			return m_Scene;
		}
		std::shared_ptr<ISceneNode3D> GetParent() const
		{
			return m_Parent;
		}
	private:
		IScene* m_Scene;
		std::shared_ptr<ISceneNode3D> m_Parent;
	} const creationArgs(Scene, Parent);

	return std::dynamic_pointer_cast<ISceneNode3D>(CreateObject(ObjectClassKey, &creationArgs));
}

std::shared_ptr<ISceneNode3D> CSceneNode3DFactory::LoadSceneNode3D(IScene * Scene, std::shared_ptr<IByteBuffer> Bytes, const std::shared_ptr<ISceneNode3D>& Parent)
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
std::shared_ptr<ISceneNodeUI> CSceneNodeUIFactory::CreateSceneNodeUI(IScene* Scene, ObjectClassType ObjectClassKey, const std::shared_ptr<ISceneNodeUI>& Parent)
{
	class CSceneNodeUICreationArgs
		: public ISceneNodeUICreationArgs
	{
	public:
		CSceneNodeUICreationArgs(IScene* Scene, const std::shared_ptr<ISceneNodeUI>& Parent)
			: m_Scene(Scene)
			, m_Parent(Parent)
		{}

		IScene* GetScene() const override
		{
			return m_Scene;
		}

		std::shared_ptr<ISceneNodeUI> GetParent() const
		{
			return m_Parent;
		}

	private:
		IScene* m_Scene;
		std::shared_ptr<ISceneNodeUI> m_Parent;

	} const creationArgs(Scene, Parent);

	return std::dynamic_pointer_cast<ISceneNodeUI>(CreateObject(ObjectClassKey, &creationArgs));
}
