#include "stdafx.h"

// General
#include "ConsolePass.h"


#if 0

CUIControlConsolePass::CUIControlConsolePass(IRenderDevice& RenderDevice)
	: RenderPassPipelined(RenderDevice)
	, m_PerObjectParameter(nullptr)
{
	m_Text = "Hello console!\nHello 2 console";

	m_Font = GetBaseManager().GetManager<IznFontsManager>()->GetMainFont();

	m_PerObjectConstantBuffer = GetRenderDevice().GetObjectsFactory().CreateConstantBuffer(PerObject());

	m_FontBuffer = GetRenderDevice().GetObjectsFactory().CreateConstantBuffer(SMaterialFontProperties());
}

CUIControlConsolePass::~CUIControlConsolePass()
{
}

void CUIControlConsolePass::Render(RenderEventArgs & e)
{
	{
		PerObject perObject;
		perObject.Model = glm::mat4(1.0f);
		m_PerObjectConstantBuffer->Set(perObject);

		if (m_PerObjectParameter == nullptr)
			m_PerObjectParameter = &(GetPipeline().GetShaders().at(EShaderType::VertexShader)->GetShaderParameterByName("PerObject"));

		if (m_PerObjectParameter->IsValid())
		{
			m_PerObjectParameter->SetConstantBuffer(m_PerObjectConstantBuffer);
			m_PerObjectParameter->Bind();
		}
	}



	{
		const auto& font = m_Font;
		const auto& fontGeometry = font->GetGeometry();
		const auto& fontGeometryInternal = std::dynamic_pointer_cast<IGeometryInternal>(fontGeometry);

		const auto& shaders = GetPipeline().GetShaders();
		auto vertexShader = shaders.at(EShaderType::VertexShader).get();

		fontGeometryInternal->Render_BindAllBuffers(vertexShader);
		{
			const std::string textToRender = m_Text;
			glm::vec2 currentCharOffset = glm::vec2(5.0f);

			for (uint32 i = 0; i < textToRender.length(); i++)
			{
				const uint8 ch = static_cast<uint8>(textToRender.c_str()[i]);
				if (ch == '\n')
				{
					currentCharOffset.x = 5.0f;
					currentCharOffset.y += static_cast<float>(font->GetHeight()) + 0.01f;
					continue;
				}

				{
					SMaterialFontProperties fontProperties;
					fontProperties.Color = color;
					fontProperties.Offset = currentCharOffset;
					m_FontBuffer->Set(fontProperties);
				}

				fontMaterial->Bind(shaders);
				{
					SGeometryDrawArgs GeometryDrawArgs;
					GeometryDrawArgs.VertexStartLocation = (ch) * 6;
					GeometryDrawArgs.VertexCnt = 6;
					fontGeometryInternal->Render_Draw(GeometryDrawArgs);
				}
				fontMaterial->Unbind(shaders);

				currentCharOffset.x += static_cast<float>(font->GetWidth(ch)) + 0.01f;
			}
		}
		fontGeometryInternal->Render_UnbindAllBuffers(vertexShader);
	}


}



//
// IRenderPassPipelined
//
std::shared_ptr<IRenderPassPipelined> CUIControlConsolePass::ConfigurePipeline(std::shared_ptr<IRenderTarget> RenderTarget)
{
	RenderPassPipelined::ConfigurePipeline(RenderTarget);

	GetPipeline().GetBlendState()->SetBlendMode(alphaBlending);
	GetPipeline().GetDepthStencilState()->SetDepthMode(disableDepthWrites);

	std::shared_ptr<IShader> vertexShader = GetRenderDevice().GetObjectsFactory().LoadShader(EShaderType::VertexShader, "UI/UI_Font.hlsl", "VS_main");
	vertexShader->LoadInputLayoutFromReflector();
	GetPipeline().SetShader(EShaderType::VertexShader, vertexShader);

	std::shared_ptr<IShader> pixelShader = GetRenderDevice().GetObjectsFactory().LoadShader(EShaderType::PixelShader, "UI/UI_Font.hlsl", "PS_main");
	GetPipeline().SetShader(EShaderType::PixelShader, pixelShader);

	m_FontBufferParameter = &(vertexShader->GetShaderParameterByName("Material"));
	_ASSERT(m_FontBufferParameter->IsValid());
	m_FontBufferParameter->SetConstantBuffer(m_FontBuffer);

	// Diryy hack!
	GetPipeline().SetTexture(0, GetBaseManager().GetManager<IznFontsManager>()->GetMainFont()->GetTexture());

	return shared_from_this();
}



//
// Protected
//
PerFrame CUIControlConsolePass::GetPerFrameData() const
{
	const Viewport& viewport = GetPipeline().GetRenderTarget()->GetViewport();

	PerFrame perFrame(
		glm::mat4(1.0f),
		viewport.GetOrthoMatix(),
		glm::vec2(viewport.GetSize())
	);
	return perFrame;
}

/*
//
// IVisitor
//
EVisitResult CUIControlConsolePass::Visit(const IUIControl * node)
{
	const CUIControlText* textNode = dynamic_cast<const CUIControlText*>(node);
	if (textNode == nullptr)
		return EVisitResult::AllowVisitChilds;

	RenderPassPipelined::Visit(node);

	const auto& font = textNode->GetFont();
	const auto& fontMaterial = textNode->GetMaterial();
	const auto& fontGeometry = font->GetGeometry();
	const auto& fontGeometryInternal = std::dynamic_pointer_cast<IGeometryInternal>(fontGeometry);

	auto vertexShader = GetRenderEventArgs().PipelineState->GetShaders().at(EShaderType::VertexShader).get();

	fontGeometryInternal->Render_BindAllBuffers(GetRenderEventArgs(), vertexShader);
	{
		const std::string textToRender = textNode->GetText();
		glm::vec2 currentCharOffset = textNode->GetOffset();

		for (uint32 i = 0; i < textToRender.length(); i++)
		{
			const uint8 ch = static_cast<uint8>(textToRender.c_str()[i]);
			if (ch == '\n')
			{
				currentCharOffset.x = textNode->GetOffset().x;
				currentCharOffset.y += static_cast<float>(font->GetHeight()) + 0.01f;
				continue;
			}

			fontMaterial->SetOffset(currentCharOffset);
			fontMaterial->Bind(GetPipeline().GetShaders());
			{
				SGeometryDrawArgs GeometryDrawArgs;
				GeometryDrawArgs.VertexStartLocation = (ch) * 6;
				GeometryDrawArgs.VertexCnt = 6;
				fontGeometryInternal->Render_Draw(GeometryDrawArgs);
			}
			fontMaterial->Unbind(GetPipeline().GetShaders());

			currentCharOffset.x += static_cast<float>(font->GetWidth(ch)) + 0.01f;
		}
	}
	fontGeometryInternal->Render_UnbindAllBuffers(GetRenderEventArgs(), vertexShader);

	return EVisitResult::AllowVisitChilds;
}

EVisitResult CUIControlConsolePass::Visit(const IModel * Model)
{
	_ASSERT(false);
	return EVisitResult::Block;
}
*/

#endif
