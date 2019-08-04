#pragma once

#include "DebugOutput.h"

class OW_ENGINE_API DebugOutput_Log : public CDebugOutput
{
public:
	DebugOutput_Log();
	virtual ~DebugOutput_Log();

protected:
	void Print(std::string _messageFmt, CDebugOutput::DebugMessageType _type);

private:
	std::ofstream m_LogStream;
};
