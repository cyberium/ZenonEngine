#include "stdafx.h"

// General
#include "FontModel.h"

namespace
{
	const char* cDefaultText = "<empty>";
	const glm::vec4 cDefaultColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
}

CFontModel::CFontModel(IRenderDevice& RenderDevice)
	: ModelProxie(RenderDevice.GetObjectsFactory().CreateModel())
{
	m_TextProperty = MakeShared(CProperty<std::string>, "Text", "descr", cDefaultText);
	//m_TextProperty->SetValueChangedCallback([this](const std::string& NewValue) {
	//	SetSize(GetFont()->GetSize(NewValue));
	//});
	GetProperties()->AddProperty(m_TextProperty);

	m_TextAlignHorizontalProperty = MakeShared(CProperty<ETextAlignHorizontal>, "TextAlignHorizontal", "Text align horizontal.", ETextAlignHorizontal::Left);
	GetProperties()->AddProperty(m_TextAlignHorizontalProperty);

	m_TextAlignVerticalProperty = MakeShared(CProperty<ETextAlignVertical>, "TextAlignVertical", "Text align vertical.", ETextAlignVertical::Top);
	GetProperties()->AddProperty(m_TextAlignVerticalProperty);

	//m_OffsetProperty = MakeShared(CProperty<glm::vec2>, "Offset", "Offset of first string character relatieve to local transform.", cDefaultOffset);
	//GetProperties()->AddProperty(m_OffsetProperty);

	m_ColorProperty = MakeShared(CProperty<ColorRGBA>, "Color", "Color of text", ColorRGBA(1.0f, 1.0f, 1.0f, 1.0f));
	GetProperties()->AddProperty(m_ColorProperty);
}

CFontModel::~CFontModel()
{
}

void CFontModel::SetText(std::string Text)
{
	m_TextProperty->Set(Text);
}

std::string CFontModel::GetText() const
{
	return m_TextProperty->Get();
}

void CFontModel::SetTextAlignHorizontal(ETextAlignHorizontal TextAlignHorizontal)
{
	m_TextAlignHorizontalProperty->Set(TextAlignHorizontal);
}

ETextAlignHorizontal CFontModel::GetTextAlignHorizontal() const
{
	return m_TextAlignHorizontalProperty->Get();
}

void CFontModel::SetTextAlignVertical(ETextAlignVertical TextAlignVertical)
{
	m_TextAlignVerticalProperty->Set(TextAlignVertical);
}

ETextAlignVertical CFontModel::GetTextAlignVertical() const
{
	return m_TextAlignVerticalProperty->Get();
}


/*void CFontModel::SetOffset(glm::vec2 Offset)
{
	m_OffsetProperty->Set(Offset);
}

glm::vec2 CFontModel::GetOffset() const
{
	return m_OffsetProperty->Get();
}*/

void CFontModel::SetColor(ColorRGBA Color)
{
	m_ColorProperty->Set(Color);
}

ColorRGBA CFontModel::GetColor() const
{
	return m_ColorProperty->Get();
}