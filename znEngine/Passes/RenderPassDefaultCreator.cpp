#include "stdafx.h"

// General
#include "RenderPassDefaultCreator.h"

// Additional
#include "Passes/ClearRenderTargetPass.h"

#include "Passes/TextureDebugPass.h"
#include "Passes/TextureMaterialPass.h"
#include "Models/FBX/FBXMaterialPassOpaque.h"
#include "Models/FBX/FBXMaterialPassTransperent.h"

CRenderPassDefaultCreator::CRenderPassDefaultCreator()
{
}

CRenderPassDefaultCreator::~CRenderPassDefaultCreator()
{
}

size_t CRenderPassDefaultCreator::GetRenderPassCount() const
{
	return 6;
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
		return "FBXPassOpaque";
	}
	else if (Index == 5)
	{
		return "FBXPassTransperent";
	}

	throw CException("CRenderPassDefaultCreator: GetRenderPassName(%d) is out of bounds. Count = %d", Index, GetRenderPassCount());
}

std::shared_ptr<IRenderPass> CRenderPassDefaultCreator::CreateRenderPass(size_t Index, std::shared_ptr<IRenderDevice> RenderDevice, std::shared_ptr<IRenderTarget> RenderTarget, const Viewport * Viewport, std::shared_ptr<IScene> Scene) const
{
	IBlendState::BlendMode alphaBlending(true, false, IBlendState::BlendFactor::SrcAlpha, IBlendState::BlendFactor::OneMinusSrcAlpha, IBlendState::BlendOperation::Add, IBlendState::BlendFactor::SrcAlpha, IBlendState::BlendFactor::OneMinusSrcAlpha);
	IBlendState::BlendMode disableBlending;
	IDepthStencilState::DepthMode enableDepthWrites(true, IDepthStencilState::DepthWrite::Enable);
	IDepthStencilState::DepthMode disableDepthWrites(false, IDepthStencilState::DepthWrite::Disable);

	if (Index == 0)
	{
		return std::make_shared<ClearRenderTargetPass>(RenderDevice, RenderTarget, ClearFlags::All, glm::vec4(0.2f, 0.2f, 0.2f, 0.2f), 1.0f, 0);
	}
	else if (Index == 1)
	{
		std::shared_ptr<IShader> g_pVertexShader;
		std::shared_ptr<IShader> g_pGeometryShader;
		std::shared_ptr<IShader> g_pPixelShader;

		if (RenderDevice->GetDeviceType() == RenderDeviceType::RenderDeviceType_DirectX)
		{
			g_pVertexShader = RenderDevice->CreateShader(SShaderType::VertexShader, "IDB_SHADER_3D_DEBUG", IShader::ShaderMacros(), "VS_main", "latest");
			g_pGeometryShader = RenderDevice->CreateShader(SShaderType::GeometryShader, "IDB_SHADER_3D_DEBUG", IShader::ShaderMacros(), "GS_main", "latest");
			g_pPixelShader = RenderDevice->CreateShader(SShaderType::PixelShader, "IDB_SHADER_3D_DEBUG", IShader::ShaderMacros(), "PS_main", "latest");
		}
		else
		{
			g_pVertexShader = RenderDevice->CreateShader(SShaderType::VertexShader, "IDB_SHADER_OGL_3D_DEBUG_VS", IShader::ShaderMacros(), "", "");
			g_pPixelShader = RenderDevice->CreateShader(SShaderType::PixelShader, "IDB_SHADER_OGL_3D_DEBUG_PS", IShader::ShaderMacros(), "", "");
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
		Pipeline->SetShader(SShaderType::VertexShader, g_pVertexShader);
		Pipeline->SetShader(SShaderType::GeometryShader, g_pGeometryShader);
		Pipeline->SetShader(SShaderType::PixelShader, g_pPixelShader);

		return std::make_shared<CDebugMaterialPass>(RenderDevice, Scene, Pipeline);
	}
	else if (Index == 2)
	{
		std::shared_ptr<IShader> g_pVertexShader;
		std::shared_ptr<IShader> g_pPixelShader;

		if (RenderDevice->GetDeviceType() == RenderDeviceType::RenderDeviceType_DirectX)
		{
			g_pVertexShader = RenderDevice->CreateShader(SShaderType::VertexShader, "IDB_SHADER_3D_TEXTURED", IShader::ShaderMacros(), "VS_main", "latest");
			g_pPixelShader = RenderDevice->CreateShader(SShaderType::PixelShader, "IDB_SHADER_3D_TEXTURED", IShader::ShaderMacros(), "PS_main", "latest");
		}
		else
		{
			g_pVertexShader = RenderDevice->CreateShader(SShaderType::VertexShader, "IDB_SHADER_OGL_3D_TEXTURED_VS", IShader::ShaderMacros(), "", "");
			g_pPixelShader = RenderDevice->CreateShader(SShaderType::PixelShader, "IDB_SHADER_OGL_3D_TEXTURED_PS", IShader::ShaderMacros(), "", "");
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
		Pipeline->SetShader(SShaderType::VertexShader, g_pVertexShader);
		Pipeline->SetShader(SShaderType::PixelShader, g_pPixelShader);

		return std::make_shared<CTexturedMaterialPass>(RenderDevice, Scene, Pipeline);
	}
	else if (Index == 3)
	{
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
		std::shared_ptr<IShader> g_pVertexShader;
		std::shared_ptr<IShader> g_pPixelShader;

		if (RenderDevice->GetDeviceType() == RenderDeviceType::RenderDeviceType_DirectX)
		{
			g_pVertexShader = RenderDevice->CreateShader(SShaderType::VertexShader, "IDB_SHADER_3D_MODEL", IShader::ShaderMacros(), "VS_PTN", "latest");
			g_pPixelShader = RenderDevice->CreateShader(SShaderType::PixelShader, "IDB_SHADER_3D_MODEL", IShader::ShaderMacros(), "PS_main", "latest");
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
		Pipeline->SetShader(SShaderType::VertexShader, g_pVertexShader);
		Pipeline->SetShader(SShaderType::PixelShader, g_pPixelShader);

		return std::make_shared<CFBXMaterialPassOpaque>(RenderDevice, Scene, Pipeline);
	}
	else if (Index == 5)
	{
		std::shared_ptr<IShader> g_pVertexShader;
		std::shared_ptr<IShader> g_pPixelShader;

		if (RenderDevice->GetDeviceType() == RenderDeviceType::RenderDeviceType_DirectX)
		{
			g_pVertexShader = RenderDevice->CreateShader(SShaderType::VertexShader, "IDB_SHADER_3D_MODEL", IShader::ShaderMacros(), "VS_main", "latest");
			g_pPixelShader = RenderDevice->CreateShader(SShaderType::PixelShader, "IDB_SHADER_3D_MODEL", IShader::ShaderMacros(), "PS_main", "latest");
		}
		g_pVertexShader->LoadInputLayoutFromReflector();

		// PIPELINES
		std::shared_ptr<IPipelineState> Pipeline = RenderDevice->CreatePipelineState();
		Pipeline->GetBlendState()->SetBlendMode(alphaBlending);
		Pipeline->GetDepthStencilState()->SetDepthMode(enableDepthWrites);
		Pipeline->GetRasterizerState()->SetCullMode(IRasterizerState::CullMode::None);
		Pipeline->GetRasterizerState()->SetFillMode(IRasterizerState::FillMode::Solid);
		Pipeline->SetRenderTarget(RenderTarget);
		Pipeline->GetRasterizerState()->SetViewport(Viewport);
		Pipeline->SetShader(SShaderType::VertexShader, g_pVertexShader);
		Pipeline->SetShader(SShaderType::PixelShader, g_pPixelShader);

		return std::make_shared<CFBXMaterialPassTransperent>(RenderDevice, Scene, Pipeline);
	}

	throw CException("CRenderPassDefaultCreator: CreateRenderPass(%d) is out of bounds. Count = %d", Index, GetRenderPassCount());
}
