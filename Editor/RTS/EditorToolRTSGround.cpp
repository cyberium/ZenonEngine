#include "stdafx.h"

// General
#include "EditorToolRTSGround.h"

// Additional
#include "Materials/MaterialEditorTool.h"

/*
ground_dirt
ground_dirtRiverTile
-z g
+z g
-x g
+x g

ground_dirtRiver
ground_dirtRiverBanks
ground_dirtRiverRocks
-z r
+z r
-x g
+x g

ground_dirtRiverBanks
-z r
+z r
-x g
+x g

ground_dirtRiverCorner
ground_dirtRiverCornerBank
-z r
+z g
-x g
+x r

ground_dirtRiverCornerInner ???
ground_dirtRiverEntrance ???
ground_dirtRiverSide ???
ground_dirtRiverSideCorner ???
ground_dirtRiverWater ???

ground_dirtRiverCrossing
-z r
+z r
-x r
+x r

ground_dirtRiverEnd
-z g
+z r
-x g
+x g

ground_dirtRiverT
-z g
+z r
-x r
+x r

*/



CRTSGround::CRTSGround(IEditor& Editor)
	: CEditorToolBase(Editor)
	, m_BaseManager(Editor.GetBaseManager())
	, m_Scene(Editor.Get3DFrame().GetEditedScene())
	, m_CellType(ERTSCellType::ctEmpty)
{
	
}


CRTSGround::~CRTSGround()
{
}



//
// IEditorTool
//
void CRTSGround::Initialize()
{
	auto groundRootSceneNode = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNode3DFactory>()->CreateSceneNode3D(cCRTSSceneNodeGround, GetScene(), GetEditor().Get3DFrame().GetEditedRootNode3D());
	groundRootSceneNode->SetName("GroundRoot");
	m_GroundRoot = std::dynamic_pointer_cast<IRTSGround>(groundRootSceneNode);

	auto geom = GetRenderDevice().GetPrimitivesFactory().CreateTorus(5.0f, 0.25f);

	auto materialY = MakeShared(MaterialEditorTool, GetRenderDevice());
	materialY->SetColor(glm::vec4(0.3f, 1.0f, 0.3f, 1.0f));
	auto modelY = GetRenderDevice().GetObjectsFactory().CreateModel();
	modelY->AddConnection(materialY, geom);

	m_GroundSelectorNode = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNode3DFactory>()->CreateSceneNode3D(cSceneNode3D, GetScene());
	m_GroundSelectorNode->SetName("GroundSelectedNode");
	m_GroundSelectorNode->GetComponent<IModelsComponent3D>()->SetModel(modelY);
	m_GroundSelectorNode->GetComponent<IColliderComponent3D>()->SetBounds(geom->GetBounds());
}

void CRTSGround::Finalize()
{
}

void CRTSGround::Enable()
{
	CEditorToolBase::Enable();

	//dynamic_cast<IEditorQtUIFrame&>(GetEditor().GetUIFrame()).getUI().RTSEditor_GroundBtn->setChecked(IsEnabled());
}

void CRTSGround::Disable()
{
	CEditorToolBase::Disable();

	SetNodeType(ERTSCellType::ctEmpty);
	//dynamic_cast<IEditorQtUIFrame&>(GetEditor().GetUIFrame()).getUI().RTSEditor_GroundBtn->setChecked(IsEnabled());
}



//
// IEditorToolUI
//
void CRTSGround::DoInitializeUI(IEditorQtUIFrame & QtUIFrame)
{
	QtUIFrame.getQObject().connect(QtUIFrame.getUI().RTSEditor_GroundBtn, &QPushButton::released, [this]() {
		GetEditor().GetTools().Enable(ETool::EToolEditorRTS);
		SetNodeType(ERTSCellType::ctGround);
	});

	QtUIFrame.getQObject().connect(QtUIFrame.getUI().RTSEditor_WaterBtn, &QPushButton::released, [this]() {
		GetEditor().GetTools().Enable(ETool::EToolEditorRTS);
		SetNodeType(ERTSCellType::ctWater);
	});
}



//
// 3D
//
bool CRTSGround::OnMousePressed(const MouseButtonEventArgs & e, const Ray & RayToWorld)
{
	auto mousePos = GetScene()->GetCameraController()->RayToPlane(RayToWorld, Plane(glm::vec3(0.0f, 1.0f, 0.0f), 0.0f));

	SRTSCellCoords coords = m_GroundRoot->PositionToCoords(mousePos);
	if (false == coords.IsCorrect())
		return false;

	if (e.Button == MouseButton::Left)
	{
		if (m_GroundRoot->AddCell(m_CellType, coords))
			return true;
	}
	else if (e.Button == MouseButton::Right)
	{
		if (m_GroundRoot->AddCell(ERTSCellType::ctEmpty, coords))
			return true;
	}

	return false;
}

void CRTSGround::OnMouseReleased(const MouseButtonEventArgs & e, const Ray & RayToWorld)
{
}

void CRTSGround::OnMouseMoved(const MouseMotionEventArgs & e, const Ray & RayToWorld)
{
	auto mousePos = GetScene()->GetCameraController()->RayToPlane(RayToWorld, Plane(glm::vec3(0.0f, 1.0f, 0.0f), 0.0f));

	if (m_GroundSelectorNode)
		m_GroundSelectorNode->SetTranslate(m_GroundRoot->PositionToPosition(mousePos));

	if (e.LeftButton)
	{
		auto coords = m_GroundRoot->PositionToCoords(mousePos);
		if (false == coords.IsCorrect())
			return;

		m_GroundRoot->AddCell(m_CellType, coords);
	}
	else if (e.RightButton)
	{
		auto coords = m_GroundRoot->PositionToCoords(mousePos);
		if (false == coords.IsCorrect())
			return;

		m_GroundRoot->AddCell(ERTSCellType::ctEmpty, coords);
	}
}


void CRTSGround::SetNodeType(ERTSCellType CellType)
{
	m_CellType = CellType;
}



//
// Protected
//
IBaseManager& CRTSGround::GetBaseManager() const
{
	return m_BaseManager;
}

IRenderDevice& CRTSGround::GetRenderDevice() const
{
	return GetBaseManager().GetApplication().GetRenderDevice();
}
