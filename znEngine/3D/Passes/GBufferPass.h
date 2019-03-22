#pragma once

#include "BasePass.h"

class GBufferPass : public BasePass
{
	typedef BasePass base;
public:
	GBufferPass(std::shared_ptr<Scene3D> scene, std::shared_ptr<PipelineState> pipeline, const glm::mat4& projectionMatrix,
		std::shared_ptr<Texture> diffuseTexture,
		std::shared_ptr<Texture> lightAccumulationTexture,
		std::shared_ptr<Texture> specularTexture,
		std::shared_ptr<Texture> normalTexture,
		std::shared_ptr<Texture> depthTexture
	);

	virtual void Render(Render3DEventArgs& e) override;

	virtual bool Visit(SceneNode3D& node);

protected:

private:
	glm::mat4 m_ProjectionMatrix;

	std::shared_ptr<Texture> m_DiffuseTexture;
	std::shared_ptr<Texture> m_LightAccumTexture;
	std::shared_ptr<Texture> m_SpecularTexture;
	std::shared_ptr<Texture> m_NormalTexture;
	std::shared_ptr<Texture> m_DepthTexture;
};