#pragma once

class DebugOutput_Log : public IDebugOutput
{
public:
	DebugOutput_Log();
	virtual ~DebugOutput_Log();

	// IDebugOutput
	void Print(IDebugOutput::DebugMessageType Type, const std::string& Message);

private:
	std::ofstream m_LogStream;
};
