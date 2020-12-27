#pragma once

class CTerrainComponent
	: public ITerrainComponent
	, public CComponentBase
{
public:
	CTerrainComponent(ISceneNode& Owner);
	virtual ~CTerrainComponent();

	// ITerrainComponent
	std::shared_ptr<ITerrain> GetTerrain() const override;

	// CComponentBase
	void Accept(IVisitor* visitor) override;

private:
	std::shared_ptr<ITerrain> m_Terrain;
};