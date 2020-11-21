#pragma once

#include "../../UIControls/UIControlCommon.h"
#include "UIControlRTSTowerBtn.h"

class ZN_API CUIControlRTSTowersPanel
	: public CUIControlCommon
{
public:
	ZN_OBJECTCLASS(cUIControlRTSTowersPanel);
	CUIControlRTSTowersPanel(IScene& Scene);
	virtual ~CUIControlRTSTowersPanel();

	// CUIControl
	void Initialize() override;


	void AddTowerButton(std::shared_ptr<ITexture> RTSTowerBtnTexture);
	const std::vector<std::shared_ptr<CUIControlRTSTowerBtn>>& GetTowerButtons();

protected:
	void CreateWindowGeometry(glm::vec2 Size);

private:
	std::vector<std::shared_ptr<CUIControlRTSTowerBtn>> m_TowerButtons;
};