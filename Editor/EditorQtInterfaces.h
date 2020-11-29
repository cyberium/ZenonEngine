#pragma once

#include "ui_EditorUIFrame.h"

ZN_INTERFACE IEditorQtUIFrame
{
	virtual ~IEditorQtUIFrame() {}

	virtual QObject& getQObject() = 0;
	virtual HWND getHWND() = 0;
	virtual Ui::EditorUIFrameClass& getUI() = 0;
	virtual ZenonWindow3D * getMainEditor() const = 0;
	virtual ZenonWindowMinimal3DWidget * getModelPreview() const = 0;
	virtual ZenonTreeViewWidget * getSceneViewer() const = 0;
	virtual ZenonTreeViewWidget * getCollectionViewer() const = 0;
};


ZN_INTERFACE IEditorToolUI
{
	virtual ~IEditorToolUI() {}

	virtual void DoInitializeUI(IEditorQtUIFrame& QtUIFrame) = 0;

	virtual void DropEvent(const glm::vec2& Position) = 0;
	virtual bool DragEnterEvent(const SDragData& Data) = 0;
	virtual void DragMoveEvent(const glm::vec2& Position) = 0;
	virtual void DragLeaveEvent() = 0;
};
