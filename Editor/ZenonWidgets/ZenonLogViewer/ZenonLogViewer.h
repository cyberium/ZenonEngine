#pragma once

#include <QtWidgets/qplaintextedit.h>

class ZenonLogViewer
	: public QPlainTextEdit
	, public IDebugOutput
{
	Q_OBJECT;
public:
	explicit ZenonLogViewer(QWidget * parent = nullptr);
	virtual ~ZenonLogViewer();

	void SetEditor(IEditor* Editor) { m_Editor = Editor; }

	// IDebugOutput
	void Print(IDebugOutput::DebugMessageType Type, const std::string& MessageFmt) override;

private:
	IEditor* m_Editor;
};
