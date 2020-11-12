#pragma once

#include "../ObjectClassFactory.h"

class ZN_API CComponentsFactory
	: public CObjectClassFactory
	, public IComponentFactory
{
public:
	class CComponentCreationArgs
		: public IComponentCreationArgs
	{
	public:
		CComponentCreationArgs(ISceneNode& ISceneNode)
			: m_SceneNode(ISceneNode)
		{}

		ISceneNode& GetSceneNode() const override
		{
			return m_SceneNode;
		}
	private:
		ISceneNode& m_SceneNode;
	};

public:
	CComponentsFactory(IBaseManager& BaseManager, const std::string& TypeName, ObjectType Type);
	virtual ~CComponentsFactory();

	// IComponentFactory
	virtual std::shared_ptr<ISceneNodeComponent> CreateComponent(ObjectClass ObjectClassKey, ISceneNode& SceneNode) override;
	virtual std::shared_ptr<ISceneNodeComponent> LoadComponentXML(const std::shared_ptr<IXMLReader>& Reader, ISceneNode& SceneNode) override;
	virtual std::shared_ptr<IXMLWriter>          SaveComponentXML(std::shared_ptr<ISceneNodeComponent> SceneNodeComponent) override;
};