#include "stdafx.h"

// General
#include "DebugOutputEditorLog.h"

// Additional
#include <iostream>

DebugOutput_EditorLog::DebugOutput_EditorLog(QPlainTextEdit * LogTextEdit)
	: m_LogTextEdit(LogTextEdit)
{

}

DebugOutput_EditorLog::~DebugOutput_EditorLog()
{}

//

void DebugOutput_EditorLog::Print(IDebugOutput::DebugMessageType Type, const std::string& MessageFmt)
{
	if (m_LogTextEdit)
	{
		m_LogTextEdit->appendPlainText(MessageFmt.c_str());
	}
}
