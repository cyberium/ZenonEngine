#include "stdafx.h"

// General
#include "UICommonModel.h"

CUICommonModel::CUICommonModel(const IRenderDevice & RenderDevice)
	: ModelProxie(RenderDevice.GetObjectsFactory().CreateModel())
{
}

CUICommonModel::~CUICommonModel()
{
}



//
// IUIControlModel
//
void CUICommonModel::SetTranslate(const glm::vec2& Translate)
{
	m_Translate = Translate;
}

glm::vec2 CUICommonModel::GetTranslate() const
{
	return m_Translate;
}

void CUICommonModel::SetSize(const glm::vec2& Size)
{
	m_Size = Size;
}

glm::vec2 CUICommonModel::GetSize() const
{
	return m_Size;
}

void CUICommonModel::SetMaterial(std::shared_ptr<IUIControlCommonMaterial> Material)
{
	m_Material = Material;
}

std::shared_ptr<IUIControlCommonMaterial> CUICommonModel::GetMaterial() const
{
	return m_Material;
}

void CUICommonModel::SetGeom(std::shared_ptr<IGeometry> Geom)
{
	m_Geom = Geom;
}

std::shared_ptr<IGeometry> CUICommonModel::GetGeom() const
{
	return m_Geom;
}




//
// IModel
//
bool CUICommonModel::Render(const ShaderMap& Shaders) const
{
	auto vertexShader = Shaders.at(EShaderType::VertexShader).get();
	auto pixelShader = Shaders.at(EShaderType::PixelShader).get();

	GetMaterial()->Bind(pixelShader);
	GetGeom()->Render(vertexShader, SGeometryDrawArgs());
	GetMaterial()->Unbind(pixelShader);
}
