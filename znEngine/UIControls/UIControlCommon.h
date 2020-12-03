#pragma once

#include "../Materials/MaterialUIControl.h"

class ZN_API CUIControlCommon
	: public CUIControl
	, public IUIControlCommon
{
public:
	CUIControlCommon(IScene& Scene);
	virtual ~CUIControlCommon();

	// CUIControl
	void Initialize() override;

	// IUIControlCommon
	void AddSubgeometry(const SSubgeometry& Subgeometry) override;
	const std::vector<SSubgeometry>& GetSubgeometries() const override;
	void ClearSubgeometries();

protected:
	std::vector<SSubgeometry>& GetSubgeometries();

private:
	std::vector<SSubgeometry> m_Subgeometries;
};