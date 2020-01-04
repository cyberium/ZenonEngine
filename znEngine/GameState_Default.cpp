#include "stdafx.h"

// Gerenal
#include "GameState_Default.h"

// Additional
#include "Materials/MaterialDebug.h"
#include "Materials/MaterialTextured.h"

CGameState_World::CGameState_World(IBaseManager * BaseManager, std::shared_ptr<IRenderWindow> RenderWindow, IWindowEvents* WindowEvents)
	: CGameState(BaseManager, RenderWindow, WindowEvents)
{}

CGameState_World::~CGameState_World()
{}


//
// IGameState
//
bool CGameState_World::Init()
{
	CGameState::Init();

	std::shared_ptr<CSceneNodeCamera> cameraNode = m_Scene3D->CreateSceneNode<CSceneNodeCamera>(m_Scene3D->GetRootNode());

	SetCameraController(std::make_shared<CFreeCameraController>());
	GetCameraController()->SetCamera(cameraNode);
	GetCameraController()->GetCamera()->SetPerspectiveProjection(ICamera::EPerspectiveProjectionHand::Right, 45.0f, GetRenderWindow()->GetWindowWidth() / GetRenderWindow()->GetWindowHeight(), 0.5f, 10000.0f);

	Load3D();
	LoadUI();

	GetCameraController()->GetCameraMovement()->SetTranslate(vec3(-500, 1600, 1700));
	GetCameraController()->GetCameraMovement()->SetYaw(-51);
	GetCameraController()->GetCameraMovement()->SetPitch(-38);

	return true;
}

void CGameState_World::Destroy()
{
	// Insert code here

	CGameState::Destroy();
}

void CGameState_World::OnRayIntersected(const glm::vec3& Point)
{
	std::shared_ptr<MaterialDebug> matDebug = std::make_shared<MaterialDebug>(GetRenderDevice());
	matDebug->SetDiffuseColor(vec4(1.0f, 0.0f, 0.0f, 1.0f));
	matDebug->SetWrapper(matDebug);

	std::shared_ptr<IMesh> meshPlane = GetRenderDevice()->GetPrimitiveCollection()->CreateSphere();
	meshPlane->SetMaterial(matDebug);

	std::shared_ptr<ISceneNode> sceneNodePlane = m_Scene3D->CreateWrappedSceneNode<SceneNode3D>("SceneNode3D", m_Scene3D->GetRootNode());
	sceneNodePlane->SetName("Sphere.");
	std::dynamic_pointer_cast<ISceneNode3D>(sceneNodePlane)->SetTranslate(Point);
	std::dynamic_pointer_cast<ISceneNode3D>(sceneNodePlane)->SetScale(vec3(50.0f, 50.0f, 50.0f));
	sceneNodePlane->GetComponent<IMeshComponent3D>()->AddMesh(meshPlane);

	Log::Green("Sphere created at %f %f %f", Point.x, Point.y, Point.z);
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
// Keyboard events
//
void CGameState_World::OnKeyPressed(KeyEventArgs & e)
{
	if (e.Key == KeyCode::F4)
		m_FBX_Opaque_Pass->SetEnabled(! m_FBX_Opaque_Pass->IsEnabled());

	if (e.Key == KeyCode::F5)
		m_FBX_Transperent_Pass->SetEnabled(! m_FBX_Transperent_Pass->IsEnabled());

	CGameState::OnKeyPressed(e);
}

void CGameState_World::OnKeyReleased(KeyEventArgs & e)
{
	CGameState::OnKeyReleased(e);
}



//
//
//
void CGameState_World::Load3D()
{
	std::shared_ptr<MaterialDebug> matDebug = std::make_shared<MaterialDebug>(GetRenderDevice());
	matDebug->SetDiffuseColor(vec4(0.0f, 1.0f, 1.0f, 1.0f));
	matDebug->SetWrapper(matDebug);

	std::shared_ptr<IMesh> meshPlane = GetRenderDevice()->GetPrimitiveCollection()->CreatePlane();
	meshPlane->SetMaterial(matDebug);

	//std::shared_ptr<ISceneNode> sceneNodePlane = m_Scene3D->CreateWrappedSceneNode<SceneNode3D>("SceneNode3D", m_Scene3D->GetRootNode());
	//sceneNodePlane->SetName("Ground.");
	//std::dynamic_pointer_cast<ISceneNode3D>(sceneNodePlane)->SetScale(vec3(1000.0f, 1.0f, 1000.0f));
	//sceneNodePlane->GetComponent<IMeshComponent3D>()->AddMesh(meshPlane);


	//-- Assimp -----------------

	//CAssimpLoader assLoader(m_Scene3D->GetRootNode(), GetRenderDevice());
	//assLoader.LoadFromFile(GetBaseManager()->GetManager<IFilesManager>()->Open("AmazonScene\\interior.obj"));

	//---------------------------

	const size_t iterCnt = 0;
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
				std::dynamic_pointer_cast<ISceneNode3D>(sceneNode)->SetTranslate(vec3(offset * i, offset * k, offset * j));
				std::dynamic_pointer_cast<ISceneNode3D>(sceneNode)->SetScale(vec3(15, 15, 15));
				sceneNode->GetComponent<IMeshComponent3D>()->AddMesh(mesh);

				BoundingBox bbox = BoundingBox(glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
				bbox.transform(sceneNode->GetWorldTransfom());
				sceneNode->GetComponent<CColliderComponent3D>()->SetBounds(bbox);
			}
		}
	}

	GetRenderDevice()->CreateTexture2D("D:\\_programming\\ZenonEngine\\gamedata\\BuildingTextures\\Pavement_Cobblestone_01_b_Diff.png");

	

	//LoadObj("D:\\_programming\\ZenonEngine\\gamedata\\interior.obj");
	//LoadObj("D:\\_programming\\ZenonEngine\\gamedata\\exterior.obj");

	

	std::shared_ptr<ISceneNode> fbxSceneNode = GetBaseManager()->GetManager<ISceneNodesFactory>()->CreateSceneNode(m_Scene3D->GetRootNode(), "FBXSceneNode");
	//fbxSceneNode->GetComponent<ITransformComponent3D>()->SetScale(vec3(15.0f, 15.0f, 15.0f));

	m_Technique3D.AddPass(GetBaseManager()->GetManager<IRenderPassFactory>()->CreateRenderPass("ClearPass", GetRenderDevice(), GetRenderWindow()->GetRenderTarget(), GetRenderWindow()->GetViewport(), m_Scene3D));
	m_Technique3D.AddPass(GetBaseManager()->GetManager<IRenderPassFactory>()->CreateRenderPass("DebugPass", GetRenderDevice(), GetRenderWindow()->GetRenderTarget(), GetRenderWindow()->GetViewport(), m_Scene3D));
	m_Technique3D.AddPass(GetBaseManager()->GetManager<IRenderPassFactory>()->CreateRenderPass("TexturedMaterialPass", GetRenderDevice(), GetRenderWindow()->GetRenderTarget(), GetRenderWindow()->GetViewport(), m_Scene3D));
	//m_Technique3D.AddPass(GetBaseManager()->GetManager<IRenderPassFactory>()->CreateRenderPass("OBJPass", GetRenderDevice(), GetRenderWindow()->GetRenderTarget(), GetRenderWindow()->GetViewport(), m_Scene3D));
	
	m_FBX_Opaque_Pass = GetBaseManager()->GetManager<IRenderPassFactory>()->CreateRenderPass("FBXPassOpaque", GetRenderDevice(), GetRenderWindow()->GetRenderTarget(), GetRenderWindow()->GetViewport(), m_Scene3D);
	m_Technique3D.AddPass(m_FBX_Opaque_Pass);
	
	m_FBX_Transperent_Pass = GetBaseManager()->GetManager<IRenderPassFactory>()->CreateRenderPass("FBXPassTransperent", GetRenderDevice(), GetRenderWindow()->GetRenderTarget(), GetRenderWindow()->GetViewport(), m_Scene3D);
	m_Technique3D.AddPass(m_FBX_Transperent_Pass);
}

