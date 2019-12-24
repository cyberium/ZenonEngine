#pragma once

class DebugOutput_EditorLog : public IDebugOutput
{
public:
	DebugOutput_EditorLog(QPlainTextEdit * LogTextEdit);
	virtual ~DebugOutput_EditorLog();

protected:
	// CDebugOutput
	void Print(IDebugOutput::DebugMessageType Type, const std::string& MessageFmt) override;

private:
	QPlainTextEdit * m_LogTextEdit;
};
