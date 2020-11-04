#include "stdafx.h"

// General
#include "Scene_Default.h"



void CSceneDefault::Load3D()
{
	auto newRoot = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNodeFactory>()->CreateSceneNode3D(cSceneNode3D, *this, GetRootSceneNode());
	newRoot->SetName("NewRoot3D");



	//--------------------------------------------------------------------------
	// XML
	//--------------------------------------------------------------------------
	if (auto file = GetBaseManager().GetManager<IFilesManager>()->Open("RTS2.xml"))
	{
		CXMLManager xml(GetBaseManager());
		auto reader = xml.CreateReader(file);

		auto currentRoot = newRoot;
		while (false == currentRoot->GetChilds().empty())
			currentRoot->RemoveChild(currentRoot->GetChilds()[0]);

		auto rootNodeXML = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNodeFactory>()->LoadSceneNode3DXML(reader->GetChilds()[0], currentRoot->GetScene(), currentRoot->GetParent());
		while (false == rootNodeXML->GetChilds().empty())
			currentRoot->AddChild(rootNodeXML->GetChilds()[0]);
	}


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
		node->GetComponent<IModelsComponent3D>()->SetModel(znModel);
	}*/


	//--------------------------------------------------------------------------
	// Bistro
	//--------------------------------------------------------------------------
	/*{
		if (false == GetBaseManager().GetManager<IFilesManager>()->IsFileExists("Bistro_v4/Bistro v4 Update/Bistro_v4/Bistro_Exterior.znmdl"))
		{
			std::shared_ptr<CznFBXLoaderParams> fbxLoaderParams = MakeShared(CznFBXLoaderParams);
			fbxLoaderParams->TexturesPathRoot = "Bistro_v4/Bistro v4 Update/Bistro_v4/";
			//fbxLoaderParams->TexturesTypeChange.insert(std::make_pair((uint8)MaterialModel::ETextureType::TextureAmbient, (uint8)MaterialModel::ETextureType::TextureSpecular));
			//fbxLoaderParams->TexturesTypeChange.insert(std::make_pair((uint8)MaterialModel::ETextureType::TextureBump, (uint8)MaterialModel::ETextureType::TextureNormalMap));

			auto fbxModel = GetBaseManager().GetManager<IznModelsFactory>()->LoadModel("Bistro_v4/Bistro v4 Update/Bistro_v4/Bistro_Exterior.fbx", fbxLoaderParams);
			auto znMdlFile = GetBaseManager().GetManager<IznModelsFactory>()->SaveModel(fbxModel, "Bistro_v4/Bistro v4 Update/Bistro_v4/Bistro_Exterior.znmdl");
			GetBaseManager().GetManager<IFilesManager>()->GetFilesStorage("ZenonGamedata2")->SaveFile(znMdlFile);
		}


		auto znModel = GetBaseManager().GetManager<IznModelsFactory>()->LoadModel("Bistro_v4/Bistro v4 Update/Bistro_v4/Bistro_Exterior.znmdl");

		auto node = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNodeFactory>()->CreateSceneNode3D(cSceneNode3D, this, newRoot);
		node->SetName("Bistro");
		node->SetTranslate(glm::vec3(0, 0, 0));
		node->SetScale(glm::vec3(1.0f));
		node->GetComponent<IModelsComponent3D>()->SetModel(znModel);
	}*/




	//--------------------------------------------------------------------------
	// Plane
	//--------------------------------------------------------------------------
	{
		const float cPlaneSize = 240.0f;
		const float cPlaneY = 0.0f;

		std::shared_ptr<MaterialModel> textMaterial = MakeShared(MaterialModel, GetBaseManager());
		textMaterial->SetDiffuseColor(glm::vec3(0.3f, 0.3f, 1.0f));
		textMaterial->SetSpecularColor(glm::vec3(1.0f, 1.0f, 1.0f));

		auto& modelPlane = GetRenderDevice().GetObjectsFactory().CreateModel();
		modelPlane->AddConnection(textMaterial, GetRenderDevice().GetPrimitivesFactory().CreateCube());

		auto node = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNodeFactory>()->CreateSceneNode3D(cSceneNode3D, *this, newRoot);
		node->SetName("Ground");
		node->SetTranslate(glm::vec3(0, -25, 0));
		node->SetScale(glm::vec3(cPlaneSize, 5.0f, cPlaneSize));
		node->GetComponent<IModelsComponent3D>()->SetModel(modelPlane);
		//node->GetComponent<IModelsComponent3D>()->SetCastShadows(false);
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
		node->GetComponent<IModelsComponent3D>()->SetModel(modelPlane);
	}*/



	//--------------------------------------------------------------------------
	// Cube Gold
	//--------------------------------------------------------------------------
	for (int i = 0; i < 5; i++)
	{
		std::shared_ptr<MaterialModel> textMaterial = MakeShared(MaterialModel, GetBaseManager());
		textMaterial->SetSpecularFactor(32.0f);
		//textMaterial->SetBumpFactor(16.0f);

		textMaterial->SetTexture(MaterialModel::ETextureType::TextureDiffuse, GetBaseManager().GetManager<IznTexturesFactory>()->LoadTexture2D("pirate-gold-unity//pirate-gold_albedo.png"));
		textMaterial->SetTexture(MaterialModel::ETextureType::TextureNormalMap, GetBaseManager().GetManager<IznTexturesFactory>()->LoadTexture2D("pirate-gold-unity//pirate-gold_normal-ogl.png"));
		textMaterial->SetTexture(MaterialModel::ETextureType::TextureSpecular, GetBaseManager().GetManager<IznTexturesFactory>()->LoadTexture2D("pirate-gold-unity//pirate-gold_ao.png"));
		textMaterial->SetTexture(MaterialModel::ETextureType::TextureDisplacement, GetBaseManager().GetManager<IznTexturesFactory>()->LoadTexture2D("pirate-gold-unity//pirate-gold_height.png"));

		auto& modelPlane = GetRenderDevice().GetObjectsFactory().CreateModel();
		modelPlane->AddConnection(textMaterial, GetRenderDevice().GetPrimitivesFactory().CreateCube());

		auto node = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNodeFactory>()->CreateSceneNode3D(cSceneNode3D, *this, newRoot);
		node->SetName("Sphere2");
		node->SetTranslate(glm::vec3(-10, 15, 16 * i));
		node->SetScale(glm::vec3(15.0f));
		node->GetComponent<IModelsComponent3D>()->SetModel(modelPlane);
		//node->GetComponent<IModelsComponent3D>()->SetCastShadows(false);
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
		node->GetComponent<IModelsComponent3D>()->SetModel(znModel);
	}*/

	//--------------------------------------------------------------------------
	// Orc with anims
	//--------------------------------------------------------------------------
	/*{
		auto node = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNodeFactory>()->CreateSceneNode3D(cSceneNode3D, this, newRoot);
		node->SetName("OrcAnimation");
		node->SetTranslate(glm::vec3(0.0f, 0.0f, 0.0f));
		//node->SetRotation(-glm::vec3(glm::half_pi<float>(), 0.0f, 0.0f));
		//node->SetScale(glm::vec3(0.5f));

		std::shared_ptr<CznFBXLoaderParams> fbxLoaderParams = MakeShared(CznFBXLoaderParams);
		fbxLoaderParams->TexturesPathRoot = "Toon_RTS/models/textures/";
		fbxLoaderParams->OverrideTexture = "WK_StandardUnits_generic.png";

		auto fbxModelsLoader = GetBaseManager().GetManager<IznModelsFactory>()->GetLoaderForModel("fbx");
		_ASSERT(fbxModelsLoader != nullptr);
		auto fbxSceneLoader = std::dynamic_pointer_cast<IFBXSceneLoader>(fbxModelsLoader);
		_ASSERT(fbxSceneLoader != nullptr);
		
		// Original skeleton
		auto originalSkeletonScene = fbxSceneLoader->LoadScene("Toon_RTS/models/WK_archer.FBX", fbxLoaderParams);
		//node->GetComponent<IModelsComponent3D>()->SetModel(originalSkeletonScene->MergeModels());

		// Animated skeleton
		auto animatedSkeletonScene = fbxSceneLoader->LoadScene("Toon_RTS/animation/archer/WK_archer_03_run2.FBX", fbxLoaderParams);
		auto animatedSkeleton = animatedSkeletonScene->GetFBXSkeleton()->GetSkeleton();

		auto originalSkeleton = originalSkeletonScene->GetFBXSkeleton()->GetSkeleton();
		originalSkeleton.MergeWithOther(animatedSkeleton);
		node->AddComponent<ISkeletonComponent3D>(MakeShared(CSkeletonComponent3D, *node, originalSkeleton));
		

		// Animations
		uint16 cntr = 0;
		auto animationComponent = node->AddComponent<ISkeletonAnimationComponent>(MakeShared(CAnimatorComponent3D, *node));
		for (const auto& anim : animatedSkeletonScene->GetFBXAnimation()->GetAnimations())
			animationComponent->AddAnimation(cntr++, anim);
	}*/

	/*{
		auto node = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNodeFactory>()->CreateSceneNode3D(cSceneNode3D, this, newRoot);
		node->SetName("OrcAnimation");
		node->SetTranslate(glm::vec3(0.0f, 0.0f, 0.0f));
		node->SetRotation(-glm::vec3(glm::half_pi<float>(), 0.0f, 0.0f));
		node->SetScale(glm::vec3(15.5f));

		std::shared_ptr<CznFBXLoaderParams> fbxLoaderParams = MakeShared(CznFBXLoaderParams);
		fbxLoaderParams->TexturesPathRoot = "Toon_RTS/models/textures/";
		fbxLoaderParams->OverrideTexture = "WK_StandardUnits_generic.png";

		auto fbxModelsLoader = GetBaseManager().GetManager<IznModelsFactory>()->GetLoaderForModel("fbx");
		_ASSERT(fbxModelsLoader != nullptr);
		auto fbxSceneLoader = std::dynamic_pointer_cast<IFBXSceneLoader>(fbxModelsLoader);
		_ASSERT(fbxSceneLoader != nullptr);

		// Original skeleton
		auto originalSkeletonScene = fbxSceneLoader->LoadScene("Kenney Game Assets 3 version 23/3D assets/Animated Characters 2/Model/characterMedium.fbx", fbxLoaderParams);
		node->GetComponent<IModelsComponent3D>()->SetModel(originalSkeletonScene->MergeModels());

		// Animated skeleton
		auto animatedSkeletonScene = fbxSceneLoader->LoadScene("Kenney Game Assets 3 version 23/3D assets/Animated Characters 2/Animations/run.fbx", fbxLoaderParams);
		auto animatedSkeleton = animatedSkeletonScene->GetFBXSkeleton()->GetSkeleton();

		auto originalSkeleton = originalSkeletonScene->GetFBXSkeleton()->GetSkeleton();
		originalSkeleton.MergeWithOther(animatedSkeleton);
		node->AddComponent<ISkeletonComponent3D>(MakeShared(CSkeletonComponent3D, *node, originalSkeleton));


		// Animations
		uint16 cntr = 0;
		auto animationComponent = node->AddComponent<ISkeletonAnimationComponent>(MakeShared(CAnimatorComponent3D, *node));
		for (const auto& anim : animatedSkeletonScene->GetFBXAnimation()->GetAnimations())
			animationComponent->AddAnimation(cntr++, anim);
	}*/
}






