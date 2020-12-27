#pragma once

class CTerrain
	: public ITerrain
	, public Object
{
public:
	CTerrain(const IBaseManager& BaseManager);
	virtual ~CTerrain();

	// ITerrain
	const float* GetHeightsArray() const override;
	size_t GetTerrainWidth() const override;
	size_t GetTerrainHeight() const override;

	std::shared_ptr<IGeometry> GetTerrainGeometry() const override;

private:
	float* m_HeightsArray;
	size_t m_Width;
	size_t m_Height;

	const IBaseManager& m_BaseManager;
};