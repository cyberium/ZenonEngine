#pragma once

class CCollectLightPass
	: public CBaseScenePass
{
public:
	CCollectLightPass(std::shared_ptr<IRenderDevice> RenderDevice, std::shared_ptr<IScene> Scene);
	virtual ~CCollectLightPass();

	// CCollectLightPass
	std::shared_ptr<IStructuredBuffer> GetLightBuffer() const;

	// IRenderPass
	virtual void PreRender(RenderEventArgs& e) override;
	virtual void PostRender(RenderEventArgs& e) override;

	// IVisitor
	virtual bool Visit(ILightComponent3D* light) override;

private:
	std::vector<SLight> m_CollectedLights;
	std::shared_ptr<IStructuredBuffer> m_LightsBuffer;
};