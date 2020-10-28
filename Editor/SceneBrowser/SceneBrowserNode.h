#pragma once

#include "SceneBrowserInfs.h"

class CznSceneBrowserNode
	: public IznSceneBrowserNode
{
public:

	// IznSceneBrowserNode
	virtual Guid                                  GetId() const override;
	virtual std::string                           GetText() const override;

private:
	std::weak_ptr<IznSceneBrowserNode>                 m_Parent;
    std::vector<std::shared_ptr<IznSceneBrowserNode>>  m_Childs;
};