#include <stdafx.h>

// General
#include "PostprocessPass.h"

PostprocessPass::PostprocessPass(std::shared_ptr<Scene3D> scene, std::shared_ptr<PipelineState> pipeline, const glm::mat4& projectionMatrix, std::shared_ptr<Texture> texture)
	: base(scene, pipeline)
	, m_ProjectionMatrix(projectionMatrix)
	, m_Texture(texture)
{}

void PostprocessPass::Render(Render3DEventArgs& e)
{
	PerObject perObjectData;
	perObjectData.ModelView = glm::mat4(1.0f); // Identity
	perObjectData.ModelViewProjection = m_ProjectionMatrix;

	SetPerObjectConstantBufferData(perObjectData);

	/*std::shared_ptr<Shader> pShader = e.PipelineState->GetShader(Shader::PixelShader);

	if (m_Texture)
	{
		// Bind the texture to be used as the source for the post process pass to slot 0 of the pixel shader.
		m_Texture->Bind(0, pShader, ShaderParameter::Type::Texture);
	}

	base::Render(e);

	if (m_Texture)
	{
		m_Texture->UnBind(0, pShader, ShaderParameter::Type::Texture);
	}*/
}

bool PostprocessPass::Visit(SceneNode3D& node)
{
	// Do nothing in this case
	return false;
}
