#pragma once

#include "RenderUIPass.h"

#include "../UIScene.h"
#include "../UIBaseNode.h"

// Base pass provides implementations for functions used by most passes.
class BaseUIPass : public IRenderUIPass
{
public:
	BaseUIPass();
	BaseUIPass(std::shared_ptr<CUIScene> uiScene, std::shared_ptr<PipelineState> pipeline);
	virtual ~BaseUIPass();

	// Enable or disable the pass. If a pass is disabled, the technique will skip it.
	virtual void SetEnabled(bool enabled);
	virtual bool IsEnabled() const;

	// Render the pass. This should only be called by the RenderTechnique.
	virtual void RenderUI(RenderUIEventArgs& e);

	// Inherited from Visitor
	virtual bool Visit(SceneNode3D& node3D);
	virtual bool Visit(CUIBaseNode& nodeUI);
	virtual bool Visit(IMesh& mesh, UINT indexStartLocation = 0, UINT indexCnt = 0, UINT vertexStartLocation = 0, UINT vertexCnt = 0);
	virtual bool Visit(CLight3D& light);

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

	void BindPerObjectConstantBuffer(std::shared_ptr<Shader> shader);

private:
	bool m_Enabled;
	
	RenderUIEventArgs*             m_pRenderEventArgs;

	std::shared_ptr<PipelineState> m_Pipeline;
	std::shared_ptr<CUIScene>      m_UIScene;
	std::weak_ptr<IRenderDevice>   m_RenderDevice;
};