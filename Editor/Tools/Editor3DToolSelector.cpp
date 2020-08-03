#include "stdafx.h"

// General
#include "Editor3DToolSelector.h"

CEditor3DToolSelector::CEditor3DToolSelector(IEditor& Editor)
	: CEditorToolSelector(dynamic_cast<IEditor_NodesSelectorEventListener&>(Editor))
	, CEditor3DToolBase(Editor)
	, m_IsSelecting2D(false)
{
}

CEditor3DToolSelector::~CEditor3DToolSelector()
{
}

void CEditor3DToolSelector::Initialize()
{
	m_SelectionTexture = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNodeUIFactory>()->CreateSceneNodeUI(GetScene(), cSceneNodeUI_Color, GetScene()->GetRootNodeUI());
	m_SelectionTexture->GetProperties()->GetPropertyT<glm::vec4>("Color")->Set(glm::vec4(0.1f, 0.3f, 1.0f, 0.3f));
}

void CEditor3DToolSelector::Finalize()
{
}

void CEditor3DToolSelector::Enable()
{
	CEditor3DToolBase::Enable();

	dynamic_cast<IEditorQtUIFrame&>(GetEditor().GetUIFrame()).getUI().editorToolSelectorBtn->setChecked(IsEnabled());

	// Always enabled
	//m_DrawSelectionPass->SetEnabled(true);
}

void CEditor3DToolSelector::Disable()
{
	CEditor3DToolBase::Disable();

	dynamic_cast<IEditorQtUIFrame&>(GetEditor().GetUIFrame()).getUI().editorToolSelectorBtn->setChecked(IsEnabled());

	// Always enabled
	//m_DrawSelectionPass->SetEnabled(false);
}


void CEditor3DToolSelector::AddPasses(RenderTechnique& RenderTechnique, std::shared_ptr<IRenderTarget> RenderTarget, const Viewport * Viewport)
{
	m_DrawSelectionPass = std::make_shared<CDrawSelectionPass>(GetRenderDevice(), *this);
	m_DrawSelectionPass->CreatePipeline(RenderTarget, Viewport);
	RenderTechnique.AddPass(m_DrawSelectionPass);
}

bool CEditor3DToolSelector::OnMousePressed(const MouseButtonEventArgs & e, const Ray & RayToWorld)
{
	auto nodes = GetScene()->FindIntersection(RayToWorld, GetEditor().Get3DFrame().GetEditedRootNode3D());
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

void CEditor3DToolSelector::OnMouseReleased(const MouseButtonEventArgs & e, const Ray & RayToWorld)
{
	if (!IsEnabled())
		return;

	if (e.Button == MouseButtonEventArgs::MouseButton::Left)
	{
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

				auto nodes = GetScene()->FindIntersections(f, GetEditor().Get3DFrame().GetEditedRootNode3D());
				if (!nodes.empty())
					SelectNodes(nodes);
			}
			m_IsSelecting2D = false;
		}
	}
}

void CEditor3DToolSelector::OnMouseMoved(const MouseMotionEventArgs & e, const Ray & RayToWorld)
{
	if (!IsEnabled())
		return;

	if (m_IsSelecting2D)
	{
		glm::vec2 scale = e.GetPoint() - m_SelectionTexture->GetTranslation();
		m_SelectionTexture->SetScale(scale);
	}
}



//
// IEditorToolUI
//
void CEditor3DToolSelector::DoInitializeUI(IEditorQtUIFrame& QtUIFrame)
{
	QtUIFrame.getQObject().connect(QtUIFrame.getUI().editorToolSelectorBtn, &QPushButton::released, [this]() {
		GetEditor().GetTools().Enable(ETool::EToolSelector);
	});
}



//
// Protected
//
void CEditor3DToolSelector::RaiseSelectEvent()
{
	CEditorToolSelector::RaiseSelectEvent();

	m_DrawSelectionPass->SetNeedRefresh();
}
