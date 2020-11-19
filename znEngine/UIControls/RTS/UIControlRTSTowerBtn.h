#pragma once

#include "../UIControlCommon.h"

class ZN_API CUIControlRTSTowerBtn
	: public CUIControlCommon
{
public:
	ZN_OBJECTCLASS(cUIControlRTSTowerBtn);
	CUIControlRTSTowerBtn(IScene& Scene);
	virtual ~CUIControlRTSTowerBtn();

	// CUIControl
	void Initialize() override;

	// CUIControlButton
	void SetTowerTexture(std::shared_ptr<ITexture> Texture);

protected:
	void CreateWindowGeometry(float Width);

private:
	std::shared_ptr<IUIControlCommon> m_ButtonContent;
};