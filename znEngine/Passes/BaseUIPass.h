#pragma once

#include "RenderUIPass.h"

#include "SceneFunctional/UI/SceneUI.h"
#include "SceneFunctional/UI/SceneNodeUI.h"

// Base pass provides implementations for functions used by most passes.
class BaseUIPass : public IRenderUIPass
{
public:
	BaseUIPass();
	BaseUIPass(std::shared_ptr<SceneUI> uiScene, std::shared_ptr<PipelineState> pipeline);
	virtual ~BaseUIPass();

	// Enable or disable the pass. If a pass is disabled, the technique will skip it.
	virtual void SetEnabled(bool enabled);
	virtual bool IsEnabled() const;

	// Render the pass. This should only be called by the RenderTechnique.
	virtual void RenderUI(RenderUIEventArgs& e);

	// Inherited from Visitor
	virtual bool Visit(std::shared_ptr<SceneNode3D> node3D) override;
	virtual bool Visit(std::shared_ptr<CUIBaseNode> nodeUI) override;
	virtual bool Visit(std::shared_ptr<IMesh> Mesh, UINT IndexStartLocation = 0, UINT IndexCnt = 0, UINT VertexStartLocation = 0, UINT VertexCnt = 0) override;
	virtual bool Visit(std::shared_ptr<CLight3D> light) override;

	// Update viewport (need for texture resizing)
	virtual void UpdateViewport(Viewport _viewport) override final;

	// Store render event args
	virtual void SetRenderEventArgs(RenderUIEventArgs& e);
	virtual RenderUIEventArgs& GetRenderEventArgs() const;

protected:
	__declspec(align(16)) struct PerObject
	{
		glm::mat4 ModelViewProjection;
		glm::mat4 ModelView;
		glm::mat4 Model;
	};
	PerObject* m_PerObjectData;
	std::shared_ptr<ConstantBuffer> m_PerObjectConstantBuffer;

	void SetPerObjectConstantBufferData(PerObject& perObjectData);
	std::shared_ptr<ConstantBuffer> GetPerObjectConstantBuffer() const;

private:
	bool m_Enabled;
	
	RenderUIEventArgs*              m_pRenderEventArgs;

	std::shared_ptr<PipelineState>  m_Pipeline;
	std::shared_ptr<SceneUI>        m_UIScene;
	std::weak_ptr<IRenderDevice>    m_RenderDevice;
};