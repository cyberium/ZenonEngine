#include "stdafx.h"

// General
#include "SceneNode3D.h"

// Additional
#include "TransformComponent3D.h"
#include "MeshComponent3D.h"
#include "ColliderComponent3D.h"

SceneNode3D::SceneNode3D()
{}

SceneNode3D::~SceneNode3D()
{}


std::shared_ptr<SceneNode3D> SceneNode3D::shared_from_this()
{
    return std::dynamic_pointer_cast<SceneNode3D>(SceneNodeBase::shared_from_this());
}

std::weak_ptr<SceneNode3D> SceneNode3D::weak_from_this()
{
    return std::weak_ptr<SceneNode3D>(shared_from_this());
}



//
// Components engine
//
void SceneNode3D::RegisterComponents()
{
    SetTransformComponent(AddComponent(std::make_shared<CTransformComponent3D>(shared_from_this())));
    SetMeshComponent(AddComponent(std::make_shared<CMeshComponent3D>(shared_from_this())));
    SetColliderComponent(AddComponent(std::make_shared<CColliderComponent3D>(shared_from_this())));
}

bool SceneNode3D::Accept(IVisitor* visitor)
{
	bool visitResult = visitor->Visit3D(this);

    if (visitResult)
    {
		const auto& components = GetComponents();
		std::for_each(components.begin(), components.end(), [&visitor](const std::pair<GUID, std::shared_ptr<ISceneNodeComponent>>& Component)
		{
			Component.second->Accept(visitor);
		});
    }

	// Now visit children
	const auto& childs = GetChilds();
	std::for_each(childs.begin(), childs.end(), [&visitor](const std::shared_ptr<ISceneNode>& Child)
	{
#ifdef LOADER_ENABLED
		std::shared_ptr<ILoadable> loadable = std::dynamic_pointer_cast<ILoadable>(child);
		if (loadable != nullptr && !loadable->isLoaded())
			continue;
#endif

		Child->Accept(visitor);
	});

	return visitResult;
}



//
// Protected
//
void SceneNode3D::SetTransformComponent(std::shared_ptr<ITransformComponent3D> TransformComponent)
{
    m_Components_Transform = TransformComponent;
}

void SceneNode3D::SetMeshComponent(std::shared_ptr<IMeshComponent3D> MeshComponent)
{
	m_Components_Mesh = MeshComponent;
}

void SceneNode3D::SetColliderComponent(std::shared_ptr<IColliderComponent3D> ColliderComponent)
{
    m_Components_Collider = ColliderComponent;
}
