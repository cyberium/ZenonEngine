#pragma once

#include "UIControlCommon.h"

class ZN_API CUIControlWindow
	: public CUIControlCommon
{
public:
	CUIControlWindow(IScene& Scene);
	virtual ~CUIControlWindow();

	// CUIControl
	void Initialize() override;

protected:
	void CreateWindowGeometry(glm::vec2 Size);
};