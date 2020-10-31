#include "stdafx.h"

// General
#include "ZenonLogViewer.h"

ZenonLogViewer::ZenonLogViewer(QWidget * parent)
	: QPlainTextEdit(parent)
{
}

ZenonLogViewer::~ZenonLogViewer()
{
}



//
// IDebugOutput
//
void ZenonLogViewer::Print(IDebugOutput::DebugMessageType Type, const std::string& MessageFmt)
{
	//appendPlainText(MessageFmt.c_str());

	std::string color = "";
	switch (Type)
	{
		case IDebugOutput::DebugMessageType::TYPE_INFO:
			color = "gray";
			break;
		case IDebugOutput::DebugMessageType::TYPE_PRINT:
			color = "black";
			break;
		case IDebugOutput::DebugMessageType::TYPE_GREEN:
			color = "green";
			break;
		case IDebugOutput::DebugMessageType::TYPE_WARNING:
			color = "orange";
			break;
		case IDebugOutput::DebugMessageType::TYPE_ERROR:
			color = "red";
			break;
		default:
			_ASSERT(false);
	}

	std::string message = "";
	message = message.append("<font color=\"");
	message = message.append(color);
	message = message.append("\">");
	message = message.append(MessageFmt);
	message = message.append("</font>");

	appendHtml(message.c_str());
}
