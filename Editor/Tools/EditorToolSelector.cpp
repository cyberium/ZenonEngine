#include "stdafx.h"

// General
#include "EditorToolSelector.h"

CEditorToolSelector::CEditorToolSelector(IEditor& Editor)
	: m_NodesSelectorEventListener(dynamic_cast<IEditorToolSelectorEventListener&>(Editor))
	, CEditorToolBase(Editor)
	, m_IsSelecting2D(false)
{
}

CEditorToolSelector::~CEditorToolSelector()
{
}

void CEditorToolSelector::Initialize()
{
}

void CEditorToolSelector::Finalize()
{
}

void CEditorToolSelector::Enable()
{
	CEditorToolBase::Enable();

	dynamic_cast<IEditorQtUIFrame&>(GetEditor().GetUIFrame()).getUI().editorToolSelectorBtn->setChecked(IsEnabled());

	// Always enabled
	//m_DrawSelectionPass->SetEnabled(true);
}

void CEditorToolSelector::Disable()
{
	CEditorToolBase::Disable();

	dynamic_cast<IEditorQtUIFrame&>(GetEditor().GetUIFrame()).getUI().editorToolSelectorBtn->setChecked(IsEnabled());

	// Always enabled
	//m_DrawSelectionPass->SetEnabled(false);
	m_IsSelecting2D = false;
}



//
// IEditorToolSelector
//
void CEditorToolSelector::SelectNode(std::shared_ptr<ISceneNode3D> Node)
{
	m_SelectedNodes.clear();
	m_SelectedNodes.push_back(Node);

	RaiseSelectEvent();
}

void CEditorToolSelector::SelectNodes(const std::vector<std::shared_ptr<ISceneNode3D>>& Nodes)
{
	m_SelectedNodes.clear();
	for (const auto& n : Nodes)
		m_SelectedNodes.push_back(n);

	RaiseSelectEvent();
}

void CEditorToolSelector::ClearSelection()
{
	m_SelectedNodes.clear();

	RaiseSelectEvent();
}

void CEditorToolSelector::AddNode(std::shared_ptr<ISceneNode3D> Node)
{
	auto it = std::find_if(m_SelectedNodes.begin(), m_SelectedNodes.end(), [Node](const std::weak_ptr<ISceneNode3D>& NodeW) -> bool {
		if (auto locked = NodeW.lock())
			return locked == Node;
		return false;
	});

	if (it != m_SelectedNodes.end())
		return;

	m_SelectedNodes.push_back(Node);

	RaiseSelectEvent();
}

void CEditorToolSelector::RemoveNode(std::shared_ptr<ISceneNode3D> Node)
{
	auto it = std::find_if(m_SelectedNodes.begin(), m_SelectedNodes.end(), [Node] (const std::weak_ptr<ISceneNode3D>& NodeW) -> bool {
		if (auto locked = NodeW.lock())
			return locked == Node;
		return false;
	});

	if (it == m_SelectedNodes.end())
		return;

	m_SelectedNodes.erase(it);

	RaiseSelectEvent();
}

std::shared_ptr<ISceneNode3D> CEditorToolSelector::GetFirstSelectedNode()
{
	auto first = m_SelectedNodes.begin();
	if (first == m_SelectedNodes.end())
		return nullptr;

	if (auto firstLocked = (*first).lock())
		return firstLocked;

	return nullptr;
}

bool CEditorToolSelector::IsNodeSelected(std::shared_ptr<ISceneNode3D> Node)
{
	auto it = std::find_if(m_SelectedNodes.begin(), m_SelectedNodes.end(), [Node](const std::weak_ptr<ISceneNode3D>& NodeW) -> bool {
		if (auto locked = NodeW.lock())
			return locked == Node;
		return false;
	});

	return it != m_SelectedNodes.end();
}

const SelectedNodes& CEditorToolSelector::GetSelectedNodes()
{
	return m_SelectedNodes;
}



//
// CEditorToolBase
//
void CEditorToolSelector::DoInitialize3D(const std::shared_ptr<IRenderer>& Renderer, std::shared_ptr<IRenderTarget> RenderTarget, const Viewport * Viewport)
{
	m_SelectionTexture = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNodeUIFactory>()->CreateSceneNodeUI(GetScene(), cSceneNodeUI_Color, GetScene()->GetRootNodeUI());
	m_SelectionTexture->GetProperties()->GetPropertyT<glm::vec4>("Color")->Set(glm::vec4(0.1f, 0.3f, 1.0f, 0.3f));

	m_DrawSelectionPass = MakeShared(CDrawSelectionPass, GetRenderDevice(), *this);
	m_DrawSelectionPass->CreatePipeline(Renderer->GetRenderTarget(), Viewport);
	Renderer->AddPass(m_DrawSelectionPass);
}

