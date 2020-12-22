#include "stdafx.h"

// General
#include "DrawNormals.h"

// Additional
#include "Materials/MaterialModel.h"

CPassDrawNormals::CPassDrawNormals(IRenderDevice& RenderDevice, IScene& Scene)
	: Base3DPass(Scene)
{
	
}

CPassDrawNormals::~CPassDrawNormals()
{}




//
// IRenderPassPipelined
//
std::shared_ptr<IRenderPassPipelined> CPassDrawNormals::ConfigurePipeline(std::shared_ptr<IRenderTarget> RenderTarget)
{
	__super::ConfigurePipeline(RenderTarget);

	// Vertex shader
	{
		std::shared_ptr<IShader> vertexShader = GetRenderDevice().GetObjectsFactory().LoadShader(EShaderType::VertexShader, "3D/ModelVS.hlsl", "VS_PTN", { { "SKELETON_ANIMATION", "1" } });
		vertexShader->LoadInputLayoutFromReflector();

		m_ShaderBonesBufferParameter = vertexShader->GetShaderParameterByName("Bones");

		GetPipeline().SetShader(vertexShader);
	}

	// Geometry shader
	{
		std::shared_ptr<IShader> geomShader = GetRenderDevice().GetObjectsFactory().LoadShader(EShaderType::GeometryShader, "3D/DrawNormals.hlsl", "GS_Main");
		GetPipeline().SetShader(geomShader);
	}

	// Pixel shader
	{
		std::shared_ptr<IShader> pixelShader = GetRenderDevice().GetObjectsFactory().LoadShader(EShaderType::PixelShader, "3D/DrawNormals.hlsl", "PS_main");
		GetPipeline().SetShader(pixelShader);
	}

	GetPipeline().GetDepthStencilState()->SetDepthMode(enableTestDisableWrites);
	GetPipeline().GetRasterizerState()->SetCullMode(IRasterizerState::CullMode::None);

	return shared_from_this();
}



//
// IVisitor
//
EVisitResult CPassDrawNormals::Visit(const std::shared_ptr<ISceneNode>& SceneNode)
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

EVisitResult CPassDrawNormals::Visit(const std::shared_ptr<IModel>& Model)
{
	return Base3DPass::Visit(Model);
}

EVisitResult CPassDrawNormals::Visit(const std::shared_ptr<IGeometry>& Geometry, const std::shared_ptr<IMaterial>& Material, SGeometryDrawArgs GeometryDrawArgs)
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
