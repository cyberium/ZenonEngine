#include "stdafx.h"

// General
#include "UIControlCommon.h"

CUIControlCommon::CUIControlCommon(IScene& Scene)
	: CUIControl(Scene)
{}

CUIControlCommon::~CUIControlCommon()
{}



//
// CUIControl
//
void CUIControlCommon::Initialize()
{
	__super::Initialize();
}



//
// IUIControlCommon
//
void CUIControlCommon::AddSubgeometry(const std::shared_ptr<IUIControlCommonModel>& Subgeometry)
{
	m_Subgeometries.push_back(Subgeometry);
}

const std::vector<std::shared_ptr<IUIControlCommonModel>>& CUIControlCommon::GetSubgeometries() const
{
	return m_Subgeometries;
}

void CUIControlCommon::ClearSubgeometries()
{
	m_Subgeometries.clear();
}



//
// Protected
//
EVisitResult CUIControlCommon::AcceptContent(IVisitor * visitor)
{
	for (const auto& model : m_Subgeometries)
		visitor->Visit(model);
	return EVisitResult::AllowAll;
}

std::vector<std::shared_ptr<IUIControlCommonModel>>& CUIControlCommon::GetSubgeometries()
{
	return m_Subgeometries;
}
