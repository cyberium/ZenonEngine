#pragma once

//
// Components engine template access
//
template<typename T>
inline std::shared_ptr<T> SceneNode3D::GetComponent()
{
    return ISceneNode::GetComponent<T>();
}



//
// Components engine template access specialization
//
template<>
inline std::shared_ptr<CTransformComponent3D> SceneNode3D::GetComponent<CTransformComponent3D>()
{
    return m_Components_Transform;
}

template<>
inline std::shared_ptr<CMeshComponent3D> SceneNode3D::GetComponent<CMeshComponent3D>()
{
	return m_Components_Mesh;
}

template<>
inline std::shared_ptr<CColliderComponent3D> SceneNode3D::GetComponent<CColliderComponent3D>()
{
    return m_Components_Collider;
}
