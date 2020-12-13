#pragma once

#include "../../UIControls/UIControlCommon.h"
#include "UIControlRTSTowerBtn.h"

struct STowerDescription
{
	std::string                            TowerName;
	std::string                            TowerIcon;
	uint32                                 TowerCost;
	std::shared_ptr<CUIControlRTSTowerBtn> UIButton;
};

class ZN_API CUIControlRTSTowersPanel
	: public CUIControlCommon
{
public:
	ZN_OBJECTCLASS(cUIControlRTSTowersPanel);
	CUIControlRTSTowersPanel(IScene& Scene);
	virtual ~CUIControlRTSTowersPanel();

	// CUIControl
	void Initialize() override;

	void AddTowerButton(std::string TowerName, std::string TowerIcon, uint32 TowerCost, IScene& Scene);
	const std::vector<STowerDescription>& GetTowerButtons();
	void SetTowerButtonClickCallback(std::function<bool(const STowerDescription&)> Func);

protected:
	void CreateWindowGeometry(glm::vec2 Size);
	void OnTowerButtonClick(const IUIControl* Node, glm::vec2 PosInsideButton);

private:
	std::vector<STowerDescription> m_TowerButtons;
	std::function<bool(const STowerDescription&)> m_OnTowerButtonClicked;
};