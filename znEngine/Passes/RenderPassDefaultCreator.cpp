#include "stdafx.h"

// General
#include "RenderPassDefaultCreator.h"

// Additional
#include "Passes/ClearRenderTargetPass.h"

#include "Passes/TextureDebugPass.h"
#include "Passes/TextureMaterialPass.h"
#include "Passes/OBJMaterialPass.h"

CRenderPassDefaultCreator::CRenderPassDefaultCreator()
{
}

CRenderPassDefaultCreator::~CRenderPassDefaultCreator()
{
}

size_t CRenderPassDefaultCreator::GetRenderPassCount() const
{
	return 5;
}

std::string CRenderPassDefaultCreator::GetRenderPassName(size_t Index) const
{
	if (Index == 0)
	{
		return "ClearPass";
	}
	else if (Index == 1)
	{
		return "DebugPass";
	}
	else if (Index == 2)
	{
		return "TexturedMaterialPass";
	}
	else if (Index == 3)
	{
		return "BaseUIPass";
	}
	else if (Index == 4)
	{
		return "OBJPass";
	}

	return "<error>";
}

std::shared_ptr<IRenderPass> CRenderPassDefaultCreator::CreateRenderPass(size_t Index, std::shared_ptr<IRenderDevice> RenderDevice, std::shared_ptr<IRenderTarget> RenderTarget, const Viewport * Viewport, std::shared_ptr<IScene> Scene) const
{
	if (Index == 0)
	{
		return std::make_shared<ClearRenderTargetPass>(RenderDevice, RenderTarget, ClearFlags::All, glm::vec4(0.2f, 0.2f, 0.2f, 0.3f), 1.0f, 0);
	}
	else if (Index == 1)
	{
		IBlendState::BlendMode alphaBlending(true, false, IBlendState::BlendFactor::SrcAlpha, IBlendState::BlendFactor::OneMinusSrcAlpha, IBlendState::BlendOperation::Add, IBlendState::BlendFactor::SrcAlpha, IBlendState::BlendFactor::OneMinusSrcAlpha);
		IBlendState::BlendMode disableBlending;
		IDepthStencilState::DepthMode enableDepthWrites(true, IDepthStencilState::DepthWrite::Enable);
		IDepthStencilState::DepthMode disableDepthWrites(false, IDepthStencilState::DepthWrite::Disable);

		std::shared_ptr<IShader> g_pVertexShader;
		std::shared_ptr<IShader> g_pPixelShader;

		if (RenderDevice->GetDeviceType() == RenderDeviceType::RenderDeviceType_DirectX)
		{
			g_pVertexShader = RenderDevice->CreateShader(
				IShader::ShaderType::VertexShader, "IDB_SHADER_3D_DEBUG", IShader::ShaderMacros(), "VS_main", "latest"
			);
			g_pPixelShader = RenderDevice->CreateShader(
				IShader::ShaderType::PixelShader, "IDB_SHADER_3D_DEBUG", IShader::ShaderMacros(), "PS_main", "latest"
			);
		}
		else
		{
			g_pVertexShader = RenderDevice->CreateShader(
				IShader::ShaderType::VertexShader, "IDB_SHADER_OGL_3D_DEBUG_VS", IShader::ShaderMacros(), "", ""
			);
			g_pPixelShader = RenderDevice->CreateShader(
				IShader::ShaderType::PixelShader, "IDB_SHADER_OGL_3D_DEBUG_PS", IShader::ShaderMacros(), "", ""
			);
		}
		g_pVertexShader->LoadInputLayoutFromReflector();

		// PIPELINES
		std::shared_ptr<IPipelineState> Pipeline = RenderDevice->CreatePipelineState();
		Pipeline->GetBlendState()->SetBlendMode(disableBlending);
		Pipeline->GetDepthStencilState()->SetDepthMode(enableDepthWrites);
		Pipeline->GetRasterizerState()->SetCullMode(IRasterizerState::CullMode::None);
		Pipeline->GetRasterizerState()->SetFillMode(IRasterizerState::FillMode::Solid);
		Pipeline->SetRenderTarget(RenderTarget);
		Pipeline->GetRasterizerState()->SetViewport(Viewport);
		Pipeline->SetShader(IShader::ShaderType::VertexShader, g_pVertexShader);
		Pipeline->SetShader(IShader::ShaderType::PixelShader, g_pPixelShader);

		return std::make_shared<CDebugMaterialPass>(RenderDevice, Scene, Pipeline);
	}
	else if (Index == 2)
	{
		IBlendState::BlendMode alphaBlending(true, false, IBlendState::BlendFactor::SrcAlpha, IBlendState::BlendFactor::OneMinusSrcAlpha, IBlendState::BlendOperation::Add, IBlendState::BlendFactor::SrcAlpha, IBlendState::BlendFactor::OneMinusSrcAlpha);
		IBlendState::BlendMode disableBlending;
		IDepthStencilState::DepthMode enableDepthWrites(true, IDepthStencilState::DepthWrite::Enable);
		IDepthStencilState::DepthMode disableDepthWrites(false, IDepthStencilState::DepthWrite::Disable);

		std::shared_ptr<IShader> g_pVertexShader;
		std::shared_ptr<IShader> g_pPixelShader;

		if (RenderDevice->GetDeviceType() == RenderDeviceType::RenderDeviceType_DirectX)
		{
			g_pVertexShader = RenderDevice->CreateShader(
				IShader::ShaderType::VertexShader, "IDB_SHADER_3D_TEXTURED", IShader::ShaderMacros(), "VS_main", "latest"
			);
			g_pPixelShader = RenderDevice->CreateShader(
				IShader::ShaderType::PixelShader, "IDB_SHADER_3D_TEXTURED", IShader::ShaderMacros(), "PS_main", "latest"
			);
		}
		else
		{
			g_pVertexShader = RenderDevice->CreateShader(
				IShader::ShaderType::VertexShader, "IDB_SHADER_OGL_3D_TEXTURED_VS", IShader::ShaderMacros(), "", ""
			);
			g_pPixelShader = RenderDevice->CreateShader(
				IShader::ShaderType::PixelShader, "IDB_SHADER_OGL_3D_TEXTURED_PS", IShader::ShaderMacros(), "", ""
			);
		}
		g_pVertexShader->LoadInputLayoutFromReflector();

		//std::vector<SCustomVertexElement> elements;
		//elements.push_back({ 0, 0,  ECustomVertexElementType::FLOAT3, ECustomVertexElementUsage::POSITION, 0 });
		//elements.push_back({ 0, 12, ECustomVertexElementType::FLOAT2, ECustomVertexElementUsage::TEXCOORD, 0 });
		//elements.push_back({ 0, 20, ECustomVertexElementType::FLOAT3, ECustomVertexElementUsage::NORMAL, 0 });
		//g_pVertexShader->LoadInputLayoutFromCustomElements(elements);

		// PIPELINES
		std::shared_ptr<IPipelineState> Pipeline = RenderDevice->CreatePipelineState();
		Pipeline->GetBlendState()->SetBlendMode(disableBlending);
		Pipeline->GetDepthStencilState()->SetDepthMode(enableDepthWrites);
		Pipeline->GetRasterizerState()->SetCullMode(IRasterizerState::CullMode::None);
		Pipeline->GetRasterizerState()->SetFillMode(IRasterizerState::FillMode::Solid);
		Pipeline->SetRenderTarget(RenderTarget);
		Pipeline->GetRasterizerState()->SetViewport(Viewport);
		Pipeline->SetShader(IShader::ShaderType::VertexShader, g_pVertexShader);
		Pipeline->SetShader(IShader::ShaderType::PixelShader, g_pPixelShader);

		return std::make_shared<CTexturedMaterialPass>(RenderDevice, Scene, Pipeline);
	}
	else if (Index == 3)
	{
		IBlendState::BlendMode alphaBlending(true, false, IBlendState::BlendFactor::SrcAlpha, IBlendState::BlendFactor::OneMinusSrcAlpha, IBlendState::BlendOperation::Add, IBlendState::BlendFactor::SrcAlpha, IBlendState::BlendFactor::OneMinusSrcAlpha);
		IBlendState::BlendMode disableBlending;
		IDepthStencilState::DepthMode enableDepthWrites(true, IDepthStencilState::DepthWrite::Enable);
		IDepthStencilState::DepthMode disableDepthWrites(false, IDepthStencilState::DepthWrite::Disable);

		std::shared_ptr<IPipelineState> UIPipeline = RenderDevice->CreatePipelineState();
		UIPipeline->GetBlendState()->SetBlendMode(alphaBlending);
		UIPipeline->GetDepthStencilState()->SetDepthMode(disableDepthWrites);
		UIPipeline->GetRasterizerState()->SetCullMode(IRasterizerState::CullMode::None);
		UIPipeline->GetRasterizerState()->SetFillMode(IRasterizerState::FillMode::Solid);
		UIPipeline->GetRasterizerState()->SetAntialiasedLineEnable(true);
		UIPipeline->GetRasterizerState()->SetMultisampleEnabled(true);
		UIPipeline->SetRenderTarget(RenderTarget);
		UIPipeline->GetRasterizerState()->SetViewport(Viewport);

		return std::make_shared<BaseUIPass>(RenderDevice, Scene, UIPipeline);
	}
	else if (Index == 4)
	{
		IBlendState::BlendMode alphaBlending(true, false, IBlendState::BlendFactor::SrcAlpha, IBlendState::BlendFactor::OneMinusSrcAlpha, IBlendState::BlendOperation::Add, IBlendState::BlendFactor::SrcAlpha, IBlendState::BlendFactor::OneMinusSrcAlpha);
		IBlendState::BlendMode disableBlending;
		IDepthStencilState::DepthMode enableDepthWrites(true, IDepthStencilState::DepthWrite::Enable);
		IDepthStencilState::DepthMode disableDepthWrites(false, IDepthStencilState::DepthWrite::Disable);

		std::shared_ptr<IShader> g_pVertexShader;
		std::shared_ptr<IShader> g_pPixelShader;

		if (RenderDevice->GetDeviceType() == RenderDeviceType::RenderDeviceType_DirectX)
		{
			g_pVertexShader = RenderDevice->CreateShader(
				IShader::ShaderType::VertexShader, "IDB_SHADER_3D_OBJMODELS", IShader::ShaderMacros(), "VS_main", "latest"
			);
			g_pPixelShader = RenderDevice->CreateShader(
				IShader::ShaderType::PixelShader, "IDB_SHADER_3D_OBJMODELS", IShader::ShaderMacros(), "PS_main", "latest"
			);
		}
		g_pVertexShader->LoadInputLayoutFromReflector();

		//std::vector<SCustomVertexElement> elements;
		//elements.push_back({ 0, 0,  ECustomVertexElementType::FLOAT3, ECustomVertexElementUsage::POSITION, 0 });
		//elements.push_back({ 0, 12, ECustomVertexElementType::FLOAT2, ECustomVertexElementUsage::TEXCOORD, 0 });
		//elements.push_back({ 0, 20, ECustomVertexElementType::FLOAT3, ECustomVertexElementUsage::NORMAL, 0 });
		//g_pVertexShader->LoadInputLayoutFromCustomElements(elements);

		// PIPELINES
		std::shared_ptr<IPipelineState> Pipeline = RenderDevice->CreatePipelineState();
		Pipeline->GetBlendState()->SetBlendMode(disableBlending);
		Pipeline->GetDepthStencilState()->SetDepthMode(enableDepthWrites);
		Pipeline->GetRasterizerState()->SetCullMode(IRasterizerState::CullMode::None);
		Pipeline->GetRasterizerState()->SetFillMode(IRasterizerState::FillMode::Solid);
		Pipeline->SetRenderTarget(RenderTarget);
		Pipeline->GetRasterizerState()->SetViewport(Viewport);
		Pipeline->SetShader(IShader::ShaderType::VertexShader, g_pVertexShader);
		Pipeline->SetShader(IShader::ShaderType::PixelShader, g_pPixelShader);

		return std::make_shared<COBJMaterialPass>(RenderDevice, Scene, Pipeline);
	}

	return nullptr;
}
