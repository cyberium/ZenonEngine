#pragma once

struct __declspec(align(16)) ZN_API PerObject
{
	glm::mat4 Model;

	explicit PerObject()
		: Model(glm::mat4(1.0f))
	{}

	explicit PerObject(const glm::vec2& Translation, float Rotation = 0.0f, const glm::vec2& Scale = glm::vec2(1.0f))
		: Model(glm::mat4(1.0f))
	{
		Model = glm::translate(Model, glm::vec3(Translation, 1.0f));
		Model = glm::rotate(Model, Rotation, glm::vec3(0.0f, 0.0f, 1.0f));
		Model = glm::scale(Model, glm::vec3(Scale, 1.0f));
	}

	explicit PerObject(const glm::vec3& Translation, const glm::vec3& Rotation = glm::vec3(0.0f), const glm::vec3& Scale = glm::vec3(1.0f))
		: Model(glm::mat4(1.0f))
	{
		Model = glm::translate(Model, Translation);
		Model = glm::rotate(Model, Rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
		Model = glm::rotate(Model, Rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
		Model = glm::rotate(Model, Rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
		Model = glm::scale(Model, Scale);
	}

	explicit PerObject(const glm::mat4& Matrix)
		: Model(Matrix)
	{}
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
	glm::vec2 __padding;
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
	static IBlendState::BlendMode alphaBlending;
	static IBlendState::BlendMode additiveBlending;
	static IBlendState::BlendMode disableBlending;
	static IDepthStencilState::DepthMode enableDepthWrites;
	static IDepthStencilState::DepthMode enableTestDisableWrites;
	static IDepthStencilState::DepthMode disableDepthWrites;

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