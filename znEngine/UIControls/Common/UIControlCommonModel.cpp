#include "stdafx.h"

// General
#include "UIControlCommonModel.h"

CUIControlCommonModel::CUIControlCommonModel(const IRenderDevice & RenderDevice)
	: ModelProxie(RenderDevice.GetObjectsFactory().CreateModel())
{
}

CUIControlCommonModel::~CUIControlCommonModel()
{
}



//
// IUIControlModel
//
void CUIControlCommonModel::SetTranslate(const glm::vec2& Translate)
{
	m_Translate = Translate;
}

glm::vec2 CUIControlCommonModel::GetTranslate() const
{
	return m_Translate;
}

void CUIControlCommonModel::SetSize(const glm::vec2& Size)
{
	m_Size = Size;
}

glm::vec2 CUIControlCommonModel::GetSize() const
{
	return m_Size;
}

void CUIControlCommonModel::SetMaterial(std::shared_ptr<IMaterialUIControl> Material)
{
	m_Material = Material;
}

std::shared_ptr<IMaterialUIControl> CUIControlCommonModel::GetMaterial() const
{
	return m_Material;
}

void CUIControlCommonModel::SetGeom(std::shared_ptr<IGeometry> Geom)
{
	m_Geom = Geom;
}

std::shared_ptr<IGeometry> CUIControlCommonModel::GetGeom() const
{
	return m_Geom;
}
