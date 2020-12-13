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
	bool OnContextMenuTreeItem(const IznTreeViewItem* Item, std::string * ContextMenuTitle, std::vector<std::shared_ptr<IPropertyAction>> * ResultActions);

private:
	IBaseManager& GetBaseManager() const;
	IEditorUIFrame& GetEditorUIFrame() const;
	IEditorQtUIFrame& GetEditorQtUIFrame() const;

private:
	IEditor& m_Editor;
	bool IsAttachedTest;
};
