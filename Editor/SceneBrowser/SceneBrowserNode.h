#pragma once

#include "SceneBrowserInfs.h"

class CznSceneBrowserNode
	: public IznSceneBrowserNode
{
public:
	CznSceneBrowserNode(std::shared_ptr<IObject> Object);

	// IznSceneBrowserNode
	virtual Guid          GetId() const override;
	virtual std::string   GetText() const override;

private:
	std::shared_ptr<IObject> m_Object;
};