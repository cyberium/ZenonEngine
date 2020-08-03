#pragma once

#include <QMainWindow>
#include "ui_MainEditor.h"

#include "Tools/EditorToolSelectorBase.h"
#include "PropertyEditor/PropertiesController.h"

class MainEditor 
	: public QMainWindow
	, public IEditorUIFrame
	, public IEditor_NodesSelectorEventListener
{
	Q_OBJECT

public:
	MainEditor(QWidget* Parent = nullptr);
	virtual ~MainEditor();

	inline Ui::MainEditorClass getUI() const { return m_UI; }
	inline ZenonWindow3D * getMainEditor() const { return m_UI.MainEditor3D; }
	inline ZenonWindowMinimal3DWidget * getModelPreview() const { return m_UI.ModelPreview; }
	inline ZenonSceneViewerWidget * getSceneViewer() const { return m_UI.SceneViewer; }
	inline ZenonCollectionViewerWidget * getCollectionViewer() const { return m_UI.CollectionViewer; }
	void SetEditor3D(IEditor3DFrame* Editor3DFrame);

	// IEditorSharedFrame
	IEditor_NodesSelector* GetNodesSelector();

	// IEditorUIFrame
	bool ExtendContextMenu(const std::shared_ptr<ISceneNode3D>& Node, std::string * Title, std::vector<std::shared_ptr<IPropertyAction>> * Actions) override;
	void OnSceneChanged();

	// IEditor_NodesSelectorEventListener
	void OnSelectNodes() override;

private slots:
	void MoverStepCurrentIndexChanged(const QString &);
	void OnActionSceneLoad();
	void OnActionSceneSave();

	void OnEditorToolSelectorBtn();
	void OnEditorToolMoverBtn();
	void OnEditorToolRotatorBtn();

private:
	CEditorToolSelector m_Selector;
	std::shared_ptr<CPropertiesController> m_PropertiesController;
	std::map<std::string, float> m_MoverValues;

private:
	IEditor3DFrame* m_Editor3D;
	Ui::MainEditorClass m_UI;
	
};
