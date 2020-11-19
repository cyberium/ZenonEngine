#include "stdafx.h"

// General
#include "DrawLightFrustumPass.h"

const float cBBoxSizeIncrement = 0.05f;

CDrawLightFrustumPass::CDrawLightFrustumPass(IRenderDevice& RenderDevice, IScene& Scene)
	: Base3DPass(Scene)
	, m_IsDirty(false)
{
}

CDrawLightFrustumPass::~CDrawLightFrustumPass()
{}

void CDrawLightFrustumPass::SetNeedRefresh()
{
	m_IsDirty = true;
}



//
// IRenderPassPipelined
//
std::shared_ptr<IRenderPassPipelined> CDrawLightFrustumPass::ConfigurePipeline(std::shared_ptr<IRenderTarget> RenderTarget, const Viewport * Viewport)
{
	__super::ConfigurePipeline(RenderTarget, Viewport);

	m_PointBox = GetRenderDevice().GetPrimitivesFactory().CreateCube();

	std::shared_ptr<IShader> vertexShader = GetRenderDevice().GetObjectsFactory().LoadShader(EShaderType::VertexShader, "3D/Debug.hlsl", "VS_main");
	vertexShader->LoadInputLayoutFromReflector();

	std::shared_ptr<IShader> pixelShader = GetRenderDevice().GetObjectsFactory().LoadShader(EShaderType::PixelShader, "3D/Debug.hlsl", "PS_main");
	
	//m_ShaderInstancesBufferParameter = &vertexShader->GetShaderParameterByName("Instances");
	//_ASSERT(m_ShaderInstancesBufferParameter->IsValid());

	m_MaterialDebug = MakeShared(MaterialDebug, GetRenderDevice());
	m_MaterialDebug->SetDiffuseColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

	// PIPELINES
	GetPipeline().GetBlendState()->SetBlendMode(alphaBlending);
	GetPipeline().GetDepthStencilState()->SetDepthMode(enableDepthWrites);
	GetPipeline().GetRasterizerState()->SetCullMode(IRasterizerState::CullMode::Back);
	GetPipeline().GetRasterizerState()->SetFillMode(IRasterizerState::FillMode::Solid, IRasterizerState::FillMode::Solid);
	GetPipeline().SetRenderTarget(RenderTarget);
	GetPipeline().SetShader(EShaderType::VertexShader, vertexShader);
	GetPipeline().SetShader(EShaderType::PixelShader, pixelShader);
	return shared_from_this();
}

const ISceneNode* n;

EVisitResult CDrawLightFrustumPass::Visit(const ISceneNode * node)
{
	if (auto lightComp = node->GetComponentT<ILightComponent3D>())
	{
		n = node;
		return EVisitResult::AllowVisitContent;
	}
	
	return EVisitResult::AllowVisitChilds;
}

EVisitResult CDrawLightFrustumPass::Visit(const ILight3D * Light)
{
	{
		BindPerObjectData(PerObject());

		auto geom = GetRenderDevice().GetPrimitivesFactory().CreateFrustum(Light->GetFrustum());

		m_MaterialDebug->Bind(GetPipeline().GetShaders());
		{
			const IShader* vertexShader = GetPipeline().GetShader(EShaderType::VertexShader).get();
			geom->Render(vertexShader);
		}
		m_MaterialDebug->Unbind(GetPipeline().GetShaders());
	}

	/*{
		float range = Light->GetLightStruct().Range;
		float rad = range * glm::atan(glm::radians(Light->GetLightStruct().SpotlightAngle));

		auto geom = GetRenderDevice().GetPrimitivesFactory().CreateCone(rad, range);


		glm::mat4 m(1.0f);
		m = glm::translate(m, n->GetTranslation() + n->GetRotation() * glm::vec3(0.0f, range / 2.0f, 0.0f));
		m = glm::rotate(m, n->GetRotation().x, glm::vec3(1, 0, 0));
		m = glm::rotate(m, n->GetRotation().y, glm::vec3(0, 1, 0));
		m = glm::rotate(m, n->GetRotation().z, glm::vec3(0, 0, 1));

		BindPerObjectData(m);


		m_MaterialDebug->Bind(GetPipeline().GetShaders());
		{
			const IShader* vertexShader = GetPipeline().GetShader(EShaderType::VertexShader).get();
			geom->Render(GetRenderEventArgs(), vertexShader);
		}
		m_MaterialDebug->Unbind(GetPipeline().GetShaders());
	}*/

	return EVisitResult::AllowAll;
}
