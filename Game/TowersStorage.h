#pragma once

class CTowersStorage
{
public:
	struct STowerXMLPrototype
	{
		std::string TowerName;
		std::string TowerIcon;
		std::string TowerObject;
		int32       TowerCost;

		// Stats
		int32 AttackDamageMin;
		int32 AttackDamageMax;
		float AttackSpeed;
		uint32 AttackRange;

		// Missile
		float MissileSpeed;
	};

public:
	CTowersStorage(const IBaseManager& BaseManager);
	virtual ~CTowersStorage();

	const std::vector<STowerXMLPrototype>& GetTowerPrototypes() const;
	std::shared_ptr<ISceneNode> CreateTower(const std::string& TowerName, IScene& Scene);

private:
	void LoadTowerPrototypes(std::shared_ptr<IXMLReader> TowerReader);

private:
	std::vector<STowerXMLPrototype> m_TowersPrototypes;

	const IBaseManager& m_BaseManager;
};