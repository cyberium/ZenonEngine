#include "stdafx.h"

// General
#include "Dragger.h"

// Additional
#include "EditorUI/DragUtils.h"

CDragger::CDragger(IEditor3DFrame& Editor3DFrame)
	: m_Editor3DFrame(Editor3DFrame)
	, m_IsDraggingPermanentCreation(false)
{
}

CDragger::~CDragger()
{
}


void CDragger::Initialize(const std::shared_ptr<IRenderer>& Renderer, std::shared_ptr<IRenderTarget> RenderTarget)
{
	m_DraggerTextUI = GetScene().CreateUIControlTCast<IUIControlText>();
	m_DraggerTextUI->SetName("DraggedNodePositionTextUI.");
	m_DraggerTextUI->GetProperties()->GetPropertyT<std::string>("Text")->Set("");
}

bool CDragger::OnMousePressed(const MouseButtonEventArgs & e, const Ray & RayToWorld)
{
	if (m_DraggerNode == nullptr)
		return false;

	if (e.Button == MouseButton::Left)
	{
		CreateCopyDraggedNode();
		return true;
	}
	else if (e.Button == MouseButton::Right)
	{
		Clear();
		return true;
	}

	return false;
}

void CDragger::OnMouseReleased(const MouseButtonEventArgs & e, const Ray & RayToWorld)
{
	// Do nothing
}

void CDragger::OnMouseMoved(const MouseMotionEventArgs & e, const Ray & RayToWorld)
{
	MoveDraggedNode(e.GetPoint());
}

bool CDragger::OnDragEnterEvent(const SDragData& Data)
{
	m_IsDraggingPermanentCreation = Data.IsCtrl;

	auto draggedNode = CreateDraggedNode(Data.Buffer);
	if (draggedNode == nullptr)
		return false;

	m_DraggerNode = draggedNode;
	auto position = CalculateDraggedNodePosition(Data.ScreenPosition);
	m_DraggerNode->SetPosition(position);
	return true;
}

void CDragger::OnDragMoveEvent(const glm::vec2& Position)
{
	MoveDraggedNode(Position);
}

void CDragger::OnDragLeaveEvent()
{
	Clear();
}

void CDragger::OnDropEvent(const glm::vec2& Position)
{
	CreateCopyDraggedNode();

	if (false == m_IsDraggingPermanentCreation)
		Clear();
}




//
// Protected
//
std::shared_ptr<ISceneNode> CDragger::CreateDraggedNode(const CByteBuffer & ByteBuffer)
{
	ETreeViewItemType dragDataSourceType = GetDragDataSourceType(ByteBuffer);
	if (dragDataSourceType == ETreeViewItemType::SceneNodeProto)
	{
		std::shared_ptr<ISceneNode> sceneNode = GetSceneNodeProtoFromDragData(GetBaseManager(), GetScene(), ByteBuffer);
		return sceneNode;
	}
	else if (dragDataSourceType == ETreeViewItemType::Model)
	{
		std::shared_ptr<IModel> model = GetModelFromDragData(GetBaseManager(), ByteBuffer);
		if (model == nullptr)
		{
			Log::Error("CDragger: Unable to create dragged node, because model is nullptr.");
			return nullptr;
		}

		auto draggerNode = GetScene().CreateSceneNodeT<ISceneNode>();
		draggerNode->SetName(model->GetName());
		draggerNode->GetComponentT<IModelComponent>()->SetModel(model);
		return draggerNode;
	}
	/*else if (dragDataSourceType == ETreeViewItemType::Texture)
	{
		std::shared_ptr<ITexture> texture = GetTextureFromDragData(GetBaseManager(), Data.Buffer);
		if (texture == nullptr)
			return;

		m_DraggerNode = GetScene().CreateSceneNodeT<ISceneNode>();
		m_DraggerNode->SetName(model->GetName());
		m_DraggerNode->GetComponentT<IModelComponent>()->SetModel(model);
	}*/
	else if (dragDataSourceType == ETreeViewItemType::ParticleSystem)
	{
		std::shared_ptr<IParticleSystem> particleSystem = GetParticleSystemFromDragData(GetBaseManager(), ByteBuffer);
		if (particleSystem == nullptr)
		{
			Log::Error("CDragger: Unable to create dragged node, because particle system is nullptr.");
			return nullptr;
		}

		auto draggerNode = GetScene().CreateSceneNodeT<ISceneNode>();
		draggerNode->SetName(particleSystem->GetName());

		auto particleSystemComponent = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<IComponentFactory>()->CreateComponentT<IParticleComponent3D>(cSceneNodeParticleComponent, *draggerNode);
		particleSystemComponent->AddParticleSystem(particleSystem);
		particleSystem->SetNode(draggerNode.get());

		draggerNode->AddComponentT<IParticleComponent3D>(particleSystemComponent);

		return draggerNode;
	}

	Log::Warn("EditorToolDragger don't support '%d' drag data source type.", dragDataSourceType);
	return nullptr;
}

