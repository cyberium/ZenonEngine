#pragma once

class CEditorToolSelector
	: public IEditor_NodesSelector
{
public:
	CEditorToolSelector(IEditor_NodesSelectorEventListener& NodesSelectorEventListener);
	virtual ~CEditorToolSelector();

	// IEditor_NodesSelector
	void SelectNode(std::shared_ptr<ISceneNode3D> Node) override;
	void SelectNodes(const SelectedNodes& Nodes) override;
	void ClearSelection() override;
	void AddNode(std::shared_ptr<ISceneNode3D> Node) override;
	void RemoveNode(std::shared_ptr<ISceneNode3D> Node) override;
	std::shared_ptr<ISceneNode3D> GetFirstSelectedNode() override;
	bool IsNodeSelected(std::shared_ptr<ISceneNode3D> Node) override;
	const SelectedNodes& GetSelectedNodes() override;

protected:
	virtual void RaiseSelectEvent();

private:
	SelectedNodes m_SelectedNodes;
	IEditor_NodesSelectorEventListener& m_NodesSelectorEventListener;
};