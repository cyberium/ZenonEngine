#pragma once

#include "TreeViewModel.h"

#include <QtWidgets/QTreeView>

// Actions with TreeViewItems
typedef std::function<bool(const IznTreeViewItem*, std::shared_ptr<IPropertiesGroup>)> OnTreeViewContextMenuCallback;
typedef std::function<bool(const IznTreeViewItem*, CByteBuffer*)>                      OnTreeViewDragStartCallback;

// Selection
typedef std::function<bool(const std::vector<const IznTreeViewItem*>&)>                OnTreeViewSelectionChangeCallback;
typedef std::function<bool(const IznTreeViewItem*)>                                    OnTreeViewSelectedItemChangeCallback;

// Accept Drag
typedef std::function<bool(const CByteBuffer&)>                                        OnTreeViewDragEnterCallback;
typedef std::function<bool(const IznTreeViewItem*, const CByteBuffer&)>                OnTreeViewDragMoveCallback;
typedef std::function<bool(const IznTreeViewItem*, const CByteBuffer&)>                OnTreeViewDragDropCallback;
typedef std::function<bool()>                                                          OnTreeViewDragLeaveCallback;

class ZenonTreeViewWidget
	: public QTreeView
{
	Q_OBJECT
	Q_DISABLE_COPY(ZenonTreeViewWidget)
public:
	explicit ZenonTreeViewWidget(QWidget * parent = nullptr);
	virtual ~ZenonTreeViewWidget();

	void Refresh();
	void AddToRoot(const std::shared_ptr<IznTreeViewItem>& RootItem, bool DeleteExisting = false);
	void AddToRoot(const std::vector<std::shared_ptr<IznTreeViewItem>>& RootItems, bool DeleteExisting = false);

	void ClearSelection();
	void SelectItem(const std::shared_ptr<IObject>& Item, bool NeedClear = true);
	void SelectItems(const std::vector<std::shared_ptr<IObject>>& Items);

	// Actions with TreeViewItems
	void SetOnContexMenu(OnTreeViewContextMenuCallback Callback);
	void SetOnStartDragging(OnTreeViewDragStartCallback Callback);

	// Selection
	void SetOnSelectionChange(OnTreeViewSelectionChangeCallback Callback);
	void SetOnSelectedItemChange(OnTreeViewSelectedItemChangeCallback Callback);

	// Accept Drag
	void SetOnDragEnter(OnTreeViewDragEnterCallback Callback);
	void SetOnDragMove(OnTreeViewDragMoveCallback Callback);
	void SetOnDragDrop(OnTreeViewDragDropCallback Callback);
	void SetOnDragLeave(OnTreeViewDragLeaveCallback Callback);

protected: // QT events
	void mousePressEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;

	void dropEvent(QDropEvent *event) override;
	void dragEnterEvent(QDragEnterEvent *event) override;
	void dragMoveEvent(QDragMoveEvent *event) override;
	void dragLeaveEvent(QDragLeaveEvent *event) override;


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
	std::shared_ptr<CznTreeViewModel> m_Model;


	// Actions with TreeViewItems
	std::shared_ptr<QMenu> m_ContextMenu;
	OnTreeViewContextMenuCallback m_OnContextMenu;
	

	// Start drag
	bool m_IsDraggedNow;
	const IznTreeViewItem* m_DraggedTreeViewItem;
	glm::vec2 m_PrevioisMousePos;
	OnTreeViewDragStartCallback m_OnDragStart;


	// Selection
	bool m_LockForSelectionChangedEvent;
	OnTreeViewSelectionChangeCallback m_OnSelectionChange;
	OnTreeViewSelectedItemChangeCallback m_OnSelectedItemChange;


	// Accept drag
	OnTreeViewDragEnterCallback m_OnDragEnterCallback;
	OnTreeViewDragMoveCallback m_OnDragMoveCallback;
	OnTreeViewDragDropCallback m_OnDragDropCallback;
	OnTreeViewDragLeaveCallback m_OnDragLeaveCallback;
};
