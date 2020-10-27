#pragma once

#include <QtWidgets/QTreeView>

#include "../TreeModelTemplate.h"

class CSceneNodeModelItem
	: public IModelCollectionItem
{
public:
	CSceneNodeModelItem(const std::shared_ptr<ISceneNode3D>& SceneNode)
		: m_SceneNode(SceneNode)
	{
		for (const auto& it : SceneNode->GetChilds())
			m_Childs.push_back(MakeShared(CSceneNodeModelItem, it));
	}

	std::string GetName() const override
	{
		return m_SceneNode->GetName();
	}
	const std::vector<std::shared_ptr<IModelCollectionItem>>& GetChilds() override
	{
		return m_Childs;
	}
	std::shared_ptr<IObject> Object() const
	{
		return m_SceneNode;
	}

private:
	std::shared_ptr<ISceneNode3D> m_SceneNode;
	std::vector<std::shared_ptr<IModelCollectionItem>> m_Childs;
};




class ZenonSceneViewerWidget
	: public QTreeView
{
	Q_OBJECT
	Q_DISABLE_COPY(ZenonSceneViewerWidget)
public:
	explicit ZenonSceneViewerWidget(QWidget * parent = nullptr);
	virtual ~ZenonSceneViewerWidget();

	void SetEditor(IEditor* Editor) { m_Editor = Editor; }

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
	std::shared_ptr<CQtToZenonTreeModel> m_Model;
	std::shared_ptr<QMenu> m_ContextMenu;
	bool m_LockForSelectionChangedEvent;

private:
	IEditor* m_Editor;
};
