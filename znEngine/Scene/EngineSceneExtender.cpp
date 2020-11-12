#include "stdafx.h"

// General
#include "EngineSceneExtender.h"

#include "Scene/Components/CameraComponent3D.h"
#include "Scene/Components/ColliderComponent3D.h"
#include "Scene/Components/LightComponent3D.h"
#include "Scene/Components/ModelsComponent3D.h"
#include "Scene/Components/ReactPhysicsComponent.h"
#include "Scene/Components/Skeleton/SkeletonComponent.h"
#include "Scene/Components/Particles/ParticlesComponent.h"

#include "Scene/Nodes/UIText.h"
#include "Scene/Nodes/UIColor.h"

#include "Scene/SceneNodeRTSGround.h"
#include "Scene/SceneNodeRTSPathAndPoint.h"
#include "Scene/SceneNodeRTSUnit.h"
#include "Scene/SceneNodeRTSTower.h"

void EngineSceneTypesExtender(IBaseManager& BaseManager)
{
	//
	// SceneNode
	//
	auto sceneNodeEngineCreator = MakeShared(CSceneNodeCreator, BaseManager);
	BaseManager.GetManager<IObjectsFactory>()->GetClassFactory(otSceneNode)->AddClassCreator(sceneNodeEngineCreator);

	sceneNodeEngineCreator->AddClass(cSceneNode3D, "SceneNode", [](const IObjectCreationArgs* ObjectCreationArgs) -> std::shared_ptr<IObject>
	{
		auto node = MakeShared(CSceneNode, static_cast<const ISceneNodeCreationArgs*>(ObjectCreationArgs)->GetScene());
		node->RegisterComponents();
		return node;
	});

	sceneNodeEngineCreator->AddClass(cSceneNodeRTSGround, "SceneNodeRTSGround",  [](const IObjectCreationArgs* ObjectCreationArgs) -> std::shared_ptr<IObject>
	{
		auto node = MakeShared(CSceneNode, static_cast<const ISceneNodeCreationArgs*>(ObjectCreationArgs)->GetScene());
		node->RegisterComponents();
		return node;
	});

	sceneNodeEngineCreator->AddClass(cSceneNodeRTSPath, "SceneNodeRTSPath",  [](const IObjectCreationArgs* ObjectCreationArgs) -> std::shared_ptr<IObject>
	{
		auto node = MakeShared(CSceneNodeRTSPath, static_cast<const ISceneNodeCreationArgs*>(ObjectCreationArgs)->GetScene());
		return node;
	});

	sceneNodeEngineCreator->AddClass(cSceneNodeRTSPoint, "SceneNodeRTSPoint",  [](const IObjectCreationArgs* ObjectCreationArgs) -> std::shared_ptr<IObject>
	{
		auto& scene = static_cast<const ISceneNodeCreationArgs*>(ObjectCreationArgs)->GetScene();
		auto node = MakeShared(CSceneNodeRTSPoint, scene);
		node->AddComponentT(scene.GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<IComponentFactory>()->CreateComponentT<IColliderComponent3D>(cSceneNodeColliderComponent, *node));
		return node;
	});

	sceneNodeEngineCreator->AddClass(cSceneNodeRTSUnit, "SceneNodeRTSUnit",  [](const IObjectCreationArgs* ObjectCreationArgs) -> std::shared_ptr<IObject>
	{
		auto node = MakeShared(CSceneNodeRTSUnit, static_cast<const ISceneNodeCreationArgs*>(ObjectCreationArgs)->GetScene());
		node->RegisterComponents();
		return node;
	});

	sceneNodeEngineCreator->AddClass(cSceneNodeRTSBullet, "SceneNodeRTSBullet",  [](const IObjectCreationArgs* ObjectCreationArgs) -> std::shared_ptr<IObject>
	{
		auto node = MakeShared(CSceneNodeRTSBullet, static_cast<const ISceneNodeCreationArgs*>(ObjectCreationArgs)->GetScene());
		node->RegisterComponents();
		return node;
	});

	sceneNodeEngineCreator->AddClass(cSceneNodeRTSTower, "SceneNodeRTSTower",  [](const IObjectCreationArgs* ObjectCreationArgs) -> std::shared_ptr<IObject>
	{
		auto node = MakeShared(CSceneNodeRTSTower, static_cast<const ISceneNodeCreationArgs*>(ObjectCreationArgs)->GetScene());
		node->RegisterComponents();
		return node;
	});



	//
	// UIControl
	//
	auto uiControlEngineCreator = MakeShared(CUIControlCreator, BaseManager);
	BaseManager.GetManager<IObjectsFactory>()->GetClassFactory(otUIControl)->AddClassCreator(uiControlEngineCreator);

	uiControlEngineCreator->AddClass(cSceneNodeUI, "CUIControl",  [](const IObjectCreationArgs* ObjectCreationArgs) -> std::shared_ptr<IObject>
	{
		auto node = MakeShared(CUIControl, static_cast<const IUIControlCreationArgs*>(ObjectCreationArgs)->GetScene());
		return node;
	});
	uiControlEngineCreator->AddClass(cSceneNodeUI_Text, "SceneNodeUIText",  [](const IObjectCreationArgs* ObjectCreationArgs) -> std::shared_ptr<IObject>
	{
		auto node = MakeShared(CUITextNode, static_cast<const IUIControlCreationArgs*>(ObjectCreationArgs)->GetScene());
		return node;
	});
	uiControlEngineCreator->AddClass(cSceneNodeUI_Color, "SceneNodeUIColor",  [](const IObjectCreationArgs* ObjectCreationArgs) -> std::shared_ptr<IObject>
	{
		auto node = MakeShared(CUIColorNode, static_cast<const IUIControlCreationArgs*>(ObjectCreationArgs)->GetScene());
		return node;
	});



	//
	// Components
	//
	auto componentsEngineCreator = MakeShared(CComponentsCreator, BaseManager);
	BaseManager.GetManager<IObjectsFactory>()->GetClassFactory(otSceneNodeComponent)->AddClassCreator(componentsEngineCreator);

	componentsEngineCreator->AddClass(cSceneNodeComponent, "Component",  [](const IObjectCreationArgs* ObjectCreationArgs) -> std::shared_ptr<IObject>
	{
		return MakeShared(CComponentBase, static_cast<const IComponentCreationArgs*>(ObjectCreationArgs)->GetSceneNode());
	});
	componentsEngineCreator->AddClass(cSceneNodeColliderComponent, "ColliderComponent",  [](const IObjectCreationArgs* ObjectCreationArgs) -> std::shared_ptr<IObject>
	{
		return MakeShared(CColliderComponent3D, static_cast<const IComponentCreationArgs*>(ObjectCreationArgs)->GetSceneNode());
	});
	componentsEngineCreator->AddClass(cSceneNodeModelsComponent, "ModelsComponent",  [](const IObjectCreationArgs* ObjectCreationArgs) -> std::shared_ptr<IObject>
	{
		return MakeShared(CModelsComponent3D, static_cast<const IComponentCreationArgs*>(ObjectCreationArgs)->GetSceneNode());
	});
	componentsEngineCreator->AddClass(cSceneNodeSkeletonComponent, "SkeletonComponent",  [](const IObjectCreationArgs* ObjectCreationArgs) -> std::shared_ptr<IObject>
	{
		return nullptr; // MakeShared(CSkeletonComponent3D, static_cast<const IComponentCreationArgs*>(ObjectCreationArgs)->GetSceneNode());
	});
	componentsEngineCreator->AddClass(cSceneNodeParticleComponent, "ParticleComponent",  [](const IObjectCreationArgs* ObjectCreationArgs) -> std::shared_ptr<IObject>
	{
		return MakeShared(CParticlesComponent, static_cast<const IComponentCreationArgs*>(ObjectCreationArgs)->GetSceneNode());
	});
	componentsEngineCreator->AddClass(cSceneNodePhysicsComponent, "PhysicsComponent",  [](const IObjectCreationArgs* ObjectCreationArgs) -> std::shared_ptr<IObject>
	{
		return nullptr; // MakeShared(CReactPhysicsComponent, static_cast<const IComponentCreationArgs*>(ObjectCreationArgs)->GetSceneNode());
	});
	componentsEngineCreator->AddClass(cSceneNodePortalsComponent, "PortalsComponent",  [](const IObjectCreationArgs* ObjectCreationArgs) -> std::shared_ptr<IObject>
	{
		return nullptr;
	});
	componentsEngineCreator->AddClass(cSceneNodeLightComponent, "LightComponent",  [](const IObjectCreationArgs* ObjectCreationArgs) -> std::shared_ptr<IObject>
	{
		return MakeShared(CLightComponent3D, static_cast<const IComponentCreationArgs*>(ObjectCreationArgs)->GetSceneNode());
	});
	componentsEngineCreator->AddClass(cSceneNodeCameraComponent, "CameraComponent",  [](const IObjectCreationArgs* ObjectCreationArgs) -> std::shared_ptr<IObject>
	{
		return MakeShared(CCameraComponent3D, static_cast<const IComponentCreationArgs*>(ObjectCreationArgs)->GetSceneNode());
	});
}
