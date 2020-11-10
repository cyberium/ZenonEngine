#include "stdafx.h"

// General
#include "ComponentsCreator.h"
#include "Scene/Components/CameraComponent3D.h"
#include "Scene/Components/ColliderComponent3D.h"
#include "Scene/Components/LightComponent3D.h"
#include "Scene/Components/ModelsComponent3D.h"
#include "Scene/Components/Particles/ParticlesComponent.h"

CComponentsCreator::CComponentsCreator(IBaseManager& BaseManager)
	: CObjectClassCreatorBase(BaseManager)
{
	AddKey("Component", cSceneNodeComponent);
	AddKey("ColliderComponent", cSceneNodeColliderComponent);
	AddKey("ModelsComponent", cSceneNodeModelsComponent);
	AddKey("SkeletonComponent", cSceneNodeSkeletonComponent);
	AddKey("ParticleComponent", cSceneNodeParticleComponent);
	AddKey("PhysicsComponent", cSceneNodePhysicsComponent);
	AddKey("PortalsComponent", cSceneNodePortalsComponent);
	AddKey("LightComponent", cSceneNodeLightComponent);
	AddKey("CameraComponent", cSceneNodeCameraComponent);
}

CComponentsCreator::~CComponentsCreator()
{}

//
// IObjectClassCreator
//
std::shared_ptr<IObject> CComponentsCreator::CreateObject(size_t Index, const Guid& AssignedGuid, const IObjectCreationArgs* ObjectCreationArgs)
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
		createdComponent = MakeShared(CParticlesComponent, componentCreationArgs->GetSceneNode());
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
		throw CException("ComponentsEngineCreator: CreateObject: Unable to create object with index %d.", Index);

	if (auto objectPrivate = std::dynamic_pointer_cast<IObjectPrivate>(createdComponent))
		objectPrivate->SetGUID(AssignedGuid);
	else
		throw CException("ComponentsEngineCreator: Object [%s] not support IObjectInternal.", AssignedGuid.CStr());

	return createdComponent;
}