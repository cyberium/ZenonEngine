#include "stdafx.h"

// General
#include "TerrainComponent.h"

// Additional
#include "Terrain.h"


CTerrainComponent::CTerrainComponent(ISceneNode& Owner)
	: CComponentBase(Owner)
{
	m_Terrain = MakeShared(CTerrain, Owner.GetBaseManager());
}

CTerrainComponent::~CTerrainComponent()
{
}



//
// ITerrainComponent
//
std::shared_ptr<ITerrain> CTerrainComponent::GetTerrain() const
{
	return m_Terrain;
}



//
// CComponentBase
//
void CTerrainComponent::Accept(IVisitor * visitor)
{
	visitor->Visit(m_Terrain);
}
