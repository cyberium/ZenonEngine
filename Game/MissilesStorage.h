#pragma once

class CMissilesStorage
{
public:
	struct SMissileXMLPrototype
	{
		std::string Name;

		float Speed;

		std::shared_ptr<IParticleSystem> ParticleSystem;
	};

public:
	CMissilesStorage(const IBaseManager& BaseManager);
	virtual ~CMissilesStorage();

	const std::vector<SMissileXMLPrototype>& GetTowerPrototypes() const;
	std::shared_ptr<ISceneNode> CreateTower(const std::string& TowerName, IScene& Scene);

private:
	void LoadTowerPrototypes(std::shared_ptr<IXMLReader> TowerReader);

private:
	std::vector<SMissileXMLPrototype> m_TowersPrototypes;

	const IBaseManager& m_BaseManager;
};