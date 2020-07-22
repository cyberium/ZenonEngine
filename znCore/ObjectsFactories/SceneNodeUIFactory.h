#pragma once

#include "ObjectClassFactory.h"

class ZN_API CSceneNodeUIFactory
	: public CObjectClassFactory
	, public ISceneNodeUIFactory
{
public:
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
	};

public:
	CSceneNodeUIFactory(IBaseManager& BaseManager, const std::string& TypeName, ObjectType Type);
	virtual ~CSceneNodeUIFactory();

	// ISceneNodeUIFactory
	std::shared_ptr<ISceneNodeUI> CreateSceneNodeUI(IScene* Scene, ObjectClass ObjectClassKey, const std::shared_ptr<ISceneNodeUI>& Parent) override;
};
