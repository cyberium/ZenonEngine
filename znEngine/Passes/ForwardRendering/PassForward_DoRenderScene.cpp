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


void CPassForward_DoRenderScene::SetEnviorementTexture(std::shared_ptr<ITexture> Texture)
{
	GetPipeline().SetTexture(15, Texture);
}

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

		GetPipeline().SetShader(EShaderType::VertexShader, vertexShader);
	}


	// Pixel shader
	{
		std::shared_ptr<IShader> pixelShader = GetRenderDevice().GetObjectsFactory().LoadShader(EShaderType::PixelShader, "3D/Model_Forward.hlsl", "PS_main");

		m_ShaderLightsBufferParameter = pixelShader->GetShaderParameterByName("GPULightsVS");
		_ASSERT(m_ShaderLightsBufferParameter);

		GetPipeline().SetShader(EShaderType::PixelShader, pixelShader);
	}

	return shared_from_this();
}



//
// IVisitor
//
EVisitResult CPassForward_DoRenderScene::Visit(const std::shared_ptr<ISceneNode>& SceneNode)
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

EVisitResult CPassForward_DoRenderScene::Visit(const std::shared_ptr<IModel>& Model)
{
	return Base3DPass::Visit(Model);
}

EVisitResult CPassForward_DoRenderScene::Visit(const std::shared_ptr<IGeometry>& Geometry, const std::shared_ptr<IMaterial>& Material, SGeometryDrawArgs GeometryDrawArgs)
{
	auto objMaterial = std::dynamic_pointer_cast<MaterialModel>(Material);
	if (objMaterial == nullptr)
		return EVisitResult::Block;

	if (m_ShaderBonesBufferParameter)
		m_ShaderBonesBufferParameter->Bind();
	if (m_ShaderLightsBufferParameter)
		m_ShaderLightsBufferParameter->Bind();

	objMaterial->Bind(GetRenderEventArgs().PipelineState->GetPixelShaderPtr());
	Geometry->Render(GetRenderEventArgs().PipelineState->GetVertexShaderPtr(), GeometryDrawArgs);
	objMaterial->Unbind(GetRenderEventArgs().PipelineState->GetPixelShaderPtr());

	if (m_ShaderLightsBufferParameter)
		m_ShaderLightsBufferParameter->Unbind();
	if (m_ShaderBonesBufferParameter)
		m_ShaderBonesBufferParameter->Unbind();

	return AllowAll;
}
