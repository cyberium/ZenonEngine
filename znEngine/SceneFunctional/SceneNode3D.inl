#pragma once

//
// Components engine template access
//
template<typename T>
inline std::shared_ptr<T> SceneNode3D::IsComponentExists()
{
    return IsComponentExists(__uuidof(T));
}

template<typename T>
inline std::shared_ptr<T> SceneNode3D::GetComponent()
{
    std::shared_ptr<ISceneNodeComponent> component = GetComponent(__uuidof(T));
    if (component == nullptr)
        return std::shared_ptr<T>();

    return std::dynamic_pointer_cast<T, ISceneNodeComponent>(component);
}

template<typename T>
inline std::shared_ptr<T> SceneNode3D::AddComponent(std::shared_ptr<T> Component)
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
inline std::shared_ptr<T> SceneNode3D::CreateSceneNode(Args && ..._Args)
{
    return GetScene()->CreateSceneNode<T>(shared_from_this(), std::forward<Args>(_Args)...);
}



//
// Components engine template access specialization
//
template<>
inline std::shared_ptr<CTransformComponent> SceneNode3D::GetComponent<CTransformComponent>()
{
    return m_Components_Transform;
}

template<>
inline std::shared_ptr<CColliderComponent> SceneNode3D::GetComponent<CColliderComponent>()
{
    return m_Components_Collider;
}
