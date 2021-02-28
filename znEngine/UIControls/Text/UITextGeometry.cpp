#include "stdafx.h"

// General
#include "UITextGeometry.h"

namespace
{
	glm::vec2 CalculateOffsetByAlign(const glm::vec2 TextSize, ETextAlignHorizontal TextAlignHorizontal, ETextAlignVertical TextAlignVertical)
	{
		glm::vec2 offsetByAlign = glm::vec2(0.0f);

		if (TextAlignHorizontal == ETextAlignHorizontal::Left)
			offsetByAlign.x = 0.0f;
		else if (TextAlignHorizontal == ETextAlignHorizontal::Center)
			offsetByAlign.x = -(TextSize.x / 2.0f);
		else if (TextAlignHorizontal == ETextAlignHorizontal::Right)
			offsetByAlign.x = -TextSize.x;

		if (TextAlignVertical == ETextAlignVertical::Top)
			offsetByAlign.y = 0.0f;
		else if (TextAlignVertical == ETextAlignVertical::Center)
			offsetByAlign.y = -(TextSize.y / 2.0f);
		else if (TextAlignVertical == ETextAlignVertical::Bottom)
			offsetByAlign.y = -TextSize.y;

		return offsetByAlign;
	}
}



CUITextGeometry::CUITextGeometry(IRenderDevice& RenderDevice, std::shared_ptr<IGeometry> OriginalGeometry)
	: GeometryProxie(OriginalGeometry)
{
	m_FontBuffer = RenderDevice.GetObjectsFactory().CreateConstantBuffer(SUITextGeometryData());
}

CUITextGeometry::~CUITextGeometry()
{
}

void CUITextGeometry::Render(const IShader * VertexShader, const std::shared_ptr<IznFont>& Font, const std::string& Text, ETextAlignHorizontal TextAlignHorizontal, ETextAlignVertical TextAlignVertical)
{
	Render_BindAllBuffers(VertexShader);
	{
		const auto lines = Utils::SplitTextToLines(Text);
		const auto textSize = Font->GetTextSize(lines);
		const auto offsetByAlign = CalculateOffsetByAlign(textSize, TextAlignHorizontal, TextAlignVertical);

		auto currentCharOffset = glm::vec2(0.0f);
		for (const auto& line : lines)
		{
			const auto lineSize = Font->GetLineSize(line);

			for (auto i = 0; i < line.length(); i++)
			{
				const uint8 ch = static_cast<uint8>(line.c_str()[i]);
				if (ch == '\n')
					throw CException("Line must not contains new-line symbols.");

				BindPerCharacterVSData(VertexShader, SUITextGeometryData(currentCharOffset + offsetByAlign));

				{
					SGeometryDrawArgs GeometryDrawArgs;
					GeometryDrawArgs.VertexStartLocation = (ch) * 6;
					GeometryDrawArgs.VertexCnt = 6;
					Render_Draw(GeometryDrawArgs);
				}

				currentCharOffset.x += static_cast<float>(Font->GetCharSize(ch).x) + 0.01f;
			}

			currentCharOffset.x = 0.0f;
			currentCharOffset.y += static_cast<float>(lineSize.y) + 0.01f;
		}
	}
	Render_UnbindAllBuffers(VertexShader);
}

void CUITextGeometry::BindPerCharacterVSData(const IShader * VertexShader, const SUITextGeometryData& GPUPerCharacterDataVS)
{
	m_FontBuffer->Set(GPUPerCharacterDataVS);

	auto* perCharacterParam = VertexShader->GetShaderParameterByName("PerCharacterDataVS");
	if (perCharacterParam)
	{
		perCharacterParam->SetConstantBuffer(m_FontBuffer);
		perCharacterParam->Bind();
	}
}
