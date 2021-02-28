#include "stdafx.h"

// General
#include "ConsolePass.h"

// Additional
#include "UIControls/Common/UICommonMaterial.h"

CUIConsolePass::CUIConsolePass(IRenderDevice& RenderDevice)
	: RenderPassMultipipelined(RenderDevice)
	//, m_PerObjectParameter(nullptr)
{
	auto colorMaterial = MakeShared(CUICommonMaterial, GetRenderDevice());
	colorMaterial->SetColor(ColorRGBA(0.33f, 0.33f, 0.33f, 1.0f));

	auto colorGeometry = GetRenderDevice().GetPrimitivesFactory().CreateUIQuad(glm::vec2(1.0f));

	m_ColorModel = MakeShared(CUICommonModel, GetRenderDevice());
	m_ColorModel->SetMaterial(colorMaterial);
	m_ColorModel->SetGeom(colorGeometry);

	m_FontModel = MakeShared(CUITextModel, GetRenderDevice(), GetBaseManager().GetManager<IznFontsManager>()->GetMainFont());
	m_FontModel->SetText("Hello console!\r\n. Hello 2 console");

	m_PerObjectConstantBuffer = GetRenderDevice().GetObjectsFactory().CreateConstantBuffer(PerObject());
}

CUIConsolePass::~CUIConsolePass()
{
}

void CUIConsolePass::Render(size_t PipelineIndex, IPipelineState& Pipeline, RenderEventArgs & e)
{
	if (PipelineIndex == m_ColorPipelineIndex)
	{
		auto perObjectParameter = Pipeline.GetShaders().at(EShaderType::VertexShader)->GetShaderParameterByName("PerObject");

		{
			m_PerObjectConstantBuffer->Set(PerObject(glm::vec2(0.0f), 0.0f, glm::vec2(512.0f, 256.0f)));

			if (perObjectParameter)
			{
				perObjectParameter->SetConstantBuffer(m_PerObjectConstantBuffer);
				perObjectParameter->Bind();
			}
		}

		m_ColorModel->Render(Pipeline.GetShaders());

		{
			if (perObjectParameter)
			{
				perObjectParameter->Unbind();
			}
		}
	}
	else if (PipelineIndex == m_FontPipelineIndex)
	{
		auto perObjectParameter = Pipeline.GetShaders().at(EShaderType::VertexShader)->GetShaderParameterByName("PerObject");

		{
			m_PerObjectConstantBuffer->Set(PerObject(glm::vec2(5.0f)));

			if (perObjectParameter)
			{
				perObjectParameter->SetConstantBuffer(m_PerObjectConstantBuffer);
				perObjectParameter->Bind();
			}
		}

		m_FontModel->Render(Pipeline.GetShaders());

		{
			if (perObjectParameter)
			{
				perObjectParameter->Unbind();
			}
		}
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

		std::shared_ptr<IShader> vertexShader = GetRenderDevice().GetObjectsFactory().LoadShader(EShaderType::VertexShader, "UI/UIControlCommon.hlsl", "VS_main");
		vertexShader->LoadInputLayoutFromReflector();
		colorPipeline->SetShader(vertexShader);

		std::shared_ptr<IShader> pixelShader = GetRenderDevice().GetObjectsFactory().LoadShader(EShaderType::PixelShader, "UI/UIControlCommon.hlsl", "PS_main");
		colorPipeline->SetShader(pixelShader);

		// Material
		colorPipeline->GetBlendState()->SetBlendMode(alphaBlending);
		colorPipeline->GetDepthStencilState()->SetDepthMode(disableDepthWrites);
		colorPipeline->GetRasterizerState()->SetCullMode(IRasterizerState::CullMode::Back);

		m_ColorPipelineIndex = AddPipeline(colorPipeline);
	}


	// Font pipeline
	{
		auto fontPipeline = GetRenderDevice().GetObjectsFactory().CreatePipelineState();
		fontPipeline->SetRenderTarget(RenderTarget);

		std::shared_ptr<IShader> vertexShader = GetRenderDevice().GetObjectsFactory().LoadShader(EShaderType::VertexShader, "UI/UI_Font.hlsl", "VS_main");
		vertexShader->LoadInputLayoutFromReflector();
		fontPipeline->SetShader(vertexShader);

		std::shared_ptr<IShader> pixelShader = GetRenderDevice().GetObjectsFactory().LoadShader(EShaderType::PixelShader, "UI/UI_Font.hlsl", "PS_main");
		fontPipeline->SetShader(pixelShader);

		fontPipeline->GetBlendState()->SetBlendMode(alphaBlending);
		fontPipeline->GetDepthStencilState()->SetDepthMode(disableDepthWrites);
		fontPipeline->GetRasterizerState()->SetCullMode(IRasterizerState::CullMode::Back);

		// Diryy hack!
		//fontPipeline->SetTexture(0, GetBaseManager().GetManager<IznFontsManager>()->GetMainFont()->GetTexture());

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