bool CEditorToolSelector::OnMousePressed(const MouseButtonEventArgs & e, const Ray & RayToWorld)
{
	if (e.Button != MouseButtonEventArgs::MouseButton::Left)
		return false;

	auto nodes = GetScene()->GetFinder().FindIntersection(RayToWorld, nullptr, GetEditor().Get3DFrame().GetEditedRootNode3D());
	if (nodes.empty())
	{
		if (IsEnabled())
		{
			m_SelectionPrevPos = e.GetPoint();
			m_SelectionTexture->SetTranslate(e.GetPoint());
			m_IsSelecting2D = true;
			return true;
		}

		return false;
	}

	auto node = nodes.begin()->second;
	_ASSERT(node != nullptr);

	if (IsEnabled())
	{
		if (e.Control)
		{
			if (IsNodeSelected(node))
				RemoveNode(node);
			else
				AddNode(node);

			return true;
		}
	}

	SelectNode(node);

	return true;
}

void CEditorToolSelector::OnMouseReleased(const MouseButtonEventArgs & e, const Ray & RayToWorld)
{
	if (e.Button != MouseButtonEventArgs::MouseButton::Left)
		return;

	auto cachedSelectionPrevPos = m_SelectionPrevPos;

	// Clear
	m_SelectionTexture->SetScale(glm::vec3(0.001f));
	m_SelectionPrevPos = e.GetPoint();

	if (m_IsSelecting2D)
	{
		if (glm::length(glm::abs(cachedSelectionPrevPos - e.GetPoint())) > 10.0f)
		{
			Frustum f;
			f.buildBoxFrustum(
				GetScene()->GetCameraController()->ScreenToRay(GetScene()->GetRenderWindow()->GetViewport(), glm::vec2(glm::min(cachedSelectionPrevPos.x, e.GetPoint().x), glm::min(cachedSelectionPrevPos.y, e.GetPoint().y))),
				GetScene()->GetCameraController()->ScreenToRay(GetScene()->GetRenderWindow()->GetViewport(), glm::vec2(glm::min(cachedSelectionPrevPos.x, e.GetPoint().x), glm::max(cachedSelectionPrevPos.y, e.GetPoint().y))),
				GetScene()->GetCameraController()->ScreenToRay(GetScene()->GetRenderWindow()->GetViewport(), glm::vec2(glm::max(cachedSelectionPrevPos.x, e.GetPoint().x), glm::min(cachedSelectionPrevPos.y, e.GetPoint().y))),
				GetScene()->GetCameraController()->ScreenToRay(GetScene()->GetRenderWindow()->GetViewport(), glm::vec2(glm::max(cachedSelectionPrevPos.x, e.GetPoint().x), glm::max(cachedSelectionPrevPos.y, e.GetPoint().y))),
				10000.0f
			);

			auto nodes = GetScene()->GetFinder().FindIntersections(f, nullptr, GetEditor().Get3DFrame().GetEditedRootNode3D());
			if (!nodes.empty())
				SelectNodes(nodes);
		}
		m_IsSelecting2D = false;
	}
}

void CEditorToolSelector::OnMouseMoved(const MouseMotionEventArgs & e, const Ray & RayToWorld)
{
	if (m_IsSelecting2D)
	{
		glm::vec2 scale = e.GetPoint() - m_SelectionTexture->GetTranslation();
		m_SelectionTexture->SetScale(scale);
	}
}



//
// IEditorToolUI
//
void CEditorToolSelector::DoInitializeUI(IEditorQtUIFrame& QtUIFrame)
{
	QtUIFrame.getQObject().connect(QtUIFrame.getUI().editorToolSelectorBtn, &QPushButton::released, [this]() {
		GetEditor().GetTools().Enable(ETool::EToolSelector);
	});
}



//
// Protected
//
void CEditorToolSelector::RaiseSelectEvent()
{
	m_NodesSelectorEventListener.OnSelectNode();

	m_DrawSelectionPass->SetNeedRefresh();
}
