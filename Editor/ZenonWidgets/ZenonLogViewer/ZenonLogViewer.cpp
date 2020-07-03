#include "stdafx.h"

// General
#include "ZenonLogViewer.h"

ZenonLogViewer::ZenonLogViewer(QWidget * parent)
{
}

ZenonLogViewer::~ZenonLogViewer()
{
}

void ZenonLogViewer::Print(IDebugOutput::DebugMessageType Type, const std::string & MessageFmt)
{
	appendPlainText(MessageFmt.c_str());
}
