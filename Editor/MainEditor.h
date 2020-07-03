#pragma once

#include <QMainWindow>
#include "ui_MainEditor.h"

#include "EditorInterfaces.h"

#include "PropertyEditor/PropertiesController.h"

class MainEditor 
	: public QMainWindow
	, public IEditorUIFrame
{
	Q_OBJECT

public:
	MainEditor(QWidget* Parent = nullptr);
	virtual ~MainEditor();

	inline Ui::MainEditorClass getUI() const { return m_UI; }
	inline ZenonWindow3D * getMainEditor() const { return m_UI.MainEditor3D; }
	inline SceneNodeTreeViewerWidget * getSceneTree() const { return m_UI.SceneTreeViewer; }
	inline void SetEditor3D(IEditor3DFrame* Editor3DFrame) { 
		m_Editor3D = Editor3DFrame; 
		getMainEditor()->SetEditors(Editor3DFrame, this);
		getSceneTree()->SetEditors(Editor3DFrame, this);
	}

	// IEditorUIFrame
	void ExtendContextMenu(QMenu * Menu, const std::shared_ptr<ISceneNode3D>& Node) override;
	void OnSceneChanged();
	void OnSceneNodeSelectedIn3DEditor(const std::shared_ptr<ISceneNode3D>& SceneNode3D) override;

	// IEditorSharedFrame
	void OnSceneNodeSelected(const std::shared_ptr<ISceneNode3D>& SceneNode3D) override;

private:
	std::shared_ptr<CPropertiesController> m_PropertiesController;

private:
	IEditor3DFrame* m_Editor3D;
	Ui::MainEditorClass m_UI;
	
};
