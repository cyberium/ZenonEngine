#pragma once

class 
	__declspec(uuid("D2D74A10-6CC3-480E-8CE1-7A71D4372662"))
	CGroupRenderCaps : public ISettingGroup
{
public:
	bool	texFloat;
	bool	texNPOT;
	bool	rtMultisampling;
	bool	geometryShaders;
	bool	tesselation;
	bool	computeShaders;

	// ISettingGroup
	void InitDefault() override;
};