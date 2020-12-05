#include "stdafx.h"

// Gerenal
#include "Editor3DPreviewScene.h"


namespace
{
	void ExtendsBoundsRecursive(BoundingBox& Bounds, std::shared_ptr<ISceneNode> SceneNode)
	{
		if (auto collider = SceneNode->GetComponentT<IColliderComponent3D>())
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
	_ASSERT(SceneNode != nullptr);

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
	GetCameraController()->GetCamera()->SetTranslation(glm::vec3(radius * 1.5f));
	GetCameraController()->GetCamera()->SetDirection(glm::vec3(-0.5f));

}

void CEditor3DPreviewScene::SetModel(std::shared_ptr<IModel> Model)
{
	_ASSERT(Model != nullptr);

	Clean();

	auto modelComponent = m_ModelNode->GetComponentT<IModelsComponent3D>();
	if (modelComponent->GetModel())
		modelComponent->ResetModel();

	modelComponent->SetModel(Model);

	const auto& modelBBox = Model->GetBounds();
	float radius = modelBBox.getRadius();

	m_ModelNode->SetPosition(- modelBBox.getCenter());
	GetCameraController()->GetCamera()->SetTranslation(glm::vec3(radius * 1.5f));
	GetCameraController()->GetCamera()->SetDirection(glm::vec3(-0.5f));
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

	std::shared_ptr<CMaterialUIControl> contentMaterial = MakeShared(CMaterialUIControl, GetBaseManager().GetApplication().GetRenderDevice());
	contentMaterial->SetTexture(Texture);

	auto textureBounds = CalculateTextureSize(Texture->GetSize(), GetRenderWindow().GetViewport().GetSize());

	IUIControlCommon::SSubgeometry subGeom;
	subGeom.Translate = textureBounds.second;
	subGeom.Size = textureBounds.first;
	subGeom.Material = contentMaterial;
	subGeom.Geom = GetBaseManager().GetApplication().GetRenderDevice().GetPrimitivesFactory().CreateUIQuad(glm::vec2(1.0f));

	m_TextureNode->AddSubgeometry(subGeom);
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
		lightNode->SetRotationEuler(glm::vec3(-0.5f, -0.5f, -0.5f));

		auto lightComponent = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<IComponentFactory>()->CreateComponentT<CLightComponent3D>(cSceneNodeLightComponent, *lightNode.get());
		lightComponent->SetCastShadows(true);
		lightComponent->SetType(ELightType::Spot);
		lightComponent->SetAmbientColor(glm::vec3(0.25f));
		lightComponent->SetColor(glm::vec3(1.0f, 1.0f, 1.0f));
		lightComponent->SetRange(1000.0f);
		lightComponent->SetIntensity(1.2f);
		lightComponent->SetSpotlightAngle(45.0f);

		lightNode->AddComponent(cSceneNodeLightComponent, lightComponent);
	}


	{
		auto cameraNode = CreateSceneNodeT<ISceneNode>();
		cameraNode->SetName("Camera");
		cameraNode->AddComponentT(GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<IComponentFactory>()->CreateComponentT<ICameraComponent3D>(cSceneNodeCameraComponent, *cameraNode));

		SetCameraController(MakeShared(CFreeCameraController));
		GetCameraController()->SetCamera(cameraNode->GetComponentT<ICameraComponent3D>());
		GetCameraController()->GetCamera()->SetPerspectiveProjection(ICameraComponent3D::EPerspectiveProjectionHand::Right, 75.0f, static_cast<float>(GetRenderWindow().GetWindowWidth()) / static_cast<float>(GetRenderWindow().GetWindowHeight()), 1.0f, 5000.0f);
		GetCameraController()->GetCamera()->SetTranslation(glm::vec3(15.0f * 2.0f));
		GetCameraController()->GetCamera()->SetDirection(glm::vec3(-0.5f));
		GetCameraController()->GetCamera()->SetYaw(225);
		GetCameraController()->GetCamera()->SetPitch(-45);
	}

	// SceneNode
	{
		m_SceneNode = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNodeFactory>()->CreateSceneNode3D(cSceneNode3D, *this);
		m_SceneNode->SetName("SceneNodeProtoPreview");
	}

	// Model
	{
		m_ModelNode = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNodeFactory>()->CreateSceneNode3D(cSceneNode3D, *this);
		m_ModelNode->SetName("NodeModelPreview");
	}

	// Texture
	{
		m_TextureNode = CreateUIControlTCast<IUIControlCommon>(nullptr);
	}


	/*{
		auto node = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNodeFactory>()->CreateSceneNode3D(cSceneNode3D, *this);
		node->SetName("GridNodeX10");
		node->SetTranslate(glm::vec3(0.0f, 0.03f, 0.0f));
		node->SetScale(glm::vec3(10.0f));

		auto geom = GetRenderDevice().GetPrimitivesFactory().CreateLines();

		auto mat = MakeShared(MaterialDebug, GetRenderDevice());
		mat->SetDiffuseColor(glm::vec4(0.6f, 0.6f, 0.6f, 1.0f));

		auto model = GetRenderDevice().GetObjectsFactory().CreateModel();
		model->AddConnection(mat, geom);

		node->GetComponentT<IModelsComponent3D>()->SetModel(model);
	}*/


	//
	// Renderer
	//
	auto forwardrender = MakeShared(CRendererForward, GetBaseManager(), *this);
	forwardrender->Initialize(GetRenderWindow().GetRenderTarget());
	SetRenderer(forwardrender);
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
	if (m_SceneNode != nullptr)
	{
		while (m_SceneNode->GetChilds().size() > 0)
		{
			auto editorChild = *(m_SceneNode->GetChilds().begin());
			m_SceneNode->RemoveChild(editorChild);
		}
	}

	if (m_ModelNode != nullptr)
	{
		auto modelComponent = m_ModelNode->GetComponentT<IModelsComponent3D>();
		if (modelComponent->GetModel())
			modelComponent->ResetModel();
	}

	if (m_TextureNode != nullptr)
	{
		m_TextureNode->ClearSubgeometries();
	}
}
