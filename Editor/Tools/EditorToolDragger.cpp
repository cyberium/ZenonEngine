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

void CEditorToolDragger::Initialize()
{
	m_DraggerTextUI = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNodeUIFactory>()->CreateSceneNodeUI(GetScene(), cSceneNodeUI_Text, GetScene()->GetRootNodeUI());
	m_DraggerTextUI->SetName("DraggedNodePositionTextUI.");
}

void CEditorToolDragger::Finalize()
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

bool CEditorToolDragger::OnMousePressed(const MouseButtonEventArgs & e, const Ray & RayToWorld)
{
	if (e.Button == MouseButtonEventArgs::MouseButton::Left)
	{
		CreateCopyDraggedNode();
		return true;
	}
	else if (e.Button == MouseButtonEventArgs::MouseButton::Right)
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

	auto ray = GetScene()->GetCameraController()->ScreenToRay(GetScene()->GetRenderWindow()->GetViewport(), Data.Position);
	auto pos = GetScene()->GetCameraController()->RayToPlane(ray, Plane(glm::vec3(0.0f, 1.0f, 0.0f), m_DraggerNode->GetModelsComponent()->GetModel()->GetBounds().getCenter().y));
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
	//auto posReal = GetCameraController()->ScreenToPlane(GetRenderWindow()->GetViewport(), MousePos, Plane(glm::vec3(0.0f, 1.0f, 0.0f), 0.0f));

	auto ray = GetScene()->GetCameraController()->ScreenToRay(GetScene()->GetRenderWindow()->GetViewport(), MousePos);
	auto bounds = m_DraggerNode->GetModelsComponent()->GetModel()->GetBounds();
	auto pos = GetScene()->GetCameraController()->RayToPlane(ray, Plane(glm::vec3(0.0f, 1.0f, 0.0f), bounds.getMax().y / 2.0f));
	pos -= bounds.getCenter();
	//pos = pos / 10.0f;//reinterpret_cast<IEditorToolMover&>(GetEditor().GetTools().GetTool(EToolMover)).FixBoxCoords(pos);

	pos /= 10.0f;
	pos = glm::round(pos);
	pos *= 10.0f;

	m_DraggerNode->SetTranslate(pos);

	// Refresh dragged bounds
	dynamic_cast<IEditorToolSelector&>(GetEditor().GetTools().GetTool(ETool::EToolSelector)).SelectNode(m_DraggerNode);

	m_DraggerTextUI->GetProperties()->GetPropertyT<std::string>("Text")->Set("Pos: " + std::to_string(pos.x) + ", " + std::to_string(pos.y) + ", " + std::to_string(pos.z));
	m_DraggerTextUI->SetTranslate(MousePos + glm::vec2(0.0f, -15.0f));
}

void CEditorToolDragger::CreateCopyDraggedNode()
{
	auto copiedNode = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNode3DFactory>()->CreateSceneNode3D(cSceneNode3D, GetScene());
	m_DraggerNode->Copy(copiedNode);
	copiedNode->SetTranslate(m_DraggerNode->GetTranslation());
	GetEditor().Get3DFrame().GetEditedRootNode3D()->AddChild(copiedNode);

	m_LastCreatedNode = copiedNode;
	dynamic_cast<IEditorToolSelector&>(GetEditor().GetTools().GetTool(ETool::EToolSelector)).SelectNode(copiedNode);
}

void CEditorToolDragger::Clear()
{
	m_IsDraggingPermanentCreation = false;
	if (m_DraggerNode)
	{
		if (auto parent = m_DraggerNode->GetParent().lock())
			parent->RemoveChild(m_DraggerNode);
		m_DraggerNode = nullptr;
	}

	// Select last created node
	if (m_LastCreatedNode)
		dynamic_cast<IEditorToolSelector&>(GetEditor().GetTools().GetTool(ETool::EToolSelector)).SelectNode(m_LastCreatedNode);

	m_LastCreatedNode = nullptr;
	m_DraggerTextUI->GetProperties()->GetPropertyT<std::string>("Text")->Set("");

	GetEditor().GetTools().Enable(ETool::EToolDefault);
}

std::shared_ptr<ISceneNode3D> CEditorToolDragger::CreateNode(const glm::ivec3& Position, const std::string& Type)
{
	auto node = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNode3DFactory>()->CreateSceneNode3D(cSceneNode3D, GetEditor().Get3DFrame().GetEditedScene().get());
	node->SetName(Type);
	auto model = GetRenderDevice().GetObjectsFactory().CreateModel();
	if (auto loadable = std::dynamic_pointer_cast<IObjectLoadSave>(model))
	{
		auto fileName = "C:\\_engine\\ZenonEngine_gamedata\\models\\" + Type + ".fbx.znmdl";
		auto file = GetBaseManager().GetManager<IFilesManager>()->Open(fileName);
		if (file == nullptr)
			throw CException("File not found.");
		loadable->Load(file);
		model->SetFileName(fileName);
	}

	node->GetComponent<IModelsComponent3D>()->SetModel(model);
	node->GetComponent<IColliderComponent3D>()->SetBounds(model->GetBounds());

	return node;
}