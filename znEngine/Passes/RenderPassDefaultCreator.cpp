#include "stdafx.h"

// General
#include "RenderPassDefaultCreator.h"

// Additional
#include "Passes/TextureDebugPass.h"
#include "Passes/TextureMaterialPass.h"
#include "Passes/MaterialPassOpaque.h"
#include "Passes/MaterialPassTransperent.h"

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
		return "ModelPassOpaque";
	}
	else if (Index == 5)
	{
		return "ModelPassTransperent";
	}

	throw CException("CRenderPassDefaultCreator: GetRenderPassName(%d) is out of bounds. Count = %d", Index, GetRenderPassCount());
}

std::shared_ptr<IRenderPass> CRenderPassDefaultCreator::CreateRenderPass(size_t Index, IRenderDevice& RenderDevice, std::shared_ptr<IRenderTarget> RenderTarget, const Viewport * Viewport, std::shared_ptr<IScene> Scene) const
{
	if (Index == 0)
	{
		vec4 color = vec4(1.0, 1.0f, 1.0f, 1.0f);
		return std::make_shared<ClearRenderTargetPass>(RenderDevice, RenderTarget, ClearFlags::All, color /*glm::vec4(0.2f, 0.2f, 0.2f, 0.2f)*/, 1.0f, 0);
	}
	else if (Index == 1)
	{
		std::shared_ptr<IRenderPassPipelined> passPipelined = std::make_shared<CDebugMaterialPass>(RenderDevice, Scene);
		passPipelined->CreatePipeline(RenderTarget, Viewport);
		return passPipelined;
	}
	else if (Index == 2)
	{
		std::shared_ptr<IRenderPassPipelined> passPipelined = std::make_shared<CTexturedMaterialPass>(RenderDevice, Scene);
		passPipelined->CreatePipeline(RenderTarget, Viewport);
		return passPipelined;
	}
	else if (Index == 3)
	{
		std::shared_ptr<IRenderPassPipelined> passPipelined = std::make_shared<BaseUIPass>(RenderDevice, Scene);
		passPipelined->CreatePipeline(RenderTarget, Viewport);
		return passPipelined;
	}
	else if (Index == 4)
	{
		std::shared_ptr<IRenderPassPipelined> passPipelined = std::make_shared<CMaterialPassOpaque>(RenderDevice, Scene);
		passPipelined->CreatePipeline(RenderTarget, Viewport);
		return passPipelined;
	}
	else if (Index == 5)
	{
		std::shared_ptr<IRenderPassPipelined> passPipelined = std::make_shared<CMaterialPassTransperent>(RenderDevice, Scene);
		passPipelined->CreatePipeline(RenderTarget, Viewport);
		return passPipelined;
	}

	throw CException("CRenderPassDefaultCreator: CreateRenderPass(%d) is out of bounds. Count = %d", Index, GetRenderPassCount());
}
