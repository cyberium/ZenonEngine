#pragma once

#include "TreeViewModel.h"
//#include "SceneBrowser/SceneBrowserTreeModel.h"

#include <QtWidgets/QTreeView>

typedef std::function<bool(const CznTreeViewItem*, std::string *, std::vector<std::shared_ptr<IPropertyAction>> *)> OnContextMenuCallback;
typedef std::function<bool(const CznTreeViewItem*, CByteBuffer * Bytes)> OnDragStartCallback;
typedef std::function<bool(const std::vector<const CznTreeViewItem*>& Selection)> OnSelectionChangeCallback;
typedef std::function<bool(const CznTreeViewItem*)> OnSelectedItemChangeCallback;

class ZenonTreeViewWidget
	: public QTreeView
{
	Q_OBJECT
	Q_DISABLE_COPY(ZenonTreeViewWidget)
public:
	explicit ZenonTreeViewWidget(QWidget * parent = nullptr);
	virtual ~ZenonTreeViewWidget();

	void Refresh();
	void AddToRoot(const std::shared_ptr<IznTreeViewItemSource>& RootItem, bool DeleteExisting = false);
	void AddToRoot(const std::vector<std::shared_ptr<IznTreeViewItemSource>>& RootItems, bool DeleteExisting = false);
	//void SetRootItems(const std::vector<std::shared_ptr<IznTreeViewItemSource>>& RootItems);

	void ClearSelection();
	void SelectItem(const std::shared_ptr<IObject>& Item, bool NeedClear = true);
	void SelectItems(const std::vector<std::shared_ptr<IObject>>& Items);

	void SetOnContexMenu(OnContextMenuCallback Callback);
	void SetOnStartDragging(OnDragStartCallback Callback);
	void SetOnSelectionChange(OnSelectionChangeCallback Callback);
	void SetOnSelectedItemChange(OnSelectedItemChangeCallback Callback);

protected:
	void mousePressEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;

private slots:
	void onCustomContextMenu(const QPoint& point);
	void onCurrentChanged(const QModelIndex& current, const QModelIndex& previous);
	void onSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected);
	void onPressed(const QModelIndex &index); // This signal is emitted when a mouse button is pressed. The item the mouse was pressed on is specified by index. The signal is only emitted when the index is valid.
	void onClicked(const QModelIndex &index); // This signal is emitted when a mouse button is clicked. The item the mouse was clicked on is specified by index. The signal is only emitted when the index is valid.
	void onDoubleClicked(const QModelIndex &index);

protected:
	void mousePressEventInternal(QMouseEvent* event);
	void mouseReleaseEventInternal(QMouseEvent* event);
	void mouseMoveEventInternal(QMouseEvent* event);

private:
	std::shared_ptr<CznQTTreeViewModel> m_Model;
	//std::shared_ptr<CSceneBrowserTreeModel> m_Model;
	std::shared_ptr<QMenu> m_ContextMenu;
	bool m_LockForSelectionChangedEvent;
	bool m_StartDragging;
	glm::vec2 m_PrevioisMousePos;

	OnContextMenuCallback m_OnContextMenu;
	OnDragStartCallback m_OnDragStart;
	OnSelectionChangeCallback m_OnSelectionChange;
	OnSelectedItemChangeCallback m_OnSelectedItemChange;
};
