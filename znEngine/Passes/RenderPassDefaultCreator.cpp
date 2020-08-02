#include "stdafx.h"

// General
#include "RenderPassDefaultCreator.h"

// Additional
#include "Passes/Technical/ClearRenderTargetPass.h"
#include "Passes/MaterialDebugPass.h"
#include "Passes/MaterialTexturedPass.h"
#include "Passes/MaterialModelPass.h"
#include "Passes/Technical/InvokeFunctionPass.h"

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
		return "MaterialModelPass";
	}
	else if (Index == 5)
	{
		return "InvokePass";
	}

	throw CException("CRenderPassDefaultCreator: GetRenderPassName(%d) is out of bounds. Count = %d", Index, GetRenderPassCount());
}

std::shared_ptr<IRenderPass> CRenderPassDefaultCreator::CreateRenderPass(size_t Index, IRenderDevice& RenderDevice, std::shared_ptr<IRenderTarget> RenderTarget, const Viewport * Viewport, std::shared_ptr<IScene> Scene) const
{
	if (Index == 0)
	{
		glm::vec4 color = glm::vec4(0.3, 0.3f, 0.3f, 1.0f);
		return std::make_shared<ClearRenderTargetPass>(RenderDevice, RenderTarget, ClearFlags::All, color /*glm::vec4(0.2f, 0.2f, 0.2f, 0.2f)*/, 1.0f, 0);
	}
	else if (Index == 1)
	{
		std::shared_ptr<IRenderPassPipelined> passPipelined = std::make_shared<CMaterial_Debug_Pass>(RenderDevice, Scene);
		passPipelined->CreatePipeline(RenderTarget, Viewport);
		return passPipelined;
	}
	else if (Index == 2)
	{
		std::shared_ptr<IRenderPassPipelined> passPipelined = std::make_shared<CMaterial_Textured_Pass>(RenderDevice, Scene);
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
		std::shared_ptr<IRenderPassPipelined> passPipelined = std::make_shared<CMaterialModelPass>(RenderDevice, Scene);
		passPipelined->CreatePipeline(RenderTarget, Viewport);
		return passPipelined;
	}
	else if (Index == 5)
	{
		std::shared_ptr<IRenderPass> passPipelined = std::make_shared<InvokeFunctionPass>(RenderDevice, nullptr);
		return passPipelined;
	}



	throw CException("CRenderPassDefaultCreator: CreateRenderPass(%d) is out of bounds. Count = %d", Index, GetRenderPassCount());
}
