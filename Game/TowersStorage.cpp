#include "stdafx.h"

// General
#include "TowersStorage.h"

CTowersStorage::CTowersStorage(const IBaseManager& BaseManager)
	: m_BaseManager(BaseManager)
{
	CXMLManager xml(m_BaseManager);
	auto xmlTowersReader = xml.CreateReaderFromFile("towers.xml");

	for (const auto& xmlTower : xmlTowersReader->GetChilds())
	{
		if (xmlTower->GetName() != "Tower")
			throw CException("Unknown tower definition xml name '%s'.", xmlTower->GetName().c_str());

		LoadTowerPrototypes(xmlTower);
	}
}

CTowersStorage::~CTowersStorage()
{
}



//
// CTowersStorage
//
const std::vector<CTowersStorage::STowerXMLPrototype>& CTowersStorage::GetTowerPrototypes() const
{
	return m_TowersPrototypes;
}

std::shared_ptr<ISceneNode> CTowersStorage::CreateTower(const std::string& TowerName, IScene& Scene)
{
	const auto& towersIt = std::find_if(m_TowersPrototypes.begin(), m_TowersPrototypes.end(), [TowerName](const STowerXMLPrototype& TowerPrototype) {
		return TowerPrototype.TowerName == TowerName;
	});
	if (towersIt == m_TowersPrototypes.end())
		throw CException("Tower definition for '%s' not found.", TowerName.c_str());

	CXMLManager xmlManager(m_BaseManager);
	auto reader = xmlManager.CreateReaderFromFile(towersIt->TowerObject);
	_ASSERT(false == reader->GetChilds().empty());
	auto firstXMLChild = reader->GetChilds()[0];

	std::shared_ptr<ISceneNode> sceneNode = m_BaseManager.GetManager<IObjectsFactory>()->GetClassFactoryCast<CSceneNodeFactory>()->LoadSceneNode3DXML(firstXMLChild, Scene);
	sceneNode->MakeMeOrphan();

	std::shared_ptr<ISceneNodeRTSTower> towerNode = std::dynamic_pointer_cast<ISceneNodeRTSTower>(sceneNode);

	towerNode->SetAttackDamageMin(towersIt->AttackDamageMin);
	towerNode->SetAttackDamageMax(towersIt->AttackDamageMax);
	towerNode->SetAttackSpeed(towersIt->AttackSpeed);
	towerNode->SetAttackRange(towersIt->AttackRange);

	return sceneNode;
}



//
// Private
//
void CTowersStorage::LoadTowerPrototypes(std::shared_ptr<IXMLReader> TowerReader)
{
	STowerXMLPrototype towerPrototype;

	towerPrototype.TowerName       = TowerReader->GetChild("TowerName")->GetValue();
	towerPrototype.TowerIcon       = TowerReader->GetChild("TowerIcon")->GetValue();
	towerPrototype.TowerObject     = TowerReader->GetChild("TowerObject")->GetValue();
	towerPrototype.TowerCost       = TowerReader->GetChild("TowerCost")->GetInt();

	// Stats
	towerPrototype.AttackDamageMin = TowerReader->GetChild("AttackDamageMin")->GetInt();
	towerPrototype.AttackDamageMax = TowerReader->GetChild("AttackDamageMax")->GetInt();
	towerPrototype.AttackSpeed     = TowerReader->GetChild("AttackSpeed")->GetFloat();
	towerPrototype.AttackRange     = TowerReader->GetChild("AttackRange")->GetInt();

	// Missile
	towerPrototype.MissileSpeed    = TowerReader->GetChild("MissileSpeed")->GetFloat();

	const auto& towersIt = std::find_if(m_TowersPrototypes.begin(), m_TowersPrototypes.end(), [&towerPrototype](const STowerXMLPrototype& TowerPrototype) {
		return TowerPrototype.TowerName == towerPrototype.TowerName;
	});
	if (towersIt != m_TowersPrototypes.end())
		throw CException("Tower definition for '%s' already exists.", towersIt->TowerName.c_str());

	m_TowersPrototypes.push_back(towerPrototype);
}
