#pragma once

class CSceneNodesSelector
	: public IEditor_NodesSelector
	, public IEditor_NodesSelectorInternal
{
public:
	CSceneNodesSelector(IEditor_NodesSelectorEventListener * Listener);
	virtual ~CSceneNodesSelector();

	// IEditor_NodesSelector
	void SelectNode(std::shared_ptr<ISceneNode3D> Node) override;
	void SelectNodes(const SelectedNodes& Nodes) override;
	void ClearSelection() override;
	void AddNode(std::shared_ptr<ISceneNode3D> Node) override;
	void RemoveNode(std::shared_ptr<ISceneNode3D> Node) override;
	std::shared_ptr<ISceneNode3D> GetFirstSelectedNode() override;
	bool IsNodeSelected(std::shared_ptr<ISceneNode3D> Node) override;
	const SelectedNodes& GetSelectedNodes() override;

	// IEditor_NodesSelectorInternal
	void SetOtherSelector(IEditor_NodesSelector * OtherSelector) override;
	void SynchronizeWithOtherSelector(IEditor_NodesSelector * OtherSelector) override;

protected:
	void DoSynchronizeWithOtherSelector();

private:
	SelectedNodes m_SelectedNodes;
	IEditor_NodesSelector * m_OtherSelector;
	IEditor_NodesSelectorEventListener * m_SelectionEventsListener;
};