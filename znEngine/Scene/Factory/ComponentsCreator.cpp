#include "stdafx.h"

// General
#include "ComponentsCreator.h"
#include "Scene/Components/CameraComponent3D.h"
#include "Scene/Components/ColliderComponent3D.h"
#include "Scene/Components/LightComponent3D.h"
#include "Scene/Components/ModelsComponent3D.h"
#include "Scene/Components/ParticlesComponent3D.h"

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
		createdComponent = MakeShared(CComponentBase, componentCreationArgs->GetSceneNode());
	} 
	else if (Index == 1)
	{
		createdComponent = MakeShared(CColliderComponent3D, componentCreationArgs->GetSceneNode());
	}
	else if (Index == 2)
	{
		createdComponent = MakeShared(CModelsComponent3D, componentCreationArgs->GetSceneNode());
	}
	else if(Index == 3)
	{
		createdComponent = nullptr; // SceneNodeSkeletonComponent MakeShared(, componentCreationArgs->GetSceneNode());
	}
	else if (Index == 4)
	{
		createdComponent = MakeShared(CParticlesComponent3D, componentCreationArgs->GetSceneNode());
	}
	else if (Index == 5)
	{
		createdComponent = nullptr; // SceneNodePhysicsComponent MakeShared(CPhy, componentCreationArgs->GetSceneNode());
	}
	else if (Index == 6)
	{
		createdComponent = nullptr; // SceneNodePortalsComponent MakeShared(, componentCreationArgs->GetSceneNode());
	}
	else if (Index == 7)
	{
		createdComponent = MakeShared(CLightComponent3D, componentCreationArgs->GetSceneNode());
	}
	else if (Index == 8)
	{
		createdComponent = MakeShared(CCameraComponent3D, componentCreationArgs->GetSceneNode());
	}

	if (createdComponent == nullptr)
		throw CException("CComponentsEngineCreator: CreateObject: Unable to create object with index %d.", Index);

	return createdComponent;
}