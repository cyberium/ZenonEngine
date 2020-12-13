#pragma once

#include "EditorQtInterfaces.h"
#include "ResourcesBrowser/Filesystem/ResourcesFilesystemIntfs.h"

class CEditorResourceBrowser
{
public:
	CEditorResourceBrowser(IEditor& Editor);
	virtual ~CEditorResourceBrowser();

	// CEditorResourceBrowser
	void Initialize();


private:
	bool OnSelectTreeItem(const IznTreeViewItem * Item);
	bool OnStartDraggingTreeItem(const IznTreeViewItem * Item, CByteBuffer * Value);

private:
	IBaseManager& GetBaseManager() const;
	IEditorUIFrame& GetEditorUIFrame() const;
	IEditorQtUIFrame& GetEditorQtUIFrame() const;

private:
	IEditor& m_Editor;
};
