#pragma once


ZN_INTERFACE IznSceneBrowserNode
{
	virtual Guid                                  GetId() const = 0;
	virtual std::string                           GetText() const = 0;
};


ZN_INTERFACE IznSceneBrowserBuilder
{
	virtual ObjectType GetParentType() = 0;
	virtual void       FillParent(const std::shared_ptr<IznSceneBrowserNode>& ParentNode) = 0;
};


ZN_INTERFACE IznSceneBrowser
{
	virtual std::shared_ptr<IznSceneBrowserNode> GetRoot() const = 0; // invisible root
	virtual std::shared_ptr<IznSceneBrowserNode> GetNodeById(Guid Guid) const = 0;

	virtual void                                 RefreshAll() = 0;                 
	virtual void                                 RefreshNode(Guid Guid) = 0;

	virtual void                                 AddBuilder(const std::shared_ptr<IznSceneBrowserBuilder>& Builder) = 0;
};
