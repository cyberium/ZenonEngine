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
inline std::shared_ptr<ITransformComponent3D> SceneNode3D::GetComponent<ITransformComponent3D>()
{
    return m_Components_Transform;
}

template<>
inline std::shared_ptr<IMeshComponent3D> SceneNode3D::GetComponent<IMeshComponent3D>()
{
	return m_Components_Mesh;
}

template<>
inline std::shared_ptr<IColliderComponent3D> SceneNode3D::GetComponent<IColliderComponent3D>()
{
    return m_Components_Collider;
}
