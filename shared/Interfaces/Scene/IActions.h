#pragma once

// FORWARD BEGIN
ZN_INTERFACE ISceneNode;
// FORWARD END

ZN_INTERFACE ZN_API IAction
{
	virtual ~IAction() {}

	virtual std::string GetName() const = 0;
	virtual void        SetName(const std::string& Name) = 0;
	virtual std::string GetDescription() const = 0;
	virtual void        SetDescription(const std::string& Description) = 0;

	virtual void        SetAction(std::function<bool(const std::shared_ptr<ISceneNode>&)> Action) = 0;
	virtual void        ExecuteAction(const std::shared_ptr<ISceneNode>& ProcessedNode) = 0;
};

ZN_INTERFACE ZN_API IActionsGroup
{
	virtual ~IActionsGroup() {}

	virtual std::string GetName() const = 0;
	virtual void SetName(const std::string& Name) = 0;

	virtual void AddAction(std::shared_ptr<IAction> Action) = 0;
	virtual std::shared_ptr<IAction> GetAction(const std::string& ActionName) = 0;
	virtual const std::unordered_map<std::string, std::shared_ptr<IAction>>& GetActions() const = 0;
};
