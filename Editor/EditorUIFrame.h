#pragma once

#include <QMainWindow>

#include "EditorQtInterfaces.h"

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

	inline ZenonWindow3D * getMainEditor() const { return m_UI.MainEditor3D; }
	inline ZenonWindowMinimal3DWidget * getModelPreview() const { return m_UI.ModelPreview; }
	inline ZenonSceneViewerWidget * getSceneViewer() const { return m_UI.SceneViewer; }
	inline ZenonCollectionViewerWidget * getCollectionViewer() const { return m_UI.CollectionViewer; }

	// IEditorSharedFrame
	IEditor& GetEditor() const override;
	bool InitializeEditorFrame() override;

	// IEditorUIFrame
	void DoInitializeToolsUI() override;
	bool ExtendContextMenu(const std::shared_ptr<ISceneNode3D>& Node, std::string * Title, std::vector<std::shared_ptr<IPropertyAction>> * Actions) override;
	void OnSceneChanged(ESceneChangeType SceneChangeType, const std::shared_ptr<ISceneNode3D>& ParentNode, const std::shared_ptr<ISceneNode3D>& ChildNode) override;

	// IEditorQtUIFrame
	QObject& getQObject() override final;
	HWND getHWND() override;
	Ui::EditorUIFrameClass& getUI() override final;

	// IEditorToolSelectorEventListener
	void OnSelectNode() override;

private:
	std::shared_ptr<CPropertiesController> m_PropertiesController;
	
private:
	IEditor& m_Editor;
	Ui::EditorUIFrameClass m_UI;
};
