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
	bool OnSelectTreeItem(const IznTreeViewItem * Item);
	bool OnStartDraggingTreeItem(const IznTreeViewItem * Item, CByteBuffer * ByteBuffer);
	bool OnContextMenuTreeItem(const IznTreeViewItem* Item, std::shared_ptr<IPropertiesGroup> PropertiesGroup);

	bool OnDragMove(const IznTreeViewItem* TreeViewItem, const CByteBuffer& ByteBuffer);
	bool OnDragDrop(const IznTreeViewItem* TreeViewItem, const CByteBuffer& ByteBuffer);
private:
	IBaseManager& GetBaseManager() const;
	IEditorUIFrame& GetEditorUIFrame() const;
	IEditorQtUIFrame& GetEditorQtUIFrame() const;

private:
	IEditor& m_Editor;
	bool IsAttachedTest;
};
