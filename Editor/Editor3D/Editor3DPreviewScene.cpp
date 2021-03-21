#include "stdafx.h"

// Gerenal
#include "Editor3DPreviewScene.h"


namespace
{
	void ExtendsBoundsRecursive(BoundingBox& Bounds, std::shared_ptr<ISceneNode> SceneNode)
	{
		if (auto collider = SceneNode->GetComponentT<IColliderComponent>())
		{
			const auto& colliderBounds = collider->GetBounds();
			if (false == colliderBounds.IsInfinite())
			{
				Bounds.makeUnion(colliderBounds);
			}
		}

		for (const auto& ch : SceneNode->GetChilds())
			ExtendsBoundsRecursive(Bounds, ch);
	}
}


CEditor3DPreviewScene::CEditor3DPreviewScene(IEditor& Editor, IRenderWindow& RenderWindow)
	: SceneBase(Editor.GetBaseManager(), RenderWindow)
{
	dynamic_cast<IEditorPrivate&>(Editor).Set3DPreviewFrame(this);
}

CEditor3DPreviewScene::~CEditor3DPreviewScene()
{
}



//
// IEditor3DPreviewFrame
//
void CEditor3DPreviewScene::SetSceneNode(std::shared_ptr<ISceneNode> SceneNode)
{
	Clean();

	auto copy = SceneNode->CopyCast<ISceneNode>();
	m_SceneNode->AddChild(copy);

	BoundingBox bbox;
	ExtendsBoundsRecursive(bbox, copy);
	if (bbox.IsInfinite())
	{
		bbox.setMin(glm::vec3(-0.5f));
		bbox.setMax(glm::vec3(0.5f));
	}

	float radius = bbox.getRadius();

	m_SceneNode->SetPosition(-bbox.getCenter());
	GetCameraController()->GetCamera()->SetPosition(glm::vec3(radius * 1.5f));
}

void CEditor3DPreviewScene::SetModel(std::shared_ptr<IModel> Model)
{
	Clean();

	auto modelComponent = m_SceneNodeForModelPreview->GetComponentT<IModelComponent>();
	modelComponent->SetModel(Model);

	// Play first animation
	if (false == Model->GetAnimations().empty())
		modelComponent->PlayAnimation(Model->GetAnimations().begin()->second->GetName(), true);

	auto modelBBox = Model->GetBounds();
	if (modelBBox.IsInfinite())
		modelBBox = BoundingBox(glm::vec3(-25.0f), glm::vec3(25.0f));

	m_SceneNodeForModelPreview->SetPosition(- modelBBox.getCenter());
	GetCameraController()->GetCamera()->SetPosition(glm::vec3(modelBBox.getRadius() * 1.5f));
}

namespace
{
	std::pair<glm::vec2, glm::vec2> CalculateTextureSize(const glm::uvec2& TextureSize, const glm::uvec2& ViewportSize)
	{
		glm::vec2 newSize(32.0f);
		glm::vec2 newPosition(0.0f);

		glm::vec2 viewport(ViewportSize);
		if (ViewportSize.x > ViewportSize.y)
			viewport.x = ViewportSize.y;
		else if (ViewportSize.x < ViewportSize.y)
			viewport.y = ViewportSize.x;

		if (TextureSize.x > TextureSize.y)
		{
			float koeff = static_cast<float>(viewport.x) / static_cast<float>(TextureSize.x);
			newSize.x = static_cast<float>(TextureSize.x) * koeff;
			newSize.y = static_cast<float>(TextureSize.y) * koeff;
			newPosition.x = 0.0f;
			newPosition.y = (ViewportSize.y - newSize.y) / 2.0f;
		}
		else if (TextureSize.x < TextureSize.y)
		{
			float koeff = static_cast<float>(viewport.y) / static_cast<float>(TextureSize.y);
			newSize.x = static_cast<float>(TextureSize.x) * koeff;
			newSize.y = static_cast<float>(TextureSize.y) * koeff;
			newPosition.x = (ViewportSize.x - newSize.x) / 2.0f;
			newPosition.y = 0.0f;
		}
		else
		{
			float koeffX = static_cast<float>(viewport.x) / static_cast<float>(TextureSize.x);
			float koeffY = static_cast<float>(viewport.y) / static_cast<float>(TextureSize.y);
			newSize.x = static_cast<float>(TextureSize.x) * koeffX;
			newSize.y = static_cast<float>(TextureSize.y) * koeffY;
			newPosition.x = (ViewportSize.x - newSize.x) / 2.0f;
			newPosition.y = (ViewportSize.y - newSize.y) / 2.0f;
		}

		return std::make_pair(newSize, newPosition);
	}
}

