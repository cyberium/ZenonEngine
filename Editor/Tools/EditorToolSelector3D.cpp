#include "stdafx.h"

// General
#include "EditorToolSelector3D.h"

CEditorToolSelector3D::CEditorToolSelector3D(IEditor3DFrame & EditorFrame)
	: CEditorToolSelector(reinterpret_cast<IEditor_NodesSelectorEventListener&>(EditorFrame))
	, CEditor3DToolBase(EditorFrame)
	, m_IsSelecting2D(false)
{
}

CEditorToolSelector3D::~CEditorToolSelector3D()
{
}

void CEditorToolSelector3D::Initialize()
{
	m_SelectionTexture = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNodeUIFactory>()->CreateSceneNodeUI(GetScene(), cSceneNodeUI_Color, GetScene()->GetRootNodeUI());
	m_SelectionTexture->GetProperties()->GetPropertyT<glm::vec4>("Color")->Set(glm::vec4(0.1f, 0.3f, 1.0f, 0.3f));
}

void CEditorToolSelector3D::Finalize()
{
}

void CEditorToolSelector3D::Enable()
{
	CEditor3DToolBase::Enable();

	// Always enabled
	//m_DrawSelectionPass->SetEnabled(true);
}

void CEditorToolSelector3D::Disable()
{
	CEditor3DToolBase::Disable();

	// Always enabled
	//m_DrawSelectionPass->SetEnabled(false);
}

bool CEditorToolSelector3D::OnMouseClickToWorld(const MouseButtonEventArgs & e, const Ray & RayToWorld)
{
	auto nodes = GetScene()->FindIntersection(RayToWorld, GetEditor3DFrame().GetRealRootNode3D());
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

void CEditorToolSelector3D::OnMouseReleaseToWorld(const MouseButtonEventArgs & e, const Ray & RayToWorld)
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

				auto nodes = GetScene()->FindIntersections(f, GetEditor3DFrame().GetRealRootNode3D());
				if (!nodes.empty())
					SelectNodes(nodes);
			}
			m_IsSelecting2D = false;
		}
	}
}

void CEditorToolSelector3D::OnMouseMoveToWorld(const MouseMotionEventArgs & e, const Ray & RayToWorld)
{
	if (!IsEnabled())
		return;

	if (m_IsSelecting2D)
	{
		glm::vec2 scale = e.GetPoint() - m_SelectionTexture->GetTranslation();
		m_SelectionTexture->SetScale(scale);
	}
}

void CEditorToolSelector3D::AddPasses(RenderTechnique& RenderTechnique, std::shared_ptr<IRenderTarget> RenderTarget, const Viewport * Viewport)
{
	m_DrawSelectionPass = std::make_shared<CDrawSelectionPass>(GetRenderDevice(), *this);
	m_DrawSelectionPass->CreatePipeline(RenderTarget, Viewport);
	RenderTechnique.AddPass(m_DrawSelectionPass);
}


//
// Protected
//
void CEditorToolSelector3D::RaiseSelectEvent()
{
	CEditorToolSelector::RaiseSelectEvent();

	m_DrawSelectionPass->SetNeedRefresh();
}
