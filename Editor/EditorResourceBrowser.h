#pragma once

#include "EditorQtInterfaces.h"
#include "ResourcesBrowser/ResourcesFilesystemIntfs.h"

class CEditorResourceBrowser
{
public:
	CEditorResourceBrowser(IEditor& Editor);
	virtual ~CEditorResourceBrowser();

	// CEditorResourceBrowser
	void Initialize();

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
