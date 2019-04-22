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
    return std::dynamic_pointer_cast<T, ISceneNodeComponent>(GetComponent(__uuidof(T)));
}

template<typename T>
inline std::shared_ptr<T> SceneNode3D::AddComponent(std::shared_ptr<T> Component)
{
    return std::dynamic_pointer_cast<T, ISceneNodeComponent>(AddComponent(__uuidof(T), Component));
}



//
// Components engine template access specialization
//
template<>
inline std::shared_ptr<CTransformComponent> SceneNode3D::GetComponent<CTransformComponent>()
{
    return m_Components_Transform;
}