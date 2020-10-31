#pragma once

#include "EngineSceneIntfs.h"

class CRTSSceneNodeGround
	: public SceneNode3D
	, public IRTSGround
{
public:


public:
	CRTSSceneNodeGround(IScene& Scene);
	virtual ~CRTSSceneNodeGround();

	// ISceneNode3D
	void CopyTo(std::shared_ptr<ISceneNode3D> Destination) const override;

	// IRTSGround
	bool AddCell(ERTSCellType CellType, SRTSCellCoords Coords) override;
	SRTSCell& GetCell(SRTSCellCoords Coords) override;
	const SRTSCell& GetCellConst(SRTSCellCoords Coords) const override;
	SRTSCellCoords PositionToCoords(const glm::vec3& Position) override;
	glm::vec3 PositionToPosition(const glm::vec3& Position) override;

	// Others
	void Update(const UpdateEventArgs& e) override;
	void Accept(IVisitor* visitor) override;

	// IObjectLoadSave
	void Load(const std::shared_ptr<IXMLReader>& Reader) override;
	void Save(const std::shared_ptr<IXMLWriter>& Writer) const override;



private: // Consts


private:
	SRTSCell*** m_Cells;
};