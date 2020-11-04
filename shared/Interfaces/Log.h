#pragma once

// Forward BEGIN
struct IManager;
struct IDebugOutput;
// Forward END

ZN_INTERFACE ZN_API ILog 
	: public IManager
{
	ZN_OBJECTCLASS(cLog)

	virtual ~ILog() {}

	virtual bool AddDebugOutput(std::shared_ptr<IDebugOutput> DebugOutput) = 0;
	virtual bool DeleteDebugOutput(std::shared_ptr<IDebugOutput> DebugOutput) = 0;
};