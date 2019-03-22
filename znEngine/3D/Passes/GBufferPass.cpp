#include <stdafx.h>

// General
#include "GBufferPass.h"

GBufferPass::GBufferPass(
	std::shared_ptr<Scene3D> scene, 
	std::shared_ptr<PipelineState> pipeline, 
	const glm::mat4& projectionMatrix,		
	std::shared_ptr<Texture> diffuseTexture,
	std::shared_ptr<Texture> lightAccumulationTexture,
	std::shared_ptr<Texture> specularTexture,
	std::shared_ptr<Texture> normalTexture,
	std::shared_ptr<Texture> depthTexture
)
	: base(scene, pipeline)
	, m_ProjectionMatrix(projectionMatrix)
	, m_DiffuseTexture(diffuseTexture)
	, m_LightAccumTexture(lightAccumulationTexture)
	, m_SpecularTexture(specularTexture)
	, m_NormalTexture(normalTexture)
	, m_DepthTexture(depthTexture)
{}

void GBufferPass::Render(Render3DEventArgs& e)
{
	PerObject perObjectData;
	perObjectData.ModelViewProjection = m_ProjectionMatrix;

	SetPerObjectConstantBufferData(perObjectData);

	std::shared_ptr<Shader> pShader = e.PipelineState->GetShader(Shader::PixelShader);

	m_DiffuseTexture->Bind(0, pShader, ShaderParameter::Type::Texture);
	m_LightAccumTexture->Bind(1, pShader, ShaderParameter::Type::Texture);
	m_SpecularTexture->Bind(2, pShader, ShaderParameter::Type::Texture);
	m_NormalTexture->Bind(3, pShader, ShaderParameter::Type::Texture);
	m_DepthTexture->Bind(4, pShader, ShaderParameter::Type::Texture);

	base::Render(e);

	m_DiffuseTexture->UnBind(0, pShader, ShaderParameter::Type::Texture);
	m_LightAccumTexture->UnBind(1, pShader, ShaderParameter::Type::Texture);
	m_SpecularTexture->UnBind(2, pShader, ShaderParameter::Type::Texture);
	m_NormalTexture->UnBind(3, pShader, ShaderParameter::Type::Texture);
	m_DepthTexture->UnBind(4, pShader, ShaderParameter::Type::Texture);
}

bool GBufferPass::Visit(SceneNode3D& node)
{
	return true;
}
