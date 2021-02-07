#pragma once

#include "Application.h"

class ZN_API CApplicationConsole
	: public CApplication
{
public:
	CApplicationConsole();
	virtual ~CApplicationConsole();

protected:
	void DoBeforeRunImpl() override;
	void DoRunImpl(UpdateEventArgs& UpdateEventArgs) override;
	void DoAfterRunImpl() override;

	void OnStopAfterPluginManager() override;
};
