#include "stdafx.h"

// Gerenal
#include "Editor3DFrame.h"

// Additional
#include <filesystem>
namespace fs = std::experimental::filesystem;

namespace
{
	std::string str_tolower(std::string s)
	{
		std::transform(s.begin(), s.end(), s.begin(), [](char c)
		{
			return std::tolower(c, std::locale()); }
		);
		return s;
	}

	std::vector<std::string> GetAllFilesInDirectory(const std::string& Directory, const std::vector<std::string> DirSkipList = { }, const std::string& FileExtention = "")
	{
		std::vector<std::string> listOfFiles;

		try
		{
			bool isExists = fs::exists(Directory);
			bool isDir = fs::is_directory(Directory);

			// Check if given path exists and points to a directory
			if (isExists && isDir)
			{
				// Create a Recursive Directory Iterator object and points to the starting of directory
				fs::recursive_directory_iterator iter(Directory);

				// Create a Recursive Directory Iterator object pointing to end.
				fs::recursive_directory_iterator end;

				// Iterate till end
				while (iter != end)
				{
					// Check if current entry is a directory and if exists in skip list
					if (fs::is_directory(iter->path()) && (std::find(DirSkipList.begin(), DirSkipList.end(), iter->path().filename()) != DirSkipList.end()))
					{
						iter.disable_recursion_pending();
					}
					else if (!fs::is_directory(iter->path()) && (!FileExtention.empty()) && (iter->path().has_extension()) && (str_tolower(iter->path().extension().string()) == str_tolower(FileExtention)))
					{
						listOfFiles.push_back(iter->path().string());
					}
					else
					{
						iter.disable_recursion_pending();
					}

					std::error_code ec;
					// Increment the iterator to point to next entry in recursive iteration
					iter.increment(ec);
					if (ec)
					{
						Log::Error("GetAllFilesInDirectory: Error while accessing '%s'. Error: '%s'.", iter->path().string().c_str(), ec.message().c_str());
					}
				}
			}
		}
		catch (const std::system_error& e)
		{
			Log::Error("GetAllFilesInDirectory: Exception '%s'", e.what());
		}

		return listOfFiles;
	}
}

CSceneEditor::CSceneEditor(IBaseManager& BaseManager)
	: SceneBase(BaseManager)
{
}

CSceneEditor::~CSceneEditor()
{
}


//
// IGameState
//
void CSceneEditor::Initialize()
{
	SceneBase::Initialize();

	auto cameraNode = GetRootNode3D()->CreateSceneNode<SceneNode3D>();
	cameraNode->AddComponent(std::make_shared<CCameraComponent3D>(*cameraNode));

	SetCameraController(std::make_shared<CFreeCameraController>());
	GetCameraController()->SetCamera(cameraNode->GetComponent<ICameraComponent3D>());
	GetCameraController()->GetCamera()->SetPerspectiveProjection(ICameraComponent3D::EPerspectiveProjectionHand::Right, 45.0f, static_cast<float>(GetRenderWindow()->GetWindowWidth()) / static_cast<float>(GetRenderWindow()->GetWindowHeight()), 0.5f, 10000.0f);

	Load3D();
	LoadUI();

	cameraNode->SetTranslate(glm::vec3(-50, 160, 170));
	GetCameraController()->GetCamera()->SetYaw(-51);
	GetCameraController()->GetCamera()->SetPitch(-38);
}

void CSceneEditor::Finalize()
{
	// Insert code here

	SceneBase::Finalize();
}

void CSceneEditor::OnMouseClickToWorld(MouseButtonEventArgs::MouseButton & MouseButton, const glm::vec2 & MousePosition, const Ray & RayToWorld)
{
	auto node = FindIntersection(RayToWorld);
	if (node == nullptr)
		return;
	node->SetTranslate(node->GetTranslation() + glm::vec3(0.0f, 1.0f, 0.0f));
}

void CSceneEditor::OnMouseMoveToWorld(MouseButtonEventArgs::MouseButton & MouseButton, const glm::vec2& MousePosition, const Ray & RayToWorld)
{
	if (MouseButton == MouseButtonEventArgs::MouseButton::Left)
	{
		glm::vec3 resultPosition = GetCameraController()->RayToPlane(RayToWorld, Plane(glm::vec3(0.0f, 1.0f, 0.0f), 0.0f));
		resultPosition /= 10.0f;
		resultPosition = glm::round(resultPosition);
		resultPosition *= 10.0f;

		sceneNodeParentt->SetTranslate(resultPosition);
	}
}




