#pragma once

class CSceneNodeRTSGround
	: public CSceneNode
	, public ISceneNodeRTSGround
{
public:
	CSceneNodeRTSGround(IScene& Scene);
	virtual ~CSceneNodeRTSGround();

	// ISceneNodeRTSGround
	bool AddCell(ERTSCellType CellType, SRTSCellCoords Coords) override;
	SRTSCell& GetCell(SRTSCellCoords Coords) override;
	const SRTSCell& GetCellConst(SRTSCellCoords Coords) const override;
	SRTSCellCoords PositionToCoords(const glm::vec3& Position) override;
	glm::vec3 PositionToPosition(const glm::vec3& Position, float Height = 0.0f) override;

	// Others
	void Update(const UpdateEventArgs& e) override;
	void Accept(IVisitor* visitor) override;

	// IObjectLoadSave
	void CopyTo(std::shared_ptr<IObject> Destination) const override;
	void Load(const std::shared_ptr<IXMLReader>& Reader) override;
	void Save(const std::shared_ptr<IXMLWriter>& Writer) const override;

private:
	void DoProcessNodesNear(SRTSCellCoords Coords);
	
private:
	SRTSCell m_Cells[cCellsCount][cCellsCount];
};