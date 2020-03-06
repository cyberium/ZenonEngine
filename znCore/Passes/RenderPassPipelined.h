#pragma once

class ZN_API RenderPassPipelined
	: public IRenderPassPipelined
	, public Object
{
public:
	RenderPassPipelined(IRenderDevice& RenderDevice);
	virtual ~RenderPassPipelined();

	std::shared_ptr<IRenderPassPipelined> shared_from_this();
	std::weak_ptr<IRenderPassPipelined> weak_from_this();

	// IRenderPass
	void SetEnabled(bool Value) override final;
	bool IsEnabled() const override final;
	virtual void PreRender(RenderEventArgs& e);
	virtual void Render(RenderEventArgs& e) = 0;
	virtual void PostRender(RenderEventArgs& e);
    
	// IRenderPassPipelined
	virtual std::shared_ptr<IRenderPassPipelined> CreatePipeline(std::shared_ptr<IRenderTarget> RenderTarget, const Viewport* Viewport) override;
	virtual std::shared_ptr<IRenderPassPipelined> SetPipeline(std::shared_ptr<IPipelineState> Pipeline) override;
	virtual IPipelineState& GetPipeline() const override;
	virtual void UpdateViewport(const Viewport& _viewport) override;

	// IVisitor
	virtual EVisitResult Visit(const ISceneNode3D* node) override;
	virtual EVisitResult Visit(const ISceneNodeUI* node) override;
	virtual EVisitResult Visit(const IModel* Model) override;
	virtual EVisitResult Visit(const IGeometry* Geometry, const IMaterial* Material, SGeometryDrawArgs GeometryDrawArgs = SGeometryDrawArgs()) override;
	virtual EVisitResult Visit(const ISceneNodeComponent* Component) override;
	virtual EVisitResult Visit(const ILightComponent3D* light) override;

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
	static IDepthStencilState::DepthMode disableDepthWrites;

protected:
	__declspec(align(16)) struct PerFrame
	{
		glm::mat4 View;
		glm::mat4 Projection;
	};
	void SetPerFrameData(const PerFrame& PerFrame);
	void BindPerFrameDataToVertexShader(const IShader* VertexShader) const;

private:
	std::shared_ptr<IConstantBuffer> m_PerFrameConstantBuffer;

private:
	bool											m_Enabled;
	std::shared_ptr<IPipelineState>					m_Pipeline;
    const RenderEventArgs*							m_RenderEventArgs;
   
	const IBaseManager&                             m_BaseManager;
	const IRenderDevice&							m_RenderDevice;
};