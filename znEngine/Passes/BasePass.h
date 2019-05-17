#pragma once

#include "AbstractPass.h"
#include "SceneFunctional/3D/Scene3D.h"

// Base pass provides implementations for functions used by most passes.
class BasePass : public AbstractPass
{
    typedef AbstractPass base;
public:
	BasePass();
	BasePass(std::shared_ptr<Scene3D> scene, std::shared_ptr<PipelineState> pipeline);
	virtual ~BasePass();

	// Render the pass. This should only be called by the RenderTechnique.
	virtual void PreRender(Render3DEventArgs& e);
	virtual void Render(Render3DEventArgs& e);
	virtual void PostRender(Render3DEventArgs& e);

	// Inherited from Visitor
	virtual bool Visit(std::shared_ptr<SceneNode3D> node) override;
	virtual bool Visit(std::shared_ptr<IMesh> Mesh, UINT IndexStartLocation = 0, UINT IndexCnt = 0, UINT VertexStartLocation = 0, UINT VertexCnt = 0) override;
	virtual bool Visit(std::shared_ptr<CLight3D> light) override;

	// Update viewport (need for texture resizing)
	virtual void UpdateViewport(Viewport _viewport);

	void SetRenderEventArgs(Render3DEventArgs& e) override;
	Render3DEventArgs& GetRenderEventArgs() const override;

	std::shared_ptr<IRenderDevice> GetRenderDevice() const;
	std::shared_ptr<PipelineState> GetPipelineState() const;

private:
	Render3DEventArgs* m_pRenderEventArgs;

	std::shared_ptr<PipelineState>  m_Pipeline;
	std::shared_ptr<Scene3D>        m_Scene;
	std::weak_ptr<IRenderDevice>    m_RenderDevice;
};