//
//
//
void CSceneEditor::OnPreRender(RenderEventArgs& e)
{
	//m_World.update(e.DeltaTime / 1000.0f * 2.0f);

	//m_RootForBoxes->SetRotation(glm::vec3(m_RootForBoxes->GetRotation().x, m_RootForBoxes->GetRotation().y + 0.01, 0.0f));

	SceneBase::OnPreRender(e);
}



//
// Keyboard events
//
bool CSceneEditor::OnWindowKeyPressed(KeyEventArgs & e)
{
	//if (e.Key == KeyCode::F4)
	//	m_Model_Pass_Opaque->SetEnabled(!m_Model_Pass_Opaque->IsEnabled());

	//if (e.Key == KeyCode::F5)
	//	m_Model_Pass_Transperent->SetEnabled(!m_Model_Pass_Transperent->IsEnabled());

	return SceneBase::OnWindowKeyPressed(e);
}

void CSceneEditor::OnWindowKeyReleased(KeyEventArgs & e)
{
	SceneBase::OnWindowKeyReleased(e);
}



//
//
//
void CSceneEditor::Load3D()
{
	{
		auto sceneNodeLight = GetRootNode3D()->CreateSceneNode<SceneNode3D>();
		sceneNodeLight->SetName("Light node");
		sceneNodeLight->SetTranslate(glm::vec3(80.0f, 600.0f, 80.0f));
		sceneNodeLight->SetRotation(glm::vec3(0.0f, -1.0f, 0.0f));

		sceneNodeLight->GetComponent<ILightComponent3D>()->SetType(ELightType::Spot);
		sceneNodeLight->GetComponent<ILightComponent3D>()->SetColor(glm::vec3(1.0f, 1.0f, 1.0f));
		sceneNodeLight->GetComponent<ILightComponent3D>()->SetRange(48000.0f);
		sceneNodeLight->GetComponent<ILightComponent3D>()->SetIntensity(1.0f);
		sceneNodeLight->GetComponent<ILightComponent3D>()->SetSpotlightAngle(55.0f);
	}

	auto fileNames = GetAllFilesInDirectory("C:\\_engine\\ZenonEngine_gamedata\\natureKit\\models\\fbxformat", {}, ".fbx");

	std::vector<std::string> modelsList;
	modelsList.push_back("C:\\_engine\\ZenonEngine_gamedata\\natureKit\\models\\fbxformat\\ground_dirt.fbx");
	modelsList.push_back("C:\\_engine\\ZenonEngine_gamedata\\natureKit\\models\\fbxformat\\ground_dirtRiver.fbx");
	modelsList.push_back("C:\\_engine\\ZenonEngine_gamedata\\natureKit\\models\\fbxformat\\ground_dirtRiverBanks.fbx");
	modelsList.push_back("C:\\_engine\\ZenonEngine_gamedata\\natureKit\\models\\fbxformat\\ground_dirtRiverCorner.fbx");
	modelsList.push_back("C:\\_engine\\ZenonEngine_gamedata\\natureKit\\models\\fbxformat\\ground_dirtRiverCornerBank.fbx");
	modelsList.push_back("C:\\_engine\\ZenonEngine_gamedata\\natureKit\\models\\fbxformat\\ground_dirtRiverCornerInner.fbx");
	modelsList.push_back("C:\\_engine\\ZenonEngine_gamedata\\natureKit\\models\\fbxformat\\ground_dirtRiverCrossing.fbx");
	modelsList.push_back("C:\\_engine\\ZenonEngine_gamedata\\natureKit\\models\\fbxformat\\ground_dirtRiverEnd.fbx");
	modelsList.push_back("C:\\_engine\\ZenonEngine_gamedata\\natureKit\\models\\fbxformat\\ground_dirtRiverEntrance.fbx");
	modelsList.push_back("C:\\_engine\\ZenonEngine_gamedata\\natureKit\\models\\fbxformat\\ground_dirtRiverRocks.fbx");
	modelsList.push_back("C:\\_engine\\ZenonEngine_gamedata\\natureKit\\models\\fbxformat\\ground_dirtRiverSide.fbx");
	modelsList.push_back("C:\\_engine\\ZenonEngine_gamedata\\natureKit\\models\\fbxformat\\ground_dirtRiverSideCorner.fbx");
	modelsList.push_back("C:\\_engine\\ZenonEngine_gamedata\\natureKit\\models\\fbxformat\\ground_dirtRiverT.fbx");
	modelsList.push_back("C:\\_engine\\ZenonEngine_gamedata\\natureKit\\models\\fbxformat\\ground_dirtRiverTile.fbx");
	modelsList.push_back("C:\\_engine\\ZenonEngine_gamedata\\natureKit\\models\\fbxformat\\ground_dirtRiverWater.fbx");
	modelsList.push_back("C:\\_engine\\ZenonEngine_gamedata\\natureKit\\models\\fbxformat\\ground_grass.fbx");
	modelsList.push_back("C:\\_engine\\ZenonEngine_gamedata\\natureKit\\models\\fbxformat\\ground_pathBend.fbx");
	modelsList.push_back("C:\\_engine\\ZenonEngine_gamedata\\natureKit\\models\\fbxformat\\ground_pathBendBank.fbx");
	modelsList.push_back("C:\\_engine\\ZenonEngine_gamedata\\natureKit\\models\\fbxformat\\ground_pathCorner.fbx");
	modelsList.push_back("C:\\_engine\\ZenonEngine_gamedata\\natureKit\\models\\fbxformat\\ground_pathCornerSmall.fbx");
	modelsList.push_back("C:\\_engine\\ZenonEngine_gamedata\\natureKit\\models\\fbxformat\\ground_pathCross.fbx");
	modelsList.push_back("C:\\_engine\\ZenonEngine_gamedata\\natureKit\\models\\fbxformat\\ground_pathEnd.fbx");
	modelsList.push_back("C:\\_engine\\ZenonEngine_gamedata\\natureKit\\models\\fbxformat\\ground_pathEndClosed.fbx");
	modelsList.push_back("C:\\_engine\\ZenonEngine_gamedata\\natureKit\\models\\fbxformat\\ground_pathOpen.fbx");
	modelsList.push_back("C:\\_engine\\ZenonEngine_gamedata\\natureKit\\models\\fbxformat\\ground_pathRocks.fbx");
	modelsList.push_back("C:\\_engine\\ZenonEngine_gamedata\\natureKit\\models\\fbxformat\\ground_pathSide.fbx");
	modelsList.push_back("C:\\_engine\\ZenonEngine_gamedata\\natureKit\\models\\fbxformat\\ground_pathSideOpen.fbx");
	modelsList.push_back("C:\\_engine\\ZenonEngine_gamedata\\natureKit\\models\\fbxformat\\ground_pathSplit.fbx");
	modelsList.push_back("C:\\_engine\\ZenonEngine_gamedata\\natureKit\\models\\fbxformat\\ground_pathStraight.fbx");
	modelsList.push_back("C:\\_engine\\ZenonEngine_gamedata\\natureKit\\models\\fbxformat\\ground_pathTile.fbx");
	modelsList.push_back("C:\\_engine\\ZenonEngine_gamedata\\natureKit\\models\\fbxformat\\ground_riverBend.fbx");
	modelsList.push_back("C:\\_engine\\ZenonEngine_gamedata\\natureKit\\models\\fbxformat\\ground_riverBendBank.fbx");
	modelsList.push_back("C:\\_engine\\ZenonEngine_gamedata\\natureKit\\models\\fbxformat\\ground_riverCorner.fbx");
	modelsList.push_back("C:\\_engine\\ZenonEngine_gamedata\\natureKit\\models\\fbxformat\\ground_riverCornerSmall.fbx");
	modelsList.push_back("C:\\_engine\\ZenonEngine_gamedata\\natureKit\\models\\fbxformat\\ground_riverCross.fbx");
	modelsList.push_back("C:\\_engine\\ZenonEngine_gamedata\\natureKit\\models\\fbxformat\\ground_riverEnd.fbx");

	if (!modelsList.empty())
	{
		auto it = modelsList.begin();
		auto sizeSqrtDouble = glm::sqrt(modelsList.size());
		size_t sizeSqrt = glm::round(sizeSqrtDouble);
		//sizeSqrt = 6;

		for (size_t x = 0; x < sizeSqrt; x++)
		{
			for (size_t y = 0; y < sizeSqrt; y++)
			{
				auto fileName = (*it++);
				if (it == modelsList.end())
					continue;

				Log::Info(fileName.c_str());

				try
				{
					auto name = CFile(fileName + ".znmdl").Name();
					name = name.substr(0, name.find_first_of('.'));

					std::shared_ptr<ISceneNode3D> sceneNodeParent = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNode3DFactory>(ofkSceneNode3D)->CreateSceneNode3D(this, cSceneNode3D, GetRootNode3D());
					sceneNodeParent->SetName(name);
					sceneNodeParent->SetTranslate(glm::vec3(float(x) * 10.0f, 0.0f, float(y) * 10.0f));

					if (GetBaseManager().GetManager<IFilesManager>()->IsFileExists(fileName + ".znmdl"))
					{
						auto model = GetRenderDevice().GetObjectsFactory().CreateModel();
						if (auto loadable = std::dynamic_pointer_cast<IObjectLoadSave>(model))
						{
							loadable->Load(GetBaseManager().GetManager<IFilesManager>()->Open(fileName + ".znmdl"));
						}

						sceneNodeParent->GetComponent<IModelsComponent3D>()->AddModel(model);
						sceneNodeParent->GetComponent<IColliderComponent3D>()->SetBounds(model->GetBounds());
						continue;
					}
				}
				catch (const CException& e)
				{
					Log::Error(e.MessageCStr());
				}
			}
		}
	}

	sceneNodeParentt = CreateSceneNode<SceneNode3D>(GetRootNode3D());
	sceneNodeParentt->SetTranslate(glm::vec3(0.0f, 0.0f, 0.0f));

	auto model = GetRenderDevice().GetObjectsFactory().CreateModel();
	if (auto loadable = std::dynamic_pointer_cast<IObjectLoadSave>(model))
	{
		loadable->Load(GetBaseManager().GetManager<IFilesManager>()->Open("C:\\_engine\\ZenonEngine_gamedata\\natureKit\\models\\fbxformat\\ground_grass.fbx.znmdl"));
	}

	sceneNodeParentt->GetComponent<IModelsComponent3D>()->AddModel(model);
	sceneNodeParentt->GetComponent<IColliderComponent3D>()->SetBounds(BoundingBox(glm::vec3(-5.0f), glm::vec3(5.0f)));

	glm::vec4 color = glm::vec4(0.0, 0.0f, 0.0f, 1.0f);
	m_Technique3D.AddPass(std::make_shared<ClearRenderTargetPass>(GetRenderDevice(), GetRenderWindow()->GetRenderTarget(), ClearFlags::All, color /*glm::vec4(0.2f, 0.2f, 0.2f, 0.2f)*/, 1.0f, 0));
	m_Technique3D.AddPass(GetBaseManager().GetManager<IRenderPassFactory>()->CreateRenderPass("ModelPassOpaque", GetRenderDevice(), GetRenderWindow()->GetRenderTarget(), &GetRenderWindow()->GetViewport(), shared_from_this()));
	m_Technique3D.AddPass(GetBaseManager().GetManager<IRenderPassFactory>()->CreateRenderPass("ModelPassTransperent", GetRenderDevice(), GetRenderWindow()->GetRenderTarget(), &GetRenderWindow()->GetViewport(), shared_from_this()));
	m_Technique3D.AddPass(std::make_shared<CDrawBoundingBoxPass>(GetRenderDevice(), shared_from_this())->CreatePipeline(GetRenderWindow()->GetRenderTarget(), &GetRenderWindow()->GetViewport()));
}

void CSceneEditor::LoadUI()
{
	m_TechniqueUI.AddPass(std::make_shared<CUIFontPass>(GetRenderDevice(), shared_from_this())->CreatePipeline(GetRenderWindow()->GetRenderTarget(), &GetRenderWindow()->GetViewport()));
}
