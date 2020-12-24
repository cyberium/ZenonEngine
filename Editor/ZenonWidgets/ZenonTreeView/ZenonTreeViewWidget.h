#pragma once

#include "TreeViewModel.h"

#include <QtWidgets/QTreeView>

typedef std::function<bool(const IznTreeViewItem*, std::shared_ptr<IPropertiesGroup>)> OnContextMenuCallback;
typedef std::function<bool(const std::vector<const IznTreeViewItem*>&)> OnSelectionChangeCallback;
typedef std::function<bool(const IznTreeViewItem*)> OnSelectedItemChangeCallback;

// Create Drag & Drop
typedef std::function<bool(const IznTreeViewItem*, CByteBuffer*)>       OnDragStartCallback;

// Accept Drag & Drop
typedef std::function<bool(const CByteBuffer&)>                         OnDragEnterCallback;
typedef std::function<bool(const IznTreeViewItem*, const CByteBuffer&)> OnDragMoveCallback;
typedef std::function<bool(const IznTreeViewItem*, const CByteBuffer&)> OnDragDropCallback;
typedef std::function<bool()>                                           OnDragLeaveCallback;

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

	void SetOnContexMenu(OnContextMenuCallback Callback);
	void SetOnSelectionChange(OnSelectionChangeCallback Callback);
	void SetOnSelectedItemChange(OnSelectedItemChangeCallback Callback);

	// Create Drag & Drop
	void SetOnStartDragging(OnDragStartCallback Callback);

	// Accept Drag & Drop
	void SetOnDragEnter(OnDragEnterCallback Callback);
	void SetOnDragMove(OnDragMoveCallback Callback);
	void SetOnDragDrop(OnDragDropCallback Callback);
	void SetOnDragLeave(OnDragLeaveCallback Callback);

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
	std::shared_ptr<QMenu> m_ContextMenu;

	bool m_LockForSelectionChangedEvent;

	bool m_IsDraggedNow;
	const IznTreeViewItem* m_DraggedTreeViewItem;
	glm::vec2 m_PrevioisMousePos;

	OnContextMenuCallback m_OnContextMenu;
	OnDragStartCallback m_OnDragStart;
	OnSelectionChangeCallback m_OnSelectionChange;
	OnSelectedItemChangeCallback m_OnSelectedItemChange;
	
	OnDragEnterCallback m_OnDragEnterCallback;
	OnDragMoveCallback m_OnDragMoveCallback;
	OnDragDropCallback m_OnDragDropCallback;
	OnDragLeaveCallback m_OnDragLeaveCallback;
};
