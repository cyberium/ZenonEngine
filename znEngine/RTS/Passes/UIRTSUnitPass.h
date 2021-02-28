#pragma once

class ZN_API CUIRTSUnitPass
	: public BaseUIPass
{
public:
	CUIRTSUnitPass(IRenderDevice& RenderDevice, IScene& Scene);
	virtual ~CUIRTSUnitPass();

	// IRenderPass
	void PreRender(RenderEventArgs& e);
	void Render(RenderEventArgs& e);

	// IRenderPassPipelined
	virtual std::shared_ptr<IRenderPassPipelined> ConfigurePipeline(std::shared_ptr<IRenderTarget> RenderTarget) override;

	// IVisitor
	virtual EVisitResult Visit(const std::shared_ptr<IUIControl>& node) override;
	virtual EVisitResult Visit(const std::shared_ptr<IModel>& Model) override;

private:
	std::vector<std::shared_ptr<ISceneNodeRTSUnit>> m_Units;

	std::shared_ptr<IGeometry> m_HPBarOutlineGeometry;
	std::shared_ptr<IGeometry> m_HPBarGeometry;

	std::shared_ptr<IUIControlCommonMaterial> m_HPBarOutlineMaterial;
	std::shared_ptr<IUIControlCommonMaterial> m_HPBarMaterial;
};