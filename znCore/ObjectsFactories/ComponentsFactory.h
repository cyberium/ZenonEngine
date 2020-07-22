#pragma once

#include "ObjectClassFactory.h"

class ZN_API CComponentsFactory
	: public CObjectClassFactory
	, public IComponentFactory
{
public:
	class CComponentCreationArgs
		: public IComponentCreationArgs
	{
	public:
		CComponentCreationArgs(ISceneNode3D& ISceneNode3D)
			: m_SceneNode(ISceneNode3D)
		{}

		ISceneNode3D& GetSceneNode() override
		{
			return m_SceneNode;
		}
	private:
		ISceneNode3D& m_SceneNode;
	};

public:
	CComponentsFactory(IBaseManager& BaseManager, const std::string& TypeName, ObjectType Type);
	virtual ~CComponentsFactory();

	// IComponentFactory
	virtual std::shared_ptr<ISceneNodeComponent> CreateComponent(ObjectClass ObjectClassKey, ISceneNode3D& SceneNode) override;
	virtual std::shared_ptr<ISceneNodeComponent> LoadComponentXML(const std::shared_ptr<IXMLReader>& Reader, ISceneNode3D& SceneNode) override;
	virtual std::shared_ptr<IXMLWriter> SaveComponentXML(std::shared_ptr<ISceneNodeComponent> Object) override;
};