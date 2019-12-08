#pragma once

class OW_ENGINE_API ClearRenderTargetPass : public AbstractPass
{
public:
	ClearRenderTargetPass
	(
		std::shared_ptr<IRenderDevice> RenderDevice, 
		std::shared_ptr<IRenderTarget> renderTarget,
		ClearFlags clearFlags = ClearFlags::All,
		const glm::vec4& color = glm::vec4(0),
		float depth = 1.0f,
		uint8_t stencil = 0
	);
	ClearRenderTargetPass
	(
		std::shared_ptr<IRenderDevice> RenderDevice, 
		std::shared_ptr<ITexture> texture,
		ClearFlags clearFlags = ClearFlags::All,
		const glm::vec4& color = glm::vec4(0),
		float depth = 1.0f,
		uint8_t stencil = 0
	);
	virtual ~ClearRenderTargetPass();

	virtual void Render(RenderEventArgs& e) override;

private:
	std::shared_ptr<IRenderTarget> m_RenderTarget;
	std::shared_ptr<ITexture> m_Texture;
	ClearFlags m_ClearFlags;
	glm::vec4 m_ClearColor;
	float m_ClearDepth;
	uint8_t m_ClearStencil;
};
