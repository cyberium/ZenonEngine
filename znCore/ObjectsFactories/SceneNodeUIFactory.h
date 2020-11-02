#pragma once

#include "ObjectClassFactory.h"

class ZN_API CSceneNodeUIFactory
	: public CObjectClassFactory
	, public IUIControlFactory
{
public:
	class CSceneNodeUICreationArgs
		: public IUIControlCreationArgs
	{
	public:
		CSceneNodeUICreationArgs(IScene& Scene, const std::shared_ptr<IUIControl>& Parent)
			: m_Scene(Scene)
			, m_Parent(Parent)
		{}

		IScene& GetScene() const override
		{
			return m_Scene;
		}

		std::shared_ptr<IUIControl> GetParent() const
		{
			return m_Parent;
		}

	private:
		IScene& m_Scene;
		std::shared_ptr<IUIControl> m_Parent;
	};

public:
	CSceneNodeUIFactory(IBaseManager& BaseManager, const std::string& TypeName, ObjectType Type);
	virtual ~CSceneNodeUIFactory();

	// IUIControlFactory
	std::shared_ptr<IUIControl> CreateSceneNodeUI(ObjectClass ObjectClassKey, IScene& Scene, const std::shared_ptr<IUIControl>& Parent) override;
};
