#include "stdafx.h"

// General
#include "Editor3DToolDragger.h"

CEditor3DToolDragger::CEditor3DToolDragger(IEditor& Editor)
	: CEditor3DToolBase(Editor)
	, m_IsDraggingEnabled(false)
	, m_IsDraggingPermanentCreation(false)
{
}

CEditor3DToolDragger::~CEditor3DToolDragger()
{
}

void CEditor3DToolDragger::Initialize()
{
	m_DraggedNode = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNode3DFactory>()->CreateSceneNode3D(cSceneNode3D, GetScene());
	m_DraggedNode->SetName("DraggedNodeRoot");

	m_DraggerTextUI = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNodeUIFactory>()->CreateSceneNodeUI(GetScene(), cSceneNodeUI_Text, GetScene()->GetRootNodeUI());
	m_DraggedNode->SetName("DraggedNodePositionTextUI.");
}

void CEditor3DToolDragger::Finalize()
{
}

void CEditor3DToolDragger::Enable()
{
	CEditor3DToolBase::Enable();
}

void CEditor3DToolDragger::Disable()
{
	CEditor3DToolBase::Disable();
}

bool CEditor3DToolDragger::OnMousePressed(const MouseButtonEventArgs & e, const Ray & RayToWorld)
{
	if (!m_IsDraggingEnabled)
		return false;

	if (e.Button == MouseButtonEventArgs::MouseButton::Left)
	{
		DoDropNodeAndCreateIt();
		return true;
	}
	else if (e.Button == MouseButtonEventArgs::MouseButton::Right)
	{
		DragLeaveEvent();
		return true;
	}

	return false;
}

void CEditor3DToolDragger::OnMouseReleased(const MouseButtonEventArgs & e, const Ray & RayToWorld)
{
	// Do nothing
}

void CEditor3DToolDragger::OnMouseMoved(const MouseMotionEventArgs & e, const Ray & RayToWorld)
{
	if (!m_IsDraggingEnabled)
		return;

	DoMoveNode(e.GetPoint());
}



//
// IEditorToolUI
//
void CEditor3DToolDragger::DoInitializeUI(IEditorQtUIFrame & QtUIFrame)
{
}

void CEditor3DToolDragger::DropEvent(const glm::vec2& Position)
{
	if (!m_IsDraggingEnabled)
		return;

	DoDropNodeAndCreateIt();

	if (!m_IsDraggingPermanentCreation)
		DragLeaveEvent();
}

void CEditor3DToolDragger::DragEnterEvent(const SDragData& Data)
{
	m_IsDraggingEnabled = true;
	m_IsDraggingPermanentCreation = Data.IsCtrl;

	auto ray = GetScene()->GetCameraController()->ScreenToRay(GetScene()->GetRenderWindow()->GetViewport(), Data.Position);
	auto node = CreateNode(glm::vec3(), Data.Message);
	auto pos = GetScene()->GetCameraController()->RayToPlane(ray, Plane(glm::vec3(0.0f, 1.0f, 0.0f), node->GetModelsComponent()->GetModel()->GetBounds().getCenter().y));
	m_DraggedNode->AddChild(node);
	m_DraggedNode->SetTranslate(pos);
}

void CEditor3DToolDragger::DragMoveEvent(const glm::vec2& Position)
{
	if (m_IsDraggingEnabled)
		DoMoveNode(Position);
}

void CEditor3DToolDragger::DragLeaveEvent()
{
	m_IsDraggingEnabled = false;
	m_IsDraggingPermanentCreation = false;
	m_DraggerTextUI->GetProperties()->GetPropertyT<std::string>("Text")->Set("");
	while (!m_DraggedNode->GetChilds().empty())
		m_DraggedNode->RemoveChild(m_DraggedNode->GetChilds()[0]);
}




//
// Protected
//

void CEditor3DToolDragger::DoMoveNode(const glm::vec2& MousePos)
{
	if (!m_IsDraggingEnabled)
		return;

	//auto posReal = GetCameraController()->ScreenToPlane(GetRenderWindow()->GetViewport(), MousePos, Plane(glm::vec3(0.0f, 1.0f, 0.0f), 0.0f));

	auto ray = GetScene()->GetCameraController()->ScreenToRay(GetScene()->GetRenderWindow()->GetViewport(), MousePos);
	auto bounds = m_DraggedNode->GetChilds()[0]->GetModelsComponent()->GetModel()->GetBounds();
	auto pos = GetScene()->GetCameraController()->RayToPlane(ray, Plane(glm::vec3(0.0f, 1.0f, 0.0f), bounds.getMax().y / 2.0f));
	pos -= bounds.getCenter();
	//pos = m_Mover->FixBoxCoords(pos);

	m_DraggedNode->SetTranslate(pos);

	m_DraggerTextUI->GetProperties()->GetPropertyT<std::string>("Text")->Set("Pos: " + std::to_string(pos.x) + ", " + std::to_string(pos.y) + ", " + std::to_string(pos.z));
	m_DraggerTextUI->SetTranslate(MousePos + glm::vec2(0.0f, -15.0f));
}

void CEditor3DToolDragger::DoDropNodeAndCreateIt()
{
	if (!m_IsDraggingEnabled)
		return;

	const auto& childs = m_DraggedNode->GetChilds();
	if (childs.empty())
		throw CException("TEST FUCK!!!!");

	auto firstChild = *childs.begin();

	auto copiedNode = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNode3DFactory>()->CreateSceneNode3D(cSceneNode3D, GetScene());
	firstChild->Copy(copiedNode);
	copiedNode->SetTranslate(m_DraggedNode->GetTranslation());
	GetEditor().Get3DFrame().GetEditedRootNode3D()->AddChild(copiedNode);

	if (!m_IsDraggingPermanentCreation)
	{
		m_DraggedNode->RemoveChild(firstChild);
	}
}

std::shared_ptr<ISceneNode3D> CEditor3DToolDragger::CreateNode(const glm::ivec3& Position, const std::string& Type)
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