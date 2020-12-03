#pragma once

class CDrawWaypointsPass
	: public Base3DPass
{
public:
	CDrawWaypointsPass(IRenderDevice& RenderDevice, IScene& Scene);
	virtual ~CDrawWaypointsPass();

	void SetNeedRefresh();

	// IRenderPassPipelined
	std::shared_ptr<IRenderPassPipelined> ConfigurePipeline(std::shared_ptr<IRenderTarget> RenderTarget) override final;

	// IVisitor
	EVisitResult Visit(const std::shared_ptr<ISceneNode>& node) override final;
	EVisitResult Visit(const std::shared_ptr<IModel>& Model) override final;

protected:
	bool                               m_IsDirty;

	std::shared_ptr<MaterialDebug>     m_MaterialDebug;
	std::shared_ptr<IGeometry>         m_WaypointGeometry;

	std::shared_ptr<IGeometry>         m_PointBox;
	IShaderParameter*                  m_ShaderInstancesBufferParameter;
	std::shared_ptr<IStructuredBuffer> m_InstancesBuffer;
	size_t							   m_InstancesCnt;
};
