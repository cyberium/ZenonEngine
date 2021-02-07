#include "stdafx.h"

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

// General
#include "FontGeometry.h"

CFontGeometry::CFontGeometry(IRenderDevice& RenderDevice, std::shared_ptr<IGeometry> OriginalGeometry)
	: GeometryProxie(OriginalGeometry)
{
	m_FontBuffer = RenderDevice.GetObjectsFactory().CreateConstantBuffer(SGPUPerCharacterDataVS());
}

CFontGeometry::~CFontGeometry()
{
}

void CFontGeometry::Render(const IShader * VertexShader, const std::shared_ptr<IznFont>& Font, const std::string& Text, ETextAlignHorizontal TextAlignHorizontal, ETextAlignVertical TextAlignVertical)
{
	Render_BindAllBuffers(VertexShader);
	{
		auto currentCharOffset = glm::vec2(0.0f);

		const glm::vec2 textSize = glm::vec2(Font->GetWidth(Text), Font->GetHeight());
		const glm::vec2 offsetByAlign = CalculateOffsetByAlign(textSize, TextAlignHorizontal, TextAlignVertical);

		for (size_t i = 0; i < Text.length(); i++)
		{
			const uint8 ch = static_cast<uint8>(Text.c_str()[i]);
			if (ch == '\n')
			{
				currentCharOffset.x = 0.0f;
				currentCharOffset.y += static_cast<float>(Font->GetHeight()) + 0.01f;
				continue;
			}

			{
				SGPUPerCharacterDataVS fontProperties;
				fontProperties.Offset = currentCharOffset + offsetByAlign;

				BindPerCharacterVSData(VertexShader, fontProperties);
			}

			{
				SGeometryDrawArgs GeometryDrawArgs;
				GeometryDrawArgs.VertexStartLocation = (ch) * 6;
				GeometryDrawArgs.VertexCnt = 6;
				Render_Draw(GeometryDrawArgs);
			}

			currentCharOffset.x += static_cast<float>(textSize.x) + 0.01f;
		}
	}
	Render_UnbindAllBuffers(VertexShader);
}

void CFontGeometry::BindPerCharacterVSData(const IShader * VertexShader, const SGPUPerCharacterDataVS& GPUPerCharacterDataVS)
{
	m_FontBuffer->Set(GPUPerCharacterDataVS);

	auto* perCharacterParam = VertexShader->GetShaderParameterByName("PerCharacterDataVS");
	if (perCharacterParam)
	{
		perCharacterParam->SetConstantBuffer(m_FontBuffer);
		perCharacterParam->Bind();
	}
}
