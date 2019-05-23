#pragma once

class AbstractPass : public IRenderPass
{
public:
	                                                AbstractPass();
                                                    AbstractPass(std::shared_ptr<PipelineState> Pipeline);
	virtual                                         ~AbstractPass();

	virtual void                                    SetEnabled(bool enabled);
	virtual bool                                    IsEnabled() const;

	virtual void                                    PreRender(RenderEventArgs& e);
	virtual void                                    Render(RenderEventArgs& e) = 0;
	virtual void                                    PostRender(RenderEventArgs& e);

    virtual void                                    UpdateViewport(Viewport _viewport);

	// IVisitor
	virtual bool                                    Visit(std::shared_ptr<SceneNode3D> node) override;
	virtual bool                                    Visit(std::shared_ptr<CUIBaseNode> nodeUI) override;
	virtual bool                                    Visit(std::shared_ptr<IMesh> Mesh, UINT IndexStartLocation = 0, UINT IndexCnt = 0, UINT VertexStartLocation = 0, UINT VertexCnt = 0) override;
	virtual bool                                    Visit(std::shared_ptr<CLight3D> light) override;

	virtual void                                    SetRenderEventArgs(RenderEventArgs* e);
	virtual RenderEventArgs*                        GetRenderEventArgs() const;

protected: // PerObject functional
	__declspec(align(16)) struct PerObject
	{
		glm::mat4 ModelViewProjection;
		glm::mat4 ModelView;
		glm::mat4 Model;

		glm::mat4 View;
		glm::mat4 Projection;
	};
	PerObject*                                      m_PerObjectData;
	std::shared_ptr<ConstantBuffer>                 m_PerObjectConstantBuffer;

	void                                            SetPerObjectConstantBufferData(PerObject& perObjectData);
	std::shared_ptr<ConstantBuffer>                 GetPerObjectConstantBuffer() const;
    void                                            BindPerObjectConstantBuffer(std::shared_ptr<Shader> shader);

protected:
    std::shared_ptr<PipelineState>                  GetPipelineState() const;
    std::shared_ptr<IRenderDevice>                  GetRenderDevice() const;

private:
	bool                                            m_Enabled;

    RenderEventArgs*                                m_RenderEventArgs;

    std::shared_ptr<PipelineState>                  m_Pipeline;
    std::weak_ptr<IRenderDevice>                    m_RenderDevice;
};