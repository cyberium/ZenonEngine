#pragma once

class CGroupVideo 
	: public CSettingsGroupBase
{
public:
	CGroupVideo();
	virtual ~CGroupVideo();

	// ISettingGroup
	void AddDefaultSettings() override;
};