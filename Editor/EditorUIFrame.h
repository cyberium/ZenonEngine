#pragma once

#include "EditorQtInterfaces.h"
#include "EditorResourceBrowser.h"

#include <QMainWindow>
#include "PropertyEditor/PropertiesController.h"

class CEditorUIFrame 
	: public QMainWindow
	, public IEditorUIFrame
	, public IEditorQtUIFrame
	, public IEditorToolSelectorEventListener
{
	Q_OBJECT

public:
	CEditorUIFrame(IEditor& Editor);
	virtual ~CEditorUIFrame();

	// IEditorUIFrame
	IEditor& GetEditor() const override;
	bool InitializeEditorFrame() override;
	void DoInitializeToolsUI() override;
	bool ExtendContextMenu(const std::shared_ptr<ISceneNode>& Node, std::string * Title, std::vector<std::shared_ptr<IPropertyAction>> * Actions) override;
	void OnSceneChanged(ESceneChangeType SceneChangeType, const std::shared_ptr<ISceneNode>& ParentNode, const std::shared_ptr<ISceneNode>& ChildNode) override;

	// IEditorQtUIFrame
	QObject& getQObject() override;
	HWND getHWND() override;
	Ui::EditorUIFrameClass& getUI() override;
	inline ZenonWindow3D * getMainEditor() const override { return m_UI.MainEditor3D; }
	inline ZenonWindowMinimal3DWidget * getModelPreview() const override { return m_UI.ModelPreview; }
	inline ZenonTreeViewWidget * getSceneViewer() const override { return m_UI.SceneViewer; }
	inline ZenonTreeViewWidget * getCollectionViewer() const override { return m_UI.CollectionViewer; }

	// IEditorToolSelectorEventListener
	void OnSelectNode() override;

protected:
	void OnSceneLoadFromFile();
	void OnSceneSaveToFile();
	void OnSceneClose();

private:
	CEditorResourceBrowser m_EditorResourceBrowser;
	std::shared_ptr<CPropertiesController> m_PropertiesController;
	
private:
	IEditor& m_Editor;
	Ui::EditorUIFrameClass m_UI;
};