void CDragger::MoveDraggedNode(const glm::vec2& MousePos)
{
	if (m_DraggerNode == nullptr)
		return;

	auto position = CalculateDraggedNodePosition(MousePos);
	m_DraggerNode->SetPosition(position);

	// Refresh dragged bounds
	GetEditor().GetTools().GetToolT<IEditorToolSelector>(ETool::EToolSelector).SelectNode(m_DraggerNode);

	m_DraggerTextUI->GetProperties()->GetPropertyT<std::string>("Text")->Set("Pos: " + std::to_string(position.x) + ", " + std::to_string(position.y) + ", " + std::to_string(position.z));
	m_DraggerTextUI->SetTextAlignHorizontal(ETextAlignHorizontal::Center);
	m_DraggerTextUI->SetTextAlignVertical(ETextAlignVertical::Center);
}

void CDragger::CreateCopyDraggedNode()
{
	if (m_DraggerNode == nullptr)
		return;

	auto copiedNode = GetScene().GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNodeFactory>()->CreateSceneNode3D(m_DraggerNode->GetClass(), GetScene());
	m_DraggerNode->CopyTo(copiedNode);

	copiedNode->SetPosition(m_DraggerNode->GetPosition());

	// TODO: Maybe to selected node?
	GetEditor().Get3DFrame().GetEditedRootNode3D()->AddChild(copiedNode);
	GetEditor().GetTools().GetToolT<IEditorToolSelector>(ETool::EToolSelector).SelectNode(copiedNode);

	m_LastCreatedNode = copiedNode;
}

void CDragger::Clear()
{
	m_IsDraggingPermanentCreation = false;

	if (m_DraggerNode)
	{
		if (auto parent = m_DraggerNode->GetParent())
			parent->RemoveChild(m_DraggerNode);
		m_DraggerNode = nullptr;
	}

	// Select last created node
	if (m_LastCreatedNode)
		GetEditor().GetTools().GetToolT<IEditorToolSelector>(ETool::EToolSelector).SelectNode(m_LastCreatedNode);

	m_LastCreatedNode = nullptr;
	m_DraggerTextUI->GetProperties()->GetPropertyT<std::string>("Text")->Set("");

	GetEditor().GetTools().Enable(ETool::EToolDefault);
}

glm::vec3 CDragger::CalculateDraggedNodePosition(const glm::vec2& MousePos)
{
	auto ray = GetScene().GetCameraController()->ScreenToRay(GetScene().GetRenderWindow().GetViewport(), MousePos);

	BoundingBox bounds(glm::vec3(-1.0f), glm::vec3(1.0f));
	if (auto collider = m_DraggerNode->GetComponentT<IColliderComponent>())
	{
		const auto& colliderBounds = collider->GetBounds();
		if (false == colliderBounds.IsInfinite())
			bounds = collider->GetBounds();
	}

	auto pos = GetScene().GetCameraController()->RayToPlane(ray, Plane(glm::vec3(0.0f, 1.0f, 0.0f), bounds.getCenter().y));
	pos -= bounds.getCenter();

	pos = GetEditor().GetUIFrame().FixMoverCoords(pos);

	return pos;
}



//
// Private
//
IEditor & CDragger::GetEditor() const
{
	return m_Editor3DFrame.GetEditor();
}

const IBaseManager & CDragger::GetBaseManager() const
{
	return GetEditor().GetBaseManager();
}

const IRenderDevice & CDragger::GetRenderDevice() const
{
	return GetEditor().GetRenderDevice();
}

IScene & CDragger::GetScene() const
{
	return m_Editor3DFrame.GetScene();
}
