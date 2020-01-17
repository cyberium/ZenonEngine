#pragma once

class ZN_API RenderPass 
	: public IRenderPass
	, public Object
{
public:
	RenderPass(std::shared_ptr<IRenderDevice> RenderDevice);
	virtual ~RenderPass();

	// IRenderPass
	virtual void SetEnabled(bool Value) override final;
	virtual bool IsEnabled() const override final;
	virtual void PreRender(RenderEventArgs& e) override;
	virtual void Render(RenderEventArgs& e) = 0;
	virtual void PostRender(RenderEventArgs& e) override;

	// IVisitor
	virtual bool VisitBase(ISceneNode* node) override;
	virtual bool Visit3D(ISceneNode* node) override;
	virtual bool VisitUI(ISceneNode* node) override;
	virtual bool Visit(IMesh* Mesh, SGeometryPartParams GeometryPartParams = SGeometryPartParams()) override;
	virtual bool Visit(IGeometry* Geometry, const IMaterial* Material, SGeometryPartParams GeometryPartParams = SGeometryPartParams()) override;
	virtual bool Visit(ILightComponent3D* light) override;

protected:
	const RenderEventArgs* GetRenderEventArgs() const;
    std::shared_ptr<IRenderDevice> GetRenderDevice() const;
	const IBaseManager* GetBaseManager() const;

private:
	bool                                            m_Enabled;
    RenderEventArgs*                                m_RenderEventArgs;
    std::weak_ptr<IRenderDevice>                    m_RenderDevice;
	const IBaseManager*                             m_BaseManager;
};