#include "stdafx.h"

// General
#include "ZenonCollectionViewerWidget.h"

ZenonCollectionViewerWidget::ZenonCollectionViewerWidget(QWidget * parent)
	: QTreeView(parent)
	, m_Editor(nullptr)
	, m_LockForSelectionChangedEvent(false)
	, m_StartDragging(false)
{
	// Add context menu for scene node viewer
	m_SceneTreeViewerContextMenu = MakeShared(QMenu, this);
	m_SceneTreeViewerContextMenu->setTitle("Some context menu title.");

	this->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(this, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(onCustomContextMenu(const QPoint &)));

	// SceneNodeTreeView: Main settings
	m_Model = MakeShared(CQtToZenonTreeModel, this);
	this->setModel(m_Model.get());

	// SceneNodeTreeView: Selection settings
	this->setSelectionMode(QAbstractItemView::SingleSelection);
	this->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectItems);

	QItemSelectionModel* selectionModel = this->selectionModel();
	connect(selectionModel, SIGNAL(currentChanged(const QModelIndex&, const QModelIndex&)), this, SLOT(onCurrentChanged(const QModelIndex&, const QModelIndex&)));
	connect(selectionModel, SIGNAL(selectionChanged(const QItemSelection&, const QItemSelection&)), this, SLOT(onSelectionChanged(const QItemSelection&, const QItemSelection&)));

	connect(this, SIGNAL(clicked(const QModelIndex&)), this, SLOT(onClicked(const QModelIndex&)));
}

ZenonCollectionViewerWidget::~ZenonCollectionViewerWidget()
{
	Log::Info("Test");
}

void ZenonCollectionViewerWidget::SetModelsList(const std::vector<std::string>& Nodes)
{
	std::vector<std::shared_ptr<IModelCollectionItem>> models;
	for (const auto& it : Nodes)
	{
		auto name = CFile(it).Name();
		name = name.substr(0, name.find_first_of('.'));

		auto model = m_Editor->GetRenderDevice().GetObjectsFactory().CreateModel();
		if (auto loadable = std::dynamic_pointer_cast<IObjectLoadSave>(model))
			loadable->Load(m_Editor->GetBaseManager().GetManager<IFilesManager>()->Open(it));
		model->SetName(name);
		models.push_back(MakeShared(C3DModelModelItem, model));
	}

	this->reset();
	m_Model->SetChildRootItemsData(models);
	this->expandAll();
}

void ZenonCollectionViewerWidget::mousePressEvent(QMouseEvent * event)
{
	if (m_LockForSelectionChangedEvent)
		return;

	if (event->button() == Qt::RightButton)
		return;

	m_StartDragging = false;
	m_PrevioisMousePos = glm::ivec2(event->pos().x(), event->pos().y());

	__super::mousePressEvent(event);
}

void ZenonCollectionViewerWidget::mouseReleaseEvent(QMouseEvent * event)
{
	m_StartDragging = false;
	__super::mouseReleaseEvent(event);
}

void ZenonCollectionViewerWidget::mouseMoveEvent(QMouseEvent * event)
{
	auto currentPos = glm::vec2(event->pos().x(), event->pos().y());
	if (!m_StartDragging && glm::length(glm::abs(m_PrevioisMousePos - currentPos)) > 5.0f)
	{
		QModelIndex index = indexAt(event->pos());
		if (!index.isValid())
			return;

		auto item = static_cast<CQtToZenonTreeItem*>(index.internalPointer());
		if (item == nullptr)
			return;

		m_StartDragging = true;

		QMimeData *mimeData = DEBUG_NEW QMimeData;
		mimeData->setText(item->GetTObject()->GetName().c_str());

		m_Editor->GetTools().Enable(ETool::EToolDragger);

		auto drag = DEBUG_NEW QDrag(this);
		drag->setMimeData(mimeData);
		drag->setHotSpot(event->pos());
		Qt::DropAction dropAction = drag->exec();
		if (dropAction != Qt::DropAction::IgnoreAction)
		{
			if (dropAction == Qt::DropAction::CopyAction)
				drag->cancel();
		}
	}

	__super::mouseMoveEvent(event);
}


//
// Slots
//
void ZenonCollectionViewerWidget::onCustomContextMenu(const QPoint& point)
{
	if (m_LockForSelectionChangedEvent)
		return;

	QModelIndex index = indexAt(point);
	if (!index.isValid())
		return;

	auto item = static_cast<CQtToZenonTreeItem*>(index.internalPointer());
	_ASSERT_EXPR(item != nullptr, L"Item is null.");

	m_SceneTreeViewerContextMenu->clear();
	//m_EditorUI->ExtendContextMenu(m_SceneTreeViewerContextMenu.get(), item->GetTObject());
	m_SceneTreeViewerContextMenu->popup(mapToGlobal(point));
}

void ZenonCollectionViewerWidget::onCurrentChanged(const QModelIndex& current, const QModelIndex& previous)
{
	if (!current.isValid())
		return;

	auto item = static_cast<CQtToZenonTreeItem*>(current.internalPointer());
	_ASSERT_EXPR(item != nullptr, L"Item is null.");

	m_Editor->Get3DFrame().OnCollectionWidget_ModelSelected(std::dynamic_pointer_cast<IModel>(item->GetTObject()));
}

void ZenonCollectionViewerWidget::onSelectionChanged(const QItemSelection& selected, const QItemSelection &deselected)
{
	if (m_LockForSelectionChangedEvent)
		return;

}

void ZenonCollectionViewerWidget::onPressed(const QModelIndex & index)
{

}

void ZenonCollectionViewerWidget::onClicked(const QModelIndex & index)
{

}

void ZenonCollectionViewerWidget::onDoubleClicked(const QModelIndex & index)
{

}
