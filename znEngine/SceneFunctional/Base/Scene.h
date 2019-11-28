#pragma once

#include "SceneNode.h"

class OW_ENGINE_API Scene : public Object, public std::enable_shared_from_this<Scene>
{
public:
	Scene(std::shared_ptr<IBaseManager> BaseManager);
	virtual ~Scene();

	const std::shared_ptr<IBaseManager> GetBaseManager() const;

    // Creates new SceneNode and initialize it. You !must! call this method instead of creating nodes in code
    template<class T, typename... Args>
    std::shared_ptr<T> CreateSceneNode(std::weak_ptr<SceneNode> Parent, Args &&... _Args);

	virtual void Accept(IVisitor* visitor) = 0;

    virtual void OnUpdate(UpdateEventArgs& e) = 0;

private:
	std::shared_ptr<IBaseManager> m_BaseManager;
};

#include "Scene.inl"
