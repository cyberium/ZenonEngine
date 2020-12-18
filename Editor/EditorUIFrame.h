#pragma once

#include "EditorQtInterfaces.h"

#include "SceneBrowser/EditorSceneBrowser.h"
#include "ResourcesBrowser/EditorResourceBrowser.h"

#include <QMainWindow>

class CEditorUIFrame 
	: public QMainWindow
	, public IEditorUIFrame
	, public IEditorQtUIFrame
	, public IEditorToolSelectorEventListener
	, public ISceneEventsListener
{
	Q_OBJECT

public:
	CEditorUIFrame(IEditor& Editor);
	virtual ~CEditorUIFrame();

	// IEditorUIFrame
	IEditor& GetEditor() const override;
	bool InitializeEditorFrame() override;
	void DoInitializeToolsUI() override;
	bool ExtendContextMenu(const std::shared_ptr<ISceneNode>& Node, std::shared_ptr<IPropertiesGroup> PropertiesGroup) override;

	// IEditorQtUIFrame
	QObject& getQObject() override;
	HWND getHWND() override;
	Ui::EditorUIFrameClass& getUI() override;
	inline ZenonWindow3D * getMainEditor() const override { return m_UI.MainEditor3D; }
	
	inline ZenonTreeViewWidget * getSceneViewer() const override { return m_UI.SceneViewer; }

	inline ZenonTreeViewWidget * getResourcesBrowser() const override { return m_UI.ResourcesBrowserTreeView; }
	inline ZenonWindowMinimal3DWidget * getResourcePreview() const override { return m_UI.ResourcePreviewWindow3D; }

	// IEditorToolSelectorEventListener
	void OnSelectNode() override;

	// ISceneEventsListener
	void OnSceneNodeAdded(std::shared_ptr<ISceneNode> ParentNode, std::shared_ptr<ISceneNode> ChildNode) override;
	void OnSceneNodeRemoved(std::shared_ptr<ISceneNode> ParentNode, std::shared_ptr<ISceneNode> ChildNode) override;

protected:
	void OnSceneLoadFromFile();
	void OnSceneSaveToFile();
	void OnSceneClose();

private:
	CEditorSceneBrowser m_EditorSceneBrowser;
	CEditorResourceBrowser m_EditorResourceBrowser;
	//std::shared_ptr<CPropertiesController> m_PropertiesController;
	
private:
	IEditor& m_Editor;
	Ui::EditorUIFrameClass m_UI;
};
