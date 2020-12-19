#include "stdafx.h"

// General
#include "Scene_Default.h"

#include "../znPluginFBXModels/FBXInterfaces.h"

#include "UnitsStorage.h"



void CSceneDefault::Load3D()
{
	//--------------------------------------------------------------------------
	// Sponza
	//--------------------------------------------------------------------------
	/*{
		if (false == GetBaseManager().GetManager<IFilesManager>()->IsFileExists("Sponza/Sponza.znmdl"))
		{
			std::shared_ptr<CznFBXLoaderParams> fbxLoaderParams = MakeShared(CznFBXLoaderParams);
			fbxLoaderParams->TexturesPathRoot = "Sponza";
			//fbxLoaderParams->TexturesTypeChange.insert(std::make_pair((uint8)MaterialModel::ETextureType::TextureAmbient, (uint8)MaterialModel::ETextureType::TextureSpecular));
			fbxLoaderParams->TexturesTypeChange.insert(std::make_pair((uint8)MaterialModel::ETextureType::TextureBump, (uint8)MaterialModel::ETextureType::TextureNormalMap));

			auto fbxModel = GetBaseManager().GetManager<IznModelsFactory>()->LoadModel("Sponza/Sponza.fbx", fbxLoaderParams);
			auto znMdlFile = GetBaseManager().GetManager<IznModelsFactory>()->SaveModel(fbxModel, "Sponza/Sponza.znmdl");
			znMdlFile->Save();
		}


		auto znModel = GetBaseManager().GetManager<IznModelsFactory>()->LoadModel("Sponza/Sponza.znmdl");

		auto node = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNodeFactory>()->CreateSceneNode3D(cSceneNode3D, this, newRoot);
		node->SetName("Sponza");
		node->SetTranslate(glm::vec3(0, 0, 0));
		node->SetScale(glm::vec3(1.0f));
		node->GetComponent<IModelComponent>()->SetModel(znModel);
	}*/


	//--------------------------------------------------------------------------
	// Bistro
	//--------------------------------------------------------------------------
	/*{
		auto znModel = GetBaseManager().GetManager<IznModelsFactory>()->LoadModel("Bistro_v4/Bistro_Interior.fbx");

		auto node = CreateSceneNodeT<ISceneNode>();
		node->SetName("Bistro");
		node->SetTranslate(glm::vec3(0, 0, 0));
		node->SetRotation(glm::vec3(-glm::half_pi<float>(), 0.0f, 0.0f));
		node->SetScale(glm::vec3(15.0f));
		node->GetComponentT<IModelComponent>()->SetModel(znModel);
	}*/




	//--------------------------------------------------------------------------
	// Plane
	//--------------------------------------------------------------------------
	{
		const float cPlaneSize = 5240.0f;
		const float cPlaneY = 0.0f;

		std::shared_ptr<MaterialModel> textMaterial = MakeShared(MaterialModel, GetBaseManager());
		textMaterial->SetDiffuseColor(ColorRGB(0.3f, 0.3f, 1.0f));
		textMaterial->SetSpecularColor(ColorRGB(1.0f, 1.0f, 1.0f));

		auto& modelPlane = GetRenderDevice().GetObjectsFactory().CreateModel();
		modelPlane->AddConnection(textMaterial, GetRenderDevice().GetPrimitivesFactory().CreateCube());

		auto node = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNodeFactory>()->CreateSceneNode3D(cSceneNode3D, *this, GetRootSceneNode());
		node->SetName("Ground");
		node->SetPosition(glm::vec3(0, 0, 0));
		node->SetScale(glm::vec3(cPlaneSize, cPlaneSize, cPlaneSize));
		node->GetComponentT<IModelComponent>()->SetModel(modelPlane);
		//node->GetComponent<IModelComponent>()->SetCastShadows(false);
	}



	//--------------------------------------------------------------------------
	// Sphere Metal
	//--------------------------------------------------------------------------
	/*{
		std::shared_ptr<MaterialModel> textMaterial = MakeShared(MaterialModel, GetBaseManager());
		//textMaterial->SetSpecularFactor(8.0f);
		//textMaterial->SetBumpFactor(8.0f);
		textMaterial->SetTexture(MaterialModel::ETextureType::TextureDiffuse, GetBaseManager().GetManager<IznTexturesFactory>()->LoadTexture2D("beaten-up-metal1-unity//beaten-up-metal1-albedo.png"));
		textMaterial->SetTexture(MaterialModel::ETextureType::TextureNormalMap, GetBaseManager().GetManager<IznTexturesFactory>()->LoadTexture2D("beaten-up-metal1-unity//beaten-up-metal1-Normal-ogl.png"));
		textMaterial->SetTexture(MaterialModel::ETextureType::TextureSpecular, GetBaseManager().GetManager<IznTexturesFactory>()->LoadTexture2D("beaten-up-metal1-unity//beaten-up-metal1-ao.png"));

		auto& modelPlane = GetRenderDevice().GetObjectsFactory().CreateModel();
		modelPlane->AddConnection(textMaterial, GetRenderDevice().GetPrimitivesFactory().CreateSphere());

		auto node = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNodeFactory>()->CreateSceneNode3D(cSceneNode3D, this, newRoot);
		node->SetName("Sphere");
		node->SetTranslate(glm::vec3(-10, 15, -10));
		node->SetScale(glm::vec3(15.0f));
		node->GetComponent<IModelComponent>()->SetModel(modelPlane);
	}*/



	//--------------------------------------------------------------------------
	// Cube Gold
	//--------------------------------------------------------------------------
	for (int i = 0; i < 0; i++)
	{
		std::shared_ptr<MaterialModel> textMaterial = MakeShared(MaterialModel, GetBaseManager());
		textMaterial->SetSpecularFactor(32.0f);
		textMaterial->SetBumpFactor(16.0f);

		//textMaterial->SetTexture(MaterialModel::ETextureType::TextureDiffuse, GetBaseManager().GetManager<IznTexturesFactory>()->LoadTexture2D("bricks//bricks2.png"));
		//textMaterial->SetTexture(MaterialModel::ETextureType::TextureNormalMap, GetBaseManager().GetManager<IznTexturesFactory>()->LoadTexture2D("bricks//bricks2_normal.png"));
		//textMaterial->SetTexture(MaterialModel::ETextureType::TextureDisplacement, GetBaseManager().GetManager<IznTexturesFactory>()->LoadTexture2D("bricks//bricks2_disp.png"));

		//textMaterial->SetEnviorementMappingEnable(true);

		textMaterial->SetTexture(MaterialModel::ETextureType::TextureDiffuse, GetBaseManager().GetManager<IznTexturesFactory>()->LoadTexture2D("pirate-gold-unity//pirate-gold_albedo.png"));
		textMaterial->SetTexture(MaterialModel::ETextureType::TextureNormalMap, GetBaseManager().GetManager<IznTexturesFactory>()->LoadTexture2D("pirate-gold-unity//pirate-gold_normal-ogl.png"));
		textMaterial->SetTexture(MaterialModel::ETextureType::TextureSpecular, GetBaseManager().GetManager<IznTexturesFactory>()->LoadTexture2D("pirate-gold-unity//pirate-gold_ao.png"));
		textMaterial->SetTexture(MaterialModel::ETextureType::TextureDisplacement, GetBaseManager().GetManager<IznTexturesFactory>()->LoadTexture2D("pirate-gold-unity//pirate-gold_height.png"));

		auto& modelPlane = GetRenderDevice().GetObjectsFactory().CreateModel();
		modelPlane->AddConnection(textMaterial, GetRenderDevice().GetPrimitivesFactory().CreateCube());

		auto node = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNodeFactory>()->CreateSceneNode3D(cSceneNode3D, *this);
		node->SetName("Sphere2");
		node->SetPosition(glm::vec3(-10, 15, 16 * i));
		node->SetScale(glm::vec3(15.0f));
		node->GetComponentT<IModelComponent>()->SetModel(modelPlane);
		//node->GetComponent<IModelComponent>()->SetCastShadows(false);
	}


	//--------------------------------------------------------------------------
	// Orc with anims
	//--------------------------------------------------------------------------
	/*{
		std::shared_ptr<CznFBXLoaderParams> fbxLoaderParams = MakeShared(CznFBXLoaderParams);
		fbxLoaderParams->TexturesPathRoot = "Toon_RTS/models/textures/";
		fbxLoaderParams->OverrideTexture = "WK_StandardUnits_generic.png";

		auto znModel = GetBaseManager().GetManager<IznModelsFactory>()->LoadModel("Toon_RTS/models/WK_archer.FBX", fbxLoaderParams);

		auto node = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNodeFactory>()->CreateSceneNode3D(cSceneNode3D, this, newRoot);
		node->SetName("Orc");
		node->SetTranslate(glm::vec3(0.0f, 0.0f, 0.0f));
		node->SetRotation(-glm::vec3(glm::half_pi<float>(), 0.0f, 0.0f));
		node->SetScale(glm::vec3(0.5f));
		node->GetComponent<IModelComponent>()->SetModel(znModel);
	}*/

	
	auto unitsStorage = MakeShared(CUnitsStorage, GetBaseManager());

	{
		auto node = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNodeFactory>()->CreateSceneNode3D(cSceneNode3D, *this, GetRootSceneNode());
		node->SetName("model");
		node->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
		//node->SetRotation(-glm::vec3(glm::half_pi<float>(), 0.0f, 0.0f));
		node->SetScale(glm::vec3(0.33f));
		node->GetComponentT<IModelComponent>()->SetModel(unitsStorage->GetModel(0));
		node->GetComponentT<IModelComponent>()->PlayAnimation("idle", true);
	}

	{
		auto node = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNodeFactory>()->CreateSceneNode3D(cSceneNode3D, *this, GetRootSceneNode());
		node->SetName("model");
		node->SetPosition(glm::vec3(25.0f, 0.0f, 0.0f));
		//node->SetRotation(-glm::vec3(glm::half_pi<float>(), 0.0f, 0.0f));
		node->SetScale(glm::vec3(0.33f));
		node->GetComponentT<IModelComponent>()->SetModel(unitsStorage->GetModel(0));
		//node->GetComponentT<IModelComponent>()->PlayAnimation("idle", true);
	}

	

	/*{
		auto node = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNodeFactory>()->CreateSceneNode3D(cSceneNode3D, *this, GetRootSceneNode());
		node->SetName("model");
		node->SetPosition(glm::vec3(-25.0f, 0.0f, 0.0f));
		//node->SetRotation(-glm::vec3(glm::half_pi<float>(), 0.0f, 0.0f));
		node->SetScale(glm::vec3(0.33f));

		auto fbxModelsLoader = GetBaseManager().GetManager<IznModelsFactory>()->GetLoaderForModel("fbx");
		_ASSERT(fbxModelsLoader != nullptr);
		auto fbxSceneLoader = std::dynamic_pointer_cast<IFBXSceneLoader>(fbxModelsLoader);
		_ASSERT(fbxSceneLoader != nullptr);
		auto animatedSkeletonModel = fbxSceneLoader->LoadScene("Toon_RTS\\WesternKingdoms\\animation\\Archer\\WK_archer_01_idle.FBX", nullptr)->MergeModels();
		
		
		node->GetComponentT<IModelComponent>()->SetModel(animatedSkeletonModel);
		//node->GetComponentT<IModelComponent>()->PlayAnimation("idle", true);
	}*/

	//--------------------------------------------------------------------------
	// Orc with anims
	//--------------------------------------------------------------------------
	/*{
		auto node = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNodeFactory>()->CreateSceneNode3D(cSceneNode3D, *this, GetRootSceneNode());
		node->SetName("OrcAnimation");
		node->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
		//node->SetRotation(-glm::vec3(glm::half_pi<float>(), 0.0f, 0.0f));
		node->SetScale(glm::vec3(0.33f));

		CznFBXLoaderParams fbxLoaderParams;
		fbxLoaderParams.TexturesPathRoot = "Toon_RTS/models/textures/";
		fbxLoaderParams.OverrideTextureByMaterial["WK_StandardUnits_Blue"] = "WK_StandardUnits_Blue.png";

		auto fbxModelsLoader = GetBaseManager().GetManager<IznModelsFactory>()->GetLoaderForModel("fbx");
		_ASSERT(fbxModelsLoader != nullptr);
		auto fbxSceneLoader = std::dynamic_pointer_cast<IFBXSceneLoader>(fbxModelsLoader);
		_ASSERT(fbxSceneLoader != nullptr);
		
		// Original skeleton
		auto originalSkeletonModel = fbxSceneLoader->LoadScene("Toon_RTS/models/WK_archer.FBX", &fbxLoaderParams)->MergeModels();
		
		// Fix materials
		//for (const auto& connection : originalSkeletonModel->GetConnections())
		//	std::dynamic_pointer_cast<MaterialModel>(connection.Material)->SetDiffuseFactor(2.0f);

		// Animated skeleton
		auto animatedSkeletonModel = fbxSceneLoader->LoadScene("Toon_RTS/animation/archer/WK_archer_03_run.FBX", &fbxLoaderParams)->MergeModels();
		for (const auto& anim : animatedSkeletonModel->GetAnimations())
			originalSkeletonModel->AddAnimation("run", anim.second);

		auto animatedSkeletonModel2 = fbxSceneLoader->LoadScene("Toon_RTS/animation/archer/WK_archer_10_death_A.FBX", &fbxLoaderParams)->MergeModels();
		for (const auto& anim : animatedSkeletonModel2->GetAnimations())
			originalSkeletonModel->AddAnimation("death", anim.second);

		std::string znModelFilename = "OrcWithAnims.znmdl";
		if (GetBaseManager().GetManager<IFilesManager>()->IsFileExists(znModelFilename))
			GetBaseManager().GetManager<IFilesManager>()->Delete(znModelFilename);
	
		auto znMdlFile = GetBaseManager().GetManager<IznModelsFactory>()->SaveModel(originalSkeletonModel, znModelFilename);
		znMdlFile->Save();

		node->GetComponentT<IModelComponent>()->SetModel(originalSkeletonModel);
		node->GetComponentT<IModelComponent>()->PlayAnimation("run", true);
	}*/

	//--------------------------------------------------------------------------
	// Orc with anims ZNMDL
	//--------------------------------------------------------------------------
	/*{
		auto node = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNodeFactory>()->CreateSceneNode3D(cSceneNode3D, *this, GetRootSceneNode());
		node->SetName("OrcAnimationZNMDL");
		node->SetTranslate(glm::vec3(0.0f, 0.0f, 0.0f));
		//node->SetRotation(-glm::vec3(glm::half_pi<float>(), 0.0f, 0.0f));
		node->SetScale(glm::vec3(0.33f));

		auto znMdlFile = GetBaseManager().GetManager<IznModelsFactory>()->LoadModel("OrcWithAnims.znmdl");

		node->GetComponentT<IModelComponent>()->SetModel(znMdlFile);
		//node->GetComponentT<IModelComponent>()->PlayAnimation("run", true);
	}*/
}
