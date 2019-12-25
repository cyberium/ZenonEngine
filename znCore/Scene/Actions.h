#pragma once

class ZN_API CAction
	: public IAction
{
public:
	CAction();
	CAction(std::string Name, std::string Description);
	virtual ~CAction();

	// IAction
	std::string GetName() const;
	void SetName(const std::string& Name);
	std::string GetDescription() const;
	void SetDescription(const std::string& Description);

	void SetAction(std::function<bool(const std::shared_ptr<ISceneNode>&) > Action);
	void ExecuteAction(const std::shared_ptr<ISceneNode>& ProcessedNode);

private:
	std::string m_Name;
	std::string m_Description;
	std::function<bool(const std::shared_ptr<ISceneNode>&)> m_Action;
};

class ZN_API CActionsGroup
	: public IActionsGroup
{
public:
	CActionsGroup();
	CActionsGroup(std::string Name);
	virtual ~CActionsGroup();

	// IActionsGroup
	std::string GetName() const;
	void SetName(const std::string& Name);

	void AddAction(std::shared_ptr<IAction> Action);
	std::shared_ptr<IAction> GetAction(const std::string& ActionName);
	const std::unordered_map<std::string, std::shared_ptr<IAction>>& GetActions() const;

private:
	std::string m_Name;
	std::unordered_map<std::string, std::shared_ptr<IAction>> m_Actions;
};