#include "stdafx.h"

// General
#include "RTSGround_Pass.h"

// Additional
#include "Materials/MaterialModel.h"

CRTSGround_Pass::CRTSGround_Pass(IRenderDevice& RenderDevice, IScene& scene)
	: Base3DPass(scene)
{

}

CRTSGround_Pass::~CRTSGround_Pass()
{}


IShaderParameter * CRTSGround_Pass::GetLightsShaderParameter() const
{
	return m_ShaderLightsBufferParameter;
}



//
// IRenderPassPipelined
//
std::shared_ptr<IRenderPassPipelined> CRTSGround_Pass::ConfigurePipeline(std::shared_ptr<IRenderTarget> RenderTarget)
{
	__super::ConfigurePipeline(RenderTarget);

	std::shared_ptr<IShader> vertexShader;
	std::shared_ptr<IShader> pixelShader;

	if (GetRenderDevice().GetDeviceType() == RenderDeviceType::RenderDeviceType_DirectX11)
	{
		vertexShader = GetRenderDevice().GetObjectsFactory().LoadShader(EShaderType::VertexShader, "3D/ModelVS.hlsl", "VS_PTN", { { "INSTANCED", "1" } });
		pixelShader = GetRenderDevice().GetObjectsFactory().LoadShader(EShaderType::PixelShader, "3D/Model_Forward.hlsl", "PS_main");
	}
	vertexShader->LoadInputLayoutFromReflector();

	// PIPELINES
	GetPipeline().GetBlendState()->SetBlendMode(disableBlending);
	GetPipeline().GetDepthStencilState()->SetDepthMode(enableDepthWrites);
	GetPipeline().GetRasterizerState()->SetCullMode(IRasterizerState::CullMode::Back);
	GetPipeline().GetRasterizerState()->SetFillMode(IRasterizerState::FillMode::Solid, IRasterizerState::FillMode::Solid);
	GetPipeline().SetRenderTarget(RenderTarget);
	GetPipeline().SetShader(EShaderType::VertexShader, vertexShader);
	GetPipeline().SetShader(EShaderType::PixelShader, pixelShader);

	m_ShaderLightsBufferParameter = pixelShader->GetShaderParameterByName("LightsVS");
	//_ASSERT(m_ShaderLightsBufferParameter->IsValid());

	m_ShaderInstancesBufferParameter = vertexShader->GetShaderParameterByName("Instances");
	//_ASSERT(m_ShaderInstancesBufferParameter->IsValid());

	return shared_from_this();
}



//
// IVisitor
//
EVisitResult CRTSGround_Pass::Visit(const ISceneNode * SceneNode)
{
	if (SceneNode->GetClass() != cSceneNodeRTSGround)
		return EVisitResult::AllowVisitChilds;

	const ISceneNodeRTSGround* rtsGround = dynamic_cast<const ISceneNodeRTSGround*>(SceneNode);
	_ASSERT(rtsGround != nullptr);

	std::unordered_map<const IModel*, std::vector<PerObject>> modelPriorMap;
	for (int32 x = 0; x < cCellsCount; x++)
	{
		for (int32 z = 0; z < cCellsCount; z++)
		{
			const SRTSCell& cell = rtsGround->GetCellConst(SRTSCellCoords(x, z));
			if (cell.Model == nullptr)
				continue;
			_ASSERT(cell.Model.use_count() > 1);
			modelPriorMap[cell.Model.get()].push_back(PerObject{ glm::translate(cell.Coords.ToPosition()) });
		}
	}

	for (const auto& it : modelPriorMap)
	{
		std::vector<PerObject> instances = it.second;

		if (m_InstancesBuffer == nullptr || instances.size() > m_InstancesBuffer->GetElementCount())
			m_InstancesBuffer = GetRenderDevice().GetObjectsFactory().CreateStructuredBuffer(instances, EAccess::CPUWrite);
		else
			m_InstancesBuffer->Set(instances);

		m_ShaderInstancesBufferParameter->SetStructuredBuffer(m_InstancesBuffer);
		m_ShaderInstancesBufferParameter->Bind();

		for (const auto& c : it.first->GetConnections())
		{
			if (c.Material)
				c.Material->Bind(GetRenderEventArgs().PipelineState->GetPixelShaderPtr());

			SGeometryDrawArgs args = c.GeometryDrawArgs;
			args.InstanceCnt = instances.size();
			c.Geometry->Render(GetRenderEventArgs().PipelineState->GetVertexShaderPtr(), args);

			if (c.Material)
				c.Material->Unbind(GetRenderEventArgs().PipelineState->GetPixelShaderPtr());
		}

		m_ShaderInstancesBufferParameter->Unbind();
	}

	return EVisitResult::AllowVisitChilds;
}
