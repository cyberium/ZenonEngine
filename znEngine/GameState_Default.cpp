#include "stdafx.h"

// Gerenal
#include "GameState_Default.h"

// Additional
#include "Materials/MaterialDebug.h"
#include "Materials/MaterialTextured.h"

#include "Models/OBJLoader.h"

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
	const size_t iterCnt = 5;
	const float offset = 45.0f;

	std::shared_ptr<MaterialDebug> matDebug = std::make_shared<MaterialDebug>(GetRenderDevice());
	matDebug->SetDiffuseColor(vec4(0.0f, 1.0f, 1.0f, 1.0f));
	matDebug->SetWrapper(matDebug);

	std::shared_ptr<MaterialTextured> mat = std::make_shared<MaterialTextured>(GetRenderDevice());
	mat->SetDiffuseColor(vec4(1.0f, 0.0f, 1.0f, 1.0f));
	mat->SetTexture(0, GetRenderDevice()->CreateTexture2D("default.png"));
	mat->SetWrapper(mat);

	//std::shared_ptr<IMesh> mesh = GetRenderDevice()->GetPrimitiveCollection()->CreateSphere();
	//mesh->SetMaterial(mat);

	/*for (size_t i = 0; i < iterCnt; i++)
	{
		for (size_t j = 0; j < iterCnt; j++)
		{
			for (size_t k = 0; k < iterCnt; k++)
			{
				std::shared_ptr<ISceneNode> sceneNode = m_Scene3D->CreateWrappedSceneNode<SceneNode3D>("SceneNode3D", m_Scene3D->GetRootNode());
				sceneNode->GetComponent<ITransformComponent3D>()->SetTranslate(vec3(offset * i, offset * k, offset * j));
				sceneNode->GetComponent<ITransformComponent3D>()->SetScale(vec3(15, 15, 15));
				sceneNode->GetComponent<IMeshComponent3D>()->AddMesh(mesh);

				BoundingBox bbox = BoundingBox(glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
				bbox.transform(sceneNode->GetComponent<CTransformComponent3D>()->GetWorldTransfom());
				sceneNode->GetComponent<CColliderComponent3D>()->SetBounds(bbox);
			}
		}
	}*/

	GetRenderDevice()->CreateTexture2D("D:\\_programming\\ZenonEngine\\gamedata\\BuildingTextures\\Pavement_Cobblestone_01_b_Diff.png");

	

	//LoadObj("D:\\_programming\\ZenonEngine\\gamedata\\interior.obj");
	//LoadObj("D:\\_programming\\ZenonEngine\\gamedata\\exterior.obj");

	

	std::shared_ptr<ISceneNode> fbxSceneNode = GetManager<ISceneNodesFactory>(GetBaseManager())->CreateSceneNode(m_Scene3D->GetRootNode(), "FBXSceneNode");
	//fbxSceneNode->GetComponent<ITransformComponent3D>()->SetScale(vec3(15.0f, 15.0f, 15.0f));

	for (auto& m : fbxSceneNode->GetComponent<IMeshComponent3D>()->GetMeshes())
	{
		if (m->GetMaterial() == nullptr)
		{
			m->SetMaterial(mat);
		}
		else
		{
			(void*)m->GetMaterial().get();
		}
	}

	m_Technique3D.AddPass(GetManager<IRenderPassFactory>(GetBaseManager())->CreateRenderPass("ClearPass", GetRenderDevice(), GetRenderWindow()->GetRenderTarget(), GetRenderWindow()->GetViewport(), m_Scene3D));
	m_Technique3D.AddPass(GetManager<IRenderPassFactory>(GetBaseManager())->CreateRenderPass("DebugPass", GetRenderDevice(), GetRenderWindow()->GetRenderTarget(), GetRenderWindow()->GetViewport(), m_Scene3D));
	m_Technique3D.AddPass(GetManager<IRenderPassFactory>(GetBaseManager())->CreateRenderPass("TexturedMaterialPass", GetRenderDevice(), GetRenderWindow()->GetRenderTarget(), GetRenderWindow()->GetViewport(), m_Scene3D));
	m_Technique3D.AddPass(GetManager<IRenderPassFactory>(GetBaseManager())->CreateRenderPass("OBJPass", GetRenderDevice(), GetRenderWindow()->GetRenderTarget(), GetRenderWindow()->GetViewport(), m_Scene3D));
	m_Technique3D.AddPass(GetManager<IRenderPassFactory>(GetBaseManager())->CreateRenderPass("FBXPass", GetRenderDevice(), GetRenderWindow()->GetRenderTarget(), GetRenderWindow()->GetViewport(), m_Scene3D));
}

void CGameState_World::LoadUI()
{
	m_TechniqueUI.AddPass(GetManager<IRenderPassFactory>(GetBaseManager())->CreateRenderPass("BaseUIPass", GetRenderDevice(), GetRenderWindow()->GetRenderTarget(), GetRenderWindow()->GetViewport(), m_SceneUI));
}

void CGameState_World::LoadObj(const std::string& ObjFilename)
{
	objl::Loader l(GetRenderDevice());
	l.LoadFile(GetManager<IFilesManager>(GetBaseManager())->Open(ObjFilename));

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
}
