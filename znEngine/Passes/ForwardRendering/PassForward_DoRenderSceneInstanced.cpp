#include "stdafx.h"

// General
#include "PassForward_DoRenderSceneInstanced.h"

// Additional
#include "Materials/MaterialModel.h"

CPassForward_DoRenderSceneInstanced::CPassForward_DoRenderSceneInstanced(IRenderDevice& RenderDevice, const std::shared_ptr<IRenderPassCreateTypelessList>& SceneCreateTypelessListPass)
	: RenderPassPipelined(RenderDevice)
	, m_SceneCreateTypelessListPass(SceneCreateTypelessListPass)
{

}

CPassForward_DoRenderSceneInstanced::~CPassForward_DoRenderSceneInstanced()
{}


IShaderParameter * CPassForward_DoRenderSceneInstanced::GetLightsShaderParameter() const
{
	return m_ShaderLightsBufferParameter;
}



//
// IRenderPass
//
void CPassForward_DoRenderSceneInstanced::Render(RenderEventArgs & e)
{
	std::unordered_map<const IModel*, std::vector<const ISceneNode*>> modelPriorMap;
	for (const auto& it : m_SceneCreateTypelessListPass->GetModelsList())
	{
		if (it.SceneNode->GetClass() != cSceneNode3D)
			continue;
		modelPriorMap[it.Model.get()].push_back(it.SceneNode.get());
	}

	for (const auto& it : modelPriorMap)
	{
		std::vector<PerObject> instances;
		instances.reserve(it.second.size());
		std::for_each(it.second.begin(), it.second.end(), [&instances](const ISceneNode* sceneNode) {
			instances.push_back(PerObject{ sceneNode->GetWorldTransfom() });
		});

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

}



//
// IRenderPassPipelined
//
std::shared_ptr<IRenderPassPipelined> CPassForward_DoRenderSceneInstanced::ConfigurePipeline(std::shared_ptr<IRenderTarget> RenderTarget)
{
	__super::ConfigurePipeline(RenderTarget);

	std::shared_ptr<IShader> vertexShader = GetRenderDevice().GetObjectsFactory().LoadShader(EShaderType::VertexShader, "3D/ModelVS.hlsl", "VS_PTN", { {"INSTANCED", "1" } });
	std::shared_ptr<IShader> pixelShader = GetRenderDevice().GetObjectsFactory().LoadShader(EShaderType::PixelShader, "3D/Model_Forward.hlsl", "PS_main");
	vertexShader->LoadInputLayoutFromReflector();

	// PIPELINES
	GetPipeline().SetShader(vertexShader);
	GetPipeline().SetShader(pixelShader);

	m_ShaderBonesBufferParameter = vertexShader->GetShaderParameterByName("Bones");
	//_ASSERT(m_ShaderBonesBufferParameter->IsValid());

	m_ShaderLightsBufferParameter = pixelShader->GetShaderParameterByName("LightsVS");
	//_ASSERT(m_ShaderLightsBufferParameter->IsValid());

	m_ShaderInstancesBufferParameter = vertexShader->GetShaderParameterByName("Instances");
	_ASSERT(m_ShaderInstancesBufferParameter);

	return shared_from_this();
}
