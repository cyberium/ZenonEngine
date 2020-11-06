#include "stdafx.h"

// General
#include "EditorToolWaypoints.h"

// Additional
#include "Passes/DrawWaypointsPass.h"


CEditorToolWaypoints::CEditorToolWaypoints(IEditor& Editor)
	: CEditorToolBase(Editor)
	, m_BaseManager(Editor.GetBaseManager())
	, m_IsActivated(false)
{
	
}


CEditorToolWaypoints::~CEditorToolWaypoints()
{
}



//
// IEditorTool
//
void CEditorToolWaypoints::Enable()
{
	CEditorToolBase::Enable();
}

void CEditorToolWaypoints::Disable()
{
	CEditorToolBase::Disable();

	m_IsActivated = false;
}



//
// IEditorToolUI
//
void CEditorToolWaypoints::DoInitializeUI(IEditorQtUIFrame & QtUIFrame)
{
	QtUIFrame.getQObject().connect(QtUIFrame.getUI().RTSEditor_CreateWaypointBtn, &QPushButton::released, [this]() {
		GetEditor().GetTools().Enable(ETool::EToolEditorRTSWaypoints);
		m_IsActivated = true;
	});
}



//
// 3D
//
void CEditorToolWaypoints::DoInitialize3D(const std::shared_ptr<IRenderer>& Renderer, std::shared_ptr<IRenderTarget> RenderTarget, const Viewport * Viewport)
{
	auto m_DrawSelectionPass = MakeShared(CDrawWaypointsPass, GetRenderDevice(), GetScene());
	m_DrawSelectionPass->ConfigurePipeline(RenderTarget, Viewport);
	Renderer->AddPass(m_DrawSelectionPass);
}

bool CEditorToolWaypoints::OnMousePressed(const MouseButtonEventArgs & e, const Ray & RayToWorld)
{
	if (false == m_IsActivated)
		return false;

	auto mousePos = GetScene().GetCameraController()->RayToPlane(RayToWorld, Plane(glm::vec3(0.0f, 1.0f, 0.0f), 2.0f));

	if (e.Button == MouseButton::Left)
	{
		if (m_CurrectWaypointsNode == nullptr)
		{
			m_CurrectWaypointsNode = GetScene().CreateSceneNodeT<ISceneNodeRTSPath>(GetEditor().Get3DFrame().GetEditedRootNode3D());
			m_CurrectWaypointsNode->SetName("Waypoint");
			AddPoint(mousePos);
		}

		AddPoint(mousePos);

		return true;
	}
	else if (e.Button == MouseButton::Right)
	{
		if (m_LastCreatedPoint != nullptr)
			m_CurrectWaypointsNode->RemoveChild(m_LastCreatedPoint);

		m_IsActivated = false;
		m_CurrectWaypointsNode = nullptr;
		m_LastCreatedPoint = nullptr;
	}

	return false;
}

void CEditorToolWaypoints::OnMouseReleased(const MouseButtonEventArgs & e, const Ray & RayToWorld)
{
}

void CEditorToolWaypoints::OnMouseMoved(const MouseMotionEventArgs & e, const Ray & RayToWorld)
{
	if (false == m_IsActivated)
		return;

	if (m_LastCreatedPoint == nullptr)
		return;

	auto bounds = m_LastCreatedPoint->GetComponentT<IColliderComponent3D>()->GetBounds();
	_ASSERT(false == bounds.IsInfinite());

	auto pos = GetScene().GetCameraController()->RayToPlane(RayToWorld, Plane(glm::vec3(0.0f, 1.0f, 0.0f), bounds.getMax().y / 2.0f + 2.0f));
	pos -= bounds.getCenter();

	//pos = dynamic_cast<IEditorToolMover&>(GetEditor().GetTools().GetTool(ETool::EToolMover)).FixBoxCoords(pos);

	auto lastPoint = m_CurrectWaypointsNode->GetLastPoint();
	lastPoint->SetTranslate(pos);

	m_LastCreatedPoint->SetTranslate(pos);
}

void CEditorToolWaypoints::OnNodeSelected(const std::shared_ptr<ISceneNode> SelectedNode)
{
	if (SelectedNode->GetClass() != cSceneNodeRTSPath)
		return;

	m_IsActivated = true;
	m_CurrectWaypointsNode = std::dynamic_pointer_cast<ISceneNodeRTSPath>(SelectedNode);
}


//
// Protected
//
IBaseManager& CEditorToolWaypoints::GetBaseManager() const
{
	return m_BaseManager;
}

IRenderDevice& CEditorToolWaypoints::GetRenderDevice() const
{
	return GetBaseManager().GetApplication().GetRenderDevice();
}

void CEditorToolWaypoints::AddPoint(glm::vec3 XYZ)
{
	m_LastCreatedPoint = m_CurrectWaypointsNode->AddPoint(XYZ);
}
