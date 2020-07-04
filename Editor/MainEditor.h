#pragma once

#include <QMainWindow>
#include "ui_MainEditor.h"

#include "EditorInterfaces.h"
#include "SceneNodesSelector.h"

#include "PropertyEditor/PropertiesController.h"

class MainEditor 
	: public QMainWindow
	, public IEditorUIFrame
	, public CSceneNodesSelector
{
	Q_OBJECT

public:
	MainEditor(QWidget* Parent = nullptr);
	virtual ~MainEditor();

	inline Ui::MainEditorClass getUI() const { return m_UI; }
	inline ZenonWindow3D * getMainEditor() const { return m_UI.MainEditor3D; }
	inline ZenonSceneViewerWidget * getSceneViewer() const { return m_UI.SceneViewer; }
	inline ZenonCollectionViewerWidget * getCollectionViewer() const { return m_UI.CollectionViewer; }
	void SetEditor3D(IEditor3DFrame* Editor3DFrame);

	// IEditorUIFrame
	void ExtendContextMenu(QMenu * Menu, const std::shared_ptr<ISceneNode3D>& Node) override;
	void OnSceneChanged();

	// CSceneNodesSelector
	void Selector_OnSelectionChange() override;

private:
	std::shared_ptr<CPropertiesController> m_PropertiesController;

private:
	IEditor3DFrame* m_Editor3D;
	Ui::MainEditorClass m_UI;
	
};
