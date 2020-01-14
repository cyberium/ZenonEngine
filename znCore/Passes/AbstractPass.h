#pragma once

class ZN_API AbstractPass 
	: public IRenderPass
	, public Object
{
public:
	AbstractPass(std::shared_ptr<IRenderDevice> RenderDevice);
	AbstractPass(std::shared_ptr<IRenderDevice> RenderDevice, std::shared_ptr<IPipelineState> Pipeline);
	virtual ~AbstractPass();

	virtual void SetEnabled(bool enabled);
	virtual bool IsEnabled() const;

	virtual void PreRender(RenderEventArgs& e);
	virtual void PostRender(RenderEventArgs& e);

    virtual void UpdateViewport(const Viewport * _viewport);

	// IVisitor
	virtual bool VisitBase(ISceneNode* node) override;
	virtual bool Visit3D(ISceneNode* node) override;
	virtual bool VisitUI(ISceneNode* node) override;
	virtual bool Visit(IMesh* Mesh, SGeometryPartParams GeometryPartParams = SGeometryPartParams()) override;
	virtual bool Visit(IGeometry* Geometry, const IMaterial* Material, SGeometryPartParams GeometryPartParams = SGeometryPartParams()) override;
	virtual bool Visit(CLight3D* light) override;

	virtual RenderEventArgs* GetRenderEventArgs() const;
    std::shared_ptr<IPipelineState> GetPipelineState() const;

protected:
    std::shared_ptr<IRenderDevice> GetRenderDevice() const;
	void SetPipeline(std::shared_ptr<IPipelineState> Pipeline);

private:
	bool                                            m_Enabled;

    RenderEventArgs*                                m_RenderEventArgs;

    std::shared_ptr<IPipelineState>                 m_Pipeline;
    std::weak_ptr<IRenderDevice>                    m_RenderDevice;
};