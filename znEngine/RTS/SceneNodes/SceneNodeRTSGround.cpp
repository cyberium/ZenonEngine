#include "stdafx.h"

// General
#include "SceneNodeRTSGround.h"

CSceneNodeRTSGround::CSceneNodeRTSGround(IScene& Scene)
	: CSceneNode(Scene)
{
	SetPersistanceInternal(true);

	for (size_t x = 0; x < cCellsCount; x++)
	{
		for (size_t z = 0; z < cCellsCount; z++)
		{
			m_Cells[x][z].Coords = SRTSCellCoords(x, z);
			AddCell(ERTSCellType::ctGround, m_Cells[x][z].Coords);
		}
	}
}

CSceneNodeRTSGround::~CSceneNodeRTSGround()
{
	/*for (size_t x = 0; x < cCellsCount; x++)
	{
		for (size_t z = 0; z < cCellsCount; z++)
		{
			delete m_Cells[x][z];
		}
		delete[] m_Cells[x];
	}
	delete[] m_Cells;*/
}



bool CSceneNodeRTSGround::AddCell(ERTSCellType CellType, SRTSCellCoords Coords)
{
	if (false == Coords.IsCorrect())
		return false;

	SRTSCell& cell = GetCell(Coords);
	//if (cell.Type == CellType)
	//	return false;

	cell.Type = CellType;

	std::shared_ptr<IModel> model = nullptr;
	/*if (CellType == ERTSCellType::ctEmpty)
	{
		cell.Model = nullptr;
		return true;
	}
	else */if (CellType == ERTSCellType::ctGround)
	{
		model = GetBaseManager().GetManager<IznModelsFactory>()->LoadModel("models/ground_dirt.znmdl");
	}
	else if (CellType == ERTSCellType::ctWater)
	{
		model = GetBaseManager().GetManager<IznModelsFactory>()->LoadModel("models/ground_dirtRiverWater.znmdl");
	}
	else
		_ASSERT(false);

	cell.Model = model;
	Log::Info("RTSSceneNodeGround: Cell '%s' added.", Coords.ToString().c_str());

	return true;
}

SRTSCell& CSceneNodeRTSGround::GetCell(SRTSCellCoords Coords)
{
	_ASSERT(Coords.IsCorrect());
	return m_Cells[Coords.GetX()][Coords.GetZ()];
}

const SRTSCell& CSceneNodeRTSGround::GetCellConst(SRTSCellCoords Coords) const
{
	_ASSERT(Coords.IsCorrect());
	return m_Cells[Coords.GetX()][Coords.GetZ()];
}

SRTSCellCoords CSceneNodeRTSGround::PositionToCoords(const glm::vec3& Position)
{
	glm::vec3 mousePosWithOffset = Position + glm::vec3(cCellSize, 0.0f, cCellSize) / 2.0f;

	glm::ivec2 newPosition = glm::ivec2(mousePosWithOffset.x, mousePosWithOffset.z);
	newPosition /= static_cast<float>(cCellSize);
	return SRTSCellCoords(newPosition.x, newPosition.y);
}

glm::vec3 CSceneNodeRTSGround::PositionToPosition(const glm::vec3& Position, float Height)
{
	glm::vec3 newPosition = glm::vec3(Position.x, Height, Position.z);
	newPosition /= static_cast<float>(cCellSize);
	newPosition = glm::round(newPosition);
	newPosition *= static_cast<float>(cCellSize);
	newPosition.y = Height;
	return newPosition;
}

void CSceneNodeRTSGround::Update(const UpdateEventArgs & e)
{
}

void CSceneNodeRTSGround::Accept(IVisitor * visitor)
{
	CSceneNode::Accept(visitor);
}



//
// IObjectLoadSave
//
void CSceneNodeRTSGround::CopyTo(std::shared_ptr<IObject> Destination) const
{
	Object::CopyTo(Destination);

	auto destCast = std::dynamic_pointer_cast<CSceneNodeRTSGround>(Destination);

	for (int32 x = 0; x < cCellsCount; x++)
	{
		for (int32 z = 0; z < cCellsCount; z++)
		{
			destCast->m_Cells[x][z] = m_Cells[x][z];
		}
	}
}

void CSceneNodeRTSGround::Load(const std::shared_ptr<IXMLReader>& Reader)
{
	Object::Load(Reader);

	// Remove all nodes from parent
	/*for (int32 x = 0; x < cCellsCount; x++)
	{
		for (int32 z = 0; z < cCellsCount; z++)
		{
			auto& cell = m_Cells[x][z];
			if (auto node = cell.Node)
			{
				if (auto parent = node->GetParent().lock())
				{
					_ASSERT(parent == shared_from_this());
					parent->RemoveChild(node);
				}
			}
		}
	}*/

	if (auto childsWriter = Reader->GetChild("RTSCells"))
	{
		for (const auto& ch : childsWriter->GetChilds())
		{
			int32 x = ch->GetIntAttribute("X");
			int32 z = ch->GetIntAttribute("Z");
			ERTSCellType type = (ERTSCellType)ch->GetUIntAttribute("Type");

			SRTSCellCoords coords = SRTSCellCoords(x, z);
			_ASSERT(coords.IsCorrect());

			AddCell(type, coords);
		}
	}
}

void CSceneNodeRTSGround::Save(const std::shared_ptr<IXMLWriter>& Writer) const
{
	Object::Save(Writer);

	auto rtsCellsWriter = Writer->CreateChild("RTSCells");
	for (int32 x = 0; x < cCellsCount; x++)
	{
		for (int32 z = 0; z < cCellsCount; z++)
		{
			CXMLManager xmlManager(GetBaseManager());
			auto cellWriter = xmlManager.CreateWriter("RTSCell");
			cellWriter->SetIntAttribute(x, "X");
			cellWriter->SetIntAttribute(z, "Z");

			cellWriter->SetUIntAttribute((int8)m_Cells[x][z].Type, "Type");

			rtsCellsWriter->AddChild(cellWriter);
		}
	}
}

void CSceneNodeRTSGround::DoProcessNodesNear(SRTSCellCoords Coords)
{
	for (int32 x = -1; x <= 1; x++)
	{
		for (int32 z = -1; z <= 1; z++)
		{
			SRTSCellCoords coords(x, z);
			if (false == coords.IsCorrect())
				continue;
		}
	}
}
