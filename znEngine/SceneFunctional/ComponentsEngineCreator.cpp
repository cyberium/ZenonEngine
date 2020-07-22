#include "stdafx.h"

// General
#include "ComponentsEngineCreator.h"

CComponentsEngineCreator::CComponentsEngineCreator(IBaseManager& BaseManager)
	: CObjectClassCreator(BaseManager)
{
	AddKey("SceneNodeComponent", cSceneNodeComponent);
	AddKey("SceneNodeColliderComponent", cSceneNodeColliderComponent);
	AddKey("SceneNodeModelsComponent", cSceneNodeModelsComponent);
	AddKey("SceneNodeSkeletonComponent", cSceneNodeSkeletonComponent);
	AddKey("SceneNodeParticleComponent", cSceneNodeParticleComponent);
	AddKey("SceneNodePhysicsComponent", cSceneNodePhysicsComponent);
	AddKey("SceneNodePortalsComponent", cSceneNodePortalsComponent);
	AddKey("SceneNodeLightComponent", cSceneNodeLightComponent);
	AddKey("SceneNodeCameraComponent", cSceneNodeCameraComponent);
}

CComponentsEngineCreator::~CComponentsEngineCreator()
{}

//
// IObjectClassCreator
//
std::shared_ptr<IObject> CComponentsEngineCreator::CreateObject(size_t Index, const IObjectCreationArgs* ObjectCreationArgs)
{
	auto componentCreationArgs = static_cast<IComponentCreationArgs*>(const_cast<IObjectCreationArgs*>(ObjectCreationArgs));
	std::shared_ptr<ISceneNodeComponent> createdComponent = nullptr;

	if (Index == 0)
	{
		createdComponent = std::make_shared<CComponentBase>(componentCreationArgs->GetSceneNode());
	} 
	else if (Index == 1)
	{
		createdComponent = std::make_shared<CColliderComponent3D>(componentCreationArgs->GetSceneNode());
	}
	else if (Index == 2)
	{
		createdComponent = std::make_shared<CModelsComponent3D>(componentCreationArgs->GetSceneNode());
	}
	else if(Index == 3)
	{
		createdComponent = nullptr; // SceneNodeSkeletonComponent std::make_shared<>(componentCreationArgs->GetSceneNode());
	}
	else if (Index == 4)
	{
		createdComponent = std::make_shared<CParticlesComponent3D>(componentCreationArgs->GetSceneNode());
	}
	else if (Index == 5)
	{
		createdComponent = nullptr; // SceneNodePhysicsComponent std::make_shared<CPhy>(componentCreationArgs->GetSceneNode());
	}
	else if (Index == 6)
	{
		createdComponent = nullptr; // SceneNodePortalsComponent std::make_shared<>(componentCreationArgs->GetSceneNode());
	}
	else if (Index == 7)
	{
		createdComponent = std::make_shared<CLightComponent3D>(componentCreationArgs->GetSceneNode());
	}
	else if (Index == 8)
	{
		createdComponent = std::make_shared<CCameraComponent3D>(componentCreationArgs->GetSceneNode());
	}

	if (createdComponent == nullptr)
		throw CException("CComponentsEngineCreator: CreateObject: Unable to create object with index %d.", Index);

	return createdComponent;
}