void CEditor3DPreviewScene::SetTexture(std::shared_ptr<ITexture> Texture)
{
	_ASSERT(Texture != nullptr);

	Clean();

	std::shared_ptr<CUICommonMaterial> contentMaterial = MakeShared(CUICommonMaterial, GetBaseManager().GetApplication().GetRenderDevice());
	contentMaterial->SetTexture(Texture);

	auto textureBounds = CalculateTextureSize(Texture->GetSize(), GetRenderWindow().GetViewport().GetSize());

	/*IUIControlCommon::SSubgeometry subGeom;
	subGeom.Translate = textureBounds.second;
	subGeom.Size = textureBounds.first;
	subGeom.Material = contentMaterial;
	subGeom.Geom = GetBaseManager().GetApplication().GetRenderDevice().GetPrimitivesFactory().CreateUIQuad(glm::vec2(1.0f));*/

	//m_TextureNode->AddSubgeometry(subGeom);
}

void CEditor3DPreviewScene::SetParticleSystem(std::shared_ptr<IParticleSystem> ParticleSystem)
{
	Clean();

	auto particlesComponent = m_SceneNodeForParticlePreview->GetComponentT<IParticleComponent3D>();
	particlesComponent->DeleteAllParticleSystem();

	ParticleSystem->SetNode(m_SceneNodeForParticlePreview.get());
	particlesComponent->AddParticleSystem(ParticleSystem);

	auto bbox = BoundingBox(glm::vec3(-5.0f), glm::vec3(5.0f));

	m_SceneNodeForParticlePreview->SetPosition(-bbox.getCenter());
	GetCameraController()->GetCamera()->SetPosition(glm::vec3(bbox.getRadius() * 1.5f));
}

void CEditor3DPreviewScene::SetMaterial(std::shared_ptr<IMaterial> Material)
{
	Clean();

	auto sphereGeometry = GetRenderDevice().GetPrimitivesFactory().CreateSphere(15.0f);

	auto model = GetRenderDevice().GetObjectsFactory().CreateModel();
	model->AddConnection(Material, sphereGeometry);

	auto modelComponent = m_SceneNodeForModelPreview->GetComponentT<IModelComponent>();
	modelComponent->SetModel(model);

	// Play first animation
	if (false == model->GetAnimations().empty())
		modelComponent->PlayAnimation(model->GetAnimations().begin()->second->GetName(), true);

	auto modelBBox = model->GetBounds();
	if (modelBBox.IsInfinite())
		modelBBox = BoundingBox(glm::vec3(-10.0f), glm::vec3(10.0f));

	m_SceneNodeForModelPreview->SetPosition(-modelBBox.getCenter());
	GetCameraController()->GetCamera()->SetPosition(glm::vec3(modelBBox.getRadius() * 1.0f));
}



