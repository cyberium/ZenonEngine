#pragma once

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
	PerFrame(glm::mat4 ViewMatrix, glm::mat4 ProjectionMatrix, glm::vec2 ScreenDimensions)
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

class ZN_API RenderPassPipelined
	: public IRenderPassPipelined
	, public std::enable_shared_from_this<IRenderPassPipelined>
{
public:
	RenderPassPipelined(IRenderDevice& RenderDevice);
	virtual ~RenderPassPipelined();

	// IRenderPass
	void SetEnabled(bool Value) override final;
	bool IsEnabled() const override final;
	virtual void PreRender(RenderEventArgs& e);
	virtual void Render(RenderEventArgs& e) = 0;
	virtual void PostRender(RenderEventArgs& e);
    
	// IRenderPassPipelined
	virtual std::shared_ptr<IRenderPassPipelined> ConfigurePipeline(std::shared_ptr<IRenderTarget> RenderTarget, const Viewport* Viewport) override;
	virtual IPipelineState& GetPipeline() const override;

	// IVisitor
	virtual EVisitResult Visit(const ISceneNode* node) override;
	virtual EVisitResult Visit(const IUIControl* node) override;
	virtual EVisitResult Visit(const IModel* Model) override;
	virtual EVisitResult Visit(const IGeometry* Geometry, const IMaterial* Material, SGeometryDrawArgs GeometryDrawArgs = SGeometryDrawArgs()) override;
	virtual EVisitResult Visit(const ISceneNodeComponent* Component) override;
	virtual EVisitResult Visit(const ILight3D* light) override;
	virtual EVisitResult Visit(const IParticleSystem* ParticleSystem) override;

protected:
	virtual void FillPerFrameData();

	const RenderEventArgs& GetRenderEventArgs() const;
    const IRenderDevice& GetRenderDevice() const;
	const IBaseManager& GetBaseManager() const;

protected:
	static IBlendState::BlendMode alphaBlending;
	static IBlendState::BlendMode additiveBlending;
	static IBlendState::BlendMode disableBlending;
	static IDepthStencilState::DepthMode enableDepthWrites;
	static IDepthStencilState::DepthMode enableTestDisableWrites;
	static IDepthStencilState::DepthMode disableDepthWrites;

protected:
	std::shared_ptr<IConstantBuffer> m_PerFrameConstantBuffer;
	void SetPerFrameData(const PerFrame& PerFrame);
	void BindPerFrameData(const IShader* Shader) const;

private:
	bool											m_Enabled;
	std::shared_ptr<IPipelineState>					m_Pipeline;
    const RenderEventArgs*							m_RenderEventArgs;
   
	const IBaseManager&                             m_BaseManager;
	const IRenderDevice&							m_RenderDevice;
};