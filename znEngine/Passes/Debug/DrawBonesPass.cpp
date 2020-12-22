#include "stdafx.h"

// General
#include "DrawBonesPass.h"

// Include 
#include "Materials/MaterialDebug.h"

CDrawBonesPass::CDrawBonesPass(IScene& scene)
	: Base3DPass(scene)
{
}

CDrawBonesPass::~CDrawBonesPass()
{
}



//
// IRenderPassPipelined
//
std::shared_ptr<IRenderPassPipelined> CDrawBonesPass::ConfigurePipeline(std::shared_ptr<IRenderTarget> RenderTarget)
{
	__super::ConfigurePipeline(RenderTarget);

	m_ConeGeometry = GetRenderDevice().GetPrimitivesFactory().CreateCone();
	m_SphereGeometry = GetRenderDevice().GetPrimitivesFactory().CreateSphere();
	m_Material = MakeShared(MaterialDebug, GetRenderDevice());

	{
		std::shared_ptr<IShader> vertexShader = GetRenderDevice().GetObjectsFactory().LoadShader(EShaderType::VertexShader, "3D/Debug.hlsl", "VS_main");
		vertexShader->LoadInputLayoutFromReflector();
		GetPipeline().SetShader(vertexShader);
	}

	{
		std::shared_ptr<IShader> pixelShader = GetRenderDevice().GetObjectsFactory().LoadShader(EShaderType::PixelShader, "3D/Debug.hlsl", "PS_main");
		GetPipeline().SetShader(pixelShader);
	}
	

	GetPipeline().GetDepthStencilState()->SetDepthMode(disableDepthWrites);
	GetPipeline().GetRasterizerState()->SetCullMode(IRasterizerState::CullMode::None);
	GetPipeline().GetRasterizerState()->SetFillMode(IRasterizerState::FillMode::Wireframe, IRasterizerState::FillMode::Wireframe);
	
	return shared_from_this();
}



//
// IVisitor
//
EVisitResult CDrawBonesPass::Visit(const std::shared_ptr<ISceneNode>& CSceneNode)
{
	std::shared_ptr<IModelComponent> modelsComponent = CSceneNode->GetComponentT<IModelComponent>();
	if (modelsComponent == nullptr)
		return EVisitResult::AllowVisitChilds;

	auto bones = modelsComponent->GetCalculatedBones();
	if (bones.empty())
		return EVisitResult::AllowVisitChilds;

	/*std::vector<glm::mat4> bonesVisualMatrices;
	for (const auto& b : bones)
	{
		const glm::mat4& rootBoneMatrix = modelsComponent->GetModel()->GetSkeleton()->GetRootBone()->GetLocalMatrix();
		glm::mat4 resultBoneMatrix = CSceneNode->GetWorldTransfom() * rootBoneMatrix * b.BoneMatrix;
		bonesVisualMatrices.push_back(resultBoneMatrix);
	}*/


	m_Material->Bind(GetRenderEventArgs().PipelineState->GetPixelShaderPtr());

	std::vector<glm::vec3> lines;

	for (const auto& b : bones)
	{
		if (false == b.IsCalculated)
			continue;

		const glm::mat4& rootBoneMatrix = modelsComponent->GetModel()->GetSkeleton()->GetRootBone()->GetLocalMatrix();
		glm::mat4 resultBoneMatrix = CSceneNode->GetWorldTransfom() /** rootBoneMatrix*/ * b.BoneMatrix;

		if (false == b.Bone->IsRootBone())
		{
			lines.push_back(resultBoneMatrix * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));

			const auto& resultParentBoneMatrix = CSceneNode->GetWorldTransfom() /** rootBoneMatrix*/ * bones[b.Bone->GetParentIndex()].BoneMatrix;
			lines.push_back(resultParentBoneMatrix * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
		}

		BindPerObjectData(PerObject(resultBoneMatrix));
		//m_SphereGeometry->Render(GetRenderEventArgs().PipelineState->GetVertexShaderPtr());
		m_ConeGeometry->Render(GetRenderEventArgs().PipelineState->GetVertexShaderPtr());
	}

	if (lines.size() >= 2)
	{
		BindPerObjectData(PerObject());
		auto linesGeom = GetRenderDevice().GetPrimitivesFactory().CreateLines2(lines);
		linesGeom->Render(GetRenderEventArgs().PipelineState->GetVertexShaderPtr());
	}

	m_Material->Unbind(GetRenderEventArgs().PipelineState->GetPixelShaderPtr());

	return EVisitResult::AllowAll;
}

EVisitResult CDrawBonesPass::Visit(const std::shared_ptr<IModel>& Model)
{
	return EVisitResult::Block;
}

EVisitResult CDrawBonesPass::Visit(const std::shared_ptr<IGeometry>& Geometry, const std::shared_ptr<IMaterial>& Material, SGeometryDrawArgs GeometryDrawArgs)
{
	return EVisitResult::Block;
}
