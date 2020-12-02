#include "stdafx.h"

// General
#include "Postprocess_Gauss.h"

namespace
{
	struct __declspec(align(16)) SGauss
	{
		SGauss()
			: IsHorozontal(0)
		{}

		explicit SGauss(bool IsHorizontal)
			: IsHorozontal(IsHorizontal ? 1 : 0)
		{}

		uint32 IsHorozontal;
		glm::vec3 __padding;
	};
}

CPassPostprocess_Gauss::CPassPostprocess_Gauss(IRenderDevice& RenderDevice, std::shared_ptr<ITexture> InputTexture, bool IsHorizontal)
	: CPassPostprocessBase(RenderDevice, InputTexture)
	, m_IsHorizontal(IsHorizontal)
{
	m_GaussConstantBuffer = GetRenderDevice().GetObjectsFactory().CreateConstantBuffer(SGauss());
}

CPassPostprocess_Gauss::~CPassPostprocess_Gauss()
{}



//
// IRenderPass
// 
void CPassPostprocess_Gauss::Render(RenderEventArgs & e)
{
	//{
	//	m_GaussConstantBuffer->Set(SGauss(true));
	//	m_GaussShaderParameter->Bind();
	//	__super::Render(e);
	//	m_GaussShaderParameter->Unbind();
	//}


	{
		m_GaussConstantBuffer->Set(SGauss(m_IsHorizontal));
		m_GaussShaderParameter->Bind();
		__super::Render(e);
		m_GaussShaderParameter->Unbind();
	}
}



std::shared_ptr<IRenderTarget> CPassPostprocess_Gauss::LoadRenderTarget(std::shared_ptr<IRenderTarget> OriginalRenderTarget)
{
	auto newRenderTarget = CreateRenderTarget(OriginalRenderTarget);
	SetOutputTexture(newRenderTarget->GetTexture(IRenderTarget::AttachmentPoint::Color0));
	return newRenderTarget;
}

std::shared_ptr<IShader> CPassPostprocess_Gauss::LoadVertexShader()
{
	auto samplesCnt = std::to_string(GetPipeline().GetRenderTarget()->GetTexture(IRenderTarget::AttachmentPoint::Color0)->GetSamplesCount());

	auto vertexShader = GetRenderDevice().GetObjectsFactory().LoadShader(EShaderType::VertexShader, "3D/PostprocessGauss.hlsl", "VS_ScreenQuad", { {"MULTISAMPLED", samplesCnt.c_str() } });
	vertexShader->LoadInputLayoutFromReflector();
	return vertexShader;
}

std::shared_ptr<IShader> CPassPostprocess_Gauss::LoadPixelShader()
{
	auto samplesCnt = std::to_string(GetPipeline().GetRenderTarget()->GetTexture(IRenderTarget::AttachmentPoint::Color0)->GetSamplesCount());
	auto pixelShader = GetRenderDevice().GetObjectsFactory().LoadShader(EShaderType::PixelShader, "3D/PostprocessGauss.hlsl", "PS_ScreenQuad", { {"MULTISAMPLED", samplesCnt.c_str() } });
	
	m_GaussShaderParameter = pixelShader->GetShaderParameterByName("Parameters");
	_ASSERT(m_GaussShaderParameter->IsValid());
	m_GaussShaderParameter->SetConstantBuffer(m_GaussConstantBuffer);

	return pixelShader;
}