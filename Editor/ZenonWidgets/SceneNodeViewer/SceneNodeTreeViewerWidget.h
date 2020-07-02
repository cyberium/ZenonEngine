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

	void RefreshTreeViewModel(const std::shared_ptr<ISceneNode3D>& SceneNode);

private slots:
	void onCustomContextMenu(const QPoint& point);
	void onCurrentChanged(const QModelIndex& current, const QModelIndex& previous);

private:
	std::shared_ptr<CSceneNodeTreeModel> m_SceneTreeViewerModel;
	std::shared_ptr<QMenu> m_SceneTreeViewerContextMenu;

private:
	IEditor3DFrame* m_Editor3D;
	IEditorUIFrame* m_EditorUI;
};
