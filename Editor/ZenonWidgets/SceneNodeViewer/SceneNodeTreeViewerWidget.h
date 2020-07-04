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
	void SelectNodes(const std::vector<std::shared_ptr<ISceneNode3D>>& Nodes);

protected:
	virtual void mousePressEvent(QMouseEvent *event) override;

private slots:
	void onCustomContextMenu(const QPoint& point);
	void onCurrentChanged(const QModelIndex& current, const QModelIndex& previous);
	void onSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected);
	void onPressed(const QModelIndex &index);
	void onClicked(const QModelIndex &index);
	void onDoubleClicked(const QModelIndex &index);

private:
	std::shared_ptr<SceneNodeTreeModel> m_Model;
	std::shared_ptr<QMenu> m_SceneTreeViewerContextMenu;
	bool m_LockForSelectionChangedEvent;

private:
	IEditor3DFrame* m_Editor3D;
	IEditorUIFrame* m_EditorUI;
};
