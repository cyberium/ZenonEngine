#pragma once

#include "ObjectClassFactory.h"

class ZN_API CSceneNode3DFactory
	: public CObjectClassFactory
	, public ISceneNode3DFactory
{
public:
	class CSceneNode3DCreationArgs
		: public ISceneNode3DCreationArgs
	{
	public:
		CSceneNode3DCreationArgs(IScene& Scene, const std::shared_ptr<ISceneNode>& Parent)
			: m_Scene(Scene)
			, m_Parent(Parent)
		{}

		IScene& GetScene() const override
		{
			return m_Scene;
		}

		std::shared_ptr<ISceneNode> GetParent() const
		{
			return m_Parent;
		}

	private:
		IScene& m_Scene;
		std::shared_ptr<ISceneNode> m_Parent;
	};

public:
	CSceneNode3DFactory(IBaseManager& BaseManager, const std::string& TypeName, ObjectType Type);
	virtual ~CSceneNode3DFactory();

	// ISceneNode3DFactory
	std::shared_ptr<ISceneNode>   CreateSceneNode3D(ObjectClass ObjectClassKey, IScene& Scene, const std::shared_ptr<ISceneNode>& Parent) override;
	std::shared_ptr<ISceneNode>   LoadSceneNode3DXML(const std::shared_ptr<IXMLReader>& Reader, IScene& Scene, const std::shared_ptr<ISceneNode>& Parent = nullptr) override;
	std::shared_ptr<IXMLWriter>   SaveSceneNode3DXML(std::shared_ptr<ISceneNode> Object) override;
	std::shared_ptr<ISceneNode>   LoadSceneNode3D(const std::shared_ptr<IByteBuffer>& Bytes, IScene& Scene, const std::shared_ptr<ISceneNode>& Parent = nullptr) override;
	std::shared_ptr<IByteBuffer>  SaveSceneNode3D(std::shared_ptr<ISceneNode> Object) override;
};
