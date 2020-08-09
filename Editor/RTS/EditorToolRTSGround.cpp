#include "stdafx.h"

// General
#include "EditorToolRTSGround.h"

CRTSGround::CRTSGround(IEditor& Editor)
	: CEditorToolBase(Editor)
	, m_CellSize(10.0f)
	, m_Scene(m_Scene)
{
	m_GroundRoot = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNode3DFactory>()->CreateSceneNode3D(cSceneNode3D, &m_Scene);
}


CRTSGround::~CRTSGround()
{
}



//
// IEditorTool
//
void CRTSGround::Initialize()
{
}

void CRTSGround::Finalize()
{
}

void CRTSGround::Enable()
{
}

void CRTSGround::Disable()
{
}

bool CRTSGround::OnMousePressed(const MouseButtonEventArgs & e, const Ray & RayToWorld)
{
	return false;
}

void CRTSGround::OnMouseReleased(const MouseButtonEventArgs & e, const Ray & RayToWorld)
{
}

void CRTSGround::OnMouseMoved(const MouseMotionEventArgs & e, const Ray & RayToWorld)
{
}

void CRTSGround::AddCell(CRTSGround::SRTSCell::ERTSCellType CellType, uint32 IndexX, uint32 IndexZ)
{
	auto cell = GetCell(IndexX, IndexZ);
	auto node = cell.Node;

	if (node != nullptr)
	{
		auto parent = node->GetParent();
		if (!parent.expired())
		{
			if (auto parentPtr = parent.lock())
			{
				_ASSERT(parentPtr == m_GroundRoot);
				parentPtr->RemoveChild(node);
			}
		}
	}

	auto modelName = "";
	if (CellType == CRTSGround::SRTSCell::ERTSCellType::ctGround)
	{
		modelName = "C:\\_engine\\ZenonEngine_gamedata\\models\\ground_dirt.fbx.znmdl";
	}
	else if (CellType == CRTSGround::SRTSCell::ERTSCellType::ctWater)
	{
		modelName = "C:\\_engine\\ZenonEngine_gamedata\\models\\ground_dirtRiver.fbx.znmdl";
	}
	
	if (modelName == "")
		return;

	cell.Type = CellType;
	
	auto model = GetRenderDevice().GetObjectsFactory().CreateModel();
	if (auto loadable = std::dynamic_pointer_cast<IObjectLoadSave>(model))
	{
		auto file = GetBaseManager().GetManager<IFilesManager>()->Open(modelName);
		if (file == nullptr)
			throw CException("File not found.");
		loadable->Load(file);
		model->SetFileName(modelName);
	}

	cell.Node = GetBaseManager().GetManager<IObjectsFactory>()->GetClassFactoryCast<ISceneNode3DFactory>()->CreateSceneNode3D(cSceneNode3D, &m_Scene, m_GroundRoot);
	cell.Node->GetComponent<IModelsComponent3D>()->SetModel(model);
	cell.Node->GetComponent<IColliderComponent3D>()->SetBounds(model->GetBounds());
	cell.Node->SetTranslate(glm::vec3(1.0f * (IndexX - 32), 0.0f, 1.0f * (IndexZ - 32)));
}

CRTSGround::SRTSCell& CRTSGround::GetCell(uint32 IndexX, uint32 IndexZ)
{
	return m_Cells[IndexX][IndexZ];
}

IBaseManager & CRTSGround::GetBaseManager() const
{
	return dynamic_cast<IBaseManagerHolder&>(m_Scene).GetBaseManager();
}

IRenderDevice& CRTSGround::GetRenderDevice() const
{
	return GetBaseManager().GetApplication().GetRenderDevice();
}
