#pragma once

#include "EditorQtInterfaces.h"

class CEditorResourceBrowser
{
public:
	CEditorResourceBrowser(IEditor& Editor);
	virtual ~CEditorResourceBrowser();

	// CEditorResourceBrowser
	void Initialize();

	std::shared_ptr<IznTreeViewItem> CreateSceneNodeProtosFromFolder(const std::string& FolderName);
	std::shared_ptr<IznTreeViewItem> CreateModelsFromFolder(const std::string& FolderName);

	void InitializeSceneBrowser();
	void UpdateSceneBrowser();

private:
	IBaseManager& GetBaseManager() const;
	IEditorUIFrame& GetEditorUIFrame() const;
	IEditorQtUIFrame& GetEditorQtUIFrame() const;

private:
	IEditor& m_Editor;
	bool IsAttachedTest;
};
