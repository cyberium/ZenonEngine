#pragma once

class CGroupVideo 
	: public CSettingsGroup
{
public:
	CGroupVideo();
	virtual ~CGroupVideo();

	// ISettingGroup
	void AddDefaultSettings() override;
};