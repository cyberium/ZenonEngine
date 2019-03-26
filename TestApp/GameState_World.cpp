#include "stdafx.h"

// General
#include "GameState_World.h"

// Additional

CGameState_World::CGameState_World(IApplication * Application)
    : base(Application)
{
	m_Viewport = Viewport(0, 0, 1280.0f, 1024.0f);
}

CGameState_World::~CGameState_World()
{
}


//
// IGameState
//
bool CGameState_World::Init()
{
	std::shared_ptr<IRenderDevice> renderDevice = GetApplication()->GetRenderDevice();

	//
	// Camera controller
	//
	m_CameraController = std::make_shared<CFreeCameraController>();
	m_CameraController->GetCamera()->SetViewport(m_Viewport);
	m_CameraController->GetCamera()->SetProjectionRH(45.0f, 1280.0f / 1024.0f, 1.0f, 4000.0f);
	
	m_FrameQuery = renderDevice->CreateQuery(Query::QueryType::Timer, 1);

	m_GB = std::make_shared<CGBuffer>(m_3DScene);
	m_GB->Load(c_WindowsWidth, c_WindowsHeight);
	m_GB->Load2(m_Viewport);

	Load3D();
	LoadUI();

	return base::Init();
}

void CGameState_World::Destroy()
{
	/*renderDevice->CreateTexture2D("Textures\\ShaneCube.blp"); // DXT1
	renderDevice->CreateTexture2D("Textures\\Minimap\\00b445de1413eeca80cc683deb9af58b.blp"); // DXT1A
	renderDevice->CreateTexture2D("Textures\\MinimapMask.blp"); // DXT3
	renderDevice->CreateTexture2D("Textures\\SpellChainEffects\\VR_Chain.blp"); // DXT5
	renderDevice->CreateTexture2D("Textures\\Moon02Glare.blp"); // RAW0
	renderDevice->CreateTexture2D("Textures\\ShadowBlob.blp"); // RAW1
	renderDevice->CreateTexture2D("Textures\\moon.blp"); // RAW8
	renderDevice->CreateTexture2D("Textures\\SunGlare.blp"); // PURE*/

	base::Destroy();
}


//
//
//
void CGameState_World::OnPreRender(Render3DEventArgs& e)
{
	m_FrameQuery->Begin(e.FrameCounter);

}

void CGameState_World::OnRender(Render3DEventArgs& e)
{
	e.Camera = m_CameraController->GetCamera().operator->(); // TODO: Shit code. Refactor me.
	//Application::Get().GetLoader()->SetCamera(m_CameraController->GetCamera());

	m_3DDeferredTechnique.Render(e);
}

void CGameState_World::OnPostRender(Render3DEventArgs& e)
{
	m_FrameQuery->End(e.FrameCounter);

	vec3 cameraTrans = m_CameraController->GetCamera()->GetTranslation();
	m_CameraPosText->SetText("Pos: " + std::to_string(cameraTrans.x) + ", " + std::to_string(cameraTrans.y) + ", " + std::to_string(cameraTrans.z));

	vec3 cameraRot = m_CameraController->GetCamera()->GetDirection();
	m_CameraRotText->SetText("Rot: " + std::to_string(cameraRot.x) + ", " + std::to_string(cameraRot.y) + ", " + std::to_string(cameraRot.z));

	Query::QueryResult frameResult = m_FrameQuery->GetQueryResult(e.FrameCounter - (m_FrameQuery->GetBufferCount() - 1));
	if (frameResult.IsValid)
	{
		// Frame time in milliseconds
#ifdef  IS_DX11
		m_FrameTime = frameResult.ElapsedTime * 1000.0;
#else
		m_FrameTime = frameResult.ElapsedTime / 1000000.0;
#endif

		Application::Get().GetRenderWindow()->SetWindowName(std::to_string(m_FrameTime));
	}
}

void CGameState_World::OnRenderUI(RenderUIEventArgs& e)
{
	e.Viewport = &m_Viewport;

	//m_UITechnique.RenderUI(e);
}

//
//
//