void CGameState_World::LoadUI()
{
	m_TechniqueUI.AddPass(GetBaseManager()->GetManager<IRenderPassFactory>()->CreateRenderPass("BaseUIPass", GetRenderDevice(), GetRenderWindow()->GetRenderTarget(), GetRenderWindow()->GetViewport(), m_SceneUI));
}


/*void CGameState_World::LoadObj(const std::string& ObjFilename)
{
	objl::Loader l(GetRenderDevice());
	l.LoadFile(GetBaseManager()->GetManager<IFilesManager>()->Open(ObjFilename));

	for (const auto& mesh : l.LoadedMeshes)
	{
		std::shared_ptr<IMesh> mesh2 = GetRenderDevice()->CreateMesh();
		mesh2->AddVertexBuffer(BufferBinding("POSITION", 0), GetRenderDevice()->CreateVoidVertexBuffer(mesh.Vertices.data(), mesh.Vertices.size(), 0, sizeof(objl::Vertex)));
		mesh2->AddVertexBuffer(BufferBinding("NORMAL", 0), GetRenderDevice()->CreateVoidVertexBuffer(mesh.Vertices.data(), mesh.Vertices.size(), 12, sizeof(objl::Vertex)));
		mesh2->AddVertexBuffer(BufferBinding("TEXCOORD", 0), GetRenderDevice()->CreateVoidVertexBuffer(mesh.Vertices.data(), mesh.Vertices.size(), 24, sizeof(objl::Vertex)));

		mesh2->SetIndexBuffer(GetRenderDevice()->CreateVoidIndexBuffer(mesh.Indices.data(), mesh.Indices.size(), 0, sizeof(uint32)));
		mesh2->SetMaterial(mesh.MeshMaterial);
		
		std::shared_ptr<ISceneNode> sceneNode = m_Scene3D->CreateWrappedSceneNode<SceneNode3D>("SceneNode3D", m_Scene3D->GetRootNode());
		//sceneNode->GetComponent<ITransformComponent3D>()->SetTranslate(vec3(offset * i, offset * k, offset * j));
		//sceneNode->GetComponent<ITransformComponent3D>()->SetScale(vec3(15, 15, 15));
		sceneNode->GetComponent<IMeshComponent3D>()->AddMesh(mesh2);
	}
}*/
