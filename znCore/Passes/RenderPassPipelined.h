#pragma once

class ZN_API RenderPassPipelined
	: public IRenderPassPipelined
	, public Object
{
public:
	RenderPassPipelined(std::shared_ptr<IRenderDevice> RenderDevice);
	virtual ~RenderPassPipelined();

	// IRenderPass
	void SetEnabled(bool Value) override final;
	bool IsEnabled() const override final;
	virtual void PreRender(RenderEventArgs& e);
	virtual void Render(RenderEventArgs& e) = 0;
	virtual void PostRender(RenderEventArgs& e);
    
	// IRenderPassPipelined
	virtual void CreatePipeline(std::shared_ptr<IRenderTarget> RenderTarget, const Viewport* Viewport) override;
	virtual void SetPipeline(std::shared_ptr<IPipelineState> Pipeline) override;
	virtual std::shared_ptr<IPipelineState> GetPipeline() const override;
	virtual void UpdateViewport(const Viewport * _viewport) override;

	// IVisitor
	virtual bool VisitBase(ISceneNode* node) override;
	virtual bool Visit3D(ISceneNode* node) override;
	virtual bool VisitUI(ISceneNode* node) override;
	virtual bool Visit(IMesh* Mesh, SGeometryPartParams GeometryPartParams = SGeometryPartParams()) override;
	virtual bool Visit(IGeometry* Geometry, const IMaterial* Material, SGeometryPartParams GeometryPartParams = SGeometryPartParams()) override;
	virtual bool Visit(ILightComponent3D* light) override;

protected:
	RenderEventArgs* GetRenderEventArgs() const;
    std::shared_ptr<IRenderDevice> GetRenderDevice() const;
	const IBaseManager* GetBaseManager() const;

protected:
	static IBlendState::BlendMode alphaBlending;
	static IBlendState::BlendMode disableBlending;
	static IDepthStencilState::DepthMode enableDepthWrites;
	static IDepthStencilState::DepthMode disableDepthWrites;

private:
	bool                                            m_Enabled;
	std::shared_ptr<IPipelineState>                 m_Pipeline;
    RenderEventArgs*                                m_RenderEventArgs;
    std::weak_ptr<IRenderDevice>                    m_RenderDevice;
	const IBaseManager*                             m_BaseManager;
};