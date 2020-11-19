#include "stdafx.h"

// General
#include "UIControlCommon.h"

CUIControlCommon::CUIControlCommon(IScene& Scene)
	: CUIControl(Scene)
{}

CUIControlCommon::~CUIControlCommon()
{}



void CUIControlCommon::Initialize()
{
	__super::Initialize();

	//m_Material = MakeShared(CMaterialUIControl, GetBaseManager().GetApplication().GetRenderDevice());

	//m_DefaultQuadGeometry = GetBaseManager().GetApplication().GetRenderDevice().GetPrimitivesFactory().CreateUIQuad(1.0f, 1.0f);
	//m_DefaultQuadGeometry->SetBounds(BoundingBox(glm::vec3(-100.0f), glm::vec3(100.0f)));

	//m_Mesh = GetBaseManager().GetApplication().GetRenderDevice().GetObjectsFactory().CreateModel();
	//m_Mesh->AddConnection(m_Material, m_DefaultQuadGeometry);

	//m_ColorProperty = MakeShared(CPropertyWrapped<glm::vec4>);
	//m_ColorProperty->SetName("Color");
	//m_ColorProperty->SetValueSetter(std::bind(&CUIControlCommon::SetColor, this, std::placeholders::_1));
	//m_ColorProperty->SetValueGetter(std::bind(&CUIControlCommon::GetColor, this));
	//GetProperties()->AddProperty(m_ColorProperty);


	// <SubTexture height="128" width="128" y="502" x="528" name="panel_woodPaper.png"/> // 1193

	glm::vec2 texStart = glm::vec2(528.0f, 502.0f) / 1193.0f;
	glm::vec2 texEnd = texStart + glm::vec2(128.0f) / 1193.0f;

	SSubgeometry subGeom;
	subGeom.Translate = glm::vec2(0.0f);
	subGeom.Size = glm::vec2(64);
	subGeom.Material = MakeShared(CMaterialUIControl, GetBaseManager().GetApplication().GetRenderDevice());
	subGeom.Material->SetTexture(GetBaseManager().GetManager<IznTexturesFactory>()->LoadTexture2D("Interface Pack/Spritesheet/interfacePack_sheet@2.png"));
	subGeom.Geom = GetBaseManager().GetApplication().GetRenderDevice().GetPrimitivesFactory().CreateUIQuad2(glm::vec2(64.0f), texStart, texEnd);
	m_Subgeometries.push_back(subGeom);
}



//
// CUIControl
//
glm::vec2 CUIControlCommon::GetSize() const
{
    return m_Size;
}

const std::vector<CUIControlCommon::SSubgeometry>& CUIControlCommon::GetSubgeometries() const
{
	return m_Subgeometries;
}
