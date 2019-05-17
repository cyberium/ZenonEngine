#pragma once

//
// Components engine template access specialization
//
template<>
inline std::shared_ptr<CTransformComponent3D> SceneNode3D::GetComponent<CTransformComponent3D>()
{
    return m_Components_Transform;
}

template<>
inline std::shared_ptr<CColliderComponent3D> SceneNode3D::GetComponent<CColliderComponent3D>()
{
    return m_Components_Collider;
}
