#include "stdafx.h"

// General
#include "UITextModel.h"

namespace
{
	const char* cDefaultText = "<empty>";
	const glm::vec4 cDefaultColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
}

CUITextModel::CUITextModel(IRenderDevice& RenderDevice, std::shared_ptr<IznFont> Font)
	: ModelProxie(RenderDevice.GetObjectsFactory().CreateModel())
	, m_Font(Font)
{
	m_Material = MakeShared(CUITextMaterial, RenderDevice);
	m_Material->SetTexture(Font->GetTexture());

	m_Geometry = MakeShared(CUITextGeometry, RenderDevice, Font->GetGeometry());

	GetProperties()->SetName("FontModel");

	m_TextProperty = MakeShared(CProperty<std::string>, "Text", "descr", cDefaultText);
	GetProperties()->AddProperty(m_TextProperty);

	m_TextAlignHorizontalProperty = MakeShared(CProperty<ETextAlignHorizontal>, "TextAlignHorizontal", "Text align horizontal.", ETextAlignHorizontal::Left);
	GetProperties()->AddProperty(m_TextAlignHorizontalProperty);

	m_TextAlignVerticalProperty = MakeShared(CProperty<ETextAlignVertical>, "TextAlignVertical", "Text align vertical.", ETextAlignVertical::Top);
	GetProperties()->AddProperty(m_TextAlignVerticalProperty);

	m_ColorProperty = MakeShared(CProperty<ColorRGBA>, "Color", "Color of text", ColorRGBA(1.0f, 1.0f, 1.0f, 1.0f));
	GetProperties()->AddProperty(m_ColorProperty);
}

CUITextModel::~CUITextModel()
{
}

void CUITextModel::SetText(std::string Text)
{
	m_TextProperty->Set(Text);
}

std::string CUITextModel::GetText() const
{
	return m_TextProperty->Get();
}

void CUITextModel::SetTextAlignHorizontal(ETextAlignHorizontal TextAlignHorizontal)
{
	m_TextAlignHorizontalProperty->Set(TextAlignHorizontal);
}

ETextAlignHorizontal CUITextModel::GetTextAlignHorizontal() const
{
	return m_TextAlignHorizontalProperty->Get();
}

void CUITextModel::SetTextAlignVertical(ETextAlignVertical TextAlignVertical)
{
	m_TextAlignVerticalProperty->Set(TextAlignVertical);
}

ETextAlignVertical CUITextModel::GetTextAlignVertical() const
{
	return m_TextAlignVerticalProperty->Get();
}

void CUITextModel::SetColor(const ColorRGBA& Color)
{
	m_Material->SetColor(Color);
	m_ColorProperty->Set(Color);
}

ColorRGBA CUITextModel::GetColor() const
{
	return m_ColorProperty->Get();
}



//
// IModel
//
bool CUITextModel::Render(const ShaderMap& Shaders) const
{
	auto vertexShader = Shaders.at(EShaderType::VertexShader).get();
	auto pixelShader = Shaders.at(EShaderType::PixelShader).get();

	m_Material->Bind(pixelShader);
	m_Geometry->Render(vertexShader, m_Font, m_TextProperty->Get(), m_TextAlignHorizontalProperty->Get(), m_TextAlignVerticalProperty->Get());
	m_Material->Unbind(pixelShader);

	return true;
}
