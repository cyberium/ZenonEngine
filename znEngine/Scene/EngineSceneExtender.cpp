#include "stdafx.h"

// General
#include "EngineSceneExtender.h"

#include "Scene/Components/CameraComponent.h"
#include "Scene/Components/ColliderComponent.h"
#include "Scene/Components/LightComponent.h"
#include "Scene/Components/ModelComponent/ModelComponent.h"
#include "Scene/Components/ReactPhysicsComponent.h"
#include "Scene/Components/Particles/ParticlesComponent.h"

#include "UIControls/UIControlCommon.h"
#include "UIControls/UIControlText.h"

// RTS
#include "RTS/UIControls/UIControlRTSResourcesPanel.h"
#include "RTS/UIControls/UIControlRTSTowersPanel.h"
#include "RTS/UIControls/UIControlRTSTowerBtn.h"

#include "RTS/SceneNodes/SceneNodeRTSGround.h"
#include "RTS/SceneNodes/SceneNodeRTSPathAndPoint.h"
#include "RTS/SceneNodes/SceneNodeRTSUnit.h"
#include "RTS/SceneNodes/SceneNodeRTSBullet.h"
#include "RTS/SceneNodes/SceneNodeRTSTower.h"

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
		node->AddComponentT(scene.GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<IComponentFactory>()->CreateComponentT<IColliderComponent>(cSceneNodeColliderComponent, *node));
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

	uiControlEngineCreator->AddClass(cUIControl, "UIControl",  [](const IObjectCreationArgs* ObjectCreationArgs) -> std::shared_ptr<IObject>
	{
		auto node = MakeShared(CUIControl, static_cast<const IUIControlCreationArgs*>(ObjectCreationArgs)->GetScene());
		return node;
	});
	uiControlEngineCreator->AddClass(cUIControlText, "UIControlText",  [](const IObjectCreationArgs* ObjectCreationArgs) -> std::shared_ptr<IObject>
	{
		auto node = MakeShared(CUIControlText, static_cast<const IUIControlCreationArgs*>(ObjectCreationArgs)->GetScene());
		return node;
	});
	uiControlEngineCreator->AddClass(cUIControlCommon, "UIContolCommon", [](const IObjectCreationArgs* ObjectCreationArgs) -> std::shared_ptr<IObject>
	{
		auto node = MakeShared(CUIControlCommon, static_cast<const IUIControlCreationArgs*>(ObjectCreationArgs)->GetScene());
		return node;
	});
	uiControlEngineCreator->AddClass(cUIControlRTSResourcesPanel, "UIControlRTSResourcesPanel", [](const IObjectCreationArgs* ObjectCreationArgs) -> std::shared_ptr<IObject>
	{
		auto node = MakeShared(CUIControlRTSResourcesPanel, static_cast<const IUIControlCreationArgs*>(ObjectCreationArgs)->GetScene());
		return node;
	});
	uiControlEngineCreator->AddClass(cUIControlRTSTowersPanel, "UIControlRTSTowersPanel", [](const IObjectCreationArgs* ObjectCreationArgs) -> std::shared_ptr<IObject>
	{
		auto node = MakeShared(CUIControlRTSTowersPanel, static_cast<const IUIControlCreationArgs*>(ObjectCreationArgs)->GetScene());
		return node;
	});
	uiControlEngineCreator->AddClass(cUIControlRTSTowerBtn, "UIControlRTSTowerBtn", [](const IObjectCreationArgs* ObjectCreationArgs) -> std::shared_ptr<IObject>
	{
		auto node = MakeShared(CUIControlRTSTowerBtn, static_cast<const IUIControlCreationArgs*>(ObjectCreationArgs)->GetScene());
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
		return MakeShared(CColliderComponent, static_cast<const IComponentCreationArgs*>(ObjectCreationArgs)->GetSceneNode());
	});
	componentsEngineCreator->AddClass(cSceneNodeModelsComponent, "ModelsComponent",  [](const IObjectCreationArgs* ObjectCreationArgs) -> std::shared_ptr<IObject>
	{
		return MakeShared(CModelComponent, static_cast<const IComponentCreationArgs*>(ObjectCreationArgs)->GetSceneNode());
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
		return MakeShared(CLightComponent, static_cast<const IComponentCreationArgs*>(ObjectCreationArgs)->GetSceneNode());
	});
	componentsEngineCreator->AddClass(cSceneNodeCameraComponent, "CameraComponent",  [](const IObjectCreationArgs* ObjectCreationArgs) -> std::shared_ptr<IObject>
	{
		return MakeShared(CCameraComponent, static_cast<const IComponentCreationArgs*>(ObjectCreationArgs)->GetSceneNode());
	});
}
