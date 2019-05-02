#pragma once

#include "SceneNode3D.h"

class Scene3D : public Object, public std::enable_shared_from_this<Scene3D>
{
public:
	Scene3D();
	virtual ~Scene3D();

    void CreateRootNode();
	std::shared_ptr<SceneNode3D> GetRootNode() const;

    template<class T, typename... Args>
    std::shared_ptr<T> CreateSceneNode(std::weak_ptr<SceneNode3D> Parent, Args &&... _Args);

	void Accept(IVisitor& visitor);

	void OnUpdate(UpdateEventArgs& e);

private:
	std::shared_ptr<SceneNode3D> m_RootNode;
};

#include "Scene3D.inl"
