#include "stdafx.h"

// General
#include "ConsolePass.h"


#if 1

CUIConsolePass::CUIConsolePass(IRenderDevice& RenderDevice)
	: RenderPassMultipipelined(RenderDevice)
	, m_PerObjectParameter(nullptr)
{

	m_FontModel = MakeShared(CUITextModel, GetRenderDevice(), GetBaseManager().GetManager<IznFontsManager>()->GetMainFont());
	m_FontModel->SetText("Hello console!\r\n. Hello 2 console");

	m_PerObjectConstantBuffer = GetRenderDevice().GetObjectsFactory().CreateConstantBuffer(PerObject());
}

CUIConsolePass::~CUIConsolePass()
{
}

void CUIConsolePass::Render(size_t PipelineIndex, IPipelineState& Pipeline, RenderEventArgs & e)
{
	if (PipelineIndex == m_FontPipelineIndex)
	{
		{
			PerObject perObject;
			perObject.Model = glm::mat4(1.0f);
			m_PerObjectConstantBuffer->Set(perObject);

			if (m_PerObjectParameter == nullptr)
				m_PerObjectParameter = (Pipeline.GetShaders().at(EShaderType::VertexShader)->GetShaderParameterByName("PerObject"));

			if (m_PerObjectParameter)
			{
				m_PerObjectParameter->SetConstantBuffer(m_PerObjectConstantBuffer);
				m_PerObjectParameter->Bind();
			}
		}

		m_FontModel->Render(Pipeline.GetShaders());

		{
			if (m_PerObjectParameter)
			{
				m_PerObjectParameter->Unbind();
			}
		}
	}
	else if (true)
	{

	}
	else
		throw CException("Unknown pipeline index.");
}




//
// IRenderPassPipelined
//
void CUIConsolePass::Configure(std::shared_ptr<IRenderTarget> RenderTarget)
{
	__super::Configure(RenderTarget);
	
	// Color pipeline
	{
		auto colorPipeline = GetRenderDevice().GetObjectsFactory().CreatePipelineState();
		colorPipeline->SetRenderTarget(RenderTarget);

		std::shared_ptr<IShader> vertexShader = GetRenderDevice().GetObjectsFactory().LoadShader(EShaderType::VertexShader, "UI/UI_Texture.hlsl", "VS_main");
		vertexShader->LoadInputLayoutFromReflector();
		colorPipeline->SetShader(vertexShader);

		std::shared_ptr<IShader> pixelShader = GetRenderDevice().GetObjectsFactory().LoadShader(EShaderType::PixelShader, "UI/UI_Texture.hlsl", "PS_main");
		colorPipeline->SetShader(pixelShader);

		// Material
		colorPipeline->GetRasterizerState()->SetCullMode(IRasterizerState::CullMode::Back);

		m_ColorPipelineIndex = AddPipeline(colorPipeline);
	}


	// Font pipeline
	{
		auto fontPipeline = GetRenderDevice().GetObjectsFactory().CreatePipelineState();
		fontPipeline->SetRenderTarget(RenderTarget);

		fontPipeline->GetBlendState()->SetBlendMode(alphaBlending);
		fontPipeline->GetDepthStencilState()->SetDepthMode(disableDepthWrites);

		std::shared_ptr<IShader> vertexShader = GetRenderDevice().GetObjectsFactory().LoadShader(EShaderType::VertexShader, "UI/UI_Font.hlsl", "VS_main");
		vertexShader->LoadInputLayoutFromReflector();
		fontPipeline->SetShader(vertexShader);

		std::shared_ptr<IShader> pixelShader = GetRenderDevice().GetObjectsFactory().LoadShader(EShaderType::PixelShader, "UI/UI_Font.hlsl", "PS_main");
		fontPipeline->SetShader(pixelShader);

		// Diryy hack!
		fontPipeline->SetTexture(0, GetBaseManager().GetManager<IznFontsManager>()->GetMainFont()->GetTexture());

		m_FontPipelineIndex = AddPipeline(fontPipeline);
	}
}



//
// Protected
//
PerFrame CUIConsolePass::GetPerFrameData(IPipelineState& Pipeline) const
{
	const Viewport& viewport = Pipeline.GetRenderTarget()->GetViewport();

	PerFrame perFrame(
		glm::mat4(1.0f),
		viewport.GetOrthoMatix(),
		glm::vec2(viewport.GetSize())
	);
	return perFrame;
}

#endif
