#include "stdafx.h"

// General
#include "ParticleSystemTreeViewItem.h"

CParticleSystemTreeViewItem::CParticleSystemTreeViewItem(const std::shared_ptr<IParticleSystem>& ParticleSystem)
	: m_ParticleSystem(ParticleSystem)
{
}

CParticleSystemTreeViewItem::~CParticleSystemTreeViewItem()
{
}



//
// IznTreeViewItem
//
ETreeViewItemType CParticleSystemTreeViewItem::GetType() const
{
	return ETreeViewItemType::ParticleSystem;
}

std::string CParticleSystemTreeViewItem::GetText() const
{
	return Utils::SplitFilename(m_ParticleSystem->GetName()).NameWithoutExtension;
}

std::string CParticleSystemTreeViewItem::GetIconName() const
{
	return "node";
}

std::shared_ptr<IObject> CParticleSystemTreeViewItem::GetObject_() const
{
	return m_ParticleSystem;
}
