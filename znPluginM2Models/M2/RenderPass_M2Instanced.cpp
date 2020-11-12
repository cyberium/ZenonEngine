#include "stdafx.h"

// General
#include "RenderPass_M2Instanced.h"

// Additional
#include "M2_Part_Color.h"
#include "M2_Part_Material.h"
#include "M2_Part_Texture.h"
#include "M2_Part_TextureTransform.h"
#include "M2_Part_TextureWeight.h"

// Additional (SceneNodes)
#include "M2_Base_Instance.h"

// Additional (meshes)
#include "M2_Skin_Batch.h"


CRenderPass_M2_Instanced::CRenderPass_M2_Instanced(IRenderDevice & RenderDevice, const std::shared_ptr<IRenderPassCreateTypelessList>& SceneCreateTypedListsPass, bool OpaqueDraw)
	: CRenderPass_M2(RenderDevice, SceneCreateTypedListsPass, OpaqueDraw)
{
	m_InstancesBuffer = GetRenderDevice().GetObjectsFactory().CreateStructuredBuffer(nullptr, 1000, sizeof(M2PerObject), EAccess::CPUWrite);
}

CRenderPass_M2_Instanced::~CRenderPass_M2_Instanced()
{
}

void CRenderPass_M2_Instanced::Render(RenderEventArgs & e)
{
	std::unordered_map<const CM2_Skin*, std::vector<const CM2_Base_Instance*>> modelPriorMap;

	for (const auto& it : m_SceneCreateTypelessListPass->GetModelsList())
	{
		if (it.SceneNode->GetClass() == cSceneNodeM2)
		{
			modelPriorMap[static_cast<const CM2_Skin*>(it.Model)].push_back(dynamic_cast<const CM2_Base_Instance*>(it.SceneNode));
			DoRenderSceneNode(it.SceneNode);
			DoRenderModel(it.Model);
		}
	}

	for (const auto& it : modelPriorMap)
	{
		std::vector<M2PerObject> instances;
		instances.reserve(it.second.size());
		std::for_each(it.second.begin(), it.second.end(), [&instances](const CM2_Base_Instance* sceneNode) {
			instances.push_back(M2PerObject(sceneNode->GetWorldTransfom(), sceneNode->getColor())); 
		});

		if (instances.size() > m_InstancesBuffer->GetElementCount())
			m_InstancesBuffer = GetRenderDevice().GetObjectsFactory().CreateStructuredBuffer(instances, EAccess::CPUWrite);
		else
			m_InstancesBuffer->Set(instances);

		m_ShaderInstancesBufferParameter->SetStructuredBuffer(m_InstancesBuffer);
		m_ShaderInstancesBufferParameter->Bind();
		{
			DoRenderM2Model(*it.second.begin(), it.first, m_OpaqueDraw, instances.size());
		}
		m_ShaderInstancesBufferParameter->Unbind();
	}


	//Log::Info("Test = '%d'", modelPriorMap.size());

}

std::shared_ptr<IRenderPassPipelined> CRenderPass_M2_Instanced::ConfigurePipeline(std::shared_ptr<IRenderTarget> RenderTarget, const Viewport * Viewport)
{
	__super::ConfigurePipeline(RenderTarget, Viewport);

	// CreateShaders
	std::shared_ptr<IShader> vertexShader = GetRenderDevice().GetObjectsFactory().LoadShader(EShaderType::VertexShader, "shaders_D3D/M2.hlsl", "VS_main_Inst");
	//vertexShader->LoadInputLayoutFromReflector();
	std::vector<SCustomInputElement> elements;
	elements.push_back({ 0, 0,  ECustomVertexElementType::FLOAT3, ECustomVertexElementUsage::POSITION, 0 });
	elements.push_back({ 0, 12,  ECustomVertexElementType::FLOAT4, ECustomVertexElementUsage::BLENDWEIGHT, 0 });
	elements.push_back({ 0, 28,  ECustomVertexElementType::FLOAT4, ECustomVertexElementUsage::BLENDINDICES, 0 });
	elements.push_back({ 0, 44, ECustomVertexElementType::FLOAT3, ECustomVertexElementUsage::NORMAL, 0 });
	elements.push_back({ 0, 56, ECustomVertexElementType::FLOAT2, ECustomVertexElementUsage::TEXCOORD, 0 });
	elements.push_back({ 0, 68, ECustomVertexElementType::FLOAT2, ECustomVertexElementUsage::TEXCOORD, 1 });
	vertexShader->LoadInputLayoutFromCustomElements(elements);
	GetPipeline().SetShader(EShaderType::VertexShader, vertexShader);
	

	std::shared_ptr<IShader> pixelShader = GetRenderDevice().GetObjectsFactory().LoadShader(EShaderType::PixelShader, "shaders_D3D/M2.hlsl", "PS_main");
	GetPipeline().SetShader(EShaderType::PixelShader, pixelShader);

	GetPipeline().GetBlendState()->SetBlendMode(alphaBlending);
	GetPipeline().GetDepthStencilState()->SetDepthMode(enableDepthWrites);
	GetPipeline().GetRasterizerState()->SetCullMode(IRasterizerState::CullMode::None);
	GetPipeline().GetRasterizerState()->SetFillMode(IRasterizerState::FillMode::Solid, IRasterizerState::FillMode::Solid);
	
	std::shared_ptr<ISamplerState> sampler = GetRenderDevice().GetObjectsFactory().CreateSamplerState();
	sampler->SetFilter(ISamplerState::MinFilter::MinLinear, ISamplerState::MagFilter::MagLinear, ISamplerState::MipFilter::MipLinear);
	GetPipeline().SetSampler(0, sampler);
	GetPipeline().SetSampler(1, sampler);

	m_ShaderInstancesBufferParameter = &vertexShader->GetShaderParameterByName("Instances");
	_ASSERT(m_ShaderInstancesBufferParameter->IsValid());

	m_ShaderM2GeometryParameter = &vertexShader->GetShaderParameterByName("M2Geometry");
	_ASSERT(m_ShaderM2GeometryParameter->IsValid());

	m_ShaderM2GeometryBonesParameter = &vertexShader->GetShaderParameterByName("Bones");
	_ASSERT(m_ShaderM2GeometryBonesParameter->IsValid());

	return shared_from_this();
}
