#pragma once

#include <QtWidgets/QTreeView>

#include "../TreeModelTemplate.h"
//#include "SceneBrowser/SceneBrowserTreeModel.h"






class ZenonSceneViewerWidget
	: public QTreeView
{
	Q_OBJECT
	Q_DISABLE_COPY(ZenonSceneViewerWidget)
public:
	explicit ZenonSceneViewerWidget(QWidget * parent = nullptr);
	virtual ~ZenonSceneViewerWidget();

	void SetEditor(IEditor* Editor);

	void RefreshTreeViewModel(ESceneChangeType SceneChangeType, const std::shared_ptr<ISceneNode3D>& ParentNode, const std::shared_ptr<ISceneNode3D>& ChildNode);
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
	std::shared_ptr<CQtToZenonTreeModel> m_Model;
	//std::shared_ptr<CSceneBrowserTreeModel> m_Model;
	std::shared_ptr<QMenu> m_ContextMenu;
	bool m_LockForSelectionChangedEvent;

private:
	IEditor* m_Editor;
};
