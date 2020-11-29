#include "stdafx.h"

// General
#include "PassForward_DoRenderScene.h"

// Additional
#include "Materials/MaterialModel.h"

CPassForward_DoRenderScene::CPassForward_DoRenderScene(IRenderDevice& RenderDevice, IScene& Scene)
	: Base3DPass(Scene)
{
	
}

CPassForward_DoRenderScene::~CPassForward_DoRenderScene()
{}


IShaderParameter * CPassForward_DoRenderScene::GetLightsShaderParameter() const
{
	return m_ShaderLightsBufferParameter;
}



//
// IRenderPassPipelined
//
std::shared_ptr<IRenderPassPipelined> CPassForward_DoRenderScene::ConfigurePipeline(std::shared_ptr<IRenderTarget> RenderTarget)
{
	__super::ConfigurePipeline(RenderTarget);

	std::shared_ptr<IShader> vertexShader;
	std::shared_ptr<IShader> pixelShader;

	if (GetRenderDevice().GetDeviceType() == RenderDeviceType::RenderDeviceType_DirectX11)
	{
		vertexShader = GetRenderDevice().GetObjectsFactory().LoadShader(EShaderType::VertexShader, "3D/ModelVS.hlsl", "VS_PTN", { { "SKELETON_ANIMATION", "1" } });
		pixelShader = GetRenderDevice().GetObjectsFactory().LoadShader(EShaderType::PixelShader, "3D/Model_Forward.hlsl", "PS_main");
	}
	vertexShader->LoadInputLayoutFromReflector();

	/*std::vector<SCustomInputElement> customElements;
	customElements.push_back({ 0,  0, ECustomVertexElementType::FLOAT3, ECustomVertexElementUsage::POSITION,     0 });
	customElements.push_back({ 0, 12, ECustomVertexElementType::FLOAT2, ECustomVertexElementUsage::TEXCOORD,     0 });
	customElements.push_back({ 0, 20, ECustomVertexElementType::FLOAT3, ECustomVertexElementUsage::NORMAL,       0 });
	customElements.push_back({ 0, 32, ECustomVertexElementType::FLOAT4, ECustomVertexElementUsage::BLENDWEIGHT,  0 });
	customElements.push_back({ 0, 48, ECustomVertexElementType::UINT4,  ECustomVertexElementUsage::BLENDINDICES, 0 });
	vertexShader->LoadInputLayoutFromCustomElements(customElements);*/

	// PIPELINES
	GetPipeline().GetRasterizerState()->SetCullMode(IRasterizerState::CullMode::None);
	GetPipeline().SetShader(EShaderType::VertexShader, vertexShader);
	GetPipeline().SetShader(EShaderType::PixelShader, pixelShader);

	m_ShaderBonesBufferParameter = &vertexShader->GetShaderParameterByName("Bones");
	//_ASSERT(m_ShaderBonesBufferParameter->IsValid());

	m_ShaderLightsBufferParameter = &pixelShader->GetShaderParameterByName("LightsVS");
	//_ASSERT(m_ShaderLightsBufferParameter->IsValid());

	m_ShaderInstancesBufferParameter = &vertexShader->GetShaderParameterByName("Instances");
	//_ASSERT(m_ShaderInstancesBufferParameter->IsValid());

	return shared_from_this();
}



//
// IVisitor
//
EVisitResult CPassForward_DoRenderScene::Visit(const ISceneNode * SceneNode)
{
	if (SceneNode->GetClass() == cSceneNode3D || SceneNode->GetClass() == cSceneNodeRTSUnit || SceneNode->GetClass() == cSceneNodeRTSBullet)
	{
		auto modelsComponent = SceneNode->GetComponentT<IModelsComponent3D>();
		if (modelsComponent != nullptr)
			if (m_ShaderBonesBufferParameter->IsValid())
				m_ShaderBonesBufferParameter->Set(modelsComponent->GetBonesBuffer());

		return Base3DPass::Visit(SceneNode);
	}

	return EVisitResult::AllowVisitChilds;
}

EVisitResult CPassForward_DoRenderScene::Visit(const IModel * Model)
{
	return Base3DPass::Visit(Model);
}

EVisitResult CPassForward_DoRenderScene::Visit(const IGeometry * Geometry, const IMaterial* Material, SGeometryDrawArgs GeometryDrawArgs)
{
	const MaterialModel* objMaterial = dynamic_cast<const MaterialModel*>(Material);
	if (objMaterial == nullptr)
		return EVisitResult::Block;

	const auto& shaders = GetRenderEventArgs().PipelineState->GetShaders();
	const auto& vertexShader = shaders.at(EShaderType::VertexShader).get();

	if (m_ShaderBonesBufferParameter->IsValid())
		m_ShaderBonesBufferParameter->Bind();
	if (m_ShaderLightsBufferParameter->IsValid())
		m_ShaderLightsBufferParameter->Bind();

	objMaterial->Bind(shaders);
	Geometry->Render(vertexShader, GeometryDrawArgs);
	objMaterial->Unbind(shaders);

	if (m_ShaderLightsBufferParameter->IsValid())
		m_ShaderLightsBufferParameter->Unbind();
	if (m_ShaderBonesBufferParameter->IsValid())
		m_ShaderBonesBufferParameter->Unbind();

	return AllowAll;
}
