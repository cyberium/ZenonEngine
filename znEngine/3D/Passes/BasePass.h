#pragma once

#include "AbstractPass.h"
#include "../Scene3D.h"

// Base pass provides implementations for functions used by most passes.
class BasePass : public AbstractPass
{
public:
	typedef AbstractPass base;

	BasePass();
	BasePass(std::shared_ptr<Scene3D> scene, std::shared_ptr<PipelineState> pipeline);
	virtual ~BasePass();

	// Render the pass. This should only be called by the RenderTechnique.
	virtual void PreRender(Render3DEventArgs& e);
	virtual void Render(Render3DEventArgs& e);
	virtual void PostRender(Render3DEventArgs& e);

	// Inherited from Visitor
	virtual bool Visit(SceneNode3D& node);
	virtual bool Visit(IMesh& mesh, UINT indexStartLocation = 0, UINT indexCnt = 0, UINT vertexStartLocation = 0, UINT vertexCnt = 0);
	virtual bool Visit(CLight3D& light);

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