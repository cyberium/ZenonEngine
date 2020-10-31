#pragma once

#include <QtWidgets/QTreeView>

#include "../TreeModelTemplate.h"
//#include "SceneBrowser/SceneBrowserTreeModel.h"

typedef std::function<bool(const CQtToZenonTreeItem*, std::string *, std::vector<std::shared_ptr<IPropertyAction>> *)> OnContextMenuCallback;
typedef std::function<bool(const CQtToZenonTreeItem*, std::string * MimeData)> OnDragStartCallback;
typedef std::function<bool(const std::vector<const CQtToZenonTreeItem*>& Selection)> OnSelectionChangeCallback;
typedef std::function<bool(const CQtToZenonTreeItem*)> OnSelectedItemChangeCallback;

class ZenonTreeViewWidget
	: public QTreeView
{
	Q_OBJECT
	Q_DISABLE_COPY(ZenonTreeViewWidget)
public:
	explicit ZenonTreeViewWidget(QWidget * parent = nullptr);
	virtual ~ZenonTreeViewWidget();

	void SetEditor(IEditor* Editor) { m_Editor = Editor; }

	void SetRootItem(const std::shared_ptr<IModelCollectionItem>& RootItem);
	void SetRootItems(const std::vector<std::shared_ptr<IModelCollectionItem>>& RootItems);

	void ClearSelection();
	void SelectItem(const std::shared_ptr<IObject>& Item, bool NeedClear = true);
	void SelectItems(const std::vector<std::shared_ptr<IObject>>& Items);

	void SetOnContexMenu(OnContextMenuCallback Callback);
	void SetOnStartDragging(OnDragStartCallback Callback);
	void SetOnSelectionChange(OnSelectionChangeCallback Callback);
	void SetrOnSelectedItemChange(OnSelectedItemChangeCallback Callback);

protected:
	void mousePressEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;

private slots:
	void onCustomContextMenu(const QPoint& point);
	void onCurrentChanged(const QModelIndex& current, const QModelIndex& previous);
	void onSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected);
	void onPressed(const QModelIndex &index);
	void onClicked(const QModelIndex &index);
	void onDoubleClicked(const QModelIndex &index);

protected:
	void mousePressEventInternal(QMouseEvent* event);
	void mouseReleaseEventInternal(QMouseEvent* event);
	void mouseMoveEventInternal(QMouseEvent* event);

private:
	std::shared_ptr<CQtToZenonTreeModel> m_Model;
	//std::shared_ptr<CSceneBrowserTreeModel> m_Model;
	std::shared_ptr<QMenu> m_ContextMenu;
	bool m_LockForSelectionChangedEvent;
	bool m_StartDragging;
	glm::vec2 m_PrevioisMousePos;

	OnContextMenuCallback m_OnContextMenu;
	OnDragStartCallback m_OnDragStart;
	OnSelectionChangeCallback m_OnSelectionChange;
	OnSelectedItemChangeCallback m_OnSelectedItemChange;

private:
	IEditor* m_Editor;
};
