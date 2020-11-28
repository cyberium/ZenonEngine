#pragma once

class CDebugOutputLog
	: public IDebugOutput
{
public:
	CDebugOutputLog();
	virtual ~CDebugOutputLog();

	// IDebugOutput
	void Print(IDebugOutput::DebugMessageType Type, const std::string& Message);

private:
	std::ofstream m_LogStream;
};
