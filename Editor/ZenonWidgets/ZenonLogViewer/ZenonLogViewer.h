#pragma once

#include <QtWidgets/qplaintextedit.h>

class ZenonLogViewer
	: public QPlainTextEdit
	, public IDebugOutput
	, public IDebugOutputInternal
{
	Q_OBJECT;
public:
	explicit ZenonLogViewer(QWidget * parent = nullptr);
	virtual ~ZenonLogViewer();

	void SetEditor(IEditor* Editor) { m_Editor = Editor; }

	// IDebugOutput
	void Print(IDebugOutput::DebugMessageType Type, const std::string& MessageFmt) override;

	// IDebugOutputInternal
	void SetLastPushedIndex(size_t Index) override;
	size_t GetLastPushedIndex() const override;

private:
	IEditor* m_Editor;
	size_t m_LastPushedMessageIndex;
};
