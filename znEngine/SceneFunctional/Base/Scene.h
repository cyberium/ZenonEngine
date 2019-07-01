#pragma once

#include "SceneNode.h"

class OW_ENGINE_API Scene : public Object, public std::enable_shared_from_this<Scene>
{
public:
	Scene();
	virtual ~Scene();

    // Creates new SceneNode and initialize it. You !must! call this method instead of creating nodes in code
    template<class T, typename... Args>
    std::shared_ptr<T> CreateSceneNode(std::weak_ptr<SceneNode> Parent, Args &&... _Args);

	virtual void Accept(std::shared_ptr<IVisitor> visitor) = 0;

    virtual void OnUpdate(UpdateEventArgs& e) = 0;
};

#include "Scene.inl"
