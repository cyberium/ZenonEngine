#include "stdafx.h"

IBlendState::BlendMode RenderPass::alphaBlending = IBlendState::BlendMode(true, false, IBlendState::BlendFactor::SrcAlpha, IBlendState::BlendFactor::OneMinusSrcAlpha, IBlendState::BlendOperation::Add, IBlendState::BlendFactor::SrcAlpha, IBlendState::BlendFactor::OneMinusSrcAlpha);
IBlendState::BlendMode RenderPass::additiveBlending = IBlendState::BlendMode(true, false, IBlendState::BlendFactor::One, IBlendState::BlendFactor::One);
IBlendState::BlendMode RenderPass::disableBlending = IBlendState::BlendMode(false);
IDepthStencilState::DepthMode RenderPass::enableDepthWrites = IDepthStencilState::DepthMode(true, IDepthStencilState::DepthWrite::Enable);
IDepthStencilState::DepthMode RenderPass::enableTestDisableWrites = IDepthStencilState::DepthMode(true, IDepthStencilState::DepthWrite::Disable);
IDepthStencilState::DepthMode RenderPass::disableDepthWrites = IDepthStencilState::DepthMode(false, IDepthStencilState::DepthWrite::Disable);

// General
#include "RenderPass_M2.h"

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

CRenderPass_M2::CRenderPass_M2(IRenderDevice& RenderDevice, const std::shared_ptr<IRenderPassCreateTypelessList>& SceneNodeListPass, bool OpaqueDraw)
	: RenderPassPipelined(RenderDevice)
	, m_SceneCreateTypelessListPass(SceneNodeListPass)
	, m_CurrentM2Model(nullptr)
	, m_OpaqueDraw(OpaqueDraw)
{
	m_M2PerObjectConstantBuffer = GetRenderDevice().GetObjectsFactory().CreateConstantBuffer(nullptr, sizeof(M2PerObject));
}

CRenderPass_M2::~CRenderPass_M2()
{}



//
// CRenderPass_M2
//
void CRenderPass_M2::DoRenderM2Model(const CM2_Base_Instance* M2SceneNode, const CM2_Skin* M2Model, bool OpaqueDraw, UINT InstancesCnt)
{
	const ShaderMap& shaders = GetPipeline().GetShaders();
	const IShader* vertexShader = shaders.at(EShaderType::VertexShader).get();

	for (const auto& it : M2Model->Get_Geom_Bathes_Map())
	{
		const auto& geom = M2Model->GetSections()[it.first];

		uint32 meshPartID = geom->getProto().meshPartID;
		if (false == M2SceneNode->isMeshEnabled(meshPartID))
			continue;


		geom->UpdateGeometryProps(GetRenderEventArgs(), M2SceneNode);

		m_ShaderM2GeometryParameter->SetConstantBuffer(geom->GetGeometryPropsBuffer());
		m_ShaderM2GeometryParameter->Bind();
		
		m_ShaderM2GeometryBonesParameter->SetStructuredBuffer(geom->GetGeometryBonesBuffer());
		m_ShaderM2GeometryBonesParameter->Bind();
		{
			std::shared_ptr<IGeometryInternal> geomInternal = std::dynamic_pointer_cast<IGeometryInternal>(geom);
			geomInternal->Render_BindAllBuffers(vertexShader);


			for (const auto& mat : it.second)
			{
				bool isOpaqueGeom = mat->GetM2Material()->getBlendMode() == 0 || mat->GetM2Material()->getBlendMode() == 1;
				if (OpaqueDraw)
				{
					if (! isOpaqueGeom)
						continue;
				}
				else
				{
					if (isOpaqueGeom)
						continue;
				}

				mat->GetM2Material()->GetBlendState()->Bind();
				mat->GetM2Material()->GetDepthStencilState()->Bind();
				mat->GetM2Material()->GetRasterizerState()->Bind();

				mat->UpdateMaterialProps(GetRenderEventArgs(), M2SceneNode);
				mat->Bind(shaders);
				{
					SGeometryDrawArgs geometryDrawArgs = { 0 };
					//geometryDrawArgs.IndexStartLocation = geom->getProto().indexStart;
					geometryDrawArgs.IndexCnt = geom->getProto().indexCount;
					//geometryDrawArgs.VertexStartLocation = geom->getProto().vertexStart;
					geometryDrawArgs.VertexCnt = geom->getProto().vertexCount;
					geometryDrawArgs.InstanceCnt = InstancesCnt;
					geomInternal->Render_Draw(geometryDrawArgs);
				}
				mat->Unbind(shaders);
			}

			geomInternal->Render_UnbindAllBuffers(vertexShader);
		}
		m_ShaderM2GeometryBonesParameter->Unbind();

		m_ShaderM2GeometryParameter->Unbind();
	}
}

