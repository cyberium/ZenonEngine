#include "stdafx.h"

// General
#include "ParticleTreeViewItem.h"

CParticleTreeViewItem::CParticleTreeViewItem(const std::shared_ptr<IParticleSystem>& ParticleSystem)
	: m_ParticleSystem(ParticleSystem)
{
}

CParticleTreeViewItem::~CParticleTreeViewItem()
{
}



//
// IznTreeViewItem
//
ETreeViewItemType CParticleTreeViewItem::GetType() const
{
	return ETreeViewItemType::ParticleSystem;
}

std::string CParticleTreeViewItem::GetText() const
{
	return Utils::SplitFilename(m_ParticleSystem->GetName()).NameWithoutExtension;
}

std::string CParticleTreeViewItem::GetIconName() const
{
	return "node";
}

std::shared_ptr<IObject> CParticleTreeViewItem::GetObject_() const
{
	return m_ParticleSystem;
}
