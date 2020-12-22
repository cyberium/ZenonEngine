#include "stdafx.h"

// General
#include "Postprocess_RenderGlowSceneObjects.h"

// Additional
#include "Materials/MaterialModel.h"

CPassPostprocess_RenderGlowSceneObjects::CPassPostprocess_RenderGlowSceneObjects(IRenderDevice& RenderDevice, IScene& Scene)
	: Base3DPass(Scene)
{
	
}

CPassPostprocess_RenderGlowSceneObjects::~CPassPostprocess_RenderGlowSceneObjects()
{}


//
// IRenderPassPipelined
//
std::shared_ptr<IRenderPassPipelined> CPassPostprocess_RenderGlowSceneObjects::ConfigurePipeline(std::shared_ptr<IRenderTarget> RenderTarget)
{
	__super::ConfigurePipeline(RenderTarget);


	// Vertex shader
	{
		std::shared_ptr<IShader> vertexShader = GetRenderDevice().GetObjectsFactory().LoadShader(EShaderType::VertexShader, "3D/ModelVS.hlsl", "VS_PTN", { { "SKELETON_ANIMATION", "1" } });
		vertexShader->LoadInputLayoutFromReflector();
		/*std::vector<SCustomInputElement> customElements;
		customElements.push_back({ 0,  0, ECustomVertexElementType::FLOAT3, ECustomVertexElementUsage::POSITION,     0 });
		customElements.push_back({ 0, 12, ECustomVertexElementType::FLOAT2, ECustomVertexElementUsage::TEXCOORD,     0 });
		customElements.push_back({ 0, 20, ECustomVertexElementType::FLOAT3, ECustomVertexElementUsage::NORMAL,       0 });
		customElements.push_back({ 0, 32, ECustomVertexElementType::FLOAT4, ECustomVertexElementUsage::BLENDWEIGHT,  0 });
		customElements.push_back({ 0, 48, ECustomVertexElementType::UINT4,  ECustomVertexElementUsage::BLENDINDICES, 0 });
		vertexShader->LoadInputLayoutFromCustomElements(customElements);*/

		m_ShaderBonesBufferParameter = vertexShader->GetShaderParameterByName("Bones");
		_ASSERT(m_ShaderBonesBufferParameter);

		GetPipeline().SetShader(vertexShader);
	}


	// Pixel shader
	{
		std::shared_ptr<IShader> pixelShader = GetRenderDevice().GetObjectsFactory().LoadShader(EShaderType::PixelShader, "3D/PostprocessEmissiveObjects.hlsl", "PS_main");
		GetPipeline().SetShader(pixelShader);
	}

	//GetPipeline().GetBlendState()->SetBlendMode(//alphaBlending
	//	IBlendState::BlendMode(true, false,
	//		IBlendState::BlendFactor::SrcAlpha, IBlendState::BlendFactor::OneMinusSrcAlpha,
	//		IBlendState::BlendOperation::Add,
	//		IBlendState::BlendFactor::SrcAlpha, IBlendState::BlendFactor::OneMinusSrcAlpha)
	//);
	//GetPipeline().GetDepthStencilState()->SetDepthMode(enableTestDisableWrites);

	return shared_from_this();
}



//
// IVisitor
//
EVisitResult CPassPostprocess_RenderGlowSceneObjects::Visit(const std::shared_ptr<ISceneNode>& SceneNode)
{
	// TODO Here?
	//if (auto colliderComponent = SceneNode->GetComponentT<IColliderComponent>())
	//	if (colliderComponent->IsCulled(GetRenderEventArgs().CameraForCulling))
	//		return EVisitResult::Block;

	if (SceneNode->GetClass() == cSceneNode3D || SceneNode->GetClass() == cSceneNodeRTSUnit || SceneNode->GetClass() == cSceneNodeRTSBullet)
	{
		auto modelsComponent = SceneNode->GetComponentT<IModelComponent>();
		if (modelsComponent != nullptr)
			if (m_ShaderBonesBufferParameter)
				m_ShaderBonesBufferParameter->Set(modelsComponent->GetBonesSkinBuffer());

		return Base3DPass::Visit(SceneNode);
	}

	return EVisitResult::AllowVisitChilds;
}

EVisitResult CPassPostprocess_RenderGlowSceneObjects::Visit(const std::shared_ptr<IModel>& Model)
{
	return Base3DPass::Visit(Model);
}

EVisitResult CPassPostprocess_RenderGlowSceneObjects::Visit(const std::shared_ptr<IGeometry>& Geometry, const std::shared_ptr<IMaterial>& Material, SGeometryDrawArgs GeometryDrawArgs)
{
	auto objMaterial = std::dynamic_pointer_cast<MaterialModel>(Material);
	if (objMaterial == nullptr)
		return EVisitResult::Block;

	if (m_ShaderBonesBufferParameter)
		m_ShaderBonesBufferParameter->Bind();

	objMaterial->Bind(GetRenderEventArgs().PipelineState->GetPixelShaderPtr());
	Geometry->Render(GetRenderEventArgs().PipelineState->GetVertexShaderPtr(), GeometryDrawArgs);
	objMaterial->Unbind(GetRenderEventArgs().PipelineState->GetPixelShaderPtr());

	if (m_ShaderBonesBufferParameter)
		m_ShaderBonesBufferParameter->Unbind();

	return AllowAll;
}
