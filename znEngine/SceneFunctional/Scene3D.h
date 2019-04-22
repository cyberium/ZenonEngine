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
	std::shared_ptr<SceneNode3D> m_pRootNode;
};


template<class T, class ... Args>
inline std::shared_ptr<T> Scene3D::CreateSceneNode(std::shared_ptr<SceneNode3D> Parent, Args&& ..._Args)
{
    std::shared_ptr<T> newNode = std::make_shared<T>(_Args);
    newNode->SetParent(Parent);
    newNode->RegisterComponents();

    return newNode;
}
