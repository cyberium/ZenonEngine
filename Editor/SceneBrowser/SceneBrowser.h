#pragma once

#include "SceneBrowserInfs.h"
#include "SceneBrowserTreeItem.h"

class CSceneBrowser
	: public IznSceneBrowser
{
public:

	// IznSceneBrowser
	std::shared_ptr<IznSceneBrowserNode> GetRoot() const override; // invisible root
	std::shared_ptr<IznSceneBrowserNode> GetNodeById(Guid Guid) const override;
	void                                 RefreshAll() override;
	void                                 RefreshNode(Guid Guid) override;
	void                                 AddBuilder(const std::shared_ptr<IznSceneBrowserBuilder>& Builder) override;

private:
	std::shared_ptr<CSceneBrowserTreeItem> m_TreeRoot;
};