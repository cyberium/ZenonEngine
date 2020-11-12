#include "stdafx.h"

// General
#include "znPluginM2Models.h"

#include "EngineExtenders/ImageBLP.h"
#include "EngineExtenders/M2ModelsLoader.h"
#include "DBC/DBC__Storage.h"
#include "M2/M2.h"
#include "M2/M2_Base_Instance.h"
#include "M2/RenderPass_M2.h"
#include "WorldObjectsCreator.h"

CznPluginM2Models::CznPluginM2Models(IBaseManager& BaseManager)
	: m_BaseManager(BaseManager)
{}
CznPluginM2Models::~CznPluginM2Models()
{}



//
// IznPlugin
//
bool CznPluginM2Models::Initialize()
{
	// Files
	{
		m_BaseManager.GetManager<IFilesManager>()->AddStorage(EFilesStorageType::ADDITIONAL, MakeShared(CLibraryResourceFileStotage, GetModuleHandle(L"znPluginM2Models.dll")));
		m_BaseManager.GetManager<IFilesManager>()->AddStorage(EFilesStorageType::ADDITIONAL, MakeShared(CLocalFilesStorage, "E:/_games/World of Warcraft 3.3.5a/ExData/"));
	}

	// M2 format
	m_M2ModelsLoader = MakeShared(CznM2ModelsLoader, m_BaseManager);
	m_BaseManager.GetManager<IznModelsFactory>()->AddModelsLoader(m_M2ModelsLoader);

	// BLP format
	m_BLPImageLoader = MakeShared(CImageLoaderT<CImageBLP>);
	m_BaseManager.GetManager<IImagesFactory>()->AddImageLoader(m_BLPImageLoader);

	// Managers
	{
		m_BaseManager.AddManager<CDBCStorage>(MakeShared(CDBCStorage, m_BaseManager));
		m_BaseManager.AddManager<CWorldObjectCreator>(MakeShared(CWorldObjectCreator, m_BaseManager));
	}

	// M2 scene node
	{
		auto m2sceneNodeCreator = MakeShared(CSceneNodeCreator, m_BaseManager);
		m_BaseManager.GetManager<IObjectsFactory>()->GetClassFactory(otSceneNode)->AddClassCreator(m2sceneNodeCreator);

		m2sceneNodeCreator->AddClass(cSceneNodeM2, "SceneNodeM2", [](const IObjectCreationArgs* ObjectCreationArgs) -> std::shared_ptr<IObject>
		{
			auto& scene = static_cast<const ISceneNodeCreationArgs*>(ObjectCreationArgs)->GetScene();

			scene.GetBaseManager().GetManager<CWorldObjectCreator>()->InitEGxBlend(scene.GetRenderDevice());

			std::shared_ptr<CM2> m2Model = MakeShared(CM2, scene.GetBaseManager(), scene.GetRenderDevice(), "creature/ArthasLichKing/ArthasLichKing.M2");
			m2Model->Load();
			m2Model->SetState(ILoadable::ELoadableState::Loaded);

			auto node = MakeShared(CM2_Base_Instance, scene, m2Model);
			node->RegisterComponents();
			node->Load();
			node->SetState(ILoadable::ELoadableState::Loaded);
			return node;
		});
	}


	return true;
}

void CznPluginM2Models::Finalize()
{
	m_BaseManager.GetManager<IznModelsFactory>()->RemoveModelsLoader(m_M2ModelsLoader);
	m_BaseManager.GetManager<IImagesFactory>()->RemoveImageLoader(m_BLPImageLoader);
}

std::string CznPluginM2Models::GetName() const
{
	return "World of Warcraft *.m2 plugin.";
}

std::string CznPluginM2Models::GetDescription() const
{
	return "TODO: Description.";
}



//
// IRendererExtender
//
void CznPluginM2Models::Extend3DPasses(IRenderer& Renderer, IRenderDevice& RenderDevice, IScene& Scene, std::shared_ptr<IRenderTarget> RenderTarget, const Viewport * Viewport) const
{
	
}

void CznPluginM2Models::Extend3DPasses(IRenderer& Renderer, IRenderDevice& RenderDevice, const std::shared_ptr<IRenderPassCreateTypelessList>& SceneNodeListPass, std::shared_ptr<IRenderTarget> RenderTarget, const Viewport * Viewport) const
{
	Renderer.AddPass(MakeShared(CRenderPass_M2, RenderDevice, SceneNodeListPass, true)->ConfigurePipeline(RenderTarget, Viewport));
	Renderer.AddPass(MakeShared(CRenderPass_M2, RenderDevice, SceneNodeListPass, false)->ConfigurePipeline(RenderTarget, Viewport));
}


IznPlugin* GetPlugin(IBaseManager* BaseManager)
{
	return new CznPluginM2Models(*BaseManager);
}
