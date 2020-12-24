#pragma once

#include "EditorQtInterfaces.h"

class CEditorSceneBrowser
{
public:
	CEditorSceneBrowser(IEditor& Editor);
	virtual ~CEditorSceneBrowser();

	// CEditorSceneBrowser
	void Initialize();
	void Update();

private:
	// Actions with TreeItems
	bool OnContextMenuTreeItem(const IznTreeViewItem* Item, std::shared_ptr<IPropertiesGroup> PropertiesGroup);

	// Selection
	bool OnSelectTreeItem(const IznTreeViewItem * Item);

	// Start drag
	bool OnStartDraggingTreeItem(const IznTreeViewItem * Item, CByteBuffer * ByteBuffer);
	
	// Accept drag
	bool OnDragMoveTreeItem(const IznTreeViewItem* TreeViewItem, const CByteBuffer& ByteBuffer);
	bool OnDragDropTreeItem(const IznTreeViewItem* TreeViewItem, const CByteBuffer& ByteBuffer);

private:
	IBaseManager& GetBaseManager() const;
	IEditorUIFrame& GetEditorUIFrame() const;
	IEditorQtUIFrame& GetEditorQtUIFrame() const;

private:
	IEditor& m_Editor;
	bool IsAttachedTest;
};
