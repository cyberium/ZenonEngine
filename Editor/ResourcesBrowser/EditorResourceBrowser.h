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
	bool OnContextMenuTreeItem(const IznTreeViewItem* Item, std::string * ContextMenuTitle, std::vector<std::shared_ptr<IPropertyAction>> * ResultActions);

	std::shared_ptr<IParticleSystem> CreateNewParticle() const;

private:
	IBaseManager& GetBaseManager() const;
	IEditorUIFrame& GetEditorUIFrame() const;
	IEditorQtUIFrame& GetEditorQtUIFrame() const;

private:
	IEditor& m_Editor;
};
