#pragma once

#include <QtWidgets/QTableView>

#include "EditorInterfaces.h"

#include "../TreeModelTemplate.h"

class C3DModelModelItem
	: public IModelCollectionItem
{
public:
	C3DModelModelItem(const std::shared_ptr<IModel>& Model)
		: m_Model(Model)
	{
	}

	std::string GetName() const override
	{
		return m_Model->GetName();
	}
	const std::vector<std::shared_ptr<IModelCollectionItem>>& GetChilds() override
	{
		return m_Childs;
	}
	std::shared_ptr<IObject> Object() const
	{
		return m_Model;
	}

private:
	std::shared_ptr<IModel> m_Model;
	std::vector<std::shared_ptr<IModelCollectionItem>> m_Childs;
};

class ZenonCollectionViewerWidget
	: public QTreeView
{
	Q_OBJECT
	Q_DISABLE_COPY(ZenonCollectionViewerWidget)
public:
	explicit ZenonCollectionViewerWidget(QWidget * parent = nullptr);
	virtual ~ZenonCollectionViewerWidget();

	void SetEditors(IEditor3DFrame* Editor3DFrame, IEditorUIFrame* EditorUIFrame) { m_Editor3D = Editor3DFrame; m_EditorUI = EditorUIFrame; }
	void SetModelsList(const std::vector<std::string>& Nodes);

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
	std::shared_ptr<QMenu> m_SceneTreeViewerContextMenu;
	bool m_LockForSelectionChangedEvent;

private:
	IEditor3DFrame* m_Editor3D;
	IEditorUIFrame* m_EditorUI;
};
