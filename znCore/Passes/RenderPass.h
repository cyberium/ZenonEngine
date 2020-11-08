#pragma once

// TODO: Move me
extern IBlendState::BlendMode alphaBlending;
extern IBlendState::BlendMode additiveBlending;
extern IBlendState::BlendMode disableBlending;
extern IDepthStencilState::DepthMode enableDepthWrites;
extern IDepthStencilState::DepthMode enableTestDisableWrites;
extern IDepthStencilState::DepthMode disableDepthWrites;

struct __declspec(align(16)) ZN_API PerObject
{
	glm::mat4 Model;
};

struct __declspec(align(16)) ZN_API PerFrame
{
	PerFrame()
		: View(glm::mat4(1.0f))
		, Projection(glm::mat4(1.0f))
		, InverseView(glm::mat4(1.0f))
		, InverseProjection(glm::mat4(1.0f))
		, InverseProjectionView(glm::mat4(1.0f))
		, ScreenDimensions(glm::vec2(1.0f))
	{}
	explicit PerFrame(glm::mat4 ViewMatrix, glm::mat4 ProjectionMatrix, glm::vec2 ScreenDimensions)
		: View(ViewMatrix)
		, Projection(ProjectionMatrix)
		, ScreenDimensions(ScreenDimensions)
	{
		InverseView = glm::inverse(View);
		InverseProjection = glm::inverse(Projection);
		InverseProjectionView = glm::inverse(Projection * View);
	}

	glm::mat4 View;
	glm::mat4 Projection;
	glm::mat4 InverseView;
	glm::mat4 InverseProjection;
	glm::mat4 InverseProjectionView;
	glm::vec2 ScreenDimensions;
};


class ZN_API RenderPass 
	: virtual public IRenderPass
{
public:
	RenderPass(IRenderDevice& RenderDevice);
	virtual ~RenderPass();

	// IRenderPass
	virtual void SetEnabled(bool Value) override final;
	virtual bool IsEnabled() const override final;
	virtual void PreRender(RenderEventArgs& e) override;
	virtual void PostRender(RenderEventArgs& e) override;

protected:
	IBaseManager& GetBaseManager() const;
	IRenderDevice& GetRenderDevice() const;
	const RenderEventArgs& GetRenderEventArgs() const;

private:
	bool                                            m_Enabled;

	IBaseManager&                                   m_BaseManager;
	IRenderDevice&									m_RenderDevice;
	const RenderEventArgs*                          m_RenderEventArgs;
};