//
// IGameState
//
void CEditor3DPreviewScene::Initialize()
{
	SceneBase::Initialize();

	ShowStatistics(false);

	// Light
	{
		auto lightNode = CreateSceneNodeT<ISceneNode>();
		lightNode->SetName("Light2");
		lightNode->SetLocalPosition(glm::vec3(150.0f, 150.0f, 150.0f));
		lightNode->SetLocalRotationEuler(glm::vec3(-45.0f, -45.0f, 25.0f));

		auto lightComponent = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<IComponentFactory>()->CreateComponentT<CLightComponent>(cSceneNodeLightComponent, *lightNode.get());
		lightComponent->SetLight(MakeShared(CLight, GetBaseManager()));
		lightComponent->GetLight()->SetCastShadows(true);
		lightComponent->GetLight()->SetType(ELightType::Directional);
		lightComponent->GetLight()->SetAmbientColor(ColorRGB(0.25f));
		lightComponent->GetLight()->SetColor(ColorRGB(1.0f, 1.0f, 1.0f));
		lightComponent->GetLight()->SetRange(1000.0f);
		lightComponent->GetLight()->SetIntensity(1.0077f);
		lightComponent->GetLight()->SetSpotlightAngle(30.0f);

		lightNode->AddComponent(cSceneNodeLightComponent, lightComponent);
	}


	{
		auto cameraNode = CreateSceneNodeT<ISceneNode>();
		cameraNode->SetName("Camera");
		cameraNode->AddComponentT(GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<IComponentFactory>()->CreateComponentT<ICameraComponent3D>(cSceneNodeCameraComponent, *cameraNode));

		SetCameraController(MakeShared(CFreeCameraController));
		GetCameraController()->SetCamera(cameraNode->GetComponentT<ICameraComponent3D>());
		GetCameraController()->GetCamera()->SetPerspectiveProjection(75.0f, static_cast<float>(GetRenderWindow().GetWindowWidth()) / static_cast<float>(GetRenderWindow().GetWindowHeight()), 1.0f, 5000.0f);
		GetCameraController()->GetCamera()->SetPosition(glm::vec3(15.0f * 2.0f));
		GetCameraController()->GetCamera()->SetYaw(180 + 45);
		GetCameraController()->GetCamera()->SetPitch(-45);
	}

	// SceneNode
	{
		m_SceneNode = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNodeFactory>()->CreateSceneNode3D(cSceneNode3D, *this);
		m_SceneNode->SetName("SceneNodeProtoPreview");
	}

	// Model
	{
		m_SceneNodeForModelPreview = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNodeFactory>()->CreateSceneNode3D(cSceneNode3D, *this);
		m_SceneNodeForModelPreview->SetName("NodeModelPreview");
	}

	// Texture
	{
		m_TextureNode = CreateUIControlTCast<IUIControlCommon>(nullptr);
	}

	// Particle
	{
		m_SceneNodeForParticlePreview = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNodeFactory>()->CreateSceneNode3D(cSceneNode3D, *this);
		m_SceneNodeForParticlePreview->SetName("NodeParticlePreview");
		m_SceneNodeForParticlePreview->AddComponent(cSceneNodeParticleComponent, MakeShared(CParticlesComponent, *m_SceneNodeForParticlePreview));
	}

	// Material
	{
		m_SceneNodeForMaterialPreview= GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNodeFactory>()->CreateSceneNode3D(cSceneNode3D, *this);
		m_SceneNodeForMaterialPreview->SetName("NodeMaterialPreview");
		m_SceneNodeForMaterialPreview->AddComponent(cSceneNodeParticleComponent, MakeShared(CParticlesComponent, *m_SceneNodeForParticlePreview));
	}





	/*
	{
		auto node = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNodeFactory>()->CreateSceneNode3D(cSceneNode3D, *this);
		node->SetName("GridNodeX10");
		node->SetPosition(glm::vec3(0.0f, 0.03f, 0.0f));
		node->SetScale(glm::vec3(10.0f));

		auto geom = GetRenderDevice().GetPrimitivesFactory().CreateLines();

		auto mat = MakeShared(MaterialDebug, GetRenderDevice());
		mat->SetDiffuseColor(glm::vec4(0.6f, 0.6f, 0.6f, 1.0f));

		auto model = GetRenderDevice().GetObjectsFactory().CreateModel();
		model->AddConnection(mat, geom);

		node->GetComponentT<IModelComponent>()->SetModel(model);
	}*/
}

void CEditor3DPreviewScene::Finalize()
{
	// Insert code here

	SceneBase::Finalize();
}



//
// Private
//
void CEditor3DPreviewScene::Clean()
{
	// NodeProto preview
	if (m_SceneNode != nullptr)
	{
		while (false == m_SceneNode->GetChilds().empty())
		{
			auto editorChild = *(m_SceneNode->GetChilds().begin());
			m_SceneNode->RemoveChild(editorChild);
		}
	}

	// IModel preview
	if (m_SceneNodeForModelPreview != nullptr)
	{
		if (auto modelComponent = m_SceneNodeForModelPreview->GetComponentT<IModelComponent>())
			modelComponent->ResetModel();
	}

	// ITexture
	if (m_TextureNode != nullptr)
	{
		m_TextureNode->ClearSubgeometries();
	}

	// IParticlesSystem
	if (m_SceneNodeForParticlePreview != nullptr)
	{
		if (auto particlesComponent = m_SceneNodeForParticlePreview->GetComponentT<IParticleComponent3D>())
			particlesComponent->DeleteAllParticleSystem();
	}

	// IMaterials
	if (m_SceneNodeForMaterialPreview != nullptr)
	{
		if (auto modelComponent = m_SceneNodeForModelPreview->GetComponentT<IModelComponent>())
			modelComponent->ResetModel();
	}
}