//
//
//
void CSceneDefault::Load3DOld()
{
	//--------------------------------------------------------------------------
	// Cubes & Spheres
	//--------------------------------------------------------------------------

	Random r(0);


	{
		const int iterCnt = 0;
		const float offset = 13.0f;
		const float scale = 5.0f;

		std::shared_ptr<MaterialModel> textMaterial = MakeShared(MaterialModel, GetBaseManager());
		textMaterial->SetDiffuseColor(glm::vec3(1.0f, 1.0f, 1.0f));
		textMaterial->SetSpecularColor(glm::vec3(1.0f, 1.0f, 1.0f));
		textMaterial->SetSpecularFactor(4.0f);
		textMaterial->SetBumpFactor(8.0f);
		textMaterial->SetTexture(MaterialModel::ETextureType::TextureDiffuse, GetBaseManager().GetManager<IznTexturesFactory>()->LoadTexture2D("AmazonScene//BuildingTextures//concrete_smooth_03_diff.dds"));
		//textMaterial->SetTexture(MaterialModel::ETextureType::TextureNormalMap, GetBaseManager().GetManager<IznTexturesFactory>()->LoadTexture2D("Sponza_Ceiling_normal.png"));
		textMaterial->SetTexture(MaterialModel::ETextureType::TextureSpecular, GetBaseManager().GetManager<IznTexturesFactory>()->LoadTexture2D("AmazonScene//BuildingTextures//concrete_smooth_03_spec.dds"));
		textMaterial->SetTexture(MaterialModel::ETextureType::TextureBump, GetBaseManager().GetManager<IznTexturesFactory>()->LoadTexture2D("AmazonScene//BuildingTextures//concrete_smooth_03_ddna.dds"));

		//std::shared_ptr<MaterialTextured> textMaterial = MakeShared(MaterialTextured, GetRenderDevice());
		//textMaterial->SetTexture(0, GetBaseManager().GetManager<IznTexturesFactory>()->LoadTexture2D("Sponza_Floor_diffuse.png"));

		auto cubeModel = GetRenderDevice().GetObjectsFactory().CreateModel();
		cubeModel->AddConnection(textMaterial, GetRenderDevice().GetPrimitivesFactory().CreateCube());

		auto sphereModel = GetRenderDevice().GetObjectsFactory().CreateModel();
		sphereModel->AddConnection(textMaterial, GetRenderDevice().GetPrimitivesFactory().CreateSphere());


		m_RootForBoxes = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNodeFactory>()->CreateSceneNode3D(cSceneNode3D, *this, GetRootSceneNode());
		//m_RootForBoxes->SetTranslate(glm::vec3(150, 0, 150));


		for (int i = -(iterCnt / 2); i < iterCnt / 2; i++)
		{
			for (int j = -(iterCnt / 2); j < iterCnt / 2; j++)
			{
				for (int k = 0; k < iterCnt; k++)
				{
					auto sceneNode = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNodeFactory>()->CreateSceneNode3D(cSceneNode3D, *this, m_RootForBoxes);
					sceneNode->SetName("Ball [" + std::to_string(i) + ", " + std::to_string(j) + ", " + std::to_string(k) + "]");
					sceneNode->SetTranslate(glm::vec3(offset * i, offset * k, offset * j));
					sceneNode->SetScale(glm::vec3(scale));
					//sceneNode->GetComponent<IModelsComponent3D>()->AddModel(((i % 2 == 0) || (j % 2 == 0) && (k % 2 == 0)) ? cubeModel : sphereModel);
					sceneNode->GetComponent<IModelsComponent3D>()->SetModel(sphereModel);

					BoundingBox bbox = BoundingBox(glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
					bbox.transform(sceneNode->GetWorldTransfom());
					sceneNode->GetComponent<IColliderComponent3D>()->SetBounds(bbox);

#if 0
					rp3d::Vector3 initPosition(offset * i + r.NextFloat() / 5.0f, offset * k, offset * j + r.NextFloat() / 5.0f);
					rp3d::Quaternion initOrientation = rp3d::Quaternion::identity();
					rp3d::Transform transform(initPosition, initOrientation);

					// Create a rigid body in the world
					rp3d::RigidBody * body;
					body = m_World.createRigidBody(transform);
					body->setType(rp3d::BodyType::DYNAMIC);
					body->getMaterial().setFrictionCoefficient(0.8);

					// Create the sphere collision shape
					float radius = rp3d::decimal(scale);
					rp3d::SphereShape * shape = new rp3d::SphereShape(radius / 2.0f);

					rp3d::ProxyShape * proxyShape;
					proxyShape = body->addCollisionShape(shape, rp3d::Transform::identity(), rp3d::decimal(5.0));


					std::shared_ptr<CReactPhysicsComponent> component = MakeShared(CReactPhysicsComponent, *sceneNode, body);
					sceneNode->AddComponent(component);
#endif
				}
			}
		}
	}

	//--------------------------------------------------------------------------
	// Plane
	//--------------------------------------------------------------------------

#if 0
	{
		const float cPlaneSize = 120.0f;
		const float cPlaneY = -50.0f;

		std::shared_ptr<MaterialModel> textMaterial = MakeShared(MaterialModel, GetBaseManager());
		textMaterial->SetDiffuseColor(glm::vec3(1.0f, 1.0f, 1.0f));
		textMaterial->SetSpecularColor(glm::vec3(1.0f, 1.0f, 1.0f));
		textMaterial->SetSpecularFactor(1.0f);
		textMaterial->SetBumpFactor(8.0f);
		textMaterial->SetTexture(MaterialModel::ETextureType::TextureDiffuse, GetBaseManager().GetManager<IznTexturesFactory>()->LoadTexture2D("AmazonScene//OtherTextures//Colors//Orange.dds"));
		//textMaterial->SetTexture(MaterialModel::ETextureType::TextureNormalMap, GetBaseManager().GetManager<IznTexturesFactory>()->LoadTexture2D("Sponza_Ceiling_normal.png"));
		//textMaterial->SetTexture(MaterialModel::ETextureType::TextureSpecular, GetBaseManager().GetManager<IznTexturesFactory>()->LoadTexture2D("AmazonScene//BuildingTextures//concrete_smooth_03_spec.dds"));
		//textMaterial->SetTexture(MaterialModel::ETextureType::TextureBump, GetBaseManager().GetManager<IznTexturesFactory>()->LoadTexture2D("AmazonScene//BuildingTextures//concrete_smooth_03_ddna.dds"));

		//std::shared_ptr<MaterialTextured> textMaterial = MakeShared(MaterialTextured, GetRenderDevice());
		//textMaterial->SetTexture(0, GetBaseManager().GetManager<IznTexturesFactory>()->LoadTexture2D("idi na huy.png"));

		auto& modelPlane = GetRenderDevice().GetObjectsFactory().CreateModel();
		modelPlane->AddConnection(textMaterial, GetRenderDevice().GetPrimitivesFactory().CreatePlane());

		auto node = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNodeFactory>()->CreateSceneNode3D(cSceneNode3D, this);
		node->SetName("Ground");
		node->SetTranslate(glm::vec3(0, cPlaneY, 0));
		node->SetScale(glm::vec3(cPlaneSize));
		node->GetComponent<IModelsComponent3D>()->SetModel(modelPlane);

		rp3d::Vector3 initPosition(0, cPlaneY, 0);
		rp3d::Quaternion initOrientation = rp3d::Quaternion::identity();
		rp3d::Transform transform(initPosition, initOrientation);

		// Create a rigid body in the world

#if 0
		rp3d::RigidBody * body;
		body = m_World.createRigidBody(transform);
		body->setType(rp3d::BodyType::STATIC);
		body->getMaterial().setFrictionCoefficient(0.8);
		body->getMaterial().setRollingResistance(0.2);

		rp3d::BoxShape * shape = new rp3d::BoxShape(rp3d::Vector3(cPlaneSize, 0.001f, cPlaneSize));

		rp3d::ProxyShape * proxyShape;
		proxyShape = body->addCollisionShape(shape, rp3d::Transform::identity(), rp3d::decimal(5.0));

		std::shared_ptr<CReactPhysicsComponent> component = MakeShared(CReactPhysicsComponent, *node, body);
		node->AddComponent(component);
#endif
	}
#endif



	//
	// PARTICLES
	{

		/*auto buffer = GetRenderDevice().GetObjectsFactory().CreateVertexBuffer(particles);

		auto geom = GetRenderDevice().GetObjectsFactory().CreateGeometry();
		geom->SetPrimitiveTopology(PrimitiveTopology::TriangleStrip);
		geom->AddVertexBuffer(BufferBinding("POSITION", 0), buffer);

		auto model = GetRenderDevice().GetObjectsFactory().CreateModel();
		model->AddConnection(material, geom);*/



		/*

		auto material = MakeShared(MaterialParticle, GetRenderDevice());
		material->SetTexture(0, GetBaseManager().GetManager<IznTexturesFactory>()->LoadTexture2D("particle.png"));

		auto particlesNode = m_RootForBoxes->CreateSceneNode<CSceneNode>();
		particlesNode->SetName("Particles");
		//particlesNode->GetComponent<IModelsComponent3D>()->AddModel(model);
		auto particlesComponent = MakeShared(CParticlesComponent3D, *particlesNode);
		particlesComponent->SetMaterial(material);
		particlesNode->AddComponent(particlesComponent);



		float areaSize = 2000.0f;
		Random r(time(0));
		for (size_t i = 0; i < 1000000; i++)
		{
			SParticle particle;
			particle.Position = glm::vec3(r.NextFloat(), r.NextFloat(), r.NextFloat()) * areaSize - glm::vec3(areaSize / 2.0f, 0.0f, areaSize / 2.0f);
			float size = r.NextFloat() * 3.0f + 1.0f;
			particle.Size = glm::vec2(size, size);
			particle.Color = glm::vec4(r.NextFloat(), r.NextFloat(), r.NextFloat(), 1.0f);
			particlesComponent->AddParticle(particle);
		}
		*/
	}


	//m_World.raycast(reactphysics3d::Ray(), reactphysics3d::RaycastCallback);

#if 0

	std::shared_ptr<ISceneNode> sceneNode = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNodeFactory>()->CreateSceneNode3D(cSceneNode3D, this);

	// Models
	auto fbxSceneModel = GetBaseManager().GetManager<IFBXManager>()->LoadFBX("C:/Users/Alexander/Downloads/Assets/Toon_RTS/Orcs/models/Single_Mesh/Orc_SM_shaman.FBX");
	DoAddModels(sceneNode, fbxSceneModel->GetFBXRootNode());

	// Animator
	sceneNode->AddComponent<ISkeletonAnimationComponent>(MakeShared(CAnimatorComponent3D, *sceneNode));
	uint16 cntr = 0;
	auto fbxSceneAnim = GetBaseManager().GetManager<IFBXManager>()->LoadFBX("C:/Users/Alexander/Downloads/Assets/Toon_RTS/Orcs/animation/shaman/Orc_shaman_01_idle_A.FBX");
	for (const auto& anim : fbxSceneAnim->GetFBXAnimation()->GetAnimations())
		if (sceneNode->GetComponent<ISkeletonAnimationComponent>())
			sceneNode->GetComponent<ISkeletonAnimationComponent>()->AddAnimation(cntr++, anim);

	// Skeleton
	//auto skeletonFromModel = fbxSceneModel->GetSkeleton()->GetSkeleton();
	auto skeletonFromAnim = fbxSceneAnim->GetFBXSkeleton()->GetSkeleton();
	//skeletonFromModel.MergeWithOther(skeletonFromAnim);

	// Skeleton component
	sceneNode->AddComponent<ISkeletonComponent3D>(MakeShared(CSkeletonComponent3D, *sceneNode, skeletonFromAnim));
#endif

	//std::shared_ptr<IFBXNode> fbxSceneNode = std::dynamic_pointer_cast<IFBXNode>(sceneNode);
	//fbxSceneNode->InitializeFromFile("C:/Users/Alexander/Downloads/Assets/Toon_RTS/Orcs/animation/shaman/Orc_shaman_02_walk.FBX");
	//fbxSceneNode->InitializeFromFile("C:/Users/Alexander/Downloads/Assets/Toon_RTS/Orcs/models/Single_Mesh/Orc_SM_shaman.FBX");

	//m_Technique3D.AddPass(GetBaseManager().GetManager<IRenderPassFactory>()->CreateRenderPass("TexturedMaterialPass", GetRenderDevice(), GetRenderWindow()->GetRenderTarget(), &GetRenderWindow()->GetViewport(), shared_from_this()));
	//m_Technique3D.AddPass(MakeShared(CMaterialParticlePass, GetRenderDevice(), shared_from_this())->ConfigurePipeline(GetRenderWindow()->GetRenderTarget(), &GetRenderWindow()->GetViewport()));
}