void CGameState_World::Load3D()
{
	std::shared_ptr<IRenderDevice> renderDevice = GetApplication()->GetRenderDevice();
	std::shared_ptr<RenderWindow> renderWindow = GetApplication()->GetRenderWindow();

	// ADD CODE HERE

	

	//
	// PreRender passes
	//

	m_3DDeferredTechnique.AddPass(std::make_shared<ClearRenderTargetPass>(m_GB->GetRenderTarget(), ClearFlags::All, g_ClearColor, 1.0f, 0));


	//
	// 3D Passes
	//
	//AddSkyPasses(renderDevice, m_GB->GetRenderTarget(), &m_3DDeferredTechnique, &m_Viewport, m_3DScene);
	//AddWDLPasses(renderDevice, m_GB->GetRenderTarget(), &m_3DDeferredTechnique, &m_Viewport, m_3DScene);
	//AddDebugPasses(renderDevice, m_GB->GetRenderTarget(), &m_3DDeferredTechnique, &m_Viewport, m_3DScene);
	//AddMCNKPasses(renderDevice, m_GB->GetRenderTarget(), &m_3DDeferredTechnique, &m_Viewport, m_3DScene);
	//AddWMOPasses(renderDevice, m_GB->GetRenderTarget(), &m_3DDeferredTechnique, &m_Viewport, m_3DScene);
	//AddLiquidPasses(renderDevice, m_GB->GetRenderTarget(), &m_3DDeferredTechnique, &m_Viewport, m_3DScene);
	//AddM2Passes(renderDevice, m_GB->GetRenderTarget(), &m_3DDeferredTechnique, &m_Viewport, m_3DScene);



	/*Light dir;
	dir.m_Enabled = true;
	dir.m_Type = Light::LightType::Directional;
	dir.m_DirectionWS = vec4(0.0, -1.0f, 0.0f, 0.0f);
	dir.m_Color = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	dir.m_Intensity = 0.75f;

	m_DirLight = std::make_shared<CLight3D>();
	m_DirLight->setLight(dir);
	m_3DScene->GetRootNode()->AddLight(m_DirLight);*/


	m_3DDeferredTechnique.AddPass(std::make_shared<ClearRenderTargetPass>(renderWindow->GetRenderTarget(), ClearFlags::All, g_ClearColor, 1.0f, 0));
	m_3DDeferredTechnique.AddPass(m_GB->GetPass());
}

void CGameState_World::LoadUI()
{
	std::shared_ptr<IRenderDevice> renderDevice = GetApplication()->GetRenderDevice();


	// Font
	m_CameraPosText = std::make_shared<CUITextNode>();
	m_CameraPosText->SetParent(m_UIScene->GetRootNode());
	m_CameraPosText->SetText("Camera position");
	m_CameraPosText->SetTranslate(vec2(0.0f, 0.0f));

	m_CameraRotText = std::make_shared<CUITextNode>();
	m_CameraRotText->SetParent(m_UIScene->GetRootNode());
	m_CameraRotText->SetText("Camera rotation");
	m_CameraRotText->SetTranslate(vec2(0.0f, 20.0f));

	// Texture 2
	//std::shared_ptr<UITexture> node4 = std::make_shared<UITexture>();
	//node4->SetParent(m_UIScene->GetRootNode());
	//node4->SetTexture(renderDevice->CreateTexture2D("Textures\\Moon02Glare.blp"));
	//node4->SetTranslate(vec2(200.0f, 000.0f));
	//node4->SetScale(vec2(100.0f, 100.0f));


	// Color quad
	//std::shared_ptr<SceneNodeUI> node2 = std::make_shared<SceneNodeUI>();
	//node2->SetParent(m_UIScene->GetRootNode());
	//node2->SetMesh(__geom);
	//node2->SetTranslate(vec2(180.0f, 180.0f));
	//node2->SetScale(vec2(10.0f, 10.0f));

	//
	// UI Passes
	//
	AddUIPasses(renderDevice, GetApplication()->GetRenderWindow()->GetRenderTarget(), &m_UITechnique, m_Viewport, m_UIScene);
}
