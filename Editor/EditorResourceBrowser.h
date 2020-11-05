#pragma once

#include "EditorQtInterfaces.h"

class CEditorResourceBrowser
{
public:
	CEditorResourceBrowser(IEditor& Editor);
	virtual ~CEditorResourceBrowser();

	// CEditorResourceBrowser
	void Initialize();

	std::shared_ptr<IznTreeViewItemSource> CreateSceneNodeProtosFromFolder(const std::string& FolderName);
	std::shared_ptr<IznTreeViewItemSource> CreateModelsFromFolder(const std::string& FolderName);

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
