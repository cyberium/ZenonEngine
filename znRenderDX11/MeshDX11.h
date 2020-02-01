#pragma once

class ZN_API MeshDX11 
	: public MeshBase
{
public:
	MeshDX11(IRenderDeviceDX11& RenderDeviceDX11);
	virtual	~MeshDX11();

	virtual void SetPrimitiveTopology(PrimitiveTopology _topology);

	virtual bool Render(const RenderEventArgs* renderArgs, const IConstantBuffer* perObject, SGeometryPartParams GeometryPartParams);
};