#pragma once

class ZN_API MeshDX11 
	: public MeshBase
{
public:
	MeshDX11(IRenderDeviceDX11& RenderDeviceDX11);
	virtual	~MeshDX11();

	virtual void SetPrimitiveTopology(PrimitiveTopology _topology) override;

	virtual bool Render(const RenderEventArgs& renderArgs, const std::shared_ptr<IConstantBuffer>& perObject, const SGeometryPartParams& GeometryPartParams) override final;
};