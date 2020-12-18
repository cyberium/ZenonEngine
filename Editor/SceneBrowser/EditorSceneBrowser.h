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
	bool OnStartDraggingTreeItem(const IznTreeViewItem * Item, CByteBuffer * Value);
	bool OnContextMenuTreeItem(const IznTreeViewItem* Item, std::shared_ptr<IPropertiesGroup> PropertiesGroup);

private:
	IBaseManager& GetBaseManager() const;
	IEditorUIFrame& GetEditorUIFrame() const;
	IEditorQtUIFrame& GetEditorQtUIFrame() const;

private:
	IEditor& m_Editor;
	bool IsAttachedTest;
};
