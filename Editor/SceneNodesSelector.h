#pragma once

class CSceneNodesSelector
{
public:
	typedef std::vector<std::shared_ptr<ISceneNode3D>> SelectedNodes;

public:
	CSceneNodesSelector()
		: m_SelectionBlocked(false)
		, m_OtherSelector(nullptr)
	{}

	void Selector_Block();
	void Selector_Unblock();
	void Selector_SelectNode(const std::shared_ptr<ISceneNode3D>& Node, bool NeedSync = true);
	void Selector_SelectNodes(const SelectedNodes& Nodes, bool NeedSync = true);
	void Selector_ClearSelection(bool NeedSync = true);
	void Selector_AddNode(const std::shared_ptr<ISceneNode3D>& Node, bool NeedSync = true);
	void Selector_RemoveNode(const std::shared_ptr<ISceneNode3D>& Node, bool NeedSync = true);
	std::shared_ptr<ISceneNode3D> Selector_GetFirstSelectedNode();
	bool Selector_IsNodeSelected(const std::shared_ptr<ISceneNode3D>& Node);
	const SelectedNodes& Selector_GetSelectedNodes();
	void Selector_SetOtherSelector(CSceneNodesSelector * OtherSelector);
	void Selector_SyncronizeWithOtherSelector();

	virtual void Selector_OnSelectionChange();
	
private:
	SelectedNodes m_SelectedNodes;
	bool m_SelectionBlocked;
	CSceneNodesSelector * m_OtherSelector;
};