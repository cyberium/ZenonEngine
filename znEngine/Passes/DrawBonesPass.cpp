#include "stdafx.h"

// General
#include "DrawBonesPass.h"

// Include 
#include "Materials/MaterialDebug.h"

CDrawBonesPass::CDrawBonesPass(IRenderDevice& RenderDevice, std::shared_ptr<IScene> scene)
	: Base3DPass(RenderDevice, scene)
{
	m_MaterialConstantBuffer = GetRenderDevice().GetObjectsFactory().CreateConstantBuffer(SMaterialDebugProperties());
}

CDrawBonesPass::~CDrawBonesPass()
{
}



//
// IRenderPassPipelined
//
std::shared_ptr<IRenderPassPipelined> CDrawBonesPass::ConfigurePipeline(std::shared_ptr<IRenderTarget> RenderTarget, const Viewport * Viewport)
{
	m_QuadGeometry = GetRenderDevice().GetPrimitivesFactory().CreateCone();
	m_SphereGeometry = GetRenderDevice().GetPrimitivesFactory().CreateSphere();

	std::shared_ptr<IShader> vertexShader;
	std::shared_ptr<IShader> pixelShader;

	if (GetRenderDevice().GetDeviceType() == RenderDeviceType::RenderDeviceType_DirectX11)
	{
		vertexShader = GetRenderDevice().GetObjectsFactory().LoadShader(EShaderType::VertexShader, "3D/Debug.hlsl", "VS_main");
		pixelShader = GetRenderDevice().GetObjectsFactory().LoadShader(EShaderType::PixelShader, "3D/Debug.hlsl", "PS_main");
	}
	vertexShader->LoadInputLayoutFromReflector();

	// PIPELINES
	GetPipeline().GetBlendState()->SetBlendMode(disableBlending);
	GetPipeline().GetDepthStencilState()->SetDepthMode(enableDepthWrites);
	GetPipeline().GetRasterizerState()->SetCullMode(IRasterizerState::CullMode::None);
	GetPipeline().GetRasterizerState()->SetFillMode(IRasterizerState::FillMode::Wireframe, IRasterizerState::FillMode::Solid);
	GetPipeline().GetRasterizerState()->SetMultisampleEnabled(true);
	GetPipeline().GetRasterizerState()->SetAntialiasedLineEnable(true);
	GetPipeline().SetRenderTarget(RenderTarget);
	GetPipeline().SetShader(EShaderType::VertexShader, vertexShader);
	GetPipeline().SetShader(EShaderType::PixelShader, pixelShader);

	m_MaterialParameter = &(pixelShader->GetShaderParameterByName("Material"));
	_ASSERT(m_MaterialParameter->IsValid());

	return shared_from_this();
}


glm::quat LookAt(glm::vec3 Position, glm::vec3 LookAt)
{
	glm::vec3 lookVector = LookAt;
	//assert(lookVector != Position);

	glm::vec3 direction = glm::normalize(lookVector - Position);
	float dot = glm::dot(glm::vec3(0, 1, 0), direction);
	if (glm::abs(dot - (-1.0f)) < 0.000001f) 
	{
		return glm::angleAxis(glm::degrees(glm::pi<float>()), glm::vec3(0, 1, 0));
	}
	else if (glm::abs(dot - (1.0f)) < 0.000001f) 
	{
		return glm::quat();
	}

	float angle = -glm::degrees(glm::acos(dot));
	glm::vec3 cross = glm::normalize(glm::cross(glm::vec3(0, 1, 0), direction));
	return glm::normalize(glm::angleAxis(angle, cross));
}



//
// IVisitor
//
EVisitResult CDrawBonesPass::Visit(const ISceneNode3D * SceneNode3D)
{
	const std::shared_ptr<ISkeletonComponent3D>& skeletonComponent = SceneNode3D->GetComponent<ISkeletonComponent3D>();
	if (skeletonComponent == nullptr)
		return EVisitResult::AllowVisitChilds;

	auto bones = skeletonComponent->GetBones();
	if (bones.empty())
		return EVisitResult::AllowVisitChilds;

	const IShader* pixelShader = GetPipeline().GetShaders().at(EShaderType::PixelShader).get();
	
	SMaterialDebugProperties props;
	props.DiffuseColor = glm::vec4(1.0f);
	m_MaterialConstantBuffer->Set(props);

	m_MaterialParameter->SetConstantBuffer(m_MaterialConstantBuffer);
	m_MaterialParameter->Bind();

	const IShader* vertexShader = GetPipeline().GetShaders().at(EShaderType::VertexShader).get();
	if (m_PerObjectParameter == nullptr)
		m_PerObjectParameter = &(vertexShader->GetShaderParameterByName("PerObject"));

	for (const auto& b : bones)
	{
		glm::mat4 m = b->GetPivotMatrix();
		glm::vec3 mPos = glm::vec3(m[3][0], m[3][1], m[3][2]);

		if (b->GetParentBone().lock())
		{
			glm::mat4 mP = b->GetParentBone().lock()->GetPivotMatrix();
			glm::vec3 mPPos = glm::vec3(mP[3][0], mP[3][1], mP[3][2]);

			m = glm::mat4(1.0f);
			m = glm::translate(m, mPos);

			//m = glm::rotate(m, glm::half_pi<float>(), glm::vec3(1, 0, 0));
			//m = glm::rotate(m, m_Rotate.y, glm::vec3(0, 1, 0));
			//m = glm::rotate(m, glm::half_pi<float>(), glm::vec3(0, 0, 1));

			m *= glm::toMat4(LookAt(mPos, mPPos));

			//glm::vec3 dir2 = glm::normalize(mPPos - mPos);
			//m = m * glm::lookAt(mPos, mPos + dir2, glm::vec3(0.0f, 1.0f, 0.0f));

			m = glm::scale(m, glm::vec3(2.0f, glm::length(glm::abs(mPPos - mPos)), 2.0f));
		}
		PerObject perObject;
		perObject.Model = m;// ->GetMatrix();
		m_PerObjectConstantBuffer->Set(perObject);

		if (m_PerObjectParameter->IsValid() && m_PerObjectConstantBuffer != nullptr)
		{
			m_PerObjectParameter->SetConstantBuffer(m_PerObjectConstantBuffer);
			m_PerObjectParameter->Bind();
		}

		m_QuadGeometry->Render(GetRenderEventArgs(), vertexShader);








		{
			//glm::mat4 m = b->GetPivotMatrix();
			//m = glm::scale(m, glm::vec3(4.0f));

			PerObject perObject;
			perObject.Model = b->GetMatrix();
			m_PerObjectConstantBuffer->Set(perObject);

			if (m_PerObjectParameter->IsValid() && m_PerObjectConstantBuffer != nullptr)
			{
				m_PerObjectParameter->SetConstantBuffer(m_PerObjectConstantBuffer);
				m_PerObjectParameter->Bind();
			}


			m_SphereGeometry->Render(GetRenderEventArgs(), vertexShader);
		}
	}

	return EVisitResult::AllowAll;
}

EVisitResult CDrawBonesPass::Visit(const IModel * Model)
{
	return EVisitResult::Block;
}

EVisitResult CDrawBonesPass::Visit(const IGeometry * Geometry, const IMaterial * Material, SGeometryDrawArgs GeometryDrawArgs)
{
	return EVisitResult::Block;
}
