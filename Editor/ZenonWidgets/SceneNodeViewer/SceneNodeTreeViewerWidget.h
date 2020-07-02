#pragma once

#include <QtWidgets/QTreeView>

#include "EditorInterfaces.h"
#include "SceneNodeTreeModel.h"

class SceneNodeTreeViewerWidget
	: public QTreeView
{
	Q_OBJECT
	Q_DISABLE_COPY(SceneNodeTreeViewerWidget)
public:
	explicit SceneNodeTreeViewerWidget(QWidget * parent = nullptr);
	virtual ~SceneNodeTreeViewerWidget();

	void SetEditors(IEditor3DFrame* Editor3DFrame, IEditorUIFrame* EditorUIFrame) { m_Editor3D = Editor3DFrame; m_EditorUI = EditorUIFrame; }

	void RefreshTreeViewModel();
	void SelectNode(const std::shared_ptr<ISceneNode3D>& Node);

private slots:
	void onCustomContextMenu(const QPoint& point);
	void onCurrentChanged(const QModelIndex& current, const QModelIndex& previous);

private:
	std::shared_ptr<SceneNodeTreeModel> m_SceneTreeViewerModel;
	std::shared_ptr<QMenu> m_SceneTreeViewerContextMenu;
	bool m_SelectionBlocked;

private:
	IEditor3DFrame* m_Editor3D;
	IEditorUIFrame* m_EditorUI;
};
