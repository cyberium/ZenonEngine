#include "stdafx.h"

// General
#include "EditorToolDragger.h"

// Additional
#include "DragUtils.h"

CEditorToolDragger::CEditorToolDragger(IEditor& Editor)
	: CEditorToolBase(Editor)
	, m_IsDraggingPermanentCreation(false)
{
}

CEditorToolDragger::~CEditorToolDragger()
{
}

void CEditorToolDragger::Enable()
{
	CEditorToolBase::Enable();
}

void CEditorToolDragger::Disable()
{
	CEditorToolBase::Disable();
}


void CEditorToolDragger::DoInitialize3D(const std::shared_ptr<IRenderer>& Renderer, std::shared_ptr<IRenderTarget> RenderTarget)
{
	m_DraggerTextUI = GetScene().CreateUIControlTCast<IUIControlText>(GetScene().GetRootUIControl());
	m_DraggerTextUI->SetName("DraggedNodePositionTextUI.");
	m_DraggerTextUI->GetProperties()->GetPropertyT<std::string>("Text")->Set("");

}

bool CEditorToolDragger::OnMousePressed(const MouseButtonEventArgs & e, const Ray & RayToWorld)
{
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

void CEditorToolDragger::OnMouseReleased(const MouseButtonEventArgs & e, const Ray & RayToWorld)
{
	// Do nothing
}

void CEditorToolDragger::OnMouseMoved(const MouseMotionEventArgs & e, const Ray & RayToWorld)
{
	MoveDraggedNode(e.GetPoint());
}



//
// IEditorToolUI
//
void CEditorToolDragger::DoInitializeUI(IEditorQtUIFrame & QtUIFrame)
{
}

void CEditorToolDragger::DropEvent(const glm::vec2& Position)
{
	CreateCopyDraggedNode();

	if (false == m_IsDraggingPermanentCreation)
		Clear();
}

bool CEditorToolDragger::DragEnterEvent(const SDragData& Data)
{
	m_IsDraggingPermanentCreation = Data.IsCtrl;

	EDragDataSourceType dragDataSourceType = GetDragDataSourceType(Data.Buffer);
	if (dragDataSourceType == EDragDataSourceType::SceneNodeProto)
	{
		std::shared_ptr<ISceneNode> sceneNode = GetSceneNodeFromDragData(GetBaseManager(), GetScene(), Data.Buffer);
		
		m_DraggerNode = sceneNode;
	}
	else if (dragDataSourceType == EDragDataSourceType::Model)
	{
		std::shared_ptr<IModel> model = GetModelFromDragData(GetBaseManager(), Data.Buffer);
		if (model == nullptr)
			return false;

		m_DraggerNode = GetScene().CreateSceneNodeT<ISceneNode>();
		m_DraggerNode->SetName(model->GetName());
		m_DraggerNode->GetComponentT<IModelComponent>()->SetModel(model);
	}
	/*else if (dragDataSourceType == EDragDataSourceType::Texture)
	{
		std::shared_ptr<ITexture> texture = GetTextureFromDragData(GetBaseManager(), Data.Buffer);
		if (texture == nullptr)
			return;

		m_DraggerNode = GetScene().CreateSceneNodeT<ISceneNode>();
		m_DraggerNode->SetName(model->GetName());
		m_DraggerNode->GetComponentT<IModelComponent>()->SetModel(model);
	}*/
	else
	{
		Log::Warn("EditorToolDragger don't support '%d' drag data source type.", dragDataSourceType);
		return false;
	}

	auto ray = GetScene().GetCameraController()->ScreenToRay(GetScene().GetRenderWindow().GetViewport(), Data.ScreenPosition);
	if (auto collider = m_DraggerNode->GetComponentT<IColliderComponent>())
	{
		const auto& colliderBounds = collider->GetBounds();
		if (false == colliderBounds.IsInfinite())
		{
			auto pos = GetScene().GetCameraController()->RayToPlane(ray, Plane(glm::vec3(0.0f, 1.0f, 0.0f), m_DraggerNode->GetComponentT<IColliderComponent>()->GetBounds().getCenter().y));
			m_DraggerNode->SetPosition(pos);
			return true;
		}
	}

	auto pos = GetScene().GetCameraController()->RayToPlane(ray, Plane(glm::vec3(0.0f, 1.0f, 0.0f), 0.0f));
	m_DraggerNode->SetPosition(pos);
	return true;
}

void CEditorToolDragger::DragMoveEvent(const glm::vec2& Position)
{
	MoveDraggedNode(Position);
}

void CEditorToolDragger::DragLeaveEvent()
{
	Clear();
}




//
// Protected
//
void CEditorToolDragger::MoveDraggedNode(const glm::vec2& MousePos)
{
	if (m_DraggerNode == nullptr)
		return;

	//auto posReal = GetCameraController()->ScreenToPlane(GetRenderWindow()->GetViewport(), MousePos, Plane(glm::vec3(0.0f, 1.0f, 0.0f), 0.0f));

	auto ray = GetScene().GetCameraController()->ScreenToRay(GetScene().GetRenderWindow().GetViewport(), MousePos);

	BoundingBox bounds(glm::vec3(-1.0f), glm::vec3(1.0f));
	if (auto collider = m_DraggerNode->GetComponentT<IColliderComponent>())
	{
		const auto& colliderBounds = collider->GetBounds();
		if (false == colliderBounds.IsInfinite())
			bounds = collider->GetBounds();
	}

	auto pos = GetScene().GetCameraController()->RayToPlane(ray, Plane(glm::vec3(0.0f, 1.0f, 0.0f), bounds.getMax().y / 2.0f));
	pos -= bounds.getCenter();

	pos = dynamic_cast<IEditorToolMover&>(GetEditor().GetTools().GetTool(ETool::EToolMover)).FixBoxCoords(pos);

	m_DraggerNode->SetPosition(pos);

	// Refresh dragged bounds
	GetEditor().GetTools().GetToolT<IEditorToolSelector>(ETool::EToolSelector).SelectNode(m_DraggerNode);

	m_DraggerTextUI->GetProperties()->GetPropertyT<std::string>("Text")->Set("Pos: " + std::to_string(pos.x) + ", " + std::to_string(pos.y) + ", " + std::to_string(pos.z));
	m_DraggerTextUI->SetPosition(MousePos + glm::vec2(0.0f, -15.0f));
}

void CEditorToolDragger::CreateCopyDraggedNode()
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

void CEditorToolDragger::Clear()
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
