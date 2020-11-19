#pragma once

#include "UIControlCommon.h"

class ZN_API CUIControlButton
	: public CUIControlCommon
{
public:
	CUIControlButton(IScene& Scene);
	virtual ~CUIControlButton();

	// CUIControl
	void Initialize() override;

	// CUIControlButton
	void SetTexture(std::shared_ptr<ITexture> Texture);

protected:
	void CreateWindowGeometry(float Width);

private:
	std::shared_ptr<CUIControlCommon> m_ButtonContent;
};