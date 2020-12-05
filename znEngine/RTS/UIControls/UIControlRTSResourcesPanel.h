#pragma once

#include "../../UIControls/UIControlCommon.h"

class ZN_API CUIControlRTSResourcesPanel
	: public CUIControlCommon
{
public:
	ZN_OBJECTCLASS(cUIControlRTSResourcesPanel);
	CUIControlRTSResourcesPanel(IScene& Scene);
	virtual ~CUIControlRTSResourcesPanel();

	void SetResourceName(std::string Name);
	void SetResourceValue(int32 ResourceValue);

	// CUIControl
	void Initialize() override;

private:
	std::string m_ResourceName;
	int32 m_ResourceValue;
	std::shared_ptr<IUIControlText> m_GoldText;
};