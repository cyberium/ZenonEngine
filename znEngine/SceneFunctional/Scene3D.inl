#pragma once

template<class T, class ... Args>
inline std::shared_ptr<T> Scene3D::CreateSceneNode(std::shared_ptr<SceneNode3D> Parent, Args&& ..._Args)
{
    std::shared_ptr<T> newNode = std::make_shared<T>(_Args);
    newNode->SetParent(Parent);
    newNode->RegisterComponents();

    return newNode;
}