void CRenderPass_M2::Render(RenderEventArgs & e)
{
	for (const auto& it : m_SceneCreateTypelessListPass->GetModelsList())
	{
		if (it.SceneNode->GetClass() == cSceneNodeM2)
		{
			DoRenderSceneNode(it.SceneNode);
			DoRenderModel(it.Model);
		}
	}
}



//
// IRenderPassPipelined
//
std::shared_ptr<IRenderPassPipelined> CRenderPass_M2::ConfigurePipeline(std::shared_ptr<IRenderTarget> RenderTarget, const Viewport * Viewport)
{
	__super::ConfigurePipeline(RenderTarget, Viewport);

	// CreateShaders
	std::shared_ptr<IShader> vertexShader = GetRenderDevice().GetObjectsFactory().LoadShader(EShaderType::VertexShader, "IDB_M2_SHADER", "VS_main");

	//vertexShader->LoadInputLayoutFromReflector();

	//vertexShader->LoadInputLayoutFromReflector();
	std::vector<SCustomInputElement> elements;
	elements.push_back({ 0, 0,  ECustomVertexElementType::FLOAT3, ECustomVertexElementUsage::POSITION, 0 });
	elements.push_back({ 0, 12,  ECustomVertexElementType::FLOAT4, ECustomVertexElementUsage::BLENDWEIGHT, 0 });
	elements.push_back({ 0, 28,  ECustomVertexElementType::UINT4, ECustomVertexElementUsage::BLENDINDICES, 0 });
	elements.push_back({ 0, 44, ECustomVertexElementType::FLOAT3, ECustomVertexElementUsage::NORMAL, 0 });
	elements.push_back({ 0, 56, ECustomVertexElementType::FLOAT2, ECustomVertexElementUsage::TEXCOORD, 0 });
	elements.push_back({ 0, 68, ECustomVertexElementType::FLOAT2, ECustomVertexElementUsage::TEXCOORD, 1 });
	vertexShader->LoadInputLayoutFromCustomElements(elements);
	

	GetPipeline().SetShader(EShaderType::VertexShader, vertexShader);

	std::shared_ptr<IShader> pixelShader = GetRenderDevice().GetObjectsFactory().LoadShader(EShaderType::PixelShader, "IDB_M2_SHADER", "PS_main");
	GetPipeline().SetShader(EShaderType::PixelShader, pixelShader);

	GetPipeline().GetBlendState()->SetBlendMode(disableBlending);
	GetPipeline().GetDepthStencilState()->SetDepthMode(enableDepthWrites);
	GetPipeline().GetRasterizerState()->SetCullMode(IRasterizerState::CullMode::Back);
	GetPipeline().GetRasterizerState()->SetFillMode(IRasterizerState::FillMode::Solid, IRasterizerState::FillMode::Solid);

	std::shared_ptr<ISamplerState> sampler = GetRenderDevice().GetObjectsFactory().CreateSamplerState();
	sampler->SetFilter(ISamplerState::MinFilter::MinLinear, ISamplerState::MagFilter::MagLinear, ISamplerState::MipFilter::MipLinear);
	GetPipeline().SetSampler(0, sampler);
	GetPipeline().SetSampler(1, sampler);

	m_ShaderM2PerObjectParameter = &vertexShader->GetShaderParameterByName("M2PerObject");
	_ASSERT(m_ShaderM2PerObjectParameter->IsValid());

	m_ShaderM2GeometryParameter = &vertexShader->GetShaderParameterByName("M2Geometry");
	_ASSERT(m_ShaderM2GeometryParameter->IsValid());

	m_ShaderM2GeometryBonesParameter = &vertexShader->GetShaderParameterByName("Bones");
	_ASSERT(m_ShaderM2GeometryBonesParameter->IsValid());

	return shared_from_this();
}



//
// IVisitor
//
void CRenderPass_M2::DoRenderSceneNode(const ISceneNode * SceneNode)
{
	if (auto m2Instance = dynamic_cast<const CM2_Base_Instance*>(SceneNode))
	{
		m_CurrentM2Model = m2Instance;

		M2PerObject perObject(m2Instance->GetWorldTransfom(), m2Instance->getColor());
		m_M2PerObjectConstantBuffer->Set(perObject);

		if (m_ShaderM2PerObjectParameter->IsValid() && m_M2PerObjectConstantBuffer != nullptr)
		{
			m_ShaderM2PerObjectParameter->SetConstantBuffer(m_M2PerObjectConstantBuffer);
			m_ShaderM2PerObjectParameter->Bind();
		}
	}
}

void CRenderPass_M2::DoRenderModel(const IModel * Model)
{
	if (auto m2Skin = dynamic_cast<const CM2_Skin*>(Model))
	{
		DoRenderM2Model(m_CurrentM2Model, m2Skin, m_OpaqueDraw);
	}
}
