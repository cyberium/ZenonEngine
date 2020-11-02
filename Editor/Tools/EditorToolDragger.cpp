#include "stdafx.h"

// General
#include "EditorToolDragger.h"

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


void CEditorToolDragger::DoInitialize3D(const std::shared_ptr<IRenderer>& Renderer, std::shared_ptr<IRenderTarget> RenderTarget, const Viewport * Viewport)
{
	m_DraggerTextUI = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNodeUIFactory>()->CreateSceneNodeUI(GetScene(), cSceneNodeUI_Text, GetScene()->GetRootNodeUI());
	m_DraggerTextUI->SetName("DraggedNodePositionTextUI.");

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

	if (!m_IsDraggingPermanentCreation)
		Clear();
}

void CEditorToolDragger::DragEnterEvent(const SDragData& Data)
{
	m_IsDraggingPermanentCreation = Data.IsCtrl;
	m_DraggerNode = CreateNode(glm::vec3(), Data.Message);

	auto ray = GetScene()->GetCameraController()->ScreenToRay(GetScene()->GetRenderWindow().GetViewport(), Data.Position);
	auto pos = GetScene()->GetCameraController()->RayToPlane(ray, Plane(glm::vec3(0.0f, 1.0f, 0.0f), m_DraggerNode->GetComponent<IModelsComponent3D>()->GetModel()->GetBounds().getCenter().y));
	m_DraggerNode->SetTranslate(pos);
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

	auto ray = GetScene()->GetCameraController()->ScreenToRay(GetScene()->GetRenderWindow().GetViewport(), MousePos);
	auto bounds = m_DraggerNode->GetComponent<IModelsComponent3D>()->GetModel()->GetBounds();
	_ASSERT(false == bounds.IsInfinite());
	auto pos = GetScene()->GetCameraController()->RayToPlane(ray, Plane(glm::vec3(0.0f, 1.0f, 0.0f), bounds.getMax().y / 2.0f));
	pos -= bounds.getCenter();

	pos = dynamic_cast<IEditorToolMover&>(GetEditor().GetTools().GetTool(ETool::EToolMover)).FixBoxCoords(pos);

	m_DraggerNode->SetTranslate(pos);

	// Refresh dragged bounds
	GetEditor().GetTools().GetToolT<IEditorToolSelector>(ETool::EToolSelector).SelectNode(m_DraggerNode);

	m_DraggerTextUI->GetProperties()->GetPropertyT<std::string>("Text")->Set("Pos: " + std::to_string(pos.x) + ", " + std::to_string(pos.y) + ", " + std::to_string(pos.z));
	m_DraggerTextUI->SetTranslate(MousePos + glm::vec2(0.0f, -15.0f));
}

void CEditorToolDragger::CreateCopyDraggedNode()
{
	auto copiedNode = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNode3DFactory>()->CreateSceneNode3D(cSceneNode3D, GetScene());
	m_DraggerNode->CopyTo(copiedNode);
	copiedNode->SetTranslate(m_DraggerNode->GetTranslation());
	GetEditor().Get3DFrame().GetEditedRootNode3D()->AddChild(copiedNode);

	m_LastCreatedNode = copiedNode;
	GetEditor().GetTools().GetToolT<IEditorToolSelector>(ETool::EToolSelector).SelectNode(copiedNode);
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

std::shared_ptr<ISceneNode3D> CEditorToolDragger::CreateNode(const glm::ivec3& Position, const std::string& Type)
{
	auto node = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNode3DFactory>()->CreateSceneNode3D(cSceneNode3D, GetEditor().Get3DFrame().GetEditedScene().get());
	node->SetName(Type);

	auto model = GetBaseManager().GetManager<IznModelsFactory>()->LoadModel("models_td/" + Type + ".znmdl");
	node->GetComponent<IModelsComponent3D>()->SetModel(model);

	return node;
}