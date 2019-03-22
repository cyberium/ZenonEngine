#pragma once

// Forward BEGIN
struct IManager;
struct IDebugOutput;
// Forward END

struct 
	__declspec(novtable, uuid("114337A3-0FD0-4AB4-9CBC-40B6FE4E1796"))
	ILog : IManager
{
	virtual ~ILog() {}

	virtual bool AddDebugOutput(std::shared_ptr<IDebugOutput> _debugOutput) = 0;
	virtual bool DeleteDebugOutput(std::shared_ptr<IDebugOutput> _debugOutput) = 0;
};