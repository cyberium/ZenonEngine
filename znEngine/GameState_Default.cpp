#include "stdafx.h"

// Gerenal
#include "GameState_Default.h"

// Additional
#include "CameraControllers/FreeCameraController.h"

#include "Materials/MaterialDebug.h"
#include "Materials/MaterialTextured.h"

#include "SceneFunctional/3D/SceneNode3D.h"

#include "Passes/ClearRenderTargetPass.h"

#include "CreatePasses.h"

CGameState_World::CGameState_World(IBaseManager * BaseManager, std::shared_ptr<IRenderWindow> RenderWindow)
	: CGameState(BaseManager, RenderWindow)
{}

CGameState_World::~CGameState_World()
{}


//
// IGameState
//
bool CGameState_World::Init()
{
	CGameState::Init();

	SetCameraController(std::make_shared<CFreeCameraController>());
	GetCameraController()->GetCamera()->SetProjection(ICamera::ProjectionHand::Right, 45.0f, GetRenderWindow()->GetWindowWidth() / GetRenderWindow()->GetWindowHeight(), 0.5f, 4000.0f);

	Load3D();
	LoadUI();

	GetCameraController()->GetCameraMovement()->SetTranslate(vec3(-104, 105, 105));
	GetCameraController()->GetCameraMovement()->SetYaw(-45);
	GetCameraController()->GetCameraMovement()->SetPitch(-45);

	return true;
}

void CGameState_World::Destroy()
{
	// Insert code here

	CGameState::Destroy();
}


//
//
//

void CGameState_World::OnResize(ResizeEventArgs& e)
{
	CGameState::OnResize(e);
}

void CGameState_World::OnPreRender(RenderEventArgs& e)
{
	CGameState::OnPreRender(e);
}

void CGameState_World::OnRender(RenderEventArgs& e)
{
	CGameState::OnRender(e);
}

void CGameState_World::OnPostRender(RenderEventArgs& e)
{
	CGameState::OnPostRender(e);


}

void CGameState_World::OnRenderUI(RenderEventArgs& e)
{
	CGameState::OnRenderUI(e);
}

//
//
//

void CGameState_World::Load3D()
{
	for (size_t i = 0; i < 10; i++)
	{
		for (size_t j = 0; j < 10; j++)
		{
			std::shared_ptr<SceneNode3D> sceneNode = m_Scene->CreateSceneNode<SceneNode3D>(m_Scene->GetRootNode());

			std::shared_ptr<IMesh> mesh = GetRenderDevice()->GetPrimitiveCollection()->CreatePlane();

			std::shared_ptr<MaterialTextured> mat = std::make_shared<MaterialTextured>(GetRenderDevice());
			mat->SetDiffuseColor(vec4(1.0f, 0.0f, 1.0f, 1.0f));
			mat->SetTexture(0, GetRenderDevice()->CreateTexture2D("default.png"));
			mesh->SetMaterial(mat);

			sceneNode->GetComponent<ITransformComponent3D>()->SetTranslate(vec3(40 * i, 0.0f, 40 * j));
			sceneNode->GetComponent<ITransformComponent3D>()->SetScale(vec3(15, 15, 15));
			sceneNode->GetComponent<IMeshComponent3D>()->AddMesh(mesh);
		}
	}

	//CFBX fbx(m_3DScene->GetRootNode());


	m_Technique.AddPass(std::make_shared<ClearRenderTargetPass>(GetRenderDevice(), GetRenderWindow()->GetRenderTarget(), ClearFlags::All, g_ClearColor, 1.0f, 0));
	Add3DPasses(GetRenderDevice(), GetRenderWindow()->GetRenderTarget(), &m_Technique, GetRenderWindow()->GetViewport(), m_Scene);
}

void CGameState_World::LoadUI()
{
	AddUIPasses(GetRenderDevice(), GetRenderWindow()->GetRenderTarget(), &m_Technique, GetRenderWindow()->GetViewport(), m_Scene);
}