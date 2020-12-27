#pragma once

class CTerrain
	: public ITerrain
	, public Object
{
public:
	CTerrain(const IBaseManager& BaseManager);
	virtual ~CTerrain();

	// ITerrain
	std::shared_ptr<IGeometry> GetTerrainGeometry() const override;

private:
	std::shared_ptr<IGeometry> m_TerrainGeometry;

	const IBaseManager& m_BaseManager;
};