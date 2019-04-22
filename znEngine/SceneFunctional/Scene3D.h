#pragma once

#include "SceneNode3D.h"

class Scene3D : public Object
{
public:
	Scene3D();
	virtual ~Scene3D();

	std::shared_ptr<SceneNode3D> GetRootNode() const;

    template<class T, class ... Args>
    std::shared_ptr<T> CreateSceneNode(std::shared_ptr<SceneNode3D> Parent, Args&&... _Args);

	void Accept(IVisitor& visitor);

	void OnUpdate(UpdateEventArgs& e);

private:
	std::shared_ptr<SceneNode3D> m_RootNode;
};

#include "Scene3D.inl"
