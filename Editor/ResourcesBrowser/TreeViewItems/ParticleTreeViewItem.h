#pragma once

#include "ZenonWidgets/ZenonTreeView/TreeViewItem.h"

class CParticleTreeViewItem
	: public CznTreeViewItem
{
public:
	CParticleTreeViewItem(const std::shared_ptr<IParticleSystem>& ParticleSystem);
	virtual ~CParticleTreeViewItem();

	// IznTreeViewItem
	ETreeViewItemType GetType() const override;
	std::string GetText() const override;
	std::string GetIconName() const override;
	std::shared_ptr<IObject> GetObject_() const override;

private:
	std::shared_ptr<IParticleSystem> m_ParticleSystem;
};