#include <stdafx.h>

// General
#include "PostprocessPass.h"

PostprocessPass::PostprocessPass(std::shared_ptr<Scene3D> scene, std::shared_ptr<IPipelineState> pipeline, const glm::mat4& projectionMatrix, std::shared_ptr<ITexture> texture)
	: base(scene, pipeline)
	, m_ProjectionMatrix(projectionMatrix)
	, m_Texture(texture)
{}

void PostprocessPass::Render(RenderEventArgs& e)
{
	PerObject perObjectData;
	perObjectData.ModelViewProjection = m_ProjectionMatrix;

	SetPerObjectConstantBufferData(perObjectData);

	/*std::shared_ptr<IShader> pShader = e.PipelineState->GetShader(IShader::PixelShader);

	if (m_Texture)
	{
		// Bind the texture to be used as the source for the post process pass to slot 0 of the pixel shader.
		m_Texture->Bind(0, pShader, IShaderParameter::Type::Texture);
	}

	base::Render(e);

	if (m_Texture)
	{
		m_Texture->UnBind(0, pShader, IShaderParameter::Type::Texture);
	}*/
}

bool PostprocessPass::Visit(SceneNode3D& node)
{
	// Do nothing in this case
	return false;
}
