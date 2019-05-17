#pragma once

//
// Components engine template access
//
template<typename T>
inline std::shared_ptr<T> SceneNode::IsComponentExists()
{
    return IsComponentExists(__uuidof(T));
}

template<typename T>
inline std::shared_ptr<T> SceneNode::GetComponent()
{
    std::shared_ptr<ISceneNodeComponent> component = GetComponent(__uuidof(T));
    if (component == nullptr)
        return std::shared_ptr<T>();

    return std::dynamic_pointer_cast<T, ISceneNodeComponent>(component);
}

template<typename T>
inline std::shared_ptr<T> SceneNode::AddComponent(std::shared_ptr<T> Component)
{
    std::shared_ptr<ISceneNodeComponent> component = AddComponent(__uuidof(T), Component);
    if (component == nullptr)
        return std::shared_ptr<T>();

    return std::dynamic_pointer_cast<T, ISceneNodeComponent>(component);
}



//
// Scene access
//
template<typename T, typename ...Args>
inline std::shared_ptr<T> SceneNode::CreateSceneNode(Args && ..._Args)
{
    return GetScene()->CreateSceneNode<T>(shared_from_this(), std::forward<Args>(_Args)...);
}
