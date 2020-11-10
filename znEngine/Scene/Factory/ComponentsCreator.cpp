#include "stdafx.h"

// General
#include "ComponentsCreator.h"
#include "Scene/Components/CameraComponent3D.h"
#include "Scene/Components/ColliderComponent3D.h"
#include "Scene/Components/LightComponent3D.h"
#include "Scene/Components/ModelsComponent3D.h"
#include "Scene/Components/ReactPhysicsComponent.h"
#include "Scene/Components/Skeleton/SkeletonComponent.h"
#include "Scene/Components/Particles/ParticlesComponent.h"

CComponentsCreator::CComponentsCreator(IBaseManager& BaseManager)
	: CObjectClassCreatorBase(BaseManager)
{
	AddKey("Component", cSceneNodeComponent, [](const IObjectCreationArgs* ObjectCreationArgs) -> std::shared_ptr<IObject> 
	{
		return MakeShared(CComponentBase, static_cast<const IComponentCreationArgs*>(ObjectCreationArgs)->GetSceneNode());
	});
	AddKey("ColliderComponent", cSceneNodeColliderComponent, [](const IObjectCreationArgs* ObjectCreationArgs) -> std::shared_ptr<IObject> 
	{
		return MakeShared(CColliderComponent3D, static_cast<const IComponentCreationArgs*>(ObjectCreationArgs)->GetSceneNode());
	});
	AddKey("ModelsComponent", cSceneNodeModelsComponent, [](const IObjectCreationArgs* ObjectCreationArgs) -> std::shared_ptr<IObject> 
	{
		return MakeShared(CModelsComponent3D, static_cast<const IComponentCreationArgs*>(ObjectCreationArgs)->GetSceneNode());
	});
	AddKey("SkeletonComponent", cSceneNodeSkeletonComponent, [](const IObjectCreationArgs* ObjectCreationArgs) -> std::shared_ptr<IObject> 
	{
		return nullptr; // MakeShared(CSkeletonComponent3D, static_cast<const IComponentCreationArgs*>(ObjectCreationArgs)->GetSceneNode());
	});
	AddKey("ParticleComponent", cSceneNodeParticleComponent, [](const IObjectCreationArgs* ObjectCreationArgs) -> std::shared_ptr<IObject> 
	{
		return MakeShared(CParticlesComponent, static_cast<const IComponentCreationArgs*>(ObjectCreationArgs)->GetSceneNode());
	});
	AddKey("PhysicsComponent", cSceneNodePhysicsComponent, [](const IObjectCreationArgs* ObjectCreationArgs) -> std::shared_ptr<IObject> 
	{
		return nullptr; // MakeShared(CReactPhysicsComponent, static_cast<const IComponentCreationArgs*>(ObjectCreationArgs)->GetSceneNode());
	});
	AddKey("PortalsComponent", cSceneNodePortalsComponent, [](const IObjectCreationArgs* ObjectCreationArgs) -> std::shared_ptr<IObject> 
	{
		return nullptr;
	});
	AddKey("LightComponent", cSceneNodeLightComponent, [](const IObjectCreationArgs* ObjectCreationArgs) -> std::shared_ptr<IObject> 
	{
		return MakeShared(CLightComponent3D, static_cast<const IComponentCreationArgs*>(ObjectCreationArgs)->GetSceneNode());
	});
	AddKey("CameraComponent", cSceneNodeCameraComponent, [](const IObjectCreationArgs* ObjectCreationArgs) -> std::shared_ptr<IObject> 
	{
		return MakeShared(CCameraComponent3D, static_cast<const IComponentCreationArgs*>(ObjectCreationArgs)->GetSceneNode());
	});
}

CComponentsCreator::~CComponentsCreator()
{}

//
// IObjectClassCreator
//
std::shared_ptr<IObject> CComponentsCreator::CreateObject(size_t Index, const Guid& AssignedGuid, const IObjectCreationArgs* ObjectCreationArgs)
{
	std::shared_ptr<IObject> createdComponent = GetSupportedClassFunction(Index)(ObjectCreationArgs);
	if (createdComponent == nullptr)
		throw CException("ComponentsEngineCreator: CreateObject: Unable to create object with index %d.", Index);

	if (auto objectPrivate = std::dynamic_pointer_cast<IObjectPrivate>(createdComponent))
		objectPrivate->SetGUID(AssignedGuid);
	else
		throw CException("ComponentsEngineCreator: Object [%s] not support IObjectInternal.", AssignedGuid.CStr());

	return createdComponent;
}