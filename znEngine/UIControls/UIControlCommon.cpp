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

	// <SubTexture height="128" width="128" y="502" x="528" name="panel_woodPaper.png"/> // 1193
	/*
	glm::vec2 texStart = glm::vec2(528.0f, 502.0f) / 1193.0f;
	glm::vec2 texEnd = texStart + glm::vec2(128.0f) / 1193.0f;

	SSubgeometry subGeom;
	subGeom.Translate = glm::vec2(0.0f);
	subGeom.Size = glm::vec2(64);
	subGeom.Material = MakeShared(CMaterialUIControl, GetBaseManager().GetApplication().GetRenderDevice());
	subGeom.Material->SetTexture(GetBaseManager().GetManager<IznTexturesFactory>()->LoadTexture2D("Interface Pack/Spritesheet/interfacePack_sheet@2.png"));
	subGeom.Geom = GetBaseManager().GetApplication().GetRenderDevice().GetPrimitivesFactory().CreateUIQuad(glm::vec2(64.0f), texStart, texEnd);
	AddSubgeometry(subGeom);*/
}

const std::vector<CUIControlCommon::SSubgeometry>& CUIControlCommon::GetSubgeometries() const
{
	return m_Subgeometries;
}



//
// Protected
//
void CUIControlCommon::AddSubgeometry(const SSubgeometry & Subgeometry)
{
	m_Subgeometries.push_back(Subgeometry);
}
