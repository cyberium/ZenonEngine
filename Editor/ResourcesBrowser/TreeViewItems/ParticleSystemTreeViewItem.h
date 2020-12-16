#pragma once

#include "ZenonWidgets/ZenonTreeView/TreeViewItem.h"

class CParticleSystemTreeViewItem
	: public CznTreeViewItem
{
public:
	CParticleSystemTreeViewItem(const std::shared_ptr<IParticleSystem>& ParticleSystem);
	virtual ~CParticleSystemTreeViewItem();

	// IznTreeViewItem
	ETreeViewItemType GetType() const override;
	std::string GetText() const override;
	std::string GetIconName() const override;
	std::shared_ptr<IObject> GetObject_() const override;

private:
	std::shared_ptr<IParticleSystem> m_ParticleSystem;
};