#include "stdafx.h"

// Gerenal
#include "GameState_Editor.h"


CGameState_Editor::CGameState_Editor(IBaseManager * BaseManager, std::shared_ptr<IRenderWindow> RenderWindow, MainEditor * Editor)
	: CGameState(BaseManager, RenderWindow)
	, m_Editor(Editor)
{}

CGameState_Editor::~CGameState_Editor()
{}


//
// IGameState
//
bool CGameState_Editor::Init()
{
	CGameState::Init();

	SetCameraController(std::make_shared<CFreeCameraController>());
	GetCameraController()->GetCamera()->SetProjection(ICamera::ProjectionHand::Right, 45.0f, GetRenderWindow()->GetWindowWidth() / GetRenderWindow()->GetWindowHeight(), 0.5f, 10000.0f);

	Load3D();
	LoadUI();

	GetCameraController()->GetCameraMovement()->SetTranslate(vec3(-500, 1600, 1700));
	GetCameraController()->GetCameraMovement()->SetYaw(-51);
	GetCameraController()->GetCameraMovement()->SetPitch(-38);

	return true;
}

void CGameState_Editor::Destroy()
{
	// Insert code here

	CGameState::Destroy();
}

void CGameState_Editor::OnRayIntersected(const glm::vec3& Point)
{
	std::shared_ptr<MaterialDebug> matDebug = std::make_shared<MaterialDebug>(GetRenderDevice());
	matDebug->SetDiffuseColor(vec4(1.0f, 0.0f, 0.0f, 1.0f));
	matDebug->SetWrapper(matDebug);

	std::shared_ptr<IMesh> meshPlane = GetRenderDevice()->GetPrimitiveCollection()->CreateSphere();
	meshPlane->SetMaterial(matDebug);

	std::shared_ptr<ISceneNode> sceneNodePlane = m_Scene3D->CreateWrappedSceneNode<SceneNode3D>("SceneNode3D", m_Scene3D->GetRootNode());
	sceneNodePlane->SetName("Sphere.");
	sceneNodePlane->GetComponent<ITransformComponent3D>()->SetTranslate(Point);
	sceneNodePlane->GetComponent<ITransformComponent3D>()->SetScale(vec3(50.0f, 50.0f, 50.0f));
	sceneNodePlane->GetComponent<IMeshComponent3D>()->AddMesh(meshPlane);

	Log::Green("Sphere created at %f %f %f", Point.x, Point.y, Point.z);

	m_Editor->OnSceneNodeSelected(sceneNodePlane);
}


//
//
//

void CGameState_Editor::OnResize(ResizeEventArgs& e)
{
	CGameState::OnResize(e);
}

void CGameState_Editor::OnPreRender(RenderEventArgs& e)
{
	CGameState::OnPreRender(e);
}

void CGameState_Editor::OnRender(RenderEventArgs& e)
{
	CGameState::OnRender(e);
}

void CGameState_Editor::OnPostRender(RenderEventArgs& e)
{
	CGameState::OnPostRender(e);
}

void CGameState_Editor::OnRenderUI(RenderEventArgs& e)
{
	CGameState::OnRenderUI(e);
}

//
//
//
void CGameState_Editor::Load3D()
{
	std::shared_ptr<MaterialDebug> matDebug = std::make_shared<MaterialDebug>(GetRenderDevice());
	matDebug->SetDiffuseColor(vec4(0.0f, 1.0f, 1.0f, 1.0f));
	matDebug->SetWrapper(matDebug);

	std::shared_ptr<IMesh> meshPlane = GetRenderDevice()->GetPrimitiveCollection()->CreatePlane();
	meshPlane->SetMaterial(matDebug);

	std::shared_ptr<ISceneNode> sceneNodePlane = m_Scene3D->CreateWrappedSceneNode<SceneNode3D>("SceneNode3D", m_Scene3D->GetRootNode());
	sceneNodePlane->SetName("Ground.");
	sceneNodePlane->GetComponent<ITransformComponent3D>()->SetScale(vec3(1000.0f, 1.0f, 1000.0f));
	sceneNodePlane->GetComponent<IMeshComponent3D>()->AddMesh(meshPlane);

	//---------------------------

	const size_t iterCnt = 1;
	const float offset = 45.0f;

	std::shared_ptr<MaterialTextured> mat = std::make_shared<MaterialTextured>(GetRenderDevice());
	mat->SetDiffuseColor(vec4(1.0f, 0.0f, 1.0f, 1.0f));
	mat->SetTexture(0, GetRenderDevice()->CreateTexture2D("default.png"));
	mat->SetWrapper(mat);

	std::shared_ptr<IMesh> mesh = GetRenderDevice()->GetPrimitiveCollection()->CreateSphere();
	mesh->SetMaterial(mat);

	for (size_t i = 0; i < iterCnt; i++)
	{
		for (size_t j = 0; j < iterCnt; j++)
		{
			for (size_t k = 0; k < iterCnt; k++)
			{
				std::shared_ptr<ISceneNode> sceneNode = m_Scene3D->CreateWrappedSceneNode<SceneNode3D>("SceneNode3D", m_Scene3D->GetRootNode());
				sceneNode->SetName("Ball [" + std::to_string(i) + ", " + std::to_string(j) + ", " + std::to_string(k) + "]");
				sceneNode->GetComponent<ITransformComponent3D>()->SetTranslate(vec3(offset * i, offset * k, offset * j));
				sceneNode->GetComponent<ITransformComponent3D>()->SetScale(vec3(15, 15, 15));
				sceneNode->GetComponent<IMeshComponent3D>()->AddMesh(mesh);

				BoundingBox bbox = BoundingBox(glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
				bbox.transform(sceneNode->GetComponent<CTransformComponent3D>()->GetWorldTransfom());
				sceneNode->GetComponent<CColliderComponent3D>()->SetBounds(bbox);
			}
		}
	}

	m_Technique3D.AddPass(GetManager<IRenderPassFactory>(GetBaseManager())->CreateRenderPass("ClearPass", GetRenderDevice(), GetRenderWindow()->GetRenderTarget(), GetRenderWindow()->GetViewport(), m_Scene3D));
	m_Technique3D.AddPass(GetManager<IRenderPassFactory>(GetBaseManager())->CreateRenderPass("DebugPass", GetRenderDevice(), GetRenderWindow()->GetRenderTarget(), GetRenderWindow()->GetViewport(), m_Scene3D));
	m_Technique3D.AddPass(GetManager<IRenderPassFactory>(GetBaseManager())->CreateRenderPass("TexturedMaterialPass", GetRenderDevice(), GetRenderWindow()->GetRenderTarget(), GetRenderWindow()->GetViewport(), m_Scene3D));
	m_Technique3D.AddPass(GetManager<IRenderPassFactory>(GetBaseManager())->CreateRenderPass("OBJPass", GetRenderDevice(), GetRenderWindow()->GetRenderTarget(), GetRenderWindow()->GetViewport(), m_Scene3D));
	m_Technique3D.AddPass(GetManager<IRenderPassFactory>(GetBaseManager())->CreateRenderPass("FBXPass", GetRenderDevice(), GetRenderWindow()->GetRenderTarget(), GetRenderWindow()->GetViewport(), m_Scene3D));
}

void CGameState_Editor::LoadUI()
{
	m_TechniqueUI.AddPass(GetManager<IRenderPassFactory>(GetBaseManager())->CreateRenderPass("BaseUIPass", GetRenderDevice(), GetRenderWindow()->GetRenderTarget(), GetRenderWindow()->GetViewport(), m_SceneUI